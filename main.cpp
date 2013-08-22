/* main.cpp
*  Copyright 2013 Christopher Simpkins.  MIT License
*/

// Constant definitions
#define VERSION string("0.1.8")
#define APPLICATION string("Octopress Commander")
#define COPYRIGHT string("Copyright 2013 Christopher Simpkins")
#define LICENSE string("MIT License")

// Standard library headers
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

// Project headers
#include "main.h"
#include "opts.h"
#include "print.h"
#include "io.h"

// Standard Library namespace
using std::string;
using std::vector;
using std::cout;
using std::wcout;
using std::endl;


/******************************************
*  G'day, I'm main()
*******************************************/
// Handles command line parsing

int main(int argc, char const *argv[]) {
	//user did not enter enough arguments in the command
	if (argc < 2) {
		print("You didn't enter a command");
		show_usage();
		return 1;
	}
	else {
		//create the command line vector
		const vector<string> clv = create_CL_vector(argc, argv);
		//create reference to the CL vector (used to pass the CL vector by reference)
		const vector<string>& clvr = clv;
		// create a string for the main user entered command at vector position 1
		const string cmd = clv.at(1);
		// HELP ---------------------------------------------------------
		if (cmd == "help" || cmd == "-h" | cmd == "--help"){
			show_help();
		}
		// VERSION ------------------------------------------------------
		else if (cmd == "version" || cmd == "-v" || cmd == "--version") {
			show_version();
		}
		// NEW POST -----------------------------------------------------
		else if (cmd == "post") {
			if (argc < 3){
				print_error("Please include the title for your post after the command.");
				print_error("Usage : oc post 'A cool post title'");
				return 1;
			}
			else{
				Options opt(argc, clvr);
				const string title = opt.get_last_positional();
				//user requests octopress generation of the post
				if (opt.contains("--octo")){
					//create the command
					const string new_post_command = "bundle exec rake new_post['" + title + "']";
					const char * npc_c = new_post_command.c_str();
					print("Creating new post markdown file entitled: '" + title + "'");
					//run command
					system(npc_c);
					return 0;
				}
				else {
					//create the filepath string
					string date = currentDate();
					string long_title = date + '-' + title;
					string post_title = replaceChar(long_title, ' ', '-');
					std::transform(post_title.begin(), post_title.end(), post_title.begin(), ::tolower);
					string post_title_md = post_title + ".markdown";
					string post_path = "source/_posts/" + post_title_md;
					string post_head_string = getPostHeader(title, currentDate(), currentTime());
					//write to file
					IO newpost(post_path);
					newpost.write_file(post_head_string);
					string response_string = "Your new post '" + post_title_md + "' has been created in the path source/_posts.";
					print(response_string);
					return 0;
				}
			}
		}
		// NEW PAGE -----------------------------------------------------
		else if (cmd == "page") {
			if (argc < 3){
				print_error("Please include the title for your page after the command.");
				print_error("Usage : oc page 'A cool page title'");
				return 1;
			}
			else{
				//get the argument
				const string title = clv.at(2);
				//create the command
				const string new_page_command = "bundle exec rake new_page['" + title + "']";
				const char * npc_c = new_page_command.c_str();
				print("Creating new page markdown file entitled: '" + title + "'");
				//run command
				system(npc_c);
				return 0;
			}
		}
		// GENERATE -----------------------------------------------------
		else if (cmd == "generate") {
			const char * gen_string = "bundle exec rake generate";
			if (system(gen_string) != 0) {
				print_error("Unable to generate your static HTML files.");
				return 1;
			}
			else {
				//do nothing Jekyll provides an appropriate flat file generation message for user
			}
		}
		// PUBLISH ------------------------------------------------------
		else if (cmd == "publish") {
			Options opt = Options(argc, clvr);
			// Heroku deployment
			if (opt.contains("--heroku")){
				const char * gen_string = "bundle exec rake generate";
				const char * git_add_string = "git add .";
				string git_commit_string = "git commit -m 'updated @ " + currentDateTime() + "'";
				const char * git_commit_c = git_commit_string.c_str();
				const char * heroku_push_string = "git push heroku master";
				// generate flat html files
				print("Generating project html files...");
				system(gen_string);
				// stage files
				print("Staging files with git...");
				system(git_add_string);
				// commit staged files
				print("Committing files with git...");
				system(git_commit_c);
				// push files to remote heroku repository
				print("Pushing files to Heroku server...");
				system(heroku_push_string);
				print("Complete.");
				return 0;

			}
			// GitHub or rsync deployment
			const char * gen_string = "bundle exec rake generate";
			const char * deploy_string = "bundle exec rake deploy";
			print("Generating project html files...");
			if (system(gen_string) != 0){
				print_error("There was an error generating your html files. The files were not pushed to your server.");
				return 1;
			}
			// flat file generation worked, deploy to server
			else {
				print("Pushing files to server...");
				if (system(deploy_string) != 0){
					print_error("There was an error pushing your files to the server.  They were not successfully deployed.");
					return 1;
				}
				else{
					print("The files were successfully pushed to your server.");
					return 0;
				}
			}
		}
		// PREVIEW ------------------------------------------------------
		else if (cmd == "preview") {
			Options opt = Options(argc, clvr);
			//user specifies that they want the Octopress generated server
			if (opt.contains("--octo")){
				const char * prev_string = "bundle exec rake preview";
				print("Opening local server at http://localhost:4000.");
				if (system(prev_string) != 0){
					print_error("Unable to open the local server.");
					return 1;
				}
				else{
					print("Local server is running.");
				}
			}
			else{
				// Python 2.x
				string prev_string_v2 = "python -m SimpleHTTPServer";
				// Python 3.x
				string prev_string_v3 = "python -m http.server";
				string the_port = "8000";
				string port_string;
				if (argc > 2) {
					//confirm that the last argument was not a switch, if not it is the port
					if (opt.get_last_positional().substr(0,1) == "-"){
						port_string = "8000";
					}
					else{
						port_string = opt.get_last_positional();
					}
					prev_string_v2 = prev_string_v2 + " " + port_string;
					prev_string_v3 = prev_string_v3 + " " + port_string;
					the_port = port_string;
				}
				print("Opening server on port " + the_port);
				//create C char arrays for use with the system cmd
				const char* prev_string_2cstr = prev_string_v2.c_str();
				const char* prev_string_3cstr = prev_string_v3.c_str();
				if (opt.contains("--py3")){
					//user specifies option for Python 3 as default version
					if (system(prev_string_3cstr) != 0){
						return 1;
					}
				}
				else{
					//try Python 2.x as default
					if (system(prev_string_2cstr) != 0) {
						//if it doesn't work, try Python 3.x command
						if (system(prev_string_3cstr) != 0) {
							return 1;
						}
						// defer notification of success to Python which notifies of server start
					}
				}
			}
		}
		// READ ---------------------------------------------------------
		else if (cmd == "read"){
			if (argc == 3){
				Options opt(argc, clvr);
				string infile = opt.get_last_positional();
				IO inout(infile);
				if (inout.is_file_good()){
					cout << inout.read_file() << endl;
				}
			}
			else{
				print_error("Missing the input file path.");
				print_error("Usage:");
				print_error("\toc read <filepath>");
			}
		}
		// WATCH --------------------------------------------------------
		else if (cmd == "watch") {
			const char * watch_string = "bundle exec rake watch";
			print("Beginning to watch the source and SASS directories for changes.");
			if (system(watch_string) != 0) {
				print_error("Unable to watch the source and SASS directories.");
				return 1;
			}
		}
		// DOCTOR --------------------------------------------------------
		else if (cmd == "doctor") {
			int fail = 0;
			print("Beginning Octopress Commander tests...");
			print(" ");
			// Git test
			if (system("which git") == 0){
				print("> Git is installed.\n");
			}
			else{
				fail = 1;
				print("> Error: Git is not installed.\n");
			}
			if (system("which ruby") == 0) {
				print("> Ruby is installed. Please confirm that it is version 1.9.3 in the line below:\n");
				system("ruby --version");
				print("\n");
			}
			else{
				fail = 1;
				print("> Error: Ruby is not installed.\n");
			}
			// Rake test
			if (system("which rake") == 0){
				print("> Rake is installed.\n");
			}
			else{
				fail = 1;
				print("> Error: Rake is not installed.\n");
			}
			// Bundle test
			if (system("which bundle") == 0) {
				print("> Bundle is installed.\n");
			}
			else {
				fail = 1;
				print("> Error: Bundle is not installed.\n");
			}
			//rsync test
			if (system("which rsync") == 0) {
				print("> rsync is installed.\n");
			}
			else {
				fail = 1;
				print("> rsync is not installed.  This is only relevant if you use it to deploy your site.\n");
			}
			print("...Completed Octopress Commander tests");
			if (fail == 1){
				print("There were errors detected during these tests.  Please scan the log above.");
			}
			else{
				print("The tests completed with no errors. All is well.");
			}
		}
		else if (cmd == "test") {
			// used for testing purposes.
		}
		//otherwise if a second argument is present print error message that the second argument is not a known command or option
		else {
			if (argv[1]) {
				print_error(clvr.at(1) + " is not a known command.");
				return 1;
			}
		}
	}

}

/******************************************
*  Convert the CL character array to vector
*******************************************/

inline vector<string> create_CL_vector(int argc, char const *argv[]){
	vector<string> vs;
	for (int i = 0; i < argc; ++i){
		vs.push_back(argv[i]);
	}
	return vs;
}

/******************************************
*  Display help
*******************************************/

inline void show_help() {
	print(" ");
	print("------------------------------------------------");
	print(APPLICATION);
	print("Version: " + VERSION);
	print(COPYRIGHT);
	print(LICENSE);
	print("------------------------------------------------");
	print(" ");
	// SYNTAX HELP
	print("Usage:");
	print("\toc <command> [-shortoption] [--longoption] <argument(s)>\n");
	// HELP INFO
	print("Available Commands:");
	print("\thelp : view help documentation");
	print("\tpage <page name>: create a new page and specify file name");
	print("\tpost <post name>: create a new post and specify file name");
	print("\tpreview : open local server to view your site");
	print("\tpublish [--heroku] : publish your site");
	print("\tread <post/page path>: read your post/page in terminal");
	print("\twatch : watch the source and SASS directories for changes");
	print("\tversion: view current version number");
}

/******************************************
*  Display usage
*******************************************/

inline void show_usage() {
	print("Usage:");
	print("\toc <command> [-shortoption] [--longoption] <last positional arg>");
}

/******************************************
*  Display version
*******************************************/

inline void show_version(){
	print(" ");
	print(APPLICATION);
	print("Version: " + VERSION);
}

/******************************************
*  Current Date/Time
*******************************************/
inline string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

/******************************************
*  Current Date
*******************************************/
inline string currentDate() {
	time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return buf;
}

/******************************************
*  Current Time
*******************************************/
inline string currentTime() {
	time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%R", &tstruct);
    return buf;
}

/******************************************
*  Character replacements for strings
*******************************************/
// ch1 = char to replace ; ch2 = replacement char
inline string replaceChar(string str, char ch1, char ch2) {
  for (int i = 0; i < str.length(); ++i) {
    if (str[i] == ch1)
      str[i] = ch2;
  }
  return str;
}

/******************************************
*  Octopress Post Header Generator
*******************************************/
// Generated Octopress post header
inline string getPostHeader(string title, string postdate, string posttime) {
	string posthead = "---\nlayout: post\ntitle: '" + title + "'\ndate: " + postdate + " " + posttime + "\ndescription: \nkeywords: \ncomments: true\ncategories: \npublished: true\n---";
	return posthead;
}


