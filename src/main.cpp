//  main.cpp

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "Command_receiver.h"
#include "Command.h"
#include "Perft_command.h"

using namespace std;

int main(int argc, char* argv[])
{
  try {
	  Perft_command command = Perft_command();

    command.execute();
  } catch (int n) {
    return n; //TODO more sophisticated exception handling
  }
  cout << "done\n";
  return 0;
}
