#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;

struct Point {
	float x, y;
	int index;

	static float distance(const Point p1, const Point p2) {
		return hypot(p1.x - p2.x, p1.y - p2.y);
	}

	friend inline bool operator<(const Point p1, const Point p2) {
		return p1.index < p2.index;
	}

	friend inline bool operator==(const Point p1, const Point p2) {
		return p1.index == p2.index;
	}

	friend inline ostream& operator<<(ostream& os, const Point point) {
		return os << point.index << '(' << point.x << ',' << point.y << ')';
	}
};

Point points[]{
	Point {2.7, 33.1, 1},
	Point {2.7, 56.8, 2},
	Point {9.1, 40.3, 3},
	Point {9.1, 52.8, 4},
	Point {15.1, 49.6, 5},
	Point {15.3, 37.8, 6},
	Point {21.5, 45.8, 7},
	Point {22.9, 32.7, 8},
	Point {33.4, 60.5, 9},
	Point {28.4, 31.7, 10},
	Point {34.7, 26.4, 11},
	Point {45.7, 25.1, 12},
	Point {34.7, 45.1, 13},
	Point {46.0, 45.1, 14},
	Point {54.2, 29.1, 15},
	Point {57.7, 42.1, 16},
	Point {67.9, 19.6, 17},
	Point {51.7, 56.0, 18},
	Point {57.5, 56.0, 19},
	Point {62.0, 58.4, 20}
};

class Timer {
public:
	~Timer() {
		auto end_time = system_clock::now();
		auto ms = duration_cast<milliseconds>(end_time - start_time).count();
		auto s = ms / 1000.0f;

		cout << "Elapsed time: " << s << 's' << endl;
	}

private:
	time_point<system_clock> start_time = system_clock::now();
};

template <int N>
struct Solution {
	float distance = numeric_limits<float>::max();
	Point path[N];

	friend ostream& operator<<(ostream& os, const Solution& solution) {
		os << "Distance: " << solution.distance << endl;

		for (int i = 0; i < N; i++) {
			if (i != 0)
				os << " -> ";

			os << solution.path[i];
		}

		os << endl;

		return os;
	}
};

template <int N>
Solution<N> find_best() {
	Timer timer;

	sort(points, points + N);

	float distances[N * N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			distances[i * N + j] = Point::distance(points[i], points[j]);

	Solution<N> solution;

	do {
		auto curr_distance = 0.0f;
		for (int i = 0; i < N - 1; i++) {
			auto curr_point_index = points[i].index - 1;
			auto next_point_index = points[i + 1].index - 1;

			curr_distance += distances[curr_point_index * N + next_point_index];
			if (curr_distance > solution.distance)
				break;
		}

		if (curr_distance < solution.distance) {
			solution.distance = curr_distance;
			copy(points, points + N, solution.path);
		}
	} while (next_permutation(points, points + N));

	return solution;
}

int main() {
	cout << fixed << setprecision(1);

	cout << find_best<8>() << endl;
	cout << find_best<12>() << endl;
}