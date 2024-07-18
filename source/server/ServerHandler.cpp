#include "ServerHandler.hpp"
 
using namespace server;



template < typename RESP >
static RESP init_resp( RESP resp )
{
	resp.append_header( restinio::http_field::server, "RESTinio sample server /v.0.2" );
	resp.append_header_date_field();

	return resp;
}

static auto create_request_handler()
{
	auto router = std::make_unique< router_t >();
	router->http_get(
		"/",
		[]( auto req, auto ){
				init_resp( req->create_response() )
					.append_header( restinio::http_field::content_type, "text/plain; charset=utf-8" )
					.set_body( "Hello world!")
					.done();

				return restinio::request_accepted();
		} );

	router->http_get(
		"/json",
		[]( auto req, auto ){
				init_resp( req->create_response() )
					.append_header( restinio::http_field::content_type, "application/json" )
					.set_body( R"-({"message" : "Hello world!"})-")
					.done();

				return restinio::request_accepted();
		} );
	router->http_get(
		"/html",
		[]( auto req, auto ){
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "text/html; charset=utf-8" )
						.set_body(
							"<html>\r\n"
							"  <head><title>Hello from RESTinio!</title></head>\r\n"
							"  <body>\r\n"
							"    <center><h1>Hello world</h1></center>\r\n"
							"  </body>\r\n"
							"</html>\r\n" )
						.done();

				return restinio::request_accepted();
		} );
		return router;
}


Server::Server(int port, const std::string& adress):
_port(port), _adress(adress)
{
	Start();
}

void Server::Start()
{
    try
	{
		restinio::run(
			restinio::on_this_thread<traits_t>()
				.port( _port )
				.address( _adress )
				.request_handler( create_request_handler() ) );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
	}
}


