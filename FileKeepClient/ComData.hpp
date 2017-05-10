#ifndef COM_DATA_HPP
#define COM_DATA_HPP

#include "FileDataStruct.pb.h"
#include "SendFile.hpp"
#include "public.h"
#include "OptLog.hpp"

qiuwanli::ConfigFile Conf;
qiuwanli::BlockInfoTable BlockTable;
qiuwanli::BlockInfoTable BlockTableDiff;
qiuwanli::BlockInfoTable BlockTablePreDiff;
OptLog opplog;

#endif // !COM_DATA_HPP
