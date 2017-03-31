#ifndef SESSION_HPP
#define SESSION_HPP

#include <boost/asio.hpp>
#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>
#include <list>
#include <boost/thread.hpp>
#include "handler_allocator.hpp"

class session
{
public:
	session (boost::asio::io_service& ios, size_t block_size)
		: io_service_ (ios),
		strand_ (ios),
		socket_ (ios),
		block_size_ (block_size),
		read_data_ (new char[block_size]),
		read_data_length_ (0),
		write_data_ (new char[block_size]),
		unsent_count_ (0),
		op_count_ (0)
	{
	}

	~session ()
	{
		delete[] read_data_;
		delete[] write_data_;
	}

	boost::asio::ip::tcp::socket& socket ()
	{
		return socket_;
	}

	void start ()
	{
		boost::system::error_code set_option_err;
		boost::asio::ip::tcp::no_delay no_delay (true);
		socket_.set_option (no_delay, set_option_err);
		if (!set_option_err)
		{
			++op_count_;
			socket_.async_read_some (boost::asio::buffer (read_data_, block_size_),
				strand_.wrap (
					make_custom_alloc_handler (read_allocator_,
						boost::bind (&session::handle_read, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred))));
		}
		else
		{
			io_service_.post (boost::bind (&session::destroy, this));
		}
	}

	void handle_read (const boost::system::error_code& err, size_t length)
	{
		--op_count_;

		if (!err)
		{
			read_data_length_ = length;
			++unsent_count_;
			if (unsent_count_ == 1)
			{
				op_count_ += 2;
				std::swap (read_data_, write_data_);
				async_write (socket_, boost::asio::buffer (write_data_, read_data_length_),
					strand_.wrap (
						make_custom_alloc_handler (write_allocator_,
							boost::bind (&session::handle_write, this,
								boost::asio::placeholders::error))));
				socket_.async_read_some (boost::asio::buffer (read_data_, block_size_),
					strand_.wrap (
						make_custom_alloc_handler (read_allocator_,
							boost::bind (&session::handle_read, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred))));
			}
		}

		if (op_count_ == 0)
			io_service_.post (boost::bind (&session::destroy, this));
	}

	void handle_write (const boost::system::error_code& err)
	{
		--op_count_;

		if (!err)
		{
			--unsent_count_;
			if (unsent_count_ == 1)
			{
				op_count_ += 2;
				std::swap (read_data_, write_data_);
				async_write (socket_, boost::asio::buffer (write_data_, read_data_length_),
					strand_.wrap (
						make_custom_alloc_handler (write_allocator_,
							boost::bind (&session::handle_write, this,
								boost::asio::placeholders::error))));
				socket_.async_read_some (boost::asio::buffer (read_data_, block_size_),
					strand_.wrap (
						make_custom_alloc_handler (read_allocator_,
							boost::bind (&session::handle_read, this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred))));
			}
		}

		if (op_count_ == 0)
			io_service_.post (boost::bind (&session::destroy, this));
	}

	static void destroy (session* s)
	{
		delete s;
	}

private:
	boost::asio::io_service& io_service_;
	boost::asio::io_service::strand strand_;
	boost::asio::ip::tcp::socket socket_;
	size_t block_size_;
	char* read_data_;
	size_t read_data_length_;
	char* write_data_;
	int unsent_count_;
	int op_count_;
	handler_allocator read_allocator_;
	handler_allocator write_allocator_;
};

#endif // !SESSION_HPP
