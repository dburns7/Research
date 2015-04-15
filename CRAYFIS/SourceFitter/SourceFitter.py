import numpy as np
import scipy.optimize as opt
import pylab as pl

filein = 'android.dat.old'
#filein = 'r2.mc'
data = np.loadtxt(open(filein,"r"),delimiter=" ")

xx = data[:,0]
N  = data[:,1]
t  = data[:,2]
r  = data[:,3] 
err_r = [0, 0, 0, 0, 0]
for i in range(0, 5):
  err_r[i] = r[i] * np.sqrt(N[i] + (10/t[i])**2 + 4*(0.25/xx[i])**2)

rmax = r.max()
r = r/rmax
err_r = err_r/rmax

def chisqfunc((a, b)):
    model = a / (xx - b)**2
    chisq = np.sum(((r - model)/err_r)**2)
    return chisq

x_i = np.array([0,1])
result =  opt.minimize(chisqfunc, x_i)
print result
a,b=result.x*rmax
print a
print b

fig1 = pl.figure()
#ye = [np.sqrt(n) for n in N]
ye = [0,0,0,0,0]
for i in range(0, 5):
  ye[i] = 10* np.sqrt(N[i]) / t[i]
print ye
pl.errorbar(xx, r*rmax, yerr=ye, fmt='o', linewidth='2', label='Data x10')
#pl.plot(x, r*rmax, color='blue', linewidth='1', label='Data')
#pl.plot(x,a+b*x, color='red', linewidth='1', label='Fit') #test with linear model
x1 = np.linspace(min(xx), max(xx), num=100)
pl.plot(x1, a / (x1 - b)**2, color='red', linewidth='2', label='Fit')
pl.xlabel('X [in]')
pl.ylabel('Rate [Candidates/s]')
pl.legend(loc=1)
pl.show()
fig1.savefig('rate.png')

