
// serialDlg.h : 헤더 파일
//

/* Window Message config.h */
//#define RX_EVENT			(WM_USER + 1)
//#define DAT_SIMUL			(WM_USER + 2)
//#define DAT_SIM_STOP		(WM_USER + 3)
//#define DAT_SIM_FINISH	(WM_USER + 4)
//#define GRPH_DLG_DESTROY	(WM_USER + 5)

/* Timer */
#define T_TIMER			1
#define T_ZROTATE		2
#define T_ZMULTIPLY		3
#define T_EULER_1		4
#define T_EULER_2		5

#pragma once
#include "afxwin.h"
#include "serial_thread.h"
#include "Utils.h"
#include "afxcmn.h"
#include "Graph.h"
#include "afx.h"
#include "config.h"

#define RX_BUFF_SIZE 500
#define RX_DATA_SIZE 300
#define RX_STX	0x02
#define RX_CR	0x0D
#define RX_LF	0x0A

#define SK_STX	0x3A


typedef struct
{
	int indx_w;
	int indx_r;
	BYTE buff[RX_BUFF_SIZE];
	BYTE len;
	BYTE bcc;
	WORD cs;
	BYTE indx_d;
	BYTE data[RX_DATA_SIZE];
} str_rx;

enum ThreadWorkingType
{
	THREAD_STOP = 0,
	THREAD_RUNNING = 1,
	THREAD_PAUSE = 2
};

// CserialDlg 대화 상자
class CserialDlg : public CDialogEx
{
// 생성입니다.
public:
	CserialDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CSerialThread m_CSerial;
	CUtils m_CUtils;
	afx_msg void OnBnClickedBtnSeropen();
	int m_iSPRm_Data;
	int m_iSPRm_Stop;
	int m_iSPRm_Parity;
	bool m_bSerOpen;
	BYTE byIndexData(int xData);
	int m_iBaudIndx;
	int m_iPortIndx;
	afx_msg void OnBnClickedBtnSend();
	CString m_strSend;
protected:
	afx_msg LRESULT OnRxEvent(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_EditRx;
	CRichEditCtrl m_REditRx;
	HWND msgHwnd_0;

	CHARFORMAT chFormat;
	CString m_strRxPack;
	str_rx m_stRx;

	int OnRxFrameCheck();
//	int m_aMsg[20];
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnGraph();

	CGraph m_CGraph;
	afx_msg void OnCbnSelchangeCombo1();
	int m_iItem_i;
	int m_iGrph_i;
	afx_msg void OnCbnSelchangeComboPort();
//	int m_iYMargin;
	afx_msg void OnBnClickedBtnFileopen();
	CString m_STRdatPath;
	CStdioFile m_SFsim;

//	static UINT Thread_SendFile(LPVOID pParam);
	static UINT thread_DATsimul(LPVOID pParam);

	bool m_bTHRtxStart;
	CWinThread * m_pTHRsim;
	ThreadWorkingType m_eTHRtxWork;
//	afx_msg void OnBnClickedBtnTxstart();
//	afx_msg void OnBnClickedBtnTxpause();
//	afx_msg void OnBnClickedBtnTxstop();
//protected:
//	afx_msg LRESULT OnTxThreadStop(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnDatSimul(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnDatSimStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDatSimFinish(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnGpause();
	afx_msg void OnBnClickedBtnGclose();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL m_bItemChk[8];
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	int nGrphDMax;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL bAlOnTop;
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnGrphDlgDestroy(WPARAM wParam, LPARAM lParam);
public:
	int n2PiCycle;
	int tick_ms;
protected:
	afx_msg LRESULT OnGrphPlayPause(WPARAM wParam, LPARAM lParam);
public:
	double dblFxA[2];
	double dblFxB[2];
	double dblFxC[2];
	double dblFxD[2];
	int cbxSinX[2];
	int cbxOper;
	double dblCPerc[2];
	static UINT Thread_ProgressBar(LPVOID pParam);
	CProgressCtrl progBar[6];
//	int iProgBar[6];
	int nHeadTail[2];
	int nDiceIndx[6];
	double dblDPerc[6];
	int nDiceTry;
	double dblNatualE;
	int nNaturalE;
	int xGraph;
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedBtnGrphDataClr();
	afx_msg void OnCbnSelchangeComboSincos();
	afx_msg void OnCbnSelchangeComboSincos2();
	afx_msg void OnCbnSelchangeComboOperator();
	afx_msg void OnEnChangeFormA();
	afx_msg void OnEnChangeFormA2();
	afx_msg void OnEnChangeEditFormB();
	afx_msg void OnEnChangeEditFormC();
	afx_msg void OnEnChangeEditFormD();
	afx_msg void OnEnChangeEditFormB2();
	afx_msg void OnEnChangeEditFormC2();
	afx_msg void OnEnChangeEditFormD2();
	int nExpParam_m;
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	double dblExpX;
	double dblExpX2;
	//double dblExpX_x;
	afx_msg void OnEnChangeEdit10();
	double dblExpX_x;	
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnChangeEdit13();
	afx_msg void OnEnChangeEdit14();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit16();
//	int cbxOper2;
	double dblZMax;
//	afx_msg void OnCbnSelchangeComboOperator2();
	afx_msg void OnEnChangeEdit5();
	void Renewal_Z();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEditMaxN();
	double dblZRe[6];
	double dblZIm[6];
	double dblZDeg[6];
	double dblZMag[6];
	double dblZRotStep[2];
	CString strZRe[6];
	CString strZIm[6];
	CString strZMag[6];
	CString strZDeg[6];
	CString strZRotStep[2];
	BYTE vZRot;
	int nEuler_m;
	afx_msg void OnEnChangeEdit22();
	afx_msg void OnEnChangeEdit25();
	double dblEuler_x;
protected:
	afx_msg LRESULT OnControlRenewal(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton4();
	int iZMulStep;
	double dblDegInc;
	double dblZExtenInc;
	double dblZExten;
	int taskSinX(void);
	int taskNatural_e(void);
	int taskExpX(void);
	int taskZMultiply(void);
	int taskZRotate(void);
	int taskCoin(void);
	afx_msg void OnEnChangeEditSampleN();
	afx_msg void OnEnChangeEdit2();
	int iZOper;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	//CStatic bmpRotate;
	double dblEulerZRe[2];
	double dblEulerZIm[2];
	CString strEulerZRe[2];
	CString strEulerZIm[2];
	int taskEulerForm();
	afx_msg void OnBnClickedButton9();
	int taskEulerForm2(void);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL bEulerMFix;
	afx_msg void OnBnClickedCheck10();
	int szEuler_m;
	afx_msg void OnEnChangeEdit31();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton10();
	bool bEulerBtn;
};
