#pragma once

namespace X64 {
	enum class RegisterSize { Qword, Dword, Word, Byte};

	enum class Register {
		rbp, rsp, rax, rbx, rcx, rdx, /*  */ r8, r9, r10, r11, r12, r13, r14, r15, rsi, rdi,
		ebp, esp, eax, ebx, ecx, edx, /*  */ r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d, esi, edi,
		bp, sp, ax, bx, cx, dx, /*        */ r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w, si, di,
		bpl, spl, al, bl, cl, dl, /*      */ r8b, r9b, r10b, r11b, r12b, r13b, r14b, r15b, sil, dil,
	};

	Register Promote(Register reg);
	bool IsCalleeSaved(Register reg);
	bool IsScratch(Register reg);
	RegisterSize Size(Register reg);
	const char* ToString(Register reg);
}