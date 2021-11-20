def simplify(expression):
    '''
        Problem sam rjesio tako sto gledam "segmente" i cuvam sledece informacije o njima:
            1) Pozicije zagrada
            2) Operacije izmedju zagrada
            3) Znakove prije i posle zagrada (ukoliko postoje)

        Za cuvanje tih informacija mogu se koristiti torke, ali sam odlucio da upotrijebim
        klase radi bolje organizacije i iz tog razloga sam napravio sledecu klasu 'term'.
    '''

    class term:
        opening_bracket_index = None
        closing_bracket_index = None

        operations = []

        previous_operation = None
        next_operation = None

        should_remove = None

        def __init__(self, o_b_i, c_b_i, op, p_op, n_op):
            self.opening_bracket_index = o_b_i
            self.closing_bracket_index = c_b_i
            self.operations = op
            self.previous_operation = p_op
            self.next_operation = n_op

    n = len(expression)
    
    terms = []

    last_opened = None
    op = []
    prev_op = None
    next_op = None
    for i in range(n):
        if expression[i] == '(':
            if i != 0:
                prev_op = expression[i - 1]

            last_opened = i
        elif expression[i] in {'+', '-', '*', '/'}:
            if last_opened == None:
                prev_op = expression[i]
            else:
                op.append(expression[i])
        elif expression[i] == ')':
            if i != n - 1:
                next_op = expression[i + 1]

            terms.append(term(last_opened, i, op, prev_op, next_op))
            
            last_opened = None
            op = []
            prev_op = None
            next_op = None

    for c_term in terms:
        every_higher_order = all(operation in {'/', '*'} for operation in c_term.operations)

        if every_higher_order and c_term.previous_operation != '/':
            c_term.should_remove = True
        else:
            c_term.should_remove = c_term.previous_operation in {None, '+'} and c_term.next_operation in {None, '+', '-'}
        
    def remove_character(string, position):
        return string[:position] + string[position + 1:]

    for c_term in reversed(terms):
        if c_term.should_remove:
            expression = remove_character(expression, c_term.closing_bracket_index)
            expression = remove_character(expression, c_term.opening_bracket_index)

    return expression

def main():
    expression = input()
    simpler_expression = simplify(expression)
    print(simpler_expression, end='')

main()