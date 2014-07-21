// MyTabCrtl.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "MyTabCrtl.h"


// CTabCrtl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTabCrtl 消息处理程序




void CMyTabCtrl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CMyTabCrtl::OnPaint()


	CDC* pDC = fatherDialog->GetDC();
	COLORREF color= pDC->GetBkColor();
	
ReleaseDC(pDC);

	CRect		rectClient, rectItem;
	CBitmap		bitmapTemp, *pOldBitmap;
	CDC*		pMemDC=new CDC;
	CPaintDC	dc(this);

	//获取窗口大小
	GetClientRect(rectClient);

	//创建位图内存
	bitmapTemp.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	pMemDC->CreateCompatibleDC(&dc);
	pOldBitmap=pMemDC->SelectObject(&bitmapTemp);

	pMemDC->FillSolidRect(rectClient, color);


	for(int i=0; i< this->GetItemCount(); i++)
	{
		GetItemRect(i, rectItem);
		pMemDC->FillSolidRect(rectItem, color);
		pMemDC->FrameRect(rectItem, &CBrush(color));
		//DrawItemName(pMemDC, i);
	}

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), pMemDC, 0, 0, SRCCOPY);

	//释放内存
	pMemDC->SelectObject(pOldBitmap);
	ReleaseDC(pMemDC);

	delete	pMemDC;
}
