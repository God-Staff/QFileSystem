#ifndef HANDLER_ALLOCATOR_HPP
#define HANDLER_ALLOCATOR_HPP

#include <boost/asio.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/noncopyable.hpp>

class handler_allocator : private boost::noncopyable
{
public:
	handler_allocator () : in_use_ (false)
	{
	}

	void* allocate (std::size_t size)
	{
		if (!in_use_ && size < storage_.size)
		{
			in_use_ = true;
			return storage_.address ();
		}

		return ::operator new(size);
	}

	void deallocate (void* pointer)
	{
		if (pointer == storage_.address ())
		{
			in_use_ = false;
		}
		else
		{
			::operator delete(pointer);
		}
	}

private:
	boost::aligned_storage<1024> storage_;
	bool in_use_;
};

// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. Calls to operator() are forwarded to the
// encapsulated handler.
template <typename Handler>
class custom_alloc_handler
{
public:
	custom_alloc_handler (handler_allocator& a, Handler h)
		: allocator_ (a),
		handler_ (h)
	{
	}

	template <typename Arg1>
	void operator()(Arg1 arg1)
	{
		handler_ (arg1);
	}

	template <typename Arg1, typename Arg2>
	void operator()(Arg1 arg1, Arg2 arg2)
	{
		handler_ (arg1, arg2);
	}

	friend void* asio_handler_allocate (std::size_t size,
		custom_alloc_handler<Handler>* this_handler)
	{
		return this_handler->allocator_.allocate (size);
	}

	friend void asio_handler_deallocate (void* pointer, std::size_t /*size*/,
		custom_alloc_handler<Handler>* this_handler)
	{
		this_handler->allocator_.deallocate (pointer);
	}

private:
	handler_allocator& allocator_;
	Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline custom_alloc_handler<Handler> 
make_custom_alloc_handler(handler_allocator& a, Handler h)
{
	return custom_alloc_handler<Handler> (a, h);
}

#endif // HANDLER_ALLOCATOR_HPP
