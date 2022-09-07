#pragma once

#include <string>
#include "../../io/defaultio.hpp"
#include "../client_data.hpp"

namespace command {
    class Command {
    private:
        const std::string description;
    protected:
        io::DefaultIo &default_io;
        ClientData *client_state;
    public:
        Command(std::string desc, io::DefaultIo &io, ClientData *state) : description(std::move(desc)), default_io(io),
                                                                          client_state(state) {}

        virtual void execute();

        std::string get_description() const { return this->description; }
    };

}
