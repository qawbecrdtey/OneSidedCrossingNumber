#include <oscm/bipartite_graph.h>
#include <oscm/count_crossings.h>
#include <oscm/segment_tree.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    std::uint32_t a[2] {4, 5};
    std::vector<std::pair<std::uint32_t, std::uint32_t>> edges {
      {1, 4},
      {2, 5},
      {3, 4}
    };
    std::sort(edges.begin(), edges.end(), [](auto&& a, auto&& b) {
        return a.second < b.second || (a.second == b.second && a.first < b.first);
    });
    std::cout << oscm::count_crossings(3, 2, a, edges) << '\n';
    std::swap(a[0], a[1]);
    std::cout << oscm::count_crossings(3, 2, a, edges) << std::flush;
}
