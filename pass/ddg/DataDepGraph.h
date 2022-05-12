//
// Created by charl on 4/8/2022.
//

#ifndef SYMBOLICCOMPILER_DATADEPGRAPH_H
#define SYMBOLICCOMPILER_DATADEPGRAPH_H

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"


typedef std::string NodeType;
#define NodeConst          "const"
#define NodeSym            "sym"
#define NodeIntepretedFunc "symFunc"
#define NodeRuntime        "runtime"
#define NodePhi            "phi"

#define VoidStr            "NaS"
class SymDepGraph
{
public:
    struct Vertex_Properties                                    // property bundle for vertices
    {
        std::string name;// not used for writing graph
        int symID;
        std::string op;
        NodeType nodeType;
        long const_value;
        unsigned int bitwidth;
        unsigned long BBID;
    };
    struct Edge_Properties                                    // property bundle for vertices
    {
        unsigned arg_no;
    };
    typedef boost::adjacency_list<boost::listS, boost::vecS,  boost::directedS,
            Vertex_Properties,Edge_Properties> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_it;
    typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
    typedef boost::graph_traits<Graph>::edge_iterator edge_it;


public:
    inline static const std::string symIDPrefix         = "symid";
    inline static const std::string opPrefix            = "op";
    inline static const std::string nodeTPrefix         = "nodeT";
    inline static const std::string constantValuePrefix = "cv";
    inline static const std::string widthPrefix         = "bw";
    inline static const std::string BasicBlockPrefix    = "BB";

    template <class symIDMap,class opMap, class nodeTypeMap, class constValueMap, class bitWidthMap,class BBMap>
    class node_writer {
    public:
        node_writer(symIDMap s, opMap o,nodeTypeMap n,constValueMap c ,bitWidthMap b, BBMap bb) : sm(s),om(o),nm(n),cm(c),bm(b),bbm(bb){}
        template <class Node>

         void operator()(std::ostream &out, const Node& n) const {
            out << "[label=\"" << symIDPrefix << ":"<<sm[n] <<'|'<< opPrefix <<":"<<om[n] <<'\n' \
                                    <<nodeTPrefix<<":"<<nm[n] <<'|' <<constantValuePrefix<<":"<<cm[n] <<'|' <<widthPrefix<<":"<<bm[n] <<'\n' \
                                    <<BasicBlockPrefix<<":"<<bbm[n]<< "\"]";
        }/*
        void operator()(std::ostream &out, const Node& n) const {
            out << "["                            \
                    << symIDPrefix           <<"="  <<sm[n] <<',' \
                    << opPrefix           <<"="  <<om[n] <<','  \
                    <<nodeTPrefix         <<"="  <<nm[n] <<',' \
                    <<constantValuePrefix <<"="  <<cm[n] <<','  \
                    <<widthPrefix         <<"="  <<bm[n] <<',' \
                    <<BasicBlockPrefix    <<"="  <<bbm[n] << "]";
        }*/

    private:
        symIDMap sm;
        opMap om;
        nodeTypeMap nm;
        constValueMap cm;
        bitWidthMap bm;
        BBMap bbm;
    };

    template <class symIDMap,class opMap, class nodeTypeMap, class constValueMap, class bitWidthMap,class BBMap>
    inline node_writer<symIDMap,opMap,nodeTypeMap,constValueMap,bitWidthMap,BBMap>
    make_node_writer(symIDMap s, opMap o,nodeTypeMap n,constValueMap c ,bitWidthMap b, BBMap bb) {
        return node_writer<symIDMap,opMap,nodeTypeMap,constValueMap,bitWidthMap,BBMap>(s,o,n,c,b,bb);
    }


    SymDepGraph();
    SymDepGraph::vertex_t AddSymVertice(unsigned symID, std::string op,unsigned long);
    SymDepGraph::vertex_t AddPhiVertice(unsigned symID, unsigned long);
    SymDepGraph::vertex_t AddInterFuncVertice(unsigned symID, std::string op,unsigned long);
    SymDepGraph::vertex_t AddConstVertice( long value, unsigned int bit_width);
    SymDepGraph::vertex_t AddRuntimeVertice(unsigned int bit_width,unsigned long);
    SymDepGraph::vertex_t AddVertice(int symID,std::string op,NodeType nodeType,long const_value,unsigned int, unsigned long);

    void AddEdge(unsigned from_symid, unsigned to_symid, unsigned arg_no);
    void AddEdge(vertex_t, vertex_t, unsigned);
    SymDepGraph::vertex_it GetVerticeBySymID(unsigned symID);

    SymDepGraph::vertex_it GetVerticeEndIt();

    void writeToFile(std::string filename);

    Graph graph;                                                // the boost graph


};


#endif //SYMBOLICCOMPILER_DATADEPGRAPH_H
