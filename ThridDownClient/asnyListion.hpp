#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "public.h"
#include "OptLog.hpp"

#define IPV4

const static size_t PORT_6 = 18034;
const static size_t PORT_4 = 18033;

struct File_info
{
    unsigned char   m_RequireType;
    size_t          m_FileSize;
    size_t          m_FileNameLength;
    File_info( ) : m_FileSize(0), m_FileNameLength(0) { }
};


class AsyncServer
{
public:
    //构造函数 
    AsyncServer(boost::asio::io_service &io
                    , boost::asio::ip::tcp::endpoint &ep) 
                :ios(io)
                    , acceptor(io, ep)
    {
        start( );
    }

    //启动异步接受客户端连接 
    void start( )
    {
        sock_ptr sockListion(new boost::asio::ip::tcp::socket(ios));
        
        //当有连接进入时回调accept_handler函数 
        acceptor.async_accept(*sockListion
                              , boost::bind(&AsyncServer::accept_handler
                              , this
                              , boost::asio::placeholders::error
                              , sockListion));
    }

private:
    boost::asio::io_service &ios;
    boost::asio::ip::tcp::acceptor acceptor;
    typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_ptr;
    OptLog Logs;

private:
    void accept_handler(const boost::system::error_code &errorcode, sock_ptr socket_)
    {
        if (errorcode)
        {
            return;
        }

        //输出客户端连接信息 
        Logs.log("Remote IP :"+ socket_->remote_endpoint( ).address( ).to_string()
                + "remote port:" + (char)socket_->remote_endpoint( ).port());

        socket_->async_receive(
            boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
            , sizeof(file_info_))
            , boost::bind(&AsyncServer::CallBackDiffType
            , this
            , boost::asio::placeholders::error));

        //再次启动异步接受连接 
        start( );
    }
    
    void CallBackDiffType(const boost::system::error_code &errorcode, sock_ptr socket_)
    {
        //解析请求类型，调用不同的处理函数

        
        switch (file_info_.m_RequireType)
        {
        case 'a':       //请求建立连接
            socket_->async_receive(
                boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                , sizeof(file_info_))
                , boost::bind(&AsyncServer::handle_header
                , this
                , boost::asio::placeholders::error));
            break;
        case 'b':       //发送验证结果
            socket_->async_receive(
                boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                , sizeof(file_info_))
                , boost::bind(&AsyncServer::handle_header
                , this
                , boost::asio::placeholders::error));
            break;
        case 'c':       //
            socket_->async_receive(
                boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                , sizeof(file_info_))
                , boost::bind(&AsyncServer::handle_header
                , this
                , boost::asio::placeholders::error));
            break;
        case 'd':       //
            socket_->async_receive(
                boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                , sizeof(file_info_))
                , boost::bind(&AsyncServer::handle_header
                , this
                , boost::asio::placeholders::error));
            break;
        case 'e':       //
            socket_->async_receive(
                boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                , sizeof(file_info_))
                , boost::bind(&AsyncServer::handle_header
                , this
                , boost::asio::placeholders::error));
            break;
        case 'f':       //
            socket_->async_receive(
                boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                , sizeof(file_info_))
                , boost::bind(&AsyncServer::handle_header
                , this
                , boost::asio::placeholders::error));
            break;
        default:
            break;
        }

    }

    void handle_header(const boost::system::error_code &errorcode, sock_ptr socket_)
    {
        if (errorcode)
        {
            return;
        }
        //异步向客户端发送数据，发送完成时调用write_handler 
        socket_->async_write_some(boost::asio::buffer("I heard you!"),
                               bind(&AsyncServer::write_handler
                                      , this
                                      , boost::asio::placeholders::error));

    }

    void write_handler(const boost::system::error_code&)
    {
        std::cout << "send msg complete!" << std::endl;
    }

private:
    File_info file_info_;

};

int Listion(int argc, char* argv[ ])
{
    try
    {
        //定义io_service对象 
        boost::asio::io_service ios;
        //定义服务端endpoint对象(协议和监听端口) 
#ifdef IPV4 
        boost::asio::ip::tcp::endpoint serverep(boost::asio::ip::tcp::v4( ), PORT_4);
#endif 

        //启动异步服务 
        AsyncServer server(ios, serverep);
        //等待异步完成 
        ios.run( );
    } catch (std::exception& e)
    {
        std::cout << e.what( ) << std::endl;
    }
    return 0;
}


//protobuf xuliehua
//PlayerInfo info;
//info.set_name(name);
//// ...
//
//boost::asio::streambuf b;
//std::ostream os(&b);
//info.SerializeToOstream(&os);
//
//boost::asio::write(*sock, b);