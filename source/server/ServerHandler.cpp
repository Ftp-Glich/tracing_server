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

std::string generateUUID() {
    boost::uuids::random_generator generator;
    boost::uuids::uuid id = generator();
    return boost::uuids::to_string(id);
}


std::unique_ptr<router_t> Server::create_request_handler()
{
	auto router = std::make_unique< router_t >();
		router->http_post(
		"/login/sign_in",
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
			else if(_dbClient->CheckData(_jParser->getLogin(), _jParser->getPassword())){
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(R"-({"message" : "You've entered"})-")	
						.done();
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
		"/login/registration",
		[this]( auto req, auto ){
			_jParser->Parse(req->body());
			_dbClient->AddDataToDb(_jParser->getLogin(), _jParser->getPassword());
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
				using namespace std::chrono;
    			auto current_time = duration_cast<seconds>(system_clock::now().time_since_epoch());
    			auto expiration_time = current_time + seconds{864000}; // ten days
    			auto token = jwt::create()
    				.set_type("JWS")
    				.set_issuer("auth0")
    				.set_payload_claim("id", jwt::claim(generateUUID()))
    				.sign(jwt::algorithm::hs256{"secret"});
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(token)	
						.done();
				return restinio::request_rejected();
			}
		});
		router->http_post(
		"/login/test",
		[this]( auto req, auto ){
			_jParser->Parse(req->body());			
				init_resp( req->create_response() )
						.append_header( restinio::http_field::content_type, "application.json; charset=utf-8" )
						.set_body(_jParser->getLogin())	
						.done();
			return restinio::request_rejected();
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
		namespace asio_ns = restinio::asio_ns;

		asio_ns::ssl::context tls_context{ asio_ns::ssl::context::sslv23 };
		tls_context.set_options(
			asio_ns::ssl::context::default_workarounds
			| asio_ns::ssl::context::no_sslv2
			| asio_ns::ssl::context::single_dh_use );

		tls_context.use_certificate_chain_file("certificate.pem" );
		tls_context.use_private_key_file(
			"private_key.pem",
			asio_ns::ssl::context::pem );
		tls_context.use_tmp_dh_file( "dhparams.pem" );

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


