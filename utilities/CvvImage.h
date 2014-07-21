#pragma once
#ifndef CVVIMAGE_CLASS_DEF
#define CVVIMAGE_CLASS_DEF
#include <opencv.hpp>
/* CvvImage class definition */
class  CvvImage
{
public:
   CvvImage();
   virtual ~CvvImage();
   /* Create image (BGR or grayscale) */
   bool  Create( int width, int height, int bits_per_pixel, int image_origin = 0 );
   /* Load image from specified file */
   bool  Load( const char* filename, int desired_color = 1 );
   /* Load rectangle from the file */
   bool  LoadRect( const char* filename,
      int desired_color, CvRect r );
#if defined WIN32 || defined _WIN32
   bool  LoadRect( const char* filename,
      int desired_color, RECT r )
   {
      return LoadRect( filename, desired_color,
         cvRect( r.left, r.top, r.right - r.left, r.bottom - r.top ));
   }
#endif
   /* Save entire image to specified file. */
   bool  Save( const char* filename );
   /* Get copy of input image ROI */
   void  CopyOf( CvvImage& image, int desired_color = -1 );
   void  CopyOf( IplImage* img, int desired_color = -1 );
   IplImage* GetImage() { return m_img; };
   void  Destroy(void);
   /* width and height of ROI */
   int Width() { return !m_img ? 0 : !m_img->roi ? m_img->width : m_img->roi->width; };
   int Height() { return !m_img ? 0 : !m_img->roi ? m_img->height : m_img->roi->height;};
   int Bpp();
   void  Fill( int color );
   /* draw to highgui window */
   void  Show( const char* window );

#if defined WIN32 || defined _WIN32
   /* draw part of image to the specified DC */
   void  Show( HDC dc, int x, int y, int width, int height,
      int from_x = 0, int from_y = 0 );
   /* draw the current image ROI to the specified rectangle of the destination DC */
   void  DrawToHDC( HDC hDCDst, RECT* pDstRect );
#endif
//protected:
   IplImage*  m_img;
};
//typedef CvvImage CImage;
#endif
