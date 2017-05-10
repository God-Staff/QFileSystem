#ifndef SESSION_HPP
#define SESSION_HPP

#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>
#include <list>
#include <mutex>
#include <cstdio>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem.hpp>

#include "handler_allocator.hpp"
#include "FileManage.hpp"
#include "SerializationToStream.hpp"

class Session : public boost::enable_shared_from_this<Session>
{
public:

	static void print_asio_error (const boost::system::error_code& error)
	{
		std::cerr << error.message () << "\n";
	}

	static boost::shared_ptr<Session> create (boost::asio::io_service& io)
	{
		return boost::shared_ptr<Session> (new Session (io));
	}

	boost::asio::ip::tcp::socket& socket () 
    { 
        return socket_; 
    }

	~Session ()
	{
        if (fp_)
        {
            fclose(fp_);
        }

        WriteBlock_ToFile( );

		std::cout << "\n Thread ID: " << boost::this_thread::get_id () << std::endl;

        clock_ = clock( ) - clock_;
        DataBlockTypeInfo::Size_type bytes_writen = total_bytes_writen_;
        if (clock_ == 0)
        {
            clock_ = 1;
        }

        double speed = bytes_writen * (CLOCKS_PER_SEC / 1024.0 / 1024.0) / clock_;
		optlog.log ("cost time: " +std::to_string(clock_ / (double)CLOCKS_PER_SEC)
                    + " s  "+ "bytes_writen: " + std::to_string (bytes_writen) 
                    + " bytes\n"+ "speed: " + std::to_string (speed) + " MB/s\n\n");
		//在文件接收完成时，打开文件，解析文件参数
	}

	void start ()
	{
		clock_ = clock ();
		std::cout << "client: " << socket_.remote_endpoint ().address () << "\n";

        socket_.async_receive(
                    boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
                                        , sizeof(file_info_))
                    , boost::bind(&Session::handle_header
                                , shared_from_this( )
                                , boost::asio::placeholders::error));

	}

private:
	Session (boost::asio::io_service& io) 
        : socket_ (io)
        , fp_ (NULL)
        , total_bytes_writen_ (0)
	{ 
    
    }

	void handle_header (const boost::system::error_code& error)
	{
        if (error)
        {
            return print_asio_error(error);
        }

		size_t filename_size = file_info_.m_FileNameLength;
		if ((filename_size) > k_buffer_size) 
        {
			std::cerr << "Path name is too long!\n";

			return;
		}

		//用async_read, 不能用async_read_some，防止路径名超长时，一次接收不完
		boost::asio::async_read (socket_
                                 , boost::asio::buffer (buffer_, file_info_.m_FileNameLength)
                                 , boost::bind (&Session::CallBackDiffType
                                                 , shared_from_this ()
                                                 , boost::asio::placeholders::error));
	}

    //根据请求不同，调用不同的处理函数
    void CallBackDiffType(const boost::system::error_code &errorcode)
    {
        //解析请求类型，调用不同的处理函数
        switch ( file_info_.m_ReqiureType )
        {
        case 'a':       //请求建立连接
            socket_.async_receive(
                boost::asio::buffer(buffer_, k_buffer_size)
                , boost::bind(&Session::CheckKey
                , shared_from_this( )
                , boost::asio::placeholders::error));
            break;
        case 'b':       //发送验证结果
            socket_.async_receive(
                boost::asio::buffer(buffer_, k_buffer_size)
                , boost::bind(&Session::handle_file
                , this
                , boost::asio::placeholders::error));
            break;
        case 'c':       //
            socket_.async_receive(
                boost::asio::buffer(buffer_, k_buffer_size)
                , boost::bind(&Session::handle_file
                , shared_from_this( )
                , boost::asio::placeholders::error));
            break;
        case 'd':       //
            socket_.async_receive(
                boost::asio::buffer(buffer_, k_buffer_size)
                , boost::bind(&Session::handle_file
                , shared_from_this( )
                , boost::asio::placeholders::error));
            break;
        case 'e':       //
            socket_.async_receive(
                boost::asio::buffer(buffer_, k_buffer_size)
                , boost::bind(&Session::handle_file
                , shared_from_this( )
                , boost::asio::placeholders::error));
            break;
        case 'f':       //
            socket_.async_receive(
                boost::asio::buffer(buffer_, k_buffer_size)
                , boost::bind(&Session::handle_file
                , shared_from_this( )
                , boost::asio::placeholders::error));
            break;
        default:
            break;
        }

    }

    //检查客户端携带的PirateKey的MD5是否正确，失败则关闭链接
    void CheckKey(const boost::system::error_code& error)
    {
        //获取本机Prikey的md5与此比较;
        qiuwanli::BlockListForDownTable DownListTable;

        if (!DownListTable.ParseFromString(buffer_))
        {	//打开失败
            optlog.log("File Parse Fail!");
        }
        else
        {	//解析配置文件
            std::string md5= g_ComData.Conf.prikeymd5( );
            if (DownListTable.prikeymd5().compare(md5) == 0)
            {
                PairVec ListForUp;
                for (size_t index = 0; index < DownListTable.blocklistfordown_size( ); ++index)
                {
                    const qiuwanli::BlockListForDown& DownList = DownListTable.blocklistfordown(index);
                    std::pair<std::string, std::vector<unsigned long>> pairs;
                    pairs.first = DownList.filesha512( );
                    std::vector<unsigned long> vec;
                    for (size_t index = 0; index < DownList.blocks_size( ); ++index)
                    {
                        const qiuwanli::Block& Block = DownList.blocks(index);
                        vec.emplace_back(Block.blockitem( ));
                    }

                    ListForUp.emplace_back(pairs);
                }
                
                //启用线程进行文件传输
                auto FailList = SendFileToClient(ListForUp);
                if (FailList.size()==0)
                {
                    optlog.log("Sent File Success!");
                }
                else
                {
                    //尝试重传
                    SendFileToClient(ListForUp);
                }

            }
        }
    }
    
    //发送文件块，并将发送失败的列表，返回
    PairVec& SendFileToClient(PairVec& DownList)
    {
        //遍历文件列表，传输完成的从列表中删除
        for (auto& FileBlocks : DownList)
        {
            std::string File512 = FileBlocks.first;
            for (auto Block : FileBlocks.second)
            {
                //查找文件块，若存在则读取文件块和对应块的MD5，并发送给客户端

            }
        }

        return DownList;
    }


	//将接受到的数据块，解析为文件名+文件数据
	void handle_file (const boost::system::error_code& error)
	{
        if (error)
        {
            return print_asio_error(error);
        }

		const char *base_name_msg = buffer_ + file_info_.m_FileNameLength - 1;
        while (base_name_msg >= buffer_&& 
             (*base_name_msg != '\\'
               && *base_name_msg != '/'))
        {
            --base_name_msg;
        }
		++base_name_msg;

		const char *basename = base_name_msg;
		//const char *msg_type = "";

		//将const char* 分割
        std::string str(base_name_msg);
        //std::vector<std::string>  vstr;
        boost::split(vstr, str, boost::is_any_of("+"), boost::token_compress_on);
		basename = vstr[0].c_str ();

        optlog.log("base_name_msg:" + str + "basename:" + vstr[0]
                   + "msg_type:" + vstr[1] + "File Size:" + buffer_);

        fp_ = fopen(basename, "wb");
		if (fp_ == NULL) 
        {
			std::cerr << "Failed to open file to write\n";
			return;
		}

		receive_file_content ();
	}

	void receive_file_content ()
	{
		socket_.async_receive (boost::asio::buffer (buffer_, k_buffer_size)
               , boost::bind (&Session::handle_write
                                , shared_from_this ()
                                , boost::asio::placeholders::error
                                , boost::asio::placeholders::bytes_transferred)
        );
	}

	void handle_write (const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error) 
        {
            if (error != boost::asio::error::eof)
            {
                return print_asio_error(error);
            }

            DataBlockTypeInfo::Size_type filesize = file_info_.m_FileSize;
            if (total_bytes_writen_ != filesize)
            {
                optlog.log("filesize not matched! " 
                           + std::to_string(total_bytes_writen_)
                           + "/" + std::to_string(filesize) + "\n");
            }
			
            return;
		}
		total_bytes_writen_ += fwrite (buffer_, 1, bytes_transferred, fp_);

		receive_file_content ();
	}


    void WriteBlock_ToFile( )
    {
        //在文件传输完成时，若果传输类型为文件块则，调用文件块写入函数
        if (file_info_.m_ReqiureType == 'a')
        {
            fp_ = fopen(vstr[0].c_str( ), "rb");
            if (fp_ == NULL)
            {
                std::cerr << "Failed to open file to write\n";
                return;
            }
            else
            {
                std::string filesha512;
                size_t offset = m_FileManage.WriteFileBlockEnd(buffer_,k_buffer_size);
                extern ComData g_ComData;
                qiuwanli::BlockInfo* addItem = g_ComData.BlockTableDiff.add_block( );
                
                //addItem->filesha512 = vstr[0];
                //addItem->blockmd5 = vstr[1];
                //addItem->savefilename = m_FileManage.getFileName( );
                //addItem->blockmd5 = vstr[2];
                //addItem->cursize = total_bytes_writen_;
                //addItem->fileblockoffset = offset;

                m_serial.MakeBlockInfo(addItem, vstr[0], vstr[1]
                                       , m_FileManage.getFileName( )
                                       , std::atoi(vstr[2].c_str( ))
                                       , total_bytes_writen_, offset);
                
            }

        }
    }

   
	//发送文件
    void send_file_content_sender(  const char*	ip_address
                                  , unsigned	port
                                  , const char* filename
                                  , const char msg_type)
    {
        fopen_s(&fp_, filename, "rb");
        if (fp_ == NULL)
        {
            std::cerr << "cannot open file\n";
            return;
        }

        //使用智能指针，防止程序出现异常时，fclose未被调用。
        boost::shared_ptr<FILE> file_ptr(fp_, fclose);

        clock_t cost_time = clock( );

        size_t filename_size = strlen(filename) + 1;
        size_t file_info_size = sizeof(file_info_);
        size_t total_size = file_info_size + filename_size;

        if (total_size > k_buffer_size)
        {
            std::cerr << "File name is too long";
            return;
        }

        file_info_.m_FileNameLength = filename_size;
        file_info_.m_ReqiureType = msg_type;             //消息类型
        fseek(fp_, 0, SEEK_END);
        file_info_.m_FileSize = ftell(fp_);
        rewind(fp_);


        memcpy(buffer_, &file_info_, file_info_size);						//文件信息
        memcpy(buffer_ + file_info_size, filename, filename_size);			//文件名/消息类型

        //boost::asio::ip::tcp::socket socket(io);
        /* socket_.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address_v4::from_string(ip_address)
            , port));*/

        std::cout << "Sending file : " << filename << " MsgType:" << msg_type << std::endl;
        size_t len = total_size;

        unsigned long long total_bytes_read = 0;
        while (true)
        {
            //先发送文件头，之后发送data
            socket_.send(boost::asio::buffer(buffer_, len), 0);

            if (feof(fp_))
            {
                break;
            }

            len = fread(buffer_, 1, k_buffer_size, fp_);
            total_bytes_read += len;
        }

        //计算时间、大小和速度//
        cost_time = clock( ) - cost_time;
        if (cost_time == 0)
        {
            cost_time = 1;
        }

        double speed = total_bytes_read * (CLOCKS_PER_SEC / 1024.0 / 1024.0) / cost_time;
        std::cout << "cost time: " << cost_time / (double) CLOCKS_PER_SEC
            << " s " << "  transferred_bytes: " << total_bytes_read
            << " bytes\n" << "speed: " << speed << " MB/s\n\n";
    }

private:
	clock_t clock_;
	boost::asio::ip::tcp::socket socket_;
	FILE *fp_;
    DataBlockTypeInfo file_info_;
    DataBlockTypeInfo::Size_type total_bytes_writen_;
	char buffer_[k_buffer_size];
	OptLog optlog;
    std::vector<std::string>  vstr;

    FileManage m_FileManage;
    qiuwanli::SerialToStream m_serial;
};


#endif // !SESSION_HPP
