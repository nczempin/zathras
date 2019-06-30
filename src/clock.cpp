#include "Info.h"
void resetClock() {
	Interface::Info::start = chrono::system_clock::now();
}

bool timeUp()
{
	if (Interface::Info::timePerMove <= 0L)
		return false;
	//return false;
	auto now = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = now - Interface::Info::start;
	return elapsed_seconds.count() >= Interface::Info::timePerMove / 1000.0;
}
