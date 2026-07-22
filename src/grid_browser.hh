#ifndef _FILER_GRID_BROWSER_HH
#define _FILER_GRID_BROWSER_HH

#include <filesystem>
#include <map>
#include <memory>

#include <arc.hh>

#include "thumbnail.hh"

namespace filer {
  class grid_browser : public arc::component {
  public:
    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;

  private:
    std::shared_ptr<arc::view> _dir(
      std::filesystem::path path
    ) const noexcept;

    std::shared_ptr<arc::view> _file(
      std::filesystem::path path
    ) noexcept;

    std::vector<std::shared_ptr<view>> sorted_dirs(
      const std::filesystem::path& path
    ) noexcept;

    std::string limitter(std::string_view str, std::size_t len) const noexcept;

    std::map<std::filesystem::path, std::shared_ptr<thumbnail::preview>> _previews;
  };
}

#endif
