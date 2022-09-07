#include <memory>
#include <utility>
#include "client_data.hpp"

void ClientData::change_settings(int new_k, Metric<std::vector<double>> *new_metric) {
    if (new_k <= 0)
        throw std::logic_error("invalid value for k");
    this->k = new_k;
    if (this->metric)
        delete this->metric;
    this->metric = new_metric;
    this->state_flag = ClientState::HAS_SETTINGS;
}

uint8_t ClientData::get_state_flag() const {
    return this->state_flag;
}

void ClientData::set_classification_classes(std::set<std::string> &&new_classes) {
    this->classes = new_classes;
}

void ClientData::mark_uploaded_data() {
    this->state_flag ^= ClientState::CLASSIFIED_DATA;
    this->state_flag |= ClientState::UPLOADED_DATA;
}

ClientData::ClientData(std::string testPath, std::string trainPath, std::string resultPath)
        : test_path(std::move(testPath)), train_path(std::move(trainPath)), result_path(std::move(resultPath)) {
    this->metric = nullptr;
}

ClientData::~ClientData() {
    if (this->metric)
        delete this->metric;
}

