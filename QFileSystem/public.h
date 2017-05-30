#ifndef PUBLIC_H__
#define PUBLIC_H__
#include <vector>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <vector>

#include "PublicStruct.pb.h"
#include "Interface.h"

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
    unsigned char m_ReqiureType;
    Size_type m_FileSize;
    size_t m_FileNameLength;
    //char m_UserID[11];
    DataBlockTypeInfo( ) : m_FileSize(0), m_FileNameLength(0), m_ReqiureType(0) { }
};

struct ComData
{
    qiuwanli::ClientConfigFileTable m_ClientConfigFile;// 保存存储服务信息，并用于上传文件
    qiuwanli::FileInfoListTable m_FileListTable;    //文件存储列表
    qiuwanli::BlockList4DownTable m_BlockToFileTable;
    qiuwanli::BlockList4DownTable m_BlockToFileTableTmp;
    qiuwanli::UserInfoTable m_UserhasFile;
    qiuwanli::SharedTable m_SharedTable;
    qiuwanli::BlockListForDownCheckTable m_BlockListForDownCheckTable;  //用于文件下载时，传输文件块存储信息

    boost::filesystem::ofstream OpFile;
    boost::filesystem::ofstream OpFileBlockInfo;
    struct Vec4
    {
        std::string m_Sha512;
        std::string m_md5;
        long m_BlockNumer;
        std::string m_IP;
        Vec4(std::string& Sha512, std::string& md5, long BlockNumer, std::string& IP)
            :m_Sha512(Sha512), m_md5(md5), m_BlockNumer(BlockNumer), m_IP(IP)
        {}
    };
    std::vector<Vec4> BlockUploadList;  //块信息临时存放表
    size_t m_UIChange = (size_t)0;
    struct VecStr5
    {
        VecStr5(std::string vv1, std::string vv2, std::string vv3, std::string vv4, std::string vv5) :
            v1(vv1), v2(vv2), v3(vv3), v4(vv4), v5(vv5) {   }
        std::string v1;
        std::string v2;
        std::string v3;
        std::string v4;
        std::string v5;
    };
    std::vector<VecStr5> NotifyFileList;

    struct HeartLink
    {
        std::string ClientID;
        unsigned long long Remain;
        unsigned long long Total;
        std::string Prikeymd5;
        HeartLink(std::string ID, unsigned long long remain, unsigned long long total, std::string& Prikeymd5):
        ClientID(ID), Remain(Remain), Total(total), Prikeymd5(Prikeymd5){ }
    };
    std::vector<HeartLink> m_HeartList;

};

extern ComData g_ComData;
extern CInterface PublicData;

#endif // !PUBLIC_H__
