
// LauncherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "AuthClient.h"

using namespace tcpclient;
 
// CLauncherDlg �Ի���
class CLauncherDlg : public CDialogEx
{
// ����
public:
	CLauncherDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnConnectSucess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnectServer(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartGame(WPARAM wParam, LPARAM lParam);
	CProgressCtrl m_connectsuccess;
	std::shared_ptr<auth_client>m_client;
	io_service m_ioservice;
	io_service::work m_work;
	afx_msg void OnClose();
	
};
