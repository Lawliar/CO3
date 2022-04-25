//
// Created by charl on 4/22/2022.
//

#include "Orchestrator.h"


Orchestrator::Orchestrator(std::string cfg_filename, std::string ddg_filename, std::string sp_port, int baud_rate)
{
    cfg.readGraphViz(cfg_filename);
    ddg.readGraphViz(ddg_filename);
    sp = initSerialPort(sp_port.c_str(), baud_rate);
}
Orchestrator::~Orchestrator() {
    freeSerialPort(sp);
    sp = nullptr;
}