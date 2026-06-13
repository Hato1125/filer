#ifndef _FILER_THUMBNAIL_HH
#define _FILER_THUMBNAIL_HH

#include <map>
#include <optional>
#include <filesystem>

#include <arc.hh>

namespace filer::thumbnail {
  inline std::map<std::filesystem::path, arc::image> thumbnails;

  std::optional<
    std::reference_wrapper<arc::image>
  > get_thumbnail(
    arc::canvas& canvas,
    const std::filesystem::path& path
  ) noexcept;
}

#endif
