#ifndef _FILER_THUMBNAIL_HH
#define _FILER_THUMBNAIL_HH

#include <memory>
#include <filesystem>

#include <arc.hh>

namespace filer::thumbnail {
  struct preview_state;

  class preview : public arc::component {
  public:
    explicit preview(std::filesystem::path path) noexcept;

    std::shared_ptr<arc::view> build(arc::context& ctx) noexcept override;

  private:
    std::filesystem::path _path;
    std::shared_ptr<preview_state> _state;
    arc::image _image;
  };
}

#endif
