import math
import numpy as np
from scipy.optimize import minimize
import matplotlib.pyplot as plt


@np.vectorize
def act(x):
    return 1 / (1 + math.exp(-x))


def vectorize_fix_second(func):
    return np.vectorize(func, excluded={1})


@vectorize_fix_second
def calc_y(x, w):
    s = 0
    s += w[15]

    for k in range(5):
        s += w[k + 10] * act(w[k] * x + w[k + 5])

    return s


real_data = np.loadtxt("data.txt")
x_data, y_data = real_data[:, 0], real_data[:, 1]


def calc_error(w):
    return ((y_data - calc_y(x_data, w)) ** 2).mean()


best_w = None
best_error = math.inf

while best_error > 10**-2:
    initial_guess = np.random.uniform(low=-30, high=30, size=(16,))

    curr_result = minimize(calc_error, initial_guess, method='nelder-mead', bounds=((-30, 30),)*16)
    curr_w = curr_result.x
    curr_error = calc_error(curr_w)

    print("Iteration done!", f"Error: {curr_error:.3f}")

    if curr_error < best_error:
        best_error = curr_error
        best_w = curr_w

print()
print("Constraint satisfied!")

print(f"Error: {best_error:.15f}")
print(f"W: [{','.join(['%.15f' % elem for elem in best_w])}]")

plt.plot(x_data, y_data, label="Real data")
plt.plot(x_data, calc_y(x_data, best_w), label="Calculated data")
plt.legend()
plt.show()
