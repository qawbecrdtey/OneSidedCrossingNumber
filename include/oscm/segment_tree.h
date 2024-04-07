#ifndef ONESIDEDCROSSINGNUMBER_OSCM_SEGMENT_TREE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_SEGMENT_TREE_H

#include <cassert>
#include <cstdint>
#include <memory>

namespace oscm {
    class segment_tree {
    public:
        explicit segment_tree(std::size_t size_)
         : _tree(std::make_unique<std::uint64_t[]>(size_ << 2)),
           _size(size_) {}

        explicit segment_tree(std::size_t size_, std::uint64_t const *data_)
         : _tree(std::make_unique_for_overwrite<std::uint64_t[]>(size_ << 2)),
           _size(size_) {
            reset(data_);
        }

        void reset() const { std::fill(_tree.get(), _tree.get() + (_size << 2), 0); }

        void reset(std::uint64_t const *data_) const { reset(data_, 1, 1, _size); }

        void update(std::uint32_t idx_, std::uint64_t val_) const {
            update(1, idx_, val_, 1, _size);
        }

        void update_increment(std::uint32_t idx_, std::uint64_t val_) const {
            update_increment(1, idx_, val_, 1, _size);
        }

        void update_decrement(std::uint32_t idx_, std::uint64_t val_) const {
            update_decrement(1, idx_, val_, 1, _size);
        }

        [[nodiscard]]
        std::uint64_t sum(std::uint32_t lo_, std::uint32_t hi_) const {
            return sum(1, lo_, hi_, 1, _size);
        }

    private:
        std::unique_ptr<std::uint64_t[]> _tree;
        std::uint32_t const _size;

        void
        reset(std::uint64_t const *data_, std::uint32_t tidx_, std::uint32_t lo_, std::uint32_t hi_)
          const {
            if(lo_ == hi_) {
                _tree[tidx_] = data_[lo_];
                return;
            }
            std::uint32_t const m = (lo_ + hi_) >> 1;
            reset(data_, tidx_ << 1, lo_, m);
            reset(data_, (tidx_ << 1) | 1, m + 1, hi_);
            _tree[tidx_] = _tree[tidx_ << 1] + _tree[(tidx_ << 1) | 1];
        }

        void update(
          std::uint32_t tidx_,
          std::uint32_t idx_,
          std::uint64_t val_,
          std::uint32_t lo_,
          std::uint32_t hi_) const {
            if(idx_ < lo_ || hi_ < idx_) { return; }
            if(lo_ == hi_) {
                _tree[tidx_] = val_;
                return;
            }
            std::uint32_t const m = (lo_ + hi_) >> 1;
            update(tidx_ << 1, idx_, val_, lo_, m);
            update((tidx_ << 1) | 1, idx_, val_, m + 1, hi_);
            _tree[tidx_] = _tree[tidx_ << 1] + _tree[(tidx_ << 1) | 1];
        }

        void update_increment(
          std::uint32_t tidx_,
          std::uint32_t idx_,
          std::uint64_t val_,
          std::uint32_t lo_,
          std::uint32_t hi_) const {
            if(idx_ < lo_ || hi_ < idx_) { return; }
            _tree[tidx_] += val_;
            if(lo_ == hi_) { return; }
            std::uint32_t const m = (lo_ + hi_) >> 1;
            update_increment(tidx_ << 1, idx_, val_, lo_, m);
            update_increment((tidx_ << 1) | 1, idx_, val_, m + 1, hi_);
        }

        void update_decrement(
          std::uint32_t tidx_,
          std::uint32_t idx_,
          std::uint64_t val_,
          std::uint32_t lo_,
          std::uint32_t hi_) const {
            if(idx_ < lo_ || hi_ < idx_) { return; }
            assert(val_ <= _tree[tidx_]);
            _tree[tidx_] -= val_;
            if(lo_ == hi_) { return; }
            std::uint32_t const m = (lo_ + hi_) >> 1;
            update_decrement(tidx_ << 1, idx_, val_, lo_, m);
            update_decrement((tidx_ << 1) | 1, idx_, val_, m + 1, hi_);
        }

        [[nodiscard]]
        std::uint64_t sum(
          std::uint32_t tidx_,
          std::uint32_t start_,
          std::uint32_t end_,
          std::uint32_t lo_,
          std::uint32_t hi_) const {
            if(hi_ < start_ || end_ < lo_) { return 0; }
            if(start_ <= lo_ && hi_ <= end_) { return _tree[tidx_]; }
            std::uint32_t const m = (lo_ + hi_) >> 1;
            auto const x = sum(tidx_ << 1, start_, end_, lo_, m);
            auto const y = sum((tidx_ << 1) | 1, start_, end_, m + 1, hi_);
            return x + y;
        }
    };
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_SEGMENT_TREE_H
