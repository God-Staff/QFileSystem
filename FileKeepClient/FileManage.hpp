#ifndef FILE_MANAGE_HPP
#define FILE_MANAGE_HPP

#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
class FileManage
{
public:
	FileManage ():m_FileName("") {}
	FileManage (std::string FileName)
	{
		m_FileName = FileName;
		if (!m_FileInOut.is_open ())
		{
			m_FileInOut.open (m_FileName);
		}
	}
	~FileManage () 
	{
		if (m_FileInOut.is_open())
			m_FileInOut.close ();
	}
	bool open (std::string FileName ="")
	{
		if (0 == FileName.compare(""))
		{
			m_FileName = FileName;
		}

		if (!m_FileInOut.is_open ())
		{
			m_FileInOut.open (m_FileName);
			if (m_FileInOut)
			{
				return true;
			}
			return false;
		}
	}
private:
	boost::filesystem::fstream m_FileInOut;
	boost::filesystem::path m_FileName;
};



#endif // !FILE_MANAGE_HPP
