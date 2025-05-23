#include "nlohmann/json.hpp"

using json = nlohmann::json;


namespace j_parser
{
class Parser
{
public:
    Parser() = default;
    ~Parser() = default;
    void Parse(const std::string& jstring)
    {
        _jbody = json::parse(jstring);
    }
    std::string getLogin()
    {
        return _jbody["email"].get<std::string>();
    }
    std::string getPassword()
    {
        return _jbody["password"].get<std::string>();
    }
    std::string getId()
    {
        return  _jbody["id"].get<std::string>();
    }
    std::string getConf()
    {
        return  _jbody["confirmPassword"].get<std::string>();
    }
    json getBody()
    {
        return _jbody;
    }
    void addItem(std::string key, std::string value)
    {
        std::string item = "{\"" + key + "\": {\"" + value + "\"}";
        json adition = json::parse(item);
        _jbody.update(item);
    }
    void setBody(json body)
    {
        _jbody = body;
    }
    
private:
    json _jbody;
};
}