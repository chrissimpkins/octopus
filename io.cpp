/* io.cpp
*  Copyright 2013 Christopher Simpkins.  MIT License
*/

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "io.h"
#include "print.h"
#include "cstdlib" //exit(int successflag)

#define THE_BEGINNING int(0) //flag for getline at the beginning of the file
#define THE_END int(1)       //flag for getline at the end of the file

using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::runtime_error;
using std::ifstream;

bool IO::is_file_good() {
	//if the path variable is assigned in constructor
	if (!path.empty()){
		try{
			ifstream infile(path);
			if (infile.good()){
				infile.close();
				return true;
			}
			else{
				throw runtime_error("Unable to open the file " + path);
			}
		}
		catch(...){
			print_error("Unable to open the file '" + path + "'. Please confirm that it exists.");
			exit(EXIT_FAILURE);
		}
	}
	//otherwise use the ipath from constructor
	else if (!ipath.empty()) {
		try{
			ifstream infile(ipath);
			if (infile.good()){
				infile.close();
				return true;
			}
			else{
				throw runtime_error("Unable to open the file " + path);
			}
		}
		catch (...){
			print_error("Unable to open the file '" + ipath + "'. Please confirm that it exists.");
			exit(EXIT_FAILURE);
		}
	}
	else{
		throw runtime_error("Missing a file path for the file.");
	}
}

//public function to read entire string from a file
string& IO::read_file(){
	//if the path variable is assigned in constructor
	if (!path.empty()){
		try{
			ifstream infile(path);
			if (infile.good()){
				// define the input string from the calling function with the reference that was passed in
				ifstream& inr = infile;
				get_string(inr);
				string& isr = inputstring;
				infile.close();
				return isr;
			}
			else{
				throw runtime_error("Unable to open the input file " + path);
			}
		}
		catch(...){
			print_error("Unable to open the input file " + path);
		}
	}
	//otherwise use the ipath from constructor
	else if (!ipath.empty()) {
		try{
			ifstream infile(ipath);
			if (infile.good()){
				// define the input string from the calling function with the reference that was passed in
				ifstream& inr = infile;
				get_string(inr);
				string& isr = inputstring;
				infile.close();
				return isr;
			}
			else{
				throw runtime_error("Unable to open the input file " + path);
			}
		}
		catch (...){
			print_error("Unable to open the input file " + path);
		}
	}
	else{
		throw runtime_error("Missing a file path for the input file.");
	}
}

//private function to get the entire string from a text file
void IO::get_string(ifstream& inputstream){
	while (inputstream.good()) {
		//load the input stream from the file into a string stream
		int next = inputstream.get();
		if (next == EOF){
			break;
		}
		//implicit conversion of the int char to a string
		inputstring += next;
	}
}

//public function : get the first line of a file
string& IO::get_first_line(){
	ifstream in;
	ifstream& inr = in;
	ifstream& rinr = (get_input_stream(inr));
	get_lines(THE_BEGINNING , 1 , rinr);
	string& isr = inputstring;
	return isr;
}

//public function : get the first n lines of a file
string& IO::get_first_n_lines(int& n) {
	ifstream in;
	ifstream& inr = in;
	ifstream& rinr = (get_input_stream(inr));
	get_lines(THE_BEGINNING, n , rinr);
	string& isr = inputstring;
	return isr;
}

//private function that handles all getline() requests, arguments = THE_BEGINNING or THE_END flag for start position, int n for # lines
// the class string inputstring is defined with this function.
string& IO::get_lines(int startposition, int number_of_lines, ifstream& infile){
	if (startposition == THE_BEGINNING) {
		if (number_of_lines == 1){
			//return a single line from the file
			try{
				// if first line requested, get it and define inputstring class string with the line of text
				std::getline(infile, inputstring);
			}
			catch (...){
				print("Unable to read the first line from the input file");
				exit(EXIT_FAILURE);
			}
		}
		else {
			// return n requested lines from the file
			int linenumber = 0;
			//confirm that there are at least as many lines as the user requested
			while(! infile.eof()) {
				std::getline(infile, tempstring);
				++linenumber;
				inputstring += tempstring + "\n";
				if (linenumber == number_of_lines){
					string& isr = inputstring;
					return isr;
				}
			}
			//if did not return with above code, there were not as many lines in the file as the user requested
			//so return the entire file string
			inputstring = tempstring;
			string& isr = inputstring;
			return isr;
		}
	}
	else if (startposition == THE_END) {

	}
}

//public function to write a string to a file (uses private write_filestring() to write it)
void IO::write_file(string& outstring) {
	if (!path.empty()) {
		ofstream outfile(path);
		if (outfile.good()) {
			ofstream& outr = outfile;
			write_filestring(outr, outstring);
			outfile.close();
		}
		else{
			throw runtime_error("Unable to open the output file " + path);
		}
	}
	else if (!opath.empty()) {
		ofstream outfile(opath);
		if (outfile.good()) {
			ofstream& outr = outfile;
			write_filestring(outr, outstring);
			outfile.close();
		}
		else{
			throw runtime_error("Unable to open the output file " + opath);
		}
	}
	else{
		throw runtime_error("Missing a filepath for the output file.");
	}
}

//private function to the get the input filestream
ifstream& IO::get_input_stream(ifstream& infilestream){
	if (!path.empty()){
		try{
			infilestream.open(path);
			if (infilestream.good()) {
				ifstream& inr = infilestream;
				return inr;
			}
			else{
				throw runtime_error("Input File Error");
			}
		}
		catch (...) {
			print("Unable to open the input file " + path);
			exit(EXIT_FAILURE);
		}
	}
	else if (!ipath.empty()){
		try{
			infilestream.open(ipath);
			if (infilestream.good()){
				ifstream& inr = infilestream;
				return inr;
			}
			else{
				throw runtime_error("Input File Error");
			}
		}
		catch (...){
			print("Unable to open the input file " + ipath);
			exit(EXIT_FAILURE);
		}
	}
}

//private function to get the output filestream
ofstream& IO::get_output_stream(ofstream& outfilestream){
	if (!path.empty()){
		try{
			outfilestream.open(path);
			if (outfilestream.good()) {
				ofstream& outr = outfilestream;
				return outr;
			}
			else{
				throw runtime_error("File Write Error");
			}
		}
		catch (...) {
			print("Unable to write to the output file " + path);
			exit(EXIT_FAILURE);
		}
	}
	else if (!opath.empty()){
		try{
			outfilestream.open(opath);
			if (outfilestream.good()) {
				ofstream& outr = outfilestream;
				return outr;
			}
			else{
				throw runtime_error("File Write Error");
			}
		}
		catch (...) {
			print("Unable to write to the output file " + opath);
			exit(EXIT_FAILURE);
		}
	}
}

// private function to write a string to a file
void IO::write_filestring(ofstream& ofs, string& outstring) {
	if (ofs.good()) {
		ofs << outstring << endl;
	}
}

// public function to write string to std out
void IO::write_stdout(string& outstring){
	flush(outstring);
}

