#ifndef _FILER_ACTIONS_HH
#define _FILER_ACTIONS_HH

#include <arc.hh>

namespace filer {
  class actions : public arc::component {
  public:
    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;

  private:
    std::shared_ptr<arc::view> separator() noexcept;
  };
}

#endif
