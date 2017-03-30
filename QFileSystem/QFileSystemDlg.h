
// QFileSystemDlg.h : 头文件
//

#pragma once
#include "date.pb.h"

// CQFileSystemDlg 对话框
class CQFileSystemDlg : public CDHtmlDialog
{
// 构造
public:
	CQFileSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QFILESYSTEM_DIALOG, IDH = IDR_HTML_QFILESYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

	CListCtrl*  m_ListControl = nullptr;
	CListCtrl*  m_ListContro2 = nullptr;
	void updateList ();
	void MakeFilesLog (qiuwanli::File2Cilent * downingLog, std::string filename,
		std::string sha512, std::string client, std::string createtime)
};
