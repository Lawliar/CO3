

#include <boost/program_options.hpp>
#include <iostream>
#include "RuntimeDataDepGraph.h"
#include "RuntimeCFG.h"

boost::program_options::variables_map ParseCommand(int argc, const char *argv[]){
    try
    {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()
                ("cfg,c", boost::program_options::value<std::string>()->required(), "cfg")
                ("ddg,d", boost::program_options::value<std::string>()->required(), "ddg")
                ("sp,s", boost::program_options::value<std::string>()->required(), "sp");

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
    RuntimeSymDepGraph ddg;
    ddg.readGraphViz(ddg_path);
    RuntimeCFG cfg;
    cfg.readGraphViz(cfg_path);
}