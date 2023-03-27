from IPython import embed
import pygraphviz as pyg
import networkx as nx
import os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-i",required=True,help="input function")
parser.add_argument("-o",required=True,help="output graph merging the two")

args = parser.parse_args()


symIDPrefix         = "symid"
opPrefix            = "op"
nodeTPrefix         = "nodeT"
constantValuePrefix = "cv"
widthPrefix         = "bw"
BasicBlockPrefix    = "BB"

setLabel = True

NoOurEdgeNodes = []
ChildOfNoEdgeNode = []
def ifOutEdge(node, edges):
    for eachEdge in edges:
        if node == eachEdge[0]:
            return True
    return False


def DirectChildOf(node, nodes, edges):
    for eachEdge in edges:
        if node == eachEdge[0] and eachEdge[1] in nodes:
            return True
    return False

def nodeAttrsToLabel(d):
    return symIDPrefix + ":" + d[symIDPrefix] +'|'+ opPrefix +":"+d[opPrefix] +'\n' +nodeTPrefix+":"+d[nodeTPrefix] +'|' +constantValuePrefix+":"+d[constantValuePrefix] +'|' +widthPrefix+":"+d[widthPrefix] +'\n' +BasicBlockPrefix+":"+d[BasicBlockPrefix]
def main():
    dfg_path = args.i+'_dfg.dot'
    cfg_path = args.i+'_cfg.dot'
    if(not os.path.exists(dfg_path)):
        print("{} does not exist\n" % (dfg_path))
        return 0
    if(not os.path.exists(cfg_path)):
        print("{} does not exist\n" % (cfg_path))
        return 0
    dfg = pyg.AGraph(dfg_path, strict=False, directed=True)
    cfg = pyg.AGraph(cfg_path, strict=False, directed=True)
    cdfg = nx.DiGraph()
    cdfg.graph['compound']='true'
    if('BB' not in dfg.nodes()[0].attr):
        print("please use the non-visualized dfg")
    cdfg.add_edges_from(dfg.edges())
    cdfg = nx.nx_agraph.to_agraph(cdfg)
    ## copy the attr for edge
    for each_dfg_edge in dfg.edges():
        for k in each_dfg_edge.attr.keys():
            cdfg.get_edge(each_dfg_edge[0],each_dfg_edge[1]).attr[k] = each_dfg_edge.attr[k]
    ## collect nodes that I want to see
    for each_dfg_node in dfg.nodes():
        if not ifOutEdge(each_dfg_node, cdfg.edges()):
            NoOurEdgeNodes.append(each_dfg_node)
    for each_dfg_node in dfg.nodes():
        if DirectChildOf(each_dfg_node, NoOurEdgeNodes,cdfg.edges()):
            ChildOfNoEdgeNode.append(each_dfg_node)

    for bbid in [x.attr['label'] for x in cfg.nodes()]:
        nodes = []
        for each_dfg_node in dfg.nodes():
            cur_bbid = each_dfg_node.attr['BB']
            if(cur_bbid == bbid):
                nodes.append(each_dfg_node)
        sub = cdfg.subgraph(nodes, name="cluster"+bbid,label="BB:"+bbid)

        for each_dfg_node in nodes:
            cdfg.add_node(each_dfg_node)
            if setLabel:
                if each_dfg_node in NoOurEdgeNodes:
                    cdfg.get_node(each_dfg_node).attr["label"] = nodeAttrsToLabel(dfg.get_node(each_dfg_node).attr)
            for each_original_attr in dfg.get_node(each_dfg_node).attr:
                cdfg.get_node(each_dfg_node).attr[each_original_attr] = dfg.get_node(each_dfg_node).attr[each_original_attr]
    
    ## special case dfg node 0
    bbid = '0'
    nodes = []
    for each_dfg_node in dfg.nodes():
        cur_bbid = each_dfg_node.attr['BB']
        if(cur_bbid == bbid):
            nodes.append(each_dfg_node)
    for each_dfg_node in nodes:
        cdfg.add_node(each_dfg_node)
        #if setLabel:
        #    cdfg.get_node(each_dfg_node).attr["label"] = nodeAttrsToLabel(dfg.get_node(each_dfg_node).attr)
        for each_original_attr in dfg.get_node(each_dfg_node).attr:
            cdfg.get_node(each_dfg_node).attr[each_original_attr] = dfg.get_node(each_dfg_node).attr[each_original_attr]
    
    for f,t in cfg.edges():
        f_cluster = cdfg.get_subgraph('cluster'+f.name)
        if(len(f_cluster.nodes()) == 0):
            continue
        f_node = f_cluster.nodes()[0]
        t_cluster = cdfg.get_subgraph('cluster'+t.name)
        if(len(t_cluster.nodes()) == 0):
            continue
        t_node = t_cluster.nodes()[0]

        cdfg.add_edge(f_node, t_node, color="red", lhead ='cluster'+t.name , ltail = 'cluster'+f.name )
    cdfg.write("{}.dot".format(os.path.basename(args.i)))

    ## some check:
    for (f,t) in cdfg.edges():
        if f.attr['BB'] != t.attr['BB']:
            f_cfg_node = cfg.get_node(f.attr['BB'])
            assert(f_cfg_node.attr['id'] == f.attr['BB'])
            t_cfg_node = cfg.get_node(t.attr['BB'])
            assert(t_cfg_node.attr['id'] == t.attr['BB'])
            if(not cfg.has_edge(f_cfg_node,t_cfg_node)):
                print("there is no direct cfg edge for data flow:", f, "|", t)
    ##nx.drawing.nx_pydot.write_dot(cdfg,"cdfg.dot")
    #nx.draw(G, pos)
    #for each_dfg_node in dfg.nodes():
    #    if(each_dfg_node.attr['BB'] == '0'):
    #        pass

if __name__ == "__main__":
    main()