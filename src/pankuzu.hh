#ifndef _FILER_PANKUZU_HH
#define _FILER_PANKUZU_HH

#include <filesystem>

#include <arc.hh>

namespace filer {
  class pankuzu : public arc::component {
  public:
    std::shared_ptr<arc::view> build() noexcept override;

  private:
    std::shared_ptr<arc::view> _separator() noexcept;

    std::shared_ptr<arc::view> _item(
      std::filesystem::path path
    ) noexcept;
  };
}

#endif
