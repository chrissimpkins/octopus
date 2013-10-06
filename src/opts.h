/* opts.h
*  Copyright 2013 Christopher Simpkins.  MIT License
*/

#ifndef OPTS_H
#define OPTS_H

class Options {
public:
	//constructors
	Options() = default;
	Options(const int argnumber, const std::vector<std::string>& optv) : argc(argnumber), optionvec(optv) { }

	//functions
	std::string get_positional_option(std::string optionflag);
	std::string get_last_positional();
	int fill_arg_vector(std::vector<std::string>& fill_vector);  //collects arguments from last command to first option
	std::string get_input();
	std::string get_output();
	bool contains(std::string optionflag);

private:
	//class variables
	int argc;
	const std::vector<std::string>& optionvec;
	//functions
	std::vector<std::string> get_options();

};

#endif