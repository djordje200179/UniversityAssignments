import math
import cmath
import numpy as np
import matplotlib.pyplot as plt

n = 6
beta = 20 * math.pi
theta = math.pi / 3
d = 1 / 20

def f(delta: float):
	phi = delta + beta * d * math.cos(theta)
	f = sum(map(lambda k: cmath.exp(-1j * k * phi), range(n)))
		
	return f

def f_abs(delta: float):
	return abs(f(delta))

plotting_iterations = 1000

def plot():
	deltas = np.linspace(0, 2 * math.pi, plotting_iterations)
	fs_abs = list(map(f_abs, deltas))

	plt.plot(deltas, fs_abs, linestyle='solid', color='r', linewidth=0.3)

	plt.title("Зрачење антене у зависности од угла")
	plt.xlabel('δ')
	plt.ylabel('|Fs|')
	
accuracy = 10 ** -7
	
def find_delta_max():
	delta_1 = 4
	delta_2 = 5
	delta_3 = 5.5
	
	f_abs_1 = f_abs(delta_1)
	f_abs_2 = f_abs(delta_2)
	f_abs_3 = f_abs(delta_3) 

	def find_delta_next():
		deltas_matrix = np.array([
			[delta_1**2, delta_1, 1], 
			[delta_2**2, delta_2, 1], 
			[delta_3**2, delta_3, 1]]
		)
		
		if np.linalg.det(deltas_matrix) == 0:
			return delta_1
		
		deltas_matrix = np.linalg.inv(deltas_matrix)
	
		f_abs_vector = np.array([
			f_abs_1,
			f_abs_2,
			f_abs_3
		])
	
		a, b, c = np.matmul(deltas_matrix, f_abs_vector)
		delta_max = -b / (2 * a)
	
		return delta_max

	iterations = 0
	while max(delta_1, delta_2, delta_3) - min(delta_1, delta_2, delta_3) > accuracy:
		delta_next = find_delta_next()
		if delta_next == delta_1:
			break

		f_abs_next = f_abs(delta_next)
	
		plt.scatter(delta_next, f_abs_next, c='g', s=0.03)

		if f_abs_1 < f_abs_2 and f_abs_1 < f_abs_3:
			delta_1 = delta_next
			f_abs_1 = f_abs_next
		elif f_abs_2 < f_abs_1 and f_abs_2 < f_abs_3:
			delta_2 = delta_next
			f_abs_2 = f_abs_next
		else:
			delta_3 = delta_next
			f_abs_3 = f_abs_next
			
		iterations += 1

	delta_max = find_delta_next()

	print(f"iterations = {iterations}")
	print(f"delta = {delta_max:.7f}")
	print(f"f_abs = {f_abs(delta_max):.7f}")
	
plot()
find_delta_max()
plt.show()