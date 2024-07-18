#include "restinio/core.hpp"

#include "DatabaseHandler.hpp"
#include "jsonParser.hpp"

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
	Server(int, const std::string&);
	~Server() = default;
private:
	void Start();
	template < typename RESP > RESP init_resp( RESP resp );
	auto create_request_handler();
	int _port;
	std::string _adress;
};
}



