#include <iostream>
#include "FileRecorder.h"

FileRecorder::FileRecorder(std::string &&file_name) : _file_name(std::forward<std::string>(file_name)), _record_duration(0), _nb_records(0)
{
    _measurements_buffer.reserve(_section_size);
    _record_file.open(_file_name, std::ios::out | std::ios::binary);
}

FileRecorder::~FileRecorder()
{
    _record_file.close();
}

void FileRecorder::end_record()
{
    if (_measurements_buffer.size() > 0)
    {
        record_section(_measurements_buffer);
    }
    _measurements_buffer.clear();
    record_main_header();
    _record_file.close();
}

void FileRecorder::add_measure(int measurement, std::chrono::seconds timestamp)
{
    std::cout << "Recording measure: " << measurement << " at: " << timestamp.count() << "(s)"
              << " in file " << _file_name << std::endl;
    _record_duration = timestamp;
    _nb_records++;
    if (_measurements_buffer.size() < _section_size)
    {
        _measurements_buffer.push_back({
            measurement,
            timestamp,
        });
    }
    if (_measurements_buffer.size() == _section_size)
    {
        record_section(_measurements_buffer);
        _measurements_buffer.clear();
    }
}

void FileRecorder::record_main_header()
{
    std::cout << "> [Record] Total size: " << _nb_records << std::endl;
    std::cout << "> [Record] Total duration: " << _record_duration.count() << "(s)" << std::endl;
    _record_file.write((char *)&_nb_records, sizeof(_nb_records));
    _record_file.write((char *)&_record_duration, sizeof(_record_duration));
}

void FileRecorder::record_section_header(Section &_section)
{
    std::cout << "> [Record] Section size: " << _section.nb_records << std::endl;
    std::cout << "> [Record] Section duration: " << _section.duration.count() << "(s)" << std::endl;
    _record_file.write((char *)&_section, sizeof(_section));
}

void FileRecorder::record_section_data(Section &_section)
{
    unsigned i = 0;
    for (auto data : _section.measurements)
    {
        if (i == _section.nb_records)
        {
            break;
        }
        std::cout << "> [Record] Data in section: " << data.measurement << " timestamp: " << data.timestamp.count() << "(s)" << std::endl;
        _record_file.write((char *)&data, sizeof(data));
        i++;
    }
}

void FileRecorder::record_section(std::vector<Measurement> &_section_buffer)
{
    auto section_duration = _section_buffer.back().timestamp - _section_buffer.front().timestamp;
    auto section = Section{
        .duration = std::move(section_duration),
        .nb_records = (unsigned int)(_section_buffer.size()),
    };
    memcpy(section.measurements, &_section_buffer[0], _section_buffer.size() * sizeof(Measurement));
    record_section_header(section);
    record_section_data(section);
}
