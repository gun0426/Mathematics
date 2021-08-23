// Graph.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "serial.h"
#include "Graph.h"
#include "afxdialogex.h"


extern HWND hCommWnd;

// CGraph ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGraph, CDialogEx)

CGraph::CGraph(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraph::IDD, pParent)
	, m_bWindow(false)
	, m_iViewSize(0)
{

}

CGraph::~CGraph()
{
}

void CGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraph, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CGraph �޽��� ó�����Դϴ�.


BOOL CGraph::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			UpdateData(TRUE);
			return TRUE;
		case VK_F1:
			return TRUE;
		case VK_F4:
			// ��ũ�� ������ ���ϱ� 
			SIZE s;
			ZeroMemory(&s, sizeof(SIZE));	
			//s.cx = (LONG)::GetSystemMetrics(SM_CXFULLSCREEN);	// �ʺ�
			//s.cy = (LONG)::GetSystemMetrics(SM_CYFULLSCREEN);	// ���� 
			//s.cx = (LONG)::GetSystemMetrics(SM_CXMAXIMIZED);	// �ʺ�
			//s.cy = (LONG)::GetSystemMetrics(SM_CYMAXIMIZED);	// ���� 
			s.cx = (LONG)::GetSystemMetrics(SM_CXSCREEN);	// �ʺ�
			s.cy = (LONG)::GetSystemMetrics(SM_CYSCREEN);	// ���� 

			m_iViewSize++;
			m_iViewSize %= 5;
			switch (m_iViewSize)
			{
			case 0:
				MoveWindow(0, 0, (int)(s.cx*0.25), (int)(s.cy*0.50));
				break;
			case 1:
				MoveWindow(0, 0, (int)(s.cx*0.25), s.cy);
				break;
			case 2:
				MoveWindow(0, 0, (int)(s.cx*0.50), s.cy);
				break;
			case 3:
				MoveWindow(0, 0, (int)(s.cx*0.75), s.cy);
				break;
			case 4:
				MoveWindow(0, 0, (int)(s.cx*1.00), s.cy);
				break;
			}
			break;
		case VK_F2:
			m_LineChartCtrl.m_bF2Evnt = TRUE;
			m_LineChartCtrl.m_nViewSplit++;
			m_LineChartCtrl.m_nViewSplit %= (SPLIT_N+1);
			return TRUE;
		case VK_F3:
			m_LineChartCtrl.m_iBKColor_i++;
			m_LineChartCtrl.m_iBKColor_i %= 3;
			if (m_LineChartCtrl.m_iBKColor_i == 1)
			{
				//m_LineChartCtrl.m_ChartConfig.crDot = RGB(255, 0, 0);
				m_LineChartCtrl.m_ChartConfig.crBG = RGB(255, 255, 255);	// White
				m_LineChartCtrl.m_ChartConfig.crGuideLine = RGB(128, 128, 128);
				m_LineChartCtrl.m_ChartConfig.crAxis = RGB(0, 0, 0);
				m_LineChartCtrl.m_ChartConfig.crText = RGB(0, 0, 0);
				m_LineChartCtrl.m_ChartConfig.crTextBG = RGB(255, 255, 255);
				//m_LineChartCtrl.m_ChartConfig.crDataLine[0] = RGB(0x00, 0x00, 0x00);
				//m_LineChartCtrl.m_ChartConfig.crDataLine[1] = RGB(0x80, 0x80, 0x80);
			}
			else if (m_LineChartCtrl.m_iBKColor_i == 2)
			{
				//m_LineChartCtrl.m_ChartConfig.crDot = RGB(255, 0, 0);
				m_LineChartCtrl.m_ChartConfig.crBG = RGB(140, 170, 230);	// White
				m_LineChartCtrl.m_ChartConfig.crGuideLine = RGB(230, 230, 230); //RGB(128, 128, 128);
				m_LineChartCtrl.m_ChartConfig.crAxis = RGB(255, 255, 255);
				m_LineChartCtrl.m_ChartConfig.crText = RGB(255, 255, 255);
				m_LineChartCtrl.m_ChartConfig.crTextBG = RGB(140, 170, 230);
				//m_LineChartCtrl.m_ChartConfig.crDataLine[0] = RGB(0x00, 0x00, 0x00);
				//m_LineChartCtrl.m_ChartConfig.crDataLine[1] = RGB(0x80, 0x80, 0x80);
			}
			else if (m_LineChartCtrl.m_iBKColor_i == 0)
			{
				//m_LineChartCtrl.m_ChartConfig.crDot = RGB(255, 0, 0);
				m_LineChartCtrl.m_ChartConfig.crBG = RGB(30, 30, 30);			// Black
				m_LineChartCtrl.m_ChartConfig.crGuideLine = RGB(128, 128, 128);
				m_LineChartCtrl.m_ChartConfig.crAxis = RGB(255, 255, 255);
				m_LineChartCtrl.m_ChartConfig.crText = RGB(255, 255, 255);
				m_LineChartCtrl.m_ChartConfig.crTextBG = RGB(30, 30, 30);
				//m_LineChartCtrl.m_ChartConfig.crDataLine[0] = RGB(0xff, 0xff, 0xff);
				//m_LineChartCtrl.m_ChartConfig.crDataLine[1] = RGB(0x80, 0x80, 0x80);	
			}
			return TRUE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CGraph::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CRect rcWindow;
	GetClientRect(rcWindow);
	if (!m_LineChartCtrl.m_hWnd)
	{
		m_LineChartCtrl.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, rcWindow.Width(), rcWindow.Height()), this, 1234);
		m_bWindow = TRUE;
		for (int i = 0; i < 10; i++)
		{
			m_LineChartCtrl.m_dBaseLev[i] = 0;
		}
		SetWindowPos(NULL, 0, 0, 1600, 500, SWP_NOREPOSITION);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CGraph::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_LineChartCtrl.m_hWnd)
	{
		m_LineChartCtrl.MoveWindow(0, 0, cx, cy, TRUE);	// --> m_LineChartCtrl.OnSize()
	}
}

void CGraph::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nID == SC_CLOSE)
	{
		m_bWindow = FALSE;
		DestroyWindow();

		::PostMessage(hCommWnd, DAT_SIM_FINISH, 0, 0);
	}
	else if (nID == SC_MAXIMIZE)
	{
	}
	else if (nID == SC_MINIMIZE)
	{
	}
	else if (nID == SC_RESTORE)
	{
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL CGraph::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return TRUE;
//	return CDialogEx::OnEraseBkgnd(pDC);
}


void CGraph::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	::PostMessage(hCommWnd, GRPH_DLG_DESTROY, 0, 0);
}

