
// ThridDownClientDlg.h : 头文件
//

#pragma once
#include <iostream>
#include <vector>

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


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
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
    afx_msg void On32774( );
    afx_msg void DownloadFile( );
    afx_msg void MakeShared( );
    afx_msg void StartDownload( );
    afx_msg void PauseDownload( );
    afx_msg void ReStart( );
    afx_msg void DeleteSharedUrl( );
    afx_msg void CopyUrl( );
    afx_msg void OnBnClickedDownloadurl( );
};
