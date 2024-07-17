#include <iostream>
#include <fstream>

#include "include/ServerHandler.hpp"



int main()
{
	database::DatabaseClient DbClient;
	DbClient.AddDataToDb("Egor",
		"123456");
}