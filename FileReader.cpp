#include "FileReader.h"

FileReader::FileReader(std::string &&file_name) : _file_name(std::forward<std::string>(file_name)), _measurement_pointer(0)
{
    _record_file.open(_file_name, std::ios::in | std::ios::binary);
    read_section_header();
}

FileReader::~FileReader()
{
    _section_buffer.clear();
    _record_file.close();
}

void FileReader::jump_to_percent(unsigned int percent)
{
    if (percent == 0)
    {
        return;
    }
}

SectionHeader FileReader::jump_to_section(unsigned int section_num)
{
    // TODO: jump to section
    return read_section_header();
}

Measurement *FileReader::read_measurement()
{
    if (_section_buffer.empty() || _measurement_pointer >= _nb_records)
    {
        _section_buffer.clear();
        _measurement_pointer = 0;
        read_next_section();
    }
    if (_section_buffer.empty())
    {
        return nullptr;
    }
    auto measurement = &_section_buffer[_measurement_pointer];
    _measurement_pointer++;
    return measurement;
}

void FileReader::read_next_section()
{
    auto sectionHeader = read_section_header();
    (void)(sectionHeader); // disable builder warning
    _section_pointer++;
}

SectionHeader FileReader::read_section_header()
{
    return SectionHeader{};
}

SectionHeader FileReader::read_main_header()
{
    // move to the begining of the file
    return read_section_header();
}
