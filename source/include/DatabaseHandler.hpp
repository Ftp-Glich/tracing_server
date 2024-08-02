#pragma once

#include <cstdint>
#include <string>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"
#include "crypt.h"
#include "jsonParser.hpp"

namespace database
{
constexpr char kMongoDbUri[] = "mongodb://0.0.0.0:27017";
constexpr char kDatabaseName[] = "trace_db";
constexpr char kCollectionName[] = "LoginData";


class DatabaseClient
{
 public:
    DatabaseClient()
    : uri(mongocxx::uri(kMongoDbUri)),
      client(mongocxx::client(uri)),
      db(client[kDatabaseName]) {}
    
    bool AddDataToDb(const std::string &login,
                          const std::string &password)
    {
        mongocxx::collection collection = db["LoginData"];
        auto builder = bsoncxx::builder::stream::document{};
        const char* salt = "xy"; 
        std::string hashed_password = crypt(password.c_str(), salt);
        bsoncxx::document::value doc_to_add =
            builder << "login" << login  
             << "password" << hashed_password
             << "salt" << salt
             << bsoncxx::builder::stream::finalize;
        collection.insert_one(doc_to_add.view());
        return true;
    }
    bool RemoveDataFromDb(const std::string &character_id)
    {
        return true;
    }
    bool CheckData(const std::string& login, const std::string& password)
    {
        mongocxx::collection collection = db["LoginData"];
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "login" << login; 
        auto coursor = collection.find(filter_builder.view());
        for(auto doc: coursor)
        {			
            _jParser->Parse(bsoncxx::to_json(doc, bsoncxx::ExtendedJsonMode::k_relaxed));
            if(strcmp((_jParser->getPassword()).c_str(), crypt(login.c_str(), (_jParser->getSalt()).c_str()))) return true;
        }
        return false;
    }
 private:
 	j_parser::Parser* _jParser = jParser();
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database db; 
};
}