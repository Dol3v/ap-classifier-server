#pragma once


#include "command.hpp"

namespace command {

    class ConfusionMatrix : public Command {
    public:
        ConfusionMatrix(io::DefaultIo &io, ClientData *state);

        virtual void execute();
    };

} // command
