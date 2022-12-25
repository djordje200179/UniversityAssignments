TileMatrix = list[list[bool]]
CharMatrix = list[list[str | None]]
Variable = str
VariableLengths = dict[Variable, int]
VariableValues = dict[Variable, str | None]
Words = list[str]
Domain = list[str]
Domains = dict[Variable, Domain]
Step = (Variable, int | None, Domains)
Solution = list[Step]


class Algorithm:
	@staticmethod
	def _read_var(matrix_width: int, var: Variable) -> (int, int, str):
		position = int(var[:-1])
		direction = var[-1]

		y, x = position // matrix_width, position % matrix_width

		return y, x, direction

	@staticmethod
	def _generate_matrix(tiles: TileMatrix, filled: VariableValues) -> CharMatrix:
		height, width = len(tiles), len(tiles[0])
		matrix: CharMatrix = [[None] * width for _ in range(height)]

		for var, word in filled.items():
			if word is None:
				continue

			y, x, direction = Algorithm._read_var(width, var)

			for letter in word:
				matrix[y][x] = letter

				if direction == 'h':
					x += 1
				else:
					y += 1

		return matrix

	@staticmethod
	def _can_fit_word(matrix: CharMatrix, word: str, var: Variable) -> bool:
		y, x, direction = Algorithm._read_var(len(matrix[0]), var)

		for letter in word:
			if matrix[y][x] is not None and matrix[y][x] != letter:
				return False

			if direction == 'h':
				x += 1
			else:
				y += 1

		return True

	def get_algorithm_steps(self, tiles: TileMatrix, variables: VariableLengths, words: Words) -> Solution:
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
	def _preprocess_domains(self, tiles: TileMatrix, variables: VariableLengths, words: Words) -> Domains:
		return {
			var: [
				word for word in words if len(word) == var_len
			] for var, var_len in variables.items()
		}

	def _is_state_valid(self, tiles: TileMatrix, filled: VariableValues, var: Variable, word: str):
		matrix = Algorithm._generate_matrix(tiles, filled)
		return Algorithm._can_fit_word(matrix, word, var)

	def _get_new_domains(self, tiles: TileMatrix, filled: VariableValues, original_domains: Domains) -> Domains:
		return original_domains

	def get_algorithm_steps(self, tiles: TileMatrix, variables: VariableLengths, words: Words) -> Solution:
		preprocessed_domains = self._preprocess_domains(tiles, variables, words)

		filled: VariableValues = {var: None for var in variables}
		steps: Solution = []

		def backtrack_recursive(domains: Domains):
			if all(filled.values()):
				return True

			var = next(var for var in variables if filled[var] is None)
			for index, word in enumerate(domains[var]):
				if not self._is_state_valid(tiles, filled, var, word):
					continue

				filled[var] = word
				steps.append((var, index, domains))

				new_domains = self._get_new_domains(tiles, filled, domains)
				if [] not in new_domains.values() and backtrack_recursive(new_domains):
					return True
				else:
					filled[var] = None

			steps.append((var, None, domains))
			return False

		backtrack_recursive(preprocessed_domains)

		return steps


class ForwardChecking(Backtracking):
	def _is_state_valid(self, tiles: TileMatrix, filled: VariableValues, var: Variable, word: str):
		return True

	def _get_new_domains(self, tiles: TileMatrix, filled: VariableValues, original_domains: Domains) -> Domains:
		matrix = Algorithm._generate_matrix(tiles, filled)

		return {
			var: [
				word for word in domain if Algorithm._can_fit_word(matrix, word, var)
			] for var, domain in original_domains.items()
		}


class ArcConsistency(ForwardChecking):
	@staticmethod
	def __are_vars_dependent(matrix_width: int, var1: Variable, var2: Variable, len1: int, len2: int):
		y1, x1, dir1 = Algorithm._read_var(matrix_width, var1)
		y2, x2, dir2 = Algorithm._read_var(matrix_width, var2)

		if x1 == x2 and y1 == y2:
			return True

		if dir1 == dir2:
			return False

		if dir1 == 'h':
			return x1 <= x2 < x1 + len1 and y2 <= y1 < y2 + len2
		else:
			return x2 <= x1 < x2 + len2 and y1 <= y2 < y1 + len1

	@staticmethod
	def __get_var_dependencies(matrix_width: int, variables: VariableLengths, var: str):
		return [
			other_var
			for other_var, other_var_len in variables.items()
			if var != other_var and ArcConsistency.__are_vars_dependent(matrix_width, var, other_var, variables[var], other_var_len)
		]

	@staticmethod
	def __are_words_compatible(matrix_width: int, var1: Variable, var2: Variable, word1: str, word2: str):
		y1, x1, dir1 = Algorithm._read_var(matrix_width, var1)
		y2, x2, dir2 = Algorithm._read_var(matrix_width, var2)

		if dir1 == 'h':
			return word1[x2-x1] == word2[y1-y2]
		else:
			return word1[y2-y1] == word2[x1-x2]

	def _preprocess_domains(self, tiles: TileMatrix, variables: VariableLengths, words: Words) -> Domains:
		matrix_height, matrix_width = len(tiles), len(tiles[0])

		domains = super()._preprocess_domains(tiles, variables, words)
		var_dependencies = {var: ArcConsistency.__get_var_dependencies(matrix_width, variables, var) for var in variables}

		arc_list = [(var1, var2) for var1 in var_dependencies for var2 in var_dependencies[var1]]
		while arc_list:
			var1, var2 = arc_list.pop(0)

			var1_words_to_del = []
			for var1_word in domains[var1]:
				var2_no_word = True

				for var2_word in domains[var2]:
					if ArcConsistency.__are_words_compatible(matrix_width, var1, var2, var1_word, var2_word):
						var2_no_word = False
						break

				if var2_no_word:
					var1_words_to_del.append(var1_word)

			if var1_words_to_del:
				domains[var1] = [word for word in domains[var1] if word not in var1_words_to_del]

				if not domains[var1]:
					return None

				for var in var_dependencies[var1]:
					arc_list.append((var, var1))

		return domains
