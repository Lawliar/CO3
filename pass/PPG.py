from IPython import embed
import pygraphviz as pyg
import networkx as nx
import os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-id",required=True,
                    help="input data flow graph")
parser.add_argument("-ic",required=True,help="input control flow graph")
parser.add_argument("-o",required=True,help="output graph merging the two")

args = parser.parse_args()


symIDPrefix         = "symid"
opPrefix            = "op"
nodeTPrefix         = "nodeT"
constantValuePrefix = "cv"
widthPrefix         = "bw"
BasicBlockPrefix    = "BB"

setLabel = False

def nodeAttrsToLabel(d):
    return symIDPrefix + ":" + d[symIDPrefix] +'|'+ opPrefix +":"+d[opPrefix] +'\n' +nodeTPrefix+":"+d[nodeTPrefix] +'|' +constantValuePrefix+":"+d[constantValuePrefix] +'|' +widthPrefix+":"+d[widthPrefix] +'\n' +BasicBlockPrefix+":"+d[BasicBlockPrefix]
def main():
    if(not os.path.exists(args.id)):
        print("{} does not exist\n" % (args.id))
        return 0
    if(not os.path.exists(args.ic)):
        print("{} does not exist\n" % (args.ic))
        return 0
    dfg = pyg.AGraph(args.id, strict=False, directed=True)
    cfg = pyg.AGraph(args.ic, strict=False, directed=True)
    cdfg = nx.DiGraph()
    cdfg.graph['compound']='true'
    if('BB' not in dfg.nodes()[0].attr):
        print("please use the non-visualized dfg")
    cdfg.add_edges_from(dfg.edges())
    cdfg = nx.nx_agraph.to_agraph(cdfg)

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
        if setLabel:
            cdfg.get_node(each_dfg_node).attr["label"] = nodeAttrsToLabel(dfg.get_node(each_dfg_node).attr)
        for each_original_attr in dfg.get_node(each_dfg_node).attr:
            cdfg.get_node(each_dfg_node).attr[each_original_attr] = dfg.get_node(each_dfg_node).attr[each_original_attr]
    for f,t in cfg.edges():
        f_cluster = cdfg.get_subgraph('cluster'+f.name)
        f_node = f_cluster.nodes()[0]

        t_cluster = cdfg.get_subgraph('cluster'+t.name)
        t_node = t_cluster.nodes()[0]

        cdfg.add_edge(f_node, t_node, color="red", lhead ='cluster'+t.name , ltail = 'cluster'+f.name )
    cdfg.write("cdfg.dot")
    
    ##nx.drawing.nx_pydot.write_dot(cdfg,"cdfg.dot")
    #nx.draw(G, pos)
    #for each_dfg_node in dfg.nodes():
    #    if(each_dfg_node.attr['BB'] == '0'):
    #        pass

if __name__ == "__main__":
    main()