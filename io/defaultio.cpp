#include <iostream>
#include "defaultio.hpp"
#include "../common.hpp"


void io::DefaultIo::file_save(std::fstream& dst_file) {
    auto contents = this->read();
    if (contents == "failed") {
        throw std::runtime_error("sender failed to upload data");
    }
    dst_file << contents;
}


std::fstream::pos_type io::get_file_size(std::fstream &fstream) {
    auto prev_pos = fstream.tellg();
    fstream.seekg(std::fstream::ate);
    auto size = fstream.tellg();
    // restoring to prev position
    fstream.seekg(prev_pos);
    return size;
}
