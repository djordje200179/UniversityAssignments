def loading(dat_ulaz, dat_izlaz, year1, year2):
    numbers = []
    movies = []
    genres = []
    directors = []
    profits = []
    dates = []

    with open(dat_ulaz, 'r') as input:
        first_line = True
        for row in input:
            if first_line == True:
                first_line = False
                continue

            line = row.rstrip().split(';')
                
            numbers.append(line[0])
            movies.append(line[1])
            genres.append(line[2].split('|'))
            directors.append(line[4])
            profits.append(line[5][1:])
            dates.append(line[3].split('.')[2])

    with open(dat_izlaz, 'w+') as output:
        print("Year;Movie Genre;Genre Revenue", file=output)

        for year in range(year1, year2 + 1):
            dictionary = {}
            
            for curr in range(len(movies)):
                if int(dates[curr]) == year:
                    for i in genres[curr]:
                        if i not in dictionary:
                            dictionary[i] = 0
                        
                        dictionary[i] += float(profits[curr])
                    pass

            if len(dictionary) == 0:
                continue
            
            all_values = dictionary.values()
            min_value = min(all_values)
            
            suitable_genres = []
            for curr in range(len(movies)):
                for i in genres[curr]:
                    if i in dictionary.keys() and dictionary[i] == min_value:
                        if i not in suitable_genres:
                            suitable_genres.append(i)

            print(str(year), end='', file=output)
            print(';', end='', file=output)
            print(*suitable_genres, sep='|', end='', file=output)
            print(';', end='', file=output)
            print(min_value, file=output)  

input1, input2 = input().split()

year1 = int(input())
year2 = int(input())
work = loading(input1, input2, year1, year2)