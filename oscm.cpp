#include <oscm/bipartite_graph.h>
#include <oscm/count_crossings.h>
#include <oscm/segment_tree.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

void read_file(
  char const input_file[],
  std::uint32_t &n_,
  std::uint32_t &nA_,
  std::uint32_t &nB_,
  std::uint32_t &m_,
  std::unique_ptr<std::pair<std::uint32_t, std::uint32_t>[]> &edges_);

int main(int argc, char *argv[]) {
    // [[assume(argc > 0)]];
    if(argc != 3) {
        std::cerr << "usage: " << argv[0] << " [path/to/input] [path/to/output]" << std::endl;
        return 1;
    }

    std::uint32_t n;
    std::uint32_t nA;
    std::uint32_t nB;
    std::uint32_t m;
    std::unique_ptr<std::pair<std::uint32_t, std::uint32_t>[]> edges;
    read_file(argv[1], n, nA, nB, m, edges);


}

void read_file(
  char const input_file[],
  uint32_t &n_,
  uint32_t &nA_,
  uint32_t &nB_,
  uint32_t &m_,
  std::unique_ptr<std::pair<std::uint32_t, std::uint32_t>[]> &edges_) {
    std::ifstream ifs(input_file);
    if(!ifs) {
        std::cerr << "Failed to open file " << input_file << std::endl;
        std::exit(1);
    }

    std::string line;
    std::uint32_t idx = 0;
    while(std::getline(ifs, line)) {
        if(line.size() < 3 || line[0] == 'c') continue;
        std::istringstream iss(line);
        if(line[0] == 'p') {
            std::string str;
            iss >> str >> str >> nA_ >> nB_ >> m_;
            edges_ = std::make_unique_for_overwrite<std::pair<std::uint32_t, std::uint32_t>[]>(m_);
            continue;
        }
        iss >> edges_[idx].first >> edges_[idx].second;
        idx++;
    }
    std::sort(edges_.get(), edges_.get() + m_, [](auto a, auto b) { return a.first < b.first; });
}