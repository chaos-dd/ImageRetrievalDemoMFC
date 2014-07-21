
// ImgRetrDlg.h : 头文件
//


#ifndef IMGRETRDLG_HHH
#define IMGRETRDLG_HHH
//#pragma once
#include "afxcmn.h"

#include "resource.h"

//#include "Search.h"
//#include "DatabaseCtrl.h"





class CSearch;
class CDatabaseCtrl;
class SearchEngine;

// CImgRetrDlg 对话框
class CImgRetrDlg : public CDialogEx
{
// 构造
public:
	CImgRetrDlg(CWnd* pParent = NULL);	// 标准构造函数
	CImgRetrDlg::~CImgRetrDlg();

// 对话框数据
	enum { IDD = IDD_IMGRETR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	CTabCtrl m_tab1;
	CStatusBarCtrl m_statusBar;

	CSearch *m_searchCtrl;
	CDatabaseCtrl *m_databaseCtrl;

	SearchEngine *searchEnine;
	afx_msg void OnClose();
	
};

#endif