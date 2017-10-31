import powerlaw

A = [80, 28, 18, 16, 10,  5,  5,  2,  3,  6,  5,  1,  5,  1,  2,  2,  4, 2,  0,  1,  1,  1,  1,  0,  0,  0,  1,  0,  0,  1]
#A = [(a+2)/(max(A)+2) for a in A]
print(A)
resultado = powerlaw.Fit(A)
print(resultado.alpha)
print(resultado.xmin)

#A = [335, 281, 282, 211, 194, 174, 183, 142, 148, 146, 129]
#print(powerlaw.Fit(A).alpha)