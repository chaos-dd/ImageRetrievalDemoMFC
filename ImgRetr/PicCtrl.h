#pragma once

#include <string>

#include <opencv.hpp>
#include "../utilities/searchEngine.h"
using namespace std;
// CPicCtrl

class CPicCtrl 
{
	//DECLARE_DYNAMIC(CPicCtrl)

public:
	CPicCtrl(CWnd *pParent);
	virtual ~CPicCtrl();

protected:
	//DECLARE_MESSAGE_MAP()


public:
	
	afx_msg void OnStnClicked();
	void ShowImage(string imgPath);
	void ShowImage(ImgInfo &imgInfo);
	void ShowImage(cv::Mat &img);
	void ReDrawImage(CPaintDC *dc);
	//void MoveWindow(CRect &rc);
	//void MoveWindow(int x, int y, int nWidth, int nHeight);
	afx_msg void OnPaint();

	CWnd * m_parentWnd;
	int m_picID;	
	CStatic *m_pStaticPic;
	
	
	int m_staticID;
	CStatic *m_picSimScore;

	string m_imgName;
	cv::Mat m_imgToShow;
	float m_simScore;
};





