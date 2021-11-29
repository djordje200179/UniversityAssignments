#pragma once

#include "Node.hpp"

namespace BStarTree {
struct Root : Node {
	Root() = default;

	bool trySpill(SpillType type) override { return false; }
	void split() override;
	void join() override;
};
}