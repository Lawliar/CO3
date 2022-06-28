

#include <boost/program_options.hpp>
#include <iostream>
#include "Orchestrator.h"


boost::program_options::variables_map ParseCommand(int argc, const char *argv[]){
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
            ("inputDir,i",  boost::program_options::value<std::string>()->required(), "path to the intermediate folder")
            ("sp,s",   boost::program_options::value<std::string>()->required(), "sp")
            ("baudrate,b",boost::program_options::value<int>()->required(), "baudrate");

    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    return vm;
}
// for debug purpose only
extern set<string> SinkOps;
extern set<string> leaveOps;
extern set<string> leaves;
extern set<string> nodesDepOnRuntime;
// end of debug
int main(int argc, const char *argv[])
{
    boost::program_options::variables_map vm = ParseCommand(argc, argv);
    std::string input_path = vm["inputDir"].as<std::string>();
    std::string serial_port = vm["sp"].as<std::string>();
    int baud_rate = vm["baudrate"].as<int>();

    boost::filesystem::path dir (input_path);
    if(! boost::filesystem::exists(dir)){
        cerr << "input dir:"<<input_path<<" does not exist";
        assert(false);
    }

    Orchestrator orc(input_path,serial_port,baud_rate);
    orc.Run();
    __asm__("nop");

}