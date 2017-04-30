#ifndef FILE_MANAGE_HPP
#define FILE_MANAGE_HPP

#include <fstream>
#include <string>
#include <boost/filesystem.hpp>

class FileManage
{
public:
    FileManage( );
    ~FileManage( );

    bool open(const std::string& FileName = "");
    bool ReadFileBlock(boost::filesystem::fstream& FileStream, const size_t OffSet, const size_t CurSize);
    bool WriteFileBlock(const boost::filesystem::fstream& FileStream, const size_t OffSet, const size_t CurSize);
    size_t GetFileSize(const boost::filesystem::path& FileName);
private:
    size_t FilePtrCurPos;
	boost::filesystem::fstream m_FileInOut;
	boost::filesystem::path m_FileName;
};


#endif // !FILE_MANAGE_HPP
