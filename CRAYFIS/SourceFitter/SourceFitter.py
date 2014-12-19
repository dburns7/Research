import numpy as np
import scipy.optimize as opt
import pylab as pl

filein = 'android.dat'
#filein = 'r2.mc'
data = np.loadtxt(open(filein,"r"),delimiter=" ")

x = data[:,0]
N = data[:,1]
t = data[:,2]
r = data[:,3] 
err_r = [0, 0, 0, 0, 0]
for i in range(0, 5):
  err_r[i] = r[i] * np.sqrt(N[i] + (10/t[i])**2 + 4*(0.25/x[i])**2)
print err_r
#err_r = data[:,2]

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
#pl.plot(x,a+b*x, color='red', linewidth='1', label='Fit') #test with linear model
pl.plot(x, a / (x - b)**2, color='red', linewidth='2', label='Fit')
pl.xlabel('X [in]')
pl.ylabel('Rate [Candidates/s]')
pl.legend(loc=1)
pl.show()
fig1.savefig('rate.png')

