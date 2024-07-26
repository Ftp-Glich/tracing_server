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
        return _jbody["login"].get<std::string>();
    }
    std::string getPassword()
    {
        return _jbody["password"].get<std::string>();
    }
private:
    json _jbody;
};
}