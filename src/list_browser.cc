#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <system_error>
#include <vector>

#include "history.hh"
#include "list_browser.hh"
#include "main.hh"

using namespace arc;
namespace fs = std::filesystem;

namespace filer {
  namespace {
    struct entry_info {
      fs::path path;
      std::string name;
      std::string sort_key;
      std::string last_write_time;
      bool is_dir = false;
    };

    std::string make_sort_key(std::string name) noexcept {
      std::transform(
        name.begin(),
        name.end(),
        name.begin(),
        [](unsigned char c) -> char {
          return static_cast<char>(std::tolower(c));
        }
      );

      return name;
    }

    bool is_directory(const fs::directory_entry& entry) noexcept {
      std::error_code ec;
      return entry.is_directory(ec) && !ec;
    }

    std::string format_last_write_time(const fs::directory_entry& entry) noexcept {
      std::error_code ec;
      const auto tp = entry.last_write_time(ec);
      if (ec) {
        return {};
      }

      namespace chrono = std::chrono;
      const auto system_tp = chrono::time_point_cast<chrono::system_clock::duration>(
        tp - fs::file_time_type::clock::now() + chrono::system_clock::now()
      );

      const std::time_t t = chrono::system_clock::to_time_t(system_tp);
      const auto* lt = std::localtime(&t);
      if (!lt) {
        return "-";
      }

      std::ostringstream formatted_time;
      formatted_time << std::put_time(lt, "%c");
      return formatted_time.str();
    }
  }

  std::shared_ptr<arc::view> list_browser::build(arc::context& ctx) noexcept {
    return column({
      .children = sorted_entries(history::current.get()),
    })
      | scroll({ .offset = 24 })
      | bg({ .color = colors::black });
  }

  std::shared_ptr<arc::view> list_browser::_entry(
    std::filesystem::path path,
    bool is_dir,
    std::string last_write_time
  ) const noexcept {
    return row({
      .gap = 12,
      .align = valign::center,
      .children = {
        text({
          .label = is_dir ? "\ue2c7" : "\ue873",
          .font = &material_filled_font,
          .color = colors::white,
          .size = 20,
        }) | frame({ .width = 28, .height = 36 }),
        text({
          .label = limitter(path.filename().string(), 44),
          .font = &text_font,
          .weight = font_weights::semibold,
          .color = colors::white,
          .size = 14,
        }),
        spacer(),
        text({
          .label = last_write_time,
          .font = &text_font,
          .weight = font_weights::semibold,
          .color = colors::zinc_500,
          .size = 12,
        }),
      },
    })
      | frame({
          .width = infinity,
          .height = 40,
          .halign = halign::left,
        })
      | padding(0, 12)
      | tap([path, is_dir](mouse_button button, auto, auto) noexcept {
          if (button == mouse_button::left && is_dir) {
            history::cd(path);
          }
        });
  }

  std::vector<std::shared_ptr<arc::view>> list_browser::sorted_entries(
    const std::filesystem::path& path
  ) const noexcept {
    std::vector<std::shared_ptr<arc::view>> entries;
    std::vector<entry_info> dirs;

    std::error_code ec;
    auto it = fs::directory_iterator(
      path,
      fs::directory_options::skip_permission_denied,
      ec
    );
    if (ec) {
      return entries;
    }

    for (const auto end = fs::directory_iterator(); it != end; it.increment(ec)) {
      if (ec) {
        ec.clear();
        continue;
      }

      auto entry_path = it->path();
      auto name = entry_path.filename().string();
      dirs.push_back({
        .path = std::move(entry_path),
        .name = name,
        .sort_key = make_sort_key(std::move(name)),
        .last_write_time = format_last_write_time(*it),
        .is_dir = is_directory(*it),
      });
    }

    std::sort(
      dirs.begin(),
      dirs.end(),
      [](const entry_info& a, const entry_info& b) -> bool {
        if (a.is_dir != b.is_dir) {
          return a.is_dir;
        }

        return a.sort_key != b.sort_key
          ? a.sort_key < b.sort_key
          : a.name < b.name;
      }
    );

    entries.reserve(dirs.size());
    for (auto& entry : dirs) {
      if (entry.last_write_time.empty()) {
        entry.last_write_time = "-";
      }

      entries.push_back(_entry(
        std::move(entry.path),
        entry.is_dir,
        std::move(entry.last_write_time)
      ));
    }

    return entries;
  }

  std::string list_browser::limitter(std::string_view str, std::size_t len) const noexcept {
    return str.size() >= len
      ? str.substr(0, len - 3) + std::string("...")
      : std::string(str);
  }
}
