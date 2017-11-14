import numpy as np


var('x')

var('p0')
var('p1')
var('p2')

linear(x,p0,p1,p2) = p0*x + p1
quadratic(x,p0,p1,p2) = p0*x*x + p1*x + p2
squareroot(x,p0,p1,p2) = p0*sqrt(x+p1) + p2
power(x,p0,p1,p2) = p0*x^p1 + p2
exponential(x,p0,p1,p2) = p0*e^p1 + p2
logarithmic(x,p0,p1,p2) = p0 + p1*log(x)
logistic(x,p0,p1,p2) = p0/(1 + exp(-p1*x + p2))
logistic_inverse(x,p0,p1,p2) = (p0*x/(p1 + x))^p2
MichaelisMenten(x,p0,p1,p2) = p0*x/(p1+x)
Lomolino(x,p0,p1,p2) = p0/( 1 + p1^(log(p2/x)) )
doublelog(x,p0,p1,p2) = p0 + p1*log(x+1) + p2*log(log(x+1))

strmap = {linear:"linear",
		  quadratic:"quadratic",
		  squareroot:"squareroot",
		  power:"power",
		  exponential:"exponential",
		  logarithmic:"logarithmic",
		  logistic:"logistic",
		  logistic_inverse:"logistic_inverse",
		  MichaelisMenten:"MichaelisMenten",
		  Lomolino:"Lomolino",
		  doublelog:"loglog"}



			
def error(B,f,p0,p1,p2):
	return sum([(f(b[0],p0,p1,p2) - b[1])^2 for b in B])/len(B)

#error = [0,error1, error2, error3, error4]

def identity(x):
	return x

def CumulativeDistribution(A,area_func = identity,species_func = identity):
	A.sort()
	B = []
	i = 0
	for a in A:
		i += 1
		B.append((area_func(a),species_func(i)))
	return B

def PlotCumulative(A,area_func,species_func):
	B = CumulativeDistribution(A,area_func,species_func)
	W = sum([point2d(b) for b in B])
	return W

def ErrorNormalizado(A,f,R):
	total = 0.0
	for a in A:
		obtained = f(a[0],R[0],R[1],R[2])
		expected = a[1]
		total += (obtained - expected)^2
	
	total /= len(A)

	return sqrt(total)

def Fit(name,A,f, area_func = identity, species_func = identity):
	var('x')
	var('p0')
	var('p1')
	var('p2')
	
	B = CumulativeDistribution(A,area_func,species_func)
	var('p0')
	var('p1')
	var('p2')
	
	def E(W):
		return error(B,f,W[0],W[1],W[2])
	
	R = ()
	if (len(B) > 500):
		R = minimize(E,(50.125,1.123412412,5.01),algorithm="powell")
	else:
		R = minimize(E,(50.125,1.123412412,5.01))
	
	xmin = min([b[0] for b in B])
	xmax = max([b[0] for b in B])
	
	ymin = min([b[1] for b in B])
	ymax = max([b[1] for b in B])
	
	print "(p0,p1,p2) = ", R

	print "mins y maxes: ", xmin, xmax, ymin, ymax
	
	P = plot(f(x,R[0],R[1],R[2]),x,xmin,xmax,ymin=ymin, ymax=ymax,thickness=1,color="red")
	P += PlotCumulative(A,area_func,species_func)
	
	P += text(name, (xmax*0.45,ymax),color="black",fontsize=15)
	
	errorN = ErrorNormalizado(B,f,R)
	
	approx_symbol = u"\u2248"
	errorN = "Error " + approx_symbol + str(round(errorN,3)) 
	
	textsep = 0.06
	startx = 0.9
	starty = 0.4
	
	var('x')
	
	P += text("Fit: " + strmap[f] + " model", (startx*xmax,starty*ymax),color="red",fontsize=14)
	starty -= textsep
	
	PaR = [round(r,2) for r in R]
	
	P += text("$f(x) = " + latex(f(x,PaR[0],PaR[1],PaR[2])) + "$", (startx*xmax,starty*ymax),color="firebrick",fontsize=14)	
	#P += text(str(round(R[0],3)) + "x + " + str(round(R[1],3)), (startx*xmax,starty*ymax),color="firebrick",fontsize=14)	
	starty -= textsep
	P += text(errorN,(startx*xmax,starty*ymax),color="magenta",fontsize=14)
	
	return P