import numpy as np
import matplotlib.pyplot as plt
from scipy import optimize

#define a function (ansatz for our fit)
def fit_func(x,a,b):
    return a*np.sin(b*x)

#generate 50 points equally spaced from -5 to 5
x=np.linspace(-5,5,50)

#generate 50 random numbers between -1 and 1
noise=2*(np.random.rand(50)-0.5)

#generate 50 points of a sinuisoidal function + noise
y=2.7*np.sin(1.3*x)+noise

#fit the curve
params,_=optimize.curve_fit(fit_func,x,y)
print(params)

#plot the points with an errorbar
fig,ax=plt.subplots(1,1)
ax.errorbar(x,y,np.ones(50),linestyle='',capsize=3,ecolor='g',
    marker='o',markersize=4,label='Data',xuplims=True)
ax.plot(x,fit_func(x,params[0],params[1]),color='r',label='Fitted function')
ax.legend(loc='best')

plt.show()



