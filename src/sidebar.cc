#include "sidebar.hh"
#include "direction.hh"
#include "main.hh"

using namespace arc;

namespace {
  constexpr std::string_view DESKTOP_ICON = "\ue30c";
  constexpr std::string_view DOWNLOAD_ICON = "\uf090";
  constexpr std::string_view DEVELOP_ICON = "\uf3c8";
  constexpr std::string_view DOCUMENTS_ICON = "\uea7d";
  constexpr std::string_view PICTURES_ICON = "\ue3f4";
  constexpr std::string_view VIDEOS_ICON = "\ue02c";
  constexpr std::string_view MUSICS_ICON = "\ue405";
}

namespace filer {
  sidebar::sidebar() noexcept {
    home = std::getenv("HOME");
  }

  std::shared_ptr<arc::view> sidebar::build() noexcept {
    return column({
      .children = {
        _button(DESKTOP_ICON, home / "Desktop"),
        _button(DOWNLOAD_ICON, home / "Downloads"),
        _button(DEVELOP_ICON, home / "Develop"),
        _button(DOCUMENTS_ICON, home / "Documents"),
        _button(PICTURES_ICON, home / "Pictures"),
        _button(VIDEOS_ICON, home / "Videos"),
        _button(MUSICS_ICON, home / "Musics"),
      }
    })
      | frame({
          .frame = {
            .width = 180,
            .height = infinity
          },
          .halign = halign::left,
          .valign = valign::top,
        })
      | padding(20, 24)
      | background({ .color = colors::black });
  }

  std::shared_ptr<arc::view> sidebar::_button(
    std::string_view icon,
    std::filesystem::path path
  ) noexcept {
    return row({
      .gap = 13,
      .children = {
        text({
          .label = icon,
          .font = &material_filled_font,
          .color = colors::white,
          .size = 16,
        }),
        text({
          .label = path.filename().string(),
          .font = &text_font,
          .color = colors::white,
          .size = 14,
        })
      }
    })
      | frame({
          .frame = {
            .width = infinity,
            .height = 35,
          },
          .halign = halign::left
        })
      | tap([path](mouse_button button, auto, auto) noexcept {
          if (button == mouse_button::left) {
            filer::path.set(path);
          }
        });
  }
}
