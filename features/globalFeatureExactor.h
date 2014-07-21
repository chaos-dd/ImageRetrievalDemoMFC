#pragma once 

#include "../utilities/allInclude.h"
#include <opencv.hpp>

//#include "FeatureExactor.h"

using namespace cv;
class GlobalFeatureExactor
{
public:
	GlobalFeatureExactor()
	{
		imgSize=Size(512,512);
	}
	GlobalFeatureExactor(int _featType, string _featName):featType(_featType),featName(_featName)
	{
		imgSize=Size(512,512);
	}
	GlobalFeatureExactor(int _featType, string _featName,Size _imgSize):featType(_featType),featName(_featName),imgSize(_imgSize)
	{}
	virtual int getFeatDim()=0;
	virtual void computeFeat(Mat &img,Mat &feat,int colorSpaceType=COLOR_SPACE_HLS, int normType=NORM_L1)=0;
	virtual ~GlobalFeatureExactor()
	{
	}

protected:
	void imresizecrop(Mat &src, Mat &dst,Size _imgSize);
public:
	int featType;
	string featName;
	Size imgSize;
};

