#ifndef ONESIDEDCROSSINGNUMBER_OSCM_WRITE_TO_FILE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_WRITE_TO_FILE_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

namespace oscm {
    __attribute__((always_inline)) inline void
    write_to_file(char const output_file_name_[], std::vector<std::uint32_t> const &ordering_) {
        std::ofstream ofs(output_file_name_);
        if(!ofs) {
            std::cerr << "Failed to open file of filename. Printing result to stdout." << std::endl;
            for(std::uint32_t vertex: ordering_) { std::cout << vertex + 1 << '\n'; }
            return;
        }
        for(std::uint32_t vertex: ordering_) { ofs << vertex + 1 << '\n'; }
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_WRITE_TO_FILE_H
