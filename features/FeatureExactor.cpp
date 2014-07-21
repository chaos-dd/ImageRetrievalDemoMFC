#include "stdafx.h"
#include "FeatureExactor.h"
#include "../ImgRetr/resource.h"
#include "../ImgRetr/ImgRetrDlg.h"
#include "../utilities/searchEngine.h"
#include "../ImgRetr/DatabaseCtrl.h"
void FeatureExactor::exactBatchImgFeat(string imgFolderPath,vector<string> &imgFullNames,vector<Mat> & vec_feat,CImgRetrDlg *pMainWnd)
{
	//三种特征 预留三个空间
	vec_feat.resize(getGlobalFeatNum());	

	vector<string> tmpImgNames;

	FindFiles::findFilesInDirAndSubdirs(tmpImgNames,imgFolderPath);

	CProgressCtrl* pProg = (CProgressCtrl*)pMainWnd->m_databaseCtrl->GetDlgItem(IDC_PROGRESS);
	CStatusBarCtrl * pStatus = &pMainWnd->m_statusBar;

	pProg->SetPos(0);
	//int progress=0;
	for (int i = 0 ;  i !=tmpImgNames.size() ;++i)
	{
		if ( i %50 ==0 )
		{
			cout<<"提取第  "<< setiosflags(ios::right) <<setw(10)<<i <<"幅图像特征"<<endl;
		}
		Mat img = imread(tmpImgNames[i]);
		if (img.empty())
		{
			continue;
		}

		pProg->SetRange32(0,tmpImgNames.size()-1);
		//progress +=(int)10000/(float)tmpImgNames.size();
		pProg->OffsetPos(1);
		//pProg->SetPos(progress);
		pStatus->SetWindowTextA(tmpImgNames[i].c_str());
	
		
		//struct MyMsgParam
		//{
		//	int total;
		//	int index;
		//	string imgName;
		//};

		imgFullNames.push_back(tmpImgNames[i]);

		Mat imgFeat;
		for (int j = 0 ; j != getGlobalFeatNum() ; ++j)
		{
			m_vecGlobalFeatureExactor[j]->computeFeat(img,imgFeat);
			vec_feat[j].push_back(imgFeat);
		}
	}

	pStatus->SetWindowTextA(_T("完成！"));
}

void FeatureExactor::exactSingleImgFeat(string imgPath,vector<Mat> &vec_feat,vector<int> &featIndex)
{

	if (featIndex.size()==0)
	{
		featIndex.resize(m_vecGlobalFeatureExactor.size());
		for (int i = 0 ; i !=featIndex.size();++i)
		{
			featIndex[i]=i;
		}
	}
	Mat img = imread(imgPath);
	//vector<Mat> vec_feat;

	if (img.empty())
	{
		throw runtime_error(string("读取图像  "+imgPath +" 失败 \n"));
	}


	Mat tmp_feat;
	//三种特征 预留三个空间
	vec_feat.resize(featIndex.size());
	for (int i = 0 ; i !=featIndex.size();++i)
	{
		m_vecGlobalFeatureExactor[featIndex[i]]->computeFeat(img,tmp_feat);
		vec_feat[i].push_back(tmp_feat);
	}
}

