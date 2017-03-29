
// PDFtoJPGDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPDFtoJPGDlg ��ȭ ����
class CPDFtoJPGDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CPDFtoJPGDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	static UINT ThreadFirst(LPVOID _mothod);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PDFTOJPG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lst_prelist;
	CListCtrl m_lst_cmplist;
	CEdit m_edt_prepath;
	CEdit m_edt_cmppath;
	CStatic m_lbl_precnt;
	CStatic m_lbl_cmpcnt;
	afx_msg void OnBnClickedButtonPdftojpg();
	void initailize(void);
	afx_msg void OnBnClickedButtonPrepath();
	afx_msg void OnBnClickedButtonCmppath();
	int pdf2jpg(void);
	CProgressCtrl m_gpr_complete;
	CStatic m_total_progress;
	CStatic m_process_chk;
	afx_msg void OnBnClickedCancel();
};
