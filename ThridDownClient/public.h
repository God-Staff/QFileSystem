#ifndef PUBLIC_H__
#define PUBLIC_H__
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include "PublicStruct.pb.h"

//文件块大小//
const unsigned int BLOCK_SIZE = 1024 * 64;
const unsigned int FILE_MAX = ~0;
//k*1024
const unsigned int k_times = 32;

const unsigned long k_buffer_size = 1024 * k_times;
//常量数据

const unsigned long DelayTime_Heart = 60 * 1000;
const unsigned long DelayTime_SendInfo = 15 * 1000;
const unsigned long DelayWriteToFile = DelayTime_Heart * 15;

const unsigned int Size_Kb = 1 << 10;
const unsigned int Size_Mb = 1 << 20;
const unsigned int Size_Gb = 1 << 30;

using PairVec = std::vector<std::pair<std::string, std::vector<unsigned long>>>;

//消息类型，以及消息名称长度，文件块大小
struct DataBlockTypeInfo
{
    using  Size_type = unsigned long long;
    char m_ReqiureType;
    Size_type m_FileSize;
    unsigned int m_FileNameLength;
    char m_UserID[11];
    DataBlockTypeInfo( ) : m_FileSize(0), m_FileNameLength(0), m_ReqiureType(0) { }
};

struct ComData
{
    qiuwanli::ConfigFile Conf;
    qiuwanli::BlockInfoTable BlockTable;
    qiuwanli::BlockInfoTable BlockTableDiff;
    qiuwanli::BlockInfoTable BlockTablePreDiff;
    qiuwanli::FileInfoListTable FileInfoList;
    qiuwanli::ClientConfigFileTable ClientConfigFile;

    boost::filesystem::ofstream OpFile;
    boost::filesystem::ofstream OpFileBlockInfo;
};

extern ComData g_ComData;

#endif // !PUBLIC_H__
