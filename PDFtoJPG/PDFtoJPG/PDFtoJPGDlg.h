
// PDFtoJPGDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPDFtoJPGDlg 대화 상자
class CPDFtoJPGDlg : public CDialogEx
{
// 생성입니다.
public:
	CPDFtoJPGDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	static UINT ThreadFirst(LPVOID _mothod);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PDFTOJPG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
