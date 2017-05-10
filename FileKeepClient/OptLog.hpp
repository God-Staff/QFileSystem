#ifndef OPT_LOG_HPP
#define OPT_LOG_HPP
#include <iostream>
#include <mutex>
#include <boost/thread.hpp>
#include <fstream>
#include <boost/filesystem.hpp>
#include <string>

#include "FileDataStruct.pb.h"

class OptLog
{

public:
    OptLog( )
    {
        m_fout.open("log.txt");
        if (!m_fout.is_open( ))
        {
            return;
        }
    }

    ~OptLog( ) 
    {
        if (m_fout.is_open( ))
        {
            m_fout.close( );
        }
    }

    void log(const std::string logs)
    {
        boost::lock_guard<boost::mutex> locker_m(m_mutex);
        m_fout << GetTime( ) << "\t" << logs << std::endl;
    }

    std::string GetTime( )
    {
        std::string CurTime;
        time_t calendar_time = time(NULL);
        struct tm * tm_local{};
        
        localtime_s(tm_local,&calendar_time);

        char str_f_t[100];
        strftime(str_f_t, sizeof(str_f_t), "%G-%m-%d %H:%M:%S", tm_local);

        CurTime = str_f_t;
        CurTime += "\t";

        return CurTime;
    }

private:
    boost::mutex m_mutex;
    boost::filesystem::fstream m_fout;
};


#endif // !OPT_LOG_HPP
