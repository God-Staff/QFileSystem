#ifndef FILE_MANAGE_HPP
#define FILE_MANAGE_HPP

const size_t BLOCK_SIZE = 1024;
const size_t FILE_MAX = ~0;

#include <fstream>
#include <string>
#include <boost/filesystem.hpp>

class FileManage
{
public:
    FileManage( );
    ~FileManage( );

    bool CreateFile(std::string & File);

    bool open(std::string& FileName);
    char * ReadFileBlock(char * fileBlock, const size_t OffSet, const size_t CurSize);
    bool WriteFileBlock(char * WriteBlock, const size_t CurSize, const size_t OffSet = FILE_MAX);
    size_t GetFileSize(const boost::filesystem::path& FileName);
private:
    //char*       m_FileBlockBuf;
    size_t      m_FileBlockConut;
    size_t      m_FilePtrCurPos;
    boost::shared_ptr<boost::filesystem::fstream> m_FileInOut;
    boost::filesystem::path m_FileName;
};


#endif // !FILE_MANAGE_HPP
