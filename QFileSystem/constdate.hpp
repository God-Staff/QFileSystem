#ifndef CONST_DADTA_HPP
#define CONST_DADTA_HPP

#include "date.pb.h"
#include "Session.hpp"
#include "Servers.hpp"

#include <fstream>
#include <iostream>
#include <atlconv.h>
#include <list>
#include <string>
#include <locale> 
#include <algorithm>
#include <codecvt>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

//常量数据

//k*1024
constexpr size_t k_times = 32;	
typedef unsigned long long Size_type;


//struct File_info 
//{
//	Size_type filesize;
//	size_t filename_size;
//	File_info () : filesize (0), filename_size (0) {}
//};
#endif // !CONST_DADTA_HPP
