#include "info.h"
void resetClock() {
	zathras::interface::Info::start = chrono::system_clock::now();
}

bool timeUp()
{
	if (zathras::interface::Info::timePerMove <= 0L)
		return false;
	//return false;
	auto now = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = now - zathras::interface::Info::start;
	return elapsed_seconds.count() >= zathras::interface::Info::timePerMove / 1000.0;
}
