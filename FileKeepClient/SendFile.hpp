#ifndef SEND_FILE_HPP
#define SEND_FILE_HPP

#include "public.h"
#include "OptLog.hpp"
#include "FileDataStruct.pb.h"

#include <boost/asio.hpp>

class SendFile
{
public:
    SendFile( );
    ~SendFile( );
    
    void Init(const char* filename);

    void sender(boost::asio::io_service &io
                , const char*	ip_address
                , unsigned	int port
                , const char* filename
                , const char msg_type); 

private:
    FILE *fp = nullptr;
    OptLog Logs;
    //char buf[k_buffer_size];
    char buffer[k_buffer_size];
    DataBlockTypeInfo file_info;
};

#endif // !SEND_FILE_HPP
