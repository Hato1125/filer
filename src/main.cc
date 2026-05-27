#include <arc.hh>
#include <filesystem>

#include "main.hh"
#include "sidebar.hh"
#include "viewer.hh"
#include "navigations.hh"
#include "pankuzu.hh"
#include "history.hh"

using namespace arc;

struct app : public component {
  std::shared_ptr<view> build() noexcept override {
    if (filer::history::current.get().empty()) {
      filer::history::cd("/home/hato");
    }

    return row({
      .children = {
        std::make_shared<filer::sidebar>(),
        column({
          .children = {
            row({
              .gap = 18,
              .children = {
                std::make_shared<filer::navigations>(),
                std::make_shared<filer::pankuzu>(),
              }
            })
              | padding(20)
              | frame({
                  .frame={
                    .width = infinity,
                    .height = 45,
                  }
                }),
            std::make_shared<filer::viewer>()
              | frame({
                  .frame= {
                    .width = infinity,
                    .height = infinity,
                  }
                }),
          }
        })
          | frame({
              .frame={
                .width = infinity,
                .height = infinity,
              }
            })
          | background({ .color = colors::black }),
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
        "/usr/share/fonts/One-UI-Sans-Font/Fonts/(Bold) Samsung Sharp Sans.ttf",
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
