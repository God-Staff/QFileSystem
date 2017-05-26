#pragma once

enum ReposeType
{
    eTypeHeart = 0,
    eTypeInit0,
    eTypeInit1,
    eTypeInit2,
    eTypeDownList,
    eTypeDownFile,
    eTypeList,
    //
    eTypeConfigFile,
    eTypeBlockInfo,
    eTypeBlockInfoTable,
    eTypeFile2Cilent,
    eTypeFile2ClientTable,
    eTypeClientConfigFile,
    eTypeClientConfigFileTable,
    eTypeFileInfoList,
    eTypeFileInfoListTable,
    eTypeUserInfo,
    eTypeUserInfoTable,
    eTypeFileList,
    eTypeFileListTable,
    eTypeFileDownLog,
    eTypeFileDownLogTable,
    eTypeFileDowning,
    eTypeFileDowningTable,
    eTypeDoSomething,
    eTypeDoSomethingTable,
    eTypeBlockCheck,
    eTypeBlockListForDownCheck,
    eTypeBlockListForDownCheckTable,
    eTypeShared,
    eTypeSharedTable,
    eTypeBlockList4Down,
    eTypeBlockList4DownTable
};
class CInterface
{
public:
    template<typename T1, typename T2>
    bool ReadFromFile(T1* FileType, T2 FileName)
    {
        //加载配置文件
        boost::filesystem::fstream OpFile(FileName, std::ios::in | std::ios::binary);
        if (!OpFile)
        {
            g_ComData.opplog.log(FileName + " 打开失败！");
            return;
        }

        if (!FileType.ParseFromIstream(&OpFile))
        {	//打开失败
            OpFile.close( );
            g_ComData.opplog.log("qiuwanli::ConfigFile 解析失败！");
            return;
        }

        OpFile.close( );
    }

    template<typename T1, typename T2>
    bool SaveToFile(T1* FileType, T2 FileName)
    {
        //加载配置文件
        boost::filesystem::fstream OpFile(FileName, std::ios::out | std::ios::binary);
        if (!OpFile)
        {
            g_ComData.opplog.log(FileName + " 打开失败！");
            return;
        }

        if (!FileType.SerializePartialToOstream(&g_ComData.OpFileBlockInfo))
        {	//打开失败
            g_ComData.OpFileBlockInfo.close( );
            g_ComData.opplog.log("qiuwanli::ConfigFile 序列化失败！");
            return;
        }

        OpFile.close( );
    }

    bool DoConfigFile(qiuwanli::ConfigFile* conf
                      , const std::string& ID
                      , const std::string& IP
                      , const std::string& PirKey
                      , const std::string& PirKeyMD5
                      , const unsigned long long TotalSize
                      , const unsigned long long  RemainSize
                      , const std::string& ServersIP
                      , const std::string& ServersPort);

    bool DoBlockInfoTable(qiuwanli::BlockInfo* blockInfo
                          , const std::string& FileSHA512
                          , const std::string& BlockMD5
                          , const std::string& SaveFileName
                          , const unsigned long long BlockNumer
                          , const unsigned long long CurSize
                          , const unsigned long long FileBlockOffset);

    bool DoFile2ClientTable(qiuwanli::File2Cilent* file2Client
                            , const std::string& FileName
                            , const std::string& FileSHA512
                            , const std::string& ClientID
                            , const std::string& ClientPort);

    bool DoClientConfigFileTable(qiuwanli::ClientConfigFile* conf
                                 , const std::string& CilentID
                                 , const std::string& SaveIP
                                 , const std::string& PriKey
                                 , const std::string& PriKeyMD5
                                 , unsigned long long TotalSize
                                 , unsigned long long RemainSIze);

    bool DoFileInfoListTable(qiuwanli::FileInfoList* fileInfo
                             , const std::string& FileSHA512
                             , const std::string& FileMD5
                             , const std::string& FileName
                             , const std::string& FileCreateDate
                             , const std::string& FileChangeDate
                             , unsigned long long FileAllBlocks
                             , unsigned long long FileTotalSize
                             , const std::string& Tag);

    bool DoUserInfoTable(qiuwanli::UserInfo* user
                         , const std::string& UserID
                         , const std::string& UserPS
                         , const std::vector<std::string>& FileSHA512List);

    bool DoFileListTable(qiuwanli::FileList* FileList
                         , const std::string& FileName
                         , unsigned long long FileSize
                         , const std::string& FileSHA512);

    bool DoFileDownLogTable(qiuwanli::FileDownLog* fileDownLog
                            , const std::string& FileName
                            , unsigned long long FileSize
                            , const std::string& FileDownTime
                            , const std::string& DowningStatus
                            , const std::string& FileSHA512
                            , const std::string& FileMD5);

    bool DoFileDowningTable(qiuwanli::FileDowning * fileDowning
                            , const std::string & FileSHA512
                            , const std::string & FileMD5
                            , const std::string & FilePauseTime
                            , unsigned long long FileDownloadBlockCount
                            , unsigned long long FileTotalBlockCount
                            , unsigned long long FileSize
                            , const std::string & BitMap);

    bool DoHeart(qiuwanli::Heart* heart
                 , const std::string& ID
                 , unsigned long long RemainSize
                 , unsigned long long TotalSize
                 , const std::string& PriKeyMd5);

    bool DoSomethingTable(qiuwanli::DoSomething* doit
                          , const std::string& Key
                          , const std::string& FileSHA512);

    bool DoBlockListForDownCheck(qiuwanli::BlockCheck*
                                 , const std::string& BlockMD5
                                 , unsigned long long BlockItem);

    bool DoBlockListForDownCheckTable(qiuwanli::BlockListForDownCheck* DownListChcek
                                      , const std::string& FileSHA512
                                      , const std::string&SaveServerIP
                                      , const std::string& FileMd5
                                      , std::vector<std::pair<std::string, unsigned long long >>& vList);

    bool DoSharedTable(qiuwanli::SharedUrl* Url
                       , const std::string& FileSha512
                       , const std::string& VerificationCode
                       , const std::string& SharedTime
                       , unsigned long long FileSize
                       , const std::string& UserStutas
                       , const std::string& UserID);

    bool DoBlockList4DownTable(qiuwanli::BlockList4Down* BlockList
                               , const std::string& FileSHA512
                               , unsigned long long BlockNumer
                               , const std::string& BlockMD5
                               , const std::string& SaveServersIP);
};

//void MakeLogs (qiuwanli::Logs * Log
//                  , std::string user_id
//                  , std::string logdate
//                  , std::string loginfo
//                  , std::string logtype);