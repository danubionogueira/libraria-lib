#include "../ibpp/ibpp.h"
#include <string>

using std::string;

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
