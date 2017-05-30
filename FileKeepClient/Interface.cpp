#include "PublicStruct.pb.h"
#include "Interface.h"
#include "bitset"
#include <boost/dynamic_bitset.hpp>

bool CInterface::DoConfigFile(qiuwanli::ConfigFile* conf
                              , const std::string& ID
                              , const std::string& IP
                              , const std::string& PriKey
                              , const std::string& PriKeyMD5
                              , const unsigned long long TotalSize
                              , const unsigned long long RemainSize
                              , const std::string& ServersIP
                              , const std::string& ServersPort)
{
    conf->set_id(ID);
    conf->set_ip(IP);
    conf->set_prikey(PriKey);
    conf->set_prikeymd5(PriKeyMD5);
    conf->set_totalsize(TotalSize);
    conf->set_remainsize(RemainSize);
    conf->set_serversip(ServersIP);
    conf->set_serversport(ServersPort);

    return true;
}

bool CInterface::DoBlockInfoTable(qiuwanli::BlockInfo* blockInfo
                                  , const std::string& FileSHA512
                                  , const std::string& BlockMD5
                                  , const std::string& SaveFileName
                                  , const unsigned long long BlockNumer
                                  , const unsigned long long CurSize
                                  , const unsigned long long FileBlockOffset)
{
    blockInfo->set_filesha512(FileSHA512);
    blockInfo->set_blockmd5(BlockMD5);
    blockInfo->set_savefilename(SaveFileName);
    blockInfo->set_blocknumer(BlockNumer);
    blockInfo->set_cursize(CurSize);
    blockInfo->set_fileblockoffset(FileBlockOffset);

    return true;
}
//
//bool CInterface::DoFile2ClientTable(qiuwanli::File2Cilent* file2Client
//                                    , const std::string& FileName
//                                    , const std::string& FileSHA512
//                                    , const std::string& ClientID
//                                    , const std::string& ClientPort)
//{
//    file2Client->set_filename(FileName);
//    file2Client->set_filesha512(FileSHA512);
//    file2Client->set_cilentid(ClientID);
//    file2Client->set_cilentport(ClientPort);
//
//    return true;
//}

bool CInterface::DoClientConfigFileTable(qiuwanli::ClientConfigFile* conf
                                         , const std::string& CilentID
                                         , const std::string& SaveIP
                                         , const std::string& PriKey
                                         , const std::string& PriKeyMD5
                                         , unsigned long long TotalSize
                                         , unsigned long long RemainSize)
{
    conf->set_cilentid(CilentID);
    conf->set_saveip(SaveIP);
    conf->set_prikey(PriKey);
    conf->set_keymd5(PriKeyMD5);
    conf->set_totalsize(TotalSize);
    conf->set_remainsize(RemainSize);
    conf->set_online("ON");
    return true;
}

bool CInterface::DoFileInfoListTable(qiuwanli::FileInfoList* fileInfo
                                     , const std::string& FileSHA512
                                     , const std::string& FileMD5
                                     , const std::string& FileName
                                     , const std::string& FileCreateDate
                                     , const std::string& isshared
                                     , unsigned long long FileAllBlocks
                                     , unsigned long long FileTotalSize
                                     , const std::string& Tag)
{
    fileInfo->set_filesha512(FileSHA512);
    fileInfo->set_filemd5(FileMD5);
    fileInfo->set_filename(FileName);
    fileInfo->set_filecreatedate(FileCreateDate);
    fileInfo->set_isshared(isshared);
    fileInfo->set_fileallblock(FileAllBlocks);
    fileInfo->set_filetotalsize(FileTotalSize);
    fileInfo->set_tag(Tag);

    return true;
}

bool CInterface::DoUserInfoTable(qiuwanli::UserInfo* user
                                 , const std::string& UserID
                                 , const std::string& UserPS
                                 , const std::vector<std::string>& FileSHA512List)
{
    user->set_userid(UserID);
    user->set_userps(UserPS);
    for (const auto item : FileSHA512List)
    {
        user->add_filesha512(item);
    }

    return true;
}

bool CInterface::DoFileListTable(qiuwanli::FileList* FileList
                                 , const std::string& FileName
                                 , unsigned long long FileSize
                                 , const std::string& FileSHA512)
{
    FileList->set_filename(FileName);
    FileList->set_filesize(FileSize);
    FileList->set_filesha512(FileSHA512);

    return true;
}
//
//bool CInterface::DoFileDownLogTable(qiuwanli::FileDownLog* fileDownLog
//                                    , const std::string& FileName
//                                    , unsigned long long FileSize
//                                    , const std::string& FileDownTime
//                                    , const std::string& DowningStatus
//                                    , const std::string& FileSHA512
//                                    , const std::string& FileMD5)
//{
//    fileDownLog->set_filename(FileName);
//    fileDownLog->set_filesize(FileSize);
//    fileDownLog->set_filedowntime(FileDownTime);
//    fileDownLog->set_downingstatus(DowningStatus);
//    fileDownLog->set_filesha512(FileSHA512);
//    fileDownLog->set_filemd5(FileMD5);
//
//    return true;
//}

bool CInterface::DoFileDowningTable(qiuwanli::FileDowning* fileDowning
                                    , const std::string& FileSHA512
                                    , const std::string& FileMD5
                                    , const std::string& FilePauseTime
                                    , unsigned long long FileDownloadBlockCount
                                    , unsigned long long FileTotalBlockCount
                                    , unsigned long long FileSize
                                    , const std::string& BitMap)
{
    fileDowning->set_filesha512(FileSHA512);
    fileDowning->set_filemd5(FileMD5);
    fileDowning->set_filedowntime(FilePauseTime);
    fileDowning->set_filedownloadblockcount(FileDownloadBlockCount);
    fileDowning->set_filetotalblockcount(FileTotalBlockCount);
    fileDowning->set_filesize(FileSize);
    fileDowning->set_bitmap(BitMap);

    return true;
}

bool CInterface::DoHeart(qiuwanli::Heart* heart
                         , const std::string& ID
                         , unsigned long long RemainSize
                         , unsigned long long TotalSize
                         , const std::string& PriKeyMd5)
{
    heart->set_id(ID);
    //heart->set_remainsize(RemainSize);
    heart->set_totlesize(TotalSize);
    heart->set_prikeymd5(PriKeyMd5);

    return true;
}

bool CInterface::DoSomethingTable(qiuwanli::DoSomething* doit
                                  , const std::string& Key
                                  , const std::string& FileSHA512)
{
    doit->set_key(Key);
    doit->set_filesha512(FileSHA512);

    return true;
}

bool CInterface::DoBlockListForDownCheck(qiuwanli::BlockCheck* check
                                         , const std::string& BlockMD5
                                         , unsigned long long BlockItem)
{
    check->set_blockmd5(BlockMD5);
    check->set_blockitem(BlockItem);

    return true;
}

bool CInterface::DoBlockListForDownCheckTable(
    qiuwanli::BlockListForDownCheck* DownListChcek
    , const std::string& FileSHA512
    , const std::string& SaveServerIP
    , const std::string& FileMd5
    , std::vector<std::pair<std::string, unsigned long long >>& vList)
{
    DownListChcek->set_filesha512(FileSHA512);
    DownListChcek->set_saveserversip(SaveServerIP);
    DownListChcek->set_filemd5(FileMd5);
    for (const auto iter : vList)
    {
        auto itemCheck=DownListChcek->add_blocks( );
        itemCheck->set_blockmd5( iter.first);
        itemCheck->set_blockitem( iter.second);
    }

    return true;
}

bool CInterface::DoSharedTable(qiuwanli::Shared* Url
                               , const std::string& FileSha512
                               , const std::string& VerificationCode
                               , const std::string& SharedTime
                               , unsigned long long FileSize
                               , const std::string& UserStutas
                               , const std::string& UserID)
{
    Url->set_sha512(FileSha512);
    Url->set_verificationcode(VerificationCode);
    Url->set_sharedtime(SharedTime);
    Url->set_filesize(FileSize);
    Url->set_userstutas(UserStutas);
    Url->set_userid(UserID);

    return true;
}

bool CInterface::DoBlockList4DownTable(qiuwanli::BlockList4Down* BlockList
                                       , const std::string& FileSHA512
                                       , unsigned long long BlockNumer
                                       , const std::string& BlockMD5
                                       , const std::string& SaveServersIP)
{
    BlockList->set_filesha512(FileSHA512);
    BlockList->set_blocknumer(BlockNumer);
    BlockList->set_blockmd5(BlockMD5);
    BlockList->set_saveserversip(SaveServersIP);

    return true;
}
