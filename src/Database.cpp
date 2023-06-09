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

Transaction::Transaction(Database* database){
	this->database = database;

	ibtr = IBPP::TransactionFactory(database->ibdb, IBPP::amWrite,
                   IBPP::ilReadCommitted, IBPP::lrNoWait, IBPP::TFF(0));
}
