#include "actions.hh"
#include "main.hh"
#include "utils/color.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<arc::view> actions::build(arc::context& ctx) noexcept {
    return row({
      .align = valign::center,
      .children = {
        text({
          .label = "\ue0ee",
          .font = &material_filled_font,
          .color = colors::white,
          .size = 17,
        })
          | frame({ .width = 35, .height = 35 })
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                // TODO
              }
            }),
        box({.size = {1, 20}, .color = color{255, 255, 255, 100}, .round = 0}),
        text({
          .label = "\ue5d3",
          .font = &material_filled_font,
          .color = colors::white,
          .size = 17,
        })
          | frame({ .width = 35, .height = 35 })
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                // TODO
              }
            }),
        box({.size = {1, 20}, .color = color{255, 255, 255, 100}, .round = 0}),
        text({
          .label = "\ue8b6",
          .font = &material_filled_font,
          .color = colors::white,
          .size = 17,
        })
          | frame({ .width = 35, .height = 35 })
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                // TODO
              }
            })
      }
    }) | bg({.color = colors::stone_800, .round = 17.5f});
  }

  std::shared_ptr<arc::view> actions::separator() noexcept {
    return text({
      .label = "|",
      .font = &text_font,
      .weight = font_weights::semibold,
      .color = color{255, 255, 255, 100},
      .size = 20,
    })
      | frame({ .width = 3, .height = 30 });
  }
}
