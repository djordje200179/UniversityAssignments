import array
import numpy as np
import matplotlib.pyplot as plt

ITERATIONS = 100_000 * 10
FILES_NUM = 20

file_names = [f"errors_{i}.bat" for i in range(FILES_NUM)]
errors = []
for file_name in file_names:
	with open(file_name, "br") as file:
		numbers = array.array("i")
		numbers.fromfile(file, ITERATIONS)
		
	errors.append(numbers)
	
x_coords = np.arange(1, ITERATIONS + 1)
    
plt.figure(1)
plt.subplot(121)
plt.loglog(x_coords, np.transpose(errors))
plt.subplot(122)
plt.loglog(x_coords, np.mean(errors, axis=0)) 
plt.show()