// SingerInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Musicmanager.h"
#include "SingerInfoDlg.h"
#include "afxdialogex.h"
#include "mysql.h"


// SingerInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(SingerInfoDlg, CDialogEx)

SingerInfoDlg::SingerInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SingerInfo, pParent)
	, m_strName(_T(""))
	, m_strDd(_T(""))
	, m_strSt(_T(""))
	, m_strRs(_T(""))
	, m_strId(_T(""))
{

}

SingerInfoDlg::~SingerInfoDlg()
{
}

void SingerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
	//  DDX_Control(pDX, IDC_EDIT1, m_strId);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strDd);
	DDX_Text(pDX, IDC_EDIT4, m_strSt);
	DDX_Text(pDX, IDC_EDIT5, m_strRs);
	DDX_Text(pDX, IDC_EDIT1, m_strId);
}


BEGIN_MESSAGE_MAP(SingerInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_INSERT, &SingerInfoDlg::OnBnClickedBtnInsert)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &SingerInfoDlg::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, &SingerInfoDlg::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &SingerInfoDlg::OnClickList2)
END_MESSAGE_MAP()


// SingerInfoDlg 메시지 처리기


void SingerInfoDlg::OnBnClickedBtnInsert()
{
	UpdateData(TRUE);
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	querystr.Format("Insert into Singer values ('%s','%s','%s','%s','%s')", m_strId, m_strName, m_strDd, m_strSt, m_strRs);
	if (mysql_query(&mysql, querystr)) {
		AfxMessageBox(mysql_error(&mysql));
	}
	mysql_close(&mysql);
	SetDataToList();
}


void SingerInfoDlg::OnBnClickedBtnUpdate()
{
	UpdateData(TRUE);
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	querystr.Format("update Singer set S_Name = '%s',S_Year = '%s',S_Type = '%s',S_Song = '%s' where S_Id = '%s'",
		m_strName, m_strDd, m_strSt, m_strRs, m_strId);
	mysql_query(&mysql, querystr);

	mysql_close(&mysql);
	SetDataToList();
}


void SingerInfoDlg::OnBnClickedBtnDelete()
{
	POSITION pos = m_ListCtrl2.GetFirstSelectedItemPosition();
	if (!pos)	return;

	// DB 상에서 Delete
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	while (pos)
	{
		int nItem = m_ListCtrl2.GetNextSelectedItem(pos);
		CString str = m_ListCtrl2.GetItemText(nItem, 0);
		querystr.Format("delete from Singer where S_Id = '%s'", str);
		mysql_query(&mysql, querystr);
	}

	mysql_close(&mysql);
	SetDataToList();
}


BOOL SingerInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rt;
	m_ListCtrl2.GetClientRect(&rt);

	m_ListCtrl2.SetExtendedStyle(m_ListCtrl2.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl2.InsertColumn(0, "ID", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl2.InsertColumn(1, "Name", LVCFMT_CENTER, (int)(rt.Width() * 0.2));
	m_ListCtrl2.InsertColumn(2, "Debut Date", LVCFMT_CENTER, (int)(rt.Width() * 0.2));
	m_ListCtrl2.InsertColumn(3, "Singer Type", LVCFMT_CENTER, (int)(rt.Width() * 0.17));
	m_ListCtrl2.InsertColumn(4, "Representative Song", LVCFMT_CENTER, (int)(rt.Width() * 0.23));

	// SQL에서 데이터 가져와서 출력
	UpdateData(TRUE);
	SetDataToList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void SingerInfoDlg::SetDataToList()
{
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	CString querystr = "select S_Id,S_Name,S_Year,S_Type,S_Song from singer order by S_Name";
	mysql_query(&mysql, querystr);

	MYSQL_RES* result = mysql_store_result(&mysql);

	if (result)
	{
		m_ListCtrl2.DeleteAllItems();
		MYSQL_ROW row;
		int idx = 0;
		CString str;
		while (row = mysql_fetch_row(result)) {
			m_ListCtrl2.InsertItem(idx, row[0]);
			for (int i = 1; i <= 4; i++) {
				m_ListCtrl2.SetItemText(idx, i, row[i]);
			}
			idx++;
		}

		mysql_free_result(result);
	}
	else
	{
		AfxMessageBox("No Result!");
	}
	mysql_close(&mysql);
}

void SingerInfoDlg::OnClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int idx = pNMItemActivate->iItem;

	m_strId = m_ListCtrl2.GetItemText(idx, 0);
	m_strName = m_ListCtrl2.GetItemText(idx, 1);
	m_strDd = m_ListCtrl2.GetItemText(idx, 2);
	m_strSt = m_ListCtrl2.GetItemText(idx, 3);
	m_strRs = m_ListCtrl2.GetItemText(idx, 4);

	UpdateData(FALSE);

	*pResult = 0;
}
