load Areas.sage
load DenglerAnalysis.sage

f = doublelog

#for a in zip(AllAreas,Names):
	#area = a[0]
	#name = a[1]
	#P = Fit(name,area,f,log)
	
	#P.axes_labels(["log(Area)","Cumulative #"])
	
	##P.show(dpi=300)
	#P.save("results/cumulative/" + name + strmap[f] + ".png",dpi=300)
	
for a in zip(AllAreas,Names):
	area = a[0]
	name = a[1]
	P = Fit(name,area,f)
	
	P.axes_labels(["Area","Cumulative #"])
	
	#P.show(dpi=300)
	P.save("results/cumulative/nonlog/" + name + strmap[f] + ".png",dpi=300)