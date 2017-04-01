#ifndef OPT_LOG_HPP
#define OPT_LOG_HPP
#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <string>


class OptLog
{
private:
	std::mutex m_mutex;
	std::ifstream fout;
public:
	OptLog () { fout.open ("log.txt"); }
	~OptLog () {}
	void log (std::string ss)
	{
		std::lock_guard<std::mutex> locker_m (m_mutex);
		fout << ss << std::endl;
	}

};


#endif // !OPT_LOG_HPP
