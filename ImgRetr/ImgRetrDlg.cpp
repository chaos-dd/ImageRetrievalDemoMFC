
// ImgRetrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "ImgRetrDlg.h"
#include "afxdialogex.h"

#include "Search.h"
#include "DatabaseCtrl.h"

#include "../utilities/searchEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImgRetrDlg 对话框




CImgRetrDlg *g_pMainWnd;

CImgRetrDlg::CImgRetrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgRetrDlg::IDD, pParent)
{
	g_pMainWnd=this;

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN);
	m_searchCtrl=new CSearch(&m_tab1);
	m_databaseCtrl = new CDatabaseCtrl(&m_tab1);
	searchEnine = new SearchEngine;
	
}

CImgRetrDlg::~CImgRetrDlg()
{
	delete m_searchCtrl;
	delete m_databaseCtrl;
	delete searchEnine;
}

void CImgRetrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
}

BEGIN_MESSAGE_MAP(CImgRetrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CImgRetrDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImgRetrDlg 消息处理程序

BOOL CImgRetrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	
	// TODO: 在此添加额外的初始化代码


	MoveWindow(0,0,1080,768);


	//状态栏相关
	m_statusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW,CRect(0,0,0,0),this,0);





	
	//TAB控件相关	
	

	m_tab1.InsertItem(0,_T("检索图像"),0);      //添加参数一选项卡 
	m_tab1.InsertItem(1,_T("管理数据库"),1);    //添加参数二选项卡 
	m_tab1.SetCurSel(0);

	//创建并初始化标签
	m_searchCtrl->Create(IDD_SEARCH,&m_tab1);
	m_databaseCtrl->Create(IDD_DATABASECTRL,&m_tab1);



	

	
	//CImageList* imgList = new CImageList;
	//imgList->Create(32,32,ILC_COLOR4,2,2);

	//imgList->SetBkColor(RGB(255,255,255));
//	imgList->Add(::AfxGetApp()->LoadIcon(IDI_ICON1));
	//imgList->Add(::AfxGetApp()->LoadIcon(IDI_ICON1));

	//m_tab1.SetImageList(imgList);

	//delete imgList;



	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImgRetrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImgRetrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImgRetrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImgRetrDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int num=m_tab1.GetCurSel();
	switch(num)
	{
	case 0:
		m_searchCtrl->ShowWindow(SW_SHOW);
		m_databaseCtrl->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_searchCtrl->ShowWindow(SW_HIDE);
		m_databaseCtrl->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


void CImgRetrDlg::OnSize(UINT nType, int cx, int cy)
{

	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

}


void CImgRetrDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CImgRetrDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码

	//状态栏相关
	//int strPartDim[3]={100,200,-1};

	//m_statusBar.SetParts(3,strPartDim);

	//m_statusBar.SetText(_T("啊啊啊啊啊啊啊啊啊啊啊啊啊啊"),0,0);
	//m_statusBar.SetText(_T("分栏二"),1,0);
	//m_statusBar.SetText(_T("分栏三"),2,0);


	CRect statucRect;
	m_statusBar.GetWindowRect(&statucRect);

	//设置 tab1的大小和位置
	CRect rect;
	// Get size of dialog window.
	GetClientRect(&rect);

	//m_tab1.AdjustRect(FALSE, &rect);
	m_tab1.SetItemSize(CSize(32,32));

	int offset =10;

	rect.top+=offset;
	rect.bottom -=statucRect.Height();
	m_tab1.MoveWindow(&rect, TRUE);  	

	m_tab1.ShowWindow(true);

	//设置两个 对话框的大小和位置
	CRect rc;
	m_tab1.GetItemRect(0,&rc);

	CRect rc2;
	m_tab1.GetClientRect(rc2);
	rc2.top +=rc.Height();

	m_searchCtrl->MoveWindow(&rc2);
	m_databaseCtrl->MoveWindow(&rc2);

	m_databaseCtrl->ChangeSize(SIZE_MAXSHOW,rc2.Width(),rc2.Height());

	m_searchCtrl->ShowWindow(true);
	m_databaseCtrl->ShowWindow(false);

}


void CImgRetrDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	extern CMutex g_mutex;
	CSingleLock singleLock(&g_mutex);
	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		int flag =MessageBox(_T("有一个操作正在进行，现在关闭程序会造成数据损坏！\n 继续吗"),_T("警告"),MB_OKCANCEL);
		if (flag ==IDOK)
		{
			int flag2=MessageBox(_T("关闭程序后需要将原数据删除！\n 继续吗"),_T("警告"),MB_OKCANCEL);
			if (flag2==IDOK)
			{
				CDialogEx::OnClose();
			}
		}
		
		
	}

	CDialogEx::OnClose();
	
}

