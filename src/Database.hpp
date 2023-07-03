#include "../ibpp/ibpp.h"
#include <string>
#include <deque>
#include <exception>

using std::string;
using std::deque;
using std::exception;

class SQLException: public exception{
	private:
		int code;
		string message;
	public:
		int getCode();
		const char* what();

		SQLException(int code, const char* what);
};

class InvalidTableException: public exception{
	public:
		const char* what();
};

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

		void create();
		void drop();
		void recreate();
		void connect();
		void disconnect();
		bool connected();

		Database(Server* server, const string path, User* user);
		~Database();

		friend class Transaction;
		friend class Statement;
};

class Transaction{
	private:
		IBPP::Transaction ibtr;
		Database* database;
	public:
		Database* getDatabase();

		void start();
		void commit();
		void rollback();
		bool started();

		Transaction(Database* database);

		friend class Statement;
};

class Statement{
	private:
		IBPP::Statement ibst;
		Transaction* transaction;
	public:
		Transaction* getTransaction();

		void executeSQL(const string sql);

		int getColumnAsInt(const string name);
		string getColumnAsString(const string name);

		Statement(Transaction* transaction);
};

class ColumnMetaData{
	private:
		string type;
		string name;
		unsigned int length;
		bool notNull;
	public:
		string getType();
		string getName();
		unsigned int getLength();
		bool getNotNull();

		ColumnMetaData(
			const string type,
			const string name,
			const unsigned int length = 0,
			const bool notNull = false
		);
};

class TableMetadata{
	private:
		string name;
	public:
		string getName();

		TableMetadata(const string name);
		virtual ~TableMetadata();
};

class Column: public ColumnMetaData{
	private:
		ColumnMetaData* metadata;
		TableMetadata* table;
		Statement* statement;
	public:
		TableMetadata* getTable();

		int getAsInteger();
		string getAsString();

		void setAsInteger(const int value);
		void setAsString(const string value);

		Column(
			TableMetadata* table,
			const string type,
			const string name,
			const unsigned int length = 0,
			const bool notNull = false
		);
};

class VarcharColumn: public Column{
	public:
		VarcharColumn(
			TableMetadata* table,
			const string name,
			const unsigned int length,
			const bool notNull = false
		);
};

class IntegerColumn: public Column{
	public:
		IntegerColumn(
			TableMetadata* table,
			const string name,
			const bool notNull = false
		);
};

class IdentifierColumn: public Column{
	public:
		IdentifierColumn(
			TableMetadata* table,
			const string name,
			const bool notNull = false
		);
};

class PrimaryKey{
	private:
		string name;
		deque<Column*> columns;
		TableMetadata* table;
	public:
		TableMetadata* getTable();
		string getName();

		void add(Column* column);
		Column* at(const size_t idx);
		const size_t size();

		void create(Transaction* transaction);
		void drop(Transaction* transaction);

		PrimaryKey(TableMetadata* table, const string name = "");
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
		TableMetadata* table;
		TableMetadata* reference;
	public:
		TableMetadata* getTable();
		TableMetadata* getReference();
		string getName();

		void add(ForeignKeyReference* reference);
		void add(Column* column, Column* reference);
		ForeignKeyReference* at(const size_t idx);
		const size_t size();

		void create(Transaction* transaction);
		void drop(Transaction* transaction);

		ForeignKey(TableMetadata* table, TableMetadata* reference, const string name = "");
};

class Table: public TableMetadata{
	private:
		deque<Column*> columns;
		PrimaryKey* primaryKey;
		deque<ForeignKey*> foreignKeys;
	public:
		void addColumn(Column* column);

		void setPrimaryKey(PrimaryKey* primaryKey);
		void addForeignKey(ForeignKey* foreignKey);

		Column* columnAt(const size_t idx);
		const size_t columnsSize();

		PrimaryKey* getPrimaryKey();

		ForeignKey* foreignKeyAt(const size_t idx);
		const size_t foreignKeysSize();

		void create(Transaction* transaction);
		void drop(Transaction* transaction);

		Table(const string name);
};
