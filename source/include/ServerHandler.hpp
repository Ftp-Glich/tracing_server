#include "restinio/core.hpp"
#include "DatabaseHandler.hpp"
#include "jsonValidator.hpp"
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>



using router_t = restinio::router::express_router_t<>;
using namespace std::chrono;
using traits_t =
			restinio::traits_t<
				restinio::asio_timer_manager_t,
				restinio::single_threaded_ostream_logger_t,
				router_t >;


namespace server{
class Server
{
public:
	Server(int, const std::string&, database::DatabaseClient*, j_parser::Parser*);
	~Server() = default;
private:
	std::unique_ptr<router_t> create_request_handler();
	void Start();
	int _port;
	std::string _adress;
	database::DatabaseClient* _dbClient;
	j_parser::Parser* _jParser;
};
}



