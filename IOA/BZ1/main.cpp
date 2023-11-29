#include <array>
#include <iostream>

using namespace std;

constexpr int check_state(const array<bool, 72>& state) {
	int trailing_byte_sum = 0, leading_byte_sum = 0;

	for (int i = 0; i < 8; i++) {
		trailing_byte_sum += state[i];
		leading_byte_sum += state[8 + i];
	}

	if (trailing_byte_sum >= leading_byte_sum)
		return 0;

	for (int i = 8; i < 64; i++) {
		trailing_byte_sum -= state[i - 8];
		trailing_byte_sum += state[i];

		leading_byte_sum -= state[i];
		leading_byte_sum += state[i + 8];

		if (trailing_byte_sum >= leading_byte_sum)
			return i - 7;
	}

	return -1;
}

constexpr bool next_permutation(array<bool, 72>& state, int failure_bit) {
	for (int i = failure_bit + 1; i < state.size(); ++i)
		state[failure_bit] = 0;

	for (int i = failure_bit; i >= 0; --i) {
		if (state[i] == 0) {
			state[i] = 1;
			return true;
		} 
		
		state[i] = 0;
	}

	return false;
}

int main() {
	array<bool, 72> state = { 0 };

	while (true) {
		int failure_bit = check_state(state);

		if (failure_bit == -1)
			cout << "Found solution: " << endl;
		
		if (!next_permutation(state, failure_bit))
			break;
	}

	return 0;
}