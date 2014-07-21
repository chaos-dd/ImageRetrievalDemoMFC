#include "stdafx.h"
#include "drawToDC.h"


CV_INLINE RECT NormalizeRect( RECT r );
CV_INLINE RECT NormalizeRect( RECT r )
{
	int t;
	if( r.left > r.right )
	{
		t = r.left;
		r.left = r.right;
		r.right = t;
	}
	if( r.top > r.bottom )
	{
		t = r.top;
		r.top = r.bottom;
		r.bottom = t;
	}

	return r;
}
CV_INLINE CvRect RectToCvRect( RECT sr );
CV_INLINE CvRect RectToCvRect( RECT sr )
{
	sr = NormalizeRect( sr );
	return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );
}


int Bpp(cv::Mat &img) 
{ 
	//if (img.data)
	//{
	//	int tmp = img.depth()&255;
	//	int tmp2=img.channels();
	//	return img.depth()&255  *img.channels();
	//}
	//else
	//	return 0;
	//return img.data ? (img.depth() & 255)*img.channels() : 0; 
	if (img.channels() ==1)
	{
		return 8;
	}
	else if (img.channels()==3)
	{
		return 24;
	}
	else if (img.channels()==4)
	{
		return 32;
	}
	else
		return 0;
}


void  FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void  DrawToHDC(cv::Mat &img, HDC hDCDst, RECT* pDstRect ) 
{
	if( pDstRect  && (img.depth() == CV_8U) && img.data )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = img.cols, bmp_h = img.rows;
		CvRect roi;
		roi.x=0;
		roi.y=0;
		roi.width=img.cols;
		roi.height=img.rows;
		
		CvRect dst = RectToCvRect( *pDstRect );

		if( roi.width > dst.width )
		{
			SetStretchBltMode(
				hDCDst,           // handle to device context
				HALFTONE );
		}
		else
		{
			SetStretchBltMode(
				hDCDst,           // handle to device context
				COLORONCOLOR );
		}
		//FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), img->origin );
		FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(img), 0);
		::StretchDIBits(
			hDCDst,
			dst.x, dst.y, dst.width, dst.height,
			roi.x, roi.y, roi.width, roi.height,
			img.data, bmi, DIB_RGB_COLORS, SRCCOPY );
	}
}