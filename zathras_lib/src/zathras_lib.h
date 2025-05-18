#pragma once
#include "Square.h"
#include <string>
const std::string VERSION = "0.0.5";

namespace Zathras_lib {
	static const std::string VERSION = "0.0.5";

	void init() {
		positions::Square::init_squares();
	}


}
