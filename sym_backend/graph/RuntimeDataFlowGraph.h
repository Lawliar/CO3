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



#ifndef SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
#define SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H

#include "DataFlowGraph.h"
#include "RuntimeCFG.h"
#include "Runtime.h"
#include <map>
#include <set>
class RuntimeSymFlowGraph: public SymDepGraph{
public:

   typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_it;

    RuntimeSymFlowGraph(std::string);
    void readGraphViz(std::string filename);
    void loopCheck();

};
#endif //SYMBACKEND_RUNTIMEDATAFLOWGRAPH_H
