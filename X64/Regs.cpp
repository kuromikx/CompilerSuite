#include "Regs.hpp"
#include "Platform.hpp"
#include "pch.h"

constexpr X64::Register::Register(enum X64::Register::Name name) :
	name(name)
{
}

constexpr X64::Register X64::Register::Promoted() const {
	switch (name) {
	case rax:	case eax:	case ax:		case al:	return rax;
	case rbx:	case ebx:	case bx:		case bl:	return rbx;
	case rcx:	case ecx:	case cx:		case cl:	return rcx;
	case rdx:	case edx:	case dx:		case dl:	return rdx;
	case rbp:	case ebp:	case bp:		case bpl:	return rbp;
	case rsp:	case esp:	case sp:		case spl:	return rsp;
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

constexpr bool X64::Register::IsCalleeSaved() const {
	if constexpr (Abi == ABI::Windows) {
		switch (Promoted().Name()) {
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
		switch (Promoted().Name()) {
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

	return false;
}

constexpr bool X64::Register::IsScratch() const {
	if constexpr (Abi == ABI::Windows) {
		switch (Promoted().Name()) {
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
		switch (Promoted().Name()) {
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

constexpr enum X64::Register::Name X64::Register::Name() const noexcept {
	return name;
}

constexpr enum X64::Register::Size X64::Register::Size() const noexcept {
	if (name >= rbp && name <= rdi) return	Size::Qword;
	if (name >= ebp && name <= edi) return	Size::Dword;
	if (name >= bp && name <= di)	return	Size::Word;
	return									Size::Byte;
}

constexpr bool X64::Register::operator==(const enum X64::Register::Name name) const noexcept {
	return Name() == name;
}
