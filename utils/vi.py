#!/usr/bin/env python3
import sys
import pydot

arr = []
idnt = []
L = []

G = pydot.Dot(graph_type="graph")
G.set_node_defaults(shape="record", fontname="monospace")

def prepare():
    arr = sys.stdin.readlines()
    for i, f in enumerate(arr):
        p = pydot.Node('n'+str(i), label=('|'.join([
            "<f%s>%s" % (i, s.strip().replace('<', '@').replace('>', '@'))
                for i,s in enumerate(f.strip(" :\n").replace(',', ':').split(':'))
        ])))

        G.add_node(p)
        L.append((f.count('  '), p))

def proc(i):
    for j in range(i+1, len(L)):
        if L[j][0] == L[i][0]+1:
            G.add_edge(pydot.Edge(L[i][1], L[j][1]))
            proc(j)
        if L[j][0] == L[i][0]:
            break

if __name__ == '__main__':
    prepare()
    proc(0)
    G.write_png("ast.png")