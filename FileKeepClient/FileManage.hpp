#ifndef FILE_MANAGE_HPP
#define FILE_MANAGE_HPP

#include <fstream>
#include <string>

class FileManage
{
public:
	FileManage () { f_inout = nullptr; }
	~FileManage () 
	{
		if (!f_inout == nullptr)
			f_inout.close ();
	}
	bool open (std::string filename) 
	{
		if(!f_inout.open (filename))
			return false;
		return true;
	}
private:
	std::fstream f_inout;

};



#endif // !FILE_MANAGE_HPP
