


#ifndef SEARCH_HHH
#define SEARCH_HHH
//#pragma once
#include "afxcmn.h"
#include "ResultShow.h"
#include "LocalFeat.h"
#include "GlobalFeat.h"
#include "afxwin.h"
#include "picctrl.h"

//#include "ImgRetrDlg.h"
// CSearch 对话框

class CSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CSearch)

public:
	CSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearch();

// 对话框数据
	enum { IDD = IDD_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()	

public:
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	virtual void OnOK();
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSelImg();

	void GetSearchParam(vector<int> &featIndex,vector<float> & featWeights,int &distanceType);
public:
	CTabCtrl m_tab2;
	CLocalFeat m_localFeatDlg;
	CGlobalFeat m_globalFeatDlg;
	CResultShow m_resultShowDlg;
	CListBox m_imgListCtrl;
	CPicCtrl *m_inputPic;

	int m_simThreshold;
	CString m_imgFullName;
	CString m_imgListPath;
	vector<string> m_imgNameList;
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnStnClickedPicinput();
	
	//CPicCtrl *m_bigShowPic;
	afx_msg void OnPaint();
	afx_msg void OnStnDblclickPicinput();
	
	afx_msg void OnEnChangeEditSimthreshold();

	//afx_msg void OnCbnDropdownComboInputhistory();
	//afx_msg void OnCbnSetfocusComboInputhistory();
	
	afx_msg void OnLbnDblclkImglistbox();

	afx_msg void OnBnClickedBtnSelImgDir();
	afx_msg void OnLbnSelchangeImglistbox();
};

#endif