//#include <string>
//#include <iostream>
//#include <boost/filesystem.hpp>
//#include <windows.h>
//#include <thread>
//
////char* ReadFileBlock(char* fileBlock, const size_t OffSet, const size_t CurSize)
////{
////    //超出文件区域
////    if (OffSet > (m_FileBlockConut - 1))
////    {
////        return nullptr;
////    }
////
////    //申请内存，存储文件
////    fileBlock = new char[BLOCK_SIZE];
////
////    //将文件指针移动到指定位置
////    m_FileInOut->seekg(OffSet*BLOCK_SIZE);
////    m_FileInOut->read(fileBlock, CurSize);
////
////    return fileBlock;
////}
////
//////写入数据
////bool WriteFileBlock(char* WriteBlock, const size_t CurSize, const size_t OffSet)
////{
////    if (OffSet == FILE_MAX)
////    {
////        // return false;
////    }
////
////    m_FileInOut->seekp(0);
////    m_FileInOut->write(WriteBlock, CurSize);
////
////    return false;
////}
////
////size_t Getfilesize(const boost::filesystem::path& FileName)
////{
////    return FileName.size( );
////}
//
//
//void doItNextTime( )
//{
//    //先更新本地服务端数据
//
//    //再将数据同步到目录服务器
//
//    Sleep(3000);
//
//    std::cout << "Time:" << std::endl;
//    doItNextTime( );
//}
//
//
//void doItNextTime1( )
//{
//    //先更新本地服务端数据
//
//    //再将数据同步到目录服务器
//
//    Sleep(300);
//
//    std::cout << "3333333:" << std::endl;
//    doItNextTime1( );
//}
//#include "qq.h"
//#include "qqq.h"
//
//void ff( )
//{
//    std::cout << fff << "  " << &fff << std::endl;
//    fff = 23450;
//    std::cout << fff << "  " << &fff << std::endl;
//    int fff = 34560;
//    std::cout << fff << "  " << &fff << std::endl;
//}
//
//
//int main( )
//{
//
//    f( );
//
//    ff( );
//
//    f( );
//
//
///*
//    std::thread t1(doItNextTime);
//
//    t1.detach( );
//    doItNextTime1( );*/
//    //std::string FileName="ttt.txt";
//    //boost::filesystem::path FilePath(FileName);
//    //
//    //boost::filesystem::fstream m_FileInOut(FileName, std::ios::out | std::ios::in | std::ios::binary);
//    //if (!m_FileInOut.is_open( ))
//    //{
//    //    std::cout << "文件打开失败" << std::endl;
//    //    return 0;
//    //}
//
//    //std::cout << "File Name: " << FileName << "\nfilesize:" << FileName.size( )
//    //    << std::endl;
//    ////char* BlocksFile = new char[BLOCK_SIZE];
//    //std::string BlocksFile = "ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCDABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD              ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD";
//
//    //m_FileInOut.seekp(0);
//    //m_FileInOut.write(BlocksFile.c_str(), BlocksFile.size());
//    //m_FileInOut.close( );
//
//    return 0;
//}

#include <boost/filesystem.hpp> 
#include <iostream> 

int main( )
{
    boost::filesystem::path p("photo.jpg");
    std::cout << p.stem( ) << std::endl;
    std::cout << p.extension( ) << std::endl;
}