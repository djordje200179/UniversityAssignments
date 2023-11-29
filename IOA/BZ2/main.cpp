#include <iostream>
#include <cmath>
#include <array>
#include <algorithm>
#include <queue>

using namespace std;

struct Point {
	float x, y;
	int index;

	static float distance(const Point p1, const Point p2) {
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}

	friend constexpr inline bool operator<(const Point p1, const Point p2) {
		return p1.index < p2.index;
	}

	friend constexpr inline bool operator==(const Point p1, const Point p2) {
		return p1.index == p2.index;
	}

	friend inline ostream& operator<<(ostream& os, const Point point) {
		return os << point.index << '(' << point.x << ',' << point.y << ')';
	}
};

const Point points[]{
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

constexpr int N = 8;// sizeof(points) / sizeof(Point);

struct State {
	float distance;

	int level;
	array<int, N> points;

	friend constexpr inline bool operator<(const State& s1, const State& s2) {
		return s1.distance < s2.distance;
	}
};

int main() {
	priority_queue<State> queue;
	for (int i = 0; i < N; i++)
		queue.push(State{ 0, 0, {i}});

	while (true) {
		auto state = queue.top();
		queue.pop();

		if (state.level == N) {
			cout << "Distance: " << state.distance << endl;
			for (auto point : state.points)
				cout << point << endl;
			break;
		}

		for (int i = 0; i < N; i++) {
			if (find(state.points.begin(), state.points.begin() + state.level, i) != state.points.begin() + state.level)
				continue;

			auto newState = state;
			auto lastPoint = points[state.points[state.level]];
			newState.distance += Point::distance(lastPoint, points[i]);
			newState.level++;
			newState.points[state.level] = i;

			queue.push(newState);
		}
	}
}