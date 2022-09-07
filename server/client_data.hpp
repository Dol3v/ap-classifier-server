#pragma once

#include <fstream>
#include <vector>
#include <set>
#include "classifier/metric.hpp"
#include <memory>

enum ClientState : uint8_t {
    /// client didn't do any action yet
    NONE = 0,
    /// client initialized settings
    HAS_SETTINGS = 1 << 0,
    /// client uploaded training/classifying data
    UPLOADED_DATA = 1 << 1,
    /// client classified data and the results were saved in a local temp file
    CLASSIFIED_DATA = 1 << 2
};

/// The state of a client: their relevant classifier settings, training data, etc.
class ClientData {
private:
    int k = 0;
    Metric<std::vector<double>> *metric;
    std::set<std::string> classes;
    uint8_t state_flag = ClientState::NONE;
public:
    const std::string test_path;
    const std::string train_path;
    const std::string result_path;

    ClientData(std::string testPath, std::string trainPath, std::string resultPath);

    void change_settings(int new_k, Metric<std::vector<double>> *new_metric);

    void set_classification_classes(std::set<std::string> &&new_classes);

    void mark_uploaded_data();

    void mark_classified() { this->state_flag |= ClientState::CLASSIFIED_DATA; }

    uint8_t get_state_flag() const;

    int get_k() const { return this->k; }

    const std::set<std::string> &get_classes() const { return this->classes; }

    const Metric<std::vector<double>> &get_metric() const { return *metric; }

    ~ClientData();
};
