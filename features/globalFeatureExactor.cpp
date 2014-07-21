#include "stdafx.h"
#include "globalFeatureExactor.h"

void GlobalFeatureExactor::imresizecrop(Mat &src, Mat &dst,Size _imgSize)
{
	Mat temp_img;

	float scaling = std::max((float)_imgSize.height/src.rows,(float)_imgSize.width/src.cols);
	Size sz;
	sz.height = cvRound(scaling*src.rows);
	sz.width = cvRound(scaling*src.cols);
	Mat tmp;
	cv::resize(src,tmp,sz);

	int sr=floor((float)(tmp.rows- _imgSize.height)/2);
	int sc=float((float)(tmp.cols- _imgSize.width)/2);

	if (src.type()==CV_8UC3)
	{
		dst=Mat(_imgSize,CV_8UC3);

		for (int r=0; r<  _imgSize.height; ++r)
		{
			for (int c=0 ; c<_imgSize.width; ++c)
			{
				*(dst.data + r*dst.cols +c) = *(tmp.data + (r+sr)*tmp.cols +c+sc);
				//dst.at<Vec3b>(r,c) = tmp.at<Vec3b>(r+sr,c+sc);
			}
		}
	}
	else if (src.type()==CV_8UC1)
	{
		dst=Mat(_imgSize,CV_8UC1);

		for (int r=0; r<  _imgSize.height; ++r)
		{
			for (int c=0 ; c<_imgSize.width; ++c)
			{
				*(dst.data + r*dst.cols +c) = *(tmp.data + (r+sr)*tmp.cols +c+sc);
				//dst.at<uchar>(r,c) = tmp.at<uchar>(r+sr,c+sc);
			}
		}
	}
}