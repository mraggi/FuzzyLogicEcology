def Analyze(D):
	G = Graph(D)
	print "Welcome to graph analyzer!"
	
	print "***************************************************"
	print "************* General Statistics ******************"
	print "***************************************************"
	print "Number of vertices: ", D.num_verts()
	print "Number of edges: ", D.num_edges()
	print "Diameter: ", D.diameter()
	print "Dirth: ", D.girth()
	print "Chromatic number:", G.chromatic_number()
	print "Clan number: ", G.clique_number()
	print "Average degree: ", D.average_degree()
	print ""
	
	print "***************************************************"
	print "************* Clustering Information **************"
	print "***************************************************"
	print "Clustering Information: "
	print "Clustering average:", D.clustering_average()
	print "Clustering transitivity:", D.cluster_transitivity() # Transitivity is the fraction of all existing triangles and all connected triples (triads), T = 3*triangles / triads.
	print "Cluster Triangles:", D.cluster_triangles()
	print "Clustering coefficient:", D.clustering_coeff()
	print ""

	print "***************************************************"
	print "************* Centrality Information **************"
	print "***************************************************"
	
	print "Betweenness Centrality: ", D.centrality_betweenness()
s	print "Closeness Centrality: ", D.centrality_closeness()
	print ""
	
	print "***************************************************"
	print "************* Connectivity Information *************"
	print "***************************************************"
	print "Connected component sizes: ", D.connected_components_sizes()
	print "Strongly connected components: ", D.strongly_connected_components()
	print "Max cut: ", D.max_cut()
	
	L = D.degree_histogram()
	H = []
	for grado in range(len(L)):
		numvertices = L[grado]
		for v in range(numvertices):
			H.append(grado)

	histogram(H,bins=12).show()
	
	
	