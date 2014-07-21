#pragma once


// CResultShow 对话框
#include "PicCtrl.h"
#include "resource.h"
#include "../utilities/searchEngine.h"

class CResultShow : public CDialogEx
{
	DECLARE_DYNAMIC(CResultShow)

public:
	CResultShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResultShow();

// 对话框数据
	enum { IDD = IDD_RESULTSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);


	virtual BOOL OnInitDialog();

public:
	void SetButtonState();
	void SetImageList(vector<ImgInfo> * pVecImgList,int threshold=0);
	void ShowImageList(bool bReverse = false);
public:

	int m_picCtrlNum;
	int m_rows;
	int m_cols;
	int m_margin;
	CPicCtrl ** m_picCtrl;

	//CString **m_imgFullNames;
	vector<ImgInfo> *m_pVecImgList;
	int start;
	int end;

	int index;

	virtual void OnOK();
	afx_msg void OnBnClickedBtnPrePage();
	afx_msg void OnBnClickedBtnNextPage();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPicctrl0();
	afx_msg void OnStnClickedPicctrl1();
	afx_msg void OnStnClickedPicctrl2();
	afx_msg void OnStnClickedPicctrl3();
	afx_msg void OnStnClickedPicctrl4();
	afx_msg void OnStnClickedPicctrl5();
	afx_msg void OnStnClickedPicctrl6();
	afx_msg void OnStnClickedPicctrl7();
	afx_msg void OnStnClickedPicctrl8();
	afx_msg void OnStnClickedPicctrl9();
	afx_msg void OnStnClickedPicctrl10();
	afx_msg void OnStnClickedPicctrl11();

	afx_msg void OnStnDblclickPicctrl0();
	afx_msg void OnStnDblclickPicctrl1();
	afx_msg void OnStnDblclickPicctrl2();
	afx_msg void OnStnDblclickPicctrl3();
	afx_msg void OnStnDblclickPicctrl4();
	afx_msg void OnStnDblclickPicctrl5();
	afx_msg void OnStnDblclickPicctrl6();
	afx_msg void OnStnDblclickPicctrl7();
	afx_msg void OnStnDblclickPicctrl8();
	afx_msg void OnStnDblclickPicctrl9();
	afx_msg void OnStnDblclickPicctrl10();
	afx_msg void OnStnDblclickPicctrl11();

	
};
