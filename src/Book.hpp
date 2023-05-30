#pragma once
#include <string>
#include <exception>

using std::string;
using std::exception;

class EmptyBookTitleException: public exception{
	public:
		const char* what();
};

class Author{
	private:
		string fullName;
		string lastName;
		string firstName;
		string middleName;
		string abbreviatedFullName;
		string abbreviatedLastName;
		string abbreviatedFirstName;
		string abbreviatedMiddleName;
		string referenceName;
		string popularName;
	public:
		string getFullName();
		void setFullName(const string fullName);

		string getLastName();
		void setLastName(const string lastName);

		string getFirstName();
		void setFirstName(const string firstName);

		string getMiddleName();
		void setMiddleName(const string middleName);

		string getAbbreviatedFullName();
		void setAbbreviatedFullName(const string abbreviatedFullName);

		string getAbbreviatedLastName();
		void setAbbreviatedLastName(const string abbreviatedLastName);

		string getAbbreviatedFirstName();
		void setAbbreviatedFirstName(const string abbreviatedFirstName);

		string getAbbreviatedMiddleName();
		void setAbbreviatedMiddleName(const string abbreviatedMiddleName);

		string getPopoularName();
		void setPopularName(const string popularName);

		Author(string fullName);
};

class Book{
	protected:
		string title;
		Author* author;
	public:
		string getTitle();
		void setTitle(const string title);

		Author* getAuthor();
		void setAuthor(const string author);
		void setAuthor(Author &author);

		Book(const string title, const string author);
		Book(const string title, Author &author);
};
