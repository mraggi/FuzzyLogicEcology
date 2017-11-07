load AnalyzeArea.sage
load Areas.sage

print "************* Analyzing QUERCUS with PRODUCT *************"
PlotAndFitArea(QuercusAreas)
print "************* Analyzing QUERCUS with MIN *************"
PlotAndFitArea(QuercusFuzzyMin)

print "************* Analyzing LOBATAE with PRODUCT *************"
PlotAndFitArea(LobataeAreas)
print "************* Analyzing LOBATAE with MIN *************"
PlotAndFitArea(LobataeFuzzyMin)

print "************* Analyzing CENTRO with PRODUCT *************"
PlotAndFitArea(CentroAreas)
print "************* Analyzing CENTRO with MIN *************"
PlotAndFitArea(CentroFuzzyMin)
