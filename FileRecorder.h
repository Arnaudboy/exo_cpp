#include <fstream>
#include <chrono>
#include <vector>

#define SECTION_SIZE 3

typedef struct {
    int measurement;
    std::chrono::seconds timestamp;

} Measurement;

typedef struct {
    std::chrono::seconds duration;
    unsigned int nb_records;
    Measurement measurements[SECTION_SIZE];
} Section;

class FileRecorder {
    public:
        FileRecorder(std::string&& file_name);
        ~FileRecorder() = default;

        void end_record(); // finish the record, makes the header and save data in the file 
        void add_measure(int measurement, std::chrono::seconds timestamp);

    private:
        const int _section_size = SECTION_SIZE;
        std::string _file_name;
        std::vector<Measurement> _measurements_buffer;
        std::chrono::seconds _record_duration;
        unsigned int _nb_records;
        void record_section(std::vector<Measurement>& _section_buffer);
        void record_main_header();
        void record_section_header(Section& _section);
        void record_section_data(Section& _section);
};
