def input_sizes(): return map(int, input().split())
def input_matrix(rows): return [matrix.append([int(num) for num in input().split()]) for i in range(rows)]
def create_array(matrix):
    n = len(matrix)
    array = []
    for j in range(n - 1, 0, -1):
        for i in range(n - j):
            array.append(matrix[i][j + i])
    for i in range(n):
        for j in range(n - i):
            array.append(matrix[i + j][j])
    return array
n, m = input_sizes()
matrix = input_matrix(n)
if n == m: print(*create_array(matrix), sep=' ')