
// TestMFCControlDlg.h : ͷ�ļ�
//

#pragma once


// CTestMFCControlDlg �Ի���
class CTestMFCControlDlg : public CDialogEx
{
// ����
public:
	CTestMFCControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMFCCONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

    virtual void OnTimer(UINT_PTR nIDEvent);

    void OnUpdateUITTT( );
// ʵ��
protected:
	HICON m_hIcon;



    // ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk( );
    afx_msg void OnBnClickedCancel( );
};