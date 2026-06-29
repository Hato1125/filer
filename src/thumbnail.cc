#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <format>
#include <memory>
#include <optional>
#include <utility>

#include <md5.h>

#include "main.hh"
#include "thumbnail.hh"

namespace filer::thumbnail {
  namespace fs = std::filesystem;

  struct preview_state {
    arc::state<std::shared_ptr<arc::image::raw>> raw;
    bool loading = false;
    bool failed = false;
  };

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

    std::optional<fs::path> find_thumbnail_source(const fs::path& path) noexcept {
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

          if (fs::exists(thumbnail_path)) {
            return thumbnail_path;
          }
        }
      }

      if (is_directly_loadable_image(path)) {
        return path;
      }

      return std::nullopt;
    }

    std::shared_ptr<arc::image::raw> retain_raw(arc::image::raw&& raw) {
      return {
        new arc::image::raw(std::move(raw)),
        [](arc::image::raw* raw) {
          raw->free();
          delete raw;
        }
      };
    }

    void request_raw_load(
      arc::context& ctx,
      fs::path source,
      std::weak_ptr<preview_state> state
    ) noexcept {
      ctx.spawn([
        &ctx,
        source = std::move(source),
        state = std::move(state)
      ]() mutable {
        auto raw = arc::image::raw::decode(source.string());
        auto decoded = raw
          ? retain_raw(std::move(*raw))
          : nullptr;

        ctx.post([
          decoded = std::move(decoded),
          state = std::move(state)
        ]() mutable {
          auto locked = state.lock();
          if (!locked) {
            return;
          }

          if (!decoded) {
            locked->loading = false;
            locked->failed = true;
            return;
          }

          locked->loading = false;
          locked->raw.set(std::move(decoded));
        });
      });
    }
  }

  preview::preview(fs::path path) noexcept
    : _path(std::move(path)),
      _state(std::make_shared<preview_state>()) {}

  std::shared_ptr<arc::view> preview::build(arc::context& ctx) noexcept {
    if (_image) {
      return arc::img({
        .src = &_image,
        .size = {95, 75},
        .fit = arc::fit::contain
      });
    }

    const auto raw = _state->raw.get();
    if (!_state->failed && raw) {
      if (_image.load(ctx.canvas(), *raw)) {
        return arc::img({
          .src = &_image,
          .size = {95, 75},
          .fit = arc::fit::contain
        });
      }

      _state->failed = true;
    }

    if (!_state->loading && !_state->failed) {
      if (const auto source = find_thumbnail_source(_path)) {
        _state->loading = true;
        request_raw_load(ctx, *source, _state);
      } else {
        _state->failed = true;
      }
    }

    return arc::text({
      .label = "\uea7d",
      .font = &filer::material_filled_font,
      .color = arc::colors::white,
      .size = 48,
    });
  }
}
