#pragma once

#include "send_classified.hpp"

namespace command {
    /// Download the classification results, identical to DisplayResults from server POV.
    class DownloadResults : public SendClassified {
    public:
        DownloadResults(io::DefaultIo& io, ClientData* data) : SendClassified(io, "download results", data) {}
    };
}