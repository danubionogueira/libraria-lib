#pragma once
#include <string>

using std::string;

class Test{
	protected:
		bool printSuccesses;
		bool printErrors;

		unsigned int successes;
		unsigned int errors;
	public:
		void addSuccess(string testName, string testFile, unsigned int fileLine, string message);
		void addError(string testName, string testFile, unsigned int fileLine, string message);

		void run(bool printSuccesses=false, bool printErrors=true);
};

#define testSuccess(test, name, message) test->addSuccess(name, __FILE__, __LINE__, message)
#define testError(test, name, message) test->addError(name, __FILE__, __LINE__, message)
