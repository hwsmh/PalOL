
// LauncherDlg.cpp : ʵ���ļ�
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CLauncherDlg �Ի���



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


// CLauncherDlg ��Ϣ�������

BOOL CLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_connectsuccess.SetRange(0, 100);
	PostMessage(WM_CONNECT_SERVER);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CLauncherDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//PlaySound("��������.mp3",NULL,SND_FILENAME|SND_ASYNC);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_client->close();
	CDialogEx::OnClose();
}
