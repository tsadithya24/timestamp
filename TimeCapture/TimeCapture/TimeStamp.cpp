#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <ctime>
#include "TimeCapture.h"

BOOL AddTimeStamp(unsigned char* SrcFrame, int SrcWidth, int SrcHeight, unsigned char* DestFrame)
{
	HDC hDC = GetDC(NULL);
	HDC hDCMem = CreateCompatibleDC(hDC);

	unsigned char* NewData = (unsigned char*)malloc(sizeof(unsigned char) * (SrcWidth * SrcHeight * 2));
	
	// Create a bitmap to hold the image data
	BITMAPINFOHEADER bmpInfoHeader;
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = SrcWidth;
	bmpInfoHeader.biHeight = -SrcHeight;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 32;
	//bmpInfoHeader.biCompression = BI_RGB;

	HBITMAP hBitmap = CreateDIBSection(hDCMem, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS, (void**)&NewData, NULL, 0);
	HGDIOBJ hOldBitmap = SelectObject(hDCMem, hBitmap);

	// Copy image data from SrcFrame to DestFrame
	
	memcpy(NewData, SrcFrame, SrcWidth * SrcHeight * 2);

   //Get current timestamp
	time_t current_time;
	char* c_time_string;
	current_time = time(NULL);
	c_time_string = ctime(&current_time);

  // Set background color to transparent
	SetBkMode(hDCMem, TRANSPARENT);
	SetTextColor(hDCMem, RGB(255, 255, 255));

	//// Add timestamp to image

	TextOutA(hDCMem, 10, 10, c_time_string, strlen(c_time_string));

	//BitBlt(hDC, 0, 0, SrcWidth, SrcHeight, hDCMem, 0, 0, SRCCOPY);
	
	for (int i = 0; i < SrcWidth * SrcHeight * 2; i++)
	{
		DestFrame[i] = NewData[i];
	}

	//free(NewData);
	SelectObject(hDCMem, hOldBitmap);
	DeleteDC(hDCMem);
	DeleteObject(hBitmap);
	DeleteObject(hOldBitmap);
	ReleaseDC(NULL, hDC);
	ReleaseDC(NULL, hDCMem);

	/*memcpy(DestFrame, NewData, SrcWidth * SrcHeight * 2);*/

	

	//free(NewData);
	return TRUE;
}

//BOOL AddTimeStamp(UCHAR* SrcFrame, int SrcWidth, int SrcHeight, UCHAR* DestFrame)
//{
//	if (SrcFrame == NULL || DestFrame == NULL)
//	{
//		return FALSE;
//	}
//
//	RECT m_rcMainRect;
//	m_rcMainRect.left = 0;
//	m_rcMainRect.top = 0;
//	m_rcMainRect.right = SrcWidth;
//	m_rcMainRect.bottom = SrcHeight;
//
//	HDC hDCMem = CreateCompatibleDC(NULL);
//
//	unsigned char* lpBitmapBits;
//
//	BITMAPINFO bi;
//	ZeroMemory(&bi, sizeof(BITMAPINFO));
//	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bi.bmiHeader.biWidth = m_rcMainRect.right;
//	bi.bmiHeader.biHeight = -m_rcMainRect.bottom;  //negative so (0,0) is at top left
//	bi.bmiHeader.biPlanes = 1;
//	bi.bmiHeader.biBitCount = 32;
//
//	HBITMAP bitmap = ::CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);
//	HGDIOBJ oldbmp = ::SelectObject(hDCMem, bitmap);
//
//	 Copy source frame to the bitmap
//	for (int i = 0; i < SrcWidth * SrcHeight; i++)
//	{
//		lpBitmapBits[i] = SrcFrame[i];
//	}
//
//	 Get current timestamp
//	time_t current_time;
//	char* c_time_string;
//	current_time = time(NULL);
//	c_time_string = ctime(&current_time);
//
//	 Add timestamp to image
//	TextOutA(hDCMem, 10, 10, c_time_string, strlen(c_time_string));
//
//	HDC hDestDC = CreateCompatibleDC(NULL);
//
//	BITMAPINFO destbi;
//	ZeroMemory(&destbi, sizeof(BITMAPINFO));
//	destbi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	destbi.bmiHeader.biWidth = SrcWidth;
//	destbi.bmiHeader.biHeight = -SrcHeight;
//	destbi.bmiHeader.biPlanes = 1;
//	destbi.bmiHeader.biBitCount = 32;
//
//	HBITMAP destBitmap = ::CreateDIBSection(hDestDC, &destbi, DIB_RGB_COLORS, (VOID**)&DestFrame, NULL, 0);
//	HGDIOBJ oldDestBmp = ::SelectObject(hDestDC, destBitmap);
//
//	 Copy the bitmap with timestamp to the destination frame
//	BitBlt(hDestDC, 0, 0, SrcWidth, SrcHeight, hDCMem, 0, 0, SRCCOPY);
//
//	BITMAPFILEHEADER bmfh;
//	BITMAPINFOHEADER bmih;
//	memset(&bmfh, 0, sizeof(bmfh));
//	memset(&bmih, 0, sizeof(bmih));
//	bmfh.bfType = 0x4d42; // "BM"
//	bmfh.bfOffBits = sizeof(bmfh) + sizeof(bmih);
//	bmih.biSize = sizeof(bmih);
//	bmih.biWidth = m_rcMainRect.right;
//	bmih.biHeight = -m_rcMainRect.bottom;
//	bmih.biPlanes = 1;
//	bmih.biBitCount = 32;
//	bmih.biSizeImage = SrcHeight * SrcWidth;
//
//	 Create a file to save the image
//	HANDLE hFile = CreateFileA("screenshot.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		printf("Error: Unable to create file\n");
//		return 1;
//	}
//
//	 Write the file headers and image data to the file
//	DWORD dwBytesWritten;
//	if (WriteFile(hFile, &bmfh, sizeof(bmfh), &dwBytesWritten, NULL) == FALSE)
//	{
//		printf("\nerror writing file header");
//	}
//	if (WriteFile(hFile, &bmih, sizeof(bmih), &dwBytesWritten, NULL) == FALSE)
//	{
//		printf("\nerror writing file info header");
//	}
//	if (WriteFile(hFile, lpBitmapBits, bmih.biSizeImage, &dwBytesWritten, NULL) == FALSE)
//	{
//		printf("\nerror saving file");
//	}
//
//	 Close the file
//	CloseHandle(hFile);
//
//	SelectObject(hDestDC, oldDestBmp);
//	DeleteDC(hDestDC);
//	DeleteObject(destBitmap);
//
//	SelectObject(hDCMem, oldbmp);
//	DeleteDC(hDCMem);
//	DeleteObject(bitmap);
//
//	return TRUE;
//}

//BOOL AddTimeStamp(unsigned char* SrcFrame ,int SrcWidth, int SrcHeight, unsigned char* DestFrame)
//{
//	RECT m_rcMainRect;
//	m_rcMainRect.left = 0;
//	m_rcMainRect.top = 0;
//	m_rcMainRect.right = 500;
//	m_rcMainRect.bottom = 200;
//
//	HDC hDC = GetDC(NULL);
//	HDC hDCMem = CreateCompatibleDC(hDC);
//
//	unsigned char* DestImage = DestFrame;
//	unsigned char* SrcImage = SrcFrame;
//
//	BITMAPINFO bi;
//	ZeroMemory(&bi, sizeof(BITMAPINFO));
//	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bi.bmiHeader.biWidth = m_rcMainRect.right;
//	bi.bmiHeader.biHeight = -m_rcMainRect.bottom;  //negative so (0,0) is at top left
//	bi.bmiHeader.biPlanes = 1;
//	bi.bmiHeader.biBitCount = 32;
//
//	HBITMAP bitmap = ::CreateDIBSection(hDCMem, &bi, DIB_RGB_COLORS, (VOID**)&DestImage, NULL, 0);
//	HGDIOBJ oldbmp = ::SelectObject(hDCMem, bitmap);
//
//	BitBlt(hDCMem, 0, 0, m_rcMainRect.right, m_rcMainRect.bottom, hDC, 0, 0, SRCCOPY);
//
//	int pitch = 4 * m_rcMainRect.right; // 4 bytes per pixel but if not 32 bit, round pitch up to multiple of 4
//	int Srcindex;
//	int Destindex;
//
//	for (int x = 0; x < m_rcMainRect.right; x++)
//	{
//		for (int y = 0; y < m_rcMainRect.bottom; y++)
//		{
//			Srcindex = y * pitch;
//			Srcindex += x * 4;
//
//			Destindex = y * pitch;
//			Destindex += x * 4;
//
//			DestImage[Destindex + 0] = SrcImage[Srcindex + 0]; 
//			DestImage[Destindex + 1] = SrcImage[Srcindex + 0]; 
//			DestImage[Destindex + 2] = SrcImage[Srcindex + 0];  
//		}
//	}
//
//	// Get current timestamp
//	time_t current_time;
//	char* c_time_string;
//	current_time = time(NULL);
//	c_time_string = ctime(&current_time);
//
//	// Add timestamp to image
//	TextOutA(hDCMem, 10, 10, c_time_string, strlen(c_time_string));
//
//	BitBlt(hDC, 0, 0, m_rcMainRect.right, m_rcMainRect.bottom, hDCMem, 0, 0, SRCCOPY);
//
//	SelectObject(hDCMem, oldbmp);
//	DeleteDC(hDCMem);
//	DeleteObject(bitmap);
//	ReleaseDC(NULL, hDC);
//
//	return 0;
//	return TRUE;
//}

//cv::Point position(0,0);
//int x, y = 0;
//int thickness = 2;
//double font_scale = 1.0;
//
//BOOL position_set = FALSE;
//BOOL Add_Text = FALSE;
//BOOL DateTime = FALSE;
//
//BOOL ModifyFrame(Mat SrcFrame, Mat DestFrame)
//{
//	std::time_t now = std::time(nullptr);
//	char timestamp[20];
//	std::tm tm;
//	localtime_s(&tm, &now);
//	std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);
//	int baseLine;
//    cv::Size textSize = cv::getTextSize(timestamp, cv::FONT_HERSHEY_SIMPLEX, font_scale, thickness, &baseLine);
//
//		if (SrcFrame.data)
//		 {
//		  if (DateTime)
//		  {
//		    if (position_set)
//		     {
//			  SrcFrame.copyTo(DestFrame);
//		      cv::putText(DestFrame, timestamp, position, cv::FONT_HERSHEY_SIMPLEX, font_scale, cv::Scalar(250, 10, 120), thickness);
//		     }
//		    else
//		     {
//			  SrcFrame.copyTo(DestFrame);
//		      cv::putText(DestFrame, timestamp, cv::Point(0, 0), cv::FONT_HERSHEY_SIMPLEX, font_scale, cv::Scalar(250, 10, 120), thickness);
//		     }
//		  }
//		  else
//		  {
//			  SrcFrame.copyTo(DestFrame); 
//		  }
//		   return TRUE;
//		 }
//		else
//		 {
//		  return FALSE;
//		 }
//}
//
//BOOL AddLocation(int x_coordinate, int y_coordinate)
//{
//	if (x_coordinate > 0 && y_coordinate > 0)
//	{
//		x = x_coordinate;
//		y = y_coordinate;
//
//		position = cv::Point(x_coordinate, y_coordinate);
//		position_set = TRUE;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//BOOL AddSize(int thick, double size)
//{	
//	if (size > 0)
//	{
//		font_scale = (double)size / 100;
//		thickness = thick;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//BOOL Enable_DateTime( BOOL AddDateTime )
//{
//	DateTime = AddDateTime;
//}
//
//BOOL AddText(TCHAR Text)
//{
//	
//}
//
//
//BOOL Reset()
//{
//
//}
//
