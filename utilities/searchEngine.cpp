#include "stdafx.h"
#include "searchEngine.h"
#include <Windows.h>

CMutex g_mutex;

bool SearchEngine::setDatabase(DatabaseInfo *_databaseInfo)
{
	CSingleLock singleLock(&g_mutex);

	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("请等待上一个操作完成！"),_T("错误"),MB_ICONERROR);
		return false;
	}
	delete databaseInfo;
	databaseInfo=_databaseInfo;

	singleLock.Unlock();
	return true;
}

float SearchEngine::computeDist(Mat &arr1,Mat &arr2,int distanceType)
{
	float dist=0.0f;
	switch (distanceType)
	{
	case DISTANCE_L1:
		{
			normalize(arr1,arr1,1.0,0,NORM_L1);
			normalize(arr2,arr2,1.0,0,NORM_L1);
			for (int i = 0 ; i != arr1.total();++i)
			{
				float tmp1= arr1.at<float>(i);
				float tmp2= arr2.at<float>(i);
				dist +=fabs( arr1.at<float>(i) - arr2.at<float>(i));

			}
			dist /=2;
		}
		break;
	case DISTANCE_L2:
		{
			normalize(arr1,arr1,1.0,0,NORM_L2);
			normalize(arr2,arr2,1.0,0,NORM_L2);
			for (int i = 0 ; i != arr1.total();++i)
			{
				dist +=pow( arr1.at<float>(i) - arr2.at<float>(i),2);

			}
			dist = sqrt(dist);
		}
		break;
	case DISTANCE_HIST_INTERSECTION:
		{			
			normalize(arr1,arr1,1.0,0,NORM_L1);
			normalize(arr2,arr2,1.0,0,NORM_L1);
			for (int i = 0 ; i != arr1.total();++i)
			{
				dist += std::min(arr1.at<float>(i) ,arr2.at<float>(i));
			}
			dist = 1-dist;
		}
		break;
	case DISTANCE_COSINE:
		{
			normalize(arr1,arr1,1.0,0,NORM_L2);
			normalize(arr2,arr2,1.0,0,NORM_L2);
			float sum1=0.0f,sum2=0.0f;
			for (int i = 0 ; i != arr1.total();++i)
			{
				sum1+=powf(arr1.at<float>(i),2.0f);
				sum2+=powf(arr2.at<float>(i),2.0f);
				dist += arr1.at<float>(i) * arr2.at<float>(i);
			}
			sum1 = sqrt(sum1);
			sum2 = sqrt(sum2);
			dist = dist/sum1/sum2;
			dist = 1-dist;
		}
		break;
	default:
		;
	}

	return 1-dist;
}

bool SearchEngine::AddImageToDatabase(string imgPath)
{
	CSingleLock singleLock(&g_mutex);

	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("请等待上一个操作完成！"),_T("错误"),MB_ICONERROR);
		return false;
	}

	databaseInfo->AddImages(imgPath);
	singleLock.Unlock();
	return true;
}
//#define DISTANCE_L1  0
//#define DISTANCE_L2  1
//#define DISTANCE_HIST_INTERSECTION  2
//#define DISTANCE_COSINE  3

bool SearchEngine::singleSearch(string _imgPath,vector<ImgInfo> &toSort,vector<int> &featIndex,vector<float> &featWeights,int distanceType)
{

	CSingleLock singleLock(&g_mutex);

	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("请等待上一个操作完成！"),_T("错误"),MB_ICONERROR);
		return false ;
	}

	if (featIndex.size()==0)
	{
		featIndex.resize(m_featureExactor->m_vecGlobalFeatureExactor.size());
		for (int i = 0 ; i !=featIndex.size();++i)
		{
			featIndex[i]=i;
		}
	}

	//vec每个元素是一种全局特征，其中的Mat的行是一幅图像的特征
	vector<Mat> vec_feat;
	m_featureExactor->exactSingleImgFeat(_imgPath,vec_feat,featIndex);

	vector<vector<float>> dist(vec_feat.size(),vector<float>(databaseInfo->imgFullNames.size()));
	
	for (int i = 0 ; i !=vec_feat.size(); ++i)
	{
		for (int j = 0 ; j != databaseInfo->imgFullNames.size() ;++j)
		{
			dist[i][j]=computeDist(vec_feat[i],databaseInfo->globalFeat[featIndex[i]].row(j),distanceType);
		}
	}

	//初始化特征权重
	if (featWeights.size() == 0)
	{
		featWeights.resize(featIndex.size());
		for (int i = 0 ; i !=featWeights.size();++i)
		{
			featWeights[i]=1/(float)vec_feat.size();
		}
	}

	//feature fusion 
	vector<float> fusedDist(databaseInfo->imgFullNames.size(),0.0f);

	for (int i = 0 ; i != vec_feat.size() ;++i)
	{
		for (int j = 0 ; j !=databaseInfo->imgFullNames.size(); ++j)
		{
			fusedDist[j] += dist[i][j] * featWeights[i];
		}
	}

	//sort 

	//vector<ImgInfo> toSort(fusedDist.size());
	toSort.clear();
	toSort=vector<ImgInfo>(fusedDist.size());
	for (int i = 0 ; i != fusedDist.size(); ++i)
	{
		toSort[i].imgName=databaseInfo->imgFullNames[i];
		toSort[i].dist=fusedDist[i];
	}

	std::sort(toSort.begin(),toSort.end(),comp);

	singleLock.Unlock();

	return true;
}
