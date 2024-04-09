#include <oscm/bipartite_graph.h>
#include <oscm/count_crossings.h>
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
    read_file(argv[1], nA, nB, edges);
}