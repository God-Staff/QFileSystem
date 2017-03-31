#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "handler_allocator.hpp"

class server
{
public:
	server (boost::asio::io_service& ios, const boost::asio::ip::tcp::endpoint& endpoint,
		size_t block_size)
		: io_service_ (ios),
		acceptor_ (ios),
		block_size_ (block_size)
	{
		acceptor_.open (endpoint.protocol ());
		acceptor_.set_option (boost::asio::ip::tcp::acceptor::reuse_address (1));
		acceptor_.bind (endpoint);
		acceptor_.listen ();

		start_accept ();
	}

	void start_accept ()
	{
		session* new_session = new session (io_service_, block_size_);
		acceptor_.async_accept (new_session->socket (),
			boost::bind (&server::handle_accept, 
				this, new_session,
				boost::asio::placeholders::error));
	}

	void handle_accept (session* new_session, const boost::system::error_code& err)
	{
		if (!err)
		{
			new_session->start ();
		}
		else
		{
			delete new_session;
		}

		start_accept ();
	}

private:
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	size_t block_size_;
};

#endif // !SERVERS_HPP
