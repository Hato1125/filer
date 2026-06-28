#include <algorithm>
#include <cctype>
#include <set>
#include <vector>

#include "viewer.hh"
#include "grid.hh"
#include "history.hh"
#include "main.hh"
#include "thumbnail.hh"

using namespace arc;
namespace fs = std::filesystem;

namespace filer {
  std::shared_ptr<arc::view> viewer::build(arc::context& ctx) noexcept {
    return grid({
      .items = sorted_dirs(history::current.get()),
      .hgap = 8,
      .vgap = 8,
      .item_size = {95, 100}
    })
      | scroll({ .offset = 24 })
      | bg({ .color = colors::black });
  }

  std::shared_ptr<arc::view> viewer::_dir(
    std::filesystem::path path
  ) const noexcept {
    return column({
      .gap = 10,
      .align = halign::center,
      .children = {
        text({
          .label = "\ue2c7",
          .font = &material_filled_font,
          .color = colors::white,
          .size = 48,
        }),
        text({
          .label = limitter(path.filename().string(), 12),
          .font = &text_font,
          .weight = font_weights::semibold,
          .color = colors::white,
          .size = 13,
        })
      }
    })
      | frame({
          .width = infinity,
          .height = infinity
        })
      | tap([path](mouse_button button, auto, auto) noexcept {
          if (button == mouse_button::left) {
            history::cd(path);
          }
        });
  }

  std::shared_ptr<arc::view> viewer::_file(
    std::filesystem::path path
  ) noexcept {
    auto [preview, inserted] = _previews.try_emplace(path);
    if (inserted) {
      preview->second = std::make_shared<thumbnail::preview>(path);
    }

    return column({
      .gap = 10,
      .align = halign::center,
      .children = {
        preview->second,
        text({
          .label = limitter(path.filename().string(), 12),
          .font = &text_font,
          .weight = font_weights::semibold,
          .color = colors::white,
          .size = 13,
        })
      }
    })
      | frame({
          .width = infinity,
          .height = infinity
        });
  }

  std::vector<std::shared_ptr<view>> viewer::sorted_dirs(
    const std::filesystem::path& path
  ) noexcept {
    std::vector<std::shared_ptr<view>> entrys;
    std::set<fs::path> visible_files;

    auto dirs = std::vector<fs::directory_entry>(
      fs::directory_iterator(path),
      fs::directory_iterator()
    );

    const auto sort_key = [](const fs::directory_entry& entry) -> std::string {
      auto name = entry.path().filename().string();

      std::transform(
        name.begin(),
        name.end(),
        name.begin(),
        [](unsigned char c) -> char {
          return static_cast<char>(std::tolower(c));
        }
      );

      return name;
    };

    std::sort(
      dirs.begin(),
      dirs.end(),
      [&sort_key](const fs::directory_entry& a, const fs::directory_entry& b) -> bool {
        if (a.is_directory() != b.is_directory()) {
          return a.is_directory();
        }

        const auto a_key = sort_key(a);
        const auto b_key = sort_key(b);

        return a_key != b_key
          ? a_key < b_key
          : a.path().filename().string() < b.path().filename().string();
      }
    );

    for (const auto& entry : dirs) {
      if (!entry.is_directory()) {
        visible_files.insert(entry.path());
      }

      entrys.push_back(
        entry.is_directory()
          ? _dir(entry)
          : _file(entry)
      );
    }

    for (auto it = _previews.begin(); it != _previews.end();) {
      if (visible_files.contains(it->first)) {
        ++it;
      } else {
        it = _previews.erase(it);
      }
    }

    return entrys;
  }

  std::string viewer::limitter(std::string_view str, std::size_t len) const noexcept {
    return str.size() >= len
      ? str.substr(0, len - 3) + std::string("...")
      : std::string(str);
  }
}
