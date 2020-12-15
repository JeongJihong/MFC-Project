#pragma once


// MusicInfoDlg 대화 상자

class MusicInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MusicInfoDlg)

public:
	MusicInfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MusicInfoDlg();
	void SetDataToList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MusicInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnDelete();
	CListCtrl m_ListCtrl1;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	CString m_strId;
	CString m_strName;
	CString m_strSinger;
	CString m_strComposer;
	CString m_strRd;
	CString m_strGenre;
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
};
