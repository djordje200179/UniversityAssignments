import re

class FieldError(Exception): 
    # Proizvoljno kreiran tip greske koji se javlja ukoliko neki podatak nije u korektnom formatu
    pass

class Movie:
    # Struktura u kojoj cuvamo podatke o filmovima
    id: int = None
    title: str = None
    genres: list = None
    issued: str = None
    director: str = None
    revenue: str = None

    def __init__(self, id, title, genres, issued, director, revenue):
        self.id = id
        self.title = title
        self.genres = genres
        self.issued = issued
        self.director = director
        self.revenue = revenue

def read_data(file_path):
    #Funkcija koja cita dokument i kreira listu svih filmova sa svim podacima o njima
    with open(file_path, 'r') as file:
        all_data = []
            
        next(file)
        for line in file:
            raw_data = line.rstrip().split(';')
            raw_data[2] = raw_data[2].split('|')
                
            all_data.append(Movie(*raw_data))

    return all_data

def write_data(file_path, data):
    #Funkcija koja ispisuje u dokument podatke o reziserima i interesantnim zanrovima
    with open(file_path, 'w+') as file:
        print("Director;Movie Genre Count", file=file)

        for director in data:
            print("{};".format(director), end='', file=file)
            print('|'.join('{} : {}'.format(k, v) for k, v in data[director].items()), file=file)

def check_data(movies):
    #Funkcija koja provjerava validnost podataka (id-a, datuma i zarade)
    def check_id(id):
        try:
            int(id)
        except ValueError:
            raise FieldError

    def check_date(date):
        main_format = re.compile(r"^\w{2}.\w{2}.\w{4}$")
        main_match = main_format.match(date)
        if main_match is None:
            raise FieldError

        day, month, year = map(int, date.split('.'))
        
        month_dict = {2: 28, 4: 30, 6: 30, 9: 30, 11: 30}
        num_of_days = month_dict.get(month, 31)

        if month == 2:
            if year % 4 == 0:
                if year % 100 == 0:
                    if year % 400 == 0:
                        num_of_days = 29
                else:
                    num_of_days = 29

        if (month < 1 or month > 12) or (year < 1900 or year > 2020) or (day < 1 or day > num_of_days):
            raise FieldError

    def check_revenue(revenue):
        if revenue[0] != '$':
            raise FieldError

    for c_movie in movies:
        check_id(c_movie.id)
        check_date(c_movie.issued)
        check_revenue(c_movie.revenue)

def generate_director_objects(movies, genres):
    #Funkcija koja generise rijecnike u koje sacuvamamo brojace o interesantnim zanrovima
    director_names = []
    for c_movie in movies:
        if c_movie.director not in director_names:
            director_names.append(c_movie.director)

    counters = {genre: 0 for genre in sorted(genres)}
    directors = {director_name: counters.copy() for director_name in director_names}

    return directors

def update_movie_counters(movies, directors):
    #Funkcija koja broji filmove prema zanrovima za svakog rezisera 
    for c_movie in movies:
        for c_genre in c_movie.genres:
            director = directors[c_movie.director]

            if c_genre in director:
                director[c_genre] += 1

def main():
    #Glavna funkcija
    try:
        input_file, output_file = input().split()
        interesting_genres = input().split(',')
    
        data = read_data(input_file)
        check_data(data)
        directors = generate_director_objects(data, interesting_genres)
        update_movie_counters(data, directors)
        write_data(output_file, directors)
    except IOError:
        print("DAT_GRESKA")
    except ValueError:
        print("KONV_GRESKA")
    except FieldError:
        print("POLJE_GRESKA")
    except Exception:
        print("NEP_GRESKA")

main()