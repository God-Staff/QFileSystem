#include "SerializationToStream.hpp"

void SerialToStream::MakeSerailToStream(qiuwanli::ConfigFile& Config
                                        , const std::string& SaveClientID
                                        , const std::string& SaveClientIP
                                        , const std::string& Prikey
                                        , const std::string& PriKeyMD5
                                        , const unsigned long TotalSize
                                        , const unsigned long RemainSize)
{ 

}

void SerialToStream::MakeBlockInfoTable(qiuwanli::BlockInfoTable& BlockTable
                                        , const std::string& FileSHA512
                                        , const std::string& BlockMD5
                                        , const std::string& SaveFileName
                                        , const unsigned long BlockNum
                                        , const unsigned long CurSize
                                        , const unsigned longFileBlockOffSet)
{ 

}


#include "ProtoInterface.hpp"
namespace qiuwanli
{

BufInterface::BufInterface( )
{ }

BufInterface::~BufInterface( )
{ }

void BufInterface::MakeConfigFile(qiuwanli::Config* conf, qiuwanli::Type type, std::string value)
{
    conf->set_type(type);
    conf->set_valuestring(value);
}

void BufInterface::MakeFileDowingLog(qiuwanli::FileDowningLog* downingLog, std::string filename,
                                     std::string filesize, std::string downtime, std::string downingStatus, std::string fileSha512)
{
    downingLog->set_filename(filename);
    downingLog->set_filesize(filesize);
    downingLog->set_downtime(downtime);
    downingLog->set_downingstatus(downingStatus);
    downingLog->set_filesha512(fileSha512);
}

void BufInterface::MakeDownlog(qiuwanli::FileDownLog *log, std::string filename, std::string path,
                               std::string size, std::string downtime, std::string status)
{
    log->set_filename(filename);
    log->set_filepath(path);
    log->set_filesize(size);
    log->set_downtime(downtime);
    log->set_status(status);
}

void BufInterface::MakeUser(qiuwanli::Users* user, std::string id, std::string name, std::string ps,
                            std::string uuid, std::string code, std::string type)
{
    user->set_user_id(id);
    user->set_user_name(name);
    user->set_user_password_md5(ps);
    user->set_user_client_uuid(uuid);
    user->set_login_code(code);
    user->set_user_type(type);
}

void BufInterface::MakeRealUsers(qiuwanli::RealUsers* realuser, std::string id, std::string realname
                                 , std::string idcard, std::string liveadress, std::string phone, std::string qq
                                 , std::string mirchat, std::string email, std::string bri, std::string adress)
{
    realuser->set_user_id(id);
    realuser->set_user_name_real(realname);
    realuser->set_user_idcard(idcard);
    realuser->set_user_live_address(liveadress);
    realuser->set_user_phone(phone);
    realuser->set_user_mirchat(mirchat);
    realuser->set_user_email(email);
    realuser->set_user_bri(bri);
    realuser->set_user_address(adress);
}

void BufInterface::MakeUserLogin(qiuwanli::userreg* reg, std::string name, std::string ps, std::string phone, std::string code)
{
    reg->set_user_name(name);
    reg->set_user_password_md5(ps);
    reg->set_user_phone(phone);
    reg->set_reg_code(code);
}

void BufInterface::MakeFileSharedLog(qiuwanli::FilesSharedLog*  Shared, std::string url, std::string ps/*=""*/,
                                     unsigned long long time, unsigned long timelength, std::string type)
{
    Shared->set_shared_url(url);
    Shared->set_shared_ps(ps);
    Shared->set_shared_time(time);
    Shared->set_shared_timelenth(timelength);
    Shared->set_shared_types(type);
}

void BufInterface::MakeFriendList(qiuwanli::MyFriend* friends, std::string friendid, std::string name, std::string status)
{
    friends->set_friendid(friendid);
    friends->set_name(name);
    friends->set_status(status);
}

void BufInterface::MakePath4FileOrDir(qiuwanli::Path4FilesOrDir* filepath, std::string pathname,
                                      std::string pathdeep, unsigned long fileid)
{
    filepath->set_pathorname(pathname);
    filepath->set_pathtypes(pathdeep);
    filepath->set_fileid(fileid);
}

void BufInterface::MakeFileInfo(qiuwanli::FileInfo* Info, std::string filename, unsigned long size, std::string md51,
                                std::string md52, unsigned long long  creatime, unsigned long filepathid)
{
    Info->set_fileid(filename);
    Info->set_filesize(size);
    Info->set_filemd5_1(md51);
    Info->set_filemd5_2(md52);
    Info->set_filecreatedate(creatime);
    Info->set_filepathid(filepathid);
}

void BufInterface::MakeFileTemp(qiuwanli::filetemp* tmpfile, std::string filename, std::string crc,
                                std::string BitMap, std::string Status, unsigned long blocknum)
{
    qiuwanli::FileBlockInfo ttt;
    ttt.set_file_id(filename);
    ttt.set_fileblock_num(blocknum);
    ttt.set_file_crc(crc);
    tmpfile->set_bitmap(BitMap);
    tmpfile->set_nowstatus(Status);
    tmpfile->set_allocated_fileinfo(&ttt);

}

} //End namespace qiuwanli
