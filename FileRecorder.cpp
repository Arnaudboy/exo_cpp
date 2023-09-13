#include <iostream>
#include "FileRecorder.h"

FileRecorder::FileRecorder(std::string&& file_name): _file_name(file_name), _record_duration(0) {}

void FileRecorder::end_record() {

}

void FileRecorder::add_measure(int measurement, std::chrono::seconds timestamp) {
    std::cout << "Recording measure: " << measurement << " at: " << timestamp.count() << "(s)"
    << " in file " << _file_name << std::endl;
}

void FileRecorder::record_data() {

}

void FileRecorder::record_section() {
    
}