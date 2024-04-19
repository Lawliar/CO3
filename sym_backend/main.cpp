// This file is part of CO3.
//
// CO3 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// CO3 is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// CO3. If not, see <https://www.gnu.org/licenses/>.



#include <boost/program_options.hpp>
#include <iostream>
#include "Orchestrator.h"
#ifdef GPROFILING
#include <gperftools/profiler.h>
#endif

boost::program_options::variables_map ParseCommand(int argc, const char *argv[]){
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
            ("inputDir,i",  boost::program_options::value<std::string>()->required(), "path to the intermediate folder")
            ("port,p",   boost::program_options::value<std::string>()->required(), "port")
            ("baudrate,b",boost::program_options::value<int>()->required(), "baudrate");

    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    return vm;
}
#ifdef DEBUG_OUTPUT
extern set<string> SinkOps;
extern set<string> leaveOps;
extern set<string> leaves;
extern set<string> nodesDepOnRuntime;
#endif
extern char * dir_cstr;
int main(int argc, const char *argv[])
{
    boost::program_options::variables_map vm = ParseCommand(argc, argv);
    std::string input_path = vm["inputDir"].as<std::string>();
    std::string serial_port = vm["port"].as<std::string>();
    int baud_rate = vm["baudrate"].as<int>();

    boost::filesystem::path dir (input_path);
    if(! boost::filesystem::exists(dir)){
        cerr << "input dir:"<<input_path<<" does not exist";
        assert(false);
    }
    dir_cstr = (char *)input_path.c_str();
    Orchestrator orc(input_path,serial_port,baud_rate);
#ifdef GPROFILING
    ProfilerStart("orchestrator.prof");
#endif
    if(orc.ser->used){
        orc.SendInput();
    }

    orc.Run();
#ifdef GPROFILING
    ProfilerStop();
#endif
    __asm__("nop");
    return 0;
}