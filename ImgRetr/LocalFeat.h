#pragma once

#include "resource.h"

// CLocalFeat 对话框

class CLocalFeat : public CDialogEx
{
	DECLARE_DYNAMIC(CLocalFeat)

public:
	CLocalFeat(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLocalFeat();

// 对话框数据
	enum { IDD = IDD_LOCALFEAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
