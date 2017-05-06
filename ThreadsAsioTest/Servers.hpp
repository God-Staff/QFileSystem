#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "handler_allocator.hpp"
#include "Session.hpp"

class server
{
public:
	server (boost::asio::io_service& ios
            , const boost::asio::ip::tcp::endpoint& endpoint)
		    : io_service_ (ios),
		    acceptor_ (ios)
	{
		acceptor_.open (endpoint.protocol ());
		acceptor_.set_option (boost::asio::ip::tcp::acceptor::reuse_address (1));
		acceptor_.bind (endpoint);
		acceptor_.listen ();

		start_accept ();
	}
	static void print_asio_error (const boost::system::error_code& error) 
    { 
        std::cerr << error.message () << "\n"; 
    }

private:
	void start_accept ()
	{
		boost::shared_ptr<Session> session = Session::create (io_service_);
        acceptor_.async_accept(session->socket( )
                               , boost::bind(&server::handle_accept
                                            , this
                                            , session
                                            , boost::asio::placeholders::error));
	}

	void handle_accept (boost::shared_ptr<Session> session
                        , const boost::system::error_code& error)
	{
        if (error)
        {
            return print_asio_error(error);
        }
		session->start ();
		start_accept ();
	}

private:
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_service& io_service_;
};
#endif // !SERVERS_HPP
