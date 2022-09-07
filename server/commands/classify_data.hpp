#pragma once

#include "command.hpp"

namespace command {
    class ClassifyData : public Command {
    public:
        ClassifyData(io::DefaultIo &io, ClientData *state);
        virtual void execute();
    };
}
