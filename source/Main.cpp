#include <iostream>
#include <fstream>

#include "include/ServerHandler.hpp"


int main()
{
	database::DatabaseClient* dbClient = new database::DatabaseClient;
	j_parser::Parser* Parser = new j_parser::Parser;
	server::Server server(8080, "localhost", dbClient, Parser);
}