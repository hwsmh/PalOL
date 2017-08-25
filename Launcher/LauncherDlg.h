
// LauncherDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "AuthClient.h"

using namespace tcpclient;
 
// CLauncherDlg 对话框
class CLauncherDlg : public CDialogEx
{
// 构造
public:
	CLauncherDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
