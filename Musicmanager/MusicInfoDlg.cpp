// MusicInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Musicmanager.h"
#include "MusicInfoDlg.h"
#include "afxdialogex.h"
#include "mysql.h"

// MusicInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(MusicInfoDlg, CDialogEx)


MusicInfoDlg::MusicInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MusicInfo, pParent)
	, m_strId(_T(""))
	, m_strName(_T(""))
	, m_strSinger(_T(""))
	, m_strComposer(_T(""))
	, m_strRd(_T(""))
	, m_strGenre(_T(""))
{
}

MusicInfoDlg::~MusicInfoDlg()
{
}

void MusicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl1);
	DDX_Text(pDX, IDC_EDIT1, m_strId);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strSinger);
	DDX_Text(pDX, IDC_EDIT4, m_strComposer);
	DDX_Text(pDX, IDC_EDIT5, m_strRd);
	DDX_Text(pDX, IDC_EDIT6, m_strGenre);
}


BEGIN_MESSAGE_MAP(MusicInfoDlg, CDialogEx)
	
	
	ON_BN_CLICKED(IDC_BTN_INSERT, &MusicInfoDlg::OnBnClickedBtnInsert)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &MusicInfoDlg::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, &MusicInfoDlg::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &MusicInfoDlg::OnClickList1)
END_MESSAGE_MAP()


BOOL MusicInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// title 생성
	CRect rt;
	m_ListCtrl1.GetClientRect(&rt);

	m_ListCtrl1.SetExtendedStyle(m_ListCtrl1.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl1.InsertColumn(0, "ID", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl1.InsertColumn(1, "Name", LVCFMT_CENTER, (int)(rt.Width() * 0.2));
	m_ListCtrl1.InsertColumn(2, "Singer", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl1.InsertColumn(3, "Composer", LVCFMT_CENTER, (int)(rt.Width() * 0.17));
	m_ListCtrl1.InsertColumn(4, "Realease Date", LVCFMT_CENTER, (int)(rt.Width() * 0.23));
	m_ListCtrl1.InsertColumn(5, "Genre", LVCFMT_CENTER, (int)(rt.Width() * 0.18));

	// SQL에서 데이터 가져와서 출력
	UpdateData(TRUE);
	SetDataToList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// MusicInfoDlg 메시지 처리기

void MusicInfoDlg::OnBnClickedBtnInsert()
{	
	UpdateData(TRUE);
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	querystr.Format("insert into Music values ('%s', '%s', '%s', '%s', '%s', '%s')", m_strId, m_strName, m_strSinger, m_strComposer, m_strRd, m_strGenre);
	
	if (mysql_query(&mysql, querystr)) {
		AfxMessageBox(mysql_error(&mysql));
	}
	
	mysql_close(&mysql);
	SetDataToList();
}


void MusicInfoDlg::OnBnClickedBtnUpdate()
{
	UpdateData(TRUE);
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	querystr.Format("update Music set M_Name = '%s', S_Id = '%s', C_Id = '%s', M_Year = '%s', M_Genre = '%s' where M_Id = '%s'",
		m_strName, m_strSinger, m_strComposer, m_strRd, m_strGenre, m_strId);
	mysql_query(&mysql, querystr);

	mysql_close(&mysql);
	SetDataToList();
}


void MusicInfoDlg::OnBnClickedBtnDelete()
{
	POSITION pos = m_ListCtrl1.GetFirstSelectedItemPosition();
	if (!pos)	return;

	// DB 상에서 Delete
	MYSQL mysql;
	CString querystr;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);

	while (pos)
	{
		int nItem = m_ListCtrl1.GetNextSelectedItem(pos);
		CString str = m_ListCtrl1.GetItemText(nItem, 0);
		querystr.Format("delete from Music where M_Id = '%s'", str);
		mysql_query(&mysql, querystr);
	}

	mysql_close(&mysql);
	SetDataToList();
}


void MusicInfoDlg::SetDataToList()
{
	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	CString querystr = "select M_Id,M_Name,S_Id,C_Id,M_Year,M_Genre from Music order by M_Name";
	mysql_query(&mysql, querystr);

	MYSQL_RES* result = mysql_store_result(&mysql);

	if (result)
	{
		m_ListCtrl1.DeleteAllItems();
		MYSQL_ROW row;
		int idx = 0;
		CString str;
		while (row = mysql_fetch_row(result)) {
			m_ListCtrl1.InsertItem(idx, row[0]);
			for (int i = 1; i <= 5; i++) {
				m_ListCtrl1.SetItemText(idx, i, row[i]);
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

void MusicInfoDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int idx = pNMItemActivate->iItem;

	m_strId = m_ListCtrl1.GetItemText(idx, 0);
	m_strName = m_ListCtrl1.GetItemText(idx, 1);
	m_strSinger = m_ListCtrl1.GetItemText(idx, 2);
	m_strComposer = m_ListCtrl1.GetItemText(idx, 3);
	m_strRd = m_ListCtrl1.GetItemText(idx, 4);
	m_strGenre = m_ListCtrl1.GetItemText(idx, 5);

	UpdateData(FALSE);

	*pResult = 0;
}
