#include <print>
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>

#include <md5.h>

#include "thumbnail.hh"

namespace filer::thumbnail {
  namespace fs = std::filesystem;

  namespace {
    bool is_uri_unreserved(unsigned char c) noexcept {
      return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z')
        || (c >= '0' && c <= '9')
        || c == '-'
        || c == '.'
        || c == '_'
        || c == '~';
    }

    std::string percent_encode_uri_path(std::string_view path) {
      static constexpr char HEX[] = "0123456789ABCDEF";

      std::string encoded;
      encoded.reserve(path.size());

      for (const unsigned char c : path) {
        if (is_uri_unreserved(c) || c == '/') {
          encoded.push_back(static_cast<char>(c));
          continue;
        }

        encoded.push_back('%');
        encoded.push_back(HEX[c >> 4]);
        encoded.push_back(HEX[c & 0x0f]);
      }

      return encoded;
    }

    std::string file_uri(const fs::path& path) {
      std::error_code error;
      const auto absolute_path = path.is_absolute()
        ? path
        : fs::absolute(path, error);

      const auto& uri_path = error ? path : absolute_path;
      return "file://" + percent_encode_uri_path(uri_path.native());
    }

    std::optional<fs::path> xdg_cache_dir() noexcept {
      const char* cache_home = std::getenv("XDG_CACHE_HOME");
      if (cache_home && *cache_home) {
        return fs::path(cache_home);
      }

      const char* home = std::getenv("HOME");
      if (home && *home) {
        return fs::path(home) / ".cache";
      }

      return std::nullopt;
    }

    bool is_directly_loadable_image(const fs::path& path) {
      auto extension = path.extension().string();
      std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char c) -> char {
          return static_cast<char>(std::tolower(c));
        }
      );

      static constexpr std::array<std::string_view, 5> extensions = {
        ".png",
        ".jpg",
        ".jpeg",
        ".bmp",
        ".gif",
      };

      return std::ranges::find(extensions, extension) != extensions.end();
    }

    std::optional<
      std::reference_wrapper<arc::image>
    > load_image(arc::canvas& canvas, const fs::path& path) {
      if (thumbnails.contains(path)) {
        return thumbnails.at(path);
      }

      arc::image image;
      if (!image.load(canvas, path.string())) {
        return std::nullopt;
      }

      thumbnails.emplace(path, std::move(image));
      return thumbnails.at(path);
    }
  }

  std::optional<
    std::reference_wrapper<arc::image>
  > get_thumbnail(
    arc::canvas& canvas,
    const fs::path& path
  ) noexcept {
    static MD5 md5;
    static constexpr std::array<std::string_view, 4> sizes = {
      "xx-large",
      "x-large",
      "large",
      "normal",
    };

    const auto canonical_path = file_uri(path);
    const auto md5ed_path = md5(
      canonical_path.data(),
      canonical_path.size()
    );

    if (const auto cache_dir = xdg_cache_dir()) {
      for (const auto dir : sizes) {
        const auto thumbnail_path = *cache_dir
          / "thumbnails"
          / dir
          / std::format("{}.png", md5ed_path);

        std::println("{}", thumbnail_path);

        if (fs::exists(thumbnail_path)) {
          if (thumbnails.contains(thumbnail_path)) {
            return thumbnails.at(thumbnail_path);
          }

          arc::image thumbnail;
          if (thumbnail.load(canvas, thumbnail_path.string())) {
            thumbnails.emplace(thumbnail_path, std::move(thumbnail));
            return thumbnails.at(thumbnail_path);
          }
        }
      }
    }

    if (is_directly_loadable_image(path)) {
      if (auto image = load_image(canvas, path)) {
        return image;
      }
    }

    return std::nullopt;
  }
}
