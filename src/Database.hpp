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

class TableMetadata{
	private:
		string name;
	public:
		string getName();

		TableMetadata(const string name);
		virtual ~TableMetadata();
};

class ColumnMetaData{
	private:
		TableMetadata* table;
		string type;
		string name;
		unsigned int length;
		bool notNull;
	public:
		TableMetadata* getTable();
		string getType();
		string getName();
		unsigned int getLength();
		bool getNotNull();

		ColumnMetaData(
			TableMetadata* table,
			const string type,
			const string name,
			const unsigned int length = 0,
			const bool notNull = false
		);

		ColumnMetaData(ColumnMetaData* other);
};

class Column: public ColumnMetaData{
	private:
		ColumnMetaData* metadata;
		TableMetadata* table;
		string value;
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

		Column(Column* other);
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

class Insert{
	private:
		TableMetadata* table;
		deque <ColumnMetaData*> columns;
		deque <string> values;
	public:
		TableMetadata* getTable();
		ColumnMetaData* at(const size_t idx);
		size_t size();

		void add(ColumnMetaData* column, int value);
		void add(ColumnMetaData* column, string value);
		void execute(Transaction* transaction);

		Insert(TableMetadata* table);
		~Insert();
};

class Clause {
	protected:
		unsigned short int indent;
	public:
		virtual string SQLText();

		Clause(unsigned short int indent = 1);
};

class AndClause: public Clause {
	private:
		deque<Clause*> clauses;
	public:
		string SQLText();
		void add(Clause* clause);

		AndClause(Clause* leftClause, Clause* rightClause, unsigned short int indent = 1);
};

class OrClause: public Clause {
	private:
		deque<Clause*> clauses;
	public:
		string SQLText();
		void add(Clause* clause);

		OrClause(Clause* leftClause, Clause* rightClause, unsigned short int indent = 1);
};

class NotClause: public Clause {
	private:
		Clause* clause;
	public:
		string SQLText();

		NotClause(Clause* clause);
};

class EqualClause: public Clause {
	private:
		ColumnMetaData* column;
		string value;
	public:
		string SQLText();

		EqualClause(ColumnMetaData* column, int value);
		EqualClause(ColumnMetaData* column, string value);
};

class LikeClause: public Clause {
	public:
		string SQLText();

		LikeClause(ColumnMetaData* column, string value);
};

class LesserThanClause: public Clause {
	public:
		string SQLText();

		LesserThanClause(ColumnMetaData* column, int value);
};

class LesserThanOrEqualClause: public Clause {
	public:
		string SQLText();

		LesserThanOrEqualClause(ColumnMetaData* column, int value);
};

class GreaterThanClause: public Clause {
	public:
		string SQLText();

		GreaterThanClause(ColumnMetaData* column, int value);
};

class GreaterThanOrEqualClause: public Clause {
	public:
		string SQLText();

		GreaterThanOrEqualClause(ColumnMetaData* column, int value);
};

class BetweenClause: public Clause {
	public:
		string SQLText();

		BetweenClause(ColumnMetaData* column, int minValue, int maxValue);
};

class IsNullClause: public Clause {
	public:
		string SQLText();

		IsNullClause(ColumnMetaData* column);
};

class IsNotNullClause: public Clause {
	public:
		string SQLText();

		IsNotNullClause(ColumnMetaData* column);
};

class Row {
	private:
		deque<Column*> columns;
	public:
		size_t size();
		Column* at(const size_t idx);

		Column* getColumnByName(const string name);

		Row();
};

class Select {
	private:
		TableMetadata* table;
		deque<ColumnMetaData*> columns;
		deque<Clause*> clauses;
	public:
		void addColumn(ColumnMetaData* column);
		void addClause(Clause* clause);

		Row* fetch(Transaction* transaction);

		Select(TableMetadata* table);
};
