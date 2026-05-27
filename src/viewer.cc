#include "viewer.hh"
#include "grid.hh"
#include "history.hh"
#include "main.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<arc::view> viewer::build() noexcept {
    std::vector<std::shared_ptr<view>> entrys;

    const auto cpath = history::current.get();

    for (const auto& entry : std::filesystem::directory_iterator(cpath)) {
      entrys.push_back(
        entry.is_directory()
          ? _dir(entry.path())
          : _file(entry.path())
      );
    }

    return grid({
      .items = std::move(entrys),
      .hgap = 8,
      .vgap = 8,
      .item_size = {95, 100}
    }) | background({ .color = colors::black });
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
          .color = colors::white,
          .size = 13,
        })
      }
    })
      | frame({
          .frame = {
            .width = infinity,
            .height = infinity
          }
        })
      | tap([path](mouse_button button, auto, auto) noexcept {
          if (button == mouse_button::left) {
            history::cd(path);
          }
        });
  }

  std::shared_ptr<arc::view> viewer::_file(
    std::filesystem::path path
  ) const noexcept {
    return column({
      .gap = 10,
      .align = halign::center,
      .children = {
        text({
          .label = "\uea7d",
          .font = &material_filled_font,
          .color = colors::white,
          .size = 48,
        }),
        text({
          .label = limitter(path.filename().string(), 12),
          .font = &text_font,
          .color = colors::white,
          .size = 13,
        })
      }
    })
      | frame({
          .frame = {
            .width = infinity,
            .height = infinity
          }
        });
  }

  std::string viewer::limitter(std::string_view str, std::size_t len) const noexcept {
    return str.size() >= len
      ? str.substr(0, len - 3) + std::string("...")
      : std::string(str);
  }
}
