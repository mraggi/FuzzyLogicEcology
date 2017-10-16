
A.sort()
#A.pop()
#A.pop()
#A.pop()
#A.pop()
#A.pop()
#A.pop()
x=var('x')

P = sum([point2d((i,A[i])) for i in range(len(A))])

var('a b c')
def f(a,c):
	#return sum([(a*x^2+b*x+c - A[i])^2 for i in range(len(A))])
	return sum([(a*(e^(c*i)) - A[i])^2 for i in range(len(A))])


R = minimize(f(a,c),(1,0.09),algorithm="powell")

print "(a,c) = ", R
x=var('x')
P += plot(R[0]*e^(R[1]*x),x,0,len(A)+1,ymax=max(A)+1,ymin=min(A)-1,thickness=3,color="red")
#P += plot(2*e^(0.03*x),x,0,len(A),ymax=max(A)+1,ymin=min(A)-1,thickness=5,color="red")
P.show()