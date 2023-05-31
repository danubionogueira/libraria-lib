#include "Book.hpp"
#include <string>
#include <vector>
#include <cstring>

using std::string;
using std::vector;

string toUpper(const string str){
	char* result = (char*)str.c_str();

	for (int i=0; i<strlen(result); i++)
		result[i] = toupper(result[i]);

	return string(result);
}

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

void Book::addAuthor(Author* author){
	authors.push_back(new Author(author));
}

Author* Book::getAuthor(const size_t idx){
	return authors[0];
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

Book::Book(const string title){
	setTitle(title);
}

Book::~Book(){
	delete this->publishingDate;
}
