// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HoldingHands.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

#include <windows.h>
#include <string>
#include <stdexcept>

std::string ToUtf8(const std::wstring& wstr) {
	if (wstr.empty()) return std::string();

	int size_needed = WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	if (size_needed <= 0) {
		throw std::runtime_error("WideCharToMultiByte size query failed");
	}

	std::string str(size_needed, '\0');

	int result = WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

	if (result == 0) {
		throw std::runtime_error("WideCharToMultiByte conversion failed");
	}

	str.pop_back();

	return str;
}

// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CConfig&config,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
	, m_config(config)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditPort);
	DDX_Control(pDX, IDC_EDIT3, m_EditMaxConnection);
	DDX_Control(pDX, IDC_CHECK1, m_BnOverwrite);
	DDX_Control(pDX, IDC_EDIT2, m_EditDesktopSavePath);
	DDX_Control(pDX, IDC_EDIT4, m_EditCameraSavePath);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序
void CSettingDlg::OnBnClickedOk()
{
	CString port, conn, cameraPath, desktopPath;
	m_EditPort.GetWindowTextW(port);
	m_EditMaxConnection.GetWindowTextW(conn);
	m_EditCameraSavePath.GetWindowTextW(cameraPath);
	m_EditDesktopSavePath.GetWindowTextW(desktopPath);
	int n = m_BnOverwrite.GetCheck();

	m_config.SetStr("server", "port", ToUtf8(port.GetString()));
	m_config.SetStr("server", "max_connection", ToUtf8(conn.GetString()));
	m_config.SetStr("camera", "screenshot_save_path", ToUtf8(cameraPath.GetString()));
	m_config.SetStr("remote_desktop", "screenshot_save_path", ToUtf8(desktopPath.GetString()));
	m_config.SetInt("file_transfer", "overwrite", n);

	CDialogEx::OnOK();
}


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditPort.SetWindowTextW(m_config.GetStr("server", "port", "10086"));
	m_EditMaxConnection.SetWindowTextW(m_config.GetStr("server", "max_connection", "10000"));
	m_BnOverwrite.SetCheck(m_config.GetInt("file_transfer","overwrite", 0));
	m_EditCameraSavePath.SetWindowTextW(m_config.GetStr("camera", "screenshot_save_path", "./camera"));
	m_EditDesktopSavePath.SetWindowTextW(m_config.GetStr("remote_desktop", "screenshot_save_path", "./remote_desktop"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
