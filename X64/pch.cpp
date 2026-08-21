// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include "MC.hpp"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.


void Foo() {
	using namespace X64;
	MC mc3 = MC::Add({ Register::rax }, { Register::rbx });
}