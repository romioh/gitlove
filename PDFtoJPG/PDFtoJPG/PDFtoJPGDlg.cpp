
// PDFtoJPGDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "PDFtoJPG.h"
#include "PDFtoJPGDlg.h"
#include "afxdialogex.h"

#include "Magick++.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Magick;

CString g_prefileapath;
CString g_cmpfileapath;
int g_precount;
int g_cmpcount;
int countPercent = 0;
BOOLEAN flag = false;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPDFtoJPGDlg 대화 상자



CPDFtoJPGDlg::CPDFtoJPGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPDFtoJPGDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPDFtoJPGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRELIST, m_lst_prelist);
	DDX_Control(pDX, IDC_LIST_CMPLIST, m_lst_cmplist);
	DDX_Control(pDX, IDC_EDIT_PREPATH, m_edt_prepath);
	DDX_Control(pDX, IDC_EDIT_CMPPATH, m_edt_cmppath);
	DDX_Control(pDX, IDC_STATIC_PRECNT, m_lbl_precnt);
	DDX_Control(pDX, IDC_STATIC_CMPCNT, m_lbl_cmpcnt);
	DDX_Control(pDX, IDC_PROGRESS_CMP, m_gpr_complete);
	DDX_Control(pDX, IDC_STATIC_COMPRESSRATE, m_total_progress);
	DDX_Control(pDX, IDC_STATIC_COMPRESSFILE, m_process_chk);
}

BEGIN_MESSAGE_MAP(CPDFtoJPGDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PDFTOJPG, &CPDFtoJPGDlg::OnBnClickedButtonPdftojpg)
	ON_BN_CLICKED(IDC_BUTTON_PREPATH, &CPDFtoJPGDlg::OnBnClickedButtonPrepath)
	ON_BN_CLICKED(IDC_BUTTON_CMPPATH, &CPDFtoJPGDlg::OnBnClickedButtonCmppath)
	ON_BN_CLICKED(IDCANCEL, &CPDFtoJPGDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPDFtoJPGDlg 메시지 처리기

BOOL CPDFtoJPGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	initailize();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPDFtoJPGDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPDFtoJPGDlg::OnPaint()
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
HCURSOR CPDFtoJPGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CPDFtoJPGDlg::ThreadFirst(LPVOID _mothod)                                       
{
    CPDFtoJPGDlg *fir = (CPDFtoJPGDlg*)_mothod;
	CPDFtoJPGDlg* pDlg = (CPDFtoJPGDlg*)AfxGetApp()->m_pMainWnd;


    while (1)
    {
        // ..이 곳에 루프 시 수행할 동작을 선언하세요.
        //Sleep(1000);
		if(pDlg->pdf2jpg() == 0)
			break;
    }
    return 0;
}

void CPDFtoJPGDlg::initailize(void)
{
	g_precount = 0;
	g_cmpcount = 0;

	g_cmpfileapath = _T("");
	g_prefileapath = _T("");

	m_lst_prelist.InsertColumn(0, _T("파일명"), LVCFMT_LEFT, 200);
	m_lst_prelist.InsertColumn(1, _T("형식"), LVCFMT_LEFT, 50);
	m_lst_prelist.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lst_prelist.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_lst_cmplist.InsertColumn(0, _T("파일명"), LVCFMT_LEFT, 200);
	m_lst_cmplist.InsertColumn(1, _T("형식"), LVCFMT_LEFT, 50);
	m_lst_cmplist.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lst_cmplist.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CPDFtoJPGDlg::OnBnClickedButtonPdftojpg()
{
	if(!g_prefileapath.Compare(""))
	{
		MessageBox(_T("변경할 PDF파일 경로를 선택하세요."));
		return;
	}
	if(!g_cmpfileapath.Compare(""))
	{
		MessageBox(_T("변경된 JPG파일 저장경로를 선택하세요."));
		return;
	}

    CWinThread *p1 = NULL;
    p1 = AfxBeginThread(ThreadFirst, this);
 
    if (p1 == NULL)
    {
        MessageBox(_T("시스템에 문제가 있습니다. 관리자에게 문의 하세요"));
    }
}


void CPDFtoJPGDlg::OnBnClickedButtonPrepath()
{
	CString strFindpath;
	CString tmpFileName;
	CString tmpcnt;
	ITEMIDLIST*  pildBrowse;
	TCHAR   pszPathname[MAX_PATH];

	CFileFind finder;
	BROWSEINFO  bInfo;
	memset(&bInfo, 0, sizeof(bInfo));
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle = _T("디렉토리를 선택하세요");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = NULL;
	bInfo.lParam = (LPARAM)(LPCTSTR)"C:\\";
	bInfo.lParam = (LPARAM)NULL;
	pildBrowse = ::SHBrowseForFolder(&bInfo);
	if (pildBrowse)
	{
		m_lst_prelist.DeleteAllItems();
		SHGetPathFromIDList(pildBrowse, pszPathname);
	}
	else
	{
		return;
	}

	g_prefileapath = _T("");
	m_edt_prepath.SetWindowText(g_prefileapath);
	g_prefileapath.Append(pszPathname);
	m_edt_prepath.SetWindowText(g_prefileapath);
	strFindpath.Append(pszPathname);
	strFindpath.Append(_T("\\*.pdf"));

	BOOL bWorking = finder.FindFile(strFindpath);
	int countPdf =0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsArchived())
		{
			CString _fileName = finder.GetFileName();
			long size = finder.GetLength();
			if (_fileName == _T(".") || _fileName == _T("..") || _fileName == _T("Thumbs.db"))
				continue;
			tmpFileName = finder.GetFileTitle();
			m_lst_prelist.InsertItem(countPdf, tmpFileName, 0);
			m_lst_prelist.SetItemText(countPdf, 1, _T("PDF"));
			countPdf++;
		}
	}
	tmpcnt.Format(_T("Total: %d"), countPdf);
	m_gpr_complete.SetRange(0, countPdf);
	m_gpr_complete.SetStep(1);
	m_lbl_precnt.SetWindowText(tmpcnt);
}


void CPDFtoJPGDlg::OnBnClickedButtonCmppath()
{
	CString strFindpath;
	CString tmpFileName;
	CString tmpcnt;
	ITEMIDLIST*  pildBrowse;
	TCHAR   pszPathname[MAX_PATH];

	CFileFind finder;
	BROWSEINFO  bInfo;
	memset(&bInfo, 0, sizeof(bInfo));
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.pidlRoot = NULL;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle = _T("디렉토리를 선택하세요");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = NULL;
	bInfo.lParam = (LPARAM)(LPCTSTR)"C:\\";
	bInfo.lParam = (LPARAM)NULL;
	pildBrowse = ::SHBrowseForFolder(&bInfo);
	if (pildBrowse)
	{
		m_lst_cmplist.DeleteAllItems();
		SHGetPathFromIDList(pildBrowse, pszPathname);
	}
	else
	{
		return;
	}

	g_cmpfileapath = _T("");
	m_edt_cmppath.SetWindowText(g_cmpfileapath);
	g_cmpfileapath.Append(pszPathname);
	m_edt_cmppath.SetWindowText(g_cmpfileapath);
	strFindpath.Append(pszPathname);
}


int CPDFtoJPGDlg::pdf2jpg(void)
{
	flag = true;
	InitializeMagick(".\\ImageMagick-7.0.4-Q16");
	//InitializeMagick("C:\\Program Files (x86)\\ImageMagick-7.0.4-Q16");
	CString tmpcnt;
	CString tmpPercent;
	float percent;
	Image image;
	int index = 0;
	int count = 0;
	int i;
	
	m_lst_cmplist.DeleteAllItems();
	m_lst_prelist.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
	count = m_lst_prelist.GetItemCount();
	for (int i = 0; i < count; i++)
	{
		// 개별 항목의 현재 상태가 선택되어진 상태일 경우
		if (LVIS_SELECTED == m_lst_prelist.GetItemState(i, LVIS_SELECTED))
		{
			//D:\사진촬영용pdf 1\19570101-01-50-A_JPG.PDF
			CString tmppath = _T("");
			CString fullpath = _T("");
			tmppath = m_lst_prelist.GetItemText(i, 0);
			fullpath.Append(g_prefileapath);
			fullpath.Append(_T("\\"));
			fullpath.Append(tmppath);
			fullpath.Append(_T(".pdf"));
			std::string s((LPCTSTR)fullpath);
			image.read(s);

			image.density("300x300");
			image.resize("2000x2000");

			fullpath = _T("");
			fullpath.Append(g_cmpfileapath);
			fullpath.Append(_T("\\"));
			fullpath.Append(tmppath);
			fullpath.Append(_T("-p1.jpg"));
			std::string r((LPCTSTR)fullpath);
			image.write(r);
			m_lst_cmplist.InsertItem(i, tmppath, 0);
			m_lst_cmplist.SetItemText(i, 1, _T("JPG"));
			tmpcnt.Format(_T("Total: %d"), i+1);
			m_lbl_cmpcnt.SetWindowText(tmpcnt);
			m_gpr_complete.StepIt();
			countPercent++;
			
		}
		m_process_chk.SetWindowText("Converting...");
		
		tmpPercent.Format(_T("%d / %d..."),countPercent,count);
		m_total_progress.SetWindowText(tmpPercent);
	}
	m_process_chk.SetWindowText("Complete");
	MessageBox(_T("PDF의 JPG변환이 완료되었습니다."));
	flag = false;
	return 0;
}


void CPDFtoJPGDlg::OnBnClickedCancel()
{
	if(flag){
		if(IDYES==MessageBox(_T("파일 변환중에 있습니다. 종료하시겠습니까?"), _T("종료 확인"), MB_YESNO | MB_ICONWARNING)){	
			CDialogEx::OnCancel();
		}
	}
	else{
		CDialogEx::OnCancel();
	}
}

