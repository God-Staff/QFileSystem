#include "stdafx.h"
#include "SendFile.hpp"
#include "public.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

SendFile::SendFile( ) : fp(nullptr)
{ 

}

SendFile::~SendFile( )
{

}

void SendFile::Init(const char* filename)
{ 
    fopen_s(&fp, filename, "rb");
    if (fp == NULL)
    {
        std::cerr << "cannot open file\n";
        return;
    }
}

void SendFile::sender(boost::asio::io_service& io
                      , const char* ip_address
                      , unsigned int port
                      , const char* filename
                      , unsigned char msg_type)
{
    //分割字串(包含各种信息)
    std::string str(filename);
    const char*fileee;
    std::vector<std::string> vstr;
    boost::split(vstr, str, boost::is_any_of("+"), boost::token_compress_on);
    fileee = vstr[0].c_str( );
    Init(fileee);

    //使用智能指针，防止程序出现异常时，fclose未被调用。
    boost::shared_ptr<FILE> file_ptr(fp, fclose);

    fseek(fp, 0, SEEK_END);
    File_info::Size_type fileLenth = ftell(fp);
    rewind(fp);

    clock_t cost_time = clock( );

    const size_t k_buffer_size = 32 * 1024;
    char buffer[k_buffer_size]{ };

    std::string sbuf = filename;//防止文件名中有空白字符
    size_t filename_size = sbuf.size( );//strlen (filename) + 1;
    size_t file_info_size = sizeof(file_info);
    size_t total_size = file_info_size + filename_size;

    if (total_size > k_buffer_size)
    {
        return;
    }
    file_info.m_FileNameLength = filename_size;
    file_info.m_FileSize = fileLenth;
    file_info.m_RequireType = msg_type;
    memcpy(buffer, &file_info, file_info_size);							//文件信息
    memcpy(buffer + file_info_size, filename, filename_size);			//文件名/消息类型

    boost::asio::ip::tcp::socket socket(io);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(ip_address), port));

    size_t len = total_size;

    unsigned long long total_bytes_read = 0;
    while (true)
    {
        //先发送文件头，之后发送data
        socket.send(boost::asio::buffer(buffer, len), 0);

        if (feof(fp))
        {
            break;
        }

        len = fread(buffer, 1, k_buffer_size, fp);
        total_bytes_read += len;
    }
}

void SendFile::senderLitter(boost::asio::io_service& io
                      , const char* ip_address
                      , unsigned int port
                      , const char* filename
                      , unsigned char msg_type)
{
    //const size_t k_buffer_size = 32 * 1024;
    char buffer[k_buffer_size]{ };

    std::string sbuf = filename;//防止文件名中有空白字符
    size_t filename_size = sbuf.size( );//strlen (filename) + 1;
    size_t file_info_size = sizeof(file_info);
    size_t total_size = file_info_size + filename_size;

    if (total_size > k_buffer_size)
    {
        return;
    }
    file_info.m_FileNameLength = filename_size;
    file_info.m_RequireType = msg_type;
    memcpy(buffer, &file_info, file_info_size);							//文件信息
    memcpy(buffer + file_info_size, filename, filename_size);			//文件名/消息类型

    boost::asio::ip::tcp::socket socket(io);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(ip_address), port));

    size_t len = total_size;
    //先发送文件头，之后发送data
    socket.send(boost::asio::buffer(buffer, len), 0);

}
