#include "Database.hpp"
#include <string>
#include "Utils.hpp"
#include <exception>

using std::exception;

using std::string;

using Utils::toString;
using Utils::toInt;

int SQLException::getCode(){
	return code;
}

const char* SQLException::what(){
	return message.c_str();
}

SQLException::SQLException(int code, const char* what){
	this->code = code;
	this->message = string(what);
}

string User::getName(){
	return name;
}

string User::getPassword(){
	return password;
}

User::User(const string name, const string password){
	this->name = name;
	this->password = password;
}

string Server::getName(){
	return name;
}

unsigned int Server::getPort(){
	return port;
}

Server::Server(const string name, const unsigned int port){
	this->name = name;
	this->port = port;
}

Server* Database::getServer(){
	return server;
}

string Database::getPath(){
	return path;
}

User* Database::getUser(){
	return user;
}

void Database::create(){
	ibdb->Create(3);
	connect();
}

void Database::drop(){
	connect();
	ibdb->Drop();
}

void Database::recreate(){
	try{
		drop();
	}
	catch (IBPP::SQLException& e){
		SQLException* ex = new SQLException(e.EngineCode(), e.what());

		if (ex->getCode() != 335544344)
			throw ex;
	}

	create();
}

void Database::connect(){
	if (!connected())
		ibdb->Connect();
}

void Database::disconnect(){
	if (connected())
		ibdb->Disconnect();
}

bool Database::connected(){
	return ibdb->Connected();
}

Database::Database(Server* server, const string path, User* user){
	this->server = server;
	this->path = path;
	this->user = user;

	string sname = server->getName();
	string dname = path;
	string uname = user->getName();
	string pass = user->getPassword();
	unsigned int port = server->getPort();

	if (port > 0)
		sname = sname + "/" + toString(port);

	ibdb = IBPP::DatabaseFactory(
		sname, 
		dname, 
		uname, 
		pass,
		"",
		"",
		"PAGE_SIZE = 4096"
	);
}

Database::~Database(){
	disconnect();
}

Database* Transaction::getDatabase(){
	return database;
}

void Transaction::start(){
	if (ibtr == nullptr)
		ibtr = IBPP::TransactionFactory(
			database->ibdb,
			IBPP::amWrite,
			IBPP::ilReadCommitted,
			IBPP::lrNoWait,
			IBPP::TFF(0)
		);

	if (!started())
		ibtr->Start();
}

void Transaction::commit(){
	ibtr->CommitRetain();
}

void Transaction::rollback(){
	ibtr->Rollback();
	start();
}

bool Transaction::started(){
	return (
		(ibtr == nullptr) ||
		(ibtr->Started())
	);
}

Transaction::Transaction(Database* database){
	this->database = database;
	ibtr = nullptr;
}

Transaction* Statement::getTransaction(){
	return transaction;
}

void Statement::executeSQL(const string sql){
	transaction->start();

	ibst = IBPP::StatementFactory(
		transaction->database->ibdb,
		transaction->ibtr
	);

	ibst->ExecuteImmediate(sql);
}

int Statement::getColumnAsInt(const string name){
	int result;
	ibst->Get(name, result);
	return result;
}

string Statement::getColumnAsString(const string name){
	string result;
	ibst->Get(name, result);
	return result;
}

Statement::Statement(Transaction* transaction){
	this->transaction = transaction;
	ibst = nullptr;
}

string TableMetadata::getName(){
	return name;
}

TableMetadata::TableMetadata(string name){
	this->name = name;
}

TableMetadata::~TableMetadata(){
}

TableMetadata* ColumnMetaData::getTable(){
	return table;
}

string ColumnMetaData::getType(){
	return type;
}

string ColumnMetaData::getName(){
	return name;
}

unsigned int ColumnMetaData::getLength(){
	return length;
}

bool ColumnMetaData::getNotNull(){
	return notNull;
}

ColumnMetaData::ColumnMetaData(
	TableMetadata* table,
	const string type,
	const string name,
	const unsigned int length,
	const bool notNull
){
	this->table = table;
	this->type = type;
	this->name = name;
	this->length = length;
	this->notNull = notNull;
}

ColumnMetaData::ColumnMetaData(ColumnMetaData* other){
	this->table = other->table;
	this->type = other->type;
	this->name = other->name;
	this->length = other->length;
	this->notNull = other->notNull;
}

TableMetadata* Column::getTable(){
	return table;
}

int Column::getAsInteger(){
	return toInt(value);
}

string Column::getAsString(){
	return value;
}

void Column::setAsInteger(const int value){
	this->value = toString(value);
}

void Column::setAsString(const string value){
	this->value = value;
}

Column::Column(
	TableMetadata* table,
	const string type,
	const string name,
	const unsigned int length,
	const bool notNull
): ColumnMetaData(table, type, name, length, notNull){
	this->table = table;
	this->value = "";

	Table* tb = dynamic_cast<Table*>(table);

	if (tb != nullptr)
		tb->addColumn(this);
}

Column::Column(Column* other):
	Column(
		other->getTable(),
		other->getType(),
		other->getName(),
		other->getLength(),
		other->getNotNull()
){
	value = other->getAsString();
}

VarcharColumn::VarcharColumn(
	TableMetadata* table,
	const string name,
	const unsigned int length,
	const bool notNull
): Column(table, "VARCHAR", name, length, notNull){
	setAsString("");
}

IntegerColumn::IntegerColumn(
	TableMetadata* table, 
	const string name,
	const bool notNull
):
	Column(table, "INTEGER", name, 0, notNull){
	setAsInteger(0);
}

IdentifierColumn::IdentifierColumn(
	TableMetadata* table,
	const string name,
	const bool notNull
): Column(table, "INTEGER GENERATED BY DEFAULT AS IDENTITY", name, 0, notNull){
	setAsInteger(0);
}

const char* InvalidTableException::what(){
	return "Table is invalid";
}

TableMetadata* PrimaryKey::getTable(){
	return table;
}

string PrimaryKey::getName(){
	return name;
}

void PrimaryKey::add(Column* column){
	if (table == nullptr)
		table = column->getTable();
	else if (table != column->getTable())
		throw InvalidTableException();

	columns.push_back(column);
}

Column* PrimaryKey::at(const size_t idx){
	return columns.at(idx);
}

const size_t PrimaryKey::size(){
	return columns.size();
}

void PrimaryKey::create(Transaction* transaction){
	Statement* stmt = new Statement(transaction);
	size_t size = columns.size();
	string sql = "ALTER TABLE " + table->getName() + " ADD CONSTRAINT ";

	if (name == "")
		name = table->getName() + "_pk";

	sql = sql + name + " PRIMARY KEY(\n";

	for (size_t i=0; i<size; i++){
		Column* column = columns.at(i);
		sql = sql + "\t" + column->getName();

		if (i<size-1)
			sql = sql + ",";

		sql = sql + "\n";
	}

	sql = sql + ");\n";
	stmt->executeSQL(sql);
}

void PrimaryKey::drop(Transaction* transaction){
	Statement* stmt = new Statement(transaction);
	string sql = "ALTER TABLE " + table->getName() + " DROP CONSTRAINT " + name + ";\n";
	stmt->executeSQL(sql);
}

PrimaryKey::PrimaryKey(TableMetadata* table, const string name){
	this->name = name;
	this->table = table;

	Table* tb = dynamic_cast<Table*>(table);

	if (tb != nullptr)
		tb->setPrimaryKey(this);
}

Column* ForeignKeyReference::getColumn(){
	return column;
}

Column* ForeignKeyReference::getReference(){
	return reference;
}

ForeignKeyReference::ForeignKeyReference(Column* column, Column* reference){
	this->column = column;
	this->reference = reference;
}

TableMetadata* ForeignKey::getTable(){
	return table;
}

string ForeignKey::getName(){
	return name;
}

void ForeignKey::add(ForeignKeyReference* reference){
	if (table != reference->getColumn()->getTable())
		throw InvalidTableException();
	else if (this->reference != reference->getReference()->getTable())
		throw InvalidTableException();

	references.push_back(reference);
}

void ForeignKey::add(Column* column, Column* reference){
	add(new ForeignKeyReference(column, reference));
}

ForeignKeyReference* ForeignKey::at(const size_t idx){
	return references.at(idx);
}

const size_t ForeignKey::size(){
	return references.size();
}

void ForeignKey::create(Transaction* transaction){
	Statement* stmt = new Statement(transaction);
	size_t size = references.size();
	string sql = "ALTER TABLE " + getTable()->getName() + " ADD CONSTRAINT " + name + " FOREIGN KEY(\n";
	string reftable;

	for (size_t i=0; i<size; i++){
		ForeignKeyReference* reference = references.at(i);
		sql = sql + "\t" + reference->getColumn()->getName();
	}

	sql = sql + "\n) REFERENCES " + reference->getName() + "(\n";

	for (size_t i=0; i<size; i++){
		ForeignKeyReference* reference = references.at(i);
		sql = sql + "\t" + reference->getReference()->getName();
	}

	sql = sql + "\n);";
	stmt->executeSQL(sql);
}

void ForeignKey::drop(Transaction* transaction){
	Statement* stmt = new Statement(transaction);
	string sql = "ALTER TABLE " + getTable()->getName() + " DROP CONSTRAINT " + name + ";";
	stmt->executeSQL(sql);
}

ForeignKey::ForeignKey(TableMetadata* table, TableMetadata* reference, const string name){
	this->name = name;
	this->table = table;
	this->reference = reference;

	Table* tb = dynamic_cast<Table*>(table);

	if (tb != nullptr)
		tb->addForeignKey(this);
}

void Table::addColumn(Column* column){
	if (column->getTable() != this)
		throw InvalidTableException();

	columns.push_back(column);
}

void Table::setPrimaryKey(PrimaryKey* primaryKey){
	if (primaryKey->getTable() != this)
		throw InvalidTableException();

	this->primaryKey = primaryKey;
}

void Table::addForeignKey(ForeignKey* foreignKey){
	if (foreignKey->getTable() != this)
		throw InvalidTableException();

	foreignKeys.push_back(foreignKey);
}

Column* Table::columnAt(const size_t idx){
	return columns.at(idx);
}

const size_t Table::columnsSize(){
	return columns.size();
}

PrimaryKey* Table::getPrimaryKey(){
	return primaryKey;
}

ForeignKey* Table::foreignKeyAt(const size_t idx){
	return foreignKeys.at(idx);
}

const size_t Table::foreignKeysSize(){
	return foreignKeys.size();
}

void Table::create(Transaction* transaction){
	Statement* stmt = new Statement(transaction);
	const size_t size = columns.size();
	string sql = "CREATE TABLE " + getName() + "(\n";;

	for (size_t i=0; i<size; i++){
		Column* column = columns.at(i);
		unsigned int length = column->getLength();

		sql = sql + "\t" + column->getName() + " " + column->getType();

		if (length > 0)
			sql = sql + "(" + Utils::toString(length) + ")";

		if (column->getNotNull())
			sql = sql + " NOT NULL";

		if (i < size-1)
			sql = sql + ",";

		sql = sql + "\n";
	}

	sql = sql + ");\n";
	stmt->executeSQL(sql);

	if (primaryKey != nullptr)
		primaryKey->create(transaction);

	for (size_t i=0; i<foreignKeys.size(); i++){
		ForeignKey* foreignKey = foreignKeys.at(i);
		foreignKey->create(transaction);
	}
}

void Table::drop(Transaction* transaction){
	string sql = "DROP TABLE " + getName() + ";\n";
	Statement* stmt = new Statement(transaction);
	stmt->executeSQL(sql);
}

Table::Table(const string name): TableMetadata(name){
	primaryKey = nullptr;
}

TableMetadata* Insert::getTable(){
	return table;
}

ColumnMetaData* Insert::at(const size_t idx){
	return columns.at(idx);
}

size_t Insert::size(){
	return columns.size();
}

void Insert::add(ColumnMetaData* column, int value){
	add(column, Utils::toString(value));
}

void Insert::add(ColumnMetaData* column, string value){
	if (column->getTable() != table)
		throw InvalidTableException();

	columns.push_back(new ColumnMetaData(column));
	values.push_back(value);
}

void Insert::execute(Transaction* transaction){
	Statement* stmt = new Statement(transaction);
	size_t size = columns.size();
	string sql = "INSERT INTO " + table->getName() + "(";

	for (size_t i=0; i<size; i++){
		ColumnMetaData* column = columns.at(i);

		sql = sql + "\t" + column->getName();

		if (i < size - 1)
			sql = sql + ",";

		sql = sql + "\n";
	}

	sql = sql + ")\nVALUES(";

	for (size_t i=0; i<size; i++){
		ColumnMetaData* column = columns.at(i);
		string value = values.at(i);
		bool isVarchar = Utils::toUpper(column->getType()) == "VARCHAR";

		sql = sql + "\t";

		if (isVarchar)
			sql = sql + "'";

		sql = sql + value;

		if (isVarchar)
			sql = sql + "'";

		if (i < size - 1)
			sql = sql + ",";

		sql = sql + "\n";
	}

	sql = sql + ");";
	stmt->executeSQL(sql);
}

Insert::Insert(TableMetadata* table){
	this->table = table;
}

Insert::~Insert(){
	while (columns.size() > 0){
		ColumnMetaData* column = columns.back();
		delete column;
		columns.pop_back();
	}
}

string Clause::SQLText(){
	return "";
}

Clause::Clause(unsigned short int indent){
	this->indent = indent;
}

string AndClause::SQLText(){
	size_t size = clauses.size();
	string result = "";

	for (size_t i=0; i<size; i++){
		for (unsigned short int j=0; j<indent; j++)
			result = result + "\t";

		result = result + "(" + clauses.at(i)->SQLText() + ")";

		if (i < size - 1)
			result = result + " AND\n";
	}

	return result;
}

void AndClause::add(Clause* clause){
	clauses.push_back(clause);
}

AndClause::AndClause(
	Clause* leftClause,
	Clause* rightClause,
	unsigned short int indent
):Clause(indent){
	add(leftClause);
	add(rightClause);
}

string OrClause::SQLText(){
	size_t size = clauses.size();
	string result = "";

	for (size_t i=0; i<size; i++){
		for (unsigned short int j=0; j<indent; j++)
			result = result + "\t";

		result = result + "(" + clauses.at(i)->SQLText() + ")";

		if (i < size - 1)
			result = result + " OR\n";
	}

	return result;
}

void OrClause::add(Clause* clause){
	clauses.push_back(clause);
}

OrClause::OrClause(
	Clause* leftClause,
	Clause* rightClause,
	unsigned short int indent
): Clause(indent){
	add(leftClause);
	add(rightClause);
}

string EqualClause::SQLText(){
	return column->getName() + " = " + value;
}

string NotClause::SQLText(){
	return "(NOT " + clause->SQLText() + ")";
}

NotClause::NotClause(Clause* clause){
	this->clause = clause;
}

EqualClause::EqualClause(
	ColumnMetaData* column,
	int value
):EqualClause(column, Utils::toString(value)){
}

EqualClause::EqualClause(ColumnMetaData* column, string value):Clause(){
	this->column = column;
	this->value = value;
}

size_t Row::size(){
	return columns.size();
}

Column* Row::at(const size_t idx){
	return columns.at(idx);
}

Column* Row::getColumnByName(const string name){
	for (size_t i=0; i<columns.size(); i++){
		Column* column = columns.at(i);

		if (column->getName() == name)
			return column;
	}

	return nullptr;
}

void Select::addColumn(ColumnMetaData* column){
	if (column->getTable() != table)
		throw InvalidTableException();

	columns.push_back(column);
}

void Select::addClause(Clause* clause){
	clauses.push_back(clause);
}

Row* Select::fetch(Transaction* transaction){
	return nullptr;
}

Select::Select(TableMetadata* table){
	this->table = table;
}
