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

#include "public.h"
//#include "PublicStruct.pb.h"
//std::mutex g_mutex;
ComData g_ComData;
CInterface PublicData;
qiuwanli::BlockList4DownTable m_BlockToFileTableTmp;
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
    ON_WM_TIMER()           //定时器通知
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &CQFileSystemDlg::OnNMRClickFileList)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_Save_Servers, &CQFileSystemDlg::OnNMRClickClientList)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_Shared, &CQFileSystemDlg::OnNMRClickSharedList)
END_MESSAGE_MAP()


// CQFileSystemDlg 消息处理程序

BOOL CQFileSystemDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //::InitializeCriticalSection(&g_ComData);
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
    TCHAR rgtsz2[5][10] = {_T("存储端编号"),_T("IP"),_T("剩余空间"),_T("总空间"),_T("状态")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn2;
    CRect rect2;
    m_ListSaveServer->GetWindowRect(&rect2);
    for (int i = 0; i < 5; i++)
    {
        lvcolumn2.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn2.fmt = LVCFMT_LEFT;
        lvcolumn2.pszText = rgtsz2[i];
        lvcolumn2.iSubItem = i;
        lvcolumn2.iOrder = i;
        lvcolumn2.cx = rect2.Width( ) / 5;
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
    TCHAR rgtsz3[4][10] = {_T("文件标识"),_T("验证信息"),_T("用户ID"),_T("文件大小")};

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


    //分享列表
    m_ListBlockInfo = (CListCtrl*) GetDlgItem(IDC_LIST_BLOCKINFO);
    DWORD dwStyle4 = GetWindowLong(m_ListBlockInfo->m_hWnd, GWL_STYLE);
    SetWindowLong(m_ListBlockInfo->m_hWnd, GWL_STYLE, dwStyle4 | LVS_REPORT);

    //设置listctrl可以整行选择和网格条纹
    DWORD styles4 = m_ListBlockInfo->GetExtendedStyle( );
    m_ListBlockInfo->SetExtendedStyle(styles4 | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //给listctrl设置5个标题栏
    TCHAR rgtsz4[4][10] = {_T("文件SHA512"),_T("文件块号"),_T("块的MD5"),_T("存储位置")};

    //修改数组大小，可以确定分栏数和没栏长度，如果修改下面的数据（蓝色部分）也要跟着改变
    LV_COLUMN lvcolumn4;
    CRect rect4;
    m_ListBlockInfo->GetWindowRect(&rect4);
    for (int i = 0; i < 4; i++)
    {
        lvcolumn4.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT
            | LVCF_WIDTH | LVCF_ORDER;
        lvcolumn4.fmt = LVCFMT_LEFT;
        lvcolumn4.pszText = rgtsz4[i];
        lvcolumn4.iSubItem = i;
        lvcolumn4.iOrder = i;
        lvcolumn4.cx = rect4.Width( ) / 4;
        m_ListBlockInfo->InsertColumn(i, &lvcolumn4);
    }

    InitData( );

	//解析数据好友和分享链接数据
	updateList ();

    runServer( );

    SetTimer(1, 1000, NULL);
    SetTimer(2, 30000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


//定时器
void CQFileSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case 1: //更新数据
    {
        std::lock_guard<std::mutex> guard(g_mutex);
        //更新UI文件列表
        if (((size_t(1)&g_ComData.m_UIChange) == size_t(1)) && (g_ComData.NotifyFileList.size( )))
        {
            for (auto xx :g_ComData.NotifyFileList)
            {
                CString x1;
                x1 = xx.v1.c_str( );
                m_ListFile->InsertItem(0, x1);

                CString x2;
                x2 = xx.v2.c_str( );
                m_ListFile->SetItemText(0, 1, x2);

                CString x3;
                x3 = xx.v3.c_str( );
                m_ListFile->SetItemText(0, 1, x3);

                CString x4;
                x4 = xx.v4.c_str( );
                m_ListFile->SetItemText(0, 1, x4);

                CString x5;
                x5 = xx.v5.c_str( );
                m_ListFile->SetItemText(0, 1, x5);
            }
           g_ComData.NotifyFileList.clear( );

           //重置状态位
           size_t tmp = size_t(1);
           tmp = ~tmp;
           g_ComData.m_UIChange &= tmp;
           
           //写入文件
           WriteFileInfoList( );
        }

        //更新在线的存储端
        if (((size_t(2)&g_ComData.m_UIChange) == size_t(2)) && (g_ComData.m_HeartList.size( ) > 0))
        {
            //更新UI，Heart文件列表
            bool nee = false;

            for (auto xx : g_ComData.m_HeartList)
            {
                //std::lock_guard<std::mutex> guard(g_mutex);
                for (int index = 0; index < g_ComData.m_ClientConfigFile.clientinfo_size( ); ++index)
                {
                    if (g_ComData.m_ClientConfigFile.clientinfo(index).cilentid( ) == xx.ClientID)
                    {
                        nee = true;
                        auto item = g_ComData.m_ClientConfigFile.mutable_clientinfo(index);
                        item->set_online("ON");
                    }
                }

                //动态添加列表
                if (nee == false)
                {
                    std::string IPList[10] = {"127.0.0.1","192.4.2.9","127.0.0.1","212.4.45.9","147.0.7.1","192.34.65.9","127.53.54.1","192.4.45.9","246.23.54.23"};
                    std::string IPs;
                    std::srand(unsigned(time(0)));
                    int pos = std::rand( );
                    if (pos < 10)
                    {
                        IPs = IPList[pos];
                    }

                    PublicData.DoClientConfigFileTable(g_ComData.m_ClientConfigFile.add_clientinfo( ), xx.ClientID, IPs, "213dfsefgser", xx.Prikeymd5, xx.Remain, xx.Total);
                }
            }

           
           g_ComData.m_HeartList.clear( );

            //重置状态位
            size_t tmp = size_t(2);
            tmp = ~tmp;
           g_ComData.m_UIChange &= tmp;

            //更新界面
            UpDateUISaveServerList( );
            UpdateUIFileInfoList( );
        }

        //定时更新显示，已经接受到的数据块
        if (((size_t(4)&g_ComData.m_UIChange) == size_t(4)) && (g_ComData.g_AddBlockTmp.size( ) > 0))
        {
            //std::lock_guard<std::mutex> guard(g_mutex);
            for (auto xxxx : g_ComData.g_AddBlockTmp)
            {
                if (xxxx.size( ) == 4)
                {
                    CString Data1;
                    auto xx = xxxx.begin( );
                    Data1 = xx->c_str( );
                    m_ListBlockInfo->InsertItem(0, Data1);

                    ++xx;
                    CString Data3;
                    std::string ss = xx->c_str( );
                    Data3 = ss.c_str( );
                    m_ListBlockInfo->SetItemText(0, 1, Data3);

                    ++xx;
                    CString Data2;
                    Data2 = xx->c_str( );
                    m_ListBlockInfo->SetItemText(0, 2, Data2);

                    ++xx;
                    CString Data4;
                    Data4 = xx->c_str( );
                    m_ListBlockInfo->SetItemText(0, 3, Data4);
                }
            }
           g_ComData.g_AddBlockTmp.clear( );

            //重置状态位
            size_t tmp = size_t(4);
            tmp = ~tmp;
           g_ComData.m_UIChange &= tmp;

           //将数据块写入到文件
           //合并数据写入文件
           g_ComData.m_BlockToFileTable.MergeFrom(g_ComData.m_BlockToFileTableTmp);
           WriteBlockInfo( );
        }

        //定时更新显示，文件传输完成时进行界面更新
        if (((size_t(8)&g_ComData.m_UIChange) == size_t(8)) && (g_ComData.g_AddBlockTmp.size( ) > 0))
        {
            
            for (auto xxxx : g_ComData.g_AddBlockTmp)
            {
             
            }
            g_ComData.g_AddBlockTmp.clear( );

            //重置状态位
            size_t tmp = size_t(8);
            tmp = ~tmp;
            g_ComData.m_UIChange &= tmp;
        }

    }
    break;
    case 2:
    {
        std::lock_guard<std::mutex> guard(g_mutex);
        //定时数据到保存文件
        WriteBlockInfo( );
        WriteFileInfoList( );
        WriteSaveServerList( );
        WriteUserInfo( );
        //WriteSaveServerList( );
        WriteSharedList( );
        //

    }
    break;
    default:
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}

void runServers( )
{
    try
    {
        std::cerr << "Usage: server <address> <port> <threads> <blocksize>\n";

        boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
        short port = 8189;
        int thread_count = 16;
        size_t block_size = k_buffer_size;

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
	
}


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


//数据的初始化
void CQFileSystemDlg::InitData( )
{
    //初始化列表
    ReadBlockInfo( );
    //ReadFileInfoList( );
    ReadSaveServerList( );
    ReadSharedList( );
    readUserInfo( );

    m_ListFile->InsertItem(0, _T("VC++标准库.pdf"));
    m_ListFile->SetItemText(0, 1, _T("5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"));
    m_ListFile->SetItemText(0, 2, _T("3265235"));
    m_ListFile->SetItemText(0, 3, _T("2017-1-3 8:34:44"));
    m_ListFile->SetItemText(0, 4, _T("已分享"));

    m_ListFile->InsertItem(0, _T("BOOST标准库.pdf"));
    m_ListFile->SetItemText(0, 1, _T("4ABD812CF05FE5D4925D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"));
    m_ListFile->SetItemText(0, 2, _T("6874654"));
    m_ListFile->SetItemText(0, 3, _T("2017-5-3 8:34:44"));
    m_ListFile->SetItemText(0, 4, _T(""));

    m_ListFile->InsertItem(0, _T("轮回不止.pdf"));
    m_ListFile->SetItemText(0, 1, _T("543DD5DCA59C9E4ABD815D492643DD5DCA59C9E4ABD812CF05FE5D4922CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"));
    m_ListFile->SetItemText(0, 2, _T("68765456"));
    m_ListFile->SetItemText(0, 3, _T("2017-3-3 18:35:44"));
    m_ListFile->SetItemText(0, 4, _T("已分享"));

    m_ListFile->InsertItem(0, _T("爱的方式.pdf"));
    m_ListFile->SetItemText(0, 1, _T("HD492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"));
    m_ListFile->SetItemText(0, 2, _T("796785543"));
    m_ListFile->SetItemText(0, 3, _T("2017-1-3 8:34:44"));
    m_ListFile->SetItemText(0, 4, _T(""));

    //qiuwanli::FileInfoListTable ssss;
    g_ComData.m_FileListTable.Clear( );
    PublicData.DoFileInfoListTable(g_ComData.m_FileListTable.add_filelist( )
                                   , "5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "爱的方式.pdf"
                                   , "2017-1-3 8:34:44"
                                   , ""
                                   , 5345
                                   , 4546464
                                   , "");

    PublicData.DoFileInfoListTable(g_ComData.m_FileListTable.add_filelist( )
                                   , "HD492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "VC++标准库.pdf"
                                   , "2017-1-3 8:34:44"
                                   , "已分享"
                                   , 345
                                   , 4564645
                                   , "");

    PublicData.DoFileInfoListTable(g_ComData.m_FileListTable.add_filelist( )
                                   , "4ABD812CF05FE5D4925D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "92643DD5DCA59C9E4ABD812C43DD5DC"
                                   , "BOOST标准库.pdf"
                                   , "2017-11-3 8:34:44"
                                   , ""
                                   , 355
                                   , 47464
                                   , "");

    PublicData.DoFileInfoListTable(g_ComData.m_FileListTable.add_filelist( )
                                   , "543DD5DCA59C9E4ABD815D492643DD5DCA59C9E4ABD812CF05FE5D4922CF05FE5D492643DD5DCA59C9E4ABD812CF05FE5D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "D492643DD5DCA59C9E4ABD812CF05FE"
                                   , "爱的方式.pdf"
                                   , "2017-3-3 18:35:44"
                                   , ""
                                   , 345
                                   , 4456464
                                   , "");

    WriteFileInfoList( );

}

//载入文件存储块对应的文件列表
void CQFileSystemDlg::ReadBlockInfo( )
{
    boost::filesystem::fstream readBlockInfoFile("ServerBlockInfoFiles", std::ios::in | std::ios::binary);
    if (readBlockInfoFile.is_open( ))
    {
        if (!g_ComData.m_BlockToFileTable.ParseFromIstream(&readBlockInfoFile))
            MessageBox(_T("ServerBlockInfoFiles 文件解析失败！"));
    }
    else
    {
        MessageBox(_T("ServerBlockInfoFiles 文件打开失败！"));
    }

    for (int index = 0; index <g_ComData.m_BlockToFileTable.blocklistfordown_size( ); ++index)
    {
        //_T("文件名"), _T("SHA512"), _T("文件大小"), _T("创建时间"), _T("是否分享")
        CString Data1;
        Data1 =g_ComData.m_BlockToFileTable.blocklistfordown(index).filesha512( ).c_str( );
        m_ListBlockInfo->InsertItem(0, Data1);

        CString Data3;
        std::string ss = std::to_string(g_ComData.m_BlockToFileTable.blocklistfordown(index).blocknumer( ));
        Data3 = ss.c_str( );
        m_ListBlockInfo->SetItemText(0, 1, Data3);

        CString Data2;
        Data2 =g_ComData.m_BlockToFileTable.blocklistfordown(index).blockmd5( ).c_str( );
        m_ListBlockInfo->SetItemText(0, 2, Data2);

        CString Data4;
        Data4 =g_ComData.m_BlockToFileTable.blocklistfordown(index).saveserversip( ).c_str( );
        m_ListBlockInfo->SetItemText(0, 3, Data4);
    }

    readBlockInfoFile.close( );
}
void CQFileSystemDlg::UpdateUIBlockInfo( )
{
    m_ListBlockInfo->DeleteAllItems( );

    for (int index = 0; index <g_ComData.m_BlockToFileTable.blocklistfordown_size( ); ++index)
    {
        CString Data1;
        Data1 =g_ComData.m_BlockToFileTable.blocklistfordown(index).filesha512( ).c_str( );
        m_ListBlockInfo->InsertItem(0, Data1);

        CString Data3;
        std::string ss = std::to_string(g_ComData.m_BlockToFileTable.blocklistfordown(index).blocknumer( ));
        Data3 = ss.c_str( );
        m_ListBlockInfo->SetItemText(0, 1, Data3);

        CString Data2;
        Data2 =g_ComData.m_BlockToFileTable.blocklistfordown(index).blockmd5( ).c_str( );
        m_ListBlockInfo->SetItemText(0, 2, Data2);

        CString Data4;
        Data4 =g_ComData.m_BlockToFileTable.blocklistfordown(index).saveserversip( ).c_str( );
        m_ListBlockInfo->SetItemText(0, 3, Data4);
    }
}
void  CQFileSystemDlg::WriteBlockInfo( )
{
    boost::filesystem::fstream readBlockInfoFile("ServerBlockInfoFiles", std::ios::trunc | std::ios::out | std::ios::binary);
    if (!readBlockInfoFile.is_open( ))
        MessageBox(_T("ServerBlockInfoFiles 文件打开失败！"));
    if (!g_ComData.m_BlockToFileTable.ParseFromIstream(&readBlockInfoFile))
        MessageBox(_T("ServerBlockInfoFiles 文件解析失败！"));

    readBlockInfoFile.close( );
}

//解析文件列表
void CQFileSystemDlg::ReadFileInfoList( )
{ 
    boost::filesystem::fstream readFile("FileInfoList", std::ios::in | std::ios::binary);
    if (!readFile.is_open( ))
        return;

    if (!g_ComData.m_FileListTable.ParsePartialFromIstream(&readFile));

    readFile.close( );

    for (int index = 0; index <g_ComData.m_FileListTable.filelist_size( ); ++index)
    {
        //_T("文件名"), _T("SHA512"), _T("文件大小"), _T("创建时间"), _T("是否分享")
        CString Data1;
        Data1 =g_ComData.m_FileListTable.filelist(index).filename( ).c_str( );
        m_ListFile->InsertItem(0, Data1);

        CString Data2;
        Data2 =g_ComData.m_FileListTable.filelist(index).filesha512( ).c_str( );
        m_ListFile->SetItemText(0, 1, Data2);

        CString Data3;
        std::string ss = std::to_string(g_ComData.m_FileListTable.filelist(index).filetotalsize( ));
        Data3 = ss.c_str( );
        m_ListFile->SetItemText(0, 2, Data3);

        CString Data4;
        Data4 =g_ComData.m_FileListTable.filelist(index).filecreatedate( ).c_str( );
        m_ListFile->SetItemText(0, 3, Data4);

        CString Data5;
        Data5 =g_ComData.m_FileListTable.filelist(index).isshared( ).c_str( );
        m_ListFile->SetItemText(0, 4, Data5);

    }
}
void CQFileSystemDlg::UpdateUIFileInfoList( )
{ 
    m_ListFile->DeleteAllItems( );

    for (int index = 0; index <g_ComData.m_FileListTable.filelist_size( ); ++index)
    {
        //_T("文件名"), _T("SHA512"), _T("文件大小"), _T("创建时间"), _T("是否分享")
        CString Data1;
        Data1 =g_ComData.m_FileListTable.filelist(index).filename( ).c_str( );
        m_ListFile->InsertItem(0, Data1);

        CString Data2;
        Data2 =g_ComData.m_FileListTable.filelist(index).filesha512( ).c_str( );
        m_ListFile->SetItemText(0, 1, Data2);

        CString Data3;
        std::string ss = std::to_string(g_ComData.m_FileListTable.filelist(index).filetotalsize( ));
        Data3 = ss.c_str( );
        m_ListFile->SetItemText(0, 2, Data3);

        CString Data4;
        Data4 =g_ComData.m_FileListTable.filelist(index).filecreatedate( ).c_str( );
        m_ListFile->SetItemText(0, 3, Data4);

        CString Data5;
        Data5 =g_ComData.m_FileListTable.filelist(index).isshared( ).c_str( );
        m_ListFile->SetItemText(0, 4, Data5);

    }
}
void CQFileSystemDlg::WriteFileInfoList( )
{ 
    boost::filesystem::fstream readFile("FileInfoList", std::ios::trunc | std::ios::out | std::ios::binary);
    if (!readFile.is_open( ))
        return;

    if (!g_ComData.m_FileListTable.SerializePartialToOstream(&readFile));

    readFile.close( );
}

//分享列表
void CQFileSystemDlg::ReadSharedList( )
{ 
    boost::filesystem::fstream readFile("SharedInfoList", std::ios::in | std::ios::binary);
    if (!readFile.is_open( ))
        return;

    if (!g_ComData.m_SharedTable.ParsePartialFromIstream(&readFile));

    for (int index = 0; index <g_ComData.m_SharedTable.sharedinfo_size( ); ++index)
    {
        //_T("存储端编号"),_T("IP"),_T("剩余空间"),_T("总空间")
        CString Data1;
        Data1 =g_ComData.m_SharedTable.sharedinfo(index).sha512( ).c_str( );
        m_ListShared->InsertItem(0, Data1);

        CString Data2;
        Data2 =g_ComData.m_SharedTable.sharedinfo(index).verificationcode( ).c_str( );
        m_ListShared->SetItemText(0, 1, Data2);

        CString Data3;
        Data3 =g_ComData.m_SharedTable.sharedinfo(index).userid( ).c_str( );
        m_ListShared->SetItemText(0, 2, Data3);

        CString Data4;
        std::string ss1 = std::to_string(g_ComData.m_SharedTable.sharedinfo(index).filesize( ));
        Data4 = ss1.c_str( );
        m_ListShared->SetItemText(0, 3, Data4);
    }

    readFile.close( );
}
void CQFileSystemDlg::UpdateUISharedList( )
{ 
    m_ListShared->DeleteAllItems( );

    for (int index = 0; index <g_ComData.m_SharedTable.sharedinfo_size( ); ++index)
    {
        //_T("存储端编号"),_T("IP"),_T("剩余空间"),_T("总空间")
        CString Data1;
        Data1 =g_ComData.m_SharedTable.sharedinfo(index).sha512( ).c_str( );
        m_ListShared->InsertItem(0, Data1);

        CString Data2;
        Data2 =g_ComData.m_SharedTable.sharedinfo(index).verificationcode( ).c_str( );
        m_ListShared->SetItemText(0, 1, Data2);

        CString Data3;
        Data3 =g_ComData.m_SharedTable.sharedinfo(index).userid( ).c_str( );
        m_ListShared->SetItemText(0, 2, Data3);

        CString Data4;
        std::string ss1 = std::to_string(g_ComData.m_SharedTable.sharedinfo(index).filesize( ));
        Data4 = ss1.c_str( );
        m_ListShared->SetItemText(0, 3, Data4);
    }
}
void CQFileSystemDlg::WriteSharedList( )
{
    boost::filesystem::fstream readFile("SharedInfoList", std::ios::trunc | std::ios::out | std::ios::binary);
    if (!readFile.is_open( ))
        return;

    if (!g_ComData.m_SharedTable.SerializePartialToOstream(&readFile))
        ;

    readFile.close( );
}

//解析存储端信息
void CQFileSystemDlg::ReadSaveServerList( )
{ 
    boost::filesystem::fstream readClientFile("ClientTable", std::ios::in | std::ios::binary);
    if (!readClientFile.is_open( ))
        return;

    if (!g_ComData.m_ClientConfigFile.ParsePartialFromIstream(&readClientFile))
        ;

    readClientFile.close( );

    for (int index = 0; index <g_ComData.m_ClientConfigFile.clientinfo_size( ); ++index)
    {
        //_T("存储端编号"),_T("IP"),_T("剩余空间"),_T("总空间")
        CString Data1;
        Data1 =g_ComData.m_ClientConfigFile.clientinfo(index).cilentid( ).c_str( );
        m_ListSaveServer->InsertItem(0, Data1);

        CString Data2;
        Data2 =g_ComData.m_ClientConfigFile.clientinfo(index).saveip( ).c_str( );
        m_ListSaveServer->SetItemText(0, 1, Data2);

        CString Data3;
        std::string ss = std::to_string(g_ComData.m_ClientConfigFile.clientinfo(index).remainsize( ));
        Data3 = ss.c_str( );
        m_ListSaveServer->SetItemText(0, 2, Data3);

        CString Data4;
        std::string ss1 = std::to_string(g_ComData.m_ClientConfigFile.clientinfo(index).totalsize( ));
        Data4 = ss1.c_str( );
        m_ListSaveServer->SetItemText(0, 3, Data4);

        //默认设置为离线，客户端发送Heart是进行变动
        const std::string fff = "OFF";
        auto tt =g_ComData.m_ClientConfigFile.mutable_clientinfo(index);
        tt->set_online("OFF");          //更改Protoc数据
        m_ListSaveServer->SetItemText(0, 4, _T("OFF"));
    }
}
void CQFileSystemDlg::UpDateUISaveServerList( )
{
    m_ListSaveServer->DeleteAllItems( );

    for (int index = 0; index <g_ComData.m_ClientConfigFile.clientinfo_size( ); ++index)
    {
        //_T("存储端编号"),_T("IP"),_T("剩余空间"),_T("总空间")
        CString Data1;
        Data1 =g_ComData.m_ClientConfigFile.clientinfo(index).cilentid( ).c_str( );
        m_ListSaveServer->InsertItem(0, Data1);

        CString Data2;
        Data2 =g_ComData.m_ClientConfigFile.clientinfo(index).saveip( ).c_str( );
        m_ListSaveServer->SetItemText(0, 1, Data2);

        CString Data3;
        std::string ss = std::to_string(g_ComData.m_ClientConfigFile.clientinfo(index).remainsize( ));
        Data3 = ss.c_str( );
        m_ListSaveServer->SetItemText(0, 2, Data3);

        CString Data4;
        std::string ss1 = std::to_string(g_ComData.m_ClientConfigFile.clientinfo(index).totalsize( ));
        Data4 = ss1.c_str( );
        m_ListSaveServer->SetItemText(0, 3, Data4);

        CString Data5;
        Data5 =g_ComData.m_ClientConfigFile.clientinfo(index).online( ).c_str( );
        m_ListSaveServer->SetItemText(0, 4, Data5);
    }
}
void CQFileSystemDlg::WriteSaveServerList( )
{ 
    boost::filesystem::fstream readClientFile("ClientTable", std::ios::trunc | std::ios::out | std::ios::binary);
    if (!readClientFile.is_open( ))
        return;

    if (!g_ComData.m_ClientConfigFile.SerializePartialToOstream(&readClientFile))
        ;

    readClientFile.close( );
}

//载入用户列表
void CQFileSystemDlg::readUserInfo( )
{
    boost::filesystem::fstream readUserInfoFile("UserTable", std::ios::in | std::ios::binary);
    if (!readUserInfoFile.is_open( ))//return;
        MessageBox(_T("UserTable 文件打开失败！"));

    if (!g_ComData.m_UserhasFile.ParseFromIstream(&readUserInfoFile))
        MessageBox(_T("UserTable 文件解析失败！"));

    readUserInfoFile.close( );
}
void CQFileSystemDlg::WriteUserInfo( )
{
    boost::filesystem::fstream readUserInfoFile("UserTable", std::ios::trunc | std::ios::out | std::ios::binary);
    if (!readUserInfoFile.is_open( ))//return;
        MessageBox(_T("UserTable 文件打开失败！"));

    if (!g_ComData.m_UserhasFile.SerializePartialToOstream(&readUserInfoFile))
        MessageBox(_T("UserTable 文件解析失败！"));

    readUserInfoFile.close( );
}

