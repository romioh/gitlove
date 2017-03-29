
// PDFtoJPGDlg.cpp : ���� ����
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
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CPDFtoJPGDlg ��ȭ ����



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


// CPDFtoJPGDlg �޽��� ó����

BOOL CPDFtoJPGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	initailize();


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPDFtoJPGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
        // ..�� ���� ���� �� ������ ������ �����ϼ���.
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

	m_lst_prelist.InsertColumn(0, _T("���ϸ�"), LVCFMT_LEFT, 200);
	m_lst_prelist.InsertColumn(1, _T("����"), LVCFMT_LEFT, 50);
	m_lst_prelist.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lst_prelist.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_lst_cmplist.InsertColumn(0, _T("���ϸ�"), LVCFMT_LEFT, 200);
	m_lst_cmplist.InsertColumn(1, _T("����"), LVCFMT_LEFT, 50);
	m_lst_cmplist.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_lst_cmplist.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CPDFtoJPGDlg::OnBnClickedButtonPdftojpg()
{
	if(!g_prefileapath.Compare(""))
	{
		MessageBox(_T("������ PDF���� ��θ� �����ϼ���."));
		return;
	}
	if(!g_cmpfileapath.Compare(""))
	{
		MessageBox(_T("����� JPG���� �����θ� �����ϼ���."));
		return;
	}

    CWinThread *p1 = NULL;
    p1 = AfxBeginThread(ThreadFirst, this);
 
    if (p1 == NULL)
    {
        MessageBox(_T("�ý��ۿ� ������ �ֽ��ϴ�. �����ڿ��� ���� �ϼ���"));
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
	bInfo.lpszTitle = _T("���丮�� �����ϼ���");
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
	bInfo.lpszTitle = _T("���丮�� �����ϼ���");
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
		// ���� �׸��� ���� ���°� ���õǾ��� ������ ���
		if (LVIS_SELECTED == m_lst_prelist.GetItemState(i, LVIS_SELECTED))
		{
			//D:\�����Կ���pdf 1\19570101-01-50-A_JPG.PDF
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
	MessageBox(_T("PDF�� JPG��ȯ�� �Ϸ�Ǿ����ϴ�."));
	flag = false;
	return 0;
}


void CPDFtoJPGDlg::OnBnClickedCancel()
{
	if(flag){
		if(IDYES==MessageBox(_T("���� ��ȯ�߿� �ֽ��ϴ�. �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO | MB_ICONWARNING)){	
			CDialogEx::OnCancel();
		}
	}
	else{
		CDialogEx::OnCancel();
	}
}

