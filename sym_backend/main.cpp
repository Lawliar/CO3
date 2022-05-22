

#include <boost/program_options.hpp>
#include <iostream>
#include "Orchestrator.h"

boost::program_options::variables_map ParseCommand(int argc, const char *argv[]){
    try
    {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()
                ("cfg,c",  boost::program_options::value<std::string>()->required(), "cfg")
                ("ddg,d",  boost::program_options::value<std::string>()->required(), "ddg")
                ("sp,s",   boost::program_options::value<std::string>()->required(), "sp")
                ("baudrate,b",boost::program_options::value<int>()->required(), "baudrate");

        boost::program_options::variables_map vm;
        boost::program_options::store(parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);
        return vm;
    }
    catch (const boost::program_options::error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
}
int main(int argc, const char *argv[])
{
    boost::program_options::variables_map vm = ParseCommand(argc, argv);
    std::string ddg_path = vm["ddg"].as<std::string>();
    std::string cfg_path = vm["cfg"].as<std::string>();
    std::string serial_port = vm["sp"].as<std::string>();
    int baud_rate = vm["baudrate"].as<int>();

    Orchestrator orc(cfg_path,ddg_path,serial_port,baud_rate);
    orc.ddg.loopCheck();
}