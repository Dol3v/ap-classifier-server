
#include "classify_data.hpp"
#include "../classifier/knn.hpp"
#include "../csv/csv.hpp"

#include <sstream>

/// Trains the model on a pre-classified dataset.
void train_model(KnnClassifier<std::vector<double>, std::string> &model, std::fstream &data_file) {
    while (data_file) {
        std::string line;
        std::getline(data_file, line);
        if (line.empty()) {
            break;
        }
        std::stringstream ss(line);

        auto values = csv::read_line<double, double, double, double, std::string>(ss);

        auto data_class = std::get<4>(values);
        auto data = utils::tup2vec4<double, std::string>(values);

        model.load_data(data, data_class);
    }
}

/// Classifies the data from a csv input file, and writes the classified data to an output file in csv form.
void classify(KnnClassifier<std::vector<double>, std::string> &model, std::fstream &unclassified,
              std::fstream &classified, const Metric<std::vector<double>> &metric) {
    while (unclassified) {
        std::string line;
        std::getline(unclassified, line);
        if (line.empty()) {
            break;
        }
        std::stringstream ss(line);
        auto values = csv::read_line<double, double, double, double, std::string>(ss);

        std::string data_class = model.predict(utils::tup2vec4(values), metric);
        csv::write_line(classified, std::get<0>(values),
                        std::get<1>(values),
                        std::get<2>(values),
                        std::get<3>(values),
                        data_class);
    }
}

command::ClassifyData::ClassifyData(io::DefaultIo &io, ClientData *state) : Command("classify data", io, state) {}

void command::ClassifyData::execute() {
    // verifying prereqs
    if (!(client_state->get_state_flag() & ClientState::UPLOADED_DATA) ||
        !(client_state->get_state_flag() & ClientState::HAS_SETTINGS)) {
        this->default_io.write("upload the required data first\n");
        return;
    }
    this->default_io.write("classifying data...\n");
    // training model
    KnnClassifier<std::vector<double>, std::string> classifier(this->client_state->get_k());

    // file initializations
    auto train_stream = std::fstream(this->client_state->train_path, std::fstream::out | std::fstream::in);
    auto test_stream = std::fstream(this->client_state->test_path, std::fstream::out | std::fstream::in);
    train_stream.seekg(std::fstream::beg);
    test_stream.seekg(std::fstream::beg);
    auto result_stream = std::fstream(this->client_state->result_path, std::ios::out | std::ios::trunc);

    train_model(classifier, train_stream);
    // classifying and saving into file
    classify(classifier, test_stream, result_stream,
             this->client_state->get_metric());

    train_stream.close();
    test_stream.close();
    result_stream.close();

    this->client_state->mark_classified();
    this->client_state->set_classification_classes(std::move(*classifier.get_classes()));
    this->default_io.write("classifying data complete\n");
}
