
// TestMFCControlDlg.h : 头文件
//

#pragma once


// CTestMFCControlDlg 对话框
class CTestMFCControlDlg : public CDialogEx
{
// 构造
public:
	CTestMFCControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMFCCONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    virtual void OnTimer(UINT_PTR nIDEvent);

    void OnUpdateUITTT( );
// 实现
protected:
	HICON m_hIcon;



    // 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk( );
    afx_msg void OnBnClickedCancel( );
};
