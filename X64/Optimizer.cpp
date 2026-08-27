#include "pch.h"
#include "Optimizer.hpp"

void X64::Optimizer::Optimize(X64::Code& code) {
	bool changed;

	do {
		changed = false;
		for (auto& pass : passes) {
			changed |= pass->Optimize(code);
		}
	} while (changed);
}