#include "Info.h"
#include "Move.h"

int Info::seldepth = 0;
Move Info::currmove;
int Info::currmovenumber;
int Info::nodes;
int Info::nps;
chrono::system_clock::time_point Info::start;