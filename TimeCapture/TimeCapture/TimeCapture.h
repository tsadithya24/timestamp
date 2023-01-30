#pragma once
#include "stdint.h"

#ifdef TIMESTAMP_EXPORTS
#define TIMESTAMP_API __declspec(dllexport)
#else
#define TIMESTAMP_API __declspec(dllimport)
#endif


TIMESTAMP_API BOOL AddTimeStamp(UCHAR* SrcFrame, int SrcWidth, int SrcHeight, UCHAR* DestFrame);

