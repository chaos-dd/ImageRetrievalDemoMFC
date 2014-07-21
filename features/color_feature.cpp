#include "stdafx.h"
#include "color_feature.h"


/************************************************************************/
/* 颜色直方图类实现                                                                     */
/************************************************************************/
void ColorHist::initSteps()
{
	m_steps[0]=std::ceil((float)256/m_binsNum[0]);
	m_steps[1]=std::ceil((float)256/m_binsNum[1]);
	m_steps[2]=std::ceil((float)256/m_binsNum[2]);
}

void ColorHist::computeFeat(Mat &src, Mat &hist,int colorSpaceType, int normType)
{
	assert(src.channels()==3);
	assert(src.type() == CV_8UC3);

	hist = Mat::zeros(1,getFeatDim(),CV_32FC1);

	Mat img;

	if (colorSpaceType == COLOR_SPACE_HLS)
	{
		cvtColor(src,img,CV_BGR2HLS);
		for (int r=0; r < img.rows; ++r)
		{
			for (int c=0; c<img.cols;++c)
			{
				int tmp = img.at<Vec3b>(r,c)[0];
				img.at<Vec3b>(r,c)[0] = (float)tmp*2/360*255;
			}
		}
	}
	else
	{
		img=src;
	}

	imresizecrop(img,img,imgSize);
	initSteps();     


	for (int r = 0; r < img.rows ; ++r)
	{
		for (int c =0 ; c <img.cols ;++c)
		{

			Vec3b index=img.at<Vec3b>(r,c);
			index[0] = index[0]/m_steps[0];
			index[1] = index[1]/m_steps[1];
			index[2] = index[2]/m_steps[2];

			//根据颜色量化bin确定在结果直方图中的位置
			int location= index[0]*m_binsNum[1]*m_binsNum[2] + index[1]*m_binsNum[2] +index[2];
			hist.at<float>(location)  +=1;
		}
	}

	//L1 normalization

	//normalize(hist,hist,1.0,0,normType);


}


int ColorHist::getFeatDim()
{
	return m_binsNum[0]*m_binsNum[1]*m_binsNum[2];
}



/************************************************************************/
/*  class implementation of color coherence vector                                                  */
/************************************************************************/

int ColorCoherenceVec::getFeatDim()
{
	return m_binsNum[0]*m_binsNum[1]*m_binsNum[2]*2;
}

void ColorCoherenceVec::initSteps()
{
	m_steps[0]=std::ceil((float)256/m_binsNum[0]);
	m_steps[1]=std::ceil((float)256/m_binsNum[1]);
	m_steps[2]=std::ceil((float)256/m_binsNum[2]);
}


void ColorCoherenceVec::computeFeat(Mat &src,Mat &ccv,int colorSpaceType, int normType)
{
	assert(src.channels()==3);
	assert(src.type() == CV_8UC3);

	ccv = Mat::zeros(1,getFeatDim(),CV_32FC1);

	Mat img;

	if (colorSpaceType == COLOR_SPACE_HLS)
	{
		cvtColor(src,img,CV_BGR2HLS);
		for (int r=0; r < img.rows; ++r)
		{
			for (int c=0; c<img.cols;++c)
			{
				int tmp = img.at<Vec3b>(r,c)[0];
				img.at<Vec3b>(r,c)[0] = (float)tmp*2/360*255;
			}
		}
	}
	else
	{
		img=src;
	}

	imresizecrop(img,img,imgSize);

	initSteps();

	Mat bluredImg;
	//blur(img,bluredImg,Size(3,3));
	bluredImg=img;


	Mat flag=Mat::zeros(bluredImg.rows,bluredImg.cols,CV_8UC1);
	
	std::stack<Point> *pStack=new std::stack<Point>();

	for (int r =0; r < bluredImg.rows ;++r)
	{
		for (int c=0; c <bluredImg.cols ;++c)
		{


			if(flag.at<uchar>(r,c) == 0)
			{
				int connectedPixNum=0;


				Point pt(c,r);
				pStack->push(pt);
				flag.at<uchar>(pt)=1;

		
				RegionGrow(bluredImg,flag,pStack,connectedPixNum);

				Vec3b index=img.at<Vec3b>(pt);
				index[0] = index[0]/m_steps[0];
				index[1] = index[1]/m_steps[1];
				index[2] = index[2]/m_steps[2];

				//根据颜色量化bin确定在结果直方图中的位置
				int location= index[0]*m_binsNum[1]*m_binsNum[2] + index[1]*m_binsNum[2] +index[2];
				location *=2;


				//这里根据位置在把新得到的聚合像素或者非聚合像素加到相应位置，需要原始ccv矩阵是0初始化的
				if(connectedPixNum>=int(m_thresholdFactor*bluredImg.total()))
					ccv.at<float>(location) +=connectedPixNum;
				else
					ccv.at<float>(location+1) +=connectedPixNum;
			}
		}
	}
	//ccv /= img.total();
	//normalize(ccv,ccv,1.0,0,normType);
	delete pStack;
}

//四连通,img 原图像，flag标记是否访问过的图像
//访问下一个结点满足两个条件，一是没超边界并且每访问过，二是满足颜色相似

void ColorCoherenceVec::RegionGrow(Mat &img,Mat &flag,std::stack<Point>*& pStack,int &connectedPixNum)
{

	while (pStack->size() !=0)
	{
		Point pt=pStack->top();
		connectedPixNum +=1;
		//flag.at<uchar>(pt)=1;

		pStack->pop();

		Vec3b index=img.at<Vec3b>(pt);
		index[0] = index[0]/m_steps[0];
		index[1] = index[1]/m_steps[1];
		index[2] = index[2]/m_steps[2];

		//go up
		if (pt.y -1 >=0 && flag.at<uchar>(pt.y-1,pt.x)==0 )
		{
			Vec3b indexNext=img.at<Vec3b>(pt.y-1,pt.x);
			indexNext[0] = indexNext[0]/m_steps[0];
			indexNext[1] = indexNext[1]/m_steps[1];
			indexNext[2] = indexNext[2]/m_steps[2];
			if(index[0] == indexNext[0] && index[1] == indexNext[1] && index[2]==indexNext[2])
			{
				pStack->push(Point(pt.x,pt.y-1));
				flag.at<uchar>(Point(pt.x,pt.y-1))=1;
			}
		}
		//go down
		if (pt.y+1<img.rows && flag.at<uchar>(pt.y+1,pt.x)==0)
		{
			Vec3b indexNext=img.at<Vec3b>(pt.y+1,pt.x);
			indexNext[0] = indexNext[0]/m_steps[0];
			indexNext[1] = indexNext[1]/m_steps[1];
			indexNext[2] = indexNext[2]/m_steps[2];
			if(index[0] == indexNext[0] && index[1] == indexNext[1] && index[2]==indexNext[2])
			{
				pStack->push(Point(pt.x,pt.y+1));
				flag.at<uchar>(Point(pt.x,pt.y+1))=1;
			}
		}
		//go right
		if (pt.x+1<img.cols && flag.at<uchar>(pt.y,pt.x+1)==0)
		{
			Vec3b indexNext=img.at<Vec3b>(pt.y,pt.x+1);
			indexNext[0] = indexNext[0]/m_steps[0];
			indexNext[1] = indexNext[1]/m_steps[1];
			indexNext[2] = indexNext[2]/m_steps[2];
			if(index[0] == indexNext[0] && index[1] == indexNext[1] && index[2]==indexNext[2])
			{
				pStack->push(Point(pt.x+1,pt.y));
				flag.at<uchar>(Point(pt.x+1,pt.y))=1;
			}
		}
		//go left
		if (pt.x-1>=0 && flag.at<uchar>(pt.y,pt.x-1)==0)
		{
			Vec3b indexNext=img.at<Vec3b>(pt.y,pt.x-1);
			indexNext[0] = indexNext[0]/m_steps[0];
			indexNext[1] = indexNext[1]/m_steps[1];
			indexNext[2] = indexNext[2]/m_steps[2];
			if(index[0] == indexNext[0] && index[1] == indexNext[1] && index[2]==indexNext[2])
			{
				pStack->push(Point(pt.x-1,pt.y));
				flag.at<uchar>(Point(pt.x-1,pt.y))=1;
			}
		}
	}
}


/************************************************************************/
/* class implementation of color moment                                                               */
/************************************************************************/

void ColorMoment::computeFeat(Mat &src, Mat &colorMoment,int colorSpaceType,int normType)
{
	assert(src.channels() ==3);
	
	colorMoment = Mat::zeros(1,getFeatDim(),CV_32FC1);

	Mat img;

	if (colorSpaceType == COLOR_SPACE_HLS)
	{
		cvtColor(src,img,CV_BGR2HLS);
	}
	else
	{
		img=src;
	}

	imresizecrop(img,img,imgSize);

	//u = mean  v = second order moment variance, s = third order skewness
	float u1=0.0f,u2=0.0f,u3=0.0f,v1=0.0f,v2=0.0f,v3=0.0f,s1=0.0f,s2=0.0f,s3=0.0f;

	for (int r = 0 ; r < img.rows ; ++r)
	{
		Vec3b * tmp = img.ptr<Vec3b>(r);
		for (int c = 0; c <img.cols ; ++ c)
		{
			u1 += tmp[c][0];
			u2 += tmp[c][1];
			u3 += tmp[c][2];
		}
	}
	u1 /= img.total();
	u2 /= img.total();
	u3 /= img.total();

	for (int r = 0 ; r < img.rows ; ++r)
	{
		Vec3b * tmp = img.ptr<Vec3b>(r);
		for (int c = 0; c <img.cols ; ++ c)
		{
			v1 += std::powf(tmp[c][0]-u1,2);
			v2 += std::powf(tmp[c][1]-u1,2);
			v3 += std::powf(tmp[c][2]-u1,2);

			s1 += std::powf(tmp[c][0]-u1,3);
			s2 += std::powf(tmp[c][1]-u1,3);
			s3 += std::powf(tmp[c][2]-u1,3);
		}
	}

	v1 /= img.total(), v1= std::powf(v1,1/2);
	v2 /= img.total(), v2= std::powf(v2,1/2);
	v3 /= img.total(), v3= std::powf(v3,1/2);

	s1 /=img.total(), s1= std::powf(s1,1/3);
	s2 /=img.total(), s2= std::powf(s2,1/3);
	s3 /=img.total(), s3= std::powf(s3,1/3);

	//B
	colorMoment.at<float>(0)=u1;
	colorMoment.at<float>(1)=v1;
	colorMoment.at<float>(2)=s1;
	//G
	colorMoment.at<float>(3)=u2;
	colorMoment.at<float>(4)=v2;
	colorMoment.at<float>(5)=s2;
	//R
	colorMoment.at<float>(6)=u3;
	colorMoment.at<float>(7)=v3;
	colorMoment.at<float>(8)=s3;

	//normalize(colorMoment,colorMoment,1.0,0,normType);
}