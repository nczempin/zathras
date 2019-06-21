#pragma once

#include <chrono>

#include "Move.h"

class Info{
public:
	static int seldepth;
	static Move currmove;
	static int currmovenumber;
	static int nodes;
	static int nps;
	static chrono::system_clock::time_point start;

};