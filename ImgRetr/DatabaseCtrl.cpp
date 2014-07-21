// DatabaseCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DatabaseCtrl.h"

#include "ImgRetrDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include "../utilities/searchEngine.h"


// CDatabaseCtrl 对话框

IMPLEMENT_DYNAMIC(CDatabaseCtrl, CDialogEx)

CDatabaseCtrl::CDatabaseCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseCtrl::IDD, pParent)
{

}

CDatabaseCtrl::~CDatabaseCtrl()
{
}

void CDatabaseCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDatabaseCtrl, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NEW_DB, &CDatabaseCtrl::OnBnClickedBtnNewDb)
	ON_BN_CLICKED(IDC_BTN_OPEN_DB, &CDatabaseCtrl::OnBnClickedBtnOpenDb)
	ON_BN_CLICKED(IDC_BTN_SEL_IMG, &CDatabaseCtrl::OnBnClickedBtnSelImg)
	ON_BN_CLICKED(IDC_ADD_IMG, &CDatabaseCtrl::OnBnClickedAddImg)
END_MESSAGE_MAP()


// CDatabaseCtrl 消息处理程序


BOOL CDatabaseCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

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



	//设置progress bar
	CProgressCtrl *pProg=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProg->SetRange(0,10000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDatabaseCtrl::ChangeSize(UINT nType, int cx, int cy)
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


//给选择文件夹对话框增加新建文件夹功能

void CDatabaseCtrl::OnBnClickedBtnNewDb()
{
	// TODO: 在此添加控件通知处理程序代码
	
	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("请选择空目录存放数据文件！");   
	bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_DONTGOBELOWDOMAIN|BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(SHGetPathFromIDList(lp, szPath))   
	{
		
		CFileFind finder;

		// build a string with wildcards
		CString strWildcard(szPath);
		strWildcard += _T("\\*.*");

		

		BOOL bWorking = finder.FindFile(strWildcard);

		if (bWorking ==0 )
		{
			MessageBox(_T("输入不是路径!"),_T("错误"),MB_ICONERROR);
			return;
		}

		int cnt=2;
		while (bWorking )
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}			
			else if (finder.IsDirectory())
			{				
				cnt++;
				break;
			}
			else
			{
				cnt++;
				break;
			}
		}
		finder.Close();	

		if (cnt > 2  )
		{
			MessageBox(_T("输入路径非空!"),_T("错误"),MB_ICONERROR);
			return;
		}
	}	
	else if (!szPath[0])
	{
		return;
	}
	else
	{
		//SHGetPathFromIDList 函数返回错误
		MessageBox(_T("选择的路径非法!"),_T("错误"),MB_ICONERROR);
		return;
	}
	
	ShowDBNameAndLocation(szPath);

	CImgRetrDlg * pMainWnd=(CImgRetrDlg*)AfxGetMainWnd();
	string databasePath(szPath);
	DatabaseInfo *databaseInfo;

	CMutex g_mutex;

	CSingleLock singleLock(&g_mutex);
	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("请等待上一个操作完成！"),_T("错误"),MB_ICONERROR);
		return ;
	}

	databaseInfo = new DatabaseInfo(databasePath,pMainWnd->searchEnine->m_featureExactor);


	pMainWnd->searchEnine->setDatabase(databaseInfo);

	singleLock.Unlock();
}




UINT ThreadNewDatabaseInfo( LPVOID pParam )
{
	extern CMutex g_mutex;	
	CSingleLock singleLock(&g_mutex);

	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("请等待上一个操作完成！"),_T("错误"),MB_ICONERROR);
		return 0;
	}

	char * strDBPath = (char*)pParam;
	extern CImgRetrDlg* g_pMainWnd;
	DatabaseInfo *databaseInfo = new DatabaseInfo(string(strDBPath),g_pMainWnd->searchEnine->m_featureExactor);

	g_pMainWnd->searchEnine->setDatabase(databaseInfo);

	delete strDBPath;

	singleLock.Unlock();
	return 0;
}


void CDatabaseCtrl::OnBnClickedBtnOpenDb()
{
	// TODO: 在此添加控件通知处理程序代码


	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("请选择已存在的数据文件夹！");   
	bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_DONTGOBELOWDOMAIN|BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{

		CFileFind finder;

		// build a string with wildcards
		CString strWildcard(szPath);
		strWildcard += _T("\\*.*");



		BOOL bWorking = finder.FindFile(strWildcard);

		if (bWorking ==0 )
		{
			MessageBox(_T("输入不是路径!"),_T("错误"),MB_ICONERROR);
			return;
		}

		int cnt=2;
		while (bWorking )
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}			
			else if (finder.IsDirectory())
			{				
				cnt++;
				break;
			}
			else
			{
				cnt++;
				break;
			}
		}
		finder.Close();	

		if (cnt == 2  )
		{
			MessageBox(_T("输入路径为空，不存在数据!"),_T("错误"),MB_ICONERROR);
			return;
		}
	}	
	else if (!szPath[0])
	{
		return;
	}
	else
	{
		MessageBox(_T("选择的路径非法!"),_T("错误"),MB_ICONERROR);
		return;
	}


	ShowDBNameAndLocation(szPath);



	CImgRetrDlg * pMainWnd=(CImgRetrDlg*)AfxGetMainWnd();
	string databasePath(szPath);




	//线程参数，在线程函数中释放
	char * strDBPath = new char[MAXBUFSIZE];
	strcpy(strDBPath,databasePath.c_str());
	AfxBeginThread(ThreadNewDatabaseInfo,strDBPath);

}



void CDatabaseCtrl::ShowDBNameAndLocation(string _path)
{
	string folderName;
	FindFiles::getFatherPath(_path,folderName);

	CStatic * pDBNameStatic =(CStatic*) GetDlgItem(IDC_DB_NAME_SHOW);
	pDBNameStatic->SetWindowTextA(folderName.c_str());
	CStatic * pDBLocationStatic = (CStatic*)GetDlgItem(IDC_DB_LOCATION_SHOW);
	pDBLocationStatic->SetWindowTextA(_path.c_str());

}

void CDatabaseCtrl::OnBnClickedBtnSelImg()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFolderPickerDialog folderDlg;
	//folderDlg.DoModal();

	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("请选择已存在的数据文件夹！");   
	bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_DONTGOBELOWDOMAIN|BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{

		CFileFind finder;

		// build a string with wildcards
		CString strWildcard(szPath);
		strWildcard += _T("\\*.*");



		BOOL bWorking = finder.FindFile(strWildcard);

		if (bWorking ==0 )
		{
			MessageBox(_T("输入不是路径!"),_T("错误"),MB_ICONERROR);
			return;
		}

		int cnt=2;
		while (bWorking )
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}			
			else if (finder.IsDirectory())
			{				
				cnt++;
				break;
			}
			else
			{
				cnt++;
				break;
			}
		}
		finder.Close();	

		if (cnt == 2  )
		{
			MessageBox(_T("输入路径为空，不存在图像!"),_T("错误"),MB_ICONERROR);
			return;
		}
	}	
	else if(!szPath[0])
	{
		return ;
	}
	else
	{
		MessageBox(_T("选择的路径非法!"),_T("错误"),MB_ICONERROR);
		return;
	}

	m_toAddImgPath=string(szPath);


}


UINT ThreadAddImages( LPVOID pParam )
{
	//CDatabaseCtrl * pCDBctrl = (CDatabaseCtrl*)pParam;

	CImgRetrDlg  * pMainWnd=(CImgRetrDlg*)pParam;

	pMainWnd->searchEnine->AddImageToDatabase(pMainWnd->m_databaseCtrl->m_toAddImgPath);
	//pMainWnd->searchEnine.databaseInfo->AddImages(pMainWnd->m_databaseCtrl.m_toAddImgPath);
	return 0;
}


void CDatabaseCtrl::OnBnClickedAddImg()
{
	// TODO: 在此添加控件通知处理程序代码	

	//CImgRetrDlg * pMainWnd=(CImgRetrDlg*)AfxGetMainWnd();
	extern CImgRetrDlg * g_pMainWnd;
	//pMainWnd->searchEnine.setDatabase(databaseInfo);

	if (g_pMainWnd->searchEnine->databaseInfo==NULL)
	{
		MessageBox(_T("请新建或者选择已有数据库！"),_T("错误"),MB_ICONERROR);
		return;
	}


	//pMainWnd->searchEnine.databaseInfo->AddImages(m_toAddImgPath);

	AfxBeginThread(ThreadAddImages,g_pMainWnd,0,0,0,NULL);

}
