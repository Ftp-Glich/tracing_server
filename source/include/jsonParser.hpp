#include "nlohmann/json.hpp"

using json = nlohmann::json;


namespace j_parser
{
class Parser
{
public:
    Parser() = delete;
    Parser(const std::string &json)
    {
        _jbody = json::parse(json);
    }
    ~Parser() = default;
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