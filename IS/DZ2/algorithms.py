class Algorithm:
	def get_algorithm_steps(self, tiles, variables, words):
		pass


class ExampleAlgorithm(Algorithm):
	def get_algorithm_steps(self, tiles, variables, words):
		moves_list = [
			['0h', 0], ['0v', 2], ['1v', 1], ['2h', 1], ['4h', None],
			['2h', None], ['1v', None], ['0v', 3], ['1v', 1], ['2h', 1],
			['4h', 4], ['5v', 5]
		]

		domains = {var: [word for word in words] for var in variables}
		solution = []
		for move in moves_list:
			solution.append([move[0], move[1], domains])
		return solution


class Backtracking(Algorithm):
	@staticmethod
	def _get_domains(tiles: list[list[bool]], variables: list[str], words: list[str]) -> dict[str, list[str]]:
		height, width = len(tiles), len(tiles[0])

		def get_domain(var: str) -> list[str]:
			position, direction = var
			position = int(position)

			start_y, start_x = position // width, position % width

			var_length = 1
			if direction == 'h':
				while start_x + var_length < len(tiles[0]) and not tiles[start_y][start_x + var_length]:
					var_length += 1
			else:
				while start_y + var_length < len(tiles) and not tiles[start_y + var_length][start_x]:
					var_length += 1

			return [word for word in words if len(word) == var_length]

		return {var: get_domain(var) for var in variables}

	@staticmethod
	def __read_var(width: int, var: str) -> (int, int, str):
		position, direction = var
		position = int(position)

		y, x = position // width, position % width

		return y, x, direction

	@staticmethod
	def _generate_matrix(tiles: list[list[bool]], filled: dict[str, str | None]) -> list[list[str | None]]:
		height, width = len(tiles), len(tiles[0])
		matrix: list[list[str | None]] = [[None for _ in range(width)] for _ in range(height)]

		for var, word in filled.items():
			if word is None:
				continue

			y, x, direction = Backtracking.__read_var(width, var)

			for letter in word:
				matrix[y][x] = letter

				if direction == 'h':
					x += 1
				else:
					y += 1

		return matrix

	@staticmethod
	def _can_fit_word(matrix: list[list[str | None]], word: str, var: str) -> bool:
		y, x, direction = Backtracking.__read_var(len(matrix[0]), var)

		for letter in word:
			if matrix[y][x] is not None and matrix[y][x] != letter:
				return False

			if direction == 'h':
				x += 1
			else:
				y += 1

		return True

	def get_algorithm_steps(self, tiles: list[list[bool]], variables: list[str], words: list[str]):
		domains: dict[str, list[str]] = Backtracking._get_domains(tiles, variables, words)
		filled = {var: None for var in variables}
		steps: list[(str, int | None, dict[str, list[str]])] = []

		def backtrack():
			if all(filled.values()):
				return True

			var = next(var for var in variables if filled[var] is None)
			for index, word in enumerate(domains[var]):
				matrix = Backtracking._generate_matrix(tiles, filled)
				if not Backtracking._can_fit_word(matrix, word, var):
					continue

				filled[var] = word
				steps.append((var, index, domains))

				if backtrack():
					return True
				else:
					filled[var] = None

			steps.append((var, None, domains))
			return False

		backtrack()

		return steps


class ForwardChecking(Backtracking):
	@staticmethod
	def __get_domains(tiles: list[list[bool]], filled: dict[str, None | str], original_domains: dict[str, list[str]]) -> dict[str, list[str]]:
		matrix = Backtracking._generate_matrix(tiles, filled)

		return {
			var: [
				word for word in original_domains[var] if Backtracking._can_fit_word(matrix, word, var)
			] for var in original_domains
		}

	def get_algorithm_steps(self, tiles: list[list[bool]], variables: list[str], words: list[str]):
		original_domains: dict[str, list[str]] = Backtracking._get_domains(tiles, variables, words)
		filled: dict[str, None | str] = {var: None for var in variables}
		steps: list[(str, int | None, dict[str, list[str]])] = []

		def backtrack(domains: dict[str, list[str]]):
			if all(filled.values()):
				return True

			var = next(var for var in variables if filled[var] is None)

			for index, word in enumerate(domains[var]):
				filled[var] = word
				steps.append((var, index, domains))

				new_domains = ForwardChecking.__get_domains(tiles, filled, domains)
				if backtrack(new_domains):
					return True
				else:
					filled[var] = None

			steps.append((var, None, domains))
			return False

		backtrack(original_domains)

		return steps


class ArcConsistency(ForwardChecking):
	def get_algorithm_steps(self, tiles: list[list[bool]], variables: list[str], words: list[str]) -> list[list]:
		pass
