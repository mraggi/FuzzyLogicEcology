L = G.degree_histogram()
H = []
for grado in range(len(L)):
	numvertices = L[grado]
	for v in range(numvertices):
		H.append(grado)

histogram(H,bins=12).show()