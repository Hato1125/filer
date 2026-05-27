#include "navigations.hh"
#include "history.hh"
#include "main.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<arc::view> navigations::build() noexcept {
    return row({
      .align = valign::center,
      .children = {
        text({
          .label = "\ue5e0",
          .font = &material_filled_font,
          .color = history::backable.get()
            ? colors::white
            : color{255, 255, 255, 120},
          .size = 20,
        })
          | frame({ .frame = {40, 40}})
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                history::back();
              }
            }),
        text({
          .label = "\ue5e1",
          .font = &material_filled_font,
          .color = history::forwardable.get()
            ? colors::white
            : color{255, 255, 255, 120},
          .size = 20,
        })
          | frame({ .frame = {40, 40}})
          | tap([](mouse_button button, auto, auto) noexcept {
              if (button == mouse_button::left) {
                history::forward();
              }
            })
      }
    })
      | frame({
          .frame = {
            .width = 80,
            .height = infinity,
          },
          .halign = halign::left,
          .valign = valign::center,
        });
  }
}
