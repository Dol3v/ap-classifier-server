#pragma once

#include <utility>

#include "command.hpp"

namespace command {
    class Upload : public Command {
    public:
        Upload(io::DefaultIo &defaultIo, ClientData *state) : Command("upload test/train csv data files",
                                                                   defaultIo, state) {}

        virtual void execute();
    };
}
