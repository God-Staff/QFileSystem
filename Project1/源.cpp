#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

const size_t BLOCK_SIZE = 1024;
const size_t FILE_MAX = ~0;

//char* ReadFileBlock(char* fileBlock, const size_t OffSet, const size_t CurSize)
//{
//    //超出文件区域
//    if (OffSet > (m_FileBlockConut - 1))
//    {
//        return nullptr;
//    }
//
//    //申请内存，存储文件
//    fileBlock = new char[BLOCK_SIZE];
//
//    //将文件指针移动到指定位置
//    m_FileInOut->seekg(OffSet*BLOCK_SIZE);
//    m_FileInOut->read(fileBlock, CurSize);
//
//    return fileBlock;
//}
//
////写入数据
//bool WriteFileBlock(char* WriteBlock, const size_t CurSize, const size_t OffSet)
//{
//    if (OffSet == FILE_MAX)
//    {
//        // return false;
//    }
//
//    m_FileInOut->seekp(0);
//    m_FileInOut->write(WriteBlock, CurSize);
//
//    return false;
//}
//
//size_t GetFileSize(const boost::filesystem::path& FileName)
//{
//    return FileName.size( );
//}

int main( )
{
    std::string FileName="ttt.txt";
    boost::filesystem::path FilePath(FileName);
    
    boost::filesystem::fstream m_FileInOut(FileName, std::ios::out | std::ios::in | std::ios::binary);
    if (!m_FileInOut.is_open( ))
    {
        std::cout << "文件打开失败" << std::endl;
        return 0;
    }

    std::cout << "File Name: " << FileName << "\nFileSize:" << FileName.size( )
        << std::endl;
    //char* BlocksFile = new char[BLOCK_SIZE];
    std::string BlocksFile = "ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCDABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD              ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD ABCDABCDABCDABCD";

    m_FileInOut.seekp(0);
    m_FileInOut.write(BlocksFile.c_str(), BlocksFile.size());
    m_FileInOut.close( );

    [&]( ){

    };
    return 0;
}