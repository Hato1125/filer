#ifndef _FILER_SIDEBAR_HH
#define _FILER_SIDEBAR_HH

#include <string_view>
#include <filesystem>

#include <arc.hh>

namespace filer {
  class sidebar : public arc::component {
  public:
    sidebar() noexcept;

    std::shared_ptr<arc::view> build() noexcept override;

  private:
    std::filesystem::path home;

    std::shared_ptr<arc::view> _button(
      std::string_view icon,
      std::filesystem::path path
    ) noexcept;
  };
}

#endif
