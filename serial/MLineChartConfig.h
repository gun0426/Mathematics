#pragma once

#define CHART_DATA_N	300

class MLineChartConfig
{
public:
	MLineChartConfig(void);
public:
	~MLineChartConfig(void);

public:
	int nInitWidth;
	int nWidthExtent;

	COLORREF crDot;
	COLORREF crDataLine[CHART_DATA_N];
	COLORREF crBG;
	COLORREF crGuideLine;
	COLORREF crAxis;
	COLORREF crText;
	COLORREF crTextBG;

	BOOL bShowGuideLine;
	BOOL bShowDataLine;

	BOOL bAxisYSensitive;
	double dblAxisYMin;
	double dblAxisYMax;

	// LEFT, BOTTOM Margin에 축 정보가 표시됨.
	int nMarginTop;
	int nMarginLeft;
	int nMarginBottom;
	int nMarginRight;

	CString szAxisXNM;
	CString szAxisYNM;
	CString szFormat;
};
