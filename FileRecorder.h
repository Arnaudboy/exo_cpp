#include <fstream>
#include <chrono>
#include <vector>

typedef struct {
    int measurement;
    std::chrono::seconds timestamp;

} Measurement;

typedef struct {
    std::chrono::seconds duration;
    unsigned int nb_records;
    Measurement measurements[100];
} Section;

class FileRecorder {
    public:
        FileRecorder(std::string&& file_name);
        ~FileRecorder() = default;

        void end_record(); // finish the record, makes the header and save data in the file 
        void add_measure(int measurement, std::chrono::seconds timestamp);

    private:
        std::string _file_name;
        std::vector<Measurement> _measurements_buffer;
        std::chrono::seconds _record_duration;
        void record_data();
        void record_section();
};
