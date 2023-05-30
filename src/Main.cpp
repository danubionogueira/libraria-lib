#include <string>
#include <iostream>
#include "Test.hpp"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char** argv){
	bool printSuccesses = false, printErrors = true;
	Test *test = new Test();

	for (int i=1; i<argc; i++){
		string arg = string(argv[i]);

		if ((arg == "-v") || (arg == "--verbose")){
			printSuccesses = true;
			printErrors = true;
		}
		else if ((arg == "-q") || (arg == "--quiet")){
			printSuccesses = false;
			printErrors = false;
		}
		else {
			cout << "Unknwon argument " << arg << endl;
			return EXIT_FAILURE;
		}
	}

	test->run(printSuccesses, printErrors);
	delete test;

	return EXIT_SUCCESS;
}
