#include <iostream>
#include <memory>
#include "cmd_ops.h"
#include "recorder.h"
#include "reader.h"

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage record_reader [-pr] [-j][num] -f [file]" << endl
                  << endl
                  << "\t-f file" << endl
                  << "\t\tbinary file containing the record" << endl
                  << endl
                  << "\t-p" << endl
                  << "\t\tplay record" << endl
                  << endl
                  << "\t-r" << endl
                  << "\t\trun record" << endl
                  << endl
                  << "\t-j num" << endl
                  << "\t\trun with option -p" << endl
                  << "\t\tjump at num percent of the record and play from there" << endl;
        return 1;
    }
    struct MyOpts
    {
        std::string fileOpt{};
        int jumpOpt{};
        bool doRecord{};
        bool doPlay{};
    };

    auto parser = CmdOpts<MyOpts>::Create({{"-f", &MyOpts::fileOpt},
                                           {"-j", &MyOpts::jumpOpt},
                                           {"-r", &MyOpts::doRecord},
                                           {"-p", &MyOpts::doPlay}});

    auto myopts = parser->parse(argc, argv);

    std::cout << "stringOpt = " << myopts.fileOpt << endl;
    std::cout << "doRecord = " << myopts.doRecord << endl;
    std::cout << "jumpOpt = " << myopts.jumpOpt << endl;
    if (myopts.doRecord && !myopts.fileOpt.empty())
    {
        record_and_save(std::move(myopts.fileOpt));
    }

    if (myopts.doPlay && !myopts.fileOpt.empty() && !myopts.doRecord) {
        play_file_from(myopts.jumpOpt, std::move(myopts.fileOpt));
    }
    return 0;
}