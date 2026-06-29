#include "navigations.hh"
#include "history.hh"
#include "main.hh"
#include "utils/color.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<arc::view> navigations::build(arc::context& ctx) noexcept {
    return row({
      .align = valign::center,
      .children = {
        text({
          .label = "\ue5e0",
          .font = &material_filled_font,
          .color = history::backable.get()
            ? colors::white
            : colors::zinc_500,
          .size = 15,
        })
          | frame({ .width = 37, .height = 35 })
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                history::back();
              }
            }),
        box({.size = {1, 20}, .color = colors::zinc_500, .round = 0}),
        text({
          .label = "\ue5e1",
          .font = &material_filled_font,
          .color = history::forwardable.get()
            ? colors::white
            : colors::zinc_500,
          .size = 15,
        })
          | frame({ .width = 37, .height = 35 })
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                history::forward();
              }
            })
      }
    }) | bg({.color = colors::zinc_800, .round = 17.5f});
  }
}
