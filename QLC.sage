load AnalyzeArea.sage
load Areas.sage

print "************* Analyzing QUERCUS with PRODUCT *************"
AnalyzeAreas(QuercusAreas)
print "************* Analyzing QUERCUS with MIN *************"
AnalyzeAreas(QuercusFuzzyMin)

print "************* Analyzing LOBATAE with PRODUCT *************"
AnalyzeAreas(LobataeAreas)
print "************* Analyzing LOBATAE with MIN *************"
AnalyzeAreas(LobataeFuzzyMin)

print "************* Analyzing CENTRO with PRODUCT *************"
AnalyzeAreas(CentroAreas)
print "************* Analyzing CENTRO with MIN *************"
AnalyzeAreas(CentroFuzzyMin)