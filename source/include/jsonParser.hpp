#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace j_parser
{
class Parser
{
public:
    Parser() = delete;
    Parser(const std::string &json) noexcept;
    ~Parser() = default;
    std::string getLogin();
    std::string getPassword();
private:
    json _jbody;
};
}