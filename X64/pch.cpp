// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include "MC.hpp"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.


void Foo() {
	using namespace X64;
	using enum Register::Name;

	MC mc = MC::Add(Operand(Register(rax)), Operand(Register(rbx)));
	MC mc2 = MC::Add({ Register(rax) }, { Register(rbx) });
	MC mc3 = MC::Add({ rax }, { rbx });
}