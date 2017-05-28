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

    struct File_info
    {
        typedef unsigned long long Size_type;
        unsigned char   m_RequireType;
        Size_type       m_FileSize;
        size_t          m_FileNameLength;
        File_info( ) : m_FileSize(0), m_FileNameLength(0), m_RequireType(0) { }
    };

    void Init(const char* filename);

    void sender(boost::asio::io_service &io
                , const char*	ip_address
                , unsigned	int port
                , const char* filename
                , unsigned char msg_type);

    void SendFile::senderLitter(boost::asio::io_service& io
                                , const char* ip_address
                                , unsigned int port
                                , const char* filename
                                , unsigned char msg_type);

private:
    FILE *fp;
    //OptLog Logs;
    char buffer[k_buffer_size];
    File_info file_info;
    std::string filename;
};

#endif // !SEND_FILE_HPP
