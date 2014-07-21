#pragma once

#include "allInclude.h"


#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include "../features/FeatureExactor.h"
#include "../features/color_feature.h"
#include "../features/shapeFeature.h"
#include "../utilities/FindFiles.h"
#include "../features/lbp.h"

//#include "../ImgRetr/DatabaseCtrl.h"
#include "../ImgRetr/ImgRetrDlg.h"
using namespace std;

//字符串缓冲区大小,文件名包括路径不能大于此值
const int MAXBUFSIZE = 256;

class CDatabaseCtrl;

class DatabaseInfo
{
public:

	DatabaseInfo(string _path,FeatureExactor *_pFeatureExactor)
	{
		m_pFeatureExactor = _pFeatureExactor;
		extern CImgRetrDlg * g_pMainWnd;
		m_pMainWnd = g_pMainWnd;
		if(-1==_access(_path.c_str(),0))
		{
			_mkdir(_path.c_str());
		}

		FindFiles::getFatherPath(_path,databaseName);
		outDataPath = _path;


		globalFeat.resize(m_pFeatureExactor->m_vecGlobalFeatureExactor.size());
		try
		{
			LoadDatabase();
		}
		catch (runtime_error &e)
		{
			//cout<<e.what()<<endl;
		}

		m_status=false;
	}

	~DatabaseInfo()
	{
		try
		{
			if(m_status==true)
			{
				SaveDatabase();
			}	
		}
		catch (runtime_error &e)
		{
			::MessageBox(NULL,e.what(),_T("错误"),MB_ICONERROR);
		}
		
	}	

	void AddImages(string imgPath);
	inline int getFeatTypeNum()
	{
		return m_pFeatureExactor->m_vecGlobalFeatureExactor.size();
	}
	//void exactSingleImgFeat(string imgPath,vector<Mat> &vec_feat);
	//void exactBatchImgFeat(string imgFolderPath,vector<string> &imgFullNames,vector<Mat> &vec2_feat);
	
private:
	//load database structure data member
	void LoadDatabase();
	void SaveDatabase();

	void SaveFeat()  ;
	void LoadFeat() ;
public:
	//标记数据库是否被修改过
	bool m_status;
	string databaseName;
	string outDataPath;
	vector<string> imgFullNames;
	vector<Mat> globalFeat;
	//vector<Mat> localFeat;

	FeatureExactor *m_pFeatureExactor;
	//static vector<GlobalFeatureExactor*> globalFeatureExactor;

	CImgRetrDlg * m_pMainWnd;
	//CDatabaseCtrl * pDatabaseCtrl;
};


