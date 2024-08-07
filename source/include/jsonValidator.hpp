#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/utils/nlohmann_json_utils.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>
using valijson::Schema;
using valijson::SchemaParser;
using valijson::Validator;
using valijson::adapters::NlohmannJsonAdapter;

namespace jvalidator
{
constexpr char registerShema[] = "/home/egor/project/source/include/schemas/register_request_shema.json";
constexpr char loginShema[] = "/home/egor/project/source/include/schemas/login_request_schema.json";

class JValidator
{
public:
    bool Validate (const nlohmann::json &doc) const
    {
        Validator validator;
        NlohmannJsonAdapter targetAdapter(doc);
        if (! validator.validate(_schema, targetAdapter, NULL)) {
            return false;
        }
        return true;
    }
protected:
    Schema _schema;
};


class LoginValidator:
public JValidator
{
public:
    LoginValidator()
    {
        nlohmann::json shema;
         if (!valijson::utils::loadDocument(loginShema, shema)) {
            throw std::runtime_error("Failed to load schema document");
         }
        SchemaParser parser;
        NlohmannJsonAdapter schemaAdapter(shema);
        parser.populateSchema(schemaAdapter, _schema);
    }
    
};

class RegisterValidator:
public JValidator
{
public:
    RegisterValidator()
    {
        nlohmann::json shema;
         if (!valijson::utils::loadDocument(registerShema, shema)) {
            throw std::runtime_error("Failed to load schema document");
         }
        SchemaParser parser;
        NlohmannJsonAdapter schemaAdapter(shema);
        parser.populateSchema(schemaAdapter, _schema);
    }
    
};




}