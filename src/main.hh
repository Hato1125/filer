#ifndef _FILER_MAIN_HH
#define _FILER_MAIN_HH

#include <arc.hh>

namespace filer {
  inline arc::font_family material_filled_font;
  inline arc::font_family material_round_font;
  inline arc::font_family text_font;

  struct app : public arc::component {
    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;
  };
}

#endif
