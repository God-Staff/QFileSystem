// QFileSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SendFile.hpp"
#include "Servers.hpp"
#include "QFileSystem.h"
#include "QFileSystemDlg.h"

//#include "public.h"
//#include "PublicStruct.pb.h"

ComData g_ComData;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CQFileSystemDlg 对话框

//BEGIN_DHTML_EVENT_MAP(CQFileSystemDlg)
//	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
//	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
//END_DHTML_EVENT_MAP()


CQFileSystemDlg::CQFileSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QFILESYSTEM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQFileSystemDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQFileSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &CQFileSystemDlg::OnNMRClickFileList)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_Save_Servers, &CQFileSystemDlg::OnNMRClickClientList)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_Shared, &CQFileSystemDlg::OnNMRClickSharedList)
    ON_BN_CLICKED(IDC_BUTTON1, &CQFileSystemDlg::OnBnClickedStart)
    ON_BN_CLICKED(IDC_BUTTON2, &CQFileSystemDlg::OnBnClickedend)
END_MESSAGE_MAP()


// CQFileSystemDlg 消息处理程序

BOOL CQFileSystemDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	
	///////////////////////////////////
	//文件列表
	m_ListFile = (CListCtrl*)GetDlgItem (IDC_LIST_FILE);
	DWORD dwStyle1 = GetWindowLong (m_ListFile->m_hWnd, GWL_STYLE);
	SetWindowLong (m_ListFile->m_hWnd, GWL_STYLE, dwStyle1 | LVS_REPORT);

	//设置listctrl可以整行选择和网格条纹
	DWORD styles1 = m_ListFile->GetExtendedStyle ();
    m_ListFile->SetExtendedStyle (styles1 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//给listctrl设置5个标题栏
    TCHAR rgtsz1[5][10] = {_T("文件名"), _T("SHA512"), _T("文件大小")
                            ,_T("创建时间"),_T("是否分享")};

	//修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变

	LV_COLUMN lvcolumn1;
	CRect rect1;
    m_ListFile->GetWindowRect (&rect1);
	for (int i = 0; i < 5; i++)
	{
        lvcolumn1.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
			| LVCF_WIDTH | LVCF_ORDER;
        lvcolumn1.fmt = LVCFMT_LEFT;
        lvcolumn1.pszText = rgtsz1[i];
        lvcolumn1.iSubItem = i;
        lvcolumn1.iOrder = i;
        lvcolumn1.cx = rect1.Width () / 5;
        m_ListFile->InsertColumn (i, &lvcolumn1);
	}

	// TODO: 在此添加额外的初始化代码
    //主机列表
    m_ListSaveServer = (CListCtrl*) GetDlgItem(IDC_LIST_Save_Servers);
    DWORD dwStyle2 = GetWindowLong(m_ListSaveServer->m_hWnd, GWL_STYLE);
    SetWindowLong(m_ListSaveServer->m_hWnd, GWL_STYLE, dwStyle2 | LVS_REPORT);

    //设置listctrl可以整行选择和网格条纹
    DWORD styles2 = m_ListSaveServer->GetExtendedStyle( );
    m_ListSaveServer->SetExtendedStyle(styles2 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //给listctrl设置5个标题栏
    TCHAR rgtsz2[4][10] = {_T("存储端编号"),_T("IP"),_T("剩余空间"),_T("总空间")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn2;
    CRect rect2;
    m_ListSaveServer->GetWindowRect(&rect2);
    for (int i = 0; i < 4; i++)
    {
        lvcolumn2.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn2.fmt = LVCFMT_LEFT;
        lvcolumn2.pszText = rgtsz2[i];
        lvcolumn2.iSubItem = i;
        lvcolumn2.iOrder = i;
        lvcolumn2.cx = rect2.Width( ) / 4;
        m_ListSaveServer->InsertColumn(i, &lvcolumn2);
    }


    //分享列表
    m_ListShared = (CListCtrl*)GetDlgItem(IDC_LIST_Shared);
    DWORD dwStyle3 = GetWindowLong(m_ListShared->m_hWnd, GWL_STYLE);
    SetWindowLong(m_ListShared->m_hWnd, GWL_STYLE, dwStyle3 | LVS_REPORT);

    //设置listctrl可以整行选择和网格条纹
    DWORD styles3 = m_ListShared->GetExtendedStyle( );
    m_ListShared->SetExtendedStyle(styles3 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //给listctrl设置5个标题栏
    TCHAR rgtsz3[4][10] = {_T("URL"),_T("文件标识"),_T("验证信息"),_T("用户ID")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn3;
    CRect rect3;
    m_ListShared->GetWindowRect(&rect3);
    for (int i = 0; i < 4; i++)
    {
        lvcolumn3.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn3.fmt = LVCFMT_LEFT;
        lvcolumn3.pszText = rgtsz3[i];
        lvcolumn3.iSubItem = i;
        lvcolumn3.iOrder = i;
        lvcolumn3.cx = rect3.Width( ) / 4;
        m_ListShared->InsertColumn(i, &lvcolumn3);
    }



    //下载端在线列表
    m_ListClient = (CListCtrl*) GetDlgItem(IDC_LIST_Client);
    DWORD dwStyle4 = GetWindowLong(m_ListClient->m_hWnd, GWL_STYLE);
    SetWindowLong(m_ListClient->m_hWnd, GWL_STYLE, dwStyle4 | LVS_REPORT);

    //设置listctrl可以整行选择和网格条纹
    DWORD styles4 = m_ListClient->GetExtendedStyle( );
    m_ListClient->SetExtendedStyle(styles4 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //给listctrl设置5个标题栏
    TCHAR rgtsz4[2][10] = {_T("用户ID"),_T("IP")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn4;
    CRect rect4;
    m_ListClient->GetWindowRect(&rect4);
    for (int i = 0; i < 2; i++)
    {
        lvcolumn4.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn4.fmt = LVCFMT_LEFT;
        lvcolumn4.pszText = rgtsz4[i];
        lvcolumn4.iSubItem = i;
        lvcolumn4.iOrder = i;
        lvcolumn4.cx = rect4.Width( ) / 2;
        m_ListClient->InsertColumn(i, &lvcolumn4);
    }



    //日志列表
    m_ListLogs = (CListCtrl*) GetDlgItem(IDC_LIST_Logs);
    DWORD dwStyle5 = GetWindowLong(m_ListLogs->m_hWnd, GWL_STYLE);
    SetWindowLong(m_ListLogs->m_hWnd, GWL_STYLE, dwStyle5 | LVS_REPORT);

    //设置listctrl可以整行选择和网格条纹
    DWORD styles5 = m_ListLogs->GetExtendedStyle( );
    m_ListLogs->SetExtendedStyle(styles5 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //给listctrl设置5个标题栏
    TCHAR rgtsz5[4][10] = {_T("时间"),_T("事件"),_T("用户ID"),_T("详情")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn5;
    CRect rect5;
    m_ListLogs->GetWindowRect(&rect5);
    for (int i = 0; i < 4; i++)
    {
        lvcolumn5.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
                        | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn5.fmt = LVCFMT_LEFT;
        lvcolumn5.pszText = rgtsz5[i];
        lvcolumn5.iSubItem = i;
        lvcolumn5.iOrder = i;
        lvcolumn5.cx = rect5.Width( ) / 4;
        m_ListLogs->InsertColumn(i, &lvcolumn5);
    }

    InitData( );

	//解析数据好友和分享链接数据
	updateList ();

    runServer( );


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void runServers( )
{
    try
    {
        std::cerr << "Usage: server <address> <port> <threads> <blocksize>\n";

        boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
        short port = 8089;
        int thread_count = 6;
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
void CQFileSystemDlg::runServer( )
{
    boost::thread serverThread(runServers);
    serverThread.detach( );
}
void CQFileSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
        CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQFileSystemDlg::OnPaint()
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
HCURSOR CQFileSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQFileSystemDlg::updateList ()
{
	//qiuwanli::ID2IPTable ips;
	//qiuwanli::File2ClientServers client;

	//std::fstream ipInfo ("ID2IP", std::ios::in | std::ios::binary);
	//std::fstream clientInfo ("FileSharedLog", std::ios::in | std::ios::binary);

	//if (!ipInfo)
	//	MessageBox (L"配置文件打开失败！");

	//if (!ips.ParseFromIstream (&ipInfo))
	//{	//打开失败
	//	MessageBox (L"配置文件加载失败！");
	//	ipInfo.close ();
	//}
	//else
	//{	//解析配置文件
	//	for (int i = 0; i < ips.ip_size(); ++i)
	//	{
	//		const qiuwanli::ID2IP& myfriend = ips.ip(i);

	//		m_ListSaveServer->InsertItem (i, StringToWstring (myfriend.cilentid()).c_str ());
 //           m_ListSaveServer->SetItemText (i, 1, StringToWstring (myfriend.ip ()).c_str ());
 //           m_ListSaveServer->SetItemText (i, 2, StringToWstring (myfriend.keymd5 ()).c_str ());
	//	}
	//}
	//ipInfo.close ();

	//if (!clientInfo)
	//	MessageBox (L"sharedlist 配置文件打开失败！");

	//if (!client.ParseFromIstream (&clientInfo))
	//{	//打开失败
	//	MessageBox (L" sharedlist 配置文件加载失败！");
	//	clientInfo.close ();
	//}
	//else
	//{	//解析配置文件
	//	for (int i = 0; i < client.client_size(); ++i)
	//	{
	//		const qiuwanli::File2Cilent& shared = client.client (i);

	//		m_ListFile->InsertItem (i, StringToWstring (shared.filename ()).c_str ());
 //           m_ListFile->SetItemText (i, 1, StringToWstring (shared.sha512 ()).c_str ());
 //           m_ListFile->SetItemText (i, 2, StringToWstring (shared.cilentid ()).c_str ());
 //           m_ListFile->SetItemText (i, 3, StringToWstring (shared.createdate ()).c_str ());
	//	}
	//}

	//clientInfo.close ();
}

//void CQFileSystemDlg::MakeFilesLog (qiuwanli::File2Cilent * file
//                                    , std::string filename
//                                    , std::string sha512
//                                    , std::string client
//                                    , std::string createtime)
//{
//	file->set_filename (filename);
//	file->set_sha512 (sha512);
//	file->set_cilentid (client);
//	file->set_createdate (createtime);
//}
//
//void CQFileSystemDlg::MakeLogs (qiuwanli::Logs * Log
//                                , std::string user_id
//                                , std::string logdate
//                                , std::string loginfo
//                                , std::string logtype) 
//{
//	Log->set_user_id (user_id);
//	Log->set_log_date (logdate);
//	Log->set_log_info (loginfo);
//	Log->set_log_type (logtype);
//}
//void CQFileSystemDlg::MakeLogs (qiuwanli::ID2IP * id2ip
//                                , std::string clientid
//                                , std::string ip
//                                , std::string Prikey
//                                , std::string KeyMd5
//                                , std::string Others)
//{
//	id2ip->set_cilentid (clientid);
//	id2ip->set_ip (ip);
//	id2ip->set_prikey (Prikey);
//	id2ip->set_keymd5 (KeyMd5);
//}

void CQFileSystemDlg::sender (boost::asio::io_service &io
                              , const char*	ip_address
                              , unsigned	port
                              , const char* filename
                              , const char* msg_type)
{
	FILE *fp;
	fopen_s (&fp, filename, "rb");
	if (fp == NULL) 
    {
		std::cerr << "cannot open file\n";
		return;
	}

	//使用智能指针，防止程序出现异常时，fclose未被调用。
	boost::shared_ptr<FILE> file_ptr (fp, fclose);

	clock_t cost_time = clock ();

	const size_t k_buffer_size = k_times * 1024;
	char buffer[k_buffer_size];
    DataBlockTypeInfo file_info;

	char buf[k_times * 1024];
	strcat_s (buf, filename);
	strcat_s (buf, "+");
	strcat_s (buf, msg_type);
	filename = (const char*)buf;

	//MessageBox ();
	//const char* filename_msg = filename + msg_type;

	int filename_size = strlen (filename) + 1;
	size_t file_info_size = sizeof (file_info);
	size_t total_size = file_info_size + filename_size;
	if (total_size > k_buffer_size) 
    {
		std::cerr << "File name is too long";
		return;
	}
	file_info.m_FileNameLength = filename_size;
	fseek (fp, 0, SEEK_END);
	file_info.m_FileSize = ftell (fp);
	rewind (fp);

	memcpy (buffer, &file_info, file_info_size);								//文件信息
	memcpy (buffer + file_info_size, filename, filename_size);					//文件名/消息类型

	boost::asio::ip::tcp::socket socket (io);
	socket.connect (boost::asio::ip::tcp::endpoint (boost::asio::ip::address_v4::from_string (ip_address), port));

	std::cout << "Sending file : " << filename << " MsgType:" << msg_type << std::endl;
	size_t len = total_size;

	unsigned long long total_bytes_read = 0;
	while (true) 
    {
		//先发送文件头，之后发送data
		socket.send (boost::asio::buffer (buffer, len), 0);
		if (feof (fp))
        {
            break;
        }

		len = fread (buffer, 1, k_buffer_size, fp);
		total_bytes_read += len;
	}

	//计算时间、大小和速度//
	cost_time = clock () - cost_time;
	if (cost_time == 0) cost_time = 1;
	double speed = total_bytes_read * (CLOCKS_PER_SEC / 1024.0 / 1024.0) / cost_time;
	std::cout << "cost time: " << cost_time / (double)CLOCKS_PER_SEC << " s "
		<< "  transferred_bytes: " << total_bytes_read << " bytes\n"
		<< "speed: " << speed << " MB/s\n\n";
}

//每次只允许选择一项
//右键响应事件
void CQFileSystemDlg::OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取点击Item项
    m_ListFile->SetExtendedStyle (m_ListFile->GetExtendedStyle () | LVS_EX_FULLROWSELECT);
	POSITION sSelectedPos = m_ListFile->GetFirstSelectedItemPosition ();  //获取首选中行位置
	if (sSelectedPos == NULL)
	{
		return;
	}

	int nSelItem = -1;
	nSelItem = m_ListFile->GetNextSelectedItem (sSelectedPos);
	if (nSelItem >= 0 && nSelItem < m_ListFile->GetItemCount ())
	{
		CString sFullPath = m_ListFile->GetItemText (nSelItem, 1);
		//将文件的SHA512存入到Vector中
		std::string ss = CT2A (sFullPath);
		CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
		menu.LoadMenu (IDR_MENU1); //装载自定义的右键菜单
		CPoint oPoint; //定义一个用于确定光标位置的位置

		pSubMenu = menu.GetSubMenu ((int)(checkItem (ss) ? 1 : 0)); //获取第一个弹出菜单，所以第一个菜单必须有子菜单

		GetCursorPos (&oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标
								//在指定位置显示弹出菜单
		pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	*pResult = 0;
}

bool CQFileSystemDlg::checkItem (std::string item)
{
	return true;
}


void CQFileSystemDlg::OnNMRClickClientList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码


    *pResult = 0;
}


void CQFileSystemDlg::OnNMRClickSharedList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码


    *pResult = 0;
}


void CQFileSystemDlg::InitData( )
{
    m_ListFile->InsertItem(0, L"vc++开发大全");
    m_ListFile->SetItemText(0, 1, L"df89cia9da7dasd80ad87as0da0sd");
    m_ListFile->SetItemText(0, 2, L"8764253");
    m_ListFile->SetItemText(0, 3, L"20160304133307");
    m_ListFile->InsertItem(1, L"Boost开发大全");
    m_ListFile->SetItemText(1, 1, L"df89cia9da7dasd80ad87as0da0sd");
    m_ListFile->SetItemText(1, 2, L"432534");
    m_ListFile->SetItemText(1, 3, L"20160304134409");
    m_ListFile->InsertItem(2, L"CPlusPlus");
    m_ListFile->SetItemText(2, 1, L"df89cia9da7dasd80ad87as0da0sd");
    m_ListFile->SetItemText(2, 2, L"743563");
    m_ListFile->SetItemText(3, 3, L"20160304132055");

    m_ListSaveServer->InsertItem(0, L"10004");
    m_ListSaveServer->SetItemText(0, 1, L"124.54.77.33");
    m_ListSaveServer->SetItemText(0, 2, L"23452623");
    m_ListSaveServer->SetItemText(0, 3, L"923452623");
    m_ListSaveServer->InsertItem(1, L"10070");
    m_ListSaveServer->SetItemText(1, 1, L"124.54.74.33");
    m_ListSaveServer->SetItemText(1, 2, L"86452623");
    m_ListSaveServer->SetItemText(1, 3, L"234452623");
    m_ListSaveServer->InsertItem(2, L"10008");
    m_ListSaveServer->SetItemText(2, 1, L"124.54.77.123");
    m_ListSaveServer->SetItemText(2, 2, L"23452623");
    m_ListSaveServer->SetItemText(3, 3, L"777452623");

    m_ListShared->InsertItem(0, L"etynnh89adasdaa0nendjfaij09434f343");
    m_ListShared->SetItemText(0, 1, L"89adasdaa0nendjfaij09434f343");
    m_ListShared->SetItemText(0, 2, L"5efwg4");
    m_ListShared->SetItemText(0, 3, L"2054534");
    m_ListShared->InsertItem(1, L"bsgrts89adasdaa0nendjfaij09434f343");
    m_ListShared->SetItemText(1, 1, L"89adasdaa0nendjfaij09434f343");
    m_ListShared->SetItemText(1, 2, L"234534");
    m_ListShared->SetItemText(1, 3, L"2364565");
    m_ListShared->InsertItem(2, L"fgfawe89adasdaa0nendjfaij09434f343");
    m_ListShared->SetItemText(2, 1, L"89adasdaa0nendjfaij09434f343");
    m_ListShared->SetItemText(2, 2, L"5hf2fd");
    m_ListShared->SetItemText(2, 3, L"5634543");

    m_ListClient->InsertItem(0, L"5634543");
    m_ListClient->SetItemText(0, 1, L"174.74.77.33");
    m_ListClient->InsertItem(1, L"2364565");
    m_ListClient->SetItemText(1, 1, L"129.54.74.33");
    m_ListClient->InsertItem(2, L"2054534");
    m_ListClient->SetItemText(2, 1, L"177.58.97.33");

    m_ListLogs->InsertItem(0, L"2017-05-14 05:30:22");
    m_ListLogs->SetItemText(0, 1, L"上传");
    m_ListLogs->SetItemText(0, 2, L"2364565");
    m_ListLogs->SetItemText(0, 3, L"dafs");
    m_ListLogs->InsertItem(1, L"2017-05-14 05:50:33");
    m_ListLogs->SetItemText(1, 1, L"分享");
    m_ListLogs->SetItemText(1, 2, L"2364565");
    m_ListLogs->SetItemText(1, 3, L"dafs");
    m_ListLogs->InsertItem(2, L"2017-05-15 09:07:42");
    m_ListLogs->SetItemText(2, 1, L"下载");
    m_ListLogs->SetItemText(2, 2, L"5634543");
    m_ListLogs->SetItemText(3, 3, L"dafs");
    m_ListLogs->InsertItem(2, L"2017-05-15 09:07:42");
    m_ListLogs->SetItemText(2, 1, L"删除");
    m_ListLogs->SetItemText(2, 2, L"2364565");
    m_ListLogs->SetItemText(3, 3, L"dafs");

    //
    LoadFileList( );
    LoadSaveServerList( );
    LoadSharedList( );
    LoadClientList( );
    LoadLogsList( );

}

void CQFileSystemDlg::LoadFileList( )
{

}

void CQFileSystemDlg::LoadSharedList( )
{

}

void CQFileSystemDlg::LoadSaveServerList( )
{

}

void CQFileSystemDlg::LoadClientList( )
{

}

void CQFileSystemDlg::LoadLogsList( )
{

}



void CQFileSystemDlg::OnBnClickedStart( )
{
    // TODO: 在此添加控件通知处理程序代码
}


void CQFileSystemDlg::OnBnClickedend( )
{
    // TODO: 在此添加控件通知处理程序代码
}
