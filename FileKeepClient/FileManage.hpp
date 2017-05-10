#ifndef FILE_MANAGE_HPP
#define FILE_MANAGE_HPP

#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include "public.h"

class FileManage
{
public:
    FileManage( );
    ~FileManage( );

    bool CreateFile(std::string& File);

    bool open(const std::string& FileName = "");
    char* ReadFileBlock(char * fileBlock, const size_t OffSet, const size_t CurSize);
    bool WriteFileBlock(char * WriteBlock, const size_t CurSize, const size_t OffSet = (0 - 1));
    size_t WriteFileBlockEnd(char * WriteBlock, const size_t CurSize);
    size_t Getfilesize(const boost::filesystem::path& FileName);
    std::string getFileName( )
    {
        return m_FileName.string();
    }
private:
    //char*       m_FileBlockBuf;
    size_t      m_FileBlockConut;
    size_t      m_FilePtrCurPos;
	boost::filesystem::fstream m_FileInOut;
	boost::filesystem::path m_FileName;
};


#endif // !FILE_MANAGE_HPP
