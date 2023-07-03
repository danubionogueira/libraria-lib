#include "Test.hpp"
#include "Book.hpp"
#include "Database.hpp"
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

	try{
		Book* book = new Book("Test Book");
		delete book;
		testSuccess(test, name, "Create and delete book ok");
	}
	catch (exception e){
		testError(test, name, "Create and delete book wrong");
	}

	try{
		Books* books = new Books();
		delete books;
		testSuccess(test, name, "Create and delete books ok");
	}
	catch (exception e){
		testError(test, name, "Create and delete books wrong");
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
	string name = "Testing regular Authors";

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
	test(test, name, authors->at(0)->getFullName() == "Test Author", "Book first author is ok", "Book first author is wrong");
	test(test, name, authors->at(1)->getFullName() == "One More Author", "Book second author is ok", "Book second author is wrong");
	test(test, name, authors->at(2)->getFullName() == "Another Author", "Book third author is ok", "Book third author is wrong");

	delete author;
	delete another;
	delete intruder;
	delete onemore;
	delete authors;
}

void testAuthorsSearch(Test* test){
	string name = "Testing Authors Search";
	Authors* authors = new Authors();

	Author* first = new Author("First Middle Last");
	Author* second = new Author("Second Middle Last");
	Author* third = new Author("Third Mid Las");
	Author* result = NULL;

	authors->add(first);
	authors->add(second);
	authors->add(third);

	result = authors->search("Last", "First", "M.");
	test(test, name, result->getFullName() == first->getFullName(), "Search first author ok", "Search first author wrong");

	result = authors->search("Last", "S.", "M.");
	test(test, name, result->getFullName() == second->getFullName(), "Search second author ok", "Search second author wrong");

	result = authors->search("Las", "Third", "Mid");
	test(test, name, result->getFullName() == third->getFullName(), "Search third author ok", "Search third author wrong");

	result = authors->search("Last", "Third", "Middle");
	test(test, name, result == NULL, "Search invalid author ok", "Search invalid author wrong");

	delete first;
	delete second;
	delete third;
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

	book->setEdition(2);

	book->setISBN("1234567890");
	book->setISBN("1234567890123");

	test(test, name, book->getTitle() == "Test title", "Book title is ok", "Book title is wrong");
	test(test, name, book->getAuthors()->at(0)->getFullName() == "Test Author", "Book first author is ok", "Book first author is wrong");
	test(test, name, book->getPublisher() == "Some Publisher", "Book publisher is ok", "Book publisher is wrong");
	test(test, name, book->getPublishingDate()->getYear() == 1999, "Book publishing year ok", "Book publishing year wrong");
	test(test, name, book->getPublishingDate()->getMonth() == mn_unknown, "Book publishing month ok", "Book publishing month wrong");
	test(test, name, book->getPublishingDate()->getDay() == md_unknown, "Book publishing day ok", "Book publishing day wrong");
	test(test, name, book->getPublishingPlace() == "Some Place", "Book publishing place ok", "Book publishing place wrong");
	test(test, name, book->getEdition() == 2, "Book edition ok", "Book edition wrong");
	test(test, name, book->getISBN10() == "1234567890", "Book ISBN10 ok", "Book ISBN10 wrong");
	test(test, name, book->getISBN13() == "1234567890123", "Book ISBN13 ok", "Book ISBN13 wrong");
	test(test, name, book->getISBN() == book->getISBN13(), "Book ISBN ok", "Book ISBN wrong");

	delete author;
	delete book;
}

void testBooksRegular(Test* test){
	string name = "Testing regular Books";

	Books* books = new Books;
	Book* book = new Book("First Book");
	Book* another = new Book("Another Book");
	Book* intruder = new Book("Intruder Book");
	Book* onemore = new Book("One More Book");

	books->add(book);
	books->add(another);
	books->add(intruder);

	books->insert(1, onemore);
	books->remove(3);

	test(test, name, books->size() == 3, "Books quantity is ok", "Books quantity is wrong");
	test(test, name, books->at(0)->getTitle() == "First Book", "First book title is ok", "First book title is wrong");
	test(test, name, books->at(1)->getTitle() == "One More Book", "Second book title is ok", "Second book title is wrong");
	test(test, name, books->at(2)->getTitle() == "Another Book", "Third book title is ok", "Third book title is wrong");

	delete book;
	delete another;
	delete intruder;
	delete onemore;
	delete books;
}

void testBooksSearch(Test* test){
	string name = "Testing Books search";
	Books* books = new Books();

	Author* firstA = new Author("First Middle Last");
	Author* secondA = new Author("Second Middle Last");

	Book* firstB = new Book("First Book");
	Book* secondB = new Book("Second Book");
	Book* result = NULL;

	firstB->getAuthors()->add(firstA);
	secondB->getAuthors()->add(secondA);

	books->add(firstB);
	books->add(secondB);

	result = books->search(firstA, "First Book");
	test(test, name, result->getTitle() == firstB->getTitle(), "Search first book ok", "Search first book wrong");

	result = books->search(secondA, "Second Book");
	test(test, name, result->getTitle() == secondB->getTitle(), "Search second book ok", "Search second book wrong");

	result = books->search(secondA, "First Book");
	test(test, name, result == NULL, "Search invalid book ok", "Search invalid book wrong");

	firstB->setISBN("1234567890123");
	secondB->setISBN("1234567890");

	result = books->search("1234567890123");
	test(test, name, result->getTitle() == firstB->getTitle(), "Search first book ISBN ok", "Search first book ISBN wrong");

	result = books->search("1234567890");
	test(test, name, result->getTitle() == secondB->getTitle(), "Search second book ISBN ok", "Search second book ISBN wrong");

	result = books->search("3210987654321");
	test(test, name, result == NULL, "Search invalid book ISBN ok", "Search invalid book ISBN wrong");

	delete firstA;
	delete secondA;
	delete firstB;
	delete secondB;
	delete books;
}

void testBookBroken(Test* test){
	string name = "Testing empty title";

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

void testDatabase(Test* test){
	string name = "Testing database";
	User* dba = new User("SYSDBA", "masterkey");
	Server* srv = new Server("localhost");
	Database* db = new Database(srv, "/var/firebird/libraria.fdb", dba);
	Transaction* tr = new Transaction(db);

	Table* author = new Table("author");
	Column* authorId = new IdentifierColumn(author, "id", true);
	Column* authorLastName = new VarcharColumn(author, "last_name", 127, true);
	Column* authorFirstName = new VarcharColumn(author, "first_name", 127);
	Column* authorMiddleName = new VarcharColumn(author, "middle_name", 127);
	Column* authorLastNameAbbreviation = new VarcharColumn(author, "last_name_abbreviation", 127);
	Column* authorFirstNameAbbreviation = new VarcharColumn(author, "first_name_abbreviation", 127);
	Column* authorMiddleNameAbbreviation = new VarcharColumn(author, "middle_name_abbreviation", 127);
	PrimaryKey* authorPK = new PrimaryKey(author, "author_pk");

	Table* book = new Table("book");
	Column* bookId = new IdentifierColumn(book, "id", true);
	Column* bookTitle = new VarcharColumn(book, "title", 255, true);
	PrimaryKey* bookPK = new PrimaryKey(book, "book_pk");

	Table* bookAuthor = new Table("book_author");
	Column* bookAuthorBookId = new IntegerColumn(bookAuthor, "book_id", true);
	Column* bookAuthorAuthorId = new IntegerColumn(bookAuthor, "author_id", true);
	PrimaryKey* bookAuthorPK = new PrimaryKey(bookAuthor, "book_author_pk");
	ForeignKey* bookAuthorBookFK = new ForeignKey(bookAuthor, book, "book_author_book_fk");
	ForeignKey* bookAuthorAuthorFK = new ForeignKey(bookAuthor, author, "book_author_author_fk");

	authorPK->add(authorId);
	bookPK->add(bookId);
	bookAuthorPK->add(bookAuthorBookId);
	bookAuthorPK->add(bookAuthorAuthorId);

	bookAuthorBookFK->add(bookAuthorBookId, bookId);
	bookAuthorAuthorFK->add(bookAuthorAuthorId, authorId);

	db->recreate();
	testSuccess(test, name, "Recreated Libraria database");

	db->connect();
	testSuccess(test, name, "Connected Libraria database");

	author->create(tr);
	testSuccess(test, name, "Created Author table");

	book->create(tr);
	testSuccess(test, name, "Created Book table");

	bookAuthor->create(tr);
	testSuccess(test, name, "Created Book Author table");

	bookAuthorBookFK->drop(tr);
	bookAuthorAuthorFK->drop(tr);
	testSuccess(test, name, "Dropped all foreign keys");

	bookAuthorPK->drop(tr);
	bookPK->drop(tr);
	authorPK->drop(tr);
	testSuccess(test, name, "Dropped all primary keys");

	bookAuthor->drop(tr);
	book->drop(tr);
	author->drop(tr);
	testSuccess(test, name, "Dropped all tables");

	db->drop();
	testSuccess(test, name, "Dropped Libraria database");
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
	testAuthorsRegular(this);
	testAuthorsSearch(this);
	testBookRegular(this);
	testBookBroken(this);
	testBooksRegular(this);
	testBooksSearch(this);
	testDatabase(this);

	int total = successes + errors;

	cout << "Tests finished. " << endl;
	cout << "Tests run: " << to_string(total) << endl;

	if (successes > 0)
		cout << "Successes: " << to_string(successes) << endl;

	if (errors > 0)
		cout << "Errors: " << to_string(errors) << endl;
}
