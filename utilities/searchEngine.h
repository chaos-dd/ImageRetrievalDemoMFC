#pragma once 

#include "DatabaseInfo.h"
#include "../features/FeatureExactor.h"
#include <algorithm>

#define DISTANCE_L1  0
#define DISTANCE_L2  1
#define DISTANCE_HIST_INTERSECTION  2
#define DISTANCE_COSINE  3


struct ImgInfo
{
	string imgName;
	float dist;
};

inline bool comp(ImgInfo img1,ImgInfo img2)
{
	return img1.dist > img2.dist;
}


class SearchEngine
{
public:
	SearchEngine()
	{
		m_featureExactor=new FeatureExactor();
		databaseInfo=NULL;
	}
	~SearchEngine()
	{
		delete databaseInfo;
		
		delete m_featureExactor;
	}

	bool setDatabase(DatabaseInfo *_databaseInfo);
	bool AddImageToDatabase(string imgPath);
	bool singleSearch(string _imgPath,vector<ImgInfo> &toSort,vector<int> &featIndex=vector<int>(),vector<float> &featWeights=vector<float>(),int distanceType=DISTANCE_L1);
	void batchSearch();

	float computeDist(Mat &arr1,Mat &arr2,int distanceType);
	

public:
	FeatureExactor *m_featureExactor;
	DatabaseInfo * databaseInfo;	
	vector<ImgInfo> m_searchResult;
};
