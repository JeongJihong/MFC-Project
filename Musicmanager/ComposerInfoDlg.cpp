// ComposerInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Musicmanager.h"
#include "ComposerInfoDlg.h"
#include "afxdialogex.h"
#include "mysql.h"


// ComposerInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(ComposerInfoDlg, CDialogEx)

ComposerInfoDlg::ComposerInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ComposerInfo, pParent)
	, m_strId(_T(""))
	, m_strName(_T(""))
	, m_strHt(_T(""))
	, m_strAgency(_T(""))
	, m_strRs(_T(""))
	
{

}

ComposerInfoDlg::~ComposerInfoDlg()
{
}

void ComposerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl3);
	DDX_Text(pDX, IDC_EDIT1, m_strId);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strHt);
	DDX_Text(pDX, IDC_EDIT4, m_strAgency);
	DDX_Text(pDX, IDC_EDIT5, m_strRs);
}


BEGIN_MESSAGE_MAP(ComposerInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_INSERT, &ComposerInfoDlg::OnBnClickedBtnInsert)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &ComposerInfoDlg::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, &ComposerInfoDlg::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &ComposerInfoDlg::OnClickList3)
END_MESSAGE_MAP()


// ComposerInfoDlg 메시지 처리기


void ComposerInfoDlg::OnBnClickedBtnInsert()
{
	UpdateData(TRUE);
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	querystr.Format("Insert into Composer values ('%s','%s','%s','%s','%s')", m_strId, m_strName, m_strHt, m_strAgency, m_strRs);
	if (mysql_query(&mysql, querystr)) {
		AfxMessageBox(mysql_error(&mysql));
	}
	mysql_close(&mysql);
	SetDataToList();
}


void ComposerInfoDlg::OnBnClickedBtnUpdate()
{
	UpdateData(TRUE);
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	querystr.Format("update Composer set C_Name = '%s', C_Hometown = '%s', C_Agency = '%s', C_song = '%s' where C_Id = '%s'",
		m_strName, m_strHt, m_strAgency, m_strRs, m_strId);
	mysql_query(&mysql, querystr);

	mysql_close(&mysql);
	SetDataToList();
}


void ComposerInfoDlg::OnBnClickedBtnDelete()
{
	POSITION pos = m_ListCtrl3.GetFirstSelectedItemPosition();
	if (!pos)	return;

	// DB 상에서 Delete
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	while (pos)
	{
		int nItem = m_ListCtrl3.GetNextSelectedItem(pos);
		CString str = m_ListCtrl3.GetItemText(nItem, 0);
		querystr.Format("delete from Composer where C_Id = '%s'", str);
		mysql_query(&mysql, querystr);
	}

	mysql_close(&mysql);
	SetDataToList();
}


BOOL ComposerInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rt;
	m_ListCtrl3.GetClientRect(&rt);

	m_ListCtrl3.SetExtendedStyle(m_ListCtrl3.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl3.InsertColumn(0, "ID", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl3.InsertColumn(1, "Name", LVCFMT_CENTER, (int)(rt.Width() * 0.2));
	m_ListCtrl3.InsertColumn(2, "Hometown", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl3.InsertColumn(3, "Agency", LVCFMT_CENTER, (int)(rt.Width() * 0.17));
	m_ListCtrl3.InsertColumn(4, "Representative Song", LVCFMT_CENTER, (int)(rt.Width() * 0.35));

	// SQL에서 데이터 가져와서 출력
	UpdateData(TRUE);
	SetDataToList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ComposerInfoDlg::SetDataToList()
{
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	CString querystr = "select C_ID, C_Name,C_Hometown,C_Agency,C_song from composer order by C_Name";
	mysql_query(&mysql, querystr);

	MYSQL_RES* result = mysql_store_result(&mysql);

	if (result)
	{
		m_ListCtrl3.DeleteAllItems();
		MYSQL_ROW row;
		int idx = 0;
		CString str;
		while (row = mysql_fetch_row(result)) {
			m_ListCtrl3.InsertItem(idx, row[0]);
			for (int i = 1; i <= 4; i++) {
				m_ListCtrl3.SetItemText(idx, i, row[i]);
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

void ComposerInfoDlg::OnClickList3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int idx = pNMItemActivate->iItem;

	m_strId = m_ListCtrl3.GetItemText(idx, 0);
	m_strName = m_ListCtrl3.GetItemText(idx, 1);
	m_strHt = m_ListCtrl3.GetItemText(idx, 2);
	m_strAgency = m_ListCtrl3.GetItemText(idx, 3);
	m_strRs = m_ListCtrl3.GetItemText(idx, 4);

	UpdateData(FALSE);

	*pResult = 0;
}

