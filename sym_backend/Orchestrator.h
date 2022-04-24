//
// Created by charl on 4/22/2022.
//

#ifndef SYMBACKEND_ORCHESTRATOR_H
#define SYMBACKEND_ORCHESTRATOR_H

#include <string>
#include "Runtime.h"
#include "RuntimeCFG.h"
#include "RuntimeDataDepGraph.h"
extern "C" {
#include "serialport.h"
}


class Orchestrator{
public:
    Orchestrator(std::string cfg_filename, std::string ddg_filename, std::string sp_port, int);
    ~Orchestrator();
    RuntimeSymDepGraph ddg;
    RuntimeCFG cfg;
    struct sp_port * sp;
};
#endif //SYMBACKEND_ORCHESTRATOR_H
