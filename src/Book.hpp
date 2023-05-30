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

typedef enum{
	mn_unknown,
	mn_january, mn_february, mn_march, mn_april, mn_may, mn_june, mn_july,
	mn_august, mn_september, mn_octorber, mn_november, mn_december
} nmonth;

typedef enum{
	md_unknown,
	md_1, md_2, md_3, md_4, md_5, md_6, md_7, md_8, md_9, md_10,
	md_11, md_12, md_13, md_14, md_15, md_16, md_17, md_18, md_19, md_20,
	md_21, md_22, md_23, md_24, md_25, md_26, md_27, md_28, md_29, md_30,
	md_31
} nmday;

class PublishingDate{
	private:
		int year;
		nmonth month;
		nmday day;
	public:
		int getYear();
		void setYear(const int year);

		nmonth getMonth();
		void setMonth(const nmonth month);

		nmday getDay();
		void setDay(const nmday day);

		PublishingDate(const int year, const nmonth month=mn_unknown, const nmday day=md_unknown);
		PublishingDate(PublishingDate &publishingDate);
};

class Book{
	protected:
		string title;
		Author* author;
		string publisher;
		PublishingDate* publishingDate;
	public:
		string getTitle();
		void setTitle(const string title);

		Author* getAuthor();
		void setAuthor(const string author);
		void setAuthor(Author &author);

		string getPublisher();
		void setPublisher(const string publisher);
		
		PublishingDate* getPublishingDate();
		void setPublishingDate(PublishingDate &publishingDate);
		void setPublishingDate(const int year, const nmonth month=mn_unknown, const nmday day=md_unknown);

		Book(const string title, const string author);
		Book(const string title, Author &author);
		~Book();
};
