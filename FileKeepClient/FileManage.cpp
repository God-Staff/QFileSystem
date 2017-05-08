#include "FileManage.hpp"
#include "public.h"

FileManage::FileManage( ) 
    : m_FileName("")
    , m_FilePtrCurPos(0)
    , m_FileBlockConut(0)
    //, m_FileBlockBuf(nullptr)
{ 

}

FileManage::~FileManage( )
{
    /*if (m_FileBlockBuf!=nullptr)
    {
        delete [ ]m_FileBlockBuf;
        m_FileBlockBuf = nullptr;
    }*/

    if (m_FileInOut.is_open( ))
    {
        m_FileInOut.close( );
    }
}

//若源文件已达到最大值，则创建新文件
bool FileManage::CreateFile( std::string& File)
{

    return true;
}

bool FileManage::open(const std::string& FileName)
{
    if (0 == FileName.compare(""))
    {
        return false;
    }

    m_FileName = FileName;
    m_FileInOut.open(FileName, std::ios::out | std::ios::in | std::ios::binary);
    if (!m_FileInOut.is_open( ))
    {
        return false;
    }

    //保存文件总长度
    m_FileInOut.seekp(0, std::ios_base::end);
    m_FileBlockConut = m_FileInOut.tellp( )/BLOCK_SIZE+1;

    //m_FileBlockBuf = new char[BLOCK_SIZE];

    return true;
}

char* FileManage::ReadFileBlock(char* fileBlock, const size_t OffSet, const size_t CurSize)
{
    //超出文件区域
    if (OffSet > (m_FileBlockConut-1))
    {
        return nullptr;
    }

    //申请内存，存储文件
    fileBlock = new char[BLOCK_SIZE];

    //将文件指针移动到指定位置
    m_FileInOut.seekg(OffSet*BLOCK_SIZE);
    m_FileInOut.read(fileBlock, CurSize);

    return fileBlock;
}

//写入数据
bool FileManage::WriteFileBlock(char* WriteBlock, const size_t CurSize,const size_t OffSet )
{
    if (OffSet == FILE_MAX)
    {
        return false;
    }

    m_FileInOut.seekp(OffSet*BLOCK_SIZE);
    m_FileInOut.write(WriteBlock, CurSize);

    delete []WriteBlock;

    return false;
}

size_t FileManage::Getfilesize(const boost::filesystem::path& FileName)
{
    return FileName.size( );
}
