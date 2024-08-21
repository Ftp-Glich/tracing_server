#pragma once

#include <cstdint>
#include <string>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"
#include "jsonParser.hpp"
#include "bcrypt.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
    
    std::string AddDataToDb(const std::string &login,
                            const std::string &password,
                            const std::string &conf_password)
    {
        if(password != conf_password) return "conf error";
        mongocxx::collection collection = db["LoginData"];
        auto builder = bsoncxx::builder::stream::document{};
        std::string hashed_password = bcrypt::generateHash(password);
        std::string id = generateUUID();
        bsoncxx::document::value doc_to_add =
            builder 
             << "login" << login  
             << "password" << hashed_password
             << "id" << id
             << bsoncxx::builder::stream::finalize;
        collection.insert_one(doc_to_add.view());
        return id;
    }
    bool RemoveDataFromDb(const std::string &character_id)
    {
        return true;
    }
    std::string CheckData(const std::string& login, const std::string& password)
    {
        mongocxx::collection collection = db["LoginData"];
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "login" << login; 
        auto coursor = collection.find(filter_builder.view());
        for(auto doc: coursor)
        {			
            _jParser.Parse(bsoncxx::to_json(doc, bsoncxx::ExtendedJsonMode::k_relaxed));
            if(bcrypt::validatePassword(password, _jParser.getPassword())) return _jParser.getId();
        }
        return "wrong";
    }
 private:
 	j_parser::Parser _jParser = j_parser::Parser();
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database db; 
    std::string generateUUID() {
        boost::uuids::random_generator generator;
        boost::uuids::uuid id = generator();
        return boost::uuids::to_string(id);
    }
};
}