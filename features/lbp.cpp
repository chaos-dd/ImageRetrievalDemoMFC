/********************************************************************
	created:    2014/07/02
	created:    2:7:2014   15:07
	filename:   lbp.cpp
	file path:  D:\CodeCollection\Global Feature\Global Feature
	author:     Wangjun
	
	purpose:    LBP class implementation
*********************************************************************/
#include "stdafx.h"

#include "lbp.h"



/************************************************************************/
/*  implementation of class original lbp   */
/************************************************************************/


void OLBP::computeFeat(Mat &img,Mat &feat,int colorSpaceType/* =COLOR_SPACE_HLS */, int normType/* =NORM_L1 */)
{
	Mat src;
	if (img.channels()==3)
	{
		cvtColor(img,src,CV_BGR2GRAY);
	}
	else
		src=img;

	assert(img.depth()==CV_8U);
	imresizecrop(img,img,imgSize);
	feat = Mat::zeros(1,getFeatDim(),CV_32FC1);


	
	for (int r=1;r != img.rows-1 ;++r)
	{
		for (int c= 1; c!=img.cols-1 ; ++c)
		{
			unsigned int bitString = 0;
			uchar center=GetPixel8u(&img,r,c);

			if (GetPixel8u(&img,r,c+1)> center) bitString |=0x1<<0;         /* E */
			if (GetPixel8u(&img,r+1,c+1)> center) bitString |=0x1<<1;     /* SE */
			if (GetPixel8u(&img,r+1,c)> center) bitString |=0x1<<2;         /* S */
			if (GetPixel8u(&img,r+1,c-1)> center) bitString |=0x1<<3;      /* SW */
			if (GetPixel8u(&img,r,c-1)> center) bitString |=0x1<<4;          /* W */
			if (GetPixel8u(&img,r-1,c-1)> center) bitString |=0x1<<5;        /* NW */
			if (GetPixel8u(&img,r-1,c)> center) bitString |=0x1<<6;           /* N */
			if (GetPixel8u(&img,r-1,c+1)> center) bitString |=0x1<<7;       /* NE */

			GetPixel32f(&feat,0,bitString) +=1;
			//feat.at<float>(bitString)  +=1;
		}
	}

}


/************************************************************************/
/* implementation of class ulbp                                                                     */
/************************************************************************/

uchar ULBP::mapping[256]={0};
bool ULBP::flag=false;


void ULBP::init_uniform()
{
	int i, j ;

	/* all but selected patterns map to 57 */
	for (i = 0 ; i < 256 ; ++i) {
		mapping[i] = 57 ;
	}

	/* uniform patterns map to 56 */
	mapping[0x00] = 56 ;
	mapping[0xff] = 56 ;

	/* now uniform pattenrs, in order */
	/* locations: 0:E, 1:SE, 2:S, ..., 7:NE */
	for (i = 0 ; i < 8 ; ++i) { /* string[i-1]=0, string[i]=1 */
		for (j = 1 ; j <= 7 ; ++j) { /* length of sequence of ones */
			/* string starting with j ones */
			int unsigned string = (1 << j) - 1 ;
			/* put i zeroes in front */
			string <<= i ;
			/* wrap around 8 bit boundaries */
			string = (string | (string >> 8)) & 0xff ;

			mapping[string] = i * 7 + (j-1) ;
		}
	}
}


/************************************************************************/
/* implementation of class UniFormLBP                                                                      */
/************************************************************************/




void UniFormLBP::computeFeat(Mat &img,Mat &feat,int colorSpaceType/* =COLOR_SPACE_HLS */, int normType/* =NORM_L1 */)
{
	Mat src;
	if (img.channels()==3)
	{
		cvtColor(img,src,CV_BGR2GRAY);
	}
	else
	{
		src=img;
	}

	assert(img.depth()==CV_8U);
	imresizecrop(img,img,imgSize);

	feat = Mat::zeros(1,getFeatDim(),CV_32FC1);


	
	for (int r=1;r != img.rows-1 ;++r)
	{
		for (int c= 1; c!=img.cols-1 ; ++c)
		{
			unsigned int bitString = 0;
			uchar center = img.at<uchar>(r,c);

			if (GetPixel8u(&img,r,c+1)> center)
				bitString     |=0x1<<0;         /* E */
			if (GetPixel8u(&img,r+1,c+1)> center) 
				bitString   |=0x1<<1;     /* SE */
			if (GetPixel8u(&img,r+1,c)> center)
				bitString       |=0x1<<2;         /* S */
			if (GetPixel8u(&img,r+1,c-1)> center) 
				bitString   |=0x1<<3;      /* SW */
			if (GetPixel8u(&img,r,c-1)> center) 
				bitString    |=0x1<<4;          /* W */
			if (GetPixel8u(&img,r-1,c-1)> center)
				bitString    |=0x1<<5;        /* NW */
			if (GetPixel8u(&img,r-1,c)> center) 
				bitString   |=0x1<<6;           /* N */
			if (GetPixel8u(&img,r-1,c+1)> center) 
				bitString   |=0x1<<7;       /* NE */



			feat.at<float>(mapping[bitString]) +=1;
		}
	}
}


/************************************************************************/
/* implementation of cell uniform lbp                                                                     */
/************************************************************************/


void CellUniFormLBP::computeFeat(Mat &img,Mat &feat,int colorSpaceType , int normType )
{
	
	Mat src;
	if (img.channels()==3)
	{
		cvtColor(img,src,CV_BGR2GRAY);
	}
	else
	{
		src=img;
	}

	assert(img.depth()==CV_8U);
	imresizecrop(img,img,imgSize);

	feat = Mat::zeros(1,getFeatDim(),CV_32FC1);

	
	//imresizecrop(src,src,Size(imageSize,imageSize));

	uchar *image = (uchar*) src.data;

	unsigned int width = src.cols;
	unsigned int height = src.rows;
	unsigned cellSize = floor((float)width/binNum);

	float * features=(float*)feat.data;

	unsigned int cwidth = width / cellSize;
	unsigned int cheight = height / cellSize ;
	unsigned int cstride = cwidth * cheight ;
	unsigned int cdimension = 58 ;
	int x,y,cx,cy,k,bin ;

#define at(u,v) (*(image + width * (v) + (u)))
#define to(u,v,w) (*(features + cstride * (w) + cwidth * (v) + (u)))

	/* accumulate pixel-level measurements into cells */
	for (y = 1 ; y < (signed)height - 1 ; ++y) {
		float wy1 = (y + 0.5f) / (float)cellSize - 0.5f ;
		int cy1 = (int) floor(wy1) ;
		int cy2 = cy1 + 1 ;
		float wy2 = wy1 - (float)cy1 ;
		wy1 = 1.0f - wy2 ;
		if (cy1 >= (signed)cheight) continue ;

		for (x = 1 ; x < (signed)width - 1; ++x) {
			float wx1 = (x + 0.5f) / (float)cellSize - 0.5f ;
			int cx1 = (int) floor(wx1) ;
			int cx2 = cx1 + 1 ;
			float wx2 = wx1 - (float)cx1 ;
			wx1 = 1.0f - wx2 ;
			if (cx1 >= (signed)cwidth) continue ;

			{
				int unsigned bitString = 0 ;
				float center = at(x,y) ;
				
				if(at(x+1,y+0) > center) bitString |= 0x1 << 0; /*  E */
				if(at(x+1,y+1) > center) bitString |= 0x1 << 1; /* SE */
				if(at(x+0,y+1) > center) bitString |= 0x1 << 2; /* S  */
				if(at(x-1,y+1) > center) bitString |= 0x1 << 3; /* SW */
				if(at(x-1,y+0) > center) bitString |= 0x1 << 4; /*  W */
				if(at(x-1,y-1) > center) bitString |= 0x1 << 5; /* NW */
				if(at(x+0,y-1) > center) bitString |= 0x1 << 6; /* N  */
				if(at(x+1,y-1) > center) bitString |= 0x1 << 7; /* NE */
				bin = mapping[bitString] ;
			}

			if ((cx1 >= 0) & (cy1 >=0)) {
				to(cx1,cy1,bin) += wx1 * wy1;
			}
			if ((cx2 < (signed)cwidth)  & (cy1 >=0)) {
				to(cx2,cy1,bin) += wx2 * wy1 ;
			}
			if ((cx1 >= 0) & (cy2 < (signed)cheight)) {
				to(cx1,cy2,bin) += wx1 * wy2 ;
			}
			if ((cx2 < (signed)cwidth) & (cy2 < (signed)cheight)) {
				to(cx2,cy2,bin) += wx2 * wy2 ;
			}
		} /* x */
	} /* y */

	/* normalize cells */
	for (cy = 0 ; cy < (signed)cheight ; ++cy) {
		for (cx = 0 ; cx < (signed)cwidth ; ++ cx) {
			float norm = 0 ;
			for (k = 0 ; k < (signed)cdimension ; ++k) {
				norm += features[k * cstride]*features[k * cstride] ;
			}
			norm = sqrtf(norm) + 1e-10f; ;
			for (k = 0 ; k < (signed)cdimension ; ++k) {
				features[k * cstride] = sqrtf(features[k * cstride]) / norm  ;
			}
			features += 1 ;
		}
	} /* next cell to normalize */
}
