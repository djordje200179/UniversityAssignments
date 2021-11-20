def obrada(rijeci):
    nova = ''

    for rijec in rijeci:
        najduza = ''
        privremena = ''

        for slovo in rijec:
            if slovo.isalpha() and slovo.islower():
                privremena += slovo
            else:
                if len(privremena) >= len(najduza):
                    najduza = privremena

                privremena = ''

        if len(privremena) >= len(najduza):
            najduza = privremena

        nova += najduza

    return nova

def unos():
    return input().split('-')

strings = unos()
flag = False
for string in strings:
    for char in string:
        if not char.isalnum():
            flag = True

if flag == False:
    nova_rijec = obrada(strings)
    print(nova_rijec, end='')