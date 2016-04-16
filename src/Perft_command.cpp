/*
 * Perft_command.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Perft_command.h"

#include <getopt.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Position.h"
#include "Move_generator.h"

Perft_command::Perft_command(shared_ptr<Command_receiver> receiver) :
		Abstract_command(receiver) {
	// TODO Auto-generated constructor stub

}

Perft_command::~Perft_command() {
	// TODO Auto-generated destructor stub
}

void Perft_command::do_long_option(bool flag, string name, string argument) {
	if (flag) {
		cout << "flag" << endl;
	}
	cout << "name: " << name << endl;
	cout << "argument: " << argument << endl;
}

string Perft_command::get_short_options() {
	return "t";
}
vector<option> Perft_command::get_long_options() {
	vector<option> retval = { { "textconv", no_argument, 0, 0 } };
	return retval;
}

void Perft_command::do_short_option(int c, string argument) {
	switch (c) {
	case 'w':
		//cout << "option -w" << endl;
		break;
	case 't':
		//  cout << "option -t" << endl;
		//  set_option_type(true);
		break;
	case '?':
		/* getopt_long already printed an error message. */
		break;
	default:
		throw 129; //TODO is this specific or generic?
	}
}

void Perft_command::execute() {
	vector<string> path = receiver->getArguments();
	int perft_depth = 3; //TODO get this from arguments, but use a reasonable default
	shared_ptr<Position> pp = Position::create_start_position();
	Position p = *pp;
	cout << p << endl;

	Move_generator mg;
	mg.pregenerate_moves();
	mg.generate_moves(p);

}
