
#include <sstream>
#include <iostream>
#include "cli.hpp"
#include "commands/upload.hpp"
#include "commands/change_algorithm_settings.hpp"
#include "commands/classify_data.hpp"
#include "commands/display_results.hpp"
#include "commands/download_results.hpp"
#include "commands/confusion_matrix.hpp"

CLI::CLI(io::DefaultIo &dio, int client_id) : default_io(dio), client_id(client_id) {
    // creating paths for training, testing and results csv
    std::stringstream train;
    std::stringstream test;
    std::stringstream results;
    train << "train" << client_id << ".csv";
    test << "test" << client_id << ".csv";
    results << "results" << client_id << ".csv";

    // initializing client state
    this->client_data = new ClientData(test.str(), train.str(), results.str());
    // initializing commands
    this->commands.push_back(new command::Upload(this->default_io, this->client_data));
    this->commands.push_back(new command::ChangeAlgorithmSettings(this->default_io, this->client_data));
    this->commands.push_back(new command::ClassifyData(this->default_io, this->client_data));
    this->commands.push_back(new command::DisplayResults(this->default_io, this->client_data));
    this->commands.push_back(new command::DownloadResults(this->default_io, this->client_data));
    this->commands.push_back(new command::ConfusionMatrix(this->default_io,
                                                          this->client_data));
}

void CLI::start() {
    while (true) {
        this->print_menu();
        auto option = std::stoul(this->default_io.read()) - 1;
        std::cout << "[*] received option " << option << "\n";
        if (option > this->commands.size()) {
            this->default_io.write("invalid option\n");
            continue;
        }
        if (option == this->commands.size()) {
            this->default_io.write("exiting...\n");
            break;
        }
       this->commands.at(option)->execute();
    }
}

void CLI::print_menu() {
    std::stringstream prompt;
    prompt << "Welcome to the KNN Classifier Server. Please choose an option:\n";
    for (int i = 0; i < this->commands.size(); i++) {
        prompt << i + 1 << ". " << this->commands.at(i)->get_description() << "\n";
    }
    prompt << this->commands.size() + 1 << ". exit\n";
    this->default_io.write(prompt.str());
}

CLI::~CLI() {
    for (auto *command: this->commands) {
        delete command;
    }
    delete this->client_data;
}

