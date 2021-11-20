def citanje(ulaz):
    film_ime = []
    film_zanr = []
    film_reziser = []

    with open(ulaz, 'r') as file:
        ignore = True
        
        for line in file:
            if ignore:
                ignore = False
                continue

            podaci = line.split(';')
            
            zanr = podaci[2]
            zanr = zanr.split('|')
            
            film_zanr.append(zanr)
            film_ime.append(podaci[1])
            film_reziser.append(podaci[4])

    return film_ime, film_zanr, film_reziser

def ispis(izlaz, podaci):
    with open(izlaz, 'w+') as file:
        file.write("Director;Movie Genre Count\n")

        for reziser in podaci.keys():
            file.write("{};".format(reziser))

            n = len(podaci[reziser])
            cnt = 0
            for zanr in podaci[reziser]:
                cnt += 1
                
                if cnt != n:
                    file.write("{} : {}|".format(zanr, podaci[reziser][zanr]))
                else:
                    file.write("{} : {}".format(zanr, podaci[reziser][zanr]))

            file.write('\n')


def brojac(film_ime, film_zanr, film_reziser, zanrovi):
    reziseri = []
    for reziser in film_reziser:
        if reziser not in reziseri:
            reziseri.append(reziser)

    dict = {reziser : {zanr : 0 for zanr in zanrovi} for reziser in reziseri}
   
    for i in range(len(film_ime)):
        for zanr in film_zanr[i]:
            if zanr in zanrovi:
                dict[film_reziser[i]][zanr] += 1

    return dict

try:
    ulaz, izlaz = input().split()
    zanrovi = input().split(',')
    zanrovi.sort()

    podaci = brojac(*citanje(ulaz), zanrovi)
    ispis(izlaz, podaci)
except:
    print('DAT_GRESKA')