#include "SendFile.hpp"


SendFile::SendFile( ):fp(nullptr)
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

    //使用智能指针，防止程序出现异常时，fclose未被调用。
    boost::shared_ptr<FILE> file_ptr(fp, fclose);
}

void SendFile::sender(boost::asio::io_service& io
                      , const char* ip_address
                      , unsigned int port
                      , const char* filename
                      , const char msg_type)
{
    Init(filename);

    size_t filename_size = strlen(filename) + 1;
    size_t file_info_size = sizeof(file_info);
    size_t total_size = file_info_size + filename_size;

    if (total_size > k_buffer_size)
    {
        Logs.log("File name is too long");
        return;
    }

    file_info.m_FileNameLength = filename_size;
    file_info.m_ReqiureType = msg_type;
    fseek(fp, 0, SEEK_END);
    file_info.m_FileSize = ftell(fp);
    rewind(fp);

    memcpy(buffer, &file_info, file_info_size);							//文件信息
    memcpy(buffer + file_info_size, filename, filename_size);			//文件名/消息类型

    boost::asio::ip::tcp::socket socket(io);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(ip_address), port));

    std::string log = "Sending file : ";
    log += filename;
    log += " MsgType:";
    log += msg_type;

    Logs.log(log);

    size_t len = total_size;

    while (true)
    {
        //先发送文件头，之后发送data
        socket.send(boost::asio::buffer(buffer, len), 0);

        if (feof(fp))
        {
            break;
        }

        len = fread(buffer, 1, k_buffer_size, fp);
    }
}
