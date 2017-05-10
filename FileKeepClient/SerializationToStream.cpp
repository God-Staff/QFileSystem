#include "SerializationToStream.hpp"

#include <google/protobuf/repeated_field.h>

namespace qiuwanli
{

void SerialToStream::MakeSerailToStream(qiuwanli::ConfigFile* Config
                                        , const std::string& SaveClientID
                                        , const std::string& SaveClientIP
                                        , const std::string& Prikey
                                        , const std::string& PriKeyMD5
                                        , const unsigned long TotalSize
                                        , const unsigned long RemainSize)
{
    Config->set_id(SaveClientID);
    Config->set_ip(SaveClientIP);
    Config->set_prikey(Prikey);
    Config->set_prikeymd5(PriKeyMD5);
    Config->set_totalsize(TotalSize);
    Config->set_remainsize(RemainSize);
}


void SerialToStream::MakeBlockInfo(qiuwanli::BlockInfo* Blocks
                                   , const std::string& FileSHA512
                                   , const std::string& BlockMd5
                                   , const std::string& SaveFileName
                                   , const unsigned long BlockNum
                                   , const unsigned long CurSize
                                   , const unsigned long FileBlockOffSet)
{
    Blocks->set_filesha512(FileSHA512);
    Blocks->set_blockmd5(BlockMd5);
    Blocks->set_savefilename(SaveFileName);
    Blocks->set_blocknumer(BlockNum);
    Blocks->set_cursize(CurSize);
    Blocks->set_fileblockoffset(FileBlockOffSet);
}

void SerialToStream::MakeBlockInfoSend(qiuwanli::BlockInfo* Blocks
                                   , const std::string& FileSHA512
                                   , const std::string& BlockMd5
                                   , const std::string& SaveFileName
                                   , const unsigned long BlockNum
                                   , const unsigned long CurSize)
{
    Blocks->set_filesha512(FileSHA512);
    Blocks->set_blockmd5(BlockMd5);
    Blocks->set_savefilename(SaveFileName);
    Blocks->set_blocknumer(BlockNum);
    Blocks->set_cursize(CurSize);
}

void SerialToStream::MakeBlock(qiuwanli::Block* Block, const unsigned long BlockItem)
{ 
    Block->set_blockitem(BlockItem);
}

void SerialToStream::MakeBlockListForDown(qiuwanli::BlockListForDown* DownList
                                          , const std::string& FileSHA512
                                          , const std::string& SaveServersIP
                                          , const std::string& FileMd5
                                          , const std::vector<unsigned long>& BlockList)
{
    DownList->set_filesha512(FileSHA512);
    DownList->set_saveserversip(SaveServersIP);
    DownList->set_filemd5(FileMd5);
    for (auto x : BlockList)
    {
        qiuwanli::Block* Item = DownList->add_blocks( );
        MakeBlock(Item, x);
    }
}

void SerialToStream::MakeBlockCheck(qiuwanli::BlockCheck* BlockCheck
                                    , const unsigned long BlockItem
                                    , const std::string& BlockMd5)
{ 
    BlockCheck->set_blockitem(BlockItem);
    BlockCheck->set_blockmd5(BlockMd5);
}

void SerialToStream::MakeBlockListCheckForDown(qiuwanli::BlockListForDownCheck* DownList
                                                , const std::string& FileSHA512
                                                , const std::string& SaveServersIP
                                                , const std::string& FileMd5
                    , const std::vector<std::pair<unsigned long,std::string> >& BlockList)
{ 
    DownList->set_filesha512(FileSHA512);
    DownList->set_saveserversip(SaveServersIP);
    DownList->set_filemd5(FileMd5);

    //使用遍历，对数据进行填充
    for (auto x : BlockList)
    {
        qiuwanli::BlockCheck* Item = DownList->add_blocks( );
        MakeBlockCheck(Item, x.first,x.second);
    }

}

template<typename T>
void SerialToStream::MakeBlock2Server(qiuwanli::BlockInfo2Server * BlockInfo
                                      , const std::string & FileSHA512
                                      , const std::vector<T>& Blocks)
{ 
    BlockInfo->set_filesha512(FileSHA512);
    for (auto x : Blocks)
    {
        qiuwanli::BlockCheck* Item = BlockInfo->add_blocks( );
        MakeBlock(Item, x);
    }
}


}//! End NameSpace qiuwanli

