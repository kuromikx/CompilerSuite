#pragma once

namespace X64 {
	enum class ABI {
		Windows,
		SystemV
	};


#ifdef _WINDOWS
	constexpr ABI Abi = ABI::Windows;
#else
	//constexpr ABI Abi = ABI::SystemV;
	constexpr ABI Abi = ABI::Windows;
#endif
}