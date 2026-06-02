#ifndef _FILER_MAIN_HH
#define _FILER_MAIN_HH

#include <arc.hh>

namespace filer {
  inline arc::font material_filled_font;
  inline arc::font material_round_font;
  inline arc::font text_font;

  struct app : public arc::component {
    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;
  };
}

#endif
