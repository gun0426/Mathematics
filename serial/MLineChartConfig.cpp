#include "StdAfx.h"
#include "MLineChartConfig.h"

MLineChartConfig::MLineChartConfig(void)
{
	nInitWidth = 10;
	nWidthExtent = 1000;

	crDot = RGB(255, 0, 0);
	crDataLine[0] = RGB(255, 0, 0);
	crDataLine[1] = RGB(0, 0, 255);
	crDataLine[2] = RGB(0, 255, 0);
	crDataLine[3] = RGB(255, 255, 0);
	crDataLine[4] = RGB(255, 0, 255);
	crDataLine[5] = RGB(0, 255, 255);
	crDataLine[6] = RGB(200, 100, 100);
	crDataLine[7] = RGB(100, 100, 200);
	crDataLine[8] = RGB(50, 100, 150);
	crDataLine[9] = RGB(150, 100, 50);
#if (0)
	crBG = RGB(255, 255, 255);
	crGuideLine = RGB(128, 128, 128);
	crAxis = RGB(0, 0, 0);
	crText = RGB(0, 0, 0);
	crTextBG = RGB(255, 255, 255);
#else
	crBG = RGB(30, 30, 30);
	crGuideLine = RGB(128, 128, 128);
	crAxis = RGB(255, 255, 255);
	crText = RGB(255, 255, 255);
	crTextBG = RGB(30, 30, 30);
#endif
	bShowGuideLine = FALSE;
	bShowDataLine = TRUE;

	bAxisYSensitive = TRUE;
	dblAxisYMin = 0;
	dblAxisYMax = 0;

	nMarginTop = 10;	// 30;
	nMarginLeft = 50;	// 50;
	nMarginBottom = 28;	// 40;
	nMarginRight = 30;	// 20;

	szAxisXNM = "";		//"Axis X";
	szAxisYNM = "";		//"Delta ¥Ä";		// "Axis Y";
	szFormat = "%.2f";	//"%.1f";
}

MLineChartConfig::~MLineChartConfig(void)
{
}
