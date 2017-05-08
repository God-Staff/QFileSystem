#define _CRT_SECURE_NO_WARNINGS

#include "Servers.hpp"
#include "FileDataStruct.pb.h"
#include "SendFile.hpp"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <windows.h>

qiuwanli::ConfigFile Conf;
qiuwanli::BlockInfoTable BlockTable;
OptLog opplog;

//对数据进行初始化
void initData( )
{
    //加载配置文件
    boost::filesystem::fstream OpFile("config", std::ios::in | std::ios::binary);
    if (!OpFile)
    {
        opplog.log("Conf 打开失败！");
        return;
    }

    if (!Conf.ParseFromIstream(&OpFile))
    {	//打开失败
        OpFile.close( );
        opplog.log("qiuwanli::ConfigFile 解析失败！");
        return;
    }

    OpFile.close( );

    //加载文件块信息表
    boost::filesystem::fstream OpFileBlockInfo("BlockTable.db", std::ios::in | std::ios::binary);
    if (!OpFileBlockInfo)
    {
        opplog.log("BlockTable.db 打开失败！");
        return;
    }

    if (!BlockTable.ParseFromIstream(&OpFileBlockInfo))
    {	//打开失败
        OpFileBlockInfo.close( );
        opplog.log("qiuwanli::ConfigFile 解析失败！");
        return;
    }

    OpFileBlockInfo.close( );
}


//数据的更新，以及定时器
void doItNextTime( )
{
    auto CurPath = boost::filesystem::current_path( );
    boost::filesystem::space_info Space = boost::filesystem::space(CurPath);
    
    //先更新本地服务端数据
    Conf.set_totalsize((Space.capacity + Space.available) / Size_Mb);
    Conf.set_remainsize(Space.capacity / Size_Mb);
    
    //再将数据同步到目录服务器，心跳连接
    try
    {
        boost::asio::io_service io;
        SendFile senddata;
        senddata.sender(io, Conf.serversip, Conf.serversport, "config", 'a');
    }
    catch (std::exception& e)
    {
        opplog.log("Heart Fail!\t" + e.what);
    }

    //延时60秒在发送心跳连接
    Sleep(DelayTime_Heart);

    doItNextTime( );
}


//定时将blockinfo写入到文件
class WriteToFile
{
public:
    WriteToFile( )
    {
        Init( );
    }

    ~WriteToFile( )
    {
        if (OpFile.is_open( ))
        {
            OpFile.close( );
        }

        if (OpFileBlockInfo.is_open( ))
        {
            OpFileBlockInfo.close( );
        }
    }

    void Init()
    {
        //打开文件
        OpFile.open("config", std::ios::out | std::ios::binary);
        if (!OpFile)
        {
            opplog.log("Conf 打开失败！");
            return;
        }

        //加载文件块信息表
        boost::filesystem::ofstream OpFileBlockInfo("BlockTable.db", std::ios::out | std::ios::binary);
        if (!OpFileBlockInfo)
        {
            opplog.log("BlockTable.db 打开失败！");
            return;
        }

        Sleep(DelayWriteToFile);
        DelayWirte( );
    }

    void DelayWirte( )
    {
        if (!Conf.SerializePartialToOstream(&OpFile))
        {	//打开失败
            OpFile.close( );
            opplog.log("qiuwanli::ConfigFile 序列化失败！");
            return;
        }

        if (!BlockTable.SerializePartialToOstream(&OpFileBlockInfo))
        {	//打开失败
            OpFileBlockInfo.close( );
            opplog.log("qiuwanli::ConfigFile 序列化失败！");
            return;
        }

        Sleep(DelayWriteToFile);
        DelayWirte( );
    }

private:
    boost::filesystem::ofstream OpFile;
    boost::filesystem::ofstream OpFileBlockInfo;
};

int main (int argc, char* argv[])
{
	try
	{
        //先初始化数据
        initData( );

        //再执行循环心跳程序
        boost::thread HeartThread(doItNextTime);
        HeartThread.detach( );

        //创建进程去，管理序列化文件的更新
        WriteToFile FileManage;
        boost::thread ManageFileUpdate(FileManage);
        ManageFileUpdate.detach( );


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

		//server ssss (ios, port);
		server ssss (ios, boost::asio::ip::tcp::endpoint (address, port));

		// Threads not currently supported in this test.
		std::list<boost::thread*> threads;

		while (--thread_count > 0)
		{
			boost::thread* new_thread = new boost::thread (
				boost::bind (&boost::asio::io_service::run,&ios));
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