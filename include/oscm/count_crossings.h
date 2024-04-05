#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H

#include <boost/graph/graph_traits.hpp>

#include <cstdint>

namespace oscm {
    template<typename Graph>
    std::uint64_t count_crossings(Graph const &graph_);
}

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H
