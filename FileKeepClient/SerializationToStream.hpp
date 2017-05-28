#ifndef SERIALIZATION_TO_STREAM_HPP
#define SERIALIZATION_TO_STREAM_HPP

//#include "FileDataStruct.pb.h"

#include "PublicStruct.pb.h"

namespace qiuwanli
{

class SerialToStream
{
public:
    SerialToStream( ) { }
    ~SerialToStream( ) { }

    /**
     * 读取本地配置文件
     * @param Config       配置文件的序列化对象
     * @param SaveClientID 存储端ID
     * @param SaveClientIP 存储端IP
     * @param Prikey       私钥
     * @param PriKeyMD5    私钥的MD5
     * @param TotalSize    磁盘总空间
     * @param RemainSize   剩余空间
     */
    void MakeSerailToStream(qiuwanli::ConfigFile* Config
                            , const std::string& SaveClientID
                            , const std::string& SaveClientIP
                            , const std::string& Prikey
                            , const std::string& PriKeyMD5
                            , const unsigned long TotalSize
                            , const unsigned long RemainSize);

    /**
     * 本地文件块存储信息
     * @param Blocks          文件块对象
     * @param FileSHA512      文件的SHA512
     * @param BlockMd5        文件块的MD5验证信息
     * @param SaveFileName    所存储的文件名
     * @param BlockNum        文件块编号
     * @param CurSize         文件块当前大小
     * @param FileBlockOffSet 文件存储偏移量
     */
    void MakeBlockInfo(qiuwanli::BlockInfo* Blocks
                       , const std::string& FileSHA512
                       , const std::string& BlockMd5
                       , const std::string& SaveFileName
                       , const unsigned long BlockNum
                       , const unsigned long CurSize
                       , const unsigned long FileBlockOffSet = 0);

    //将客户短信息解析，写入，并进行序列化
    void MakeBlockInfoSend(qiuwanli::BlockInfo * Blocks
                           , const std::string & FileSHA512
                           , const std::string & BlockMd5
                           , const std::string & SaveFileName
                           , const unsigned long BlockNum
                           , const unsigned long CurSize);
    /**
     * [MakeBlock description]
     * @param Block     [description]
     * @param BlockItem [description]
     */
    //void MakeBlock(qiuwanli::Block* Block, const unsigned long BlockItem);
    /**
     * [MakeBlockListForDown description]
     * @param DownList      [description]
     * @param FileSHA512    [description]
     * @param SaveServersIP [description]
     * @param FileMd5       [description]
     * @param Block         [description]
     */
    //void MakeBlockListForDown(qiuwanli::BlockListForDown* DownList
    //                          , const std::string& FileSHA512
    //                          , const std::string& SaveServersIP
    //                          , const std::string& FileMd5
    //                          , const std::vector<unsigned long>& BlockList);
    /**
     * [MakeBlockCheck description]
     * @param BlockCheck [description]
     * @param BlockItem  [description]
     * @param BlockMd5   [description]
     */
    void MakeBlockCheck(qiuwanli::BlockCheck* BlockCheck
                        , const unsigned long BlockItem
                        , const std::string& BlockMd5);
    /**
     * [MakeBlockListForDown description]
     * @param DownList      [description]
     * @param FileSHA512    [description]
     * @param SaveServersIP [description]
     * @param FileMd5       [description]
     * @param BlockCheck    [description]
     */
    void MakeBlockListCheckForDown(qiuwanli::BlockListForDownCheck* DownList
                                   , const std::string& FileSHA512
                                   , const std::string& SaveServersIP
                                   , const std::string& FileMd5
                                   , const std::vector<std::pair<unsigned long, std::string> >& BlockList);


    //template<typename T>
    ///**
    // * 当文件传输完成，将块信息发送到目录服务器
    // * @param BlockInfo  快对象
    // * @param FileSHA512 文件的SHA512
    // * @param Blocks     块信息向量
    // */
    //void MakeBlock2Server(qiuwanli::BlockInfo2Server* BlockInfo
    //                      , const std::string& FileSHA512
    //                      , const std::vector<T>& Blocks);

};


}   //! end namespace qiuwanli


#endif //! SERIALIZATION_TO_STREAM_HPP
