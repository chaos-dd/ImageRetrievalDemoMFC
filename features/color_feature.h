#pragma once

#include "../utilities/allInclude.h"

#include <opencv.hpp>
#include <iostream>
#include <stack>
#include <queue>

//#include "FeatureExactor.h"
#include "globalFeatureExactor.h"

using namespace std;
using namespace cv;

#ifdef _DEBUG 

#pragma comment(lib,"opencv_core249d.lib")
#pragma comment(lib,"opencv_imgproc249d.lib")
#pragma comment(lib,"opencv_highgui249d.lib")
#else
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
#endif



/************************************************************************/
/* 颜色直方图类                                                                     */
/************************************************************************/


class ColorHist:public GlobalFeatureExactor
{
public:
	ColorHist()
	{
		m_binsNum[0] = 6;
		m_binsNum[1] = 3;
		m_binsNum[2] = 4;
		featType=GLOBAL_COLOR_FEAT;
		//featName=string("ColorHist");
		featName=string("颜色直方图");
	}
	ColorHist(int binsNum1, int binsNum2, int binsNum3)
	{
		m_binsNum[0]=binsNum1;
		m_binsNum[1]=binsNum2;
		m_binsNum[2]=binsNum3;
		featType=GLOBAL_COLOR_FEAT;
		featName=string("ColorHist");
	}
	void computeFeat(Mat &src, Mat &hist,int colorSpaceType=COLOR_SPACE_HLS, int normType=NORM_L1);
	int getFeatDim();

private:
	void initSteps();

public:
	int m_binsNum[3];
	int m_steps[3];
};

/************************************************************************/
/* 颜色聚合向量                                                                     */
/************************************************************************/

class ColorCoherenceVec:public GlobalFeatureExactor
{
public:
	ColorCoherenceVec()
	{
		m_binsNum[0]=6;
		m_binsNum[1]=3;
		m_binsNum[2]=4;
		m_thresholdFactor=0.01f;
		featType=GLOBAL_COLOR_FEAT;
		//featName = string("ColorCoherenceVec");
		featName = string("颜色聚合向量");
	}
	ColorCoherenceVec(int binsNum1, int binsNum2, int binsNum3,float threshold = 0.01)
	{
		m_binsNum[0]=binsNum1;
		m_binsNum[1]=binsNum2;
		m_binsNum[2]=binsNum3;
		m_thresholdFactor=threshold;
		featType=GLOBAL_COLOR_FEAT;
		featName = string("ColorCoherenceVec");
	}
	void computeFeat(Mat &src,Mat &ccv,int colorSpaceType = COLOR_SPACE_HLS,int normType=NORM_L1);
	int getFeatDim();

private:
	void initSteps();
	void RegionGrow(Mat &img,Mat &flag,std::stack<Point>* &pStack,int &connectedPixNum);

public:
	int m_binsNum[3];
	int m_steps[3];
	float m_thresholdFactor;    //连通区域像素计算比例，默认图像总像素的1%
};


/************************************************************************/
/* 颜色矩特征                                                                     */
/************************************************************************/
class ColorMoment :public GlobalFeatureExactor
{
public:
	ColorMoment()
	{
		//m_colorSpaceType=COLOR_SPACE_HLS;
		featType=GLOBAL_COLOR_FEAT;
		//featName = string("ColorMoment");
		featName = string("颜色矩");
	}
	//ColorMoment(int colorSpaceType)
	//{
		//m_colorSpaceType=colorSpaceType;
	//}
	void computeFeat(Mat &src, Mat &colorMoment,int colorSpaceType= COLOR_SPACE_HLS,int normType=NORM_L1);
	int getFeatDim()
	{
		return 9;
	}
	//int m_colorSpaceType;
};

