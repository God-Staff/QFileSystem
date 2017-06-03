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
//#include "FileManage.hpp"
//#include "SerializationToStream.hpp"
#include "public.h"

//ComData g_ComData;
std::mutex g_mutex;
std::mutex g_mutex1;
class Session : public boost::enable_shared_from_this<Session>
{
public:

    static void print_asio_error(const boost::system::error_code& error)
    {
        std::cerr << error.message( ) << "\n";
    }

    static boost::shared_ptr<Session> create(boost::asio::io_service& io)
    {
        return boost::shared_ptr<Session>(new Session(io));
    }

    boost::asio::ip::tcp::socket& socket( )
    {
        return socket_;
    }

    ~Session( )
    {
        if (fp_)
        {
            fclose(fp_);
        }

        std::cout << "\n Thread ID: " << boost::this_thread::get_id( ) << std::endl;

        //在文件接收完成时，打开文件，解析文件参数
    }

    void start( )
    {
        clock_ = clock( );
        std::cout << "client: " << socket_.remote_endpoint( ).address( ) << "\n";

        socket_.async_receive(
            boost::asio::buffer(reinterpret_cast<char*>(&file_info_)
            , sizeof(file_info_))
            , boost::bind(&Session::handle_header
            , shared_from_this( )
            , boost::asio::placeholders::error));

    }

private:
    Session(boost::asio::io_service& io)
        : socket_(io)
        , fp_(NULL)
        , total_bytes_writen_(0)
    {

    }

    void handle_header(const boost::system::error_code& error)
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
        boost::asio::async_read(socket_
                                , boost::asio::buffer(buffer_, file_info_.m_FileNameLength)
                                , boost::bind(&Session::CallBackDiffType
                                , shared_from_this( )
                                , boost::asio::placeholders::error));
    }

    void dof( )
    {
        //更行数据块界面         
        std::mutex some_mutex;
        std::vector<std::string> vtmp;
        vtmp.push_back(vstr[0]);
        vtmp.push_back(vstr[2]);
        vtmp.push_back(vstr[1]);
        vtmp.push_back(vstr[3]);

        {
            std::lock_guard<std::mutex> guard(g_mutex);
            g_ComData.g_AddBlockTmp.push_back(vtmp);
        }
        {
            std::lock_guard<std::mutex> guard(g_mutex1);
            PublicData.DoBlockList4DownTable(g_ComData.m_BlockToFileTableTmp.add_blocklistfordown( )
                                             , vstr[0]
                                             , std::stoll(vstr[2])
                                             , vstr[1]
                                             , vstr[3]);
        }

        //设置状态位
        g_ComData.m_UIChange |= size_t(4);
    }

    bool DoBlockList4DownTable(qiuwanli::BlockList4Down* BlockList
                               , const std::string& FileSHA512
                               , unsigned long long BlockNumer
                               , const std::string& BlockMD5
                               , const std::string& SaveServersIP)
    {
        BlockList->set_filesha512(FileSHA512);
        BlockList->set_blocknumer(BlockNumer);
        BlockList->set_blockmd5(BlockMD5);
        BlockList->set_saveserversip(SaveServersIP);

        return true;
    }

    //根据请求不同，调用不同的处理函数
    void CallBackDiffType(const boost::system::error_code &error)
    {
        if (error)
        {
            return print_asio_error(error);
        }

        const char *base_name_msg = buffer_ + file_info_.m_FileNameLength - 1;
        while (base_name_msg >= buffer_ &&
            (*base_name_msg != '\\'
               && *base_name_msg != '/'))
        {
            --base_name_msg;
        }
        ++base_name_msg;

        const char *basename = base_name_msg;

        //将const char* 分割
        std::string str(base_name_msg);
        boost::split(vstr, str, boost::is_any_of("+"), boost::token_compress_on);
        basename = vstr[0].c_str( );

        //文件名+SHA512+UserID+UsPS
        //请求目录服务端验证文件是否已存正在
        if (file_info_.m_ReqiureType == 'a')
        {
            std::cout << "请求目录服务端验证文件是否已存正在" << std::endl;
            //查找用户和密码，验证
            bool isok = false;
            {
                std::lock_guard<std::mutex> guard(g_mutex);
                for (int index = 0; index < g_ComData.m_UserhasFile.user_size( ); ++index)
                {
                    if (g_ComData.m_UserhasFile.user(index).userid( ).c_str( ) == vstr[2] &&
                        g_ComData.m_UserhasFile.user(index).userps( ).c_str( ) == vstr[3])
                    {
                        isok = true;
                    }
                }
            }
            if (!isok)
            {
                //socket_.shutdown(boost::asio::ip::tcp::socket::shutdown);
                socket_.shutdown_both;
                return;
            }

            //查找文件SHA512
            {
                std::lock_guard<std::mutex> guard(g_mutex);

                for (int i = 0; i < g_ComData.m_FileListTable.filelist_size( ); ++i)
                {
                    if (g_ComData.m_FileListTable.filelist(i).filesha512( ).compare(vstr[1]) == 0)
                    {
                        boost::asio::io_service io_ser;
                        SendFile sender;
                        try
                        {
                            std::string name = vstr[0];
                            name += "+";
                            name += vstr[1];
                            sender.senderLitter(io_ser, "127.0.0.1", 8089, name.c_str( ), 'b');
                        } catch (std::exception& e)
                        {
                            std::cout << e.what( ) << std::endl;
                        }
                        return;
                    }
                }
            }

            //文件不存在，查找当前可存储的服务端，并发送
            qiuwanli::ClientConfigFileTable ClientInfo;

            {
                std::lock_guard<std::mutex> guard(g_mutex);
                for (int i = 0; i < g_ComData.m_ClientConfigFile.clientinfo_size( ); ++i)
                {

                    const qiuwanli::ClientConfigFile& conf = g_ComData.m_ClientConfigFile.clientinfo(i);
                    PublicData.DoClientConfigFileTable(ClientInfo.add_clientinfo( )
                                                       , conf.cilentid( )
                                                       , conf.saveip( )
                                                       , ""                 //conf.prikey( )
                                                       , conf.keymd5( )
                                                       , 0                  //conf.totalsize( )
                                                       , conf.remainsize( ));
                }
            }

            //打开临时文件，用于存放列表文件
            boost::filesystem::fstream Op("ClientConfigFileTmp", std::ios::out | std::ios::binary | std::ios::trunc);

            if (!ClientInfo.SerializePartialToOstream(&Op))
            {
                return;
            }
            Op.close( );

            std::string  filename = "ClientConfigFileTmp";
            filename += "+";
            filename += vstr[1];

            boost::asio::io_service io_ser;
            SendFile sender;
            try
            {
                sender.sender(io_ser, "127.0.0.1", 8089, filename.c_str( ), 'c');
            } catch (std::exception& e)
            {
                std::cout << e.what( ) << std::endl;
            }

            ////解析请求类型，调用不同的处理函数
            //switch (file_info_.m_ReqiureType)
            //{
            //case 'a':       //请求建立连接
            //    socket_.async_receive(
            //        boost::asio::buffer(buffer_, k_buffer_size)
            //        , boost::bind(&Session::CheckKey
            //        , shared_from_this( )
            //        , boost::asio::placeholders::error));
            //    break;
            //case 'b':       //发送验证结果
            //    socket_.async_receive(
            //        boost::asio::buffer(buffer_, k_buffer_size)
            //        , boost::bind(&Session::handle_file
            //        , this
            //        , boost::asio::placeholders::error));
            //    break;
            //default:
            //    break;
            //}
        }

        //接收已经上传到存储服务端的文件块对应的信息
        if (file_info_.m_ReqiureType == 'f')
        {
            std::cout << "接收已经上传到存储服务端的文件块对应的信息" << std::endl;
            dof( );
        }

        //文件传输完成，进行块统计
        if (file_info_.m_ReqiureType == 'g')
        {
            size_t countTimes = std::stoi(vstr[4]);
            Sleep(10 * countTimes);
            //从中删选数据
            std::cout << "文件传输完成，进行块统计" << std::endl;
            qiuwanli::BlockList4DownTable TmpList;
            int countBlock = 0;
            {
                std::lock_guard<std::mutex> guard(g_mutex);
                for (int index = 0; index < g_ComData.m_BlockToFileTableTmp.blocklistfordown_size( ); ++index)
                {
                    auto item = g_ComData.m_BlockToFileTableTmp.mutable_blocklistfordown(index);
                    std::string itemT = item->filesha512( );

                    std::vector<std::string> strss;
                    boost::split(strss, itemT, boost::is_any_of("-"), boost::token_compress_on);

                    if (strss[0].compare(vstr[1]) == 0)
                    {
                        ++countBlock;
                    }

                    if (item->filesha512( ).compare(vstr[1]) == 0)
                    {
                        PublicData.DoBlockList4DownTable(TmpList.add_blocklistfordown( )
                                                         , item->filesha512( )
                                                         , item->blocknumer( )
                                                         , item->blockmd5( )
                                                         , item->saveserversip( ));
                        item->Clear( );
                    }

                }
            }

            size_t count = std::stoi(vstr[4]);
            std::string  filenamed = vstr[0];
            filenamed += "+";
            //将完整部分进行合并，并序列化到文件
            if (countBlock == count)
            {
                std::lock_guard<std::mutex> guard(g_mutex1);
                g_ComData.m_BlockToFileTable.MergeFrom(TmpList);

                //设置响应条件，更新UI
                g_ComData.m_UIChange |= (size_t) 1;

                //将块信息临时存放表写入到块信息列表
                g_ComData.NotifyFileList.push_back(ComData::VecStr5(vstr[0], vstr[1], vstr[5], vstr[3], "否"));

                //将完整的信息添加到文件列表
                PublicData.DoFileInfoListTable(g_ComData.m_FileListTable.add_filelist( )
                                               , vstr[1]
                                               , vstr[2]
                                               , vstr[0]
                                               , vstr[3]
                                               , "否"
                                               , atoll(vstr[4].c_str( ))
                                               , atoll(vstr[5].c_str( ))
                                               , "");
            }
            else
            {
                filenamed += "false";
                std::lock_guard<std::mutex> guard(g_mutex);
                //若数据不完整则将，愿数据块信息插入到块信息临时存放表
                g_ComData.m_BlockToFileTableTmp.MergeFrom(TmpList);
            }

            try
            {
                boost::asio::io_service io_ser;
                SendFile send;
                send.senderLitter(io_ser, "127.0.0.1", 8089, filenamed.c_str( ), 'h');
            } catch (std::exception& e)
            {
                std::cout << e.what( ) << std::endl;
            }

        }

        //存储端发来的心跳连接
        if (file_info_.m_ReqiureType == 'i')
        {
            std::lock_guard<std::mutex> guard(g_mutex);
            g_ComData.m_UIChange |= size_t(2);
            g_ComData.m_HeartList.push_back(ComData::HeartLink(vstr[0]
                                            , atol(vstr[1].c_str( ))
                                            , atol(vstr[2].c_str( ))
                                            , vstr[3]));
        }

        //文件下载请求
        if (file_info_.m_ReqiureType == 'A')
        {
            std::cout << "文件下载请求" << std::endl;
            //用户和密码，验证
            bool isok = false;
            {
                std::lock_guard<std::mutex> guard(g_mutex);
                for (int index = 0; index < g_ComData.m_UserhasFile.user_size( ); ++index)
                {
                    if (g_ComData.m_UserhasFile.user(index).userid( ) == vstr[2] &&
                        g_ComData.m_UserhasFile.user(index).userps( ) == vstr[3])
                    {
                        isok = true;
                    }
                }
            }
            if (!isok)
            {
                socket_.shutdown_both;
                return;
            }

            //检查文件是否存在和完整
            unsigned long long CountBlock;

            //当前在线的存储服务器列表
            qiuwanli::FileInfoList FileInfo;
            qiuwanli::ClientConfigFileTable StorageServiceIPList;
            //qiuwanli::BlockList4Down WaitingForDownloadedFileBlockList;
            qiuwanli::BlockList4DownTable BlocksList;

            if (g_ComData.m_FileListTable.filelist_size( ) > 0 &&
                g_ComData.m_UserhasFile.user_size( ) > 0)
            {
                //检查文件列表中是否存在
                for (int index = 0; index < g_ComData.m_FileListTable.filelist_size( ); ++index)
                {
                    if (g_ComData.m_FileListTable.filelist(index).filesha512( ).compare(vstr[1]))
                    {
                        //保存文件待下载的信息，用于文件还原的验证
                        FileInfo.set_filesha512(g_ComData.m_FileListTable.filelist(index).filesha512( ));
                        FileInfo.set_filemd5(g_ComData.m_FileListTable.filelist(index).filemd5( ));
                        FileInfo.set_filename(g_ComData.m_FileListTable.filelist(index).filename( ));
                        FileInfo.set_filecreatedate(g_ComData.m_FileListTable.filelist(index).filecreatedate( ));
                        FileInfo.set_isshared(g_ComData.m_FileListTable.filelist(index).isshared( ));
                        FileInfo.set_fileallblock(g_ComData.m_FileListTable.filelist(index).fileallblock( ));
                        FileInfo.set_filetotalsize(g_ComData.m_FileListTable.filelist(index).filetotalsize( ));
                        FileInfo.set_tag(g_ComData.m_FileListTable.filelist(index).tag( ));

                        //遍历当前在线的存储服务器，将存在的IP地址保存到待发送列表中
                        for (int index3 = 0; index3 < g_ComData.m_ClientConfigFile.clientinfo_size( ); ++index3)
                        {
                            const qiuwanli::ClientConfigFile item = g_ComData.m_ClientConfigFile.clientinfo(index3);
                            
                            //过滤掉不在线存储端            
                            if (item.online( ).compare("ON") )
                                //和剩余空间大小不足1GB的Server
                                //&& ( item.remainsize()>(unsigned long long)(1024 * 1024)))
                            {
                                PublicData.DoClientConfigFileTable(StorageServiceIPList.add_clientinfo( )
                                                                   , item.cilentid()
                                                                   , item.saveip()
                                                                   , ""             //不传递私钥
                                                                   ,item.keymd5()
                                                                   ,item.totalsize()
                                                                   ,item.remainsize() );
                            }
                        }

                        //将文件对应的存储位置信息过滤出来
                        CountBlock = g_ComData.m_FileListTable.filelist(index).fileallblock( );
                        for (int index2 = 0; index2 < g_ComData.m_BlockToFileTable.blocklistfordown_size( ); ++index2)
                        {
                            const qiuwanli::BlockList4Down blockItem = g_ComData.m_BlockToFileTable.blocklistfordown(index2);
                            if (blockItem.filesha512( ).compare(vstr[1]))
                            {
                                //剔除不在线的存储服务器所存储的文件块信息
                                for (int index4 = 0; index < StorageServiceIPList.clientinfo_size( ); ++index4)
                                {
                                    if (StorageServiceIPList.clientinfo(index4).saveip( ).compare(blockItem.saveserversip()))
                                    {
                                        DoBlockList4DownTable(BlocksList.add_blocklistfordown( )
                                                              , blockItem.filesha512( )
                                                              , blockItem.blocknumer( )
                                                              , blockItem.blockmd5( )
                                                              , blockItem.saveserversip( ));
                                    }
                                }
                            }
                        }

                        break;
                    }
                }
            }

            //网络连接对象
            boost::asio::io_service io_ser;
            SendFile send;

            //文件是否存在//文件是否完整
            if ((BlocksList.blocklistfordown_size( ) > 0) && (BlocksList.blocklistfordown_size( ) == CountBlock))
            {
                ////将在线的存储服务器列表写入到文件
                //std::string ClientListKeepFile = "OnLineStorageServiceIPList";
                //boost::filesystem::ofstream ClientOutFile(ClientListKeepFile, std::ios::trunc | std::ios::out | std::ios::binary);
                //if (!ClientOutFile.is_open( ))
                //    return;
                //if (!StorageServiceIPList.SerializePartialToOstream(&ClientOutFile))
                //    return;
                //ClientOutFile.close( );


                //将文件块存储列表序列化数据到文件
                std::string BlocksListKepFile = BlocksList.blocklistfordown(0).filesha512( ) + "-BlockInfoList";
                boost::filesystem::ofstream BlockOutFile(BlocksListKepFile, std::ios::out | std::ios::trunc | std::ios::binary);
                if (!BlockOutFile.is_open( ))
                    return;
                if (!BlocksList.SerializePartialToOstream(&BlockOutFile))
                    return;
                BlockOutFile.close( );
                BlocksList.Clear( );

                //try
                //{
                //    //则将文件存储表以及在线的存储端列表先将文件发送给客户端
                //    send.sender(io_ser, "127.0.0.1", 8089, ClientListKeepFile.c_str( ), 'Z');
                //    send.sender(io_ser, "127.0.0.1", 8089, BlocksListKepFile.c_str( ), 'Z');
                //} catch (const std::exception& e)
                //{
                //    std::cout << e.what( ) << std::endl;
                //    return;
                //}

                //再发送文件解析请求
                try
                {
                    //块信息所存储的文件名+文件SHA512+文件MD5+文件名+文件块数+文件大小
                    std::string filename12 = BlocksListKepFile;
                    filename12 += "+";
                    filename12 += FileInfo.filesha512( );
                    filename12 += "+";
                    filename12 += FileInfo.filemd5( );
                    filename12 += "+";
                    filename12 += FileInfo.filename( );
                    filename12 += "+";
                    filename12 += std::to_string(FileInfo.fileallblock( ));
                    filename12 += "+";
                    filename12 += std::to_string(FileInfo.filetotalsize( ));

                    send.sender(io_ser, "127.0.0.1", 8089, filename12.c_str( ), 'C');
                } 
                catch (std::exception& e)
                {
                    std::cout << e.what( ) << std::endl;
                }
            }
            else
            {
                //不存在，则返回错误消息
                try
                {
                    std::string filename12 = vstr[0];
                    filename12 += "+";
                    filename12 += vstr[1];

                    send.senderLitter(io_ser, "127.0.0.1", 8089, filename12.c_str( ), 'B');
                } catch (std::exception& e)
                {
                    std::cout << e.what( ) << std::endl;
                }
            }
        }

        //创建文件分享请求
        if (file_info_.m_ReqiureType == 'G')
        {
            std::cout << "创建文件分享请求" << std::endl;
            //如果用户存在，遍历文件表，检查文件是否存在
            //若存在，则生成一个8位的随机数，并和文件SHA512、
            //文件大小、用户ID组成一个分享标识，发送给客户端
            boost::asio::io_service io_ser;
            SendFile send;

            if (CheckKey(vstr[3], vstr[4]))
            {
                for (int index = 0; index < g_ComData.m_SharedTable.sharedinfo_size( ); ++index)
                {
                    if (g_ComData.m_SharedTable.sharedinfo(index).sha512( ).compare(vstr[1]) == 0)
                    {
                        char randStr[9];
                        rand_str(randStr, 8);
                        std::string msg = vstr[0];
                        msg += "+";
                        msg += vstr[1];
                        msg += "+";
                        msg += vstr[2];
                        msg += "+";
                        msg += vstr[3];
                        msg += "+";
                        msg += randStr;

                        try
                        {
                            send.senderLitter(io_ser, "127.0.0.1", 8089, msg.c_str( ), 'I');
                        } catch (std::exception& e)
                        {
                            std::cout << e.what( ) << std::endl;
                        }
                    }
                }
            }
            else
            {
                try
                {
                    send.senderLitter(io_ser, "127.0.0.1", 8089, "文件分享失败", 'H');
                } catch (std::exception& e)
                {
                    std::cout << e.what( ) << std::endl;
                }
            }
        }

        //分享删除请求
        if (file_info_.m_ReqiureType == 'J')
        {
            std::cout << "分享删除请求" << std::endl;
            //如果用户存在，遍历分享文件表，检查分享链接是否存在
            //若存在，则将其删除，若不存在，则返回错误信息
            std::string done = "删除失败";
            if (CheckKey(vstr[2],vstr[3]))
            {
                for (int index = g_ComData.m_SharedTable.sharedinfo_size( ); index >= 0; --index)
                {
                    auto xx = g_ComData.m_SharedTable.sharedinfo(index);
                    if (xx.sha512().compare(vstr[0])&& 
                        xx.verificationcode().compare(vstr[1]) )
                    {
                        xx.Clear( );
                        done.clear( );
                        done = "删除成功";
                    }
                }
            }

            std::string name = vstr[0];
            name += "+";
            name += done;

            boost::asio::io_service io_ser;
            SendFile send;
            try
            {
                send.senderLitter(io_ser, "127.0.0.1", 8089, name.c_str(), 'H');
            } catch (std::exception& e)
            {
                std::cout << e.what( ) << std::endl;
            }
        }

        //删除文件关联请求
        if (file_info_.m_ReqiureType == 'N')
        {
            std::cout << "删除文件关联请求" << std::endl;
            if (CheckKey( "","" ))
            {

            }
            else
            {

            }
        }

        //分享链接下载请求
        if (file_info_.m_ReqiureType == 'X')
        {
            std::cout << "分享链接下载请求" << std::endl;
            if (CheckKey(vstr[4],vstr[5]))
            {
                for (int index = 0; index < g_ComData.m_SharedTable.sharedinfo_size( ); ++index)
                {
                    if (g_ComData.m_SharedTable.sharedinfo(index).sha512( ).compare(vstr[0]) &&
                        std::to_string(g_ComData.m_SharedTable.sharedinfo(index).filesize( )).compare(vstr[1]) &&
                        g_ComData.m_SharedTable.sharedinfo(index).verificationcode( ).compare(vstr[2]) &&
                        g_ComData.m_SharedTable.sharedinfo(index).userid( ).compare(vstr[3]))
                    {
                     //查找文件块列表，将块存储列表，发送给客户端
                     //检查文件是否存在和完整
                        qiuwanli::BlockList4DownTable BlocksList;
                        unsigned long long CountBlock;
                        qiuwanli::FileInfoList FileInfo;
                        if (g_ComData.m_FileListTable.filelist_size( ) > 0 &&
                            g_ComData.m_UserhasFile.user_size( ) > 0)
                        {
                            for (int index = 0; index < g_ComData.m_FileListTable.filelist_size( ); ++index)
                            {
                                if (g_ComData.m_FileListTable.filelist(index).filesha512( ).compare(vstr[1]))
                                {
                                    FileInfo.set_filesha512(g_ComData.m_FileListTable.filelist(index).filesha512( ));
                                    FileInfo.set_filemd5(g_ComData.m_FileListTable.filelist(index).filemd5( ));
                                    FileInfo.set_filename(g_ComData.m_FileListTable.filelist(index).filename( ));
                                    FileInfo.set_filecreatedate(g_ComData.m_FileListTable.filelist(index).filecreatedate( ));
                                    FileInfo.set_isshared(g_ComData.m_FileListTable.filelist(index).isshared( ));
                                    FileInfo.set_fileallblock(g_ComData.m_FileListTable.filelist(index).fileallblock( ));
                                    FileInfo.set_filetotalsize(g_ComData.m_FileListTable.filelist(index).filetotalsize( ));
                                    FileInfo.set_tag(g_ComData.m_FileListTable.filelist(index).tag( ));

                                    CountBlock = g_ComData.m_FileListTable.filelist(index).fileallblock( );
                                    for (int index2 = 0; index < g_ComData.m_BlockToFileTable.blocklistfordown_size( ); ++index2)
                                    {
                                        if (g_ComData.m_BlockToFileTable.blocklistfordown(index2).filesha512( ).compare(vstr[1]))
                                        {
                                            DoBlockList4DownTable(BlocksList.add_blocklistfordown( )
                                                                  , g_ComData.m_BlockToFileTable.blocklistfordown(index2).filesha512( )
                                                                  , g_ComData.m_BlockToFileTable.blocklistfordown(index2).blocknumer( )
                                                                  , g_ComData.m_BlockToFileTable.blocklistfordown(index2).blockmd5( )
                                                                  , g_ComData.m_BlockToFileTable.blocklistfordown(index2).saveserversip( )
                                            );
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        //文件是否存在//文件是否完整
                        if ((BlocksList.blocklistfordown_size( ) <= 0) && (BlocksList.blocklistfordown_size( ) != CountBlock))
                        {
                            //不存在，则返回错误消息
                            try
                            {
                                std::string filename12 = vstr[0];
                                filename12 += "+";
                                filename12 += vstr[1];

                                boost::asio::io_service io_ser;
                                SendFile send;
                                send.senderLitter(io_ser, "127.0.0.1", 8089, filename12.c_str( ), 'B');
                            } catch (std::exception& e)
                            {
                                std::cout << e.what( ) << std::endl;
                            }
                        }
                        else
                        {
                            //序列化数据到文件
                            boost::filesystem::ofstream outfile(BlocksList.blocklistfordown(0).filesha512( )
                                                                , std::ios::out | std::ios::trunc | std::ios::binary);
                            //若存在，则将文件存储表发送给客户端
                            try
                            {
                                //文件SHA512+文件MD5+文件名+文件块数+文件大小
                                std::string filename12 = FileInfo.filesha512( );
                                filename12 += "+";
                                filename12 += FileInfo.filemd5( );
                                filename12 += "+";
                                filename12 += FileInfo.filename( );
                                filename12 += "+";
                                filename12 += std::to_string(FileInfo.fileallblock( ));
                                filename12 += "+";
                                filename12 += std::to_string(FileInfo.filetotalsize( ));

                                boost::asio::io_service io_ser;
                                SendFile send;
                                send.senderLitter(io_ser, "127.0.0.1", 8089, filename12.c_str( ), 'C');
                            } catch (std::exception& e)
                            {
                                std::cout << e.what( ) << std::endl;
                            }

                            BlocksList.Clear( );
                        }
                    }
                }
            }
            else
            {
                try
                {
                    std::string name="文件不存在或分享链接失效\r\n请尝试其他分享链接！";
                    boost::asio::io_service io_ser;
                    SendFile send;
                    send.senderLitter(io_ser, "127.0.0.1", 8089, name.c_str( ), 'C');
                } catch (std::exception& e)
                {

                }
            }
        }

    }
    
    //随机生成字符串
    char *rand_str(char *str, const int len)
    {
        int i;
        for (i = 0; i < len; ++i)
            str[i] = 'A' + rand( ) % 26;
        str[++i] = '\0';
        return str;
    }

    //检查客户端携带的PirateKey的MD5是否正确，失败则关闭链接
    bool CheckKey(std::string id,std::string ps)
    {
        //用户和密码，验证
        std::lock_guard<std::mutex> guard(g_mutex);
        for (int index = 0; index < g_ComData.m_UserhasFile.user_size( ); ++index)
        {
            if (g_ComData.m_UserhasFile.user(index).userid( ) == id &&
                g_ComData.m_UserhasFile.user(index).userps( ) == ps)
            {
                return true;
            }
        }
        return false;
    }

    ////发送文件块，并将发送失败的列表，返回
    //PairVec& SendFileToClient(PairVec& DownList)
    //{
    //    //遍历文件列表，传输完成的从列表中删除
    //    for (auto& FileBlocks : DownList)
    //    {
    //        std::string File512 = FileBlocks.first;
    //        for (auto Block : FileBlocks.second)
    //        {
    //            //查找文件块，若存在则读取文件块和对应块的MD5，并发送给客户端
    //        }
    //    }
    //    return DownList;
    //}


    //将接受到的数据块，解析为文件名+文件数据
    void handle_file(const boost::system::error_code& error)
    {
        if (error)
        {
            return print_asio_error(error);
        }

        const char *base_name_msg = buffer_ + file_info_.m_FileNameLength - 1;
        while (base_name_msg >= buffer_ &&
            (*base_name_msg != '\\'
               && *base_name_msg != '/'))
        {
            --base_name_msg;
        }
        ++base_name_msg;

        const char *basename = base_name_msg;

        //将const char* 分割
        std::string str(base_name_msg);
        //std::vector<std::string>  vstr;
        boost::split(vstr, str, boost::is_any_of("+"), boost::token_compress_on);
        basename = vstr[0].c_str( );

        fopen_s(&fp_,basename, "wb");
        if (fp_ == NULL)
        {
            std::cerr << "Failed to open file to write\n";
            return;
        }

        receive_file_content( );
    }

    void receive_file_content( )
    {
        socket_.async_receive(boost::asio::buffer(buffer_, k_buffer_size)
                              , boost::bind(&Session::handle_write
                              , shared_from_this( )
                              , boost::asio::placeholders::error
                              , boost::asio::placeholders::bytes_transferred)
        );
    }

    void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
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
            }

            return;
        }
        total_bytes_writen_ += fwrite(buffer_, 1, bytes_transferred, fp_);

        receive_file_content( );
    }

    //发送文件
    void send_file_content_sender(const char*	ip_address
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
    std::vector<std::string>  vstr;

};


#endif // !SESSION_HPP
