
#include <iostream>
#include <boost/filesystem.hpp>

#include "Interface.h"
#include "PublicStruct.pb.h"
#include <string>

CInterface PublicData;

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
    qiuwanli::BlockInfo blocks;

    PublicData.DoBlockInfoTable(&blocks, "Client0001", "127.0.0.1", "qwerqwetqertwetyerthsfgshdfsgdfgs", 12345ull, 123124ull, 34363656ull);

    if (!blocks.SerializeToOstream(&output1))
        std::cerr << "Failed to write Config:" << std::endl;
    output1.close( );
}


void MakeHeartFile( )
{
    boost::filesystem::fstream output1("BlockInfoList", std::ios::out | std::ios::trunc | std::ios::binary);
    qiuwanli::Heart heart;

    PublicData.DoHeart(&heart, "Client0001", 12345ull, 123124ull,"qwerqwetqertwetyerthsfgshdfsgdfgs");

    //std::ostringstream it;
    char* tmpData = new char(heart.ByteSize( ));
    std::string nn;
    heart.SerializeToString(&nn);
    std::string text = stream.str( );
    char* ctext = string.c_str( );
    heart.SerializePartialToOstream()
    if (!heart.SerializeToOstream(&output1))
        std::cerr << "Failed to write Config:" << std::endl;
    output1.close( );
}


//服务端保存用户信息
void ServerUserFiles( )
{
    std::fstream output("ServerUser", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output)
        return;

    qiuwanli::ServerUser user;
    MakeUser(user.add_user( ), "12345", "卦卦", "1234", "1234", "1234", "geren");
    MakeUser(user.add_user( ), "qwer", "qwer", "qwer", "qwer", "qrwe", "");
    MakeUser(user.add_user( ), "asdf", "asdf", "asdf", "asdf", "asdf", "");
    MakeUser(user.add_user( ), "zxcv", "zxcv", "zxcv", "zxcv", "zxcv", "");

    if (!user.SerializeToOstream(&output))
        std::cerr << "Failed to write ServerUser:" << std::endl;
    output.close( );
}
//FileDownLogFile
void FileDownLogFile( )
{
    std::fstream output1("downlog", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output1)
        return;

    qiuwanli::FileDownLogFile downlog;
    MakeDownlog(downlog.add_filelog( ), "123", "1234", "1234", "1234", "1234");
    MakeDownlog(downlog.add_filelog( ), "qwer", "qwer", "qwer", "qwer", "qrwe");
    MakeDownlog(downlog.add_filelog( ), "asdf", "asdf", "asdf", "asdf", "asdf");
    MakeDownlog(downlog.add_filelog( ), "zxcv", "zxcv", "zxcv", "zxcv", "zxcv");

    if (!downlog.SerializeToOstream(&output1))
        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
    output1.close( );
}
//
void MakeFileDowingLog( )
{
    std::fstream output1("downinglog", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output1)
        return;

    qiuwanli::FileDownLogFiles downing;
    MakeFileDowingLog(downing.add_filedownlog( ), "filename1", "1234", "20170203", "wateing", "sha512");
    MakeFileDowingLog(downing.add_filedownlog( ), "filename12", "1234", "20170203", "wite", "sha512");
    MakeFileDowingLog(downing.add_filedownlog( ), "filename13", "1234", "20170203", "downing", "sha512");
    MakeFileDowingLog(downing.add_filedownlog( ), "filename14", "1234", "20170203", "wating", "sha512");
    MakeFileDowingLog(downing.add_filedownlog( ), "filename15", "1234", "20170203", "wating", "sha512");

    if (!downing.SerializeToOstream(&output1))
        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
    output1.close( );
}

void MakePath4FileOrDir( )
{
    std::fstream output1("MakePath4FileOrDir", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output1)
        return;

    qiuwanli::AllFiles allfile;
    allfile.set_pathlist("default");
    MakePath4FileOrDir(allfile.add_filespath( ), "123a", "1234a", 1234lu);
    MakePath4FileOrDir(allfile.add_filespath( ), "123b", "1234b", 1234lu);
    MakePath4FileOrDir(allfile.add_filespath( ), "123c", "1234c", 1234lu);
    MakePath4FileOrDir(allfile.add_filespath( ), "123d", "1234d", 1234lu);

    if (!allfile.SerializeToOstream(&output1))
        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
    output1.close( );
}

void MakeFriendList( )
{
    std::fstream output1("FriendList", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output1)
        return;

    qiuwanli::FriednList friends;
    MakeFriendList(friends.add_friend_( ), "123a", "1234z", "1234");
    MakeFriendList(friends.add_friend_( ), "123s", "1234x", "1234");
    MakeFriendList(friends.add_friend_( ), "123d", "1234c", "1234");
    MakeFriendList(friends.add_friend_( ), "123f", "1234v", "1234");

    if (!friends.SerializeToOstream(&output1))
        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
    output1.close( );
}

void MakeFileSharedLog( )
{
    std::fstream output1("FileSharedLog", std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output1)
        return;

    qiuwanli::FileShared fileshared;
    MakeFileSharedLog(fileshared.add_sharedlog( ), "qwer", "1234", 1234, 1234, "1234");
    MakeFileSharedLog(fileshared.add_sharedlog( ), "qwerasdf", "1234", 1234, 1234, "1234");
    MakeFileSharedLog(fileshared.add_sharedlog( ), "qwetqweqwe", "1234", 1234, 1234, "1234");
    MakeFileSharedLog(fileshared.add_sharedlog( ), "http://www.baidu.com", "1234", 1234, 1234, "1234");
    MakeFileSharedLog(fileshared.add_sharedlog( ), "123", "1234", 1234, 1234, "1234");

    if (!fileshared.SerializeToOstream(&output1))
        std::cerr << "Failed to write FileDownLogFile:" << std::endl;
    output1.close( );
}
//初始化配置文件//
int main( )
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    makesetfiles( );
    FileDownLogFile( );
    ServerUserFiles( );
    MakeFileDowingLog( );
    MakePath4FileOrDir( );
    MakeFriendList( );
    MakeFileSharedLog( );

    google::protobuf::ShutdownProtobufLibrary( );
    return 0;
}