#include <oscm/bipartite_graph.h>
#include <oscm/compute_ordering.h>
#include <oscm/compute_ordering_primary.h>
#include <oscm/count_crossings.h>
#include <oscm/read_from_file.h>
#include <oscm/segment_tree.h>
#include <oscm/write_to_file.h>

#include <cstdint>
#include <iostream>
#include <vector>

#if __cplusplus < 202'002L
# error "This should be compiled with at least c++20."
#endif

int main(int argc, char *argv[]) {
#if __has_cpp_attribute(assume)
    [[assume(argc > 0)]];
#endif
    if(argc != 3) {
        std::cerr << "usage: " << argv[0] << " [path/to/input] [path/to/output]" << std::endl;
        return 1;
    }
#if __has_cpp_attribute(assume)
    [[assume(argv[0])]];
    [[assume(argv[1])]];
    [[assume(argv[2])]];
    [[assume(!argv[3])]];
#endif

    std::uint32_t nA;
    std::uint32_t nB;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> connections;
    oscm::read_from_file(argv[1], nA, nB, connections);
    assert(nA > 0 && nB > 0);
#if __has_cpp_attribute(assume)
    [[assume(nA > 0)]];
    [[assume(nB > 0)]];
#endif

    std::cout << "connections:\n";
    for(auto [u, v]: connections) { std::cout << u << ' ' << v << '\n'; }
    std::cout << std::endl;

    std::vector<std::uint32_t> ordering(nB);
    std::iota(ordering.begin(), ordering.end(), nA);
    std::uint64_t crossing_upper_bound = oscm::count_crossings(nA, nB, ordering.data(), connections);

    std::cout << "crossing_upper_bound: " << crossing_upper_bound << std::endl;

    oscm::compute_ordering(nA, nB, connections, ordering, crossing_upper_bound);
    // oscm::compute_ordering_primary(nA, nB, connections, ordering, crossing_upper_bound);

    std::cout
      << "current crossing count: " << oscm::count_crossings(nA, nB, ordering.data(), connections)
      << '\n';

    oscm::write_to_file(argv[2], ordering);
}

// References:
// [1] Fixed parameter algorithms for ONE-SIDED CROSSING MINIMIZATION revisited