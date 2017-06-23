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
	print "Clustering average:", G.clustering_average()
	print "Clustering transitivity:", G.cluster_transitivity() # Transitivity is the fraction of all existing triangles and all connected triples (triads), T = 3*triangles / triads.
	print "Cluster Triangles:", G.cluster_triangles()
	print "Clustering coefficient:", G.clustering_coeff()
	print ""

	print "***************************************************"
	print "************* Centrality Information **************"
	print "***************************************************"
	
	print "Betweenness Centrality: ", D.centrality_betweenness()
	print "Closeness Centrality: ", D.centrality_closeness()
	print ""
	
	print "***************************************************"
	print "************* Connectivity Information *************"
	print "***************************************************"
	print "Connected component sizes: ", D.connected_components_sizes()
	#print "Strongly connected components: ", D.strongly_connected_components()
	#print "Max cut: ", D.max_cut()
	
	L = D.degree_histogram()
	H = []
	for grado in range(len(L)):
		numvertices = L[grado]
		for v in range(numvertices):
			H.append(grado)

	histogram(H,bins=12).show()
	
	print "***************************************************"
	print "****************** Robustness *********************"
	print "***************************************************"
	print GraphRobustness(D)

# Receives a graph D and a probability p and keeps each edge with probability p. Returns the resulting graph
def SimulatePercolation(D,p):
	E = D.edges()
	newedges = []
	for e in E:
		if (random() < p):
			newedges.append(e)
	newD = DiGraph()
	newD.add_edges(newedges)
	return newD

def GraphRobustness(D,blocks = 100, numtimesperProb = 100):
	blocksize = 1.0/blocks
	print blocksize
	L = [(0,1)]
	for p in [x * blocksize for x in range(1,blocks)]:
		avg_largest_component_size = 0.0
		for i in range(numtimesperProb):
			S = SimulatePercolation(D,p)
			CS = S.connected_components_sizes()
			t = 0
			if (CS):
				t = max(CS)
			avg_largest_component_size += t
		avg_largest_component_size /= numtimesperProb
		L.append((p,avg_largest_component_size))
	P = sum([point2d(l) for l in L])
	P.show()
	return L

def remove_edges(D,k):
	L = []
	edges = D.edges()
	edges.sort() # by inverse weight... how do I do that Marisol?
	for i in range(k):
		edges.pop()
	newD = DiGraph()
	newD.add_edges(edges)
	return newD

def EdgeRemover(D):
	L = []
	edges = D.edges()
	edges.sort() # by inverse weight... how do I do that Marisol?
	while (edges):
		newD = DiGraph()
		newD.add_edges(edges)
		L.append(newD)
		edges.pop()
	return L