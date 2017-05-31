#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <boost/filesystem.hpp>

#include "Interface.h"
//#include "PublicStruct.pb.h"
#include <string>

CCInterface PublicData;

//生成config配置文件
void makesetfiles( )
{
    boost::filesystem::fstream output1("config", std::ios::out | std::ios::trunc | std::ios::binary);
    qiuwanli::ConfigFile conf;

    PublicData.DoConfigFile(&conf, "Client0001", "127.0.0.1", "qwerqwetqertwetyerthsfgshdfsgdfgs", "qwerqwet", 12345ull, 123124ull, "127.0.0.1", "9999");

    if (!conf.SerializeToOstream(&output1))
        std::cerr << "Failed to write Config:" << std::endl;
    output1.close( );
}



void makeBlockInfoTableFile( )
{
    boost::filesystem::fstream output1("BlockInfoList", std::ios::out | std::ios::trunc | std::ios::binary);
    qiuwanli::BlockInfoTable blockTable;

    PublicData.DoBlockInfoTable(blockTable.add_block( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 1ull, 32768ull, 3453ull);    
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 3ull, 32768ull, 3453ull);    
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 2ull, 32768ull, 3423ull);    
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 4ull, 43243ull, 3403ull);

    PublicData.DoBlockInfoTable(blockTable.add_block( ), "DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6", "DA96E50B0396F8B7269B3DAC406F12E6", "BlockData.db", 1ull, 32768ull, 3803ull); 
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 4ull, 4243ull, 3483ull); 
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 3ull, 32768ull, 3454ull); 
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6DA96E50B0396F8B7269B3DAC406F12E6", "F1C6A45DA26F319A01933A5C7463E85", "BlockData.db", 2ull, 32768ull, 3407ull);
   
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "5D492643DD5DCA59C9E4ABD812CF05FE", "BlockData.db", 1ull, 32768ull, 343ull);
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "5D492643DD5DCA59C9E4ABD812CF05FE", "CBlockData.db", 3ull, 4243ull, 54ull);
    PublicData.DoBlockInfoTable(blockTable.add_block( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "5D492643DD5DCA59C9E4ABD812CF05FE", "BlockData.db", 2ull, 32768ull, 307ull);

    if (!blockTable.SerializePartialToOstream(&output1))
        std::cerr << "Failed to write Config:" << std::endl;
    output1.close( );
}


bool DoBlockList4DownTable(qiuwanli::BlockList4Down* BlockList
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


//服务端保存用户信息
void ServerBlockInfoFiles( )
{
    std::fstream output("ServerBlockInfoFiles", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output)
        return;

    qiuwanli::BlockList4DownTable blocks;
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", 1ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", 2ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", 3ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", 1ull, "F1C6A45DA26F319A01933A5C7463E81", "127.0.0.1");
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", 4ull, "F1C6A45DA26F319A01933A5C7463E82", "127.0.0.1");
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", 3ull, "F1C6A45DA26F319A01933A5C7463E83", "127.0.0.1");
    DoBlockList4DownTable(blocks.add_blocklistfordown( ), "3F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E853F1C6A45DA26F319A01933A5C7463E85", 2ull, "F1C6A45DA26F319A01933A5C7463E84", "127.0.0.1");

    if (!blocks.SerializePartialToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}


bool DoSharedTable(qiuwanli::Shared* Url
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


//服务端保存用户信息
void SharedTableFiles( )
{
    std::fstream output("SharedFiles", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output)
        return;

    qiuwanli::SharedTable blocks;
    DoSharedTable(blocks.add_sharedinfo( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE","345234","", 13452ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");
    DoSharedTable(blocks.add_sharedinfo( ), "4ABD812CF05FE5D4925D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "345234", "", 153653ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");    DoSharedTable(blocks.add_sharedinfo( ), "643DD5DCA59C9E4ABD815D492643DD5DCA59C9E4ABD812CF05FE5D4922CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "353252", "", 123452546ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");    DoSharedTable(blocks.add_sharedinfo( ), "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "324452", "", 43563ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");    DoSharedTable(blocks.add_sharedinfo( ), "9C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA5", "234234", "", 34561ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");    DoSharedTable(blocks.add_sharedinfo( ), "CA59C9E4ABD812CF05FE5D492643D5D492643DD5DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE", "234523", "", 1564462ull, "5D492643DD5DCA59C9E4ABD812CF05FE", "127.0.0.1");

    if (!blocks.SerializePartialToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}
//void MakeHeartFile( )
//{
//    boost::filesystem::fstream output1("BlockInfoList", std::ios::out | std::ios::trunc | std::ios::binary);
//    qiuwanli::Heart heart;
//
//    PublicData.DoHeart(&heart, "Client0001", 12345ull, 123124ull,"qwerqwetqertwetyerthsfgshdfsgdfgs");
//
//    //std::ostringstream it;
//    char* tmpData = new char(heart.ByteSize( ));
//    std::string nn;
//    heart.SerializeToString(&nn);
//    std::string text = stream.str( );
//    char* ctext = string.c_str( );
//    heart.SerializePartialToOstream()
//    if (!heart.SerializeToOstream(&output1))
//        std::cerr << "Failed to write Config:" << std::endl;
//    output1.close( );
//}


//服务端保存用户信息
void ServerUserFiles( )
{
    std::fstream output("ServerUser", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output)
        return;

    qiuwanli::UserInfoTable user;
    PublicData.DoUserInfoTable(user.add_user( ), "100001", "123456", {"123.zip","12345.zip","123456.zip","1234567.zip"});
    PublicData.DoUserInfoTable(user.add_user( ), "100002", "123456", {"123.zip","12345.zip","123456.zip"});
    PublicData.DoUserInfoTable(user.add_user( ), "100003", "123456", {"12345.zip","123456.zip","1234567.zip"});

    if (!user.SerializePartialToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}


void DoClientConfigFile( )
{
    std::fstream output("ClientTable", std::ios::out | std::ios::trunc | std::ios::binary);
    qiuwanli::ClientConfigFileTable clinetTable;

    PublicData.DoClientConfigFileTable(clinetTable.add_clientinfo( ), "Client001", "127.0.0.1", "24352346356345asdfa", "fawefasdfa", 234563ull, 25423ull);
    PublicData.DoClientConfigFileTable(clinetTable.add_clientinfo( ), "Client004", "127.0.0.1", "24352346356345asdfa", "fawefasdfa", 234563ull, 25423ull);
    PublicData.DoClientConfigFileTable(clinetTable.add_clientinfo( ), "Client002", "127.0.0.1", "24352346356345asdfa", "fawefasdfa", 234563ull, 25423ull);
    PublicData.DoClientConfigFileTable(clinetTable.add_clientinfo( ), "Client003", "127.0.0.1", "24352346356345asdfa", "fawefasdfa", 234563ull, 25423ull);
    if (!clinetTable.SerializePartialToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}
//FileDownLogFile
void DoFileListFile( )
{
    std::fstream output("UsFileList", std::ios::out | std::ios::trunc | std::ios::binary);
    qiuwanli::FileListTable listT;

    PublicData.DoFileListTable(listT.add_file( ), "Filename",3456ull , "FileSHA512", "local");
    PublicData.DoFileListTable(listT.add_file( ), "Filename2", 345ull, "FileSHA512", "server");
    PublicData.DoFileListTable(listT.add_file( ), "Filename3",3456ull, "FileSHA512", "local");
    PublicData.DoFileListTable(listT.add_file( ), "Filename4", 3453ull, "FileSHA512", "local");

    if (!listT.SerializePartialToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}

void DoFileInfoListFile( )
{
    std::fstream output("FileInfoList", std::ios::out | std::ios::trunc | std::ios::binary);
    qiuwanli::FileInfoListTable listInfoT;

    PublicData.DoFileInfoListTable(listInfoT.add_filelist( ), "Filesha512", "FileMD5", "FileName", "FileCreatedate", "Filechangdate", 23452, 345, "");
    PublicData.DoFileInfoListTable(listInfoT.add_filelist( ), "Filesha512", "FileMD5", "FileName", "FileCreatedate", "Filechangdate", 23452, 345, "");
    PublicData.DoFileInfoListTable(listInfoT.add_filelist( ), "Filesha512", "FileMD5", "FileName", "FileCreatedate", "Filechangdate", 23452, 345, "");
    PublicData.DoFileInfoListTable(listInfoT.add_filelist( ), "Filesha512", "FileMD5", "FileName", "FileCreatedate", "Filechangdate", 23452, 345, "");


    if (!listInfoT.SerializeToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}
//
//void FileDownLogFile( )
//{
//    std::fstream output1("downlog", std::ios::out | std::ios::trunc | std::ios::binary);
//    if (!output1)
//        return;
//
//    qiuwanli::FileDownLogFile downlog;
//    MakeDownlog(downlog.add_filelog( ), "123", "1234", "1234", "1234", "1234");
//    MakeDownlog(downlog.add_filelog( ), "qwer", "qwer", "qwer", "qwer", "qrwe");
//    MakeDownlog(downlog.add_filelog( ), "asdf", "asdf", "asdf", "asdf", "asdf");
//    MakeDownlog(downlog.add_filelog( ), "zxcv", "zxcv", "zxcv", "zxcv", "zxcv");
//
//    if (!downlog.SerializeToOstream(&output1))
//        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
//    output1.close( );
//}
////
//void MakeFileDowingLog( )
//{
//    std::fstream output1("downinglog", std::ios::out | std::ios::trunc | std::ios::binary);
//    if (!output1)
//        return;
//
//    qiuwanli::FileDownLogFiles downing;
//    MakeFileDowingLog(downing.add_filedownlog( ), "filename1", "1234", "20170203", "wateing", "sha512");
//    MakeFileDowingLog(downing.add_filedownlog( ), "filename12", "1234", "20170203", "wite", "sha512");
//    MakeFileDowingLog(downing.add_filedownlog( ), "filename13", "1234", "20170203", "downing", "sha512");
//    MakeFileDowingLog(downing.add_filedownlog( ), "filename14", "1234", "20170203", "wating", "sha512");
//    MakeFileDowingLog(downing.add_filedownlog( ), "filename15", "1234", "20170203", "wating", "sha512");
//
//    if (!downing.SerializeToOstream(&output1))
//        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
//    output1.close( );
//}
//
//void MakePath4FileOrDir( )
//{
//    std::fstream output1("MakePath4FileOrDir", std::ios::out | std::ios::trunc | std::ios::binary);
//    if (!output1)
//        return;
//
//    qiuwanli::AllFiles allfile;
//    allfile.set_pathlist("default");
//    MakePath4FileOrDir(allfile.add_filespath( ), "123a", "1234a", 1234lu);
//    MakePath4FileOrDir(allfile.add_filespath( ), "123b", "1234b", 1234lu);
//    MakePath4FileOrDir(allfile.add_filespath( ), "123c", "1234c", 1234lu);
//    MakePath4FileOrDir(allfile.add_filespath( ), "123d", "1234d", 1234lu);
//
//    if (!allfile.SerializeToOstream(&output1))
//        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
//    output1.close( );
//}
//
//void MakeFriendList( )
//{
//    std::fstream output1("FriendList", std::ios::out | std::ios::trunc | std::ios::binary);
//    if (!output1)
//        return;
//
//    qiuwanli::FriednList friends;
//    MakeFriendList(friends.add_friend_( ), "123a", "1234z", "1234");
//    MakeFriendList(friends.add_friend_( ), "123s", "1234x", "1234");
//    MakeFriendList(friends.add_friend_( ), "123d", "1234c", "1234");
//    MakeFriendList(friends.add_friend_( ), "123f", "1234v", "1234");
//
//    if (!friends.SerializeToOstream(&output1))
//        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
//    output1.close( );
//}
//
//void MakeFileSharedLog( )
//{
//    std::fstream output1("FileSharedLog", std::ios::out | std::ios::trunc | std::ios::binary);
//    if (!output1)
//        return;
//
//    qiuwanli::FileShared fileshared;
//    MakeFileSharedLog(fileshared.add_sharedlog( ), "qwer", "1234", 1234, 1234, "1234");
//    MakeFileSharedLog(fileshared.add_sharedlog( ), "qwerasdf", "1234", 1234, 1234, "1234");
//    MakeFileSharedLog(fileshared.add_sharedlog( ), "qwetqweqwe", "1234", 1234, 1234, "1234");
//    MakeFileSharedLog(fileshared.add_sharedlog( ), "http://www.baidu.com", "1234", 1234, 1234, "1234");
//    MakeFileSharedLog(fileshared.add_sharedlog( ), "123", "1234", 1234, 1234, "1234");
//
//    if (!fileshared.SerializeToOstream(&output1))
//        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
//    output1.close( );
//}
//初始化配置文件//

#include <vector>
struct ttt
{
    int x1=0;
    int x2=0;
};

int main( )
{

    std::vector<ttt> tt2;
    ttt t1,t2,t3,t4,t5,t6;
    t1.x1 = 23;
    t1.x2 = 123;
    t2.x1 = 234;
    t2.x2 = 34;
    t3.x1 = 123;
    t3.x2 = 456;

    tt2.push_back(t1);
    tt2.push_back(t3);
    tt2.push_back(t2);

    t4.x1 = 123;
    t4.x2 = 456;
    t5.x1 = 123;
    t6.x2 = 456;
    std::vector<ttt> tt1;
    std::vector<ttt> tt3;
    std::vector<ttt> tt4;
    tt1.push_back(t4);
    tt1.push_back(t5);
    tt1.push_back(t6);

    for (auto x : tt1)
    {
        std::cout << x.x1 << " " << x.x2 << std::endl;
    }
    for (auto x : tt2)
    {
        std::cout << x.x1 << " " << x.x2 << std::endl;
    }

    std::cout << std::endl;
    tt1.insert(tt1.begin( ), tt2.begin( ), tt2.end( ));

    for (auto x : tt1)
    {
        std::cout << x.x1 << " " << x.x2 << std::endl;
    }

    std::cout << std::endl;
    tt2.insert(tt2.end( ), tt1.begin( ), tt1.end( ));
    for (auto x : tt2)
    {
        std::cout << x.x1 << " " << x.x2 << std::endl;
    }

    std::cout << std::endl;
    tt3.insert(tt3.end( ), tt1.begin( ), tt1.end( ));
    for (auto x : tt3)
    {
        std::cout << x.x1 << " " << x.x2 << std::endl;
    }
    std::cout << std::endl;
    tt2.insert(tt2.end( ), tt4.begin( ), tt4.end( ));
    for (auto x : tt2)
    {
        std::cout << x.x1 << " " << x.x2 << std::endl;
    }
    //DoFileInfoListFile( );
    //DoFileListFile( );
    //DoClientConfigFile( ); 
    //ServerUserFiles( );
    //makesetfiles( ); 
    //makeBlockInfoTableFile( ); 
    //ServerBlockInfoFiles( );

    //SharedTableFiles( );


    ///*FileDownLogFile( );
    //MakeFileDowingLog( );
    //MakePath4FileOrDir( );
    //MakeFriendList( );
    //MakeFileSharedLog( );*/

    return 0;
}
////////
//////#include <iostream>  
//////#include <openssl/sha.h>   
//////#include <openssl/crypto.h>  // OPENSSL_cleanse
//////#include <boost/filesystem.hpp>
//////
//////const char *orgStr = "helasdfslo"; //待哈希的串  
//////
//////
//////void printHash(unsigned char *md, int len)
//////{
//////    int i = 0;
//////    for (i = 0; i < len; i++)
//////    {
//////        printf("%02x", md[i]);
//////    }
//////
//////    printf("\n");
//////}
//////
//////
//////void myHash1( )
//////{
//////    SHA_CTX c;
//////    unsigned char md[SHA_DIGEST_LENGTH];
//////    SHA1((unsigned char *) orgStr, strlen(orgStr), md);
//////    printHash(md, SHA_DIGEST_LENGTH);
//////
//////    SHA1_Init(&c);
//////    SHA1_Update(&c, orgStr, strlen(orgStr));
//////    SHA1_Final(md, &c);
//////    OPENSSL_cleanse(&c, sizeof(c));
//////    printHash(md, SHA_DIGEST_LENGTH);
//////}
//////
//////void myHash224( )
//////{
//////    SHA256_CTX c;
//////    unsigned char md[SHA224_DIGEST_LENGTH];
//////    SHA224((unsigned char *) orgStr, strlen(orgStr), md);
//////    printHash(md, SHA224_DIGEST_LENGTH);
//////
//////    SHA224_Init(&c);
//////    SHA224_Update(&c, orgStr, strlen(orgStr));
//////    SHA224_Final(md, &c);
//////    OPENSSL_cleanse(&c, sizeof(c));
//////    printHash(md, SHA224_DIGEST_LENGTH);
//////}
//////
//////void myHash256( )
//////{
//////    SHA256_CTX c;
//////    unsigned char md[SHA256_DIGEST_LENGTH];
//////    SHA256((unsigned char *) orgStr, strlen(orgStr), md);
//////    printHash(md, SHA256_DIGEST_LENGTH);
//////
//////    SHA256_Init(&c);
//////    SHA256_Update(&c, orgStr, strlen(orgStr));
//////    SHA256_Final(md, &c);
//////    OPENSSL_cleanse(&c, sizeof(c));
//////    printHash(md, SHA256_DIGEST_LENGTH);
//////}
//////
//////void myHash384( )
//////{
//////    SHA512_CTX c;
//////    unsigned char md[SHA384_DIGEST_LENGTH];
//////    SHA384((unsigned char *) orgStr, strlen(orgStr), md);
//////    printHash(md, SHA384_DIGEST_LENGTH);
//////
//////    SHA384_Init(&c);
//////    SHA384_Update(&c, orgStr, strlen(orgStr));
//////    SHA384_Final(md, &c);
//////    OPENSSL_cleanse(&c, sizeof(c));
//////    printHash(md, SHA384_DIGEST_LENGTH);
//////}
//////
//////void myHash512( )
//////{
//////    SHA512_CTX ccc;
//////    boost::filesystem::fstream opfile("2222.zip", std::ios::in | std::ios::binary);
//////
//////    opfile.seekp(0, std::ios::end);
//////    unsigned long long TxtLength = opfile.tellp( );//获得文件的大小  
//////    opfile.seekp(0, std::ios::beg);
//////
//////    unsigned char * orgS = new unsigned char(TxtLength);
//////    opfile >> orgS;;
//////    unsigned char md[SHA512_DIGEST_LENGTH];
//////    SHA512(orgS, TxtLength, md);
//////
//////    printHash(md, SHA512_DIGEST_LENGTH);
//////
//////    SHA512_Init(&ccc);
//////    SHA512_Update(&ccc, orgStr, strlen(orgStr));
//////    SHA512_Final(md, &ccc);
//////    OPENSSL_cleanse(&ccc, sizeof(ccc));
//////    printHash(md, SHA512_DIGEST_LENGTH);
//////}
//////
//////
//////void myHash5122( )
//////{
//////    SHA512_CTX ccc;
//////    boost::filesystem::fstream opfile("3333.zip", std::ios::in | std::ios::binary);
//////   
//////    opfile.seekp(0,std::ios::end);
//////    unsigned long long TxtLength = opfile.tellp( );//获得文件的大小  
//////    opfile.seekp(0, std::ios::beg);
//////
//////    unsigned char * orgS = new unsigned char(TxtLength);
//////    opfile >> orgS;
//////    unsigned char md[SHA512_DIGEST_LENGTH];
//////    SHA512(orgS, strlen(orgStr), md);
//////
//////    printHash(md, SHA512_DIGEST_LENGTH);
//////
//////    SHA512_Init(&ccc);
//////    SHA512_Update(&ccc, orgStr, strlen(orgStr));
//////    SHA512_Final(md, &ccc);
//////    OPENSSL_cleanse(&ccc, sizeof(ccc));
//////    printHash(md, SHA512_DIGEST_LENGTH);
//////}
//////
//////int main( )
//////{
//////    myHash1( );
//////    myHash224( );
//////    myHash256( );
//////    myHash384( );
//////    myHash512( );
//////    myHash5122( );
//////
//////    return 0;
//////}
////
////// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。  
//////  
///////////////////////////////////SHA-512/////////////////////////////////////////////////////////////////////////  
////
////#include <iostream>  
////#include <fstream>  
////#include <stdio.h>
////
////using namespace std;
////unsigned long long vdata[0x2000000];
//////载入数据、  
////unsigned long long T1, T2, W[80];
//////本轮使用的消息，和分组中保存的消息  
////unsigned long long HashI_1[8];
//////中间结果  
////unsigned long long HashI[8];
//////最终结果  
////const unsigned long long Kt[80] = {//80个常数  
////    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
////    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
////    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
////    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
////    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
////    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
////    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
////    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
////    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
////    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
////    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
////    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
////    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
////    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
////    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
////    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
////    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
////    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
////    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
////    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
////    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
////    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
////    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
////    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
////    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
////    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
////    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL,
////};
////void InitializeHash( )
////{//初始化 HashI_1 HashI  
////    HashI[0] = 0x6a09e667f3bcc908;
////    HashI[1] = 0xbb67ae8584caa73b;
////    HashI[2] = 0x3c6ef372fe94f82b;
////    HashI[3] = 0xa54ff53a5f1d36f1;
////    HashI[4] = 0x510e527fade682d1;
////    HashI[5] = 0x9b05688c2b3e6c1f;
////    HashI[6] = 0x1f83d9abfb41bd6b;
////    HashI[7] = 0x5be0cd19137e2179;
////}
////unsigned long long ROTR(unsigned long long x, int n)//循环右移n位  
////{
////    return ((x >> n) | (x << (64 - n)));
////}
////void SHA_512(int N)
////{
////    int i, t, j;
////    for (i = 0; i < N; i++)
////    {
////        for (j = 0; j < 16; j++)
////            W[j] = vdata[i * 16 + j];//从全部数据中载入本次所需的消息  
////        for (j = 16; j < 80; j++)//将16-79轮的信息计算出  
////            W[j] = (ROTR(W[j - 2], 19) ^ ROTR(W[j - 2], 61) ^ (W[j - 2] >> 6)) + W[j - 7] + (ROTR(W[j - 15], 1) ^ ROTR(W[j - 15], 8) ^ (W[j - 15] >> 7)) + W[j - 16];
////        for (j = 0; j < 8; j++)
////            HashI_1[j] = HashI[j];//每次开始输入之前，将之前得到的输出读入，然后对中间的hashI_1值进行操作，输出给HashI  
////        for (t = 0; t < 80; t++)
////        {//80轮操作  
////            T1 = HashI_1[7] + ((HashI_1[4] & HashI_1[5]) ^ ((~HashI_1[4]) & HashI_1[6]))
////                + (ROTR(HashI_1[4], 14) ^ ROTR(HashI_1[4], 18) ^ ROTR(HashI_1[4], 41)) + W[t] + Kt[t];
////            T2 = (ROTR(HashI_1[0], 28) ^ ROTR(HashI_1[0], 34) ^ ROTR(HashI_1[0], 39))
////                + ((HashI_1[0] & HashI_1[1]) ^ (HashI_1[0] & HashI_1[2]) ^ (HashI_1[1] & HashI_1[2]));
////
////
////            HashI_1[7] = HashI_1[6];
////            HashI_1[6] = HashI_1[5];
////            HashI_1[5] = HashI_1[4];
////            HashI_1[4] = HashI_1[3] + T1;
////            HashI_1[3] = HashI_1[2];
////            HashI_1[2] = HashI_1[1];
////            HashI_1[1] = HashI_1[0];
////            HashI_1[0] = T1 + T2;
////        }
////        for (j = 0; j < 8; j++)
////            HashI[j] += HashI_1[j];//得到输出  
////    }
////}
////int main1(std::string names)
////{
////    int i, t, k, l, j = 0;
////    int N, M;//n个1024,M个256m  
////    unsigned char lastChar[128];
////    unsigned long long TxtLength;
////    unsigned long long r;
////    InitializeHash( );
////    fstream dataf(names, ios::in | ios::binary);
////    dataf.seekp(0, ios::end);
////    TxtLength = dataf.tellp( );//获得文件的大小  
////    dataf.seekp(0, ios::beg);
////    N = 1 << 21;//256m中 含有 1^21个的1024  
////    M = (TxtLength >> 28) + 1;//获得数据有多少个256m的块  
////    for (t = 0; t < M; t++)
////    {
////        if (t == M - 1)
////        {
////            N = (TxtLength - (1 << 28)*(M - 1)) >> 7;//当只剩下最后一组256m时，计算剩下的1024组数-1  
////            for (i = 0; i < N; i++)
////            {//将剩下的满1024的组先读入  
////                dataf.read((char*) lastChar, 128);//一次读取128个char  
////                for (k = 0; k < 16; k++)
////                {
////                    vdata[j] = 0;
////                    for (l = 0; l < 8; l++)
////                        vdata[j] = (vdata[j] << 8) | lastChar[k * 8 + l];
////                    j++;
////                }
////            }
////            N = TxtLength - (1 << 28)*(M - 1) - (N << 7);//计算最后剩下的字节数  
////            for (i = 0; i < N; i++)
////                dataf.read((char*) (&lastChar[i]), 1);
////            if (i >= 112)
////            {//补余时，若最后一段大于896则必须再加一层1024.  
////                lastChar[i++] = 128;//最高位填充1之后填充0   
////                for (; i < 128; i++)
////                    lastChar[i] = 0;
////                for (i = 0; i < 16; i++)
////                {
////                    vdata[j] = 0;
////                    for (k = 0; k < 8; k++)
////                        vdata[j] = (vdata[j] << 8) | lastChar[i * 8 + k];
////                    j++;
////                }
////                for (i = 0; i < 112; i++)//新的1024行要再次填充到896位  
////                    lastChar[i] = 0;
////            }
////            else
////            {
////                lastChar[i++] = 128;//最高位填充1之后填充0   
////                for (; i < 112; i++)
////                    lastChar[i] = 0;
////            }
////            //最后128位为消息长度，第一个数固定为0，第二个数直接为TextLength * 8  
////            //将数据从lastChar数组中读入到data数组中  
////            for (i = 0; i < 14; i++)
////            {
////                vdata[j] = 0;
////                for (k = 0; k < 8; k++)
////                    vdata[j] = (vdata[j] << 8) | lastChar[i * 8 + k];
////                j++;
////            }
////            vdata[j++] = 0;
////            vdata[j++] = TxtLength << 3;
////            N = j >> 4;//j的数量肯定是合理的，则可以由此时j的数量得到最后1024的组数  
////            SHA_512(N);//进行hash  
////        }
////        else
////        {
////            for (i = 0; i < N; i++)
////            {
////                dataf.read((char*) lastChar, 128);
////                for (k = 0; k < 16; k++)
////                {
////                    vdata[j] = 0;
////                    for (l = 0; l < 8; l++)
////                        vdata[j] = (vdata[j] << 8) | lastChar[k * 8 + l];
////                    j++;
////                }
////            }
////            SHA_512(N);//  
////        }
////    }
////    dataf.close( );
////
////    cout << "Hash结果如下" << endl;
////    for (j = 0; j < 8; j++)
////        printf("%016I64x", HashI[j]);
////
////    cin >> lastChar[0];
////    return 0;
////
////}
////
////int main( )
////{
////
////    main1("32.zip" );
////    //main1("3333.zip");
////
////    return 0;
////}
//
//#include <iostream>  
//#include <openssl/evp.h>  
//#include <boost/filesystem.hpp>
//
//using namespace std;
//
//
//int main( )
//{
//    char digestType[ ][20] =
//    {
//        "xxx", // 非法  
//        "md4",
//        "md5",
//        "sha1",
//        "sha256",
//        "sha384",
//        "sha512",
//    };
//    char FFFF[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//
//    int size = sizeof(digestType) / sizeof(digestType[0]);
//    int i = 0;
//    for (i = 0; i < size; i++)
//    {
//        EVP_MD_CTX mdctx;
//        const EVP_MD *md = NULL;
//        char str[ ] = "testOpenssl";
//        char buffer[256];
//
//        unsigned char mdValue[EVP_MAX_MD_SIZE] = {0};
//        unsigned int mdLen = 0;
//
//        OpenSSL_add_all_digests( );
//        md = EVP_get_digestbyname(digestType[i]);
//        if (!md) // 不支持的格式  
//        {
//            printf("Unknown message digest %s\n", digestType[i]);
//            continue;
//        }
//
//        EVP_MD_CTX_init(&mdctx);
//        EVP_DigestInit_ex(&mdctx, md, NULL);
//
//        boost::filesystem::ifstream inFile("2.zip", std::ios::in | std::ios::binary | std::ios::ate);
//
//        unsigned long endPos = inFile.tellg( );
//        inFile.seekg(0, ios::beg);
//        unsigned long inPos = 0;
//
//        while ((endPos - inPos)>256)
//        {
//            inPos = inFile.tellg( );
//            inFile.read(buffer, 256);
//            EVP_DigestUpdate(&mdctx, buffer, 256);
//        }
//
//        inFile.read(buffer, endPos - inPos);
//        EVP_DigestUpdate(&mdctx, buffer, endPos - inPos);
//
//
//        EVP_DigestFinal_ex(&mdctx, mdValue, &mdLen);
//        EVP_MD_CTX_cleanup(&mdctx);
//
//        printf("%s is ", digestType[i]);
//        int j = 0;
//        for (j = 0; j < mdLen; j++)
//        {
//            printf("%02X", mdValue[j]);
//        }
//        std::cout << std::endl;
//        for (int ii=0;ii<64;++ii)
//        {
//            int x=mdValue[ii];
//            int xx = x & 15;
//            int xxx = x & 240;
//            xxx = xxx >> 4;
//            std::cout << FFFF[xxx ]<< FFFF[xx];
//        }
//
//        printf("\n");
//    }
//
//    return 0;
//}
////
////bool DoConfigFile(qiuwanli::ConfigFile* conf
////                              , const std::string& ID
////                              , const std::string& IP
////                              , const std::string& PriKey
////                              , const std::string& PriKeyMD5
////                              , const unsigned long long TotalSize
////                              , const unsigned long long RemainSize
////                              , const std::string& ServersIP
////                              , const std::string& ServersPort)
////{
////    conf->set_id(ID);
////    conf->set_ip(IP);
////    conf->set_prikey(PriKey);
////    conf->set_prikeymd5(PriKeyMD5);
////    conf->set_totalsize(TotalSize);
////    conf->set_remainsize(RemainSize);
////    conf->set_serversip(ServersIP);
////    conf->set_serversport(ServersPort);
////
////    return 0;
////
////}
////
////
////bool DoBlockInfoTable(qiuwanli::BlockInfo* blockInfo
////                                  , const std::string& FileSHA512
////                                  , const std::string& BlockMD5
////                                  , const std::string& SaveFileName
////                                  , const unsigned long long BlockNumer
////                                  , const unsigned long long CurSize
////                                  , const unsigned long long FileBlockOffset)
////{
////    blockInfo->set_filesha512(FileSHA512);
////    blockInfo->set_blockmd5(BlockMD5);
////    blockInfo->set_savefilename(SaveFileName);
////    blockInfo->set_blocknumer(BlockNumer);
////    blockInfo->set_cursize(CurSize);
////    blockInfo->set_fileblockoffset(FileBlockOffset);
////    return true;
////}
////
////int main( )
////{
////    boost::filesystem::fstream output1("config", std::ios::out | std::ios::binary);
////    qiuwanli::ConfigFile blocks;
////    qiuwanli::BlockInfoTable TT;
////    blocks.Clear( );
////
////    DoConfigFile(&blocks, "Client0001", "127.0.0.1", "zxcvbnmasdfghjklqwertyuiop", "qwerweqtqwrew", 1234534ull, 123124ull, "127.0.0.1", "8089");
////    DoBlockInfoTable(TT.add_block( ), "weqrwer", "fgsdfer", "tyrtsfgasegrrt", 2435234, 32452, 3463);
////    DoBlockInfoTable(TT.add_block( ), "sdfsdf", "fgsdfdsfgseer", "tyrtsrt", 2435234, 32452, 3463);
////    DoBlockInfoTable(TT.add_block( ), "dsfgsdfse", "fgsdfer", "tyfgsertsrt", 2435234, 32452, 3463);
////
////    //blocks.set_id("Client0001");
////    //blocks.set_ip("Client0001");
////    //blocks.set_prikey("Client0001");
////    //blocks.set_prikeymd5("Client0001");
////    //blocks.set_totalsize(1234);
////    //blocks.set_remainsize(23235);
////    //blocks.set_serversip("Client0001");
////    //blocks.set_serversport("ServersPort");
////
////    //std::cout<<blocks.serversip( );
////    if (!blocks.SerializeToOstream(&output1))
////        std::cerr << "Failed to write Config:" << std::endl;
////    output1.close( );
////
////    return 0;
////}


//#include <iostream>
//#include <iomanip>
//#include <ctime>
//#include <stdio.h>
//
//int main( )
//{
//    char mbstr[100];
//
//    time_t now;
//    time(&now);
//
//    // 定义两个变量，存储转换结果
//    struct tm tmTmp;
//
//    // 转换为tm结构
//    localtime_s(&tmTmp, &now);
//
//
//    if (std::strftime(mbstr, 100, "%F %T", &tmTmp))
//    {
//        std::cout << mbstr << std::endl;
//    }
//
//
//    return 0;
//}