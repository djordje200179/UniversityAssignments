# %% [markdown]
# >> Neka je dat pozitivan realan broj _x_ sa konačnim decimalskim zapisom i neka su dati prirodni brojevi _n_ i _m_, tako da _n<m_. Formirati niz razlomaka _p/q_ takvih da za imenilac _q_ važi _n<=q<=m_ (tj. _q=n,n+1,...,m_) i pri tom imeniocu _q_ pridružujemo brojilac _p_ koji određujemo zaokruživanjem na najbliži priodan broj proizvoda _x⋅q_. Predstaviti svaki razlomak _p/q_ u obliku verižnog razlomaka. U nizu razlomaka _p/q_ izdvojiti:
# >> * najbolje racionalne aproksimacije I vrste,
# >> * najbolje racionalne aproksimacije II vrste,
# >> * sortirati sve razlomke _p/q_ po uslovu minimalnosti apsolutne greske _|x-p/q|_.

# %% [markdown]
# Napisani kod ćemo testirati na primjeru konstante _v=log₂(3)-1=0.5849625007211561815..._, a za parametre _n_ i _m_ uzeti brojeve 7 i 53 respektivno.

# %%
from math import log

v = log(3, 2) - 1
n, m = 7, 53

# %% [markdown]
# Prvi korak je pisanje pomoćnih funkcija, koje će olakšati generisanje verižnog oblika razlomaka i utvrđivanje "kvaliteta" razlomaka.
# 
# Prva funkcija (_generate_continued_fraction_) će generisati verižni oblik razlomka za dato _p_ i _q_.
# Naredne dvije funkcije (_is_fst_deg_best_ i _is_snd_deg_best_) utvrđuju da li se radi o najboljoj racionalnoj aproksimaciji I i II vrste.

# %%
def generate_simple_continued_fraction(p: int, q: int) -> list[int]:
	a = [0]
	a[0], p = divmod(p, q)

	while p != 0:
		quotient, remainder = divmod(q, p)
		a.append(quotient)
		p, q = remainder, p

	return a


def is_fst_deg_best(x: float, p: int, q: int) -> bool:
	for s in range(1, q):
		r = round(s * x)
		if r / s == p / q:
			continue

		if abs(x - r / s) < abs(x - p / q):
			return False

	return True

def is_snd_deg_best(x: float, p: int, q: int) -> bool:
	for s in range(1, q):
		r = round(s * x)
		if r / s == p / q:
			continue

		if abs(s * x - r) < abs(q * x - p):
			return False

	return True

# %% [markdown]
# Nakon pisanja pomoćnih funkcija, možemo preći na dio koda koji prolazi kroz svako moguće _q_ i generiše razlomke. A usput, za svaki razlomak, utvrđujemo da li se radi o najboljoj racionalnoj aproksimaciji.
# 
# Sve razlomke ćemo čuvati u listi, kako bismo ih mogli kasnije i sortirati i ispisati.

# %%
from dataclasses import dataclass

@dataclass(order=True)
class RationalApproximation:
	p: int
	q: int
	a: list[int]

	first_degree: bool
	second_degree: bool

	def __float__(self) -> float:
		return self.p / self.q

fractions: list[RationalApproximation] = []
for q in range(n, m + 1):
	p = round(q * v)

	a = generate_simple_continued_fraction(p, q)
	first_degree = is_fst_deg_best(v, p, q)
	second_degree = is_snd_deg_best(v, p, q) if first_degree else False

	fractions.append(RationalApproximation(
		p, q,
		a, first_degree, second_degree
	))

# %% [markdown]
# Potom je potrebno isfiltrirati i sortirati razlomke. Prema tekstu zadatka, sortiranje se vrši po rastućoj vrijednosti apsolutne greške. Dok, kada je riječ o filtriranju, potrebno je ukloniti razlomke koji su u suštini isti, tj. one koji se pojavljuju i u nekom skraćenom obliku (npr. _7/12_ i _14/24_).

# %%
def simplify_fraction(p: int, q: int) -> tuple[int, int]:
    i = 2
    while i < min(p, q) + 1:
        if p % i == 0 and q % i == 0:
            p = p // i
            q = q // i
        else:
            i += 1
    return p, q

unique_fractions: dict[tuple[int, int], list[RationalApproximation]] = {}
for frac in fractions:
    simplified = simplify_fraction(frac.p, frac.q)
    if simplified not in unique_fractions:
        unique_fractions[simplified] = []
    unique_fractions[simplified].append(frac)

fractions = [min(group) for group in unique_fractions.values()]
fractions.sort(key=lambda r: abs(r.p / r.q - v))

# %% [markdown]
# Ssada imamo sve informacije da ispišemo traženu tabelu. Svaki razlomak je napisan u 3 oblika:
# 1. kao razlomak
# 2. kao realan broj
# 3. kroz verižni zapis
# 
# Različite boje određuju "kvalitet" aproksimacije:
# - **Crvena** boja - Najbolja racionalna aproksimacija I vrste
# - **Plava** boja - Najbolja racionalna aproksimacija II vrste
# - **Bijela** boja - Ostali razlomci

# %%
from termcolor import colored

def format_simple_continued_fraction(a: list[int]) -> str:
	if len(a) == 1:
		return f"[{a[0]}]"

	return f"[{a[0]}; {', '.join(map(str, a[1:]))}]"

for fraction in fractions:
	if fraction.second_degree:
		color = "red"
	elif fraction.first_degree:
		color = "blue"
	else:
		color = "light_grey"

	ratio_format = f"{fraction.p}/{fraction.q}"
	real_format = f"{float(fraction):.9f}"
	difference = f"{v - float(fraction):+.9f}"
	array_format = format_simple_continued_fraction(fraction.a)

	text = f"{ratio_format} = {real_format} ({difference}), {array_format}"

	print(colored(text, color))


