#pragma once


// ComposerInfoDlg 대화 상자

class ComposerInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ComposerInfoDlg)

public:
	ComposerInfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ComposerInfoDlg();
	void SetDataToList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ComposerInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl3;
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnDelete();
	virtual BOOL OnInitDialog();
	CString m_strId;
	CString m_strName;
	CString m_strHt;
	CString m_strAgency;
//	CString m_strRd;
	CString m_strRs;
	afx_msg void OnClickList3(NMHDR* pNMHDR, LRESULT* pResult);
};
