#include "Database.hpp"
#include <string>

using std::string;
using std::to_string;

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
		sname = sname + "/" + to_string(port);

	ibdb = IBPP::DatabaseFactory(sname, dname, uname, pass,
        "", "", "PAGE_SIZE = 4096");
}

Database* Transaction::getDatabase(){
	return database;
}

Transaction::Transaction(Database* database){
	this->database = database;

	ibtr = IBPP::TransactionFactory(database->ibdb, IBPP::amWrite,
                   IBPP::ilReadCommitted, IBPP::lrNoWait, IBPP::TFF(0));
}

Transaction* Statement::getTransaction(){
	return transaction;
}

Statement::Statement(Transaction* transaction){
	this->transaction = transaction;

	ibst = IBPP::StatementFactory(
		transaction->database->ibdb,
		transaction->ibtr
	);
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

ColumnMetaData::ColumnMetaData(const string type, const string name, unsigned int length){
	this->type = type;
	this->name = name;
	this->length = length;
}

string TableMetadata::getName(){
	return name;
}

TableMetadata::TableMetadata(string name){
	this->name = name;
}

TableMetadata* Column::getTable(){
	return table;
}

int Column::getAsInteger(){
	int result;
	statement->ibst->Get(getName(), result);
	return result;
}

string Column::getAsString(){
	string result;
	statement->ibst->Get(getName(), result);
	return result;
}

Column::Column(
	TableMetadata* table,
	const string type,
	const string name,
	const unsigned int length
): ColumnMetaData(type, name, length){
	this->table = table;
}

VarcharColumn::VarcharColumn(
	TableMetadata* table,
	const string name,
	unsigned int length
): Column(table, "VARCHAR", name, length){
}

IntegerColumn::IntegerColumn(TableMetadata* table, const string name):
	Column(table, "INTEGER", name, 0){
}

AutoIncrementColumn::AutoIncrementColumn(
	TableMetadata* table,
	const string name
): Column(table, "AUTOINCREMENT", name, 0){
}
