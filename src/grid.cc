#include "grid.hh"

namespace filer {
  grid_view::grid_view(
    std::vector<std::shared_ptr<arc::view>>&& items,
    float hgap,
    float vgap,
    arc::size item_size
  ) noexcept : _items(std::move(items)),
    _hgap(hgap),
    _vgap(vgap),
    _item_size(item_size),
    _cols(0),
    _rows(0) {}

  arc::size grid_view::measure(
    const arc::constraint& cont,
    const arc::canvas& canvas
  ) noexcept {
    compute_size = cont.max;
    _cols = std::max<std::size_t>(
      1,
      static_cast<std::size_t>(compute_size.width / (_item_size.width + _hgap))
    );
    _rows = _items.size() / _cols + (_items.size() % _cols > 0 ? 1 : 0);

    for (auto& item : _items) {
      item->measure({
        .min = _item_size,
        .max = _item_size,
      }, canvas);
    }
    return compute_size;
  }

  void grid_view::layout(
    const arc::point& offset,
    const arc::canvas& canvas
  ) noexcept {
    float x = offset.x;
    float y = offset.y;
    for (auto i = 0uz; i < _rows; ++i) {
      for (auto j = 0uz; j < _cols; ++j) {
        const auto index = i * _cols + j;
        if (index >= _items.size()) {
          break;
        }
        _items[index]->layout({x, y}, canvas);
        x += _item_size.width + _hgap;
      }
      x = offset.x;
      y += _item_size.height + _vgap;
    }

    compute_pos = offset;
  }

  void grid_view::paint(arc::canvas& canvas) noexcept {
    for (auto& item : _items) {
      item->paint(canvas);
    }
  }

  std::shared_ptr<grid_view> grid(grid_options options) noexcept {
    return std::make_shared<grid_view>(
      std::move(options.items),
      options.hgap,
      options.vgap,
      options.item_size
    );
  }
}
