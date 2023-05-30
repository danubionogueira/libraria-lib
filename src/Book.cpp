#include "Book.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

const char* EmptyBookTitleException::what(){
	return "Book title can't be empty";
}

string Author::getFullName(){
	return fullName;
}

void Author::setFullName(const string fullName){
	vector<string> names;
	string name = "";
	size_t s = fullName.size();

	for (size_t i=0; i<s; i++){
		if (fullName[i] != ' ')
			name += fullName[i];
		else if (name != ""){
			names.push_back(name);
			name = "";
		}
	}

	if (name != "")
		names.push_back(name);

	s = names.size();

	this->fullName = "";
	firstName = "";
	middleName = "";
	lastName = "";
	abbreviatedFullName = "";
	abbreviatedFirstName = "";
	abbreviatedMiddleName = "";
	abbreviatedLastName = "";

	if (s > 0){
		lastName = names[s - 1];
		abbreviatedLastName += lastName[0];
		abbreviatedLastName += ".";
	}

	if(s > 1){
		firstName = names[0];
		abbreviatedFirstName += firstName[0];
		abbreviatedFirstName += ".";
	}

	if (s > 2){
		for (size_t i=1; i<s - 1; i++){
			if (i > 1){
				middleName += " ";
				abbreviatedMiddleName += " ";
			}

			middleName += names[i];
			abbreviatedMiddleName += names[i][0];
			abbreviatedMiddleName += ".";
		}
	}

	if (firstName != "")
		this->fullName = firstName + " ";

	if (middleName != "")
		this->fullName = this->fullName + middleName + " ";

	if (lastName != "")
		this->fullName = this->fullName + lastName;

	if (abbreviatedFirstName != "")
		abbreviatedFullName = abbreviatedFirstName + " ";

	if (abbreviatedMiddleName != "")
		abbreviatedFullName += abbreviatedMiddleName + " ";

	if (abbreviatedLastName != "")
		abbreviatedFullName += abbreviatedLastName;
}

string Author::getLastName(){
	return lastName;
}

void Author::setLastName(const string lastName){
	this->lastName = lastName;
}

string Author::getFirstName(){
	return firstName;
}

void Author::setFirstName(const string firstName){
	this->firstName = firstName;
}

string Author::getMiddleName(){
	return middleName;
}

void Author::setMiddleName(const string middleName){
	this->middleName = middleName;
}

string Author::getAbbreviatedFullName(){
	return abbreviatedFullName;
}

void Author::setAbbreviatedFullName(const string abbreviatedName){
	this->abbreviatedFullName = abbreviatedFullName;
}

string Author::getAbbreviatedLastName(){
	return abbreviatedLastName;
}

void Author::setAbbreviatedLastName(const string abbreviatedLastName){
	this->abbreviatedLastName = abbreviatedLastName;
}

string Author::getAbbreviatedFirstName(){
	return abbreviatedFirstName;
}

void Author::setAbbreviatedFirstName(const string abbreviatedFirstName){
	this->abbreviatedFirstName = abbreviatedFirstName;
}

string Author::getAbbreviatedMiddleName(){
	return abbreviatedMiddleName;
}

void Author::setAbbreviatedMiddleName(const string abbreviatedMiddleName){
	this->abbreviatedMiddleName = abbreviatedMiddleName;
}

string Author::getPopoularName(){
	return popularName;
}

void Author::setPopularName(const string popularName){
	this->popularName = popularName;
}

Author::Author(string fullName){
	setFullName(fullName);
}

string Book::getTitle(){
	return title;
}

void Book::setTitle(const string title){
	if (title == "")
		throw EmptyBookTitleException();

	this->title = title;
}

Author* Book::getAuthor(){
	return author;
}

void Book::setAuthor(string author){
	this->author = new Author(author);
}

void Book::setAuthor(Author &author){
	this->author = &author;
}

Book::Book(const string title, const string author){
	setTitle(title);
	setAuthor(author);
}

Book::Book(const string title, Author &author){
	setTitle(title);
	setAuthor(author);
}
