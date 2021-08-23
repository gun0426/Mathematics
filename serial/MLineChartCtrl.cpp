// MLineChartCtrl.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "resource.h"
#include "MLineChartCtrl.h"
#include "config.h"
#include "serialDlg.h"



extern HWND hCommWnd;


//CserialDlg* pMainDlg;
//CserialDlg* pMainDlg = (CserialDlg*)::AfxGetMainWnd();
//CserialDlg* pMainDlg = (CserialDlg*)GetParent();


// MLineChartCtrl

IMPLEMENT_DYNAMIC(MLineChartCtrl, CWnd)

MLineChartCtrl::MLineChartCtrl()
: m_iGrphX(0)
, m_iGrphN(0)
, m_iItemN(0)
, m_iGrphH(0)
, m_bLBtnDbClk(false)
, m_bMBtnDbClk(false)
, m_iBKColor_i(0)
, m_nViewSplit(0)
, m_bF2Evnt(false)
, m_iViewMaxN(0)
, m_bLClkDown(false)
, m_nBuffer(0)
, m_nViewPeriod(0)
, nXDataTotal(0)
, m_iXth(0)
, n2PiCycle(0)
, xGraph(0)
, dblZMax(0)
, iZMulStep(0)
{
	m_hMemBitmap = NULL;
	m_hMemDC = NULL;
}

MLineChartCtrl::~MLineChartCtrl()
{
}


BEGIN_MESSAGE_MAP(MLineChartCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// MLineChartCtrl 메시지 처리기입니다.
void MLineChartCtrl::OnPaint()	// GUN_MARK
{
	CPaintDC dc(this);		// device context for painting
	HDC hDC = dc.m_hDC;

	for (int grph_i = 0; grph_i< m_iGrphN; grph_i++)
	{	
		for (int item_i = 0; item_i < m_iItemN; item_i++)
		{
			int size = m_aData[grph_i*m_iItemN+item_i].lstData.GetSize();
			if (size > m_iViewMaxN)
			{
				for (int cnt = 0; cnt < size - m_iViewMaxN; cnt++)
				{
					m_aData[grph_i * m_iItemN + item_i].lstData.RemoveAt(0);
				}
			}
		}
	}

	GetClientRect(rcWindow);
	m_iGrphH = rcWindow.Height() / m_iGrphN;
//	DrawBG(m_hMemDC);
	for (int i = 0; i < m_iGrphN; i++)
	{
		m_iGrphX = i;
		m_dYMax = m_aData[i*m_iItemN].nYMax;
		m_dYMin = m_aData[i*m_iItemN].nYMin;
		DrawChart(m_hMemDC);
	}
	/************************************************
	BOOL BitBlt(HDC hdcDest,int nXDest,int nYDest,int nWidth,int nHeight,HDC hdcSrc,int nXSrc,int nYSrc,DWORD dwRop);

	hdcDest	: 이미지를 출력할 위치의 핸들
	nXDest	: 이미지를 출력할 위치 x 좌표
	nYDest	: 이미지를 출력할 위치 y 좌표
	nWidth	: 이 크기만큼 원본 이미지에서 잘라와 그림
	nHeight	: 이 크기만큼 원본 이미지에서 잘라와 그림
	hdcSrc	: 이미지의 핸들
	nXSrc	: 가져올 이미지의 시작지점(x)
	nYSrc	: 가져올 이미지의 시작지점(y)
	dwRop	: 이미지의 출력 방법(dwRop)


	BOOL StretchBlt(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,int nWidthSrc,int nHeightSrc,DWORD dwRop);

	hdcDest			: 이미지를 출력할 위치의 핸들입니다.
	nXOriginDest	: 이미지를 출력할 위치인 x좌표입니다.
	nYOriginDest	: 이미지를 출력할 위치인 y좌표입니다.
	nWidthDest		: 출력할 이미지의 너비 - 이 크기만큼 hdcSrc로부터 받은 이미지를 이 너비와 높이에 맞게 이미지 크기를 변경합니다.
	nHeightDest		: 출력할 이미지의 높이 - 이 크기만큼 hdcSrc로부터 받은 이미지를 이 너비와 높이에 맞게 이미지 크기를 변경합니다
	hdcSrc			: 이미지의 핸들입니다.
	nXOriginSrc		: 가져올 이미지의 시작지점인 x좌표입니다.- 지정한 위치부터 nWidthSrc, nHeightSrc만큼 이미지를 잘라옵니다.
	nYOriginSrc		: 가져올 이미지의 시작지점인 y좌표입니다.- 지정한 위치부터 nWidthSrc, nHeightSrc만큼 이미지를 잘라옵니다.
	nWidthSrc		: 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 너비
	nHeightSrc		: 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 높이
	dwRop			: 이미지의 출력 방법입니다.

		************************************************/
	if (m_nViewSplit == 0)
	{
		BitBlt(hDC, 0, 0, rcWindow.Width(), rcWindow.Height(), m_hMemDC, 0, 0, SRCCOPY);
	}
	else
	{
		int x_start = (int)(rcData.left + (rcData.Width()*(double)m_nViewSplit*(1.0f / SPLIT_N)));
		BitBlt(hDC, x_start, 0, rcData.right - x_start, rcWindow.Height(), m_hMemDC, x_start, 0, SRCCOPY);
	}
	//StretchBlt(hDC, 50, 0, rcWindow.Width()-(stt_n-100), rcWindow.Height(), m_hMemDC, 50, 0, rcWindow.Width()-50, rcWindow.Height(), SRCCOPY);
}

void MLineChartCtrl::DrawChart(HDC hDC)
{
	static int iCnt = 0;

	// CREATE FONT
	HFONT hFont, hOldFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	//strcpy_s(lf.lfFaceName, "Verdana");
	strcpy_s(lf.lfFaceName, "Consolas");
	lf.lfHeight = 13;
	hFont = CreateFontIndirect(&lf);
	hOldFont = (HFONT)SelectObject(hDC, hFont);

	CalcDataRect();
	CalcAxis();

	DrawBG(hDC);
	DrawAxisX(hDC);
	DrawAxisY(hDC);
	DrawAxisLine(hDC);
	DrawData(hDC);
	DrawMouseData(hDC);

	CString szText;
	switch (xGraph)
	{
		case 0:
			szText.Format("sin(x)");
			TextOut(hDC, rcData.left - 30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("π");
			TextOut(hDC, rcData.right, rcData.bottom + 5, szText, szText.GetLength());
			break;
		case 1:
			szText.Format("Euler e");
			TextOut(hDC, rcData.left - 30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("n");
			TextOut(hDC, rcData.right, rcData.bottom + 20, szText, szText.GetLength());
			break;
		case 2:
			szText.Format("exp(x)");
			TextOut(hDC, rcData.left - 30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("x");
			TextOut(hDC, rcData.right, rcData.bottom + 20, szText, szText.GetLength());
			break;
		case 3:
			szText.Format("Complex Number");
			TextOut(hDC, rcData.CenterPoint().x - 30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("Im");
			TextOut(hDC, rcData.left - 30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("Re");
			TextOut(hDC, rcData.right, rcData.bottom + 20, szText, szText.GetLength());
			break;
		case 4:
			szText.Format("Euler's Formula");
			TextOut(hDC, rcData.CenterPoint().x-30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("Im");
			TextOut(hDC, rcData.left - 30, rcData.top - 30, szText, szText.GetLength());
			szText.Format("Re");
			TextOut(hDC, rcData.right, rcData.bottom + 20, szText, szText.GetLength());
			break;
	}
	// DELETE FONT
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void MLineChartCtrl::CalcDataRect()
{
	// CALC DATA RECT
	if (xGraph != 0)
	{
		rcData.left		= rcWindow.left;
		rcData.right	= rcWindow.left + rcWindow.Height();
		rcData.top		= rcWindow.top;
		rcData.bottom	= rcWindow.bottom;
		rcData.left		+= m_ChartConfig.nMarginLeft + 5;
		rcData.right	-= m_ChartConfig.nMarginLeft + 5;
		rcData.top		+= m_ChartConfig.nMarginLeft + 5;
		rcData.bottom	-= m_ChartConfig.nMarginLeft + 5;
	}
	else
	{
		rcData.left		= rcWindow.left			+ m_ChartConfig.nMarginLeft;
		rcData.right	= rcWindow.right		- m_ChartConfig.nMarginRight;
		rcData.top		= m_iGrphH*m_iGrphX		+ m_ChartConfig.nMarginTop;
		rcData.bottom	= m_iGrphH*(m_iGrphX+1) - m_ChartConfig.nMarginBottom;
		rcData.left		+= rcData.bottom - rcData.top;	/////
	}
}

void MLineChartCtrl::CalcAxis()
{
	// X축 계산	
	nXDataSize = m_aData[m_iGrphX * m_iItemN].lstData.GetSize();

	switch (xGraph)
	{
		case 0:
			dblXIntv = (double)rcData.Width() / (double)nXDataSize;
			dblYIntv = (double)rcData.Height() / (double)(GRID_N_0);
			break;
		case 1:
		case 2:
			dblXIntv = (double)rcData.Width() / (double)nXDataSize;
			dblYIntv = (double)rcData.Height() / (double)(GRID_N_1);
			break;
		case 3:
		case 4:
			dblXIntv = (double)rcData.Width() / (dblZMax*2.0);
			dblYIntv = dblXIntv;
			break;
		case 5:
			dblXIntv = rcData.left+rcData.Width()/36;	// width
			dblYIntv = rcData.left+rcData.Height()/3;	// height
			dblXRoom = (dblXIntv/1.5);
			dblYRoom = (dblXIntv*1.2);
			break;
	}

	// Y축 계산
	for (int i = 0; i<m_iItemN; i++)
	{
		if (m_bItemEn[i] == TRUE)
		{
			if (m_aData[m_iGrphX*m_iItemN + i].nYMax > m_dYMax)
			{
				m_dYMax = m_aData[m_iGrphX*m_iItemN + i].nYMax;
			}
			if (m_aData[m_iGrphX*m_iItemN + i].nYMin < m_dYMin)
			{
				m_dYMin = m_aData[m_iGrphX*m_iItemN + i].nYMin;
			}
		}
	}
	dblYCenter		= (m_dYMax + m_dYMin) / 2;
	dblYDataSize	= m_dYMax - m_dYMin;
	dblYMin			= m_dYMin - dblYDataSize*.05;	
	dblYMax			= m_dYMax + dblYDataSize*.05;	
	dblYDataSize	= dblYMax - dblYMin;	
	dblYPixelSize	= rcData.Height() / dblYDataSize;
}

void MLineChartCtrl::DrawBG(HDC hDC)
{
	HBRUSH hBrush = CreateSolidBrush(m_ChartConfig.crBG);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	FillRect(hDC, rcWindow, hBrush);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);
}

void MLineChartCtrl::DrawAxisLine(HDC hDC)
{
	// DRAW AXIS LINE	
	HPEN hAxisLine = CreatePen(PS_SOLID, 1, m_ChartConfig.crAxis);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hAxisLine);
	HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	if ((xGraph == 3) || (xGraph == 4))
	{
		MoveToEx(hDC, rcData.left, rcData.CenterPoint().y, NULL);
		LineTo(hDC, rcData.right, rcData.CenterPoint().y);
		MoveToEx(hDC, rcData.CenterPoint().x, rcData.bottom, NULL);
		LineTo(hDC, rcData.CenterPoint().x, rcData.top);
	}
	else if (xGraph == 5)
	{
		for (int k=0; k<2; k++)
		{
			for (int i = 0; i < 6; i++)
			{
				/* External Rectangle */
				Rectangle(hDC, (int)(rcData.left + i*(dblXRoom + dblXIntv) + 0),
					(int)(rcData.top + k*(dblYIntv + dblYRoom) + 0),
					(int)(rcData.left + i*(dblXRoom + dblXIntv) + dblXIntv - 0),
					(int)(rcData.top + dblYIntv + k*(dblYIntv + dblYRoom) - 0));

				int x = (int)((rcData.left + i*(dblXRoom + dblXIntv) + rcData.left + i*(dblXRoom + dblXIntv) + dblXIntv) / 2);
				int y = (int)(rcData.top + dblYIntv + k*(dblYIntv + dblYRoom) + 50);

#if (1)
				CString szText;
				if (k == 0)
				{
					Rectangle(hDC, x - 25, y - 25, x + 25, y + 25);
					switch (i)
					{
						case 0 :
							Ellipse(hDC, x +  0 - 5, y +  0 - 5, x +  0 + 5, y +  0 + 5);
							break;
						case 1:
							Ellipse(hDC, x +  0 - 5, y +  0 - 5, x +  0 + 5, y +  0 + 5);
							Ellipse(hDC, x + 12 - 5, y - 12 - 5, x + 12 + 5, y - 12 + 5);
							break;
						case 2:
							Ellipse(hDC, x +  0 - 5, y +  0 - 5, x +  0 + 5, y +  0 + 5);
							Ellipse(hDC, x + 12 - 5, y - 12 - 5, x + 12 + 5, y - 12 + 5);
							Ellipse(hDC, x - 12 - 5, y + 12 - 5, x - 12 + 5, y + 12 + 5);
							break;
						case 3:
							Ellipse(hDC, x + 12 - 5, y - 12 - 5, x + 12 + 5, y - 12 + 5);
							Ellipse(hDC, x - 12 - 5, y + 12 - 5, x - 12 + 5, y + 12 + 5);
							Ellipse(hDC, x - 12 - 5, y - 12 - 5, x - 12 + 5, y - 12 + 5);
							Ellipse(hDC, x + 12 - 5, y + 12 - 5, x + 12 + 5, y + 12 + 5);
							break;
						case 4:
							Ellipse(hDC, x +  0 - 5, y +  0 - 5, x +  0 + 5, y +  0 + 5);
							Ellipse(hDC, x + 12 - 5, y - 12 - 5, x + 12 + 5, y - 12 + 5);
							Ellipse(hDC, x - 12 - 5, y + 12 - 5, x - 12 + 5, y + 12 + 5);
							Ellipse(hDC, x - 12 - 5, y - 12 - 5, x - 12 + 5, y - 12 + 5);
							Ellipse(hDC, x + 12 - 5, y + 12 - 5, x + 12 + 5, y + 12 + 5);
							break;
						case 5:
							Ellipse(hDC, x + 12 - 5, y - 12 - 5, x + 12 + 5, y - 12 + 5);
							Ellipse(hDC, x - 12 - 5, y + 12 - 5, x - 12 + 5, y + 12 + 5);
							Ellipse(hDC, x - 12 - 5, y - 12 - 5, x - 12 + 5, y - 12 + 5);
							Ellipse(hDC, x + 12 - 5, y + 12 - 5, x + 12 + 5, y + 12 + 5);
							Ellipse(hDC, x - 12 - 5, y -  0 - 5, x - 12 + 5, y -  0 + 5);
							Ellipse(hDC, x + 12 - 5, y +  0 - 5, x + 12 + 5, y +  0 + 5);
							break;
						case 6:
							Ellipse(hDC, x + 12 - 5, y - 12 - 5, x + 12 + 5, y - 12 + 5);
							Ellipse(hDC, x - 12 - 5, y + 12 - 5, x - 12 + 5, y + 12 + 5);
							Ellipse(hDC, x - 12 - 5, y - 12 - 5, x - 12 + 5, y - 12 + 5);
							Ellipse(hDC, x + 12 - 5, y + 12 - 5, x + 12 + 5, y + 12 + 5);
							Ellipse(hDC, x - 12 - 5, y -  0 - 5, x - 12 + 5, y -  0 + 5);
							Ellipse(hDC, x + 12 - 5, y +  0 - 5, x + 12 + 5, y +  0 + 5);
							break;
					}
				}
				else
				{
					HFONT hFont, hOldFont;
					LOGFONT lf;
					memset(&lf, 0, sizeof(lf));
					strcpy_s(lf.lfFaceName, "Georgia");
					lf.lfHeight = 20;
					hFont = CreateFontIndirect(&lf);
					hOldFont = (HFONT)SelectObject(hDC, hFont);

					switch (i)
					{
						case 0:
							Ellipse(hDC, x - 25, y - 25, x + 25, y + 25);
							szText = _T("H");
							TextOut(hDC, x - 6, y - 10, szText, szText.GetLength());
							break;
						case 1:
							Ellipse(hDC, x - 25, y - 25, x + 25, y + 25);
							szText = _T("T");
							TextOut(hDC, x - 6, y - 10, szText, szText.GetLength());
							break;
						case 2:
							break;
						case 3:
							break;
						case 4:
							break;
						case 5:
							break;
					}
					SelectObject(hDC, hOldFont);
					DeleteObject(hFont);
				}
#endif
			}
		}
	}
	else
	{
		MoveToEx(hDC, rcData.left, rcData.top, NULL);
		LineTo(hDC, rcData.left, rcData.bottom);
		LineTo(hDC, rcData.right, rcData.bottom);
	}
	
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hAxisLine);
	DeleteObject(hBrush);
}

void MLineChartCtrl::DrawAxisX(HDC hDC)
{
	SetTextColor(hDC, m_ChartConfig.crText);
	SetBkColor(hDC, m_ChartConfig.crTextBG);
	HPEN hAxisLine = CreatePen(PS_SOLID, 1, m_ChartConfig.crGuideLine);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hAxisLine);
	CString szText;
	SIZE size;

	if ((xGraph == 3) || (xGraph == 4))
	{
		for (int i = 0; i < (dblZMax * 2) + 1; i++)
		{
			MoveToEx(hDC, rcData.left + (int)(dblYIntv*i), (int)(rcData.top), NULL);
			LineTo(hDC, rcData.left + (int)(dblYIntv*i), (int)(rcData.bottom));

			szText.Format("%d", (int)(-dblZMax)+i);
			TextOut(hDC, (int)(rcData.left + dblXIntv*i)+3, (int)rcData.CenterPoint().y+3, szText, szText.GetLength());
			GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
		}
		SelectObject(hDC, hOldPen);
		DeleteObject(hAxisLine);
		return;
	}
	else if (xGraph == 5)
	{
		return;
	}
	
	for(int i = 1; i < nXDataSize+1; i++)
	{
		MoveToEx(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.bottom, NULL);
		LineTo(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.bottom+5);
		
		switch (xGraph)
		{
			case 0 :
				double dblXPi;
				dblXPi = 2.0 * (double)((nXDataTotal - nXDataSize) + i) / (double)n2PiCycle;
				szText.Format("%.1f", dblXPi);
				if ((((nXDataTotal - nXDataSize) + i) % (n2PiCycle / 4)) == 0)
				{
					LineTo(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.top);
					TextOut(hDC, (int)(rcData.left + dblXIntv*i), (int)rcData.bottom + 5, szText, szText.GetLength());	
					GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
				}
				break;
			case 1 :
				for (int i = 0; i < GRID_N_1 + 1; i++)
				{
					MoveToEx(hDC, rcData.left + (int)(dblYIntv*i), (int)(rcData.top), NULL);
					LineTo(hDC, rcData.left + (int)(dblYIntv*i), (int)(rcData.bottom));

					int xAxis = nXDataSize * i / GRID_N_1;
					szText.Format("%d", xAxis);
					GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
					TextOut(hDC, rcData.left + (int)(dblYIntv*i), (int)rcData.bottom + 5, szText, szText.GetLength());
				}
				break;
			case 2 :
				for (int i = 0; i < GRID_N_1 + 1; i++)
				{
					MoveToEx(hDC, rcData.left + (int)(dblYIntv*i), (int)(rcData.top), NULL);
					LineTo(hDC, rcData.left + (int)(dblYIntv*i), (int)(rcData.bottom));

					double xAxis = (double)nXDataSize *0.01 * (double)i / GRID_N_1;
					szText.Format("%.2f", xAxis);
					GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
					TextOut(hDC, rcData.left + (int)(dblYIntv*i), (int)rcData.bottom + 5, szText, szText.GetLength());
				}
				break;
			case 3:
			case 4:
				////for (int i = 1; i < (dblZMax * 2) + 1; i++)
				////{
				////	MoveToEx(hDC, rcData.left + (dblYIntv*i), (int)(rcData.top), NULL);
				////	LineTo(hDC, rcData.left + (dblYIntv*i), (int)(rcData.bottom));
				////}
				break;
		}
	}

	//SIZE sizeText;
	//GetTextExtentPoint32(hDC, m_ChartConfig.szAxisXNM, m_ChartConfig.szAxisXNM.GetLength(), &sizeText);	
	//TextOut(hDC, rcWindow.right - sizeText.cx - 5, rcWindow.bottom - sizeText.cy - 5, m_ChartConfig.szAxisXNM, m_ChartConfig.szAxisXNM.GetLength());
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hAxisLine);
}

void MLineChartCtrl::DrawAxisY(HDC hDC)
{
	SetTextColor(hDC, m_ChartConfig.crText);
	SetBkColor(hDC, m_ChartConfig.crTextBG);
	HPEN hAxisLine = CreatePen(PS_SOLID, 1, m_ChartConfig.crGuideLine);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hAxisLine);
	CString szText;
	SIZE size;

	switch (xGraph)
	{
		case 0:
			for (int i = 0; i < GRID_N_0 + 1; i++)
			{
				MoveToEx(hDC, rcData.left - 5, (int)(rcData.bottom - dblYIntv*i), NULL);
				LineTo(hDC, rcData.right, (int)(rcData.bottom - dblYIntv*i));

				double GridUnit = (dblYMax - dblYMin) / GRID_N_0;
				szText.Format("%.2f", dblYMin + GridUnit*i);
				GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
				TextOut(hDC, rcData.left - m_ChartConfig.nMarginLeft + 5, (int)(rcData.bottom - dblYIntv*i) - 6, szText, szText.GetLength());
			}
			break;
		case 1:
		case 2:
			for (int i = 0; i < GRID_N_1 + 1; i++)
			{
				MoveToEx(hDC, rcData.left - 5, (int)(rcData.bottom - dblYIntv*i), NULL);
				LineTo(hDC, rcData.right, (int)(rcData.bottom - dblYIntv*i));

				double GridUnit = (dblYMax - dblYMin) / GRID_N_1;
				szText.Format("%.4f", dblYMin + GridUnit*i);
				GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
				TextOut(hDC, rcData.left - m_ChartConfig.nMarginLeft + 5, (int)(rcData.bottom - dblYIntv*i) - 6, szText, szText.GetLength());
			}
			break;
		case 3:
		case 4:
			for (int i = 0; i < (dblZMax * 2) + 1; i++)
			{
				MoveToEx(hDC, rcData.left, (int)(rcData.bottom - dblYIntv*i), NULL);
				LineTo(hDC, rcData.right, (int)(rcData.bottom - dblYIntv*i));

				szText.Format("%d", (int)(-dblZMax) + i);
				TextOut(hDC, (int)rcData.CenterPoint().x + 3, (int)(rcData.bottom - dblYIntv*i) + 3, szText, szText.GetLength());
				GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
			}
			break;
		case 5:
			if (xGraph == 5)
			{
				for (int k=0; k<2; k++)
				{
					for (int i=0; i<6; i++)
					{
						/* External Rectangle */
						int rx = (int)(rcData.left+i*(dblXRoom+dblXIntv)+3);
						int ry = (int)(rcData.top+k*(dblYIntv+dblYRoom)+3);
						int inc =  (int)((dblYIntv-6.0)/4.0);			
						for (int j = 0; j < 5; j++)
						{
							MoveToEx(hDC, rx-5, ry+(j*inc), NULL);
							LineTo(hDC, rx-10, ry+(j*inc));
							CString szText;
							szText.Format("%d", (4-j)*25);
							TextOut(hDC, rx-10-20, ry+(j*inc)-5, szText, szText.GetLength());	
						}
					}
				}
			}
			break;
	}

	//// MIN, CENTER, MAX
	//MoveToEx(hDC, rcData.left - 4, rcData.top, NULL);
	//LineTo(hDC, rcData.right, rcData.top);
	//szText.Format(m_ChartConfig.szFormat, dblYMax);
	//GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	//TextOut(hDC, rcData.left - 6 - size.cx, rcData.top - size.cy / 2, szText, szText.GetLength());	// y축에서 6pixel 떨어져서 숫자표시
	//
	//MoveToEx(hDC, rcData.left - 4, rcData.CenterPoint().y, NULL);
	//LineTo(hDC, rcData.right, rcData.CenterPoint().y);
	//szText.Format(m_ChartConfig.szFormat, dblYCenter);
	//GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	//TextOut(hDC, rcData.left - 6 - size.cx, rcData.CenterPoint().y - size.cy / 2, szText, szText.GetLength());
	//
	//MoveToEx(hDC, rcData.left - 4, rcData.bottom, NULL);
	//LineTo(hDC, rcData.right, rcData.bottom);
	//szText.Format(m_ChartConfig.szFormat, dblYMin);
	//GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	//TextOut(hDC, rcData.left - 6 - size.cx, rcData.bottom - size.cy / 2, szText, szText.GetLength());
	//
	//if (rcData.Height() > 200)	// MIN-CENTER중간, CENTER-MAX중간
	//{
	//	MoveToEx(hDC, rcData.left - 4, (rcData.CenterPoint().y + rcData.top) / 2, NULL);
	//	LineTo(hDC, rcData.right, (rcData.CenterPoint().y + rcData.top) / 2);
	//
	//	szText.Format(m_ChartConfig.szFormat, (dblYMax + dblYCenter) / 2);
	//	GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	//	TextOut(hDC, rcData.left - 6 - size.cx, (rcData.top + rcData.CenterPoint().y) / 2 - size.cy / 2, szText, szText.GetLength());
	//
	//	MoveToEx(hDC, rcData.left - 4, (rcData.CenterPoint().y + rcData.bottom) / 2, NULL);
	//	LineTo(hDC, rcData.right, (rcData.CenterPoint().y + rcData.bottom) / 2);
	//
	//	szText.Format(m_ChartConfig.szFormat, (dblYMin + dblYCenter) / 2);
	//	GetTextExtentPoint32(hDC, szText, szText.GetLength(), &size);
	//	TextOut(hDC, rcData.left - 6 - size.cx, (rcData.bottom + rcData.CenterPoint().y) / 2 - size.cy / 2, szText, szText.GetLength());
	//}
	//TextOut(hDC, 5, 5, m_ChartConfig.szAxisYNM, m_ChartConfig.szAxisYNM.GetLength());
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hAxisLine);
}
void MLineChartCtrl::DrawData(HDC hDC)
{
	// DRAW DATA
	int x, y;
	int cx;
	int tx[10], ty[10];
	CString szText;
	HPEN hLinePen;
	HPEN hOldPen;
	HBRUSH hBrush;
	HBRUSH hOldBrush;

	static int cnt = 0;

	CserialDlg* pMainDlg = (CserialDlg*)::AfxGetMainWnd();

	if (xGraph == T_COIN)
	{
		hLinePen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
		hOldPen = (HPEN)SelectObject(hDC, hLinePen);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);	

		for (int i = 0; i < nXDataSize; i++)
		{
			double dataY = m_aData[m_iGrphX * m_iItemN + 0].lstData.GetAt(i)->dOrigin;
			int level_y = (int)(dblYIntv * (1 - dataY));
	
			int x1 = (int)(rcData.left + (i%6)*(dblXRoom + dblXIntv) + 3);		
			int x2 = (int)(rcData.left + (i%6)*(dblXRoom + dblXIntv) + dblXIntv - 3);
			int y1 = (int)(rcData.top + (i/6)*(dblYIntv + dblYRoom) + 3);
			int y2 = (int)(rcData.top + dblYIntv + (i/6)*(dblYIntv + dblYRoom) - 3);
	
			if (dataY >= 1)
			{
				Rectangle(hDC, x1, y1, x2, y2);
			}
			else if (dataY >= 0.01)
			{
				Rectangle(hDC, x1, y1 + level_y, x2, y2);
			}
			szText.Format("%.6f %%", dataY*100.0);
			TextOut(hDC, x1, y2+10, szText, szText.GetLength());

			HFONT hFont, hOldFont;
			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			strcpy_s(lf.lfFaceName, "Georgia");
			lf.lfHeight = 20;
			hFont = CreateFontIndirect(&lf);
			hOldFont = (HFONT)SelectObject(hDC, hFont);
			szText.Format("try-n: %d", pMainDlg->nDiceTry);
			TextOut(hDC, rcWindow.CenterPoint().x-50, rcWindow.top + 15, szText, szText.GetLength());
			SelectObject(hDC, hOldFont);
			DeleteObject(hFont);
		}
	
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hLinePen);
		DeleteObject(hBrush);

		return;
	}	
	 
	for (int item_i = 0; item_i < m_iItemN; item_i++)
	{
		hLinePen = CreatePen(PS_SOLID, 1, m_ChartConfig.crDataLine[item_i]);
		hOldPen = (HPEN)SelectObject(hDC, hLinePen);

		for (int i = 0; i < nXDataSize; i++)
		{
			double dataY1 = m_aData[m_iGrphX * m_iItemN + item_i].lstData.GetAt(i)->dOrigin;
			double dataY2 = m_aData[m_iGrphX * m_iItemN + item_i].lstData.GetAt(i)->dDest;
			bool bSP = m_aData[m_iGrphX * m_iItemN + item_i].lstData.GetAt(i)->bPrint;
			switch (xGraph)
			{
				case 0:
				case 1:
				case 2:
					x = (int)(rcData.left + dblXIntv*(i + 1));
					y = rcData.CenterPoint().y + (int)((dblYCenter - dataY1)*dblYPixelSize);
					break;
				case 3:
				case 4:
					if (i == 0) 
					{
						x = rcData.CenterPoint().x;
						y = rcData.CenterPoint().y;
					} 
					else 
					{
						x = rcData.CenterPoint().x + (int)(dataY1 * dblXIntv);
						y = rcData.CenterPoint().y - (int)(dataY2 * dblYIntv);
					}
					break;
			}

			if (bSP == TRUE)
			{				
			}

			if (i == 0)		// 첫 데이타
			{
				MoveToEx(hDC, x, y, NULL);
				if ((xGraph == T_COMPLEX))	//|| (xGraph == T_EULER))
				{
					x = rcData.CenterPoint().x + (int)(dataY1 * dblXIntv);
					y = rcData.CenterPoint().y - (int)(dataY2 * dblYIntv);
					LineTo(hDC, x, y);
				}
				//else if (xGraph == T_COIN)
				//{
				//	int k = 0;
				//	for (int item_i = 0; item_i < m_iItemN; item_i++)
				//	{
				//		double dataY = m_aData[m_iGrphX * m_iItemN + item_i].lstData.GetAt(0)->dOrigin;
				//		int level_y = (int)(dblYIntv * (1 - dataY));
				//
				//		int x1 = rcData.left + item_i*(dblXRoom + dblXIntv) + 3;
				//		int y1 = rcData.top + k*(dblYIntv + dblYRoom) + 3;
				//		int x2 = rcData.left + item_i*(dblXRoom + dblXIntv) + dblXIntv - 3;
				//		int y2 = rcData.top + dblYIntv + k*(dblYIntv + dblYRoom) - 3;
				//
				//		if (dataY != 0)
				//		{
				//			Rectangle(hDC, x1, y1 + level_y, x2, y2);
				//		}
				//		szText.Format("%.6f %%", dataY*100.0);
				//		TextOut(hDC, x1, y2 + 10, szText, szText.GetLength());
				//	}
				//}
			}
			else			// 나머지 데이타  
			{
				if (xGraph == T_COMPLEX) 
				{
					SetPixelV(hDC, x, y, m_ChartConfig.crDataLine[item_i]);
				}
				else if (xGraph == T_EULER)
				{
					LineTo(hDC, x, y);
					LineTo(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y);
					LineTo(hDC, x, y);
					//if (i < 7)
					//{
					//	szText.Format("(%.2f/%.2f)", dataY1, dataY2);
					//	TextOut(hDC, x - 10, y - 20, szText, szText.GetLength());
					//}
					// CREATE FONT
					HFONT hFont, hOldFont;
					LOGFONT lf;
					memset(&lf, 0, sizeof(lf));
					strcpy_s(lf.lfFaceName, "Georgia");
					lf.lfHeight = 20;
					hFont = CreateFontIndirect(&lf);				
					hOldFont = (HFONT)SelectObject(hDC, hFont);
					
					szText.Format("e    = (1 + %.1fπi/%d)", pMainDlg->dblEuler_x, pMainDlg->szEuler_m);
					TextOut(hDC, rcWindow.right-210, rcWindow.top+25, szText, szText.GetLength());					
					szText.Format("%d", pMainDlg->nEuler_m);
					TextOut(hDC, rcWindow.right-60, rcWindow.top+5, szText, szText.GetLength());
					szText.Format("%.1fπi", pMainDlg->dblEuler_x);
					TextOut(hDC, rcWindow.right-195, rcWindow.top+5, szText, szText.GetLength());
					SelectObject(hDC, hOldFont);
					DeleteObject(hFont);
				}
				else 
				{
					LineTo(hDC, x, y);
					switch (xGraph)
					{
						case T_SINX:
							MoveToEx(hDC, rcData.CenterPoint().y, rcData.CenterPoint().y, NULL);
							cx = rcData.CenterPoint().y + (int)((dataY2)*dblYPixelSize);
							if (i == nXDataSize - 1)
							{
								LineTo(hDC, cx, y);
								LineTo(hDC, x, y);
								DrawDotRect(hDC, cx, y, 3, 3);
							}
							else
							{
								DrawDotRect(hDC, cx, y, 1, 1);
							}
							MoveToEx(hDC, x, y, NULL);
							break;
						case T_E:
							if ((i < 7) && (nXDataSize > 8))
							{
								DrawDotRect(hDC, x, y, 3, 3);
								CString szText;
								szText.Format("%.4f", dataY1);
								TextOut(hDC, x + 10, y - 10, szText, szText.GetLength());
							}
							break;
					}
				}
			}

			if (i == nXDataSize - 1)	// 마지막 데이타 
			{
				DrawDotRect(hDC, x, y, 3, 3);


				////LineTo(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y);// , 3, 3);
				////EndPath(hDC);
				////StrokeAndFillPath(hDC);
				//////SelectObject(hDC, hOldBrush);


				double lastData = dataY1;
				switch(xGraph)
				{
					case T_SINX:
					case T_E:
						//DrawDotRect(hDC, x, y, 3, 3);
						szText.Format("%.4f", lastData);
						TextOut(hDC, x - 20, y - 20, szText, szText.GetLength());
						break;
					case T_EXPX:
						//DrawDotRect(hDC, x, y, 3, 3);
						szText.Format("%.4f", lastData);
						TextOut(hDC, x - 20, y - 20, szText, szText.GetLength());
						szText.Format("%.4f", dataY2);
						TextOut(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y, szText, szText.GetLength());
						break;
					case T_COMPLEX:
						//DrawDotRect(hDC, x, y, 3, 3);
						tx[item_i] = x;
						ty[item_i] = y;
						if (item_i == m_iItemN - 1)
						{
							switch (iZMulStep)
							{
								case 1:
								case 5:
									break;
								case 2:
								case 3:
								case 4:
								case 6:
								case 7:
								case 8:
									/* 삼각형 */
									//HBRUSH hBrush = CreateSolidBrush(RGB(0x00, 0xff, 0xff));
									//HBRUSH hOldBrush;
									//hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
									//POINT arr[] = { rcData.CenterPoint().x, rcData.CenterPoint().y, tx[4], ty[4], tx[5], ty[5] };
									//Polygon(hDC, arr, 3);
									//SelectObject(hDC, hOldBrush);
									/* 삼각형 */
									BeginPath(hDC);
									MoveToEx(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y, NULL);
									LineTo(hDC, tx[4], ty[4]);
									LineTo(hDC, tx[5], ty[5]);
									LineTo(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y);
									EndPath(hDC);
									StrokeAndFillPath(hDC);
									break;
							}
						}
						MoveToEx(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y, NULL);
						LineTo(hDC, x, y);
						szText.Format("(%.1f/%.1f)", dataY1, dataY2);
						TextOut(hDC, x + 10, y - 10, szText, szText.GetLength());
						break;
					case T_EULER:
						//MoveToEx(hDC, rcData.CenterPoint().x, rcData.CenterPoint().y, NULL);
						//LineTo(hDC, x, y);
						szText.Format("(%.4f/%.4f)", dataY1, dataY2);
						TextOut(hDC, x + 10, y - 20, szText, szText.GetLength());
						break;
				}
			}
		}
		
		SelectObject(hDC, hOldPen);
		DeleteObject(hLinePen);
	}
}

void MLineChartCtrl::DrawMouseData(HDC hDC)
{
	if ((m_Mouse.x >= rcData.left + 5) && (m_Mouse.x <= rcData.right - 5) && \
		(m_Mouse.y >= rcData.top + 5) && (m_Mouse.y <= rcData.bottom - 5))		// 표시영역 설정
	{
		if (m_iBKColor_i == 0)
		{
			SetTextColor(hDC, RGB(255, 255, 255));		// dc.SetTextColor(RGB(0, 0, 0)); 와 비교
			SetBkColor(hDC, RGB(30, 30, 30));			// dc.SetBkColor(RGB(255, 255, 255)); 와 비교
		}
		else if (m_iBKColor_i == 1)
		{
			SetTextColor(hDC, RGB(0, 0, 0));
			SetBkColor(hDC, RGB(255, 255, 255));
		}
		else if (m_iBKColor_i == 2)
		{
			SetTextColor(hDC, RGB(255, 255, 255));
			SetBkColor(hDC, RGB(140, 170, 230));
		}

		int height = rcData.bottom - rcData.top;
		int pos_y = rcData.bottom - m_Mouse.y;
		int width = rcData.right - rcData.left;
		int pos_x = m_Mouse.x - rcData.left;
		double MaxMin = dblYMax - dblYMin;
		double dYData = dblYMin + ((pos_y*MaxMin) / height);
		double dXData;
		CString str;

		
		if (xGraph == 0)
		{
			dXData = 2.0 * (double)(nXDataSize) / (double)n2PiCycle;
			dXData = dXData * pos_x / width;
			double offset;
			offset = 2.0 * (double)((nXDataTotal - nXDataSize) + 0) / (double)n2PiCycle;
			dXData += offset;
			str.Format("%.2f/%.2f", dXData, dYData);	// 좌표출력
		}
		else if (xGraph == 1)
		{
			dXData = nXDataSize * pos_x / width;
			str.Format("%d/%.4f", (int)dXData, dYData);	// 좌표출력
		}
		else if (xGraph == 2)
		{
			dXData = nXDataSize * 0.01 * pos_x / width;
			str.Format("%.2f/%.4f", dXData, dYData);	// 좌표출력
		}
		else if (xGraph == 3)
		{
			dXData = (dblZMax * 2) * (double)pos_x / (double)width - dblZMax;
			dYData = (dblZMax * 2) * (double)pos_y / (double)width - dblZMax;
			str.Format("%.2f/%.4f", dXData, dYData);	// 좌표출력
		}
		else if (xGraph == 4)
		{
			dXData = (dblZMax * 2) * (double)pos_x / (double)width - dblZMax;
			dYData = (dblZMax * 2) * (double)pos_y / (double)width - dblZMax;
			str.Format("%.2f/%.4f", dXData, dYData);	// 좌표출력
		}

		//if (m_bLClkDown == TRUE)
		//{
		//	m_dBaseLev[m_iGrphX] = d_value;
		//	m_bLClkDown = FALSE;
		//}
		//dYData -= m_dBaseLev[m_iGrphX];	

		TextOut(hDC, m_Mouse.x + 10, m_Mouse.y - 20, str, str.GetLength());	// dc.TextOut(m_Mouse.x, m_Mouse.y - 20, str); 와 비교


		HPEN hDotPen;
		if (m_iBKColor_i == 0)		{ hDotPen = CreatePen(PS_DOT, 1, RGB(255, 255, 0)); }
		else if (m_iBKColor_i == 1)	{ hDotPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0)); }
		else if (m_iBKColor_i == 2)	{ hDotPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255)); }
		else						{ hDotPen = CreatePen(PS_DOT, 1, RGB(255, 255, 255)); }
		HPEN hOldPen = (HPEN)SelectObject(hDC, hDotPen);
		MoveToEx(hDC, 50, m_Mouse.y, NULL);
		LineTo(hDC, rcData.right, m_Mouse.y);
		MoveToEx(hDC, m_Mouse.x, rcData.top, NULL);
		LineTo(hDC, m_Mouse.x, rcData.bottom);

		SelectObject(hDC, hOldPen);
		DeleteObject(hDotPen);
	}
}

void MLineChartCtrl::DrawDotRect(HDC hDC, int x, int y, int nW, int nH)
{
	Rectangle(hDC, x-nW, y-nH, x+nW, y+nH);
}

void MLineChartCtrl::OnSize(UINT nType, int cx, int cy)
{
	CreateMemResource();

	CWnd::OnSize(nType, cx, cy);
}

void MLineChartCtrl::CreateMemResource()
{
	CRect r;
	CDC * pDC = GetDC();

	GetClientRect(r);

	if(m_hMemDC == NULL)
		m_hMemDC = CreateCompatibleDC(pDC->m_hDC);

	if(m_hMemBitmap != NULL)
		DeleteObject(m_hMemBitmap);

	m_hMemBitmap = CreateCompatibleBitmap(pDC->m_hDC, r.Width(), r.Height());
	SelectObject(m_hMemDC, m_hMemBitmap);

	ReleaseDC(pDC);
}



void MLineChartCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_Mouse = point;

	if (m_bLBtnDbClk == TRUE)
	{
		Invalidate();	// OnPaint 호출
	}

	CWnd::OnMouseMove(nFlags, point);
}


void MLineChartCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (int i = 0; i < m_iGrphN; i++)
	{
		for (int j = 0; j < m_iItemN; j++)
		{
			m_aData[i * m_iItemN + j].Clear();
		}
	}
	nXDataTotal = 0;
	m_iXth = 0;
	RedrawWindow();
	CWnd::OnRButtonDblClk(nFlags, point);

}


void MLineChartCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_bLBtnDbClk == TRUE)
	{
		m_bLBtnDbClk = FALSE;
	}
	else
	{
		m_bLBtnDbClk = TRUE;
	}

	::PostMessage(hCommWnd, GRPH_PLAY_PAUSE, 0, 0);

	CWnd::OnLButtonDblClk(nFlags, point);
}


void MLineChartCtrl::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bMBtnDbClk == TRUE)
	{
		m_bMBtnDbClk = FALSE;
	}
	else
	{
		m_bMBtnDbClk = TRUE;
	}
//	for (int i = 0; i < m_iGrphN; i++)
//	{
//		for (int j = 0; j < m_iItemN; j++)
//		{
//			m_aData[i * m_iItem_i + j].Clear();
//		}
//	}
//	m_dYMax = 0;
//	m_dYMin = 0;

	CWnd::OnMButtonDblClk(nFlags, point);
}


BOOL MLineChartCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}


void MLineChartCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLClkDown = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}

void MLineChartCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//::PostMessage(hCommWnd, GRPH_CLEAR, 0, 0);
	m_nViewPeriod = 0;

	CWnd::OnRButtonDown(nFlags, point);
}


