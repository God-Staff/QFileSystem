#define _CRT_SECURE_NO_WARNINGS

#include "Servers.hpp"
#include "SendFile.hpp"

#include "PublicStruct.pb.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <windows.h>

ComData g_ComData;
#include "Interface.h"

CInterface PublicData;
//对数据进行初始化
void initData( )
{
    //加载配置文件
    boost::filesystem::fstream OpFile("config", std::ios::in | std::ios::binary);
    if (!OpFile.is_open())
    {
        //g_ComData.opplog.log("Conf 打开失败！");
        //return;
    }

    if (!g_ComData.Conf.ParseFromIstream(&OpFile))
    {	//打开失败
        OpFile.close( );
        //g_ComData.opplog.log("qiuwanli::ConfigFile 解析失败！");
       // return;
    }

    OpFile.close( );

    //加载文件块信息表
    boost::filesystem::fstream OpFileBlockInfo("BlockTable.db", std::ios::in | std::ios::binary);
    if (!OpFileBlockInfo)
    {
        g_ComData.opplog.log("BlockTable.db 打开失败！");
        //return;
    }

    if (!g_ComData.BlockTable.ParseFromIstream(&OpFileBlockInfo))
    {	//打开失败
        OpFileBlockInfo.close( );
        g_ComData.opplog.log("qiuwanli::ConfigFile 解析失败！");
        //return;
    }

    OpFileBlockInfo.close( );
}

//数据的更新，以及定时器
void doItNextTime( )
{
    auto CurPath = boost::filesystem::current_path( );
    boost::filesystem::space_info Space = boost::filesystem::space(CurPath);
    
    //先更新本地服务端数据
    g_ComData.Conf.set_totalsize((Space.capacity + Space.available) / Size_Mb);
    g_ComData.Conf.set_remainsize(Space.capacity / Size_Mb);
    
    //qiuwanli::Heart heart;
    //heart.set_id("");
    //heart.set_remainsize(Space.capacity / Size_Mb);
    //heart.set_totlesize((Space.capacity + Space.available) / Size_Mb);
    //heart.set_prikeymd5(g_ComData.Conf.prikeymd5( ));

    std::string name = "100001";//g_ComData.Conf.id( );
        name += '+';
        name += std::to_string(Space.capacity / Size_Mb);
        name += '+';
        name += std::to_string((Space.capacity + Space.available) / Size_Mb);
        name += '+';
        name += "rwergsergttsghts"; //g_ComData.Conf.prikeymd5( );

    //再将数据同步到目录服务器，心跳连接
    try
    {
        boost::asio::io_service io;
        SendFile senddata;
        std::string ip = g_ComData.Conf.serversip( );
        senddata.senderLitter(io
                              , "127.0.0.1"//g_ComData.Conf.serversip( ).c_str( )
                              , 8189//std::atoi(g_ComData.Conf.serversport( ).c_str( ))
                              , name.c_str( )
                              , 'i');

    }
    catch (std::exception e)
    {
        //g_ComData.opplog.log("Heart Fail!\t");
    }

    //延时60秒在发送心跳连接
    Sleep(DelayTime_Heart);

    doItNextTime( );
}

void sendBlockInfoToServers( )
{
    //将最近15秒内接收到的块信息，发送给服务端
    if (g_ComData.BlockTableDiff.block_size( ))
    {
        ////qiuwanli::BlockInfo Info;
        //std::string name = "BlockTableDiff" + g_ComData.Conf.id( );
        ////PublicData.SaveToFile(&g_ComData.BlockTableDiff, name.c_str( ));
        //boost::filesystem::fstream tmp(name, std::ios::out | std::ios::binary);
        //if (!g_ComData.BlockTableDiff.SerializePartialToOstream(&tmp))
        //{
        //    return;
        //}
        //tmp.close( );
        //try
        //{
        //    boost::asio::io_service io;
        //    SendFile senddata;
        //    senddata.sender(io
        //                    , "127.0.0.1"//g_ComData.Conf.serversip( ).c_str( )
        //                    , 8189//std::atoi(g_ComData.Conf.serversport( ).c_str( ))
        //                    , name.c_str( )
        //                    , 'u');
        //} catch (std::exception e)
        //{
        //    g_ComData.opplog.log("Heart Fail!\t");
        //}

        //将其合并
        g_ComData.BlockTablePreDiff.MergeFrom(g_ComData.BlockTableDiff);
    
        //清空数据，便于下次合并
        g_ComData.BlockTableDiff.Clear( );
    }

    Sleep(DelayTime_SendInfo);
    
    sendBlockInfoToServers( );
}

//定时将blockinfo写入到文件
void CloseWriteToFile( )
{
    if (g_ComData.OpFile.is_open( ))
    {
        g_ComData.OpFile.close( );
    }

    if (g_ComData.OpFileBlockInfo.is_open( ))
    {
        g_ComData.OpFileBlockInfo.close( );
    }
}

void Init( )
{
    //打开文件
    g_ComData.OpFile.open("config", std::ios::out | std::ios::binary);
    if (!g_ComData.OpFile)
    {
        g_ComData.opplog.log("Conf 打开失败！");
       // return;
    }

    //加载文件块信息表
    g_ComData.OpFileBlockInfo.open("BlockTable.db", std::ios::out | std::ios::binary);
    if (!g_ComData.OpFileBlockInfo)
    {
        g_ComData.opplog.log("BlockTable.db 打开失败！");
        //return;
    }
}

void DelayWirte( )
{
    if (!g_ComData.Conf.SerializePartialToOstream(&g_ComData.OpFile))
    {	//打开失败
        g_ComData.OpFile.close( );
        g_ComData.opplog.log("qiuwanli::ConfigFile 序列化失败！");
        return;
    }

    g_ComData.BlockTable.MergeFrom(g_ComData.BlockTablePreDiff);
    g_ComData.BlockTablePreDiff.Clear( );

    if (!g_ComData.BlockTable.SerializePartialToOstream(&g_ComData.OpFileBlockInfo))
    {	//打开失败
        g_ComData.OpFileBlockInfo.close( );
        g_ComData.opplog.log("qiuwanli::ConfigFile 序列化失败！");
        return;
    }

    Sleep(DelayWriteToFile);
    DelayWirte( );
}

int main (int argc, char* argv[])
{
    try
    {
        //先初始化数据
        initData( );
        std::cout << "1";
        //初始化服务端连接信息
        //再执行循环心跳程序
        boost::thread HeartThread(doItNextTime);
        HeartThread.detach( );
        std::cout << "1";

        //发送数据
        boost::thread BlockInfoThread(sendBlockInfoToServers);
        BlockInfoThread.detach( );
        std::cout << "1";

        //创建进程去，管理序列化文件的更新
        //WriteToFile FileManage;
        //boost::thread ManageFileUpdate{WriteToFile( )};
        //ManageFileUpdate.join( );
        boost::thread FileManage([&]( ){
            Init( );
            Sleep(DelayWriteToFile);
            DelayWirte( );
        });
        FileManage.detach( );
        std::cout << "1";

        //
		boost::asio::ip::address address = boost::asio::ip::address::from_string ("127.0.0.1");
        short port = 8289;//std::atoi (argv[2]);
        int thread_count = 8;//std::atoi (argv[3]);
        size_t block_size = 4096;//std::atoi (argv[4]);

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


    CloseWriteToFile( );
	return 0;
}