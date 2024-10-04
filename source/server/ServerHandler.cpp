#include "ServerHandler.hpp"

using namespace server;
using namespace std::chrono;


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
		"/auth/login",
		[this]( auto req, auto ){
			_jParser->Parse(req->body());
			jvalidator::LoginValidator lval;
			if(!lval.Validate(_jParser->getBody()))
			{
				init_resp( req->create_response(restinio::status_bad_request()) )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body("wrong schema")	
						.done();
				return restinio::request_rejected();
			}
			j_parser::Parser* data = _dbClient->CheckData(_jParser->getLogin(), _jParser->getPassword());
			if(data != NULL ){
				using namespace std::chrono;
    			auto current_time = duration_cast<seconds>(system_clock::now().time_since_epoch());
    			auto expiration_time = current_time + seconds{864000}; // ten days
				std::string message = "User signed in successfully";
    			auto token = jwt::create()
    				.set_type("JWS")
    				.set_issuer("auth0")
    				.set_payload_claim("id", jwt::claim(data->getId()))
    				.sign(jwt::algorithm::hs256{"secret"});
				sessions.insert({data->getId(), *data});
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(R"-({ "message" : "seccessful signing in!",
										"token": )-" + token +
										"}")	
						.done();
				delete(data);
				return restinio::request_accepted();
			}
			else
			{
				init_resp( req->create_response(restinio::status_bad_request()) )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body("you're not registered")	
						.done();
			return restinio::request_rejected();
			}
		});
		router->http_post(
		"/auth/registration",
		[this]( auto req, auto ){
			_jParser->Parse(req->body());
			jvalidator::RegisterValidator rval;
			if(!rval.Validate(_jParser->getBody()))
			{
				init_resp( req->create_response(restinio::status_bad_request()) )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body("wrong schema")	
						.done();
				return restinio::request_rejected();
			}
			else
			{
				std::string id = _dbClient->AddDataToDb(_jParser->getLogin(), _jParser->getPassword(), _jParser->getConf());
				std::string message = "User registered successfully";
				using namespace std::chrono;
    			auto current_time = duration_cast<seconds>(system_clock::now().time_since_epoch());
    			auto expiration_time = current_time + seconds{864000}; // ten days
    			auto token = jwt::create()
    				.set_type("JWS")
    				.set_issuer("auth0")
    				.set_payload_claim("id", jwt::claim(id))
    				.sign(jwt::algorithm::hs256{"secret"});
				_jParser->addItem()
				sessions.insert({id, *_jParser});
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(R"-({ "message" : "seccessful registration!",
										"token": )-" + token +
										"}")	
						.done();
				return restinio::request_accepted();
			}
		});
		router->http_get(
		"/auth/me",
		[this]( auto req, auto ){
			std::string auth_field = req->header().get_field("Authorization");
			std::string token = auth_field.erase(0, std::string("Bearer ").length());
			std::string id = _dbClient->check_auth(jwt::decode(token));
			if(id == "wrong")
			{
				init_resp( req->create_response(restinio::status_bad_request()) )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body("wrong token")	
						.done();
				return restinio::request_rejected();
			}
			init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(sessions[id].getBody().dump())	
						.done();
			return restinio::request_accepted();
		}	
		);
		return router;
}


Server::Server(int port, const std::string& adress, database::DatabaseClient* dbClient, j_parser::Parser* jParser):
_port(port), _adress(adress), _dbClient(dbClient), _jParser(jParser), sessions()
{
	Start();
}

void Server::Start()
{
    try
	{
		namespace asio_ns = restinio::asio_ns;

		asio_ns::ssl::context tls_context{ asio_ns::ssl::context::sslv23 };
		tls_context.set_options(
			asio_ns::ssl::context::default_workarounds
			| asio_ns::ssl::context::no_sslv2);

		tls_context.use_certificate_chain_file("certs/myCA.pem" );
		tls_context.use_private_key_file(
		"certs/myCA.key",
			asio_ns::ssl::context::pem );
		restinio::run(
			restinio::on_this_thread< traits_t >()
				.address( "localhost" )
				.request_handler( create_request_handler() )
				.read_next_http_message_timelimit( 10s )
				.write_http_response_timelimit( 1s )
				.handle_request_timeout( 1s )
				.tls_context( std::move( tls_context ) ) );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
	}
}


