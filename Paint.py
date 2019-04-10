import matplotlib.pyplot as plt

file = open('Path.txt', 'r')
n = file.readline()
print(n)
n = int(n)
X = []
Y = []
for i in range(n):
	coordinate = file.readline()
	x, y = coordinate.split(' ')
	x = int(x)
	y = int(y)
	X.append(x)
	Y.append(y)

X.append(X[0])
Y.append(Y[0])
plt.scatter(X, Y)
plt.plot(X, Y)
plt.hold(True)
plt.show()