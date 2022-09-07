
#include <sstream>
#include <iostream>
#include "send_classified.hpp"

namespace command {
    void SendClassified::execute() {
        if (!(this->client_state->get_state_flag() & ClientState::CLASSIFIED_DATA)) {
            this->default_io.write("didn't classify data yet\n");
            return;
        }
        auto results = std::fstream(this->client_state->result_path,
                                    std::fstream::in | std::fstream::out | std::fstream::app);
        results.seekg(std::fstream::beg);

        std::stringstream ss;
        std::string line;
        int counter = 1;
        while (std::getline(results, line)) {
            // getting class
            auto idx = line.find_last_of(',');
            auto class_str = line.substr(idx + 1, line.length() - idx);
            // formatting and sending output
            ss << counter << "\t" << class_str << "\n";
            ++counter;
        }
        ss << "Done.\n";
        this->default_io.write(ss.str());
    }
} // command