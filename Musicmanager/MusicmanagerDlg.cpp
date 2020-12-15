
// MusicmanagerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Musicmanager.h"
#include "MusicmanagerDlg.h"
#include "afxdialogex.h"
#include "mysql.h"
#include "MusicInfoDlg.h"
#include "SingerInfoDlg.h"
#include "ComposerInfoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMusicmanagerDlg 대화 상자



CMusicmanagerDlg::CMusicmanagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MUSICMANAGER_DIALOG, pParent)
	, m_strSearch(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMusicmanagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlCombo);
	DDX_Text(pDX, IDC_EDIT1, m_strSearch);
}

BEGIN_MESSAGE_MAP(CMusicmanagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CMusicmanagerDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDOK, &CMusicmanagerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MusicInfo, &CMusicmanagerDlg::OnBnClickedBtnMusicinfo)
	ON_BN_CLICKED(IDC_BTN_ComposerInfo, &CMusicmanagerDlg::OnBnClickedBtnComposerinfo)
	ON_BN_CLICKED(IDC_BTN_SingerInfo, &CMusicmanagerDlg::OnBnClickedBtnSingerinfo)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CMusicmanagerDlg::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// CMusicmanagerDlg 메시지 처리기

BOOL CMusicmanagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	CRect rt;
	m_ListCtrl.GetClientRect(&rt);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.InsertColumn(0, "ID", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl.InsertColumn(1, "Name", LVCFMT_CENTER, (int)(rt.Width() * 0.2));
	m_ListCtrl.InsertColumn(2, "Singer", LVCFMT_CENTER, (int)(rt.Width() * 0.14));
	m_ListCtrl.InsertColumn(3, "Composer", LVCFMT_CENTER, (int)(rt.Width() * 0.17));
	m_ListCtrl.InsertColumn(4, "Realease Date", LVCFMT_CENTER, (int)(rt.Width() * 0.23));
	m_ListCtrl.InsertColumn(5, "Genre", LVCFMT_CENTER, (int)(rt.Width() * 0.18));

	m_ctrlCombo.AddString("ID");
	m_ctrlCombo.AddString("Name");
	m_ctrlCombo.AddString("Singer");
	m_ctrlCombo.AddString("Composer");
	m_ctrlCombo.SetCurSel(1);

	// SQL에서 데이터 가져와서 출력
	UpdateData(TRUE);

	MYSQL mysql;
	mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	CString querystr = "select M_Id, M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id	left join composer on music.C_Id = composer.C_Id order by M_Name";
	mysql_query(&mysql, querystr);

	MYSQL_RES* result = mysql_store_result(&mysql);

	if (result)
	{
		MYSQL_ROW row;
		int idx = 0;
		CString str;
		while (row = mysql_fetch_row(result)) {
			m_ListCtrl.InsertItem(idx, row[0]);
			for (int i = 1; i <= 5; i++) {
				m_ListCtrl.SetItemText(idx, i, row[i]);
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMusicmanagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMusicmanagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMusicmanagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMusicmanagerDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


bool CMusicmanagerDlg::ConnectSQL()
{
	mysql_init(&mysql);		//mysql 초기화
	if (mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0))
		return true;
	return false;
}


void CMusicmanagerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}



void CMusicmanagerDlg::OnBnClickedBtnMusicinfo()
{
	MusicInfoDlg dlg;
	dlg.DoModal();
}


void CMusicmanagerDlg::OnBnClickedBtnSingerinfo()
{
	SingerInfoDlg dlg;
	dlg.DoModal();
}


void CMusicmanagerDlg::OnBnClickedBtnComposerinfo()
{
	ComposerInfoDlg dlg;
	dlg.DoModal();
}


void CMusicmanagerDlg::OnBnClickedBtnSearch()
{
	UpdateData(TRUE);
	m_ListCtrl.DeleteAllItems();

	if (!ConnectSQL()) {
		AfxMessageBox("Fail to connect DB");
		return;
	}
	CString querystr = "select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id	left join composer on music.C_Id = composer.C_Id";	// search edit에 아무것도 입력하지 않았을 경우

	if (m_strSearch.GetLength() != 0)
	{
		if (m_ctrlCombo.GetCurSel() == 0) {			// ID로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id " 
				"left join composer on music.C_Id = composer.C_Id where M_ID like '%%%s%%'", m_strSearch);
		}
		else if (m_ctrlCombo.GetCurSel() == 1) {	// Name으로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
				"left join composer on music.C_Id = composer.C_Id where M_Name like '%%%s%%'", m_strSearch);
		}
		else if (m_ctrlCombo.GetCurSel() == 2) {	// Singer로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
				"left join composer on music.C_Id = composer.C_Id where S_Name like '%%%s%%'", m_strSearch);
		}
		else if (m_ctrlCombo.GetCurSel() == 3) {	// Composer로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
				"left join composer on music.C_Id = composer.C_Id where C_Name like '%%%s%%'", m_strSearch);
		}
	}

	mysql_query(&mysql, querystr);

	MYSQL_RES* result = mysql_store_result(&mysql);
	if (!result)
	{
		AfxMessageBox("No Result!");
		return;
	}
	MYSQL_ROW row;
	int idx = 0;
	CString str;
	while (row = mysql_fetch_row(result)) {
		m_ListCtrl.InsertItem(idx, row[0]);
		for (int i = 1; i <= 5; i++) {
			m_ListCtrl.SetItemText(idx, i, row[i]);
		}
		idx++;
	}

	mysql_free_result(result);
	mysql_close(&mysql);
}
