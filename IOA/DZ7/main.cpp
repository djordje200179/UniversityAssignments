#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <thread>
#include <string>
#include <fstream>

using namespace std;

constexpr int DISK_SIZE = 1 << 25;
constexpr int ANNEALING_ITERATIONS = 100'000;
constexpr int REANNEALING_ITERATIONS = 10;
constexpr int INDEPENDENT_EXECUTIONS = 20;
constexpr int INITIAL_TEMP = 64 * 1024 * 1024;
constexpr double TEMP_ADJUSTMENT_FACTOR = 0.95;

constexpr array<int, 64> FILE_SIZES = {
	173669, 275487, 1197613, 1549805, 502334, 217684, 1796841, 274708, 631252, 148665, 150254, 4784408, 344759, 440109, 4198037, 329673,
	28602, 144173, 1461469, 187895, 369313, 959307, 1482335, 2772513, 1313997, 254845, 486167, 2667146, 264004, 297223, 94694, 1757457,
	576203, 8577828, 498382, 8478177, 123575, 4062389, 3001419, 196884, 617991, 421056, 3017627, 131936, 1152730, 2676649, 656678, 4519834,
	201919, 56080, 2142553, 326263, 8172117, 2304253, 4761871, 205387, 6148422, 414559, 2893305, 2158562, 465972, 304078, 1841018, 1915571
};

using Point = array<int, FILE_SIZES.size()>;

constexpr int MAX_CHANGES = FILE_SIZES.size() / 4;
constexpr double CHANGES_FACTOR = double(1 - MAX_CHANGES) / (ANNEALING_ITERATIONS - 1);

int calculate_error(const Point& point) {
	int disk1 = 0, disk2 = 0;

	for (int i = 0; i < FILE_SIZES.size(); i++) {
		switch (point[i]) {
		case 1:
			disk1 += FILE_SIZES[i];
			break;
		case 2:
			disk2 += FILE_SIZES[i];
			break;
		}
	}

	if (disk1 > DISK_SIZE || disk2 > DISK_SIZE)
		return 2 * DISK_SIZE;

	return 2 * DISK_SIZE - disk1 - disk2;
}

struct AnnealingResult {
	array<int, 64> point;
	int error;
};

AnnealingResult generation_simulation(const Point& initial_point, int* errors_memory) {
	AnnealingResult result;
	result.point = initial_point;
	result.error = calculate_error(initial_point);

	double curr_temp = INITIAL_TEMP;
	Point curr_point = result.point;
	int curr_error = result.error;

	for (int i = 0; i < ANNEALING_ITERATIONS; i++, curr_temp *= TEMP_ADJUSTMENT_FACTOR) {
		Point next_point = curr_point;
		int changes = CHANGES_FACTOR * i + MAX_CHANGES;

		static thread_local std::mt19937 rng(std::random_device{}());
		uniform_int_distribution<int> index_generator(0, FILE_SIZES.size() - 1);
		uniform_int_distribution<int> value_generator(0, 2);

		for (int j = 0; j < changes; j++)
			next_point[index_generator(rng)] = value_generator(rng);

		int next_error = calculate_error(next_point);

		uniform_real_distribution<double> probability_generator;

		if (next_error < curr_error || 
			probability_generator(rng) <= (
				next_error > curr_error ? 
				exp((curr_error - next_error) / curr_temp) : 
				0.5
			)
		) {
			curr_point = next_point;
			curr_error = next_error;

			if (curr_error < result.error) {
				result.point = curr_point;
				result.error = curr_error;
			}
		}

		errors_memory[i] = result.error;
	}

	return result;
}

struct EvolutionResult {
	Point point;
	int error;
	array<int, REANNEALING_ITERATIONS* ANNEALING_ITERATIONS> errors;
};

void evolution_cycle(EvolutionResult& result) {
	result.point = array<int, 64>();

	auto pid = this_thread::get_id();

	for (int i = 0; i < REANNEALING_ITERATIONS; i++) {
		int* errors_memory = result.errors.data() + i * ANNEALING_ITERATIONS;
		auto annealing_result = generation_simulation(result.point, result.errors.data() + i * ANNEALING_ITERATIONS);

		result.point = annealing_result.point;
		result.error = annealing_result.error;
	}
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

	const EvolutionResult* best_result = &results[0];
	for (const auto& result : results) {
		if (result.error < best_result->error)
			best_result = &result;
	}

	cout << "best error: " << best_result->error << endl;

	cout << "best point: ";
	for (auto p : best_result->point)
		cout << p << " ";
	cout << endl;

	for (int i = 0; i < INDEPENDENT_EXECUTIONS; i++) {
		auto file_name = "errors_" + to_string(i) + ".bat";
		ofstream file(file_name, ios::out | ios::binary);

		auto data_start = reinterpret_cast<const char*>(results[i].errors.data());
		auto data_size = best_result->errors.size() * sizeof(int);
		file.write(data_start, data_size);
	}

	return 0;
}