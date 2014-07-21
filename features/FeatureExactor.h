#pragma once 

#include "../utilities/allInclude.h"

#include "globalFeatureExactor.h"
#include "../features/color_feature.h"
#include "../features/lbp.h"
#include "../features/shapeFeature.h"
#include "../utilities/FindFiles.h"
#include <opencv.hpp>

//#include "../ImgRetr/ImgRetrDlg.h"
#include <iomanip>
#include <vector>
using namespace std;
using namespace cv;

class CImgRetrDlg;

class FeatureExactor
{
public:
	FeatureExactor()
	{
		//初始化顺序不能改变，先颜色特征，后纹理特征，最后形状特征
		//颜色
		m_vecGlobalFeatureExactor.push_back(new ColorHist());
		m_vecGlobalFeatureExactor.push_back(new ColorCoherenceVec());
		m_vecGlobalFeatureExactor.push_back(new ColorMoment());

		//纹理
		m_vecGlobalFeatureExactor.push_back(new CellUniFormLBP());
		m_vecGlobalFeatureExactor.push_back(new OLBP());
		m_vecGlobalFeatureExactor.push_back(new UniFormLBP());
		

		//形状
		m_vecGlobalFeatureExactor.push_back(new CEdgeHist());
		m_vecGlobalFeatureExactor.push_back(new HogFeat());
		m_vecGlobalFeatureExactor.push_back(new CCHuMoment());



	
	}
	~FeatureExactor()
	{
		for (int i = 0 ; i != m_vecGlobalFeatureExactor.size(); ++i)
		{
			delete m_vecGlobalFeatureExactor[i];
		}
	}
	int getGlobalFeatNum()
	{
		return m_vecGlobalFeatureExactor.size();
	}
	void exactBatchImgFeat(string imgFolderPath,vector<string> &imgFullNames,vector<Mat> & vec_feat,CImgRetrDlg *pMainWnd);
	void exactSingleImgFeat(string imgPath,vector<Mat> &vec_feat,vector<int> &featIndex=vector<int>());

	
public:
	vector<GlobalFeatureExactor*>  m_vecGlobalFeatureExactor;
};


