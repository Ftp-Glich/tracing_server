#include "ServerHandler.hpp"
 
using namespace server;



template < typename RESP >
static RESP init_resp( RESP resp )
{
	resp.append_header( restinio::http_field::server, "RESTinio sample server /v.0.2" );
	resp.append_header_date_field();

	return resp;
}

std::unique_ptr<router_t> Server::create_request_handler()
{
	auto router = std::make_unique< router_t >();
		router->http_post(
		"/login/sign_in",
		[this]( auto req, auto ){
			_jParser->Parse(req->body());
			if(_dbClient->CheckData(_jParser->getLogin(), _jParser->getPassword())){
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(R"-({"message" : "You've entered"})-")	
						.done();
				return restinio::request_accepted();
			}
			else
			{
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body("you're not registered")	
						.done();
			return restinio::request_rejected();
			}
		});
		router->http_post(
		"/login/registration",
		[this]( auto req, auto ){
			_jParser->Parse(req->body());
			_dbClient->AddDataToDb(_jParser->getLogin(), _jParser->getPassword());
			{
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body("you've been registered")	
						.done();
			return restinio::request_rejected();
			}
		});
		return router;
}


Server::Server(int port, const std::string& adress, database::DatabaseClient* dbClient, j_parser::Parser* jParser):
_port(port), _adress(adress), _dbClient(dbClient), _jParser(jParser)
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


