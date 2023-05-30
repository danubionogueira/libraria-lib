#include "Test.hpp"
#include "Book.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::to_string;

void testAuthorRegular(Test* test){
	Author* author = new Author("First Middle Last");
	string name = "Testing regular Author";

	if (author->getFirstName() == "First")
		testSuccess(test, name, "Author first name is ok");
	else
		testError(test, name, "Author first name is wrong");

	if (author->getMiddleName() == "Middle")
		testSuccess(test, name, "Author middle name is ok");
	else
		testError(test, name, "Author middle name is wrong");

	if (author->getLastName() == "Last")
		testSuccess(test, name, "Author last name is ok");
	else
		testError(test, name, "Author last name is wrong");

	if (author->getFullName() == "First Middle Last")
		testSuccess(test, name, "Author full name is ok");
	else
		testError(test, name, "Author full name is wrong");

	if (author->getAbbreviatedFirstName() == "F.")
		testSuccess(test, name, "Author abbreviated first name is ok");
	else
		testError(test, name, "Author abbreviated first name is wrong, expected 'F.' got '" + author->getAbbreviatedFirstName() + "'");

	if (author->getAbbreviatedMiddleName() == "M.")
		testSuccess(test, name, "Author abbreviated middle name is ok");
	else
		testError(test, name, "Author abbreviated middle name is wrong, expected 'M.' got '" + author->getAbbreviatedMiddleName() + "'");

	if (author->getAbbreviatedLastName() == "L.")
		testSuccess(test, name, "Author abbreviated last name is ok");
	else
		testError(test, name, "Author abbreviated last name is wrong, expected 'L.' got '" + author->getAbbreviatedLastName() + "'");

	if (author->getAbbreviatedFullName() == "F. M. L.")
		testSuccess(test, name, "Author abbreviated full name is ok");
	else
		testError(test, name, "Author abbreviated full name is wrong, expected 'F. M. L.' got '" + author->getAbbreviatedFullName() + "'");
}

void testBookRegular(Test* test){
	Book* book = new Book("Test title", "Test Author");
	string name = "Testing regular Book";

	if (book->getTitle() == "Test title")
		testSuccess(test, name, "Book title is ok");
	else
		testError(test, name, "Book title is wrong");

	if (book->getAuthor()->getFullName() == "Test Author")
		testSuccess(test, name, "Book author is ok");
	else
		testError(test, name, "Book author is wrong");
}

void testBookEmptyTitle(Test* test){
	string name = "Testing empty title";

	try{
		Book* book = new Book("", "");
		testError(test, name, "Book title is wrong");
	}
	catch (EmptyBookTitleException e){
		string message = e.what();
		testSuccess(test, name, "Book title is ok: " + message);
	}
}

void Test::addSuccess(string testName, string testFile, unsigned int fileLine, string message){
	successes += 1;

		if (printSuccesses)
		cout << "success:" + testFile + ":" + to_string(fileLine) + " -> " + message + " (" + testName + ")" << endl;
}

void Test::addError(string testName, string testFile, unsigned int fileLine, string message){
	errors += 1;

	if (printErrors)
		cout << "error:" + testFile + ":" + to_string(fileLine) + " -> " + message + " (" + testName + ")" << endl;
}

void Test::run(bool printSuccesses, bool printErrors){
	this->printSuccesses = printSuccesses;
	this->printErrors = printErrors;

	cout << "Running tests..." << endl;

	testAuthorRegular(this);
	testBookRegular(this);
	testBookEmptyTitle(this);

	int total = successes + errors;

	cout << "Tests finished. " << endl;
	cout << "Tests run: " << to_string(total) << endl;

	if (successes > 0)
		cout << "Successes: " << to_string(successes) << endl;

	if (errors > 0)
		cout << "Errors: " << to_string(errors) << endl;
}
