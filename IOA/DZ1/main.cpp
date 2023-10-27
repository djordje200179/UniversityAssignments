#include <iostream>
#include <chrono>
#include <functional>

using namespace std;

#define PRICE 7.77

void full_4_param_search() {
	const auto target_sum = (int)(PRICE * 100), 
			   target_product = (int)(PRICE * 100 * 100 * 100 * 100),
			   max_price = target_sum;

	for (int p0 = 1; p0 < max_price; p0++) {
		for (int p1 = p0; p1 < max_price; p1++) {
			for (int p2 = p1; p2 < max_price; p2++) {
				for (int p3 = p2; p3 < max_price; p3++) {
					if (p0 + p1 + p2 + p3 != target_sum)
						continue;

					if (p0 * p1 * p2 * p3 != target_product)
						continue;

					cout << p0 / 100.0 << " " << p1 / 100.0 << " " << p2 / 100.0 << " " << p3 / 100.0 << endl;
					return;
				}
			}
		}
	}
}

void full_3_param_search() {
	const auto target_sum = (int)(PRICE * 100),
			   target_product = (int)(PRICE * 100 * 100 * 100 * 100),
			   max_price = target_sum;

	for (int p0 = 1; p0 < max_price; p0++) {
		for (int p1 = p0; p1 < max_price; p1++) {
			for (int p2 = p1; p2 < max_price; p2++) {
				int p3 = target_sum - p0 - p1 - p2;

				if (p0 * p1 * p2 * p3 != target_product)
					continue;

				cout << p0 / 100.0 << " " << p1 / 100.0 << " " << p2 / 100.0 << " " << p3 / 100.0 << endl;
				return;
			}
		}
	}
}

void measure_execution_time(std::function<void()> func) {
	using namespace std::chrono;

	auto start = chrono::high_resolution_clock::now();
	func();
	auto end = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Execution time: " << duration.count() / 1000000.0 << "s" << endl;
}

int main() {
	measure_execution_time(full_4_param_search);
	measure_execution_time(full_3_param_search);
}