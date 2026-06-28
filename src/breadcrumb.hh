#ifndef _FILER_BREADCRUMB_HH
#define _FILER_BREADCRUMB_HH

#include <filesystem>

#include <arc.hh>

namespace filer {
  class breadcrumb : public arc::component {
  public:
    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;

  private:
    std::shared_ptr<arc::view> _separator() noexcept;

    std::shared_ptr<arc::view> _item(
      std::filesystem::path current,
      std::filesystem::path path
    ) noexcept;
  };
}

#endif
