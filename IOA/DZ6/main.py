import scipy
import numpy as np

production = np.array([500, 300, 700, 250, 750])
demand = np.array([1000, 500, 200, 300])

costs = np.array([
    10, 20, 40, 60,
    30, 30, 10, 15,
    50, 35, 20, 30,
    70, 45, 15, 20,
    20, 65, 30, 60
]) * 0.1

factories_num = production.size
shops_num = demand.size

A_eq = np.array([[j%shops_num == i for j in range(costs.size)] for i in range(shops_num)])
A_ub = np.array([[j//shops_num == i for j in range(costs.size)] for i in range(factories_num)])

result = scipy.optimize.linprog(costs, A_ub, production, A_eq, demand)

print(f"x=({','.join(np.char.mod('%d', result.x))})")
print(f"Total: {np.matmul(result.x, np.transpose(costs))}")