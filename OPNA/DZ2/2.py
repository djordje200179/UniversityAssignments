# %% [markdown]
# >> Neka je dat realni polinom _P(x)_ nad realnim segmentom _[a,b]_.
# >> 1. 1. Odrediti Euklidovim algoritmom najveći zajednički delilac _G(x) = GCD(P(x),P´(x))_.
# >>	  2. Za polinom _P(x):=P(x)/G(x)_, upotrebom Šturmove teoreme, odrediti broj nula na _[a,b]_.
# >> 2. Primena Šturmove teoreme.  
# >>	  Neka je _k_ broj decimala na koji se zaokružuje neki realan broj. Za polinom  
# >>	  _P(x) = a<sub>n</sub>x<sup>n</sup> + a<sub>n-1</sub>x<sup>n-1</sup> + ... + a<sub>1</sub>x + a<sub>0</sub>_,
# >>	  sa realnim koeficijentima  
# >>	  _a<sub>n</sub>,a<sub>n-1</sub>,...,a<sub>1</sub>,a<sub>0</sub>_  
# >>	  odrediti niz naniže zaokruženih racionalnih koeficijenata  
# >>	  _α<sub>n</sub>,α<sub>n-1</sub>,...,α<sub>1</sub>,α<sub>0</sub>_  
# >>	  određenih po sledećim pravilima:
# >>	  * ako je _a<sub>k</sub> = a<sub>0</sub>.a<sub>1</sub>...a<sub>k</sub>a<sub>k+1</sub> > 0_ tada _α<sub>k</sub> = a<sub>0</sub>.a<sub>1</sub>...a<sub>k</sub>_
# >>	  * ako je _a<sub>k</sub> = a<sub>0</sub>.a<sub>1</sub>...a<sub>k</sub>a<sub>k+1</sub> < 0_ tada _α<sub>k</sub> = a<sub>0</sub>.a<sub>1</sub>...a<sub>k</sub>´_, gdje je _a<sub>k</sub>´_ naviše zaokružena cifra (uz eventualno posledično zaokruživanje i prethodnih cifara za jedan broj naviše).
# >>
# >>	  Odrediti proceduru za formiranje racionalnog polinoma  
# >>	  _Ρ(x) = α<sub>n</sub>x<sup>n</sup> + α<sub>n-1</sub>x<sup>n-1</sup> + ... + α<sub>1</sub>x + α<sub>0</sub>_.
# >>
# >>	  Neka je _[a,b]_ segment sa racionalnim rubnim tačkama. Naći takav polinom _P(x)_ sa realnim koecijentima i broj _k_, da na osnovu pozitivnosti polinoma _Ρ(x)_ nad segmentom _[a,b]_ imamo dokaz o pozitivnosti polinoma _P(x)_ nad segmentom _[a,b]_.

# %% [markdown]
# Napisani kod ćemo testirati na primjeru polinoma
# _P(x) = x<sup>9</sup> - 3x<sup>7</sup> - x<sup>6</sup> + 3x<sup>5</sup> + 3x<sup>4</sup> - x<sup>3</sup> - 3x<sup>2</sup> + 1_
# i nule ćemo tražiti na segmentu _[0,3]_.

# %%
from numpy.polynomial.polynomial import Polynomial

a, b = 0, 3
p = Polynomial([1, 0, -3, -1, 3, 3, -1, -3, 0, 1]) # x^9 - 3x^7 - x^6 + 3x^5 + 3x^4 - x^3 - 3x^2 + 1
print(f"Ulazni polinom: {p}")

# %% [markdown]
# Prvi korak je uzimanje izvoda polinoma i traženje NZD-a. NZD računamo u funkciji _polynomial\_gcd_, koristeći Euklidov algoritam. Dok za računanje izvoda se koristi postojeća funkcija u _numpy_ biblioteci.
# 
# Zbog numeričkih grešaka prilikom rada sa realnim brojevima, može doći do situacije u kojoj je ostatak veoma mali, ali ne i nula. Zbog toga se u uslovu za nastavak dijeljenja poredi apsolutna vrednost koeficijenta uz najviši stepen polinoma.

# %%
def polynomial_gcd(a: Polynomial, b: Polynomial):
	if len(b) > len(a):
		a, b = b, a

	while abs(b.coef[-1]) > 1e-10:
		r = a % b
		a, b = b, r

	return a / a.coef[-1]

g = polynomial_gcd(p, p.deriv())
print(f"NZD: {g}")

# %% [markdown]
# Krećemo od prvog polinoma u Šturmov-ovom nizu (_P(x) / G(x)_), i od njega gradimo naredne polinome. Primjenjujemo Šturmov algoritam za generisanje novih polinoma u lancu.
# 
# Kada smo generisali sve polinome, možemo preći na brojanje promjena znaka u početnoj i krajnjoj tački intervala.
# Razlika između ta dva broja predstavlja broj nula u intervalu. To nam pomaže da odredimo da li je polinom strogo pozitivan/negativan.

# %%
def generate_sturms_chain(p: Polynomial):
	chain = [p, p.deriv()]

	while chain[-1].degree() > 0:
		chain.append(-(chain[-2] % chain[-1]))

	return chain

def sign_changes(chain: list[Polynomial], x: float):
	values = [p(x) for p in chain]

	changes = 0
	for i in range(1, len(values)):
		if values[i] * values[i - 1] < 0:
			changes += 1

	return changes

def calculate_number_of_zeroes(p: Polynomial, scope: tuple[float, float]):
	a, b = scope
	
	chain = generate_sturms_chain(p)

	print("Lanac polinoma: ")
	for p in chain:
		print('\t' + str(p))

	sign_changes_a = sign_changes(chain, a)
	sign_changes_b = sign_changes(chain, b)

	print(f"Promjena znaka u '{a}': {sign_changes_a}")
	print(f"Promjena znaka u '{b}': {sign_changes_b}")

	return sign_changes_a - sign_changes_b

zeros = calculate_number_of_zeroes(p // g, (a, b))
print("1) Broj nula:", zeros)

# %% [markdown]
# Za naredni dio zadatka uzimamo sledeću polinomsku funkciju i parametre:

# %%
from math import pi

a, b = 0, 1.35
p = Polynomial([
	0, 0, 0, 0, 
	2*pi/3 - 2, 
	-(pi**2)/60 + pi/30,
	-pi/30 + 1/10,
	-(pi**2)/1680+pi/840,
	pi/1260-1/420
])
print(f"Ulazni polinom: {p}")

# %% [markdown]
# Prvo je potrebno izvršiti zaokruživanje cifara po algoritmu datom u zadatku.

# %%
from math import floor, log10, ceil

def spec_round(x: float, k: int):
	if x == 0:
		return 0
	elif x > 0:
		exponent = floor(log10(x))
		mantissa = x / (10**exponent)

		mantissa = floor(mantissa * (10**k)) / (10**k)

		return mantissa * 10**exponent
	else:
		exponent = floor(log10(-x))
		mantissa = -x / (10**exponent)

		mantissa = ceil(mantissa * (10**k)) / (10**k)

		return -mantissa * 10**exponent
	
k = 2
p = Polynomial([spec_round(coef, k) for coef in p])
print(f"Sređeni polinom: {p}")

# %% [markdown]
# Gledajući vrijednosti funkcije na krajevima intervala, određujemo dozvoljeni broj nula na malo proširenom intervalu. Potom brojimo nule unutar intervala i utvrđujemo da li je funkcija strogo pozitivna.

# %%
allowed_zeros = 0
if p(a) == 0:
	a -= 0.1
	allowed_zeros += 1
if p(b) == 0:
	b += 0.1
	allowed_zeros += 1

print(f"Dozvoljeni broj nula: {allowed_zeros}")

# %%
zeros = calculate_number_of_zeroes(p, (a, b))
print(f"Broj nula na intervalu: {zeros}")

always_positive = zeros == allowed_zeros and p((a+b)/2) > 0
print(f"2) Polinom {'je' if always_positive else 'nije'} uvijek pozitivan")

# %% [markdown]
# S obzirom da je polinom uvijek pozitivan, možemo zaključiti i da je polazni polinom strogo pozitivan na datom intervalu. To implicira i da razmatrani polinom nema nula unutar istog intervala.


