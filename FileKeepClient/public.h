#ifndef PUBLIC_H__
#define PUBLIC_H__
#include <vector>
#include <iostream>
#include <string>

//文件块大小//
const unsigned int BLOCK_SIZE = 1024 * 64;
const unsigned int FILE_MAX = ~0;
//k*1024
const unsigned int k_times = 32;

const unsigned long long k_buffer_size = 1024 * k_times;
//常量数据

const unsigned long DelayTime_Heart = 60 * 1000;
const unsigned long DelayTime_SendInfo = 15 * 1000;
const unsigned long DelayWriteToFile = DelayTime_Heart * 15;

const unsigned int Size_Kb = 1 << 10;
const unsigned int Size_Mb = 1 << 20;
const unsigned int Size_Gb = 1 << 30;


//消息类型，以及消息名称长度，文件块大小
struct DataBlockTypeInfo
{
    using  Size_type = unsigned long long;
    char m_ReqiureType;
    Size_type m_FileSize;
    unsigned int m_FileNameLength;

    DataBlockTypeInfo( ) : m_FileSize(0), m_FileNameLength(0) ,m_ReqiureType(0){ }
};


//Require Type
char Type_f[ ] = {'a'       //请求验证
                ,'b'        //验证结果
                ,'c'        //
                ,'d'        //
                ,'e'        //
                ,'f'        //
                ,'g'        //
                ,'h'        //
                ,'s'        //
                ,'j'        //
                ,'t'        //
                ,'x'        //
                
                ,'y'        //文件请求下载列表+验证信息
                ,'z'        //文件块+MD5+filesha512
                ,'w'        //文件块信息+IP 发送到目录服务器
                ,'q'        //文件块上传
                ,'p'};      //

using PairVec = std::vector<std::pair<std::string, std::vector<unsigned long>>>;
#endif // !PUBLIC_H__
