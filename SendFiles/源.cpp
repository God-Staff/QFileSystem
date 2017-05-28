#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

struct File_info 
{
    typedef unsigned long long Size_type;
    unsigned char   m_RequireType;
    Size_type       m_FileSize;
    size_t          m_FileNameLength;
    File_info( ) : m_FileSize(0), m_FileNameLength(0), m_RequireType(0){ }
};

void sender (boost::asio::io_service &io
             , const char*	ip_address
             , unsigned	port
             , const char* filename
             , const char* msg_type)
{
	FILE *fp=nullptr;

	fopen_s (&fp, filename, "rb");
	if (fp == NULL) 
    {
		std::cerr << "cannot open file\n";
		return;
	}

	//使用智能指针，防止程序出现异常时，fclose未被调用。
	boost::shared_ptr<FILE> file_ptr (fp, fclose);

    fseek(fp, 0, SEEK_END);
    File_info::Size_type fileLenth = ftell(fp);
    rewind(fp);

	clock_t cost_time = clock ();

	const size_t k_buffer_size = 32 * 1024;
	char buffer[k_buffer_size]{};
	File_info file_info;

	//char buf[32 * 1024]{};

    std::string sbuf = filename;
    sbuf += '+';
    sbuf += msg_type;
    std::cout<<sbuf.size( );

	/*strcat_s (buf, filename);
	strcat_s (buf, "+");
	strcat_s (buf, msg_type);*/
	filename = sbuf.c_str();

    size_t filename_size = sbuf.size( );//strlen (filename) + 1;
	size_t file_info_size = sizeof (file_info);
	size_t total_size = file_info_size + filename_size;

	if (total_size > k_buffer_size) 
    {
		std::cerr << "File name is too long";
		return;
	}
	file_info.m_FileNameLength = filename_size;
    file_info.m_FileSize = fileLenth;
    file_info.m_RequireType = 'a';
    memcpy(buffer, &file_info, file_info_size);							//文件信息
    memcpy(buffer + file_info_size, filename, filename_size);			//文件名/消息类型

	boost::asio::ip::tcp::socket socket (io);
	socket.connect (boost::asio::ip::tcp::endpoint (boost::asio::ip::address_v4::from_string (ip_address), port));

	std::cout << "Sending file : " << filename << " MsgType:" << msg_type << std::endl;
	size_t len = total_size;

	unsigned long long total_bytes_read = 0;
	while (true) 
    {
		//先发送文件头，之后发送data
		socket.send (boost::asio::buffer (buffer, len), 0);

        if (feof(fp))
        {
            break;
        }

        len = fread (buffer, 1, k_buffer_size, fp);
		total_bytes_read += len;
	}

	//计算时间、大小和速度//
	cost_time = clock () - cost_time;
	if (cost_time == 0) cost_time = 1;
    double speed = total_bytes_read * (CLOCKS_PER_SEC / 1024.0 / 1024.0) / cost_time;
    std::cout << "cost time: " << cost_time / (double) CLOCKS_PER_SEC
        << " s " << "  transferred_bytes: " << total_bytes_read
        << " bytes\n" << "speed: " << speed << " MB/s\n\n";
}

int main ()
{
	boost::asio::io_service io_ser;
	try {
        sender(io_ser, "127.0.0.1", 9999,  "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (2)", "1001");
        sender(io_ser, "127.0.0.1", 9999,  "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (3)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (4)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (5)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (6)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (7)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (8)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (9)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (10)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (11)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (12)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (13)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (14)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (15)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (16)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (17)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (18)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (19)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (20)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (21)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (22)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (23)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (24)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (25)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (26)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (27)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (28)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (29)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (30)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (31)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (32)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (33)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (34)", "1001");
		sender (io_ser, "127.0.0.1", 9999, "F:\\WorkSpace\\QFileSystem\\Test\\ID2IP - 副本 (35)", "1001");
	}
	catch (std::exception& err) 
    {
		std::cerr << err.what () << "\n";
	}

	return 0;
}