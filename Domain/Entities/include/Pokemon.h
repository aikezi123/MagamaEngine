#pragma once

#include <string>
#include <cstdint>

namespace Magma::Domain::Entities {

	// 6维属性基础结构
	struct StateBlock {
		uint32_t hp{ 0 };
		uint32_t attack{ 0 };
		uint32_t defense{ 0 };
	};




}