import numpy as np
import matplotlib.pyplot as plt

#load data
data = np.loadtxt('dataset.dat')
x=data[:,0]
y=data[:,1]
z=data[:,2]

#init figure
fig = plt.figure()
ax1 = fig.add_subplot(221)
ax1.plot(x,y)
ax2 = fig.add_subplot(223)
ax2.plot(x,z)
ax3 = fig.add_subplot(122,projection='3d')
ax3.plot(x,y,z)

fig.tight_layout()
plt.show()