//  main.cpp

#include <getopt.h>
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

void print_usage()
{
  cout << "usage: zathras perft [<args>]" << endl;
}

void hopt(shared_ptr<Abstract_command> command, int c, int option_index,
    struct option* long_options)
{
  option long_option = long_options[option_index];
  if (c == 0) {
    command->do_long_option(long_option.flag, long_option.name, optarg);
  } else {
    string argument = "";
    if (optarg) {
      argument = string(optarg);
    }
    command->do_short_option(c, argument);
  }
}

void handle_options(shared_ptr<Abstract_command> command, int argc, char* argv[])
{
  int c;
  while (1) {
    vector<option> long_options1 = command->get_long_options();
    long_options1.push_back(
      { 0, 0, 0, 0 }); // for handling the options
    option* long_options = &long_options1[0];
    /* getopt_long stores the option index here. */
    int option_index = 0;
    string short_options_hash_object = command->get_short_options();
    c = getopt_long(argc, argv, short_options_hash_object.c_str(), long_options,
        &option_index);
    /* Detect the end of the options. */
    if (c == -1) {
      break;
    }
    hopt(command, c, option_index, long_options);
  }
}

shared_ptr<Abstract_command> determine_command(shared_ptr<Command_receiver> receiver, int argc,
    char* argv[])
{
  if (argc == 1) {
    print_usage();
    throw 1;
  }
  string searching_for(argv[1]);
  vector<string> commands =
    { "perft" };
  auto it = find(begin(commands), end(commands), searching_for);
  if (it == end(commands)) {
    cout << "zathras: '" << argv[1]
        << "' is not a zathras command." /*"See 'zathras --help'."*/ << endl;
    throw 1;
  }
  int pos = it - commands.begin();

  //TODO do this more elegantly
  Abstract_command* command;
  switch (pos) {
  case 0:
    command = new Perft_command(receiver);
    break;
/*
  case 1:
    command = new Cat_file(mygit);
    break;
  case 2:
    command = new Rev_parse(mygit);
    break;
*/
  default:
    throw -7; //TODO

  }
  shared_ptr<Abstract_command> p(command);
  return p;
}

int main(int argc, char* argv[])
{
  shared_ptr<Command_receiver> mygit(new Command_receiver());
  shared_ptr<Abstract_command> command;
  try {

    command = determine_command(mygit, argc, argv);

    --argc;
    ++argv;
    handle_options(command, argc, argv);
  } catch (const int n) {
    if (n == 129) { //TODO give this magic number a name
      print_usage();
    }
    return n;
  }
  vector<string> varargs;
  while (optind < argc) { //TODO get rid of dependency on optind
    char* file = argv[optind++];
    varargs.push_back(file);
  }
  try {
    mygit->setArguments(varargs);
    command->execute();
  } catch (int n) {
    return n; //TODO more sophisticated exception handling
  }
  return 0;
}
