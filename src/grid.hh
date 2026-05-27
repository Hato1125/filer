#ifndef _FILER_GRID_HH
#define _FILER_GRID_HH

#include <vector>
#include <memory>

#include <arc.hh>

namespace filer {
  class grid_view : public arc::view {
  public:
    grid_view(
      std::vector<std::shared_ptr<arc::view>>&& items,
      float hgap,
      float vgap,
      arc::size item_size
    ) noexcept;

    arc::size measure(
      const arc::constraint& cont,
      const arc::canvas& canvas
    ) noexcept override;

    void layout(
      const arc::point& offset,
      const arc::canvas& canvas
    ) noexcept override;

    void paint(arc::canvas& canvas) noexcept override;

  private:
    std::vector<std::shared_ptr<arc::view>> _items;
    float _hgap;
    float _vgap;
    arc::size _item_size;

    std::size_t _cols;
    std::size_t _rows;
  };

  struct grid_options {
    arc::required<std::vector<std::shared_ptr<arc::view>>> items;
    float hgap = 0.0f;
    float vgap = 0.0f;
    arc::size item_size;
  };

  std::shared_ptr<grid_view> grid(grid_options options) noexcept;
}

#endif
