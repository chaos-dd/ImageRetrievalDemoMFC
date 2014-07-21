
/********************************************************************
created:    2014/07/02
created:    2:7:2014   15:09
filename:   lbp.h
file path:  D:\CodeCollection\Global Feature\Global Feature
author:     Wangjun

purpose:    local binary pattern features 
*********************************************************************/

#pragma once

#include "../utilities/allInclude.h"
#include "../utilities/allInclude.h"

#include <opencv.hpp>
#include <iostream>
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

static   inline float& GetPixel32f( Mat  *p_poImage, int p_iRow, int p_iColumn )
{
	return ((float*)(p_poImage->data + p_poImage->step[0]*p_iRow))[p_iColumn];
}

static   inline uchar& GetPixel8u(Mat * p_poImage, int p_iRow, int p_iColumn )
{
	return ((uchar*)(p_poImage->data + p_poImage->step[0]*p_iRow))[p_iColumn];
}


class OLBP:public GlobalFeatureExactor
{
public:
	OLBP()
	{
		featType = GLOBAL_TEXTURE_FEAT;
		//featName="originalLBP";
		featName="LBP(原始)";
	}
	int getFeatDim()
	{
		return 256;
	}
	void computeFeat(Mat &img,Mat &feat,int colorSpaceType =COLOR_SPACE_HLS, int normType =NORM_L1 );
};


class ULBP:public GlobalFeatureExactor
{
public:
	ULBP()
	{
		featType = GLOBAL_TEXTURE_FEAT;
		//featName="UniformLBP";
		featName="均匀LBP";

		if (!flag)
		{
			init_uniform();
		}
	}
	int getFeatDim()
	{
		/* one bin for constant patterns, 8*7 for 2-uniform, one for rest */
		return 58;
	}
	//void computeFeat(Mat &img,Mat &feat,int colorSpaceType =COLOR_SPACE_HLS , int normType =NORM_L1 );
private:
	static void init_uniform();
protected:
	static uchar mapping[256];
	static bool flag;
};


class UniFormLBP:public ULBP
{
public:
	UniFormLBP()
	{
		featType = GLOBAL_TEXTURE_FEAT;
		featName="UniformLBP";
	}

	int getFeatDim()
	{
		/* one bin for constant patterns, 8*7 for 2-uniform, one for rest */
		return 58;
	}
	void computeFeat(Mat &img,Mat &feat,int colorSpaceType =COLOR_SPACE_HLS , int normType =NORM_L1 );
};



class CellUniFormLBP:public ULBP
{
public:
	CellUniFormLBP()
	{
		featType = GLOBAL_TEXTURE_FEAT;
		//featName="CellUniformLBP";
		featName="网格均匀LBP";

		binNum=4;
		//imageSize=512;
	}
	CellUniFormLBP(unsigned int _binNum,unsigned _imageSize):binNum(_binNum)
	{
		featType = GLOBAL_TEXTURE_FEAT;
		featName="CellUniformLBP";
	}

	int getFeatDim()
	{
		/* one bin for constant patterns, 8*7 for 2-uniform, one for rest */
		return 58 * binNum *binNum;
	}
	void computeFeat(Mat &img,Mat &feat,int colorSpaceType =COLOR_SPACE_HLS , int normType =NORM_L1 );

private:
	unsigned int binNum;
	//unsigned int imageSize;
};
