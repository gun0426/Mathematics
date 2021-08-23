#pragma once

//#define DAT_SIM_FINISH	(WM_USER + 4)	// config.h
//#define GRPH_DLG_DESTROY	(WM_USER + 5)

#include "MLineChartCtrl.h"
#include "config.h"
// CGraph ��ȭ �����Դϴ�.

class CGraph : public CDialogEx
{
	DECLARE_DYNAMIC(CGraph)

public:
	CGraph(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGraph();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GRAPH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	MLineChartCtrl m_LineChartCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	bool m_bWindow;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int m_iViewSize;
	afx_msg void OnDestroy();
};
