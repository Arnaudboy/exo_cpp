#include <iostream>
#include "cmd_ops.h"
#include <string>

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        cout << "Usage record_reader [-j][num] [file]" << endl << endl
        << "\t-f file" << endl
        << "\t\tbinary file containing the record" << endl << endl
        << "\t-j num" << endl
        << "\t\tjump at num percent of the record and play from there" << endl;
        return 1;
    }
    struct MyOpts
    {
        string stringOpt{};
        int intOpt{};
        bool boolOpt{};
    };

    auto parser = CmdOpts<MyOpts>::Create({
        {"-f", &MyOpts::stringOpt },
        {"-j", &MyOpts::intOpt}});

    auto myopts = parser->parse(argc, argv);

    cout << "stringOpt = " << myopts.stringOpt << endl;
    cout << "intOpt = " << myopts.intOpt << endl;
    return 0;
}