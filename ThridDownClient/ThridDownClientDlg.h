
// ThridDownClientDlg.h : 头文件
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include "PublicStruct.pb.h"

// CThridDownClientDlg 对话框
class CThridDownClientDlg : public CDialogEx
{
// 构造
public:
	CThridDownClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRIDDOWNCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


    virtual void OnTimer(UINT_PTR nIDEvent);
    // 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
    void runRecive( );
    afx_msg void OnPaint( );
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnNMRClickListFile(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRClickListFordownMenu(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRClickListSharedMenu(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk( );
    afx_msg void OnBnClickedCancel( );

    void FillData( );

    CListCtrl* m_FileList;
    CListCtrl* m_DownList;
    CListCtrl* m_SharedList;
    afx_msg void DeleteFile( );
    afx_msg void OnUploadFile( );
    afx_msg void DownloadFile( );
    afx_msg void MakeShared( );
    afx_msg void StartDownload( );
    afx_msg void PauseDownload( );
    afx_msg void ReStart( );
    afx_msg void DeleteSharedUrl( );
    afx_msg void CopyUrl( );
    afx_msg void OnBnClickedDownloadurl( );

    void GetFileSHA512(std::string& fileName, std::string& FileSHA512);
    void UpDataUI( );
    VOID CALLBACK CountTime( );
private:
    std::vector<std::string> m_VUpFileList;
    qiuwanli::UserInfo user;
};
