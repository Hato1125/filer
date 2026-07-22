#ifndef _FILER_LIST_BROWSER_HH
#define _FILER_LIST_BROWSER_HH

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <arc.hh>

namespace filer {
  class list_browser : public arc::component {
  public:
    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;

  private:
    std::shared_ptr<arc::view> _entry(
      std::filesystem::path path,
      bool is_dir,
      std::string last_write_time
    ) const noexcept;

    std::vector<std::shared_ptr<arc::view>> sorted_entries(
      const std::filesystem::path& path
    ) const noexcept;

    std::string limitter(std::string_view str, std::size_t len) const noexcept;
  };
}

#endif
