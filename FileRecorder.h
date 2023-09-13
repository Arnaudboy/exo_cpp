#include <fstream>
#include <chrono>
#include <vector>
#include <fstream>
#include "RecordStruct.h"

class FileRecorder
{
public:
    FileRecorder(std::string &&file_name);
    ~FileRecorder();

    void end_record(); // finish the record, makes the header and save data in the file
    void add_measure(int measurement, std::chrono::seconds timestamp);

private:
    const int _section_size = SECTION_SIZE;
    std::string _file_name;
    std::vector<Measurement> _measurements_buffer;
    std::chrono::seconds _record_duration;
    unsigned int _nb_records;
    std::fstream _record_file;
    void record_section(std::vector<Measurement> &_section_buffer);
    void record_main_header();
    void record_section_header(Section &_section);
    void record_section_data(Section &_section);
};
