#pragma once
#include "Square.h"
#include <string>
const std::string VERSION = "0.0.7";

namespace Zathras_lib {
	inline void init() {
		Positions::Square::init_squares();
	}
}