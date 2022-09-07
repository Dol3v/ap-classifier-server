
#include <map>
#include <sstream>
#include <utility>
#include <iostream>
#include "confusion_matrix.hpp"

namespace command {

    typedef std::map<std::string, std::map<std::string, double>> confusion_matrix;

    void initialize_confusion_matrix(confusion_matrix &matrix, const std::set<std::string> &classes) {
        for (const auto &true_class: classes) {
            matrix.insert(std::make_pair(true_class, std::map<std::string, double>()));
            for (const auto &predicted_class: classes) {
                matrix[true_class].insert(std::make_pair(predicted_class, 0));
            }
        }
    }

    std::string class_from_csv_entry(const std::string &line) {
        auto idx = line.find_last_of(',');
        if (idx == std::string::npos)
            throw std::logic_error("confusion matrix: read invalid csv entry");
        return line.substr(idx + 1, line.size());
    }

    std::string confusion_matrix_to_string(const confusion_matrix &matrix, const std::set<std::string> &classes) {
        std::stringstream table;
        // std::set is ordered, hence we can use that to ensure the table's correct formatting
        for (const auto &class_: classes) {
            table << class_ << '\t';
        }
        table << '\n';
        for (const auto &true_class: classes) {
            table << true_class << '\t';
            for (const auto &predicted_class: classes) {
                table << matrix.at(true_class).at(predicted_class) << '\t';
            }
            table << '\n';
        }

        return table.str();
    }

    void ConfusionMatrix::execute() {
        if (!(this->client_state->get_state_flag() & ClientState::CLASSIFIED_DATA)) {
            this->default_io.write("classify data first\n");
            return;
        }
        auto classes = this->client_state->get_classes();
        // counts the number of classes in general
        std::map<std::string, int> counters;
        for (const auto &class_: classes) {
            counters[class_] = 0;
        }

        // initializes confusion matrix
        confusion_matrix matrix;
        initialize_confusion_matrix(matrix, classes);

        auto test_stream = std::fstream(client_state->test_path, std::fstream::out | std::fstream::in);
        test_stream.seekg(std::fstream::beg);
        auto result_stream = std::fstream(client_state->result_path, std::fstream::out | std::fstream::in);
        result_stream.seekg(std::fstream::beg);

        // loading data into matrix
        std::string actual_result_line;
        std::string model_result_line;
        std::getline(test_stream, actual_result_line);
        std::getline(result_stream, model_result_line);

        while (!actual_result_line.empty() && !model_result_line.empty()) {
            auto predicted_class = class_from_csv_entry(model_result_line);
            auto actual_class = class_from_csv_entry(actual_result_line);

            matrix[actual_class][predicted_class]++;
            counters[actual_class]++;

            std::getline(test_stream, actual_result_line);
            std::getline(result_stream, model_result_line);
        }

        // some due cleanup
        test_stream.close();
        result_stream.close();

        // normalizing
        for (const auto &true_class: classes) {
            for (const auto &predicted_class: classes) {
                matrix[true_class][predicted_class] /= counters.at(true_class);
            }
        }

        // sending to user
        this->default_io.write(confusion_matrix_to_string(matrix, classes));
    }

    ConfusionMatrix::ConfusionMatrix(io::DefaultIo &io, ClientData *state) : Command("confusion matrix", io, state) {

    }
} // command