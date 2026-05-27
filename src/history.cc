#include <stack>

#include "history.hh"

namespace {
  std::stack<std::filesystem::path> backs;
  std::stack<std::filesystem::path> forwards;

  void update_navigation_state() noexcept {
    filer::history::backable.set(!backs.empty());
    filer::history::forwardable.set(!forwards.empty());
  }
}

namespace filer::history {
  void back() noexcept {
    if (backs.empty()) {
      return;
    }

    auto prev = backs.top();
    backs.pop();

    forwards.push(current.get());
    update_navigation_state();
    current.set(prev);
  }

  void forward() noexcept {
    if (forwards.empty()) {
      return;
    }

    auto next = forwards.top();
    forwards.pop();

    backs.push(current.get());
    update_navigation_state();
    current.set(next);
  }

  void cd(const std::filesystem::path& path) noexcept {
    const auto now = current.get();
    if (now == path) {
      return;
    }

    if (!now.empty()) {
      backs.push(now);
    }

    current.set(path);
    while (!forwards.empty()) forwards.pop();
    update_navigation_state();
  }

  bool is_backable() noexcept {
    return backable.get();
  }

  bool is_forwardable() noexcept {
    return forwardable.get();
  }
}
