#ifndef SERIALIZATION_TO_STREAM_HPP
#define SERIALIZATION_TO_STREAM_HPP
#include "FileDataStruct.pb.h"

class SerialToStream
{
public:
    SerialToStream( ) { }
    ~SerialToStream( ) { }
private:

    void MakeSerailToStream(qiuwanli::ConfigFile& Config
                            , const std::string& SaveClientID
                            , const std::string& SaveClientIP
                            , const std::string& Prikey
                            , const std::string& PriKeyMD5
                            , const unsigned long TotalSize
                            , const unsigned long RemainSize);

    void MakeBlockInfoTable(qiuwanli::BlockInfoTable& BlockTable
                            , const std::string& FileSHA512
                            , const std::string& BlockMD5
                            , const std::string& SaveFileName
                            , const unsigned long BlockNum
                            , const unsigned long CurSize
                            , const unsigned longFileBlockOffSet);

    void MakeLogTable(qiuwanli::LogTable& Log
                        , const std::string UserID
                        , const std::string& LogInfo);


};




#endif //! SERIALIZATION_TO_STREAM_HPP

#ifndef PROTOBUF_INTERFACE_HPP
#define PROTOBUF_INTERFACE_HPP

#include "MyDataStruct.pb.h"
#include "config.pb.h"

namespace qiuwanli
{

class BufInterface
{
public:
    BufInterface( );
    ~BufInterface( );

    /**
    * 配置文件记录
    * @param conf  配置文件对象
    * @param type  配置类型
    * @param value 对应的值
    */
    void MakeConfigFile(qiuwanli::Config* conf, qiuwanli::Type type, std::string value);

    /**
    * 文件正在下载的记录
    * @param downingLog    文件下载记录对象
    * @param filename      文件名
    * @param filesize      文件大小
    * @param downtime      下载时间
    * @param downingStatus 下载状态
    * @param fileSha512    文件对应的SHA512
    */
    void MakeFileDowingLog(qiuwanli::FileDowningLog* downingLog, std::string filename,
                           std::string filesize, std::string downtime, std::string downingStatus, std::string fileSha512);
    /**
    * 文件下载记录的封装
    * @param log      下载记录对像
    * @param filename 文件名称
    * @param path     下载路径
    * @param size     文件大小
    * @param downtime 完成下载的时间
    * @param status   现在是否存在
    */
    void MakeDownlog(qiuwanli::FileDownLog *log, std::string filename, std::string path,
                     std::string size, std::string downtime, std::string status);

    /**
    * 用户对象的封装
    * @param user 用户对象
    * @param id   用户ID
    * @param name 用户昵称
    * @param ps   密码对应的MD5
    * @param uuid PC端UUID
    * @param code 登陆验证码
    * @param type 登陆类型
    */
    void MakeUser(qiuwanli::Users* user, std::string id, std::string name, std::string ps,
                  std::string uuid, std::string code, std::string type);

    /**
    * 用于实名认证
    * @param realuser
    * @param id         用户ID
    * @param realname   真实姓名
    * @param idcard     身份证号码
    * @param liveadress 现居住地址
    * @param phone      手机号码
    * @param qq         QQ号码
    * @param mirchat    微信账号
    * @param email      邮箱
    * @param bri        生日
    * @param adress     籍贯
    */
    void MakeRealUsers(qiuwanli::RealUsers* realuser, std::string id, std::string realname
                       , std::string idcard, std::string liveadress, std::string phone, std::string qq
                       , std::string mirchat, std::string email, std::string bri, std::string adress);

    /**
    * 用户注册信息
    * @param reg   [description]
    * @param name  用户名
    * @param ps    密码MD5
    * @param phone 手机号码
    * @param code  手机验证码
    */
    void MakeUserLogin(qiuwanli::userreg* reg, std::string name, std::string ps, std::string phone, std::string code);

    /**
    * 文件分享记录
    * @param Shared     分享记录对象
    * @param url        分享链接的URL
    * @param ps         分享密码，默认为空
    * @param time       创建分享的时间
    * @param timelength 分享时间长度
    * @param type       分享类型
    */
    void MakeFileSharedLog(qiuwanli::FilesSharedLog*  Shared, std::string url, std::string ps/*=""*/,
                           unsigned long long time, unsigned long timelength, std::string type);

    /**
    * 好友列表
    * @param friends  好友对象
    * @param friendid 好友ID
    * @param name     好友备注
    * @param status   好友现在状态
    */
    void MakeFriendList(qiuwanli::MyFriend* friends, std::string friendid, std::string name, std::string status);

    /**
    * 文件目录，用于创建文件目录和下载文件到对应Path
    * @param filepath
    * @param pathname 路径或文件名称
    * @param pathdeep 路径所属深度
    * @param fileid   路径或文件对应的ID
    */
    void BufInterface::MakePath4FileOrDir(qiuwanli::Path4FilesOrDir* filepath, std::string pathname,
                                          std::string pathdeep, unsigned long fileid);

    /**
    * 文件详细信息+验证
    * @param Info
    * @param filename   文件名
    * @param size       大小
    * @param md51       md5_1
    * @param md52       MD5_2
    * @param creatime   文件创建时间
    * @param filepathid 路径对应的ID
    */
    void MakeFileInfo(qiuwanli::FileInfo* Info, std::string filename, unsigned long size, std::string md51,
                      std::string md52, unsigned long long  creatime, unsigned long filepathid);

    //一开始先保存在内存，定时（或文件下载完成时）将其写入到磁盘
    //若客户端结束时，在析构是将其写入磁盘，便于下次使用
    //断点续传，文件下载记录
    /**
    * 文件下载状态，BitMap存储
    * @param tmpfile
    * @param filename 临时文件名称
    * @param crc      对应文件CRC验证序列
    * @param BitMap   文件块对应的位图信息
    * @param Status   文件下载状态，0000 0000 0000 0000 前四位代表下载状态信息，剩余12位用作下载文件的百分比
    * @param blocknum 文件总块数
    */
    void MakeFileTemp(qiuwanli::filetemp* tmpfile, std::string filename, std::string crc,
                      std::string BitMap, std::string Status, unsigned long blocknum);

};

}
#endif // !PROTOBUF_INTERFACE_HPP
