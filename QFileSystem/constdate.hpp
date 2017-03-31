#ifndef CONST_DADTA_HPP
#define CONST_DADTA_HPP

#include "date.pb.h"
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <locale> 
#include <codecvt>
#include <atlconv.h>

//常量数据

//k*1024
constexpr size_t k_times = 32;	



struct File_info {
	typedef unsigned long long Size_type;
	Size_type filesize;
	size_t filename_size;
	File_info () : filesize (0), filename_size (0) {}
};
#endif // !CONST_DADTA_HPP
