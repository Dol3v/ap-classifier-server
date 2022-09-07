#pragma once

#include "defaultio.hpp"

namespace io {
    class StandardIo : public io::DefaultIo {
    public:
        virtual std::string read();
        virtual void write(const std::string&);
    };
}
