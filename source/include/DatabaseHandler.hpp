#pragma once

#include <cstdint>
#include <string>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

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
        bsoncxx::document::value doc_to_add =
            builder << "login" << login  
             << "password" << password
             << bsoncxx::builder::stream::finalize;
        collection.insert_one(doc_to_add.view());
        return true;
    }
    bool RemoveDataFromDb(const std::string &character_id)
    {
        return true;
    }
 private:
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database db;
};
}