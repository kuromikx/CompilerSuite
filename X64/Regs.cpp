#include "Regs.hpp"
#include "Platform.hpp"
#include "pch.h"


const char* X64::ToCString(Register reg) {
#define X(a,b,c,d) case a: return #a ; case b: return #b ; case c: return #c ; case d: return #d
	using enum Register;
	switch (reg) {
		X(rbp, ebp, bp, bpl);
		X(rsp, esp, sp, spl);
		X(rax, eax, ax, al);
		X(rbx, ebx, bx, bl);
		X(rcx, ecx, cx, cl);
		X(rdx, edx, dx, dl);
		X(r8, r8d, r8w, r8b);
		X(r9, r9d, r9w, r9b);
		X(r10, r10d, r10w, r10b);
		X(r11, r11d, r11w, r11b);
		X(r12, r12d, r12w, r12b);
		X(r13, r13d, r13w, r13b);
		X(r14, r14d, r14w, r14b);
		X(r15, r15d, r15w, r15b);
		X(rsi, esi, si, sil);
		X(rdi, edi, di, dil);
	}
#undef X
}

std::string X64::ToString(const Register& reg) {
	return { ToCString(reg) };
}

X64::Register X64::Promote(Register reg) {
	using enum Register;
	switch (reg) {
	case rbp:	case ebp:	case bp:		case bpl:	return rbp;
	case rsp:	case esp:	case sp:		case spl:	return rsp;
	case rax:	case eax:	case ax:		case al:	return rax;
	case rbx:	case ebx:	case bx:		case bl:	return rbx;
	case rcx:	case ecx:	case cx:		case cl:	return rcx;
	case rdx:	case edx:	case dx:		case dl:	return rdx;
	case rsi:	case esi:	case si:		case sil:	return rsi;
	case rdi:	case edi:	case di:		case dil:	return rdi;
	case r8:	case r8d:	case r8w:		case r8b:	return r8;
	case r9:	case r9d:	case r9w:		case r9b:	return r9;
	case r10:	case r10d:	case r10w:		case r10b:	return r10;
	case r11:	case r11d:	case r11w:		case r11b:	return r11;
	case r12:	case r12d:	case r12w:		case r12b:	return r12;
	case r13:	case r13d:	case r13w:		case r13b:	return r13;
	case r14:	case r14d:	case r14w:		case r14b:	return r14;
	case r15:	case r15d:	case r15w:		case r15b:	return r15;
	}
}

bool X64::IsCalleeSaved(Register reg) {
	reg = Promote(reg);
	using enum Register;
	if constexpr (Abi == ABI::Windows) {
		switch (reg) {
		case rbx:
		case rbp:
		case rsi:
		case rdi:
		case rsp:
		case r12:
		case r13:
		case r14:
		case r15:
			return true;
		}
	}
	else if constexpr (Abi == ABI::SystemV) {
		switch (reg) {
		case rbx:
		case rbp:
		case rsp:
		case r12:
		case r13:
		case r14:
		case r15:
			return true;
		}
	}

}

bool X64::IsScratch(Register reg) {
	using enum Register;
	reg = Promote(reg);
	if constexpr (Abi == ABI::Windows) {
		switch (reg) {
		case rax:
		case rcx:
		case rdx:
		case r8:
		case r9:
		case r10:
		case r11:
			return true;
		}
	}
	else if constexpr (Abi == ABI::SystemV) {
		switch (reg) {
		case rax:
		case rcx:
		case rdx:
		case rsi:
		case rdi:
		case r8:
		case r9:
		case r10:
		case r11:
			return true;
		}
	}
	return false;
}

X64::RegisterSize X64::Size(Register reg) {
	using enum Register;
	if (reg >= rbp && reg <= rdi) return	RegisterSize::Qword;
	if (reg >= ebp && reg <= edi) return	RegisterSize::Dword;
	if (reg >= bp && reg <= di)	return		RegisterSize::Word;
	return									RegisterSize::Byte;
}
