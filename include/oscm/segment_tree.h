#ifndef ONESIDEDCROSSINGNUMBER_OSCM_SEGMENT_TREE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_SEGMENT_TREE_H

#include <cstdint>
#include <memory>

namespace oscm {
    class segment_tree {
    public:
        explicit segment_tree(std::size_t size_);
        explicit segment_tree(std::size_t size_, std::uint64_t const *data_);

        void reset() const;
        void reset(std::uint64_t const *data_) const;

        void update(std::uint32_t idx_, std::uint64_t val_) const;
        void update_increment(std::uint32_t idx_, std::uint64_t val_) const;
        void update_decrement(std::uint32_t idx_, std::uint64_t val_) const;

    private:
        std::unique_ptr<std::uint64_t[]> _tree;
        std::uint32_t const _size;

        void
        reset(std::uint64_t const *data_, std::uint32_t tidx_, std::uint32_t lo_, std::uint32_t hi_)
          const;

        void update(
          std::uint32_t tidx_,
          std::uint32_t idx_,
          std::uint64_t val_,
          std::uint32_t lo_,
          std::uint32_t hi_) const;

        void update_increment(
          std::uint32_t tidx_,
          std::uint32_t idx_,
          std::uint64_t val_,
          std::uint32_t lo_,
          std::uint32_t hi_) const;

        void update_decrement(
          std::uint32_t tidx_,
          std::uint32_t idx_,
          std::uint64_t val_,
          std::uint32_t lo_,
          std::uint32_t hi_) const;
    };
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_SEGMENT_TREE_H
