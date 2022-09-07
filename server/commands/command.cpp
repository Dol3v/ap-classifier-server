#include <stdexcept>
#include "command.hpp"

void command::Command::execute() {
    throw std::logic_error("not implemented");
}
