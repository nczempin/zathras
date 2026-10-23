

#include <iostream>
#include <string>

#include "zathras_lib.h"
#include "Uci.h"

using namespace std;

//TODO deal with these globals
Position p;
Searcher searcher;
extern const std::string VERSION;

int main()
{
	string mystr;
	Zathras_lib::init();
	p = Position::create_start_position();
	searcher.pushHistory(p);
	while (getline(cin, mystr)) {
		if (!mystr.empty() && mystr.back() == '\r') {
			mystr.pop_back();
		}
		Interface::Uci::parse(mystr);
	}
	Interface::Uci::waitForSearch();
	return 0;
}
