def input_sizes():
    return map(int, input().split())

def input_matrix(rows):
    matrix = []
    for i in range(rows):
        matrix.append([int(num) for num in input().split()])

    return matrix

def create_array(matrix):
    n = len(matrix)
    array = []

    for j in range(n - 1, 0, -1):
        for i in range(0, n - j):
            array.append(matrix[i][j + i])

    for i in range(n):
        array.append(matrix[i][i])

    for i in range(1, n):
        for j in range(0, n - i):
            array.append(matrix[i + j][j])

    return array

def print_array(array):
    print(*array, sep=' ')
    
def main():
    n, m = input_sizes()
    
    if n == m:
        matrix = input_matrix(n)
        result = create_array(matrix)
        print_array(result)

main()