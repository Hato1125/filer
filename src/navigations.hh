#ifndef _FILER_NAVIGATIONS_HH
#define _FILER_NAVIGATIONS_HH

#include <arc.hh>

namespace filer {
  class navigations : public arc::component {
  public:
    std::shared_ptr<arc::view> build() noexcept override;
  };
}

#endif
