#include <restinio/core.hpp>
#include <restinio/tls.hpp>
#include "DatabaseHandler.hpp"
#include "jsonValidator.hpp"
#include <chrono>




using router_t = restinio::router::express_router_t<>;
using namespace std::chrono;
using traits_t =
 restinio::single_thread_tls_traits_t<
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
	std::unordered_map<std::string, j_parser::Parser> sessions;
};
}



