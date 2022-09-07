#pragma once


#include "command.hpp"

namespace command {

    /// Change the algorithm settings
    class ChangeAlgorithmSettings : public Command {
    public:
        ChangeAlgorithmSettings(io::DefaultIo& dio, ClientData *state) : Command("algorithm settings", dio, state) {}

        void execute();
    };

} // command