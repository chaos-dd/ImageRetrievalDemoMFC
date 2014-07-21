// ResultShow.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "ResultShow.h"
#include "afxdialogex.h"
#include "Search.h"

// CResultShow 对话框

IMPLEMENT_DYNAMIC(CResultShow, CDialogEx)

CResultShow::CResultShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResultShow::IDD, pParent)
	,m_pVecImgList(NULL)
	,start(0)
	,end(0)
	,index(0)
{
	m_rows = 3;
	m_cols = 4;
	m_picCtrlNum=m_rows * m_cols;
	
	m_margin = 10;

	m_picCtrl = new CPicCtrl *[m_picCtrlNum];

	for (int i = 0 ; i != m_picCtrlNum;++i)
	{
		m_picCtrl[i]=new CPicCtrl(this);

	}


	//m_imgFullNames = new CString *[m_picCtrlNum];
	
}

CResultShow::~CResultShow()
{
	for (int i = 0 ; i != m_picCtrlNum;++i)
	{
		delete m_picCtrl[i];
	}

	delete [] m_picCtrl;
}

void CResultShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX,IDC_PICCTRL0,)
}


BEGIN_MESSAGE_MAP(CResultShow, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_PRE_PAGE, &CResultShow::OnBnClickedBtnPrePage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CResultShow::OnBnClickedBtnNextPage)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PICCTRL0, &CResultShow::OnStnClickedPicctrl0)
	ON_STN_CLICKED(IDC_PICCTRL1, &CResultShow::OnStnClickedPicctrl1)
	ON_STN_CLICKED(IDC_PICCTRL2, &CResultShow::OnStnClickedPicctrl2)
	ON_STN_CLICKED(IDC_PICCTRL3, &CResultShow::OnStnClickedPicctrl3)
	ON_STN_CLICKED(IDC_PICCTRL4, &CResultShow::OnStnClickedPicctrl4)
	ON_STN_CLICKED(IDC_PICCTRL5, &CResultShow::OnStnClickedPicctrl5)
	ON_STN_CLICKED(IDC_PICCTRL6, &CResultShow::OnStnClickedPicctrl6)
	ON_STN_CLICKED(IDC_PICCTRL7, &CResultShow::OnStnClickedPicctrl7)
	ON_STN_CLICKED(IDC_PICCTRL8, &CResultShow::OnStnClickedPicctrl8)
	ON_STN_CLICKED(IDC_PICCTRL9, &CResultShow::OnStnClickedPicctrl9)
	ON_STN_CLICKED(IDC_PICCTRL10, &CResultShow::OnStnClickedPicctrl10)
	ON_STN_CLICKED(IDC_PICCTRL11, &CResultShow::OnStnClickedPicctrl11)

	ON_STN_DBLCLK(IDC_PICCTRL0, &CResultShow::OnStnDblclickPicctrl0)
	ON_STN_DBLCLK(IDC_PICCTRL1, &CResultShow::OnStnDblclickPicctrl1)
	ON_STN_DBLCLK(IDC_PICCTRL2, &CResultShow::OnStnDblclickPicctrl2)
	ON_STN_DBLCLK(IDC_PICCTRL3, &CResultShow::OnStnDblclickPicctrl3)
	ON_STN_DBLCLK(IDC_PICCTRL4, &CResultShow::OnStnDblclickPicctrl4)
	ON_STN_DBLCLK(IDC_PICCTRL5, &CResultShow::OnStnDblclickPicctrl5)
	ON_STN_DBLCLK(IDC_PICCTRL6, &CResultShow::OnStnDblclickPicctrl6)
	ON_STN_DBLCLK(IDC_PICCTRL7, &CResultShow::OnStnDblclickPicctrl7)
	ON_STN_DBLCLK(IDC_PICCTRL8, &CResultShow::OnStnDblclickPicctrl8)
	ON_STN_DBLCLK(IDC_PICCTRL9, &CResultShow::OnStnDblclickPicctrl9)
	ON_STN_DBLCLK(IDC_PICCTRL10, &CResultShow::OnStnDblclickPicctrl10)
	ON_STN_DBLCLK(IDC_PICCTRL11, &CResultShow::OnStnDblclickPicctrl11)

END_MESSAGE_MAP()


// CResultShow 消息处理程序


BOOL CResultShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	for (int i = 0 ; i !=m_picCtrlNum;++i)
	{
		//这里需要查看 IDC_PICCTRL的值 是否连续
		//GetDlgItem返回的指针可能是临时的，随用随取
		m_picCtrl[i]->m_picID=IDC_PICCTRL0+i;
		m_picCtrl[i]->m_staticID=IDC_STATIC_SIM0+i;

	}




	SetButtonState();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CResultShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	for (int i = 0 ; i !=m_picCtrlNum;++i)
	{
		//这里需要查看 IDC_PICCTRL的值 是否连续
		m_picCtrl[i]->OnPaint();
		//m_picCtrl[i]->m_pStaticPic->Invalidate();
	}

}




void CResultShow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码

	CRect clientRect;
	GetClientRect(&clientRect);

	int imWidth = (clientRect.Width() - 5*m_margin)/m_cols;
	int imHeight = imWidth;

	int textHeight=20;

	for(int r=0 ; r != m_rows; ++r)
	{
		for (int c =0; c !=m_cols ;++c)
		{
			int l_index=r*m_cols+c;
			m_picCtrl[l_index]->m_pStaticPic=(CStatic*)GetDlgItem(IDC_PICCTRL0+l_index);
			m_picCtrl[l_index]->m_picSimScore=(CStatic*)GetDlgItem(IDC_STATIC_SIM0+l_index);
		
			m_picCtrl[l_index]->m_pStaticPic->MoveWindow(m_margin+(m_margin+imWidth)*c, \
				m_margin+(textHeight+ m_margin+imHeight)*r,imWidth,imHeight);
			m_picCtrl[l_index]->m_picSimScore->MoveWindow(m_margin+(m_margin+imWidth)*c,\
				m_margin+(textHeight+ m_margin+imHeight)*r+(imHeight),imWidth,textHeight);

		}
	}

	CRect btnRect;
	CButton *btnPre = (CButton *) GetDlgItem(IDC_BTN_PRE_PAGE);

	btnPre->GetClientRect(&btnRect);

	
	btnPre->MoveWindow(clientRect.Width()/2 - btnRect.Width(), clientRect.bottom-btnRect.bottom,btnRect.Width(),btnRect.Height());
	CButton *btnNext = (CButton *)GetDlgItem(IDC_BTN_NEXT_PAGE);
	
	btnNext->MoveWindow(clientRect.Width()/2 , clientRect.bottom-btnRect.bottom,btnRect.Width(),btnRect.Height());
}




void CResultShow::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void CResultShow::SetButtonState()
{
	if (index<=12)
	{
		GetDlgItem(IDC_BTN_PRE_PAGE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_PRE_PAGE)->EnableWindow(TRUE);
	}

	if (index==end )
	{
		GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(TRUE);
	}

}

void CResultShow::SetImageList(vector<ImgInfo> * pVecImgList,int threshold)
{
	m_pVecImgList = pVecImgList;
	start=0;
	index=0;
	end = pVecImgList->size();
	if (threshold !=0)
	{
		float l_thresh=(float)threshold/100;
		for (int i = 0 ; i != pVecImgList->size();++i)
		{
			if ((*pVecImgList)[i].dist < l_thresh)
			{
				end = i;
				break;
			}
		}
	}
	
}

void CResultShow::ShowImageList(bool bReverse)
{
	//UpdateWindow();
	
	if (bReverse ==true)		//逆序输出图像
	{

		index = index-24>0?index-24:0;

	}
	for (size_t ind = 0 ; ind != m_picCtrlNum; ++ind)
	{
		if (index > end -1)
		//if ( index>m_pVecImgList->size()-1)
		{			
			DWORD color = ::GetSysColor(COLOR_WINDOWFRAME);
			int r= GetRValue(color);
			int g=GetGValue(color) ;
			int b=GetBValue(color);
			cv::Mat tmp=cv::Mat(20,20,CV_8UC1,cv::Scalar(b,g,r));
			m_picCtrl[ind]->m_simScore=0.0f;
			m_picCtrl[ind]->ShowImage(tmp);	
		}
		else
		{
			m_picCtrl[ind]->ShowImage((*m_pVecImgList)[index]);
			++index;
		}	
		
	}

	//Invalidate();
	SetButtonState();

}
void CResultShow::OnBnClickedBtnPrePage()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowImageList(true);
	SetButtonState();
}


void CResultShow::OnBnClickedBtnNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowImageList();
	SetButtonState();
}




void CResultShow::OnStnClickedPicctrl0()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[0]->m_imgName.c_str());
}



void CResultShow::OnStnClickedPicctrl1()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[1]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl2()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[2]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl3()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[3]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl4()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[4]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl5()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[5]->m_imgName.c_str());
}



void CResultShow::OnStnClickedPicctrl6()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[6]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl7()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[7]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl8()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[8]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl9()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[9]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl10()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	//pParentWnd->m_bigShowPic->ShowImage(m_picCtrl[10]->m_imgToShow);
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[10]->m_imgName.c_str());
}


void CResultShow::OnStnClickedPicctrl11()
{
	// TODO: 在此添加控件通知处理程序代码
	CSearch * pParentWnd=	(CSearch*)this->GetParent();
	//pParentWnd->m_bigShowPic->ShowImage(m_picCtrl[11]->m_imgToShow);
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();
	pMainWnd->m_statusBar.SetWindowTextA(m_picCtrl[11]->m_imgName.c_str());
}



void CResultShow::OnStnDblclickPicctrl0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[0]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[0]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[0]->m_imgName.c_str(),m_picCtrl[0]->m_imgToShow);
	//if (waitKey())
	//{
	//}
}

void CResultShow::OnStnDblclickPicctrl1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[1]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[1]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[1]->m_imgName.c_str(),m_picCtrl[1]->m_imgToShow);

}


void CResultShow::OnStnDblclickPicctrl2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[2]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[2]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[2]->m_imgName.c_str(),m_picCtrl[2]->m_imgToShow);

}

void CResultShow::OnStnDblclickPicctrl3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[3]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[3]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[3]->m_imgName.c_str(),m_picCtrl[3]->m_imgToShow);

}

void CResultShow::OnStnDblclickPicctrl4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[4]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[4]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[4]->m_imgName.c_str(),m_picCtrl[4]->m_imgToShow);

}

void CResultShow::OnStnDblclickPicctrl5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[5]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[5]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[5]->m_imgName.c_str(),m_picCtrl[5]->m_imgToShow);

}

void CResultShow::OnStnDblclickPicctrl6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[6]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[6]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[6]->m_imgName.c_str(),m_picCtrl[6]->m_imgToShow);

}

void CResultShow::OnStnDblclickPicctrl7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[7]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[7]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[7]->m_imgName.c_str(),m_picCtrl[7]->m_imgToShow);
}

void CResultShow::OnStnDblclickPicctrl8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[8]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[8]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[8]->m_imgName.c_str(),m_picCtrl[8]->m_imgToShow);
}

void CResultShow::OnStnDblclickPicctrl9()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[9]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[9]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[9]->m_imgName.c_str(),m_picCtrl[9]->m_imgToShow);
}

void CResultShow::OnStnDblclickPicctrl10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[10]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[10]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[10]->m_imgName.c_str(),m_picCtrl[10]->m_imgToShow);
}

void CResultShow::OnStnDblclickPicctrl11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_picCtrl[11]->m_imgToShow.empty())
	{
		return;
	}
	cv::namedWindow(m_picCtrl[11]->m_imgName.c_str(),CV_WINDOW_NORMAL);
	cv::imshow(m_picCtrl[11]->m_imgName.c_str(),m_picCtrl[11]->m_imgToShow);
}

