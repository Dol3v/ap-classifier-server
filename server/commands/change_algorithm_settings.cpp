#include <sstream>
#include <iostream>
#include "change_algorithm_settings.hpp"

namespace command {

    void ChangeAlgorithmSettings::execute() {
        if (this->client_state->get_state_flag() == ClientState::NONE) {
            default_io.write("no settings present, please enter them (k metric):\n");
        } else {
            // write prompt
            std::string prompt;
            std::stringstream ss(prompt);
            ss << "The current KNN parameters are: K="
               << this->client_state->get_k()
               << ", distance metric = "
               << this->client_state->get_metric()
               << "\n";
            this->default_io.write(ss.str());
        }

        // recv output and validate parameters
        auto res = this->default_io.read();
        if (res.empty() || res == "none") return;
        auto idx = res.find_first_of(' ');
        if (idx == std::string::npos) {
            this->default_io.write("invalid parameters entered\n");
            return;
        }

        auto k_str = res.substr(0, idx);
        auto metric_str = res.substr(idx + 1);
        // in case \n was sent too (happens in standardIo)
        if (res.find('\n') != std::string::npos) {
            metric_str = metric_str.substr(0, metric_str.size() - 1);
        }

        if (metric_str.empty()) {
            this->default_io.write("invalid parameters entered\n");
            return;
        }
        std::cout << "[*] metric name: " << metric_str << "\n";
        auto* metric = metric_from_name(metric_str);
        if (metric == nullptr) {
            this->default_io.write("invalid metric name entered\n");
            return;
        }
        try {
            client_state->change_settings(std::stoi(k_str), metric);
        } catch (std::exception& exception) {
            this->default_io.write("invalid parameters entered\n");
            return;
        }
        this->default_io.write("settings changed successfully\n");

    }
}