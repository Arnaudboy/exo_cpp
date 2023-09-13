#include <chrono>
#include <thread>
#include <iostream>
#include "FileReader.h"
#include "reader.h"


void play_file_from(unsigned int percent, std::string&& file_name) {
    auto file_reader = FileReader(std::forward<std::string>(file_name));
    file_reader.jump_to_percent(percent);
    play_file(file_reader);
}

void play_file(FileReader& file_reader) {
    auto measurement = file_reader.read_measurement();
    while (measurement != nullptr) {
        std::cout << "Measuring: " << measurement->measurement << std::endl;
        std::cout << "Waiting for: " << measurement->timestamp.count() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(measurement->timestamp));
        measurement = file_reader.read_measurement();
    }
}