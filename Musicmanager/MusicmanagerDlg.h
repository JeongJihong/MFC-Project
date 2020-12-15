
// MusicmanagerDlg.h: 헤더 파일
//

#pragma once
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "ws2_32.lib")
#include <mysql.h>
#include <WinSock2.h>

// CMusicmanagerDlg 대화 상자
class CMusicmanagerDlg : public CDialogEx
{
	// 생성입니다.
public:
	CMusicmanagerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICMANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;
	MYSQL mysql;
	bool ConnectSQL();

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_ListCtrl;
	afx_msg void OnBnClickedOk();
	CString m_strMusic;
	CString m_strSinger;
	CString m_strComposer;
	afx_msg void OnBnClickedBtnMusicinfo();
	afx_msg void OnBnClickedBtnComposerinfo();
	afx_msg void OnBnClickedBtnSingerinfo();

	CComboBox m_ctrlCombo;
	afx_msg void OnBnClickedBtnSearch();
	CString m_strSearch;
};
