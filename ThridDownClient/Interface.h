#pragma once
#include <openssl/evp.h>  
#include <boost/filesystem.hpp>
#include "PublicStruct.pb.h"
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

    bool DoFileListTable(qiuwanli::FileList* FileList
                         , const std::string& FileName
                         , unsigned long long FileSize
                         , const std::string& FileSHA512
                         , const std::string& Filestyle);

    bool DoBlockInfoTable(qiuwanli::BlockInfo* blockInfo
                          , const std::string& FileSHA512
                          , const std::string& BlockMD5
                          , const std::string& SaveFileName
                          , const unsigned long long BlockNumer
                          , const unsigned long long CurSize
                          , const unsigned long long FileBlockOffset);

    /*bool DoFile2ClientTable(qiuwanli::File2Cilent* file2Client
                            , const std::string& FileName
                            , const std::string& FileSHA512
                            , const std::string& ClientID
                            , const std::string& ClientPort);*/

    bool DoClientConfigFileTable(qiuwanli::ClientConfigFile* conf
                                 , const std::string& CilentID
                                 , const std::string& SaveIP
                                 , const std::string& PriKey
                                 , const std::string& PriKeyMD5
                                 , unsigned long long TotalSize
                                 , unsigned long long RemainSIze);

    bool DoFileDowningTable(qiuwanli::FileDowning *fileDowning
                            , const std::string& FileSHA512
                            , const std::string& FileMD5
                            , const std::string& FilePauseTime
                            , unsigned long long FileDownloadBlockCount
                            , unsigned long long FileTotalBlockCount
                            , unsigned long long FileSize
                            , const std::string& BitMap);

    bool DoBlockListForDownCheckTable(qiuwanli::BlockListForDownCheck* DownListChcek
                                      , const std::string& FileSHA512
                                      , const std::string&SaveServerIP
                                      , const std::string& FileMd5
                                      , std::vector<std::pair<std::string, unsigned long long >>& vList);

    bool DoSharedTable(qiuwanli::Shared* Url
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

    void GetFileSHA512(std::string& fileName, std::string& FileSHA512)
    {
        EVP_MD_CTX mdctx;
        const EVP_MD *md = NULL;
        char buffer[256];
        char FFFF[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

        unsigned char mdValue[EVP_MAX_MD_SIZE] = {0};
        unsigned int mdLen = 0;

        OpenSSL_add_all_digests( );
        md = EVP_get_digestbyname("sha512");

        EVP_MD_CTX_init(&mdctx);
        EVP_DigestInit_ex(&mdctx, md, NULL);

        boost::filesystem::ifstream inFile(fileName, std::ios::in | std::ios::binary);

        unsigned long endPos = inFile.tellg( );
        inFile.seekg(0, std::ios::beg);
        unsigned long inPos = 0;

        while ((endPos - inPos) > 256)
        {
            inPos = inFile.tellg( );
            inFile.read(buffer, 256);
            EVP_DigestUpdate(&mdctx, buffer, 256);
        }

        inFile.read(buffer, endPos - inPos);
        EVP_DigestUpdate(&mdctx, buffer, endPos - inPos);

        EVP_DigestFinal_ex(&mdctx, mdValue, &mdLen);
        EVP_MD_CTX_cleanup(&mdctx);

        int j = 0;
        for (j = 0; j < mdLen; j++)
        {
            printf("%s", mdValue[j]);
        }

        for (int ii = 0; ii < 64; ++ii)
        {
            int x = mdValue[ii];
            int xx = x & 15;
            int xxx = x & 240;
            xxx = xxx >> 4;
            FileSHA512 + FFFF[xxx] + FFFF[xx];
        }
        //FileSHA512
    }
};

//void MakeLogs (qiuwanli::Logs * Log
//                  , std::string user_id
//                  , std::string logdate
//                  , std::string loginfo
//                  , std::string logtype);