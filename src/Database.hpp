#include "../ibpp/ibpp.h"
#include <string>
#include <deque>

using std::string;
using std::deque;

class User{
	private:
		string name;
		string password;
	public:
		string getName();
		string getPassword();

		User(const string name, const string password);
};

class Server{
	private:
		string name;
		unsigned int port;
	public:
		string getName();
		unsigned int getPort();

		Server(const string name, const unsigned int port = 0);
};

class Database{
	private:
		IBPP::Database ibdb;

		Server* server;
		string path;
		User* user;
	public:
		Server* getServer();
		string getPath();
		User* getUser();

		Database(Server* server, const string path, User* user);

		friend class Transaction;
};

class Transaction{
	private:
		IBPP::Transaction ibtr;
		Database* database;
	public:
		Transaction(Database* database);
};

class ColumnMetaData{
	private:
		string type;
		string name;
		unsigned int length;
	public:
		string getType();
		string getName();
		unsigned int getLength();

		ColumnMetaData(const string type, const string name, unsigned int length = 0);
};

class TableMetadata{
	private:
		string name;
	public:
		string getName();

		TableMetadata(const string name);
};

class Column: public ColumnMetaData{
	public:
		TableMetadata* getTable();

		int getAsInt();
		unsigned int getAsUInt();
		string getAsString();

		void setAsInt(const int value);
		void setAsUInt(const unsigned int value);
		void setAsString(const string value);

		Column(
			TableMetadata* table,
			const string type,
			const string name,
			const unsigned int length = 0
		);
};

class VarcharColumn: public Column{
	private:
		unsigned int length;
	public:
		VarcharColumn(TableMetadata* table, const string name, unsigned int length);
};

class IntColumn: public Column{
	public:
		IntColumn(TableMetadata* table, const string name);
};

class AutoIncrementColumn: public IntColumn{
	public:
		AutoIncrementColumn(TableMetadata* table, const string name);
};

class PrimaryKey{
	private:
		string name;
		deque<Column*> columns;
	public:
		string getName();

		void addColumn(Column* column);

		void create();
		void drop();

		PrimaryKey(const string name = "");
};

class ForeignKeyReference{
	private:
		Column* column;
		Column* reference;
	public:
		Column* getColumn();
		Column* getReference();

		ForeignKeyReference(Column* column, Column* reference);
};

class ForeignKey{
	private:
		string name;
		deque<ForeignKeyReference*> references;
	public:
		string getName();

		void addReference(ForeignKeyReference* reference);

		ForeignKey(const string name = "");
};

class Table: public TableMetadata{
	private:
		deque<ColumnMetaData*> columns;
		PrimaryKey* primaryKey;
		deque<ForeignKey*> foreignKeys;
	public:
		void addColumn(ColumnMetaData* column);
		void setPrimaryKey(PrimaryKey* primaryKey);
		void addForeignKey(ForeignKey* foreignKey);

		void create();
		void drop();
};
