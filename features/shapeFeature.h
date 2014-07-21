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
#pragma comment(lib,"opencv_objdetect249d.lib")

#else
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
#pragma comment(lib,"opencv_objdetect249.lib")
#endif

class CCHuMoment:public GlobalFeatureExactor
{
public:
	CCHuMoment()
	{
		featType = GLOBAL_SHAPE_FEAT;
		//featName="HuMoment";
		featName="Hu矩";
	}
	void computeFeat(Mat &src, Mat &huMoment,int colorSpaceType=COLOR_SPACE_HLS, int normType=NORM_L1);
	int getFeatDim()
	{
		return 7;
	}
};

#define PI 3.141569254
// ed
class CEdgeHist : public GlobalFeatureExactor
{
public:
	CEdgeHist()
	{
		EDGE_HIST_SIZE=64;
		featName="边缘直方图";
		featType = GLOBAL_SHAPE_FEAT;
	}
	void computeFeat(Mat &src,Mat &edgeHist,int colorSpaceType=COLOR_SPACE_HLS,int normType=NORM_L1);
	int getFeatDim()
	{
		return EDGE_HIST_SIZE;
	}
private:
	int calEdgeHistogram(IplImage *image,/*IplImage * dst,*/float * edge_hist_feature);
	int EDGE_HIST_SIZE;
};


class HogFeat:public GlobalFeatureExactor
{
public:
	HogFeat():imgSize(128,128),blockSize(16,16), blockStride(8,8),
		cellSize(8,8), nbins(9),GlobalFeatureExactor(GLOBAL_SHAPE_FEAT,"梯度方向直方图")
	{	
		m_HogDescriptor=new HOGDescriptor(imgSize,blockSize,blockStride,cellSize,nbins);
	}
	HogFeat(Size _imgSize, Size _blockSize, Size _blockStride,
		Size _cellSize, int _nbins):imgSize(_imgSize),blockSize(_blockSize), 
		blockStride(_blockStride), cellSize(_cellSize),
		nbins(_nbins)
	{}
	~HogFeat()
	{
		delete m_HogDescriptor;
	}
	
	int getFeatDim()
	{
		return m_HogDescriptor->getDescriptorSize();
	}
	void computeFeat(Mat &src,Mat &hogFeat,int colorSpaceType=COLOR_SPACE_HLS,int normType=NORM_L1);

public:
	HOGDescriptor *m_HogDescriptor;
	Size imgSize;
	Size blockSize;
	Size blockStride;
	Size cellSize;
	int nbins;
	
};

