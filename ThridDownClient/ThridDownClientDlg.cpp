
// ThridDownClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "ThridDownClient.h"
#include "ThridDownClientDlg.h"

#include "SendFile.hpp"
#include "Servers.hpp"

#include <openssl/evp.h>  
#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <boost/asio.hpp>
#include "public.h"

// CThridDownClientDlg 对话框
ComData g_ComData;

CThridDownClientDlg::CThridDownClientDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_THRIDDOWNCLIENT_DIALOG, pParent)
    //, m_DownList(nullptr)
    //, m_FileList(nullptr)
    //, m_SharedList(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThridDownClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThridDownClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &CThridDownClientDlg::OnNMRClickListFile)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_ForDown, &CThridDownClientDlg::OnNMRClickListFordownMenu)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_SHARED, &CThridDownClientDlg::OnNMRClickListSharedMenu)
    ON_BN_CLICKED(IDOK, &CThridDownClientDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CThridDownClientDlg::OnBnClickedCancel)
    ON_COMMAND(ID_32773, &CThridDownClientDlg::DeleteFile)
    ON_COMMAND(ID_32774, &CThridDownClientDlg::OnUploadFile)
    ON_COMMAND(ID_32775, &CThridDownClientDlg::DownloadFile)
    ON_COMMAND(ID_32776, &CThridDownClientDlg::MakeShared)
    ON_COMMAND(ID_32779, &CThridDownClientDlg::StartDownload)
    ON_COMMAND(ID_32778, &CThridDownClientDlg::PauseDownload)
    ON_COMMAND(ID_32780, &CThridDownClientDlg::ReStart)
    ON_COMMAND(ID_32771, &CThridDownClientDlg::DeleteSharedUrl)
    ON_COMMAND(ID_32772, &CThridDownClientDlg::CopyUrl)
    ON_BN_CLICKED(IDC_DownLoadURL, &CThridDownClientDlg::OnBnClickedDownloadurl)
END_MESSAGE_MAP()

void CThridDownClientDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case 1:
              
        break;
    case 2:
        //在这添加执行定时器2的任务； 
        break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}

// CThridDownClientDlg 消息处理程序

BOOL CThridDownClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_FileList = (CListCtrl*) GetDlgItem(IDC_LIST_FILE);
    DWORD dwStyle1 = GetWindowLong(m_FileList->m_hWnd, GWL_STYLE);
    SetWindowLong(m_FileList->m_hWnd, GWL_STYLE, dwStyle1 | LVS_REPORT);

    //设置m_FileList可以整行选择和网格条纹
    DWORD styles1 = m_FileList->GetExtendedStyle( );
    m_FileList->SetExtendedStyle(styles1 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    //给m_FileList设置5个标题栏
    TCHAR rgtsz1[4][10] = {_T("文件名"),_T("大小"),_T("类型"),_T("文件状态")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn1;
    CRect rect1;
    m_FileList->GetWindowRect(&rect1);
    for (int i = 0; i < 4; i++)
    {
        lvcolumn1.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn1.fmt = LVCFMT_LEFT;
        lvcolumn1.pszText = rgtsz1[i];
        lvcolumn1.iSubItem = i;
        lvcolumn1.iOrder = i;
        lvcolumn1.cx = rect1.Width( ) / 4;
        m_FileList->InsertColumn(i, &lvcolumn1);
    }

    //! 
    m_DownList = (CListCtrl*) GetDlgItem(IDC_LIST_ForDown);
    DWORD dwStyle2 = GetWindowLong(m_DownList->m_hWnd, GWL_STYLE);
    SetWindowLong(m_DownList->m_hWnd, GWL_STYLE, dwStyle2 | LVS_REPORT);

    //设置m_DownList可以整行选择和网格条纹
    DWORD styles2 = m_DownList->GetExtendedStyle( );
    m_DownList->SetExtendedStyle(styles2 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    //给m_DownList设置4个标题栏
    TCHAR rgtsz2[4][10] = {_T("文件名"),_T("文件大小"),_T("下载时间"),_T("下载状态")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn2;
    CRect rect2;
    m_DownList->GetWindowRect(&rect2);
    for (int i = 0; i < 4; i++)
    {
        lvcolumn2.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn2.fmt = LVCFMT_LEFT;
        lvcolumn2.pszText = rgtsz2[i];
        lvcolumn2.iSubItem = i;
        lvcolumn2.iOrder = i;
        lvcolumn2.cx = rect2.Width( ) / 4;
        m_DownList->InsertColumn(i, &lvcolumn2);
    }


    m_SharedList = (CListCtrl*) GetDlgItem(IDC_LIST_SHARED);
    DWORD dwStyle3 = GetWindowLong(m_SharedList->m_hWnd, GWL_STYLE);
    SetWindowLong(m_SharedList->m_hWnd, GWL_STYLE, dwStyle3 | LVS_REPORT);

    //设置m_SharedList可以整行选择和网格条纹
    DWORD styles3 = m_SharedList->GetExtendedStyle( );
    m_SharedList->SetExtendedStyle(styles3 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    //给m_SharedList设置5个标题栏
    TCHAR rgtsz3[5][10] = {_T("分享链接"),_T("文件名"),_T("文件大小"),_T("验证信息"),_T("创建时间")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn3;
    CRect rect3;
    m_SharedList->GetWindowRect(&rect3);
    for (int i = 0; i < 5; i++)
    {
        lvcolumn3.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn3.fmt = LVCFMT_LEFT;
        lvcolumn3.pszText = rgtsz3[i];
        lvcolumn3.iSubItem = i;
        lvcolumn3.iOrder = i;
        lvcolumn3.cx = rect3.Width( ) / 5;
        m_SharedList->InsertColumn(i, &lvcolumn3);
    }


    FillData( );


    runRecive( );
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void runservers()
{
    try{
        std::cerr << "Usage: server <address> <port> <threads> <blocksize>\n";

        boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
        short port = 8889;
        int thread_count = 4;
        size_t block_size = 4096;

        boost::asio::io_service ios;
        server ssss(ios, boost::asio::ip::tcp::endpoint(address, port));
        std::list<boost::thread*> threads;

        while (--thread_count > 0)
        {
            boost::thread* new_thread = new boost::thread(
                boost::bind(&boost::asio::io_service::run, &ios));
            threads.push_back(new_thread);
        }

        ios.run( );

        while (!threads.empty( ))
        {
            threads.front( )->join( );

            delete threads.front( );

            threads.pop_front( );
        }
    } catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what( ) << "\n";
    }
}

void CThridDownClientDlg::runRecive( )
{
    boost::thread sr(runservers);
    sr.detach( );
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CThridDownClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CThridDownClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//文件列表右键菜单响应事件
void CThridDownClientDlg::OnNMRClickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    std::vector<CString> kSelectedItem; //多个选中项。
    CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
    menu.LoadMenu(IDR_MENU1); //装载自定义的右键菜单
    pSubMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
    CPoint oPoint; //定义一个用于确定光标位置的位置
    GetCursorPos(&oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标

    m_FileList->SetExtendedStyle(m_FileList->GetExtendedStyle( ) | LVS_EX_FULLROWSELECT);

    POSITION sSelectedPos = m_FileList->GetFirstSelectedItemPosition( );  //获取首选中行位置
    if (sSelectedPos == NULL)
    {
        return;
    }
    //选中的数据
    //选中的1个或多个数据
    while (sSelectedPos)//获取首选中行位置
    {
        int nSelItem = -1;
        nSelItem = m_FileList->GetNextSelectedItem(sSelectedPos);//获取选中行的索引，通过POSITION转化 

        if (nSelItem >= 0 && nSelItem < m_FileList->GetItemCount( ))
        {
            CString sFullPath = m_FileList->GetItemText(nSelItem, 1);
            //MessageBox(sFullPath ); //显示当前选中的路径
            kSelectedItem.push_back(sFullPath);
        }

    }

    //在指定位置显示弹出菜单
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

    *pResult = 0;
}


//下载列表右键菜单响应事件
void CThridDownClientDlg::OnNMRClickListFordownMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    std::vector<CString> kSelectedItem; //多个选中项。
    CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
    menu.LoadMenu(IDR_MENU1); //装载自定义的右键菜单
    pSubMenu = menu.GetSubMenu(1); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
    CPoint oPoint; //定义一个用于确定光标位置的位置
    GetCursorPos(&oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标

    m_SharedList->SetExtendedStyle(m_SharedList->GetExtendedStyle( ) | LVS_EX_FULLROWSELECT);

    POSITION sSelectedPos = m_SharedList->GetFirstSelectedItemPosition( );  //获取首选中行位置
    if (sSelectedPos == NULL)
    {
        return;
    }
    //选中的数据
    //选中的1个或多个数据
    while (sSelectedPos)//获取首选中行位置
    {
        int nSelItem = -1;
        nSelItem = m_SharedList->GetNextSelectedItem(sSelectedPos);//获取选中行的索引，通过POSITION转化 

        if (nSelItem >= 0 && nSelItem < m_SharedList->GetItemCount( ))
        {
            CString sFullPath = m_SharedList->GetItemText(nSelItem, 1);
            //MessageBox(sFullPath ); //显示当前选中的路径
            kSelectedItem.push_back(sFullPath);
        }

    }

    //在指定位置显示弹出菜单
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
    *pResult = 0;
}


//分享列表响应事件
void CThridDownClientDlg::OnNMRClickListSharedMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    std::vector<CString> kSelectedItem; //多个选中项。
    CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
    menu.LoadMenu(IDR_MENU1); //装载自定义的右键菜单
    pSubMenu = menu.GetSubMenu(2); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
    CPoint oPoint; //定义一个用于确定光标位置的位置
    GetCursorPos(&oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标

    m_DownList->SetExtendedStyle(m_DownList->GetExtendedStyle( ) | LVS_EX_FULLROWSELECT);

    POSITION sSelectedPos = m_DownList->GetFirstSelectedItemPosition( );  //获取首选中行位置
    if (sSelectedPos == NULL)
    {
        return;
    }
    //选中的数据
    //选中的1个或多个数据
    while (sSelectedPos)//获取首选中行位置
    {
        int nSelItem = -1;
        nSelItem = m_DownList->GetNextSelectedItem(sSelectedPos);//获取选中行的索引，通过POSITION转化 

        if (nSelItem >= 0 && nSelItem < m_DownList->GetItemCount( ))
        {
            CString sFullPath = m_DownList->GetItemText(nSelItem, 1);
            //MessageBox(sFullPath ); //显示当前选中的路径
            kSelectedItem.push_back(sFullPath);
        }

    }

    //在指定位置显示弹出菜单
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
    *pResult = 0;
}


void CThridDownClientDlg::OnBnClickedOk( )
{
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnOK( );
}


void CThridDownClientDlg::OnBnClickedCancel( )
{
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnCancel( );
}


void CThridDownClientDlg::FillData( )
{
    //////////////////////////////////////
    m_FileList->InsertItem(0, _T("123"));
    m_FileList->SetItemText(0, 1, _T("234"));
    m_FileList->SetItemText(0, 2, _T("345"));
    m_FileList->SetItemText(0, 3, _T("已分享"));

    m_FileList->InsertItem(1, _T("123"));
    m_FileList->SetItemText(1, 1, _T("234"));
    m_FileList->SetItemText(1, 2, _T("345"));
    m_FileList->SetItemText(1, 3, _T("已上传"));

    m_FileList->InsertItem(2, _T("123"));
    m_FileList->SetItemText(2, 1, _T("234"));
    m_FileList->SetItemText(2, 2, _T("345"));
    m_FileList->SetItemText(2, 3, _T("本地"));

    m_FileList->InsertItem(3, _T("123"));
    m_FileList->SetItemText(3, 1, _T("234"));
    m_FileList->SetItemText(3, 2, _T("345"));
    m_FileList->SetItemText(3, 3, _T("本地"));

    /////////////////////////////////////////
    m_DownList->InsertItem(0, _T("123"));
    m_DownList->SetItemText(0, 1, _T("234"));
    m_DownList->SetItemText(0, 2, _T("345"));
    m_DownList->SetItemText(0, 3, _T("已下载"));

    m_DownList->InsertItem(1, _T("123"));
    m_DownList->SetItemText(1, 1, _T("234"));
    m_DownList->SetItemText(1, 2, _T("345"));
    m_DownList->SetItemText(1, 3, _T("正在下载"));

    m_DownList->InsertItem(2, _T("123"));
    m_DownList->SetItemText(2, 1, _T("234"));
    m_DownList->SetItemText(2, 2, _T("345"));
    m_DownList->SetItemText(2, 3, _T("未下载"));

    m_DownList->InsertItem(3, _T("123"));
    m_DownList->SetItemText(3, 1, _T("234"));
    m_DownList->SetItemText(3, 2, _T("345"));
    m_DownList->SetItemText(3, 3, _T("暂停"));

    ////////////////////////////////////////
    m_SharedList->InsertItem(0, _T("234da434zz39n0v343un4374n2c9342bn364b82402n8340823402n34234023n94c29403498028234u8v3234bg254j234h3g"));
    m_SharedList->SetItemText(0, 1, _T("234"));
    m_SharedList->SetItemText(0, 2, _T("345"));
    m_SharedList->SetItemText(0, 3, _T("56"));
    m_SharedList->SetItemText(0, 4, _T("20160707234507"));

    m_SharedList->InsertItem(1, _T("234da434ttt9n0v343un4374n2c9342bn364b82402n8340823402n34234023n94c29403498028234v3234bg254j234h3grt"));
    m_SharedList->SetItemText(1, 1, _T("234"));
    m_SharedList->SetItemText(1, 2, _T("345"));
    m_SharedList->SetItemText(1, 3, _T("56"));
    m_SharedList->SetItemText(1, 4, _T("20160517234507"));

    m_SharedList->InsertItem(2, _T("234da434ggg9n0v343un4374n2c9342bn364b82402n8340823402n34234023n94c294034980282347hv3234bg254j234h3g"));
    m_SharedList->SetItemText(2, 1, _T("234"));
    m_SharedList->SetItemText(2, 2, _T("345"));
    m_SharedList->SetItemText(2, 3, _T("56"));
    m_SharedList->SetItemText(2, 4, _T("20161207234507"));

    m_SharedList->InsertItem(3, _T("234da434xxx9n0v343un4374n2c9342bn364b82402n8340823402n3423-4023n94c29403498028234 v3234bg254j234h3g"));
    m_SharedList->SetItemText(3, 1, _T("234"));
    m_SharedList->SetItemText(3, 2, _T("345"));
    m_SharedList->SetItemText(3, 3, _T("56"));
    m_SharedList->SetItemText(3, 4, _T("20170427034507"));
}


void CThridDownClientDlg::DeleteFile( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::OnUploadFile( )
{
    CString str;
    int nId;
    //首先得到点击的位置
    POSITION pos = m_FileList->GetFirstSelectedItemPosition( );
    if (pos == NULL)
    {
        MessageBox(_T("请至少选择一项"), _T("提示"), MB_ICONEXCLAMATION);
        return;
    }
    //得到行号，通过POSITION转化
    nId = (int) m_FileList->GetNextSelectedItem(pos);

    //得到列中的内容（0表示第一列，同理1,2,3...表示第二，三，四...列）
    str = m_FileList->GetItemText(nId, 0);
    //MessageBox(str);

    //获取文件的FilsSHA512值

    std::string filename = CT2A(str);
    //m_VUpFileList.push_back(filename);
   
    std::string sha512;

    GetFileSHA512(filename, sha512);
    g_ComData.m_UploadFile.push_back(ComData::Vec3(filename,sha512,0));
    //g_ComData.curUploadFile.push_back(std::make_pair(filename, sha512));
    filename += '+';
    filename += sha512;
    filename += '+';
    filename += user.userid();
    filename += '+';
    filename += user.userps( );

    boost::asio::io_service io_ser; 
    SendFile sender;
    try
    {
        sender.senderLitter(io_ser, "127.0.0.1", 8089, filename.c_str( ), 'a');
    }
    catch (CException* e)
    {
    }
    
}


void CThridDownClientDlg::DownloadFile( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::MakeShared( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::StartDownload( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::PauseDownload( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::ReStart( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::DeleteSharedUrl( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::CopyUrl( )
{
    // TODO: 在此添加命令处理程序代码
}


void CThridDownClientDlg::OnBnClickedDownloadurl( )
{
    // TODO: 在此添加控件通知处理程序代码
}

void CThridDownClientDlg::GetFileSHA512(std::string& fileName, std::string& FileSHA512)
{
    EVP_MD_CTX mdctx;
    const EVP_MD *md = NULL;
    char buffer[256];
    char FFFF[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    unsigned char mdValue[EVP_MAX_MD_SIZE] = {0};
    unsigned int mdLen = 0;

    OpenSSL_add_all_digests( );
    md = EVP_get_digestbyname("sha512");

    EVP_MD_CTX_init(&mdctx);
    EVP_DigestInit_ex(&mdctx, md, NULL);

    boost::filesystem::ifstream inFile(fileName, std::ios::in | std::ios::binary);

    unsigned long endPos = inFile.tellg( );
    inFile.seekg(0, std::ios::beg);
    unsigned long inPos = 0;

    while ((endPos - inPos) > 256)
    {
        inPos = inFile.tellg( );
        inFile.read(buffer, 256);
        EVP_DigestUpdate(&mdctx, buffer, 256);
    }

    inFile.read(buffer, endPos - inPos);
    EVP_DigestUpdate(&mdctx, buffer, endPos - inPos);

    EVP_DigestFinal_ex(&mdctx, mdValue, &mdLen);
    EVP_MD_CTX_cleanup(&mdctx);

    int j = 0;
    for (j = 0; j < mdLen; j++)
    {
        printf("%s", mdValue[j]);
    }

    for (int ii = 0; ii < 64; ++ii)
    {
        int x = mdValue[ii];
        int xx = x & 15;
        int xxx = x & 240;
        xxx = xxx >> 4;
        FileSHA512 + FFFF[xxx] + FFFF[xx];
    }
    //FileSHA512
}

void CThridDownClientDlg::UpDataUI()
{
    SetTimer(1, 15000,NULL);

}
