/* main.cpp
*  Copyright 2013 Christopher Simpkins.  MIT License
*/

// Constant definitions
#define VERSION string("0.1.3")
#define APPLICATION string("Octopress Commander")
#define COPYRIGHT string("Copyright 2013 Christopher Simpkins")
#define LICENSE string("MIT License")

// Standard library headers
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

// Project headers
#include "main.h"
#include "opts.h"
#include "print.h"

// Standard Library namespace
using std::string;
using std::vector;
using std::cout;
using std::wcout;
using std::endl;


/******************************************
*  G'day, I'm main()
*******************************************/
// Handles the command line parsing

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
				print_error("example : oc post 'A cool post title'");
				return 1;
			}
			else{
				//get the argument
				const string title = clv.at(2);
				//create the command
				const string new_post_command = "bundle exec rake new_post['" + title + "']";
				const char * npc_c = new_post_command.c_str();
				print("Creating new post markdown file entitled: '" + title + "'");
				//run command
				system(npc_c);
			}
		}
		// NEW PAGE -----------------------------------------------------
		else if (cmd == "page") {
			if (argc < 3){
				print_error("Please include the title for your page after the command.");
				print_error("example : oc page 'A cool page title'");
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
			print("Pushing files to server...");
			if (system(deploy_string) != 0){
				print_error("There was an error pushing your files to the server.  They were not successfully deployed.");
				return 1;
			}
			print("The files were successfully pushed to your server.");
		}
		// PREVIEW ------------------------------------------------------
		else if (cmd == "preview") {
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
				print("> Git is installed.");
				print(" ");
			}
			else{
				fail = 1;
				print("> Error: Git is not installed.");
				print(" ");
			}
			if (system("which ruby") == 0) {
				print("> Ruby is installed. Please confirm that it is version 1.9.3 in the line below");
				print(" ");
				system("ruby --version");
				print(" ");
			}
			else{
				fail = 1;
				print("> Error: Ruby is not installed.");
				print(" ");
			}
			// Rake test
			if (system("which rake") == 0){
				print("> Rake is installed.");
				print(" ");
			}
			else{
				fail = 1;
				print("> Error: Rake is not installed.");
				print(" ");
			}
			// Bundle test
			if (system("which bundle") == 0) {
				print("> Bundle is installed.");
				print(" ");
			}
			else {
				fail = 1;
				print("> Error: Bundle is not installed.");
				print(" ");
			}
			print("...Completed Octopress Commander tests");
			if (fail == 1){
				print("There were errors detected during these tests.  Please scan the log above.");
			}
			else{
				print("The tests completed with no errors. All is well.");
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
	print("\tpage <page name>: create a new page and specify name");
	print("\tpost <post name>: create a new post and specify name");
	print("\tpreview : open local server to view your site");
	print("\tpublish [--heroku] : publish your site");
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
inline const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}