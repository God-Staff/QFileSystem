#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <string>

class locker
{
private:
	std::mutex m_mutex;
	std::ifstream fout;
public:
	locker(){fout.open("log.txt")};
	~locker();
	void log(std::string ss)
	{
		std::lock_guard<std::mutex> locker_m(m_mutex);
		fout<<ss<<std::endl;
	}
	
};

int main(int argc, char const *argv[])
{
	
	return 0;
}