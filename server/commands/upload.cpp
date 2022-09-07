
#include <iostream>
#include "upload.hpp"

void command::Upload::execute() {
    this->default_io.write("Please upload your local train CSV file.\n");

    auto train_fstream = std::fstream(this->client_state->train_path,
                                           std::fstream::in | std::fstream::out | std::fstream::trunc);
    this->default_io.file_save(train_fstream);
    train_fstream.close();
    this->default_io.write("Please upload your local test CSV file.\n");
    auto test_fstream = std::fstream(this->client_state->test_path,
                                           std::fstream::in | std::fstream::out | std::fstream::trunc);
    this->default_io.file_save(test_fstream);
    test_fstream.close();
    this->client_state->mark_uploaded_data();
}
