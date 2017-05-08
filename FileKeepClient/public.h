#ifndef PUBLIC_H__
#define PUBLIC_H__

//文件块大小//
const size_t BLOCK_SIZE = 1024 * 64;
const size_t FILE_MAX = ~0;
//常量数据

//k*1024
constexpr size_t k_times = 32;
typedef unsigned long long Size_type;

struct DataTypeInfo
{
    Size_type filesize;
    size_t filename_size;
    DataTypeInfo( ) : filesize(0), filename_size(0) { }
};

#endif // !PUBLIC_H__
