#include <oscm/bipartite_graph.h>
#include <oscm/compute_ordering.h>
#include <oscm/count_crossings.h>
#include <oscm/write_to_file.h>
#include <oscm/read_file.h>
#include <oscm/segment_tree.h>

#include <cstdint>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    // [[assume(argc > 0)]];
    if(argc != 3) {
        std::cerr << "usage: " << argv[0] << " [path/to/input] [path/to/output]" << std::endl;
        return 1;
    }

    std::uint32_t nA;
    std::uint32_t nB;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> edges;
    oscm::read_from_file(argv[1], nA, nB, edges);

    std::vector<std::uint32_t> ordering(nB);
    std::iota(ordering.begin(), ordering.end(), nB + 1);
    std::uint64_t crossing_upper_bound = oscm::count_crossings(nA, nB, ordering.data(), edges);
    oscm::compute_ordering(nA, nB, edges, ordering, crossing_upper_bound);

    oscm::write_to_file(argv[2], ordering);
}