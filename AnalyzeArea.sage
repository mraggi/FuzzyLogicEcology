import numpy as np

def AnalyzeAreas(A):
    A.sort()
    # First, we simply fit an exponential curve a*e^(ci) for (i,Area[i])
    print "Fitting exponential model"
    x=var('x')

    P = sum([point2d((i,A[i])) for i in range(len(A))])

    var('a b c')
    def exponential_model_error(a,c):
        return sum([(a*(e^(c*i)) - A[i])^2 for i in range(len(A))])
    
    R = minimize(exponential_model_error(a,c),(1,0.09),algorithm="powell")

    print "(a,c) = ", R
    
    x=var('x')
    P += plot(R[0]*e^(R[1]*x),x,0,len(A)+1,ymax=max(A)+1,ymin=min(A)-1,thickness=2,color="red")
    P.show(dpi=300)
    print "Finished fitting exponential model\n\n"

    # Second, we fit a logistic model a/(b+A^(-c))
    print "Fitting logistic model"
    def logistic_model_error(a,b,c):
        return sum([(a/(b+(i+0.5)^(-c)) - A[i])^2 for i in range(len(A))])
    
    LM = sum([point2d((i,A[i])) for i in range(len(A))])
    var('a b c')
    
    R = minimize(logistic_model_error(a,b,c),(1,1,0.09),algorithm="powell")

    print "(a,b,c) = ", R
    x=var('x')
    LM += plot(R[0]/(R[1] + (x+0.5)^(-R[2])),x,0,len(A)+1,ymax=max(A)+1,ymin=min(A)-1,thickness=2,color="red")
    LM.show(dpi=300)
    print "Finished fitting logistic model\n\n"

##Second, we do a bin histogram

def BinHistogram(A,num_bins=15):
    maxarea = max(A)
    print "min area =",min(A)
    print "max area =",max(A)

    H = histogram(A,bins=num_bins)
    x=var('x')


    histograma = np.histogram(A,bins=num_bins)[0]

    #histograma = [80, 28, 18, 16, 10,  5,  5,  2,  3,  6,  5,  1,  5,  1,  2,  2,  4, 2,  0,  1,  1,  1,  1,  0,  0,  0,  1,  0,  0,  1]

    PH = [(maxarea*(i+0.5)/(num_bins-0.5),histograma[i]) for i in range(num_bins)]

    print PH

    def f(a,c):
        return sum([(c*(PH[i][0])^(-a) - PH[i][1])^2 for i in range(len(histograma))])


    R = minimize(f(a,c),(0.5,100),algorithm="powell")

    print "(a,c) = ", R

    P = plot(R[1]*x^(-R[0]),x,min(A),max(A),ymax=max(histograma)+1,thickness=2,color="red")
    (P+H).show(dpi=300)