#pragma once


// SingerInfoDlg 대화 상자

class SingerInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SingerInfoDlg)

public:
	SingerInfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SingerInfoDlg();
	void SetDataToList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SingerInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl2;
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnDelete();
	virtual BOOL OnInitDialog();
//	CEdit m_strId;
	CString m_strName;
	CString m_strDd;
	CString m_strSt;
	CString m_strRs;
	CString m_strId;
	afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);
};
