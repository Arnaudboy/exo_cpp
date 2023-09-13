#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "RecordStruct.h"

class FileReader
{

public:
    FileReader(std::string &&file_name);
    ~FileReader();

    void jump_to_percent(unsigned int percent);
    Measurement *read_measurement();

private:
    unsigned int _section_pointer;     // current section
    unsigned int _measurement_pointer; // current measurement inside current section
    std::chrono::seconds _record_duration;
    unsigned int _nb_records;
    std::string _file_name;
    std::vector<Measurement> _section_buffer;
    std::ofstream _record_file;

    void read_next_section();
    void read_section_header();
};