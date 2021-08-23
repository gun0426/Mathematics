#pragma once

class MLineChartData
{
public:
	typedef struct __ITEM
	{
		double dOrigin;
		double dDest;
		double dIntv;
		bool bPrint;
	} __ITEM;

public:
	MLineChartData(void);
public:
	~MLineChartData(void);

public:
	CArray<__ITEM*, __ITEM*> lstData;
	double nYMin;
	double nYMax;
public:
	int Add(double dOrigin, double dDest, bool bPrint);	//*//
	void Clear();
};
