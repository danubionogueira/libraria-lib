#include "Test.hpp"
#include "Book.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::to_string;

void testCreationDeletion(Test* test){
	string name = "Testing creations and deletions";

	try{
		Author* author = new Author("Author Name");
		delete author;
		testSuccess(test, name, "Create and delete author ok");
	}
	catch (exception e){
		testError(test, name, "Create and delete author wrong");
	}

	try{
		Authors* authors = new Authors();
		delete authors;
		testSuccess(test, name, "Create and delete authors ok");
	}
	catch (exception e){
		testError(test, name, "Create and delete authos wrong");
	}

	try{
		PublishingDate* publishingDate = new PublishingDate(2000);
		delete publishingDate;
		testSuccess(test, name, "Create and delete publishing date ok");
	}
	catch (exception e){
		testError(test, name, "Create and delete publishing date wrong");
	}
}

void testAuthorRegular(Test* test){
	Author* author = new Author("First Middle Last");
	string name = "Testing regular Author";

	test(test, name, author->getFirstName() == "First", "Author first name is ok", "Author first name is wrong");
	test(test, name, author->getMiddleName() == "Middle", "Author middle name is ok", "Author middle name is wrong");
	test(test, name, author->getLastName() == "Last", "Author last name is ok", "Author last name is wrong");
	test(test, name, author->getFullName() == "First Middle Last", "Author full name is ok", "Author full name is wrong");
	test(test, name, author->getAbbreviatedFirstName() == "F.", "Author abbreviated first name is ok", "Author abbreviated first name is wrong, expected 'F.' got '" + author->getAbbreviatedFirstName() + "'");
	test(test, name, author->getAbbreviatedMiddleName() == "M.", "Author abbreviated middle name is ok", "Author abbreviated middle name is wrong, expected 'M.' got '" + author->getAbbreviatedMiddleName() + "'");
	test(test, name, author->getAbbreviatedLastName() == "L.", "Author abbreviated last name is ok", "Author abbreviated last name is wrong, expected 'L.' got '" + author->getAbbreviatedLastName() + "'");
	test(test, name, author->getAbbreviatedFullName() == "F. M. L.", "Author abbreviated full name is ok", "Author abbreviated full name is wrong, expected 'F. M. L.' got '" + author->getAbbreviatedFullName() + "'");
	test(test, name, author->getReferenceName() == "LAST, F. M.", "Author reference name is ok", "Author reference name is wrong");
}

void testAuthorsRegular(Test* test){
	string name = "Testing regular Book";

	Authors* authors = new Authors;
	Author* author = new Author("Test Author");
	Author* another = new Author("Another Author");
	Author* intruder = new Author("Intruder Author");
	Author* onemore = new Author("One More Author");

	authors->add(author);
	authors->add(another);
	authors->add(intruder);

	authors->insert(1, onemore);
	authors->remove(3);

	test(test, name, authors->size() == 3, "Book quantity of authors is ok", "Book quantity of authors is wrong");
	test(test, name, authors->get(0)->getFullName() == "Test Author", "Book first author is ok", "Book first author is wrong");
	test(test, name, authors->get(1)->getFullName() == "One More Author", "Book second author is ok", "Book second author is wrong");
	test(test, name, authors->get(2)->getFullName() == "Another Author", "Book third author is ok", "Book third author is wrong");

	delete author;
	delete another;
	delete intruder;
	delete onemore;
	delete authors;
}

void testBookRegular(Test* test){
	string name = "Testing regular Book";

	Author* author = new Author("Test Author");
	Book* book = new Book("Test title");

	book->getAuthors()->add(author);

	book->setPublisher("Some Publisher");
	book->setPublishingDate(1999);

	book->setPublishingPlace("Some Place");

	test(test, name, book->getTitle() == "Test title", "Book title is ok", "Book title is wrong");
	test(test, name, book->getAuthors()->get(0)->getFullName() == "Test Author", "Book first author is ok", "Book first author is wrong");
	test(test, name, book->getPublisher() == "Some Publisher", "Book publisher is ok", "Book publisher is wrong");
	test(test, name, book->getPublishingDate()->getYear() == 1999, "Book publishing year ok", "Book publishing year wrong");
	test(test, name, book->getPublishingDate()->getMonth() == mn_unknown, "Book publishing month ok", "Book publishing month wrong");
	test(test, name, book->getPublishingDate()->getDay() == md_unknown, "Book publishing day ok", "Book publishing day wrong");
	test(test, name, book->getPublishingPlace() == "Some Place", "Book publishing place ok", "Book publishing place wrong");

	delete author;
	delete book;
}

void testBookBroken(Test* test){
	string name = "Testing empty title";

	try{
		Book* book = new Book("Testing Book");
		delete book;
		testSuccess(test, name, "Book deletion is ok");
	}
	catch (exception e){
		testError(test, name, "Book deletion is wrong");
	}

	try{
		Book* book = new Book("");
		testError(test, name, "Book title is wrong");
		delete book;
	}
	catch (EmptyBookTitleException e){
		string message = e.what();
		testSuccess(test, name, "Book title is ok: " + message);
	}
}

void Test::addSuccess(string testName, string testFile, unsigned int fileLine, string message){
	successes += 1;

		if (printSuccesses)
		cout << "success:" + testFile + ":" + to_string(fileLine) + " -> " + testName + " -> " + message << endl;
}

void Test::addError(string testName, string testFile, unsigned int fileLine, string message){
	errors += 1;

	if (printErrors)
		cout << "error:" + testFile + ":" + to_string(fileLine) + " -> " + testName + " -> " + message << endl;
}

void Test::run(bool printSuccesses, bool printErrors){
	this->printSuccesses = printSuccesses;
	this->printErrors = printErrors;

	cout << "Running tests..." << endl;

	testCreationDeletion(this);
	testAuthorRegular(this);
	testBookRegular(this);
	testBookBroken(this);

	int total = successes + errors;

	cout << "Tests finished. " << endl;
	cout << "Tests run: " << to_string(total) << endl;

	if (successes > 0)
		cout << "Successes: " << to_string(successes) << endl;

	if (errors > 0)
		cout << "Errors: " << to_string(errors) << endl;
}
