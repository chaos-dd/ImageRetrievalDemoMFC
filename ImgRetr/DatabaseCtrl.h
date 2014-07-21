#pragma once
#include <vector>
//#include "../utilities/DatabaseInfo.h"
#include "resource.h"
using namespace std;

// CDatabaseCtrl 对话框



class CDatabaseCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDatabaseCtrl)

public:
	CDatabaseCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatabaseCtrl();

// 对话框数据
	enum { IDD = IDD_DATABASECTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void ChangeSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnNewDb();
	afx_msg void OnBnClickedBtnOpenDb();

private:
	void ShowDBNameAndLocation(string _path);


public:
	CFont m_staticFt;
	CRect m_rect;
	vector<int> m_vecCtrlID;
	vector<CRect> m_vecCtrlRect;

	string m_toAddImgPath;
	afx_msg void OnBnClickedBtnSelImg();
	afx_msg void OnBnClickedAddImg();

};

//CFolderDialog
//{
//
//};
