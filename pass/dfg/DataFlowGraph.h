//
// Created by charl on 4/8/2022.
//

#ifndef SYMBOLICCOMPILER_DATADEPGRAPH_H
#define SYMBOLICCOMPILER_DATADEPGRAPH_H

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"
#include <iostream>

typedef std::string NodeType;
#define NodeConstInt       "constantInt"
#define NodeConstFloat     "constantFloat"
#define NodeConstDouble    "constantDouble"
#define NodeSym            "sym"
#define NodeIntepretedFunc "symFunc"
#define NodeRuntimeInt     "runtimeInt"
#define NodeRuntimeFloat   "runtimeFloat"
#define NodeRuntimeDouble  "runtimeDouble"
#define NodeRuntimePtr      "runtimePtr"
#define NodeTruePhi         "truePhi"
#define NodeFalseRootPhi        "falseRootPhi"
#define NodeFalseLeafPhi        "falseLeafPhi"

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
        unsigned int byteWidth;
        unsigned long BBID;
    };
    struct Edge_Properties                                    // property bundle for vertices
    {
        unsigned arg_no;
        unsigned incomingBB;    // used by phi only, also this field is not really used, just for debugging
        unsigned dashed;             //used by falsePhiRoot Only, to visualize the dependency between falsePhiLeaf and falsePhiRoot
    };
    typedef boost::adjacency_list<boost::listS, boost::vecS,  boost::bidirectionalS,
            Vertex_Properties,Edge_Properties> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_it;
    typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
    typedef boost::graph_traits<Graph>::edge_iterator edge_it;
    typedef boost::graph_traits<Graph>::out_edge_iterator o_edge_it;


public:
    inline static const std::string symIDPrefix         = "symid";
    inline static const std::string opPrefix            = "op";
    inline static const std::string nodeTPrefix         = "nodeT";
    inline static const std::string constantValuePrefix = "cv";
    inline static const std::string widthPrefix         = "bw";
    inline static const std::string BasicBlockPrefix    = "BB";

    template <class symIDMap,class opMap, class nodeTypeMap, class constValueMap, class byteWidthMap,class BBMap>
    class node_writer {
    public:
        node_writer(symIDMap s, opMap o,nodeTypeMap n,constValueMap c ,byteWidthMap b, BBMap bb, bool visualize) : sm(s),om(o),nm(n),cm(c),bm(b),bbm(bb), visualize(visualize){}
        template <class Node>
        void operator()(std::ostream &out, const Node& n) const {
            if(visualize){
                out << "[label=\"" << symIDPrefix << ":"<<sm[n] <<'|'<< opPrefix <<":"<<om[n] <<'\n' \
                                    <<nodeTPrefix<<":"<<nm[n] <<'|' <<constantValuePrefix<<":"<<cm[n] <<'|' <<widthPrefix<<":"<<bm[n] <<'\n' \
                                    <<BasicBlockPrefix<<":"<<bbm[n]<< "\"]";
            }else{
                out << "["                            \
                    << symIDPrefix           <<"="  <<sm[n] <<',' \
                    << opPrefix           <<"="  <<om[n] <<','  \
                    <<nodeTPrefix         <<"="  <<nm[n] <<',' \
                    <<constantValuePrefix <<"="  <<cm[n] <<','  \
                    <<widthPrefix         <<"="  <<bm[n] <<',' \
                    <<BasicBlockPrefix    <<"="  <<bbm[n];
                if(nm[n] == NodeFalseLeafPhi){
                    out <<",color=blue";
                }
                if(nm[n] == NodeFalseRootPhi){
                    out << ",color=red";
                }
                out <<  "]";
            }

        }

    private:
        symIDMap sm;
        opMap om;
        nodeTypeMap nm;
        constValueMap cm;
        byteWidthMap bm;
        BBMap bbm;
        bool visualize;
    };

    template <class symIDMap,class opMap, class nodeTypeMap, class constValueMap, class byteWidthMap,class BBMap>
    inline node_writer<symIDMap,opMap,nodeTypeMap,constValueMap,byteWidthMap,BBMap>
    make_node_writer(symIDMap s, opMap o,nodeTypeMap n,constValueMap c ,byteWidthMap b, BBMap bb, bool v) {
        return node_writer<symIDMap,opMap,nodeTypeMap,constValueMap,byteWidthMap,BBMap>(s,o,n,c,b,bb,v);
    }

    inline static const std::string argNoPrefix              = "label";
    inline static const std::string incomingBBPrefix         = "bb";

    template <class argNoMap,class incomingBBMap, class DashedMap>
    class edge_writer {
    public:
        edge_writer(argNoMap a, incomingBBMap i, DashedMap d) : am(a),im(i), dm(d){}
        template <class Edge>
        void operator()(std::ostream &out, const Edge& e) const {
            out << "["  << argNoPrefix           <<"="  <<am[e] <<',';
            if(dm[e] == 1){
                 out << "style=dashed,";
            }
            out << incomingBBPrefix           <<"="  <<im[e] << "]";
        }

    private:
        argNoMap am;
        incomingBBMap im;
        DashedMap dm;
    };

    template <class argNoMap,class incomingBBMap, class DashedMap>
    inline edge_writer<argNoMap,incomingBBMap, DashedMap>
    make_edge_writer(argNoMap a, incomingBBMap i, DashedMap d) {
        return edge_writer<argNoMap, incomingBBMap, DashedMap>(a,i, d);
    }


    SymDepGraph(bool AddNullSym);
    SymDepGraph::vertex_t AddSymVertice(unsigned symID, std::string op,unsigned long);
    SymDepGraph::vertex_t AddPhiVertice(std::string, unsigned symID, unsigned long);
    SymDepGraph::vertex_t AddInterFuncVertice(unsigned symID, std::string op,unsigned long);
    SymDepGraph::vertex_t AddConstVertice(std::string, long value, unsigned int byte_width);
    SymDepGraph::vertex_t AddRuntimeVertice(std::string, unsigned int byte_width,unsigned long);
    SymDepGraph::vertex_t AddVertice(int symID,std::string op,NodeType nodeType,long const_value,unsigned int, unsigned long);

    void AddEdge(unsigned from_symid, unsigned to_symid, unsigned arg_no);
    void AddEdge(vertex_t, vertex_t, unsigned);
    void AddPhiEdge(unsigned from_symid, unsigned to_symid, unsigned,unsigned, unsigned);
    SymDepGraph::vertex_it GetVerticeBySymID(unsigned symID);

    SymDepGraph::vertex_it GetVerticeEndIt();

    void writeToFile(std::string filename);

    Graph graph;                                                // the boost graph


};


#endif //SYMBOLICCOMPILER_DATADEPGRAPH_H
