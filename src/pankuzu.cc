#include "pankuzu.hh"
#include "history.hh"
#include "main.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<arc::view> pankuzu::build(arc::context& ctx) noexcept {
    std::vector<std::shared_ptr<view>> paths;

    std::filesystem::path now = history::current.get();
    std::filesystem::path root = now.root_directory();
    std::filesystem::path current = now;
    bool root_separator = false;

    do {
      if (root_separator) {
        paths.push_back(_separator());
      }
      root_separator = true;
      paths.push_back(_item(current, now));
      now = now.parent_path();
    } while (now != root);

    std::reverse(paths.begin(), paths.end());

    return row({
      .gap = 6,
      .align = valign::center,
      .children = std::move(paths),
    })
      | frame({
          .width = infinity,
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
    }) | padding(4, 5, 0, 0);
  }

  std::shared_ptr<arc::view> pankuzu::_item(
    std::filesystem::path current,
    std::filesystem::path path
  ) noexcept {
    const bool is_current = path == current;

    return text({
      .label = path.filename().string(),
      .font = &text_font,
      .weight = is_current
        ? font_weights::black
        : font_weights::semibold,
      .color = is_current
        ? colors::black
        : colors::white,
      .size = 12,
    })
      | padding(4, 9)
      | bg({
          .color = is_current
            ? color{255, 255, 255, 255}
            : color{255, 255, 255, 35},
          .round = 8.0f,
        })
      | tap([path](mouse_button button, auto, auto) noexcept {
          if (button == mouse_button::left) {
            history::cd(path);
          }
        });
  }
}
