#include <chrono>
#include <thread>
#include <iostream>
#include "FileReader.h"
#include "reader.h"


void play_file_from(unsigned int percent, std::string&& file_name) {
    auto file_reader = FileReader(std::forward<std::string>(file_name));
    file_reader.jump_to_percent(percent);
    play_file(file_reader, percent);
}

void play_file(FileReader& file_reader, const unsigned int & percent) {
    auto measurement = file_reader.read_measurement();
    std::chrono::seconds time_point;
    bool isFirst = true;
    while (measurement != nullptr) {
        std::cout << "Measuring: " << measurement->measurement << " ";
        if (isFirst)
        {
            isFirst = false;
            if (percent == 0 || percent >= 100) {
                std::this_thread::sleep_for(std::chrono::seconds(measurement->timestamp));
                std::cout << "Waiting for: " << measurement->timestamp.count() << std::endl;
            }
        }
        else
        {
            auto waiting_time = measurement->timestamp - time_point;
            std::this_thread::sleep_for(waiting_time);
            std::cout << "Waiting for: " << waiting_time.count() << std::endl;
        }
        time_point = measurement->timestamp;
        measurement = file_reader.read_measurement();
    }
}