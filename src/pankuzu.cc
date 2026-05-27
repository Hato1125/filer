#include "pankuzu.hh"
#include "history.hh"
#include "main.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<arc::view> pankuzu::build() noexcept {
    std::vector<std::shared_ptr<view>> paths;

    std::filesystem::path now = history::current.get();
    std::filesystem::path root = now.root_directory();
    bool root_separator = false;

    do {
      if (root_separator) {
        paths.push_back(_separator());
      }
      root_separator = true;
      paths.push_back(_item(now));
      now = now.parent_path();
    } while (now != root);

    std::reverse(paths.begin(), paths.end());

    return row({
      .gap = 8,
      .align = valign::center,
      .children = std::move(paths),
    })
      | frame({
          .frame = {
            .width = infinity,
            .height = infinity,
          },
          .halign = halign::left,
          .valign = valign::center,
        });
  }

  std::shared_ptr<arc::view> pankuzu::_separator() noexcept {
    return text({
      .label = "\ue5df",
      .font = &material_filled_font,
      .color = colors::white,
      .size = 24,
    });
  }

  std::shared_ptr<arc::view> pankuzu::_item(
    std::filesystem::path path
  ) noexcept {
    return text({
      .label = path.filename().string(),
      .font = &text_font,
      .color = colors::white,
      .size = 13,
    })
      | padding(5, 8)
      | background({
          .color = color{255, 255, 255, 35},
          .round = 9,
        })
      | tap([path](mouse_button button, auto, auto) noexcept {
          if (button == mouse_button::left) {
            history::cd(path);
          }
        });
  }
}
