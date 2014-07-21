// LocalFeat.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "LocalFeat.h"
#include "afxdialogex.h"


// CLocalFeat 对话框

IMPLEMENT_DYNAMIC(CLocalFeat, CDialogEx)

CLocalFeat::CLocalFeat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLocalFeat::IDD, pParent)
{

}

CLocalFeat::~CLocalFeat()
{
}

void CLocalFeat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLocalFeat, CDialogEx)
END_MESSAGE_MAP()


// CLocalFeat 消息处理程序


void CLocalFeat::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
