import numpy as np
import scipy.optimize as opt
import pylab as pl

filein = 'r2.mc'
data = np.loadtxt(open(filein,"r"),delimiter=" ")

x = data[:,0]
r = data[:,1]
err_r = data[:,2]

rmax = r.max()
r = r/rmax
err_r = err_r/rmax

def chisqfunc((a, b)):
    #model = a + b*x #test with linear model
    model = a / (x - b)**2
    chisq = np.sum(((r - model)/err_r)**2)
    return chisq

x_i = np.array([0,0])
result =  opt.minimize(chisqfunc, x_i)
print result
#assert result.success==True
a,b=result.x*rmax
print a
print b

fig1 = pl.figure()
pl.plot(x, r*rmax, color='blue', linewidth='1', label='Data')
pl.plot(x,a+b*x, color='red', linewidth='1', label='Fit') #test with linear model
#pl.plot(x, a / (x - b)**2, color='red', linewidth='2', label='Fit')
pl.xlabel('X [in]')
pl.ylabel('Rate [Candidates/s]')
pl.legend(loc=2)
pl.show()
fig1.savefig('rate.png')

