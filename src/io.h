/* io.h 
*  Copyright 2013 Christopher Simpkins.  MIT License
*/

#ifndef IO_H
#define IO_H

class IO {

	public:
		//contructors
		IO() = default;
		// constructor with single path
		IO(std::string userpath) : path(userpath) { }
		// constructor with input and output paths
		IO(std::string inpath, std::string outpath) : ipath(inpath), opath(outpath) { }
		
		//functions
		std::string& read_file();
		std::string& get_first_line();
		std::string& get_first_n_lines(int& number_of_lines);
		std::string& get_last_line();
		std::string& get_last_n_lines(int& number_of_lines);
		void write_file(std::string& outstring);
		void write_stdout(std::string& outstring);
		bool is_file_good();

	private:
		//variables
		std::string path; //generic filepath when single path used in constructor
		std::string ipath; //in filepath
		std::string opath;  //out filepath
		std::string inputstring; //defined with the appropriate string from the file, returned as reference from public functions
		std::string tempstring; //temporary string to hold parts of the text from file
		//functions
		void get_string(std::ifstream& filestream);
		void write_filestring(std::ofstream& filestream, std::string& outstring);
		std::ifstream& get_input_stream(std::ifstream& inr);
		std::ofstream& get_output_stream(std::ofstream& outr);
		std::string& get_lines(int startposition, int n, std::ifstream& instream); // private function that handles all of the get line requests
};

#endif