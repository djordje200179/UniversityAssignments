#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <thread>
#include <string>
#include <fstream>

using namespace std;

constexpr int POPULATION_SIZE = 20'000;
constexpr int PARENTS_SIZE = 1'000;
constexpr int GENERATIONS = 50;
constexpr double CROSSOVER_PROBABILITY = 0.8;
constexpr double MUTATION_PROBABILITY = 0.1;
constexpr int INDEPENDENT_EXECUTIONS = 20;

struct Point : array<int, 64> {
	int error = -1;

	Point() {}

	static Point create_random(mt19937& rng) {
		uniform_int_distribution<int> value_generator(0, 2);

		Point point;
		for (int i = 0; i < point.size(); i++)
			point[i] = value_generator(rng);

		return point;
	}

	static Point combine(const Point& point1, const Point& point2, mt19937& rng) {
		Point child;

		uniform_int_distribution<int> split_point_generator(0, point1.size() - 1);
		auto split_point = split_point_generator(rng);

		copy(point1.begin(), point1.begin() + split_point, child.begin());
		copy(point2.begin() + split_point, point2.end(), child.begin() + split_point);

		return child;
	}

	void mutate(mt19937& rng) {
		uniform_int_distribution<int> value_generator(0, 2);
		uniform_int_distribution<int> index_chooser(0, size() - 1);

		(*this)[index_chooser(rng)] = value_generator(rng);
	}

	friend ostream& operator<<(ostream& os, const Point& point) {
		bool first = true;
		for (auto value : point) {
			if (!first)
				os << " ";

			os << value;
			first = false;
		}

		return os;
	}

	friend bool operator<(const Point& point1, const Point& point2) {
		return point1.error < point2.error;
	}
};

int calculate_error(const Point& point) {
	static constexpr array<int, 64> file_sizes = {
		173669, 275487, 1197613, 1549805, 502334, 217684, 1796841, 274708, 631252, 148665, 150254, 4784408, 344759, 440109, 4198037, 329673,
		28602, 144173, 1461469, 187895, 369313, 959307, 1482335, 2772513, 1313997, 254845, 486167, 2667146, 264004, 297223, 94694, 1757457,
		576203, 8577828, 498382, 8478177, 123575, 4062389, 3001419, 196884, 617991, 421056, 3017627, 131936, 1152730, 2676649, 656678, 4519834,
		201919, 56080, 2142553, 326263, 8172117, 2304253, 4761871, 205387, 6148422, 414559, 2893305, 2158562, 465972, 304078, 1841018, 1915571
	};

	static constexpr int DISK_SIZE = 1 << 25;

	int disk1 = DISK_SIZE, disk2 = DISK_SIZE;

	for (int i = 0; i < file_sizes.size(); i++) {
		switch (point[i]) {
		case 1:
			disk1 -= file_sizes[i];
			break;
		case 2:
			disk2 -= file_sizes[i];
			break;
		}
	}

	if (disk1 < 0 || disk2 < 0)
		return 2 * DISK_SIZE;

	return disk1 + disk2;
}

struct EvolutionResult {
	Point point;
	array<int, POPULATION_SIZE * GENERATIONS> errors;

	friend bool operator<(const EvolutionResult& result1, const EvolutionResult& result2) {
		return result1.point.error < result2.point.error;
	}

	void write_errors(ostream& os) const {
		auto data_start = reinterpret_cast<const char*>(errors.data());
		auto data_size = errors.size() * sizeof(int);

		os.write(data_start, data_size);
	}
};

void evolution_cycle(EvolutionResult& result) {
	mt19937 rng(random_device{}());
	uniform_int_distribution<int> individual_chooser(0, POPULATION_SIZE - 1);
	uniform_int_distribution<int> parent_chooser(0, PARENTS_SIZE - 1);
	uniform_real_distribution<double> probability_generator;

	vector<Point> population(POPULATION_SIZE);
	for (auto& point : population)
		point = Point::create_random(rng);

	Point best_point;

	for (int gen_i = 0; gen_i < GENERATIONS; gen_i++) {
		for (int ind_i = 0; ind_i < POPULATION_SIZE; ind_i++) {
			population[ind_i].error = calculate_error(population[ind_i]);
			if (best_point.error == -1 || population[ind_i].error < best_point.error)
				best_point = population[ind_i];

			result.errors[gen_i * POPULATION_SIZE + ind_i] = best_point.error;
		}

		/*vector<Point> parents;
		parents.reserve(PARENTS_SIZE);

		while (parents.size() < PARENTS_SIZE) {
			int ind1 = individual_chooser(rng);
			int ind2 = individual_chooser(rng);

			parents.push_back(fitnesses[ind1] < fitnesses[ind2] ? population[ind1] : population[ind2]);
		}*/

		partial_sort(population.begin(), population.begin() + PARENTS_SIZE, population.end());
		vector<Point> parents(population.begin(), population.begin() + PARENTS_SIZE);

		population.clear();
		while (population.size() < POPULATION_SIZE) {
			int ind1 = parent_chooser(rng);
			int ind2 = parent_chooser(rng);

			auto child = Point::combine(parents[ind1], parents[ind2], rng);
			if (probability_generator(rng) < MUTATION_PROBABILITY)
				child.mutate(rng);

			population.push_back(child);
		}
	}

	result.point = best_point;
}

int main() {
	array<thread, INDEPENDENT_EXECUTIONS> threads;
	vector<EvolutionResult> results(INDEPENDENT_EXECUTIONS);
	for (int i = 0; i < INDEPENDENT_EXECUTIONS; i++) {
		threads[i] = thread(evolution_cycle, ref(results[i]));
		cout << "proc #" << threads[i].get_id() << ":\tstarted" << endl;
	}

	for (auto& thread : threads) {
		auto pid = thread.get_id();
		thread.join();

		cout << "proc #" << pid << ":\tfinished" << endl;
	}

	auto best_result = min_element(results.begin(), results.end());

	cout << "best error: " << best_result->point.error << endl;
	cout << "best point: " << best_result->point << endl;

	for (int i = 0; i < INDEPENDENT_EXECUTIONS; i++) {
		auto file_name = "errors_" + to_string(i) + ".bat";
		ofstream file(file_name, ios::out | ios::binary);
		if (!file.is_open()) {
			cerr << "failed to open file " << file_name << endl;
			continue;
		}

		results[i].write_errors(file);

		file.close();
	}

	return 0;
}