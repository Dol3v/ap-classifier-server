#pragma once
#include <string>
#include <functional>
#include <fstream>

namespace io {
    class DefaultIo {
    public:
        virtual std::string read() = 0;

        virtual void write(const std::string&) = 0;

        /// Saves a file to a given path.
        void file_save(std::fstream& dst_file);
    };

    /// Returns file size
    std::fstream::pos_type get_file_size(std::fstream& fstream);
}
