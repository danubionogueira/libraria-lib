#include <string>
#include "Test.hpp"

using std::string;

int main(int argc, char** argv){
	Test *test = new Test();
	test->run(true, true);
	delete test;

	return EXIT_SUCCESS;
}
