
// LauncherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "afxdialogex.h"

#include <thread>
#include "../Common/AuthCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



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


// CLauncherDlg 对话框



CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LAUNCHER_DIALOG, pParent),
	m_work(m_ioservice)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_connectsuccess);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CLauncherDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_CONNECT_SUCCESS,&CLauncherDlg::OnConnectSucess)
	ON_MESSAGE(WM_CONNECT_SERVER, &CLauncherDlg::OnConnectServer)
	ON_MESSAGE(WM_START_GAME, &CLauncherDlg::OnStartGame)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLauncherDlg 消息处理程序

BOOL CLauncherDlg::OnInitDialog()
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
	
	// TODO: 在此添加额外的初始化代码

	m_connectsuccess.SetRange(0, 100);
	PostMessage(WM_CONNECT_SERVER);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLauncherDlg::OnPaint()
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
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CLauncherDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strUserName, strPassWord;
	
	GetDlgItemText(IDC_EDIT1,strUserName);
	GetDlgItemText(IDC_EDIT2,strPassWord);

	char* pbuffer = m_client->get_write_buffer();
	((PPACKET_CMD)pbuffer)->dwCmd = Cmd_Login;
	int size = ((PPACKET_CMD)pbuffer)->dwSize = sizeof(CMD_LOGIN);

	pbuffer += sizeof(PACKET_CMD);

	lstrcpy((LPSTR)((PCMD_LOGIN)pbuffer)->username,strUserName.GetString());
	lstrcpy((LPSTR)((PCMD_LOGIN)pbuffer)->password, strPassWord.GetString());

	m_client->send();

	


	
}

LRESULT CLauncherDlg::OnStartGame(WPARAM wParam, LPARAM lParam)
{
 	STARTUPINFO StartupInfo = { 0 };
 	PROCESS_INFORMATION ProcessInformation = { 0 };
 
 	StartupInfo.cb = sizeof(STARTUPINFO);
 
 	BOOL IsRet = ::CreateProcess(R"(D:\CodeProject\PalOL\Debug\GAMe.exe)",
 		"hwsmh|damengge",
 		NULL,
 		NULL,
 		FALSE,
 		CREATE_NEW_CONSOLE,
 		NULL,
 		NULL,
 		&StartupInfo,
 		&ProcessInformation);
	//PlaySound("花花世界.mp3",NULL,SND_FILENAME|SND_ASYNC);
	PostMessage(WM_CLOSE);
	return 0;
}


LRESULT CLauncherDlg::OnConnectSucess(WPARAM wParam, LPARAM lParam)
{
	m_connectsuccess.SetPos(100);
	return 0;
}

LRESULT CLauncherDlg::OnConnectServer(WPARAM wParam, LPARAM lParam)
{
	tcp::resolver resolver(m_ioservice);
	tcp::resolver::query query("127.0.0.1","8888");
	tcp::resolver::iterator iterator = resolver.resolve(query);

	m_client = std::make_shared<auth_client>(m_ioservice, iterator);
	
	std::thread t(boost::bind(&io_service::run,&m_ioservice));

	t.detach();

	return 0;
}



void CLauncherDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_client->close();
	CDialogEx::OnClose();
}
