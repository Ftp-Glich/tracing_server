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
#include <jwt-cpp/jwt.h>


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
    j_parser::Parser* CheckData(const std::string& login, const std::string& password)
    {
        mongocxx::collection collection = db["LoginData"];
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "login" << login; 
        auto coursor = collection.find(filter_builder.view());
        j_parser::Parser* res = new j_parser::Parser;
        for(auto doc: coursor)
        {			
            res->Parse(bsoncxx::to_json(doc, bsoncxx::ExtendedJsonMode::k_relaxed));
            if(bcrypt::validatePassword(password, res->getPassword())) return res;
        }
        return NULL;
    }
    std::string check_auth(jwt::decoded_jwt<jwt::traits::kazuho_picojson> token)
    {
        mongocxx::collection collection = db["LoginData"];
        bsoncxx::builder::stream::document filter_builder;
        std::string id;
        for(auto& e : token.get_payload_json())
        {
            if(e.first != "id") continue;
            id = e.second.to_str();
        }
        filter_builder << "id" << id; 
        auto coursor = collection.find(filter_builder.view());
        for(auto doc: coursor)
        {			
            _jParser.Parse(bsoncxx::to_json(doc, bsoncxx::ExtendedJsonMode::k_relaxed));
            return _jParser.getId();
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