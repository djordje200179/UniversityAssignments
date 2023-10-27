#include <iostream>
#include <array>
#include <algorithm>
#include <utility>
#include <numeric>
#include <limits>
#include <iterator>
#include <chrono>
#include <bitset>

using namespace std;
using namespace std::chrono;

template <size_t K, int N>
static constexpr bool next_variant(array<int, K>& arr) {
    auto it = find_if(arr.rbegin(), arr.rend(), [](auto elem) { return elem != N - 1; });

    if (it == arr.rend()) {
        fill(arr.begin(), arr.end(), 0);
        return false;
    }

    (*it)++;
    fill(arr.rbegin(), it, 0);

    return true;
}

struct Link {
    int first, second;

    friend ostream& operator<<(ostream& os, Link link) {
        os << char('A' + link.first) << ' ' << char('A' + link.second);
        return os;
    }
};

template<size_t N>
static constexpr array<Link, N - 1> create_tree(const array<int, N - 2>& p) {
    bitset<N> v;

    array<Link, N - 1> links;
    for (int pi = 0; pi < N - 2; pi++) {
        int vi;
        for (vi = 0; vi < N; vi++) {
            if (v[vi])
                continue;

            if (find(p.begin() + pi, p.end(), vi) == p.end())
                break;
        }

        v[vi] = true;
        links[pi] = { p[pi], vi };
    }

    int first_vertex = -1, second_vertex = -1;
    for (int i = 0; i < N; i++) {
        if (v[i])
            continue;

        if (first_vertex == -1)
            first_vertex = i;
        else {
            second_vertex = i;
            break;
        }
    }

    links[N - 2] = { first_vertex, second_vertex };

    return links;
}

template<size_t N>
static constexpr int calculate_cost(const array<Link, N - 1>& links, const int distances[][N]) {
    int total_cost = 0;
    array<int, N> links_count = { 0 };

    for (auto link : links) {
        total_cost += distances[link.first][link.second];

        links_count[link.first]++;
        links_count[link.second]++;
    }

    for (int i = 0; i < N; i++) {
        if (links_count[i] > 3)
            total_cost += (links_count[i] - 3) * 250;
    }

    return total_cost;
}

template<size_t N>
static ostream& operator<<(ostream& os, const array<Link, N>& links) {
    auto first_pass = true;
    for (auto link : links) {
        if (!first_pass)
            os << " - ";
        else
            first_pass = false;

        os << link;
    }

    return os;
}

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

int main() {
    Timer timer;

    constexpr size_t N = 10;

    array<int, N - 2> arr = { 0 };

    const int distances[][N] = {
      { 0,  374,  350,  223,  108,  178,  252,  285,  240,  356 },
      { 374,    0,   27,  166,  433,  199,  135,   95,  136,   17 },
      { 350,   27,    0,   41,   52,  821,  180,  201,  131,  247 },
      { 223,  166,   41,    0,  430,   47,   52,   84,   40,  155 },
      { 108,  433,   52,  430,    0,  453,  478,  344,  389,  423 },
      { 178,  199,  821,   47,  453,    0,   91,   37,   64,  181 },
      { 252,  135,  180,   52,  478,   91,    0,   25,   83,  117 },
      { 285,   95,  201,   84,  344,   37,   25,    0,   51,   42 },
      { 240,  136,  131,   40,  389,   64,   83,   51,    0,  118 },
      { 356,   17,  247,  155,  423,  181,  117,   42,  118,    0 }
    };

    int best_cost = numeric_limits<int>::max();
    array<Link, N - 1> best_tree;

    do {
        const auto& tree = create_tree<N>(arr);
        auto cost = calculate_cost(tree, distances);

        if (cost < best_cost) {
            best_cost = cost;
			best_tree = tree;
        }
    } while (next_variant<N - 2, N>(arr));

    cout << "Best cost: " << best_cost << endl;
    cout << best_tree << endl;
}