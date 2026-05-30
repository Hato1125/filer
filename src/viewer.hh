#ifndef _FILER_VIEWER_HH
#define _FILER_VIEWER_HH

#include <filesystem>

#include <arc.hh>

namespace filer {
  class viewer : public arc::component {
  public:
    std::shared_ptr<arc::view> build() noexcept override;

  private:
    std::shared_ptr<arc::view> _dir(
      std::filesystem::path path
    ) const noexcept;

    std::shared_ptr<arc::view> _file(
      std::filesystem::path path
    ) const noexcept;

    std::vector<std::shared_ptr<view>> sorted_dirs(
      const std::filesystem::path& path
    ) const noexcept;

    std::string limitter(std::string_view str, std::size_t len) const noexcept;
  };
}

#endif
