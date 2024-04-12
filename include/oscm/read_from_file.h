#ifndef ONESIDEDCROSSINGNUMBER_OSCM_READ_FILE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_READ_FILE_H

#include <oscm/comp_second.h>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

namespace oscm {
    void read_from_file(
      char const input_file_name_[],
      std::uint32_t &nA_,
      std::uint32_t &nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> &edges_) {
#ifdef NO_COMMENTS
        std::ifstream ifs(input_file_name_);
        if(!ifs) { std::cerr << "Failed to open file " << input_file_name_ << std::endl; }
        char str[4];
        std::uint64_t m;
        ifs >> str >> str >> nA_ >> nB_ >> m;
# if __has_cpp_attribute(assume)
        [[assume(nA_)]];
        [[assume(nB_)]];
        [[assume(m)]];
# endif
        edges_.resize(m);
        for(auto &[first, second]: edges_) {
            ifs >> first >> second;
# if __has_cpp_attribute(assume)
            [[assume(first)]];
            [[assume(second)]];
# endif
            first--;
            second--;
        }
#else
        std::ifstream ifs(input_file_name_);
        if(!ifs) {
            std::cerr << "Failed to open file '" << input_file_name_ << "'." << std::endl;
            std::exit(1);
        }

        std::string line;
        std::uint32_t idx = 0;
        std::uint32_t m;
        while(std::getline(ifs, line)) {
            if(line.size() < 3 || line[0] == 'c') continue;
            std::istringstream iss(line);
            if(line[0] == 'p') {
                std::string str;
                iss >> str >> str >> nA_ >> nB_ >> m;
# if __has_cpp_attribute(assume)
                [[assume(nA_)]];
                [[assume(nB_)]];
                [[assume(m)]];
# endif
                edges_.resize(m);
                continue;
            }
            iss >> edges_[idx].first >> edges_[idx].second;
# if __has_cpp_attribute(assume)
            [[assume(edges_[idx].first)]];
            [[assume(edges_[idx].second)]];
# endif
            edges_[idx].first--;
            edges_[idx].second--;
# if __has_cpp_attribute(assume)
            [[assume(idx < std::numeric_limits<std::uint32_t>::max())]];
# endif
            idx++;
        }
#endif
        std::sort(edges_.begin(), edges_.end(), comp_second);
    }
}  // namespace oscm
#endif  // ONESIDEDCROSSINGNUMBER_OSCM_READ_FILE_H
