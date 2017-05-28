#ifndef SEND_FILE_HPP
#define SEND_FILE_HPP

#include "public.h"
//#include "FileManage.hpp"

#include <boost/asio.hpp>

class SendFile
{
public:
    SendFile( );
    ~SendFile( );
    
    struct ListSer
    {
        std::string Ip;
        size_t port;
        unsigned long blocks;
    };

    void Init(const char* filename);

    void sender(boost::asio::io_service &io
                , const char*	ip_address
                , unsigned	int port
                , const char* filename
                , const char msg_type); 

    void SendFile::senderLitter(boost::asio::io_service& io
                                , const char* ip_address
                                , unsigned int port
                                , const char* cstream
                                , const char msg_type);

private:
    FILE *fp;
    //OptLog Logs;
    char buffer[k_buffer_size];
    DataBlockTypeInfo file_infos;
    std::string filename;
};

#endif // !SEND_FILE_HPP
