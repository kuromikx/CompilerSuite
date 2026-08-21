#pragma once
#include <vector>

#include "MC.hpp"

namespace X64 {
	class MCBlock : public std::vector<MC> {
	public:
		//void InsertPrologue(std::uint64_t stacksize);
		//void InsertEpilogue();
	};
}