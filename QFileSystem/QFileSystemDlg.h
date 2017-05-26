
// QFileSystemDlg.h : 头文件
//

#pragma once
//#include "constdate.hpp"
#include "public.h"
#include "DataStruct.pb.h"

// CQFileSystemDlg 对话框
class CQFileSystemDlg : public CDialogEx
{
// 构造
public:
	CQFileSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QFILESYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//DECLARE_DHTML_EVENT_MAP()

    CListCtrl*  m_ListFile;
    CListCtrl*  m_ListSaveServer;
    CListCtrl*  m_ListShared;
    CListCtrl*  m_ListClient;
    CListCtrl*  m_ListLogs;

	void updateList ();
	//void MakeFilesLog (qiuwanli::File2Cilent * file
 //                      , std::string filename
 //                      , std::string sha512
 //                      , std::string client
 //                      , std::string createtime);
	//void MakeLogs (qiuwanli::Logs * Log
 //                  , std::string user_id
 //                  , std::string logdate
 //                  , std::string loginfo
 //                  , std::string logtype);
	//void MakeLogs (qiuwanli::ID2IP * id2ip
 //                  , std::string clientid
 //                  , std::string id
 //                  , std::string Prikey
 //                  , std::string KeyMd5
 //                  , std::string Others);

	//发送发文件
    void sender(boost::asio::io_service &io
                , const char*	ip_address
                , unsigned	port
                , const char* filename
                , const char* msg_type);
	//核对是否存在
	bool CQFileSystemDlg::checkItem (std::string item);

	std::wstring StringToWstring (const std::string& str)
	{
		int size_needed = MultiByteToWideChar (CP_UTF8, 0, &str[0]
                                               , (int)str.size (), NULL, 0);
		std::wstring wstrTo (size_needed, 0);
		MultiByteToWideChar (CP_UTF8, 0, &str[0], (int)str.size ()
                             , &wstrTo[0], size_needed);
		return wstrTo;
	}

	/*std::string& WStringToUTF8String (const std::wstring &wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::string u8str = "";
		u8str = conv.to_bytes (wstr);
		return std::move (u8str);
	}*/

public:
	afx_msg void OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRClickClientList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRClickSharedList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedend( );
    afx_msg void OnBnClickedStart( );
        

    
    //数据的初始化
    void InitData( );
    void LoadFileList( );
    void LoadSaveServerList( );
    void LoadSharedList( );
    void LoadClientList( );
    void LoadLogsList( );

public:
    qiuwanli::FileInfoList      m_CFileList;
    qiuwanli::File2ClientTable  m_CFileToClient;
    qiuwanli::SharedTable       m_CSharedTable;
    qiuwanli::UserInfoTable     m_CUserInfo;

    qiuwanli::HeartTable        m_CHeart;
    qiuwanli::BlockListDownTable     m_CBlockDownList;
};

