#pragma once

#include "command.hpp"

namespace command {
    /// Sends the classified data over dio. For internal use.
    class SendClassified : public Command {
    public:
        SendClassified(io::DefaultIo &dio, const std::string &desc, ClientData *data) : Command(desc, dio, data) {}

        virtual void execute();
    };

} // command
