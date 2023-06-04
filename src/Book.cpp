#include "Book.hpp"
#include <string>
#include <vector>
#include "Utils.hpp"

using std::string;
using std::vector;
using Utils::toUpper;

const char* EmptyBookTitleException::what(){
	return "Book title can't be empty";
}

const char* MessageException::what(){
	return message.c_str();
}

string MessageException::getMessage(){
	return message;
}

MessageException::MessageException(string message){
	this->message = message;
}

InvalidPublishingDateException::InvalidPublishingDateException(string message): MessageException(message){
}

const char* InvalidISBNException::what(){
	return "ISBN number is invalid";
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
	referenceName = "";

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

	if (lastName != ""){
		this->fullName = this->fullName + lastName;
		referenceName = toUpper(lastName);
	}

	if (abbreviatedFirstName != ""){
		abbreviatedFullName = abbreviatedFirstName + " ";

		if (lastName != "")
			referenceName += ", ";

		referenceName += abbreviatedFirstName;
	}

	if (abbreviatedMiddleName != ""){
		abbreviatedFullName += abbreviatedMiddleName + " ";

		if (firstName != "")
			referenceName += " ";
		else if (lastName != "")
			referenceName += ", ";

		referenceName += abbreviatedMiddleName;
	}

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

string Author::getReferenceName(){
	return referenceName;
}

void Author::setReferenceName(const string referenceName){
	this->referenceName = referenceName;
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

Author::Author(Author* author){
	fullName = author->fullName;
	lastName = author->lastName;
	firstName = author->firstName;
	middleName = author->middleName;
	abbreviatedFullName = author->abbreviatedFullName;
	abbreviatedLastName = author->abbreviatedLastName;
	abbreviatedFirstName = author->abbreviatedFirstName;
	abbreviatedMiddleName = author->abbreviatedMiddleName;
	referenceName = author->referenceName;
	popularName = author->popularName;
}

void Authors::add(Author* author){
	elements.push_back(author);
}

void Authors::insert(const size_t idx, Author* author){
	elements.insert(elements.begin() + idx, author);
}

void Authors::remove(const size_t idx){
	elements.erase(elements.begin() + idx);
}

Author* Authors::get(const size_t idx){
	return elements[idx];
}

const size_t Authors::size(){
	return elements.size();
}

Author* Authors::search(const string lastName, const string firstName, const string middleName){
	for (int i = 0; i<elements.size(); i++){
		Author* author = elements[i];

		string last = toUpper(lastName);
		string first = toUpper(firstName);
		string middle = toUpper(middleName);
		string aLast = toUpper(author->getLastName());
		string aFirst = toUpper(author->getFirstName());
		string aMiddle = toUpper(author->getMiddleName());

		if (
			(aLast == last) &&
			(aFirst == first) &&
			(aMiddle == middle)
		)
			return author;
	}

	return NULL;
}

int PublishingDate::getYear(){
	return year;
}

void PublishingDate::setYear(const int year){
	this->year = year;
}

nmonth PublishingDate::getMonth(){
	return month;
}

void PublishingDate::setMonth(const nmonth month){
	this->month = month;
	setDay(day);
}

nmday PublishingDate::getDay(){
	return day;
}

void PublishingDate::setDay(const nmday day){
	if ((month == mn_unknown) && (day != md_unknown))
		throw InvalidPublishingDateException("Cannot define a day for an unknow month");
	else if (day > md_30){
		switch (month){
			case mn_february:
			case mn_april:
			case mn_june:
			case mn_september:
			case mn_november:
				throw InvalidPublishingDateException("Invalid month day");
		}
	}
	else if (day > md_28){
		if (
			(month == mn_february) &&
			((year % 4) != 0) &&
			((year % 400) != 0)
		)
			throw InvalidPublishingDateException("Invalid month day");
	}

	this->day = day;
}

PublishingDate::PublishingDate(const int year, const nmonth month, const nmday day){
	setYear(year);
	setMonth(month);
	setDay(day);
}

PublishingDate::PublishingDate(PublishingDate &publishingDate){
	setYear(publishingDate.getYear());
	setMonth(publishingDate.getMonth());
	setDay(publishingDate.getDay());
}

string Book::getTitle(){
	return title;
}

void Book::setTitle(const string title){
	if (title == "")
		throw EmptyBookTitleException();

	this->title = title;
}

Authors* Book::getAuthors(){
	return authors;
}

string Book::getPublisher(){
	return publisher;
}

void Book::setPublisher(const string publisher){
	this->publisher = publisher;
}

PublishingDate* Book::getPublishingDate(){
	return publishingDate;
}

void Book::setPublishingDate(PublishingDate &publishingDate){
	this->publishingDate = new PublishingDate(publishingDate);
}

void Book::setPublishingDate(const int year, const nmonth month, const nmday day){
	this->publishingDate = new PublishingDate(year, month, day);
}

string Book::getPublishingPlace(){
	return publishingPlace;
}

void Book::setPublishingPlace(const string publishingPlace){
	this->publishingPlace = publishingPlace;
}

unsigned short int Book::getEdition(){
	return edition;
}

void Book::setEdition(const unsigned short int edition){
	this->edition = edition;
}

string Book::getISBN10(){
	return isbn10;
}

void Book::setISBN10(const string isbn10){
	this->isbn10 = isbn10;
}

string Book::getISBN13(){
	return isbn13;
}

void Book::setISBN13(const string isbn13){
	this->isbn13 = isbn13;
}

string Book::getISBN(){
	if (isbn13 != "")
		return getISBN13();
	else if (isbn10 != "")
		return getISBN10();

	return "";
}

void Book::setISBN(const string isbn){
	if (isbn.size() == 13)
		setISBN13(isbn);
	else if (isbn.size() == 10)
		setISBN10(isbn);
	else
		throw InvalidISBNException();
}

Book::Book(const string title){
	authors = new Authors();
	publishingDate = NULL;
	setTitle(title);
}

Book::~Book(){
	delete authors;

	if (publishingDate != NULL)
		delete publishingDate;
}

void Books::add(Book* book){
	elements.push_back(book);
}

void Books::insert(const size_t idx, Book* book){
	elements.insert(elements.begin() + idx, book);
}

void Books::remove(const size_t idx){
	elements.erase(elements.begin() + idx);
}

Book* Books::get(const size_t idx){
	return elements[idx];
}

const size_t Books::size(){
	return elements.size();
}
