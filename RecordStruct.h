#define SECTION_SIZE 3

typedef struct
{
    int measurement;
    std::chrono::seconds timestamp;

} Measurement;

typedef struct
{
    std::chrono::seconds duration;
    unsigned int nb_records;
    Measurement measurements[SECTION_SIZE];
} Section;

typedef struct
{
    std::chrono::seconds section_duration;
    unsigned int section_size;
} SectionHeader;
