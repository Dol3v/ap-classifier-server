#pragma once

#include "send_classified.hpp"

namespace command {
    /// Displays the classification results.
    class DisplayResults : public SendClassified {
    public:
        DisplayResults(io::DefaultIo& dio, ClientData* data) : SendClassified(dio, "display results", data) {}
    };
}