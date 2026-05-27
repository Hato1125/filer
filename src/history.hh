#ifndef _FILER_HISTORY_HH
#define _FILER_HISTORY_HH

#include <filesystem>

#include <arc.hh>

namespace filer::history {
  inline arc::state<std::filesystem::path> current;
  inline arc::state<bool> backable;
  inline arc::state<bool> forwardable;

  void back() noexcept;
  void forward() noexcept;
  void cd(const std::filesystem::path& path) noexcept;
}

#endif
