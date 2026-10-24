#pragma once
#include "Square.h"
#include <string>
const std::string VERSION = "0.0.8";

namespace Zathras_lib {
	void init() {
		Positions::Square::init_squares();
	}


}