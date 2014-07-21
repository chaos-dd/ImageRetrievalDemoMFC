#include "stdafx.h"
#include "DatabaseInfo.h"
#include <iomanip>
#include <fstream>

using namespace std;

void DatabaseInfo::AddImages(string imgPath)
{
	
	vector<string> curImgFullNames;
	
	m_pFeatureExactor->exactBatchImgFeat(imgPath,curImgFullNames,globalFeat,m_pMainWnd);
	imgFullNames.insert(imgFullNames.end(),curImgFullNames.begin(),curImgFullNames.end());
	if(curImgFullNames.size()!=0)
		m_status = true;
}


void DatabaseInfo::SaveFeat()
{
	vector<GlobalFeatureExactor*> &globalFeatureExactor= m_pFeatureExactor->m_vecGlobalFeatureExactor;
	for (int i=0; i !=globalFeatureExactor.size(); ++i)
	{
		string outFileName = string(outDataPath+'\\'+globalFeatureExactor[i]->featName + ".dat");
		
		//FILE * file=fopen(outFileName.c_str(),"wb");
		fstream file(outFileName,ios::out | ios::binary);

		if (!file)
		{
			string msg="打开 输出 特征文件" + outFileName + "失败！\n";
			throw runtime_error(msg.c_str());
		}

		int rows = globalFeat[i].rows;
		int cols = globalFeat[i].cols;
		int nCount = globalFeat[i].rows*globalFeat[i].cols*sizeof(float);
		//char* buf = (char*)globalFeat[i].data;
		
		file.write((char*)&rows,sizeof(int));
		file.write((char*)&cols,sizeof(int));
		file.write( (char*) globalFeat[i].data,nCount);

		//fprintf(file,"%12d   %10d\n",globalFeat[i].rows, globalFeat[i].cols);

		//file<<globalFeat[i].rows<<" "<<globalFeat[i].cols<<'\n';

		//for (int row = 0 ; row != globalFeat[i].rows ; ++row)
		//{
		//	for(int col = 0 ; col != globalFeat[i].cols ; ++col)
		//	{
				//fprintf(file,"%10.10f    ", globalFeat[i].at<float>(row,col));
				//file<<globalFeat[i].at<float>(row,col)<<" ";
		//	}
			//fprintf(file,"\n");
		//}
		
		//fclose(file);
		file.close();
	}
}


void DatabaseInfo::SaveDatabase()
{
	if (imgFullNames.size() ==0)
	{
		return;
	}

	this->SaveFeat();

	string outFileName = string(outDataPath+"\\imgFullNames.dat");


	//FILE * file=fopen(outFileName.c_str(),"wb");
	fstream file(outFileName,ios::out|ios::binary);
	if (!file)
	{
		string msg="打开 输出 特征文件" + outFileName + "失败！\n";
		throw runtime_error(msg.c_str());
	}

	//fprintf(file,"%12d\n",imgFullNames.size());


	//file<<imgFullNames.size()<<'\n';

	int num =imgFullNames.size();
	file.write( (char*)&num, sizeof(int));

	for (int i = 0 ; i != imgFullNames.size() ; ++i)
	{
		//fprintf(file,"%s\n",imgFullNames[i].c_str());
		
		//file<<imgFullNames[i]<<'\n';
		int strLen=imgFullNames[i].size();
		file.write((char*)&strLen,sizeof(int));
		file.write(imgFullNames[i].c_str(),imgFullNames[i].size());
	}
	//fclose(file);
	file.close();
}

void DatabaseInfo::LoadFeat()
{
	for (int i=0; i != m_pFeatureExactor->getGlobalFeatNum(); ++i)
	{
		string outFileName = string(outDataPath+'\\'+m_pFeatureExactor->m_vecGlobalFeatureExactor[i]->featName + ".dat");
		//FILE * file = fopen(outFileName.c_str(),"rb");

		fstream file(outFileName,ios::in|ios::binary);

		if (!file)
		{
			string msg="打开 读取 特征文件" + outFileName + "失败！\n";
			throw runtime_error(msg.c_str());
		}

		int rows,cols;
		//char tmp;
		//fscanf(file,"%d   %d\n",&rows, &cols);
		//file>>rows>>cols;

		file.read((char*)&rows,sizeof(int));
		file.read((char*)&cols,sizeof(int));

		globalFeat[i]=Mat::zeros(rows,cols,CV_32FC1);

		file.read((char*)globalFeat[i].data,rows*cols*sizeof(float));

		//globalFeat[i]=Mat::zeros(rows,cols,CV_32FC1);

		//for (int row = 0 ; row != globalFeat[i].rows ; ++row)
		//{
		//	for(int col = 0 ; col != globalFeat[i].cols ; ++col)
		//	{
		//		float tmp;
		//		//fscanf(file,"%f", &tmp);
		//		file>>tmp;
		//		globalFeat[i].at<float>(row,col) = tmp;
		//	}
		//}
		////fclose(file);
		file.close();
	}

}

void DatabaseInfo::LoadDatabase()
{

	this->LoadFeat();


	string outFileName = string(outDataPath+"\\imgFullNames.dat");
	
	//FILE * file = fopen(outFileName.c_str(),"r");
	fstream file(outFileName,ios::in | ios::binary);
	if (!file)
	{
		string msg="打开 图像名读取文件 " + outFileName + "  失败！\n";
		throw runtime_error(msg.c_str());
	}

	int imgNum;
	//char tmp;
	//file>>imgNum;

	file.read((char*)&imgNum,sizeof(int));
	//fscanf(file,"%d",&imgNum);
	imgFullNames.resize(imgNum);


	for (int i = 0 ; i != imgFullNames.size() ; ++i)
	{
		//char tmpImgName[charBufSize]={0};
		//fgets会把换行符也读进来
		//fgets(tmpImgName,charBufSize,file);
		//fscanf(file,"%s\n",tmpImgName);
		//string str=string(tmpImgName);
		//imgFullNames[i]=string(str.begin(),str.end()-1);

		///string str;
		//getline(file,str,'\n');
		//if (str.empty())
		//{
		//	continue;
		//}

		char buf[MAXBUFSIZE] ={0};
		int strLen;
		file.read((char*)&strLen,sizeof(int));
		file.read(buf,strLen);
		imgFullNames[i]=string(buf);
	}

	file.close();
}