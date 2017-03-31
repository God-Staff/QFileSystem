#include "constdate.hpp"

int main1 (int argc, char* argv[])
{
	try
	{
		if (argc != 5)
		{
			std::cerr << "Usage: server <address> <port> <threads> <blocksize>\n";
			return 1;
		}

		boost::asio::ip::address address = boost::asio::ip::address::from_string (argv[1]);
		short port = std::atoi (argv[2]);
		int thread_count = std::atoi (argv[3]);
		size_t block_size = std::atoi (argv[4]);

		boost::asio::io_service ios;

		server s (ios, boost::asio::ip::tcp::endpoint (address, port), block_size);

		// Threads not currently supported in this test.
		std::list<boost::thread*> threads;
		while (--thread_count > 0)
		{
			boost::thread* new_thread = new boost::thread (
				boost::bind (&boost::asio::io_service::run, &ios));
			threads.push_back (new_thread);
		}

		ios.run ();

		while (!threads.empty ())
		{
			threads.front ()->join ();
			delete threads.front ();
			threads.pop_front ();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what () << "\n";
	}

	return 0;
}