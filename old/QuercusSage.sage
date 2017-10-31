load QuercusMatriz.sage

# From here on we assume G is the appropiate digraph

print "Betweenness centrality: "
BC = G.centrality_betweenness()
for v in sorted(BC.keys()):
    print str(v)+": "+str(BC[v])
