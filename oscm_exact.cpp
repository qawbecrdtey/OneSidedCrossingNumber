#include <oscm/compute_ordering.h>
#include <oscm/count_crossings.h>
#include <oscm/read_from_file.h>
#include <oscm/write_to_file.h>

#include <chrono>
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

#if DEBUG_MESSAGE

    std::cout << "connections:\n";
    for(auto const &[u, v]: connections) { std::cout << u << ' ' << v << '\n'; }
    std::cout << std::endl;

#endif

    std::vector<std::uint32_t> ordering(nB);
    std::iota(ordering.begin(), ordering.end(), nA);
    std::uint64_t crossing_upper_bound = oscm::count_crossings(nA, nB, ordering.data(), connections);

    // Thanks, constraint!
    if(crossing_upper_bound > 300 * (nA + nB)) { crossing_upper_bound = 300 * (nA + nB); }

#if DEBUG_MESSAGE

    std::cout << "crossing_upper_bound: " << crossing_upper_bound << std::endl;

    std::cout << "ordering prev:\n";
    for(auto const now: ordering) { std::cout << now << ' '; }
    std::cout << std::endl;

#endif

#if ENABLE_TIMER
    auto const start = std::chrono::system_clock::now();
#endif

    oscm::compute_ordering(nA, nB, connections, ordering, crossing_upper_bound);
    // oscm::compute_ordering_primary(nA, nB, connections, ordering, crossing_upper_bound);

#if ENABLE_TIMER
    auto const end = std::chrono::system_clock::now();
    std::cout
      << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      << '\n';
#endif

#if DEBUG_MESSAGE

    std::cout << "ordering post:\n";
    for(auto const now: ordering) { std::cout << now << ' '; }
    std::cout << std::endl;

    std::cout
      << "current crossing count: " << oscm::count_crossings(nA, nB, ordering.data(), connections)
      << '\n';

#endif

    oscm::write_to_file(argv[2], ordering);
}

// References:
// [1] Fixed parameter algorithms for ONE-SIDED CROSSING MINIMIZATION revisited
// [2] A Fast and Simple Subexponential Fixed Parameter Algorithm for One-Sided Crossing Minimization