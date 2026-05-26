#include <arc.hh>

#include "main.hh"
#include "sidebar.hh"

using namespace arc;

struct app : public component {
  std::shared_ptr<view> build() noexcept override {
    return row({
      .children = {
        std::make_shared<filer::sidebar>(),
      }
    });
  }
};

int main() {
  run({
    .id = "org.hato1125.filer",
    .name = "Filer",
    .version = "0.1.0",
    .assets = {
      {
        filer::material_filled_font,
        "assets/fonts/MaterialSymbolsFilled.ttf",
      },
      {
        filer::material_round_font,
        "assets/fonts/MaterialSymbolsRounded.ttf",
      },
      {
        filer::text_font,
        "/usr/share/fonts/One-UI-Sans-Font/Fonts/(Medium) Samsung Sharp Sans.ttf",
      }
    },
    .scenes = {
      window({
        .root = std::make_unique<app>(),
        .title = "Filer",
      })
    }
  });
}
