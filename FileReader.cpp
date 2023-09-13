#include "FileReader.h"

FileReader::FileReader(std::string &&file_name) : _file_name(std::forward<std::string>(file_name)), _measurement_pointer(0)
{
}

FileReader::~FileReader()
{
    _section_buffer.clear();
}

void FileReader::jump_to_percent(unsigned int percent)
{
}

Measurement *FileReader::read_measurement()
{
    if (_section_buffer.empty())
    {
        read_next_section();
    }
    if (_section_buffer.empty())
    {
        return nullptr;
    }
    auto measurement = &_section_buffer[_measurement_pointer];
    _measurement_pointer++;
    if (_measurement_pointer > SECTION_SIZE)
    {
        _section_buffer.clear();
        _measurement_pointer = 0;
    }
    return measurement;
}

void FileReader::read_next_section()
{
}
