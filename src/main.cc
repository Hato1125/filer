#include <filesystem>
#include <memory>

#include <arc.hh>

#include "main.hh"
#include "sidebar.hh"
#include "viewer.hh"
#include "navigations.hh"
#include "history.hh"
#include "actions.hh"

using namespace arc;

namespace filer {
  std::shared_ptr<view> app::build(arc::context& ctx) noexcept {
    if (filer::history::current.get().empty()) {
      filer::history::cd(std::getenv("HOME"));
    }

    return row({
      .children = {
        std::make_shared<filer::sidebar>(),
        column({
          .children = {
            row({
              .children = {
                std::make_shared<filer::navigations>(),
                spacer(),
                std::make_shared<filer::actions>(),
              },
            })
              | padding(8)
              | bg({ .color = colors::black }),
            std::make_shared<filer::viewer>(),
          }
        })
          | frame({
              .width = infinity,
              .height = infinity,
              .halign = halign::left,
              .valign = valign::top,
            })
      }
    });
  }
}

int main() {
  run({
    .id = "org.hato1125.filer",
    .name = "Filer",
    .assets = {
      {
        filer::material_filled_font,
        "assets/fonts/MaterialSymbolsFilled.ttf",
      },
      {
        filer::material_round_font,
        "assets/fonts/MaterialSymbolsRounded.ttf",
      },
      { filer::text_font, "Satoshi Variable" },
    },
    .scenes = {
      window({
        .root = std::make_unique<filer::app>(),
        .title = "Filer",
      }),
    }
  });
}
