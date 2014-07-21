// GlobalFeat.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "GlobalFeat.h"
#include "afxdialogex.h"
#include "ImgRetrDlg.h"
#include "../utilities/searchEngine.h"
// CGlobalFeat 对话框

IMPLEMENT_DYNAMIC(CGlobalFeat, CDialogEx)

	CGlobalFeat::CGlobalFeat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGlobalFeat::IDD, pParent)
	
	, m_textureWeight(0)
	, m_shapeWeight(0)
	, m_colorWeight(0)
{

}

CGlobalFeat::~CGlobalFeat()
{
}

void CGlobalFeat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);




	DDX_Text(pDX, IDC_EDIT2, m_textureWeight);
	DDV_MinMaxInt(pDX,m_textureWeight,0,100);
	DDX_Text(pDX, IDC_EDIT3, m_shapeWeight);
	DDV_MinMaxInt(pDX,m_shapeWeight,0,100);
	DDX_Text(pDX, IDC_EDIT1, m_colorWeight);
	DDV_MinMaxInt(pDX, m_colorWeight, 0, 100);
}


BEGIN_MESSAGE_MAP(CGlobalFeat, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT2, &CGlobalFeat::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CGlobalFeat::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CGlobalFeat::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CGlobalFeat 消息处理程序


void CGlobalFeat::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


BOOL CGlobalFeat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	//自动处理控件大小变化
	GetClientRect(&m_rect);

	CRect rc;
	CWnd* pFirstCtrl = GetNextDlgTabItem(NULL);
	if(pFirstCtrl !=NULL)
	{
		m_vecCtrlID.push_back(pFirstCtrl->GetDlgCtrlID());
		pFirstCtrl->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_vecCtrlRect.push_back(rc);
		CWnd* pCtrl=GetNextDlgTabItem(pFirstCtrl);

		while(pCtrl!=NULL && pCtrl != pFirstCtrl)
		{        
			m_vecCtrlID.push_back(pCtrl->GetDlgCtrlID()); 
			pCtrl->GetWindowRect(&rc);
			ScreenToClient(&rc);
			m_vecCtrlRect.push_back(rc);

			pCtrl = GetNextDlgTabItem(pCtrl);
		}
	}


	//向控件中添加内容

	CImgRetrDlg *pMainDlg = (CImgRetrDlg*)AfxGetMainWnd();
	//DatabaseInfo * pdatabaseInfo=pMainDlg->searchEnine.databaseInfo;
	FeatureExactor *pFeatExactor=pMainDlg->searchEnine->m_featureExactor;
	for (int ind = 0 ; ind !=pFeatExactor->getGlobalFeatNum();++ind)
	{
		switch (pFeatExactor->m_vecGlobalFeatureExactor[ind]->featType)
		{
		case GLOBAL_COLOR_FEAT:
			{
				CString str;
				str.Format("%s",pFeatExactor->m_vecGlobalFeatureExactor[ind]->featName.c_str());
				((CComboBox*)GetDlgItem(IDC_COMBO_COLORFEAT))->AddString(str);
				break;
			}
		case GLOBAL_TEXTURE_FEAT:
			{
				CString str;
				str.Format("%s",pFeatExactor->m_vecGlobalFeatureExactor[ind]->featName.c_str());
				((CComboBox*)GetDlgItem(IDC_COMBO_TEXTUREFEAT))->AddString(str);
				break;
			}
		case GLOBAL_SHAPE_FEAT:
			{
				CString str;
				str.Format("%s",pFeatExactor->m_vecGlobalFeatureExactor[ind]->featName.c_str());
				((CComboBox*)GetDlgItem(IDC_COMBO_SHAPEFEAT))->AddString(str);

				break;
			}
		default:
			;
		}
	}



	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1))->SetBuddy(GetDlgItem(IDC_EDIT1));
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN2))->SetBuddy(GetDlgItem(IDC_EDIT2));
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN3))->SetBuddy(GetDlgItem(IDC_EDIT3));
	
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1))->SetRange(0,100);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN2))->SetRange(0,100);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN3))->SetRange(0,100);

	((CEdit *)GetDlgItem(IDC_EDIT1))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_EDIT2))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_EDIT3))->LimitText(3);

	((CComboBox*)GetDlgItem(IDC_COMBO_COLORFEAT))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TEXTUREFEAT))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_SHAPEFEAT))->SetCurSel(0);
	

	
	
	((CComboBox*)GetDlgItem(IDC_COMBO_SIM))->AddString(CString(_T("L1距离")));
	((CComboBox*)GetDlgItem(IDC_COMBO_SIM))->AddString(CString(_T("L2距离")));
	((CComboBox*)GetDlgItem(IDC_COMBO_SIM))->AddString(CString(_T("直方图交")));
	((CComboBox*)GetDlgItem(IDC_COMBO_SIM))->AddString(CString(_T("余弦距离")));
	((CComboBox*)GetDlgItem(IDC_COMBO_SIM))->SetCurSel(0);

	m_textureWeight=33;
	m_shapeWeight=33;
	m_colorWeight=33;
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGlobalFeat::ChangeSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Invalidate();
	if(nType!=SIZE_MINIMIZED ) 
	{
		if(cx>m_rect.Width() || cy>m_rect.Height())
		{
			for (int i = 0 ; i!=m_vecCtrlID.size();++i)
			{
				CRect &rec = m_vecCtrlRect[i]; 
				//pWnd->GetWindowRect(&rec);  //获取控件变化前的大小
				//ScreenToClient(&rec);   //将控件大小装换位在对话框中的区域坐标
				CRect new_rec=rec;
				new_rec.left=ceil((float)rec.left*cx/m_rect.Width());  //按照比例调整空间的新位置
				new_rec.top=ceil((float)rec.top*cy/m_rect.Height());

				new_rec.bottom=ceil((float)rec.bottom*cy/m_rect.Height());
				new_rec.right=ceil((float)rec.right*cx/m_rect.Width());
				GetDlgItem(m_vecCtrlID[i])->MoveWindow(new_rec);   //伸缩控件
			}
		}
		else
		{
			for (int i = 0 ; i!=m_vecCtrlID.size();++i)
			{
				CRect &rec = m_vecCtrlRect[i]; 
				//pWnd->GetWindowRect(&rec);  //获取控件变化前的大小
				//ScreenToClient(&rec);   //将控件大小装换位在对话框中的区域坐标
				CRect new_rec=rec;
				new_rec.left=floor((float)rec.left*cx/m_rect.Width());  //按照比例调整空间的新位置
				new_rec.top=floor((float)rec.top*cy/m_rect.Height());

				new_rec.bottom=floor((float)rec.bottom*cy/m_rect.Height());
				new_rec.right=floor((float)rec.right*cx/m_rect.Width());
				GetDlgItem(m_vecCtrlID[i])->MoveWindow(new_rec);   //伸缩控件
			}

		}
	}
}

void CGlobalFeat::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData();


}


void CGlobalFeat::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData();
	

	//CEdit * pEdit=(CEdit*)GetDlgItem(IDC_EDIT3);	
	//CString str;
	//pEdit->GetWindowTextA(str);
	//if(str.GetLength()==0)
	//	return;
	//else if(UpdateData(TRUE) && (m_shapeWeight >100 || m_shapeWeight <0))
	//	MessageBox(_T("不能大于100%"),_T("错误"),MB_ICONERROR);
}


void CGlobalFeat::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}

