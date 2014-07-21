// PicCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "PicCtrl.h"

#include "drawToDC.h"
// CPicCtrl

//IMPLEMENT_DYNAMIC(CPicCtrl, CStatic)

CPicCtrl::CPicCtrl(CWnd *pParent)
	:m_simScore(-1.0f)
	,m_pStaticPic(NULL)
	,m_picSimScore(NULL)
	,m_parentWnd(pParent)
{
	
}

CPicCtrl::~CPicCtrl()
{
}


//BEGIN_MESSAGE_MAP(CPicCtrl, CStatic)
//	ON_CONTROL_REFLECT(STN_CLICKED, &CPicCtrl::OnStnClicked)
//	ON_WM_PAINT()
//END_MESSAGE_MAP()



// CPicCtrl 消息处理程序




void CPicCtrl::OnStnClicked()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CPicCtrl::ShowImage(cv::Mat &img)
{
	//这里有bug。。。。显示结果的时候 读取图像失败！！！！
	
	if (!img.data)
	{
		return;
	}

	m_imgToShow=img;
	
	m_pStaticPic=(CStatic*)m_parentWnd->GetDlgItem(m_picID);
	m_picSimScore=(CStatic*)m_parentWnd->GetDlgItem(m_staticID);

	CDC *pDC = m_pStaticPic->GetDC();
	HDC hDC=pDC->GetSafeHdc();

	CRect rc;
	m_pStaticPic->GetClientRect(rc);

	DrawToHDC(m_imgToShow,hDC,&rc);
	m_pStaticPic->ReleaseDC(pDC);

	if (m_picSimScore !=NULL)
	{
		CString str;
		str.Format(_T("%2.1f%%"),m_simScore*100);
		m_picSimScore->SetWindowTextA(str);
	}

}

void CPicCtrl::ShowImage(string imgPath)
{	

	m_pStaticPic=(CStatic*)m_parentWnd->GetDlgItem(m_picID);
	m_picSimScore=(CStatic*)m_parentWnd->GetDlgItem(m_staticID);

	//这里有bug。。。。显示结果的时候 读取图像失败！！！！

	m_imgToShow=cv::imread(imgPath.c_str(),1);

	if (!m_imgToShow.data)
	{
		return;
	}

	//m_simScore = imgInfo.dist;
	CDC *pDC = m_pStaticPic->GetDC();
	HDC hDC=pDC->GetSafeHdc();

	CRect rc;
	m_pStaticPic->GetClientRect(rc);

	DrawToHDC(m_imgToShow,hDC,&rc);
	m_pStaticPic->ReleaseDC(pDC);

	if (m_picSimScore !=NULL)
	{
		CString str;
		str.Format(_T("%2.1f%%"),m_simScore*100);
		m_picSimScore->SetWindowTextA(str);
	}

}

void CPicCtrl::ShowImage(ImgInfo &imgInfo)
{	
	m_pStaticPic=(CStatic*)m_parentWnd->GetDlgItem(m_picID);
	m_picSimScore=(CStatic*)m_parentWnd->GetDlgItem(m_staticID);

	//这里有bug。。。。显示结果的时候 读取图像失败！！！！
	
	m_imgToShow=cv::imread(imgInfo.imgName.c_str(),1);
	m_imgName = imgInfo.imgName;
	m_simScore = imgInfo.dist;
	CDC *pDC = m_pStaticPic->GetDC();
	HDC hDC=pDC->GetSafeHdc();

	CRect rc;
	m_pStaticPic->GetClientRect(rc);
	
	DrawToHDC(m_imgToShow,hDC,&rc);
	m_pStaticPic->ReleaseDC(pDC);

	if (m_picSimScore !=NULL)
	{
		CString str;
		str.Format(_T("%.1f%%"),m_simScore*100);
		m_picSimScore->SetWindowTextA(str);
	}

}



void CPicCtrl::ReDrawImage(CPaintDC *dc)
{
	m_pStaticPic=(CStatic*)m_parentWnd->GetDlgItem(m_picID);
	m_picSimScore=(CStatic*)m_parentWnd->GetDlgItem(m_staticID);


	HDC hDC=dc->GetSafeHdc();


	CRect rc;
	m_pStaticPic->GetClientRect(rc);

	DrawToHDC(m_imgToShow,hDC,&rc);

}

void CPicCtrl::OnPaint()
{
	m_pStaticPic=(CStatic*)m_parentWnd->GetDlgItem(m_picID);
	

	CPaintDC dc(m_pStaticPic); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	//if (m_imgToShow.data)
	//{
		ReDrawImage(&dc);
	//}
	
}
