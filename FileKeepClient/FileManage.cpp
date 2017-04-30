#include "FileManage.hpp"
#include "public.h"

FileManage::FileManage( ) 
    : m_FileName("")
    , FilePtrCurPos(0)
{ 

}

FileManage::~FileManage( )
{
    if (m_FileInOut.is_open( ))
        m_FileInOut.close( );
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

    return true;
}

bool FileManage::ReadFileBlock(boost::filesystem::fstream& FileStream, const size_t OffSet, const size_t CurSize)
{
    m_FileInOut.seekg(OffSet*BLOCK_SIZE);
    //FileStream << m_FileInOut;
    return false;
}

bool FileManage::WriteFileBlock(const boost::filesystem::fstream& FileStream, const size_t OffSet, const size_t CurSize)
{
    m_FileInOut.seekp(OffSet*BLOCK_SIZE);
    m_FileInOut.write("AAABBBCCCRRRAAA", CurSize);

    return false;
}

size_t FileManage::GetFileSize(const boost::filesystem::path& FileName){
    return size_t( );
}
