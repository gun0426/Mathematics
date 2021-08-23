
// serialDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "serial.h"
#include "serialDlg.h"
#include "afxdialogex.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND hCommWnd;
DWORD dwBaud[] = { 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 76800, 115200, 230400, 250000, 500000 };
CString strPort[] = { "COM0", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15" };
int m_aMsg[100];


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CserialDlg 대화 상자



CserialDlg::CserialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CserialDlg::IDD, pParent)
	, m_iSPRm_Data(0)
	, m_iSPRm_Stop(0)
	, m_iSPRm_Parity(0)
	, m_bSerOpen(false)
	, m_iBaudIndx(0)
	, m_iPortIndx(0)
	, m_strSend(_T(""))
	, m_iItem_i(0)
	, m_iGrph_i(0)
	, m_STRdatPath(_T(""))
	, nGrphDMax(0)
	, bAlOnTop(FALSE)
	, n2PiCycle(0)
	, tick_ms(0)
	, cbxOper(0)
//	, dblCPerc(0)
//	, dblDPerc(0)
	, nDiceTry(0)
	, dblNatualE(0)
	, nNaturalE(0)
	, xGraph(0)
	, nExpParam_m(0)
	, dblExpX(0)
	, dblExpX2(0)
	, dblExpX_x(0)
	, dblZMax(0)
	, vZRot(0)
	, nEuler_m(0)
	, dblEuler_x(0)
	, iZMulStep(0)
	, dblDegInc(0)
	, dblZExtenInc(0)
	, dblZExten(0)
	, iZOper(0)
	, bEulerMFix(FALSE)
	, szEuler_m(0)
	, bEulerBtn(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_BAUD, m_iBaudIndx);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_iPortIndx);
	DDX_Text(pDX, IDC_EDIT_TX, m_strSend);
	//	DDX_Control(pDX, IDC_EDIT_RX, m_EditRx);
	DDX_Control(pDX, IDC_RICHEDIT_RX, m_REditRx);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iItem_i);
	DDX_CBIndex(pDX, IDC_COMBO2, m_iGrph_i);
	DDX_Check(pDX, IDC_CHECK1, m_bItemChk[0]);
	DDX_Check(pDX, IDC_CHECK2, m_bItemChk[1]);
	DDX_Check(pDX, IDC_CHECK3, m_bItemChk[2]);
	DDX_Check(pDX, IDC_CHECK4, m_bItemChk[3]);
	DDX_Check(pDX, IDC_CHECK5, m_bItemChk[4]);
	DDX_Check(pDX, IDC_CHECK6, m_bItemChk[5]);
	DDX_Check(pDX, IDC_CHECK7, m_bItemChk[6]);
	DDX_Check(pDX, IDC_CHECK8, m_bItemChk[7]);
	DDX_Text(pDX, IDC_EDIT_MAX_N, nGrphDMax);
	DDX_Check(pDX, IDC_CHECK9, bAlOnTop);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_N, n2PiCycle);
	DDX_Text(pDX, IDC_EDIT2, tick_ms);
	DDX_Text(pDX, IDC_FORM_A, dblFxA[0]);
	DDX_Text(pDX, IDC_EDIT_FORM_B, dblFxB[0]);
	DDX_Text(pDX, IDC_EDIT_FORM_C, dblFxC[0]);
	DDX_Text(pDX, IDC_EDIT_FORM_D, dblFxD[0]);
	DDX_CBIndex(pDX, IDC_COMBO_SINCOS, cbxSinX[0]);
	DDX_Text(pDX, IDC_FORM_A2, dblFxA[1]);
	DDX_Text(pDX, IDC_EDIT_FORM_B2, dblFxB[1]);
	DDX_Text(pDX, IDC_EDIT_FORM_C2, dblFxC[1]);
	DDX_Text(pDX, IDC_EDIT_FORM_D2, dblFxD[1]);
	DDX_CBIndex(pDX, IDC_COMBO_SINCOS2, cbxSinX[1]);
	DDX_CBIndex(pDX, IDC_COMBO_OPERATOR, cbxOper);
//	DDX_Text(pDX, IDC_EDIT3, dblCPerc);
//	DDX_Control(pDX, IDC_PROGRESS1, progBar[0]);
//	DDX_Control(pDX, IDC_PROGRESS2, progBar[1]);
//	DDX_Control(pDX, IDC_PROGRESS3, progBar[2]);
//	DDX_Control(pDX, IDC_PROGRESS4, progBar[3]);
//	DDX_Control(pDX, IDC_PROGRESS5, progBar[4]);
//	DDX_Control(pDX, IDC_PROGRESS6, progBar[5]);
//	DDX_Text(pDX, IDC_EDIT4, dblDPerc);
	DDX_Text(pDX, IDC_EDIT5, nDiceTry);
	DDX_Text(pDX, IDC_EDIT1, dblNatualE);
	DDX_Text(pDX, IDC_EDIT6, nNaturalE);
	DDX_CBIndex(pDX, IDC_COMBO3, xGraph);
	DDX_Text(pDX, IDC_EDIT7, nExpParam_m);
	DDX_Text(pDX, IDC_EDIT9, dblExpX);
	DDX_Text(pDX, IDC_EDIT8, dblExpX2);
	//	DDX_Text(pDX, IDC_EDIT10, dblExpX_x);
	DDX_Text(pDX, IDC_EDIT10, dblExpX_x);
	//	DDX_CBIndex(pDX, IDC_COMBO_OPERATOR2, cbxOper2);
	//DDX_Text(pDX, IDC_EDIT11, dblZRe[0]);
	//DDX_Text(pDX, IDC_EDIT13, dblZRe[1]);
	//DDX_Text(pDX, IDC_EDIT15, dblZRe[2]);
	//DDX_Text(pDX, IDC_EDIT12, dblZIm[0]);
	//DDX_Text(pDX, IDC_EDIT14, dblZIm[1]);
	//DDX_Text(pDX, IDC_EDIT16, dblZIm[2]);
	//DDX_Text(pDX, IDC_EDIT17, dblZDeg[0]);
	//DDX_Text(pDX, IDC_EDIT18, dblZDeg[1]);
	//DDX_Text(pDX, IDC_EDIT19, dblZDeg[2]);
	//DDX_Text(pDX, IDC_EDIT17, strAngle);
	DDX_Text(pDX, IDC_EDIT11, strZRe[0]);
	DDX_Text(pDX, IDC_EDIT13, strZRe[1]);
	DDX_Text(pDX, IDC_EDIT15, strZRe[2]);
	DDX_Text(pDX, IDC_EDIT12, strZIm[0]);
	DDX_Text(pDX, IDC_EDIT14, strZIm[1]);
	DDX_Text(pDX, IDC_EDIT16, strZIm[2]);
	DDX_Text(pDX, IDC_EDIT17, strZDeg[0]);
	DDX_Text(pDX, IDC_EDIT18, strZDeg[1]);
	DDX_Text(pDX, IDC_EDIT19, strZDeg[2]);
	DDX_Text(pDX, IDC_EDIT20, strZRotStep[0]);
	DDX_Text(pDX, IDC_EDIT21, strZRotStep[1]);
	DDX_Text(pDX, IDC_EDIT22, nEuler_m);
	//DDX_Text(pDX, IDC_EDIT23, dblZRe[3]);
	//DDX_Text(pDX, IDC_EDIT24, dblZIm[3]);
	//DDX_Text(pDX, IDC_EDIT25, dblEuler_x);
	DDX_Text(pDX, IDC_EDIT25, dblEuler_x);
	DDX_Text(pDX, IDC_EDIT26, strZMag[0]);
	DDX_Text(pDX, IDC_EDIT27, strZMag[1]);
	DDX_Text(pDX, IDC_EDIT28, strZMag[2]);
	//DDX_Control(pDX, IDC_ROTATE, bmpRotate);
	DDX_Text(pDX, IDC_EDIT23, strEulerZRe[1]);
	DDX_Text(pDX, IDC_EDIT24, strEulerZIm[1]);
	DDX_Text(pDX, IDC_EDIT29, strEulerZRe[0]);
	DDX_Text(pDX, IDC_EDIT30, strEulerZIm[0]);
	DDX_Check(pDX, IDC_CHECK10, bEulerMFix);
	DDX_Text(pDX, IDC_EDIT31, szEuler_m);
}

BEGIN_MESSAGE_MAP(CserialDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEROPEN, &CserialDlg::OnBnClickedBtnSeropen)
	ON_BN_CLICKED(IDC_BTN_SEND, &CserialDlg::OnBnClickedBtnSend)
	ON_MESSAGE(RX_EVENT, &CserialDlg::OnRxEvent)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GRAPH, &CserialDlg::OnBnClickedBtnGraph)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CserialDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CserialDlg::OnCbnSelchangeComboPort)
	ON_BN_CLICKED(IDC_BTN_FILEOPEN, &CserialDlg::OnBnClickedBtnFileopen)
//	ON_BN_CLICKED(IDC_BTN_TXSTART, &CserialDlg::OnBnClickedBtnTxstart)
//	ON_BN_CLICKED(IDC_BTN_TXPAUSE, &CserialDlg::OnBnClickedBtnTxpause)
//	ON_BN_CLICKED(IDC_BTN_TXSTOP, &CserialDlg::OnBnClickedBtnTxstop)
//	ON_MESSAGE(TX_THREAD_STOP, &CserialDlg::OnTxThreadStop)
ON_MESSAGE(DAT_SIMUL, &CserialDlg::OnDatSimul)
//ON_MESSAGE(DAT_SIM_STOP, &CserialDlg::OnDatSimStop)
ON_MESSAGE(DAT_SIM_FINISH, &CserialDlg::OnDatSimFinish)
ON_BN_CLICKED(IDC_BTN_GPAUSE, &CserialDlg::OnBnClickedBtnGpause)
ON_BN_CLICKED(IDC_BTN_GCLOSE, &CserialDlg::OnBnClickedBtnGclose)
ON_CBN_SELCHANGE(IDC_COMBO2, &CserialDlg::OnCbnSelchangeCombo2)
ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDC_CHECK1, &CserialDlg::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_CHECK2, &CserialDlg::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK3, &CserialDlg::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_CHECK4, &CserialDlg::OnBnClickedCheck4)
ON_BN_CLICKED(IDC_CHECK5, &CserialDlg::OnBnClickedCheck5)
ON_BN_CLICKED(IDC_CHECK6, &CserialDlg::OnBnClickedCheck6)
ON_BN_CLICKED(IDC_CHECK7, &CserialDlg::OnBnClickedCheck7)
ON_BN_CLICKED(IDC_CHECK8, &CserialDlg::OnBnClickedCheck8)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_CHECK9, &CserialDlg::OnBnClickedCheck9)
ON_WM_TIMER()
ON_MESSAGE(GRPH_DLG_DESTROY, &CserialDlg::OnGrphDlgDestroy)
ON_MESSAGE(GRPH_PLAY_PAUSE, &CserialDlg::OnGrphPlayPause)
ON_CBN_SELCHANGE(IDC_COMBO3, &CserialDlg::OnCbnSelchangeCombo3)
ON_BN_CLICKED(IDC_BTN_GDATA_CLR, &CserialDlg::OnBnClickedBtnGrphDataClr)
ON_CBN_SELCHANGE(IDC_COMBO_SINCOS, &CserialDlg::OnCbnSelchangeComboSincos)
ON_CBN_SELCHANGE(IDC_COMBO_SINCOS2, &CserialDlg::OnCbnSelchangeComboSincos2)
ON_CBN_SELCHANGE(IDC_COMBO_OPERATOR, &CserialDlg::OnCbnSelchangeComboOperator)
ON_EN_CHANGE(IDC_FORM_A, &CserialDlg::OnEnChangeFormA)
ON_EN_CHANGE(IDC_FORM_A2, &CserialDlg::OnEnChangeFormA2)
ON_EN_CHANGE(IDC_EDIT_FORM_B, &CserialDlg::OnEnChangeEditFormB)
ON_EN_CHANGE(IDC_EDIT_FORM_C, &CserialDlg::OnEnChangeEditFormC)
ON_EN_CHANGE(IDC_EDIT_FORM_D, &CserialDlg::OnEnChangeEditFormD)
ON_EN_CHANGE(IDC_EDIT_FORM_B2, &CserialDlg::OnEnChangeEditFormB2)
ON_EN_CHANGE(IDC_EDIT_FORM_C2, &CserialDlg::OnEnChangeEditFormC2)
ON_EN_CHANGE(IDC_EDIT_FORM_D2, &CserialDlg::OnEnChangeEditFormD2)
ON_EN_CHANGE(IDC_EDIT6, &CserialDlg::OnEnChangeEdit6)
ON_EN_CHANGE(IDC_EDIT7, &CserialDlg::OnEnChangeEdit7)
ON_EN_CHANGE(IDC_EDIT10, &CserialDlg::OnEnChangeEdit10)
ON_EN_CHANGE(IDC_EDIT11, &CserialDlg::OnEnChangeEdit11)
ON_EN_CHANGE(IDC_EDIT12, &CserialDlg::OnEnChangeEdit12)
ON_EN_CHANGE(IDC_EDIT13, &CserialDlg::OnEnChangeEdit13)
ON_EN_CHANGE(IDC_EDIT14, &CserialDlg::OnEnChangeEdit14)
ON_EN_CHANGE(IDC_EDIT15, &CserialDlg::OnEnChangeEdit15)
ON_EN_CHANGE(IDC_EDIT16, &CserialDlg::OnEnChangeEdit16)
//ON_CBN_SELCHANGE(IDC_COMBO_OPERATOR2, &CserialDlg::OnCbnSelchangeComboOperator2)
ON_EN_CHANGE(IDC_EDIT5, &CserialDlg::OnEnChangeEdit5)
ON_BN_CLICKED(IDC_BUTTON1, &CserialDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CserialDlg::OnBnClickedButton2)
//ON_BN_CLICKED(IDC_BUTTON3, &CserialDlg::OnBnClickedButton3)
ON_EN_CHANGE(IDC_EDIT_MAX_N, &CserialDlg::OnEnChangeEditMaxN)
ON_EN_CHANGE(IDC_EDIT22, &CserialDlg::OnEnChangeEdit22)
ON_EN_CHANGE(IDC_EDIT25, &CserialDlg::OnEnChangeEdit25)
//ON_MESSAGE(CONTROL_RENEWAL, &CserialDlg::OnControlRenewal)
ON_BN_CLICKED(IDC_BUTTON4, &CserialDlg::OnBnClickedButton4)
ON_EN_CHANGE(IDC_EDIT_SAMPLE_N, &CserialDlg::OnEnChangeEditSampleN)
ON_EN_CHANGE(IDC_EDIT2, &CserialDlg::OnEnChangeEdit2)
ON_BN_CLICKED(IDC_BUTTON5, &CserialDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON6, &CserialDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON8, &CserialDlg::OnBnClickedButton8)
ON_BN_CLICKED(IDC_BUTTON7, &CserialDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON9, &CserialDlg::OnBnClickedButton9)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CserialDlg::OnNMCustomdrawSlider1)
ON_BN_CLICKED(IDC_CHECK10, &CserialDlg::OnBnClickedCheck10)
ON_EN_CHANGE(IDC_EDIT31, &CserialDlg::OnEnChangeEdit31)
ON_BN_CLICKED(IDC_BUTTON11, &CserialDlg::OnBnClickedButton11)
ON_BN_CLICKED(IDC_BUTTON10, &CserialDlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// CserialDlg 메시지 처리기

BOOL CserialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	hCommWnd = m_hWnd;

	/* m_REditRx 폰트 스타일 지정 */
	////CFont fontIt;
	////fontIt.CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Consolas"));
	////m_REditRx.SetFont(&fontIt);
	////
	////chFormat.dwMask = CFM_COLOR;		// | CFM_UNDERLINE | CFM_BOLD;
	////chFormat.dwEffects = CFE_ALLCAPS;	// CFE_BOLD | CFE_UNDERLINE;
	////chFormat.yHeight = 200;
	////chFormat.crTextColor = RGB(255, 0, 0);
	
	m_iPortIndx = 3;
	m_iBaudIndx = 12;
	m_stRx.indx_w = 0;
	m_stRx.indx_r = 0;
	m_iItem_i = 2;	//1+1
	m_iGrph_i = 0;	//4+1
	for (int i = 0; i < 8; i++)
	{
		if (i <= m_iItem_i)
		{
			m_bItemChk[i] = TRUE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = TRUE;
		}
		else
		{
			m_bItemChk[i] = FALSE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = FALSE;
		}
	}
	//bAlOnTop = TRUE;
	//::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	bAlOnTop = FALSE;
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	CRect rect;
	this->GetWindowRect(rect);	//	GetClientRect(rect); 와 비교
	SIZE s;
	ZeroMemory(&s, sizeof(SIZE));
	s.cx = (LONG)::GetSystemMetrics(SM_CXSCREEN);	// 너비
	s.cy = (LONG)::GetSystemMetrics(SM_CYSCREEN);	// 높이 	
	SetWindowPos(NULL, s.cx-rect.Width(), 0, rect.Width(), rect.Height(), SWP_NOREPOSITION);
//	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOSIZE);

	nGrphDMax = 500;
	m_CGraph.m_LineChartCtrl.m_iViewMaxN = nGrphDMax;
	m_CGraph.m_LineChartCtrl.nXDataTotal = 0;

	n2PiCycle = 120;
	m_CGraph.m_LineChartCtrl.n2PiCycle = n2PiCycle;
	tick_ms = 10;

	dblFxA[0] = 1.0;
	dblFxB[0] = 1.0;
	dblFxC[0] = 0;
	dblFxD[0] = 0;
	cbxSinX[0] = 0;
	dblFxA[1] = 1.0;
	dblFxB[1] = 1.0;
	dblFxC[1] = 0;
	dblFxD[1] = 0;
	cbxSinX[1] = 1;

	for (int i = 0; i < 2; i++)
	{
		dblCPerc[i] = 0;
	}
	for (int i = 0; i < 6; i++)
	{
		dblDPerc[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		nHeadTail[i] = 0;
	}
	for (int i = 0; i < 6; i++)
	{
		nDiceIndx[i] = 0;
	}
	srand((unsigned int)(time(NULL)));

	//for (int i = 0; i < 6; i++)
	//{
	//	progBar[i].SetRange(0, 100);
	//	iProgBar[i] = 0;
	//}

	nNaturalE = 1;
	dblNatualE = (1 + (1 / 1));
	dblNatualE = pow(dblNatualE, (double)nNaturalE);
	
	nExpParam_m = 10;
	dblExpX2 = pow(1.0 + ((double)nNaturalE / (double)nExpParam_m), (double)nExpParam_m);

	dblZRe[0] = 2;
	dblZIm[0] = 1;
	dblZRe[1] = 1;
	dblZIm[1] = 2;
	dblZRe[2] = 0;
	dblZIm[2] = 0;
	dblZRe[3] = 0;
	dblZIm[3] = 0;
	dblZRe[4] = 0;
	dblZIm[4] = 0;
	dblZRe[5] = 0;
	dblZIm[5] = 0;
	dblZRotStep[0] = 1.0;
	dblZRotStep[1] = 1.0;
	m_CGraph.m_LineChartCtrl.dblZMax = 3;
	Renewal_Z();

	dblEuler_x = 1.0;
	nEuler_m = 1;
	szEuler_m = 30;
	dblEulerZRe[0] = dblEulerZRe[1] = 1.0;
	dblEulerZIm[0] = dblEulerZIm[1] = (dblEuler_x*PI) / nEuler_m;
	double ZRe, ZIm;
	for (int i = 0; i < nEuler_m - 1; i++)
	{
		ZRe = (dblEulerZRe[0]*dblEulerZRe[1]) - (dblEulerZIm[0]*dblEulerZIm[1]);
		ZIm = (dblEulerZRe[0]*dblEulerZIm[1]) + (dblEulerZRe[1]*dblEulerZIm[0]);
		dblEulerZRe[1] = ZRe;
		dblEulerZIm[1] = ZIm;
	}
	strEulerZRe[0].Format("%f", dblEulerZRe[0]);
	strEulerZIm[0].Format("%f", dblEulerZIm[0]);
	strEulerZRe[1].Format("%f", dblEulerZRe[1]);
	strEulerZIm[1].Format("%f", dblEulerZIm[1]);

	UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CserialDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CserialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		///* GUN */
		//CDC memDC;
		//memDC.CreateCompatibleDC(bmpRotate.GetDC());
		//CBitmap bitmap;
		//bitmap.LoadBitmapA(IDB_BITMAP1);
		//memDC.SelectObject(&bitmap);
		//
		//CStatic *staticSize = (CStatic *)GetDlgItem(IDC_ROTATE);
		//CRect rect;
		//staticSize->GetClientRect(rect);
		//int w = rect.Width();
		//int h = rect.Height();
		//bmpRotate.GetDC()->StretchBlt(0, 0, w, h, &memDC, 0, 0, 75, 75, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CserialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CserialDlg::OnBnClickedBtnSeropen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/* 시리얼통신 파라미터 설정 */
	m_iSPRm_Data = 8;	// 데이타비트수 = 8 
	m_iSPRm_Stop = 0;	// 스탑비트수 = 1 
	m_iSPRm_Parity = 0;	// 패러티 : no 

	/* "포트열기" "포트닫기" 토글 */
	if (m_bSerOpen == FALSE){
		m_bSerOpen = TRUE;
	}
	else{
		m_bSerOpen = FALSE;
	}

	UpdateData(TRUE);
	CString CStrPortNum;	// = "Open Port";

	if (m_bSerOpen == TRUE)
	{
		CStrPortNum.Format(_T("Open Port : %s\r\n"), strPort[m_iPortIndx]);
		if (m_CSerial.m_bPortOpen == FALSE)
		{
			if (m_CSerial.OpenPort(strPort[m_iPortIndx], dwBaud[m_iBaudIndx], m_iSPRm_Data, m_iSPRm_Stop, m_iSPRm_Parity) == TRUE)
			{
				CButton * pButton = (CButton*)GetDlgItem(IDC_BTN_SEROPEN);
				if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
					pButton->SetWindowText(_T("Close"));
				}
			}
		}
		else
		{
			AfxMessageBox(_T("Already Port Opeded !"));
		}
	}
	else
	{
		if (m_CSerial.m_bPortOpen == TRUE)
		{
			m_CSerial.ClosePort();
			CStrPortNum.Format(_T("Close Port : %s \r\n"), strPort[m_iPortIndx]);
		}
		else
		{
			CStrPortNum.Format(_T("%s Port not yet open \r\n"), strPort[m_iPortIndx]);
		}
		CButton * pButton = (CButton*)GetDlgItem(IDC_BTN_SEROPEN);
		if (pButton && ::IsWindow(pButton->GetSafeHwnd()))
		{
			pButton->SetWindowText(_T("Open"));
		}
	}
}


BYTE CserialDlg::byIndexData(int xData)
{
	BYTE byData;

	switch (xData)
	{
		case 0:
			byData = 5;
			break;
		case 1:
			byData = 6;
			break;
		case 2:
			byData = 7;
			break;
		case 3:
			byData = 8;
			break;
	}

	return byData;
}


void CserialDlg::OnBnClickedBtnSend()
{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	BYTE aTmp[2048];
//	BYTE aTx[1024]; 
//	WORD txcnt;
//	WORD size;
//	CString strTmp;
//
//	UpdateData(TRUE);
//
//	strTmp = m_strSend;
//	strTmp.Replace(_T(" "), _T(""));	// 공백 없애기
//
//	size = strTmp.GetLength();			// 공백을 없앤 문자열 길이 얻기 
//	strcpy((char*)aTmp, (CStringA)strTmp.GetBuffer(size));	// 전저리기에 _CRT_SECURE_NO_WARNINGS 추가
//
////	// HEX										
//	if (size % 2)
//	{
//		AfxMessageBox(_T("odd input error !"));
//	}
//	txcnt = size / 2;
//	m_CUtils.hexStr2hexBin(aTmp, aTx, txcnt);
//
////	// ASCII
////	{											
////		txcnt = size;
////		strcpy((char*)aTxBuff, (CStringA)strTmp.GetBuffer(strTmp.GetLength()));	// 전저리기에 _CRT_SECURE_NO_WARNINGS 추가
////	}
//
//	m_CSerial.WriteComm((BYTE *)aTx, txcnt);
//
//#if 0
//	strTmp = _T("TX	");
//	strTmp += m_strSend;
//	m_pCLogManager->Log(strTmp);
//#endif


	//m_REditRx.Clear();
	m_REditRx.SetWindowText(_T(""));
	UpdateData(FALSE);
}

//#define pi	3.14159265359
//
afx_msg LRESULT CserialDlg::OnRxEvent(WPARAM wParam, LPARAM lParam)
{
	CString strHex = _T("");
	BYTE readByte;
	int size;

	size = (m_CSerial.m_QueueRead).GetSize();		// 포트로 들어온 데이터 갯수
	m_CGraph.m_LineChartCtrl.m_nBuffer = size;
	if (size > 150)
	{
		m_CGraph.m_LineChartCtrl.m_nViewPeriod++;
	}

	for (int i = 0; i < size; i++)
	{
		(m_CSerial.m_QueueRead).GetByte(&readByte);
		m_stRx.buff[m_stRx.indx_w++] = readByte;
		m_stRx.indx_w %= RX_BUFF_SIZE;
	}
	OnRxFrameCheck();

	return 1;

//	m_strRxPack = _T("");
//	for (int i = 0; i < size; i++)
//	{
//		(m_CSerial.m_QueueRead).GetByte(&readByte);		// 큐에서 데이터 한개를 읽어옴
//		/* HEX */
//		m_stRx.buff[m_stRx.indx_w++] = readByte;
//		m_stRx.indx_w %= RX_BUFF_SIZE;
//
//		//if (readByte == RX_ETX)
//		{
//			OnRxFrameCheck();
//			if (m_REditRx.GetLineCount() > 50)
//			{
//				m_REditRx.SetWindowText(_T(""));
//				UpdateData(FALSE);
//			}
//		}
//		strHex.Format(_T("%02X "), readByte);
//		m_strRxPack += strHex;
//		// ASCII
//		//strTmp.Format(_T("%c"), aByte);
//		//CStrRst += strTmp;
//	}
//
//	m_REditRx.SetSel(0, -1);
//	m_REditRx.SetSel(-1, 0);
//	m_REditRx.SetSelectionCharFormat(chFormat);
//	m_REditRx.ReplaceSel(m_strRxPack);

//	return 1;
}

typedef enum
{
	STEP_STX = 0,
	STEP_LEN,
	STEP_CMD,
	STEP_DATA,
	STEP_BCC,
	STEP_CR,
	STEP_LF
}enumRx;
#if (1)
int CserialDlg::OnRxFrameCheck()
{
	BYTE temp;

	static enumRx s_Step = STEP_STX;
	static int stt_n = 0;
	static int sttDly[20] = { 0 };
	static BOOL sttPress[20] = { FALSE };
	static BYTE s_PreByte = 0;

	while (m_stRx.indx_w != m_stRx.indx_r)
	{
		temp = m_stRx.buff[m_stRx.indx_r++];
		m_stRx.indx_r %= RX_BUFF_SIZE;

		switch (s_Step)
		{
		case STEP_STX:
			if (temp == RX_STX)
			{
				m_stRx.bcc = 0;
				s_Step = STEP_LEN;
			}
			break;
		case STEP_LEN:
			m_stRx.bcc ^= temp;
			m_stRx.len = temp;
			m_stRx.indx_d = 0;
			s_Step = STEP_CMD;
			break;
		case STEP_CMD:
			m_stRx.data[m_stRx.indx_d++] = temp;
			m_stRx.bcc ^= temp;
			s_Step = STEP_DATA;
			break;
		case STEP_DATA:
			m_stRx.data[m_stRx.indx_d++] = temp;
			m_stRx.bcc ^= temp;
			if (m_stRx.indx_d == m_stRx.len)
			{
				s_Step = STEP_BCC;
			}
			break;
		case STEP_BCC:
			if (m_stRx.bcc == temp)
			{
				s_Step = STEP_CR;
			}
			break;
		case STEP_CR:
			if (temp == RX_CR)
			{
				s_Step = STEP_LF;
			}
			break;
		case STEP_LF:
			if (temp == RX_LF)
			{
				int i = 0;
				int dat_1 = 0;
				int dat_2 = 0;
				bool bPrint = FALSE;
				if (m_CGraph.m_LineChartCtrl.m_hWnd)
				{
					for (int GRPH_i = 0; GRPH_i < m_iGrph_i+1; GRPH_i++)
					{
						for (int ITEM_i = 0; ITEM_i < m_iItem_i + 1; ITEM_i++)
						{
							bPrint = FALSE;
							dat_1 = (int)((int)(m_stRx.data[i * 2 + 0] << 8) + (int)(m_stRx.data[i * 2 + 1] << 0));		
							if (ITEM_i == 0)	// 첫번째 데이타는 RAW, 두번째 데이타는 BASE, 세번째부터는 상관없음. 
							{
								dat_2 = (int)((int)(m_stRx.data[i * 2 + 2] << 8) + (int)(m_stRx.data[i * 2 + 3] << 0));	// BASE 데이타
							}else
							{
								dat_2 = dat_1;
							}

							if (ITEM_i == 0)
							{
								if (sttPress[GRPH_i] == FALSE)
								{
									if (dat_1 - dat_2 > 50)
									{
										sttPress[GRPH_i] = TRUE;
										sttDly[GRPH_i] = 0;
									}
								}
								else
								{
									sttDly[GRPH_i]++;
									if (sttDly[GRPH_i] == 7)
									{
										bPrint = TRUE;
									}
								}
								if (dat_1 - dat_2 < 100)
								{
									sttPress[GRPH_i] = FALSE;
								}
							}
							m_CGraph.m_LineChartCtrl.m_aData[GRPH_i * (m_iItem_i + 1) + ITEM_i].Add(dat_1, dat_2, bPrint);
							i++;
						}
					}

					if (stt_n >= m_CGraph.m_LineChartCtrl.m_nViewPeriod)
					{
						m_CGraph.m_LineChartCtrl.RedrawWindow();
						stt_n = 0;
					}
					else
					{
						stt_n++;
					}
				}
			}
			s_Step = STEP_STX;
			break;
		default:
			s_Step = STEP_STX;
			break;
		}

		if ((temp == RX_LF) && (s_PreByte == RX_CR))
		{
			s_Step = STEP_STX;
		}

		s_PreByte = temp;
	}

	return 0;
}
#endif
#if (0)
int CserialDlg::OnRxFrameCheck()
{
	BYTE temp;

	static enumRx s_Step = STEP_STX;
	static int stt_n = 0;
	static int sttDly[20] = { 0 };
	static BOOL sttPress[20] = { FALSE };
	static BYTE s_PreByte = 0;
	//static int stt_print_n = 0;

	static int status = 0;

	while (m_stRx.indx_w != m_stRx.indx_r)
	{
		temp = m_stRx.buff[m_stRx.indx_r++];
		m_stRx.indx_r %= RX_BUFF_SIZE;


		switch (status)
		{
		case 0:
			if (temp == RX_STX)
			{
				status++;
				m_stRx.indx_d = 0;
			}
			break;
		case 1:
			m_stRx.data[m_stRx.indx_d++] = temp;
			if (temp == RX_CR)
			{
				status++;
			}
			break;
		case 2:
			if (temp == RX_LF)
			{
				status = 0;
				int i = 0;
				int dat_1 = 0;
				int dat_2 = 0;
				bool bPrint = FALSE;
				if (m_CGraph.m_LineChartCtrl.m_hWnd)
				{
					for (int grph_i = 0; grph_i < m_CGraph.m_LineChartCtrl.m_iGrphN; grph_i++)
					{
						for (int item_i = 0; item_i < m_CGraph.m_LineChartCtrl.m_iItemN; item_i++)
						{
							bPrint = FALSE;
							dat_1 = (int)((int)(m_stRx.data[i * 2 + 1] << 8) + (int)(m_stRx.data[i * 2 + 2] << 0));
							if (item_i == 0)	// 첫번째 데이타는 RAW, 두번째 데이타는 BASE, 세번째부터는 상관없음. 
							{
								dat_2 = (int)((int)(m_stRx.data[i * 2 + 3] << 8) + (int)(m_stRx.data[i * 2 + 4] << 0));	// BASE 데이타
							}
							else
							{
								dat_2 = dat_1;
							}

							if (item_i == 0)
							{
								if (sttPress[grph_i] == FALSE)
								{
									if (dat_1 - dat_2 > 50)
									{
										sttPress[grph_i] = TRUE;
										sttDly[grph_i] = 0;
									}
								}
								else
								{
									sttDly[grph_i]++;
									if (sttDly[grph_i] == 7)
									{
										bPrint = TRUE;
									}
								}
								if (dat_1 - dat_2 < 100)
								{
									sttPress[grph_i] = FALSE;
								}
							}
							m_CGraph.m_LineChartCtrl.m_aData[grph_i * m_CGraph.m_LineChartCtrl.m_iItemN + item_i].Add(dat_1, dat_2, bPrint);
							i++;
						}
					}

					if (stt_n >= m_nDrawPeriod)
					{
						m_CGraph.m_LineChartCtrl.RedrawWindow();
						stt_n = 0;
					}
					else
					{
						stt_n++;
					}
				}
			}
			else
			{
				status--;
			}
			break;
		}
	}
		
	return 0;


}
#endif
#if (0)
int CserialDlg::OnRxFrameCheck()
{
	BYTE temp;

	static enumRx step = STEP_STX;
	static int sttCnt = 0;
	static int sttIndx = 0;
	static int sttData = 0;
	static bool sttEven = FALSE;
	static WORD sttCS;
	static BYTE sttPre = 0;

	while (m_stRx.indx_w != m_stRx.indx_r)
	{
		temp = m_stRx.buff[m_stRx.indx_r++];
		m_stRx.indx_r %= RX_BUFF_SIZE;

		if (temp == 0x0A)
		{
			if (sttPre == 0x0D)
			{
				if (step == STEP_ETX)
				{
					int countG = m_CGraph.m_LineChartCtrl.m_iGrph_i;
					int countI = m_CGraph.m_LineChartCtrl.m_iItem_i;
					int i = 0;
					int iData = 0;
					if (m_CGraph.m_LineChartCtrl.m_hWnd)
					{
						for (int grph_i = 0; grph_i < countG; grph_i++)
						{
							for (int item_i = 0; item_i < countI; item_i++)
							{
								iData = (int)((int)(m_stRx.data[i * 2 + 4] << 8) + (int)(m_stRx.data[i * 2 + 5] << 0));
								m_CGraph.m_LineChartCtrl.m_aData[grph_i * countI + item_i].Add(iData, 0, 0);
								i++;
							}
						}
						sttCnt++;
						sttCnt %= 10000000;

						if (m_CGraph.m_LineChartCtrl.m_aData[0].lstData.GetSize() < 600)
						{
							//m_CGraph.m_LineChartCtrl.RedrawWindow();
							m_CGraph.m_LineChartCtrl.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
						}
						else if (m_CGraph.m_LineChartCtrl.m_aData[0].lstData.GetSize() < 1200)
						{
							if (!(sttCnt % 2))
							{
								//m_CGraph.m_LineChartCtrl.RedrawWindow();
								m_CGraph.m_LineChartCtrl.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
							}
						}
						else
						{
							if (!(sttCnt % 3))
							{
								//m_CGraph.m_LineChartCtrl.RedrawWindow();
								m_CGraph.m_LineChartCtrl.RedrawWindow(NULL, NULL, RDW_INVALIDATE);
							}
						}
						//m_CGraph.m_LineChartCtrl.RedrawWindow();
					}
				}
				step = STEP_STX;
			}
		}

		switch (step)
		{
			case STEP_STX:
				if (temp == SK_STX)
				{
					m_stRx.cs = 0;
					step = STEP_LEN;
				}
				break;
			case STEP_LEN:
				m_stRx.cs += temp;
				m_stRx.len = temp&0x7f;
				m_stRx.indx_d = 0;
				step = STEP_CMD;
				break;
			case STEP_CMD:
				m_stRx.data[m_stRx.indx_d++] = temp;
				m_stRx.cs += temp;
				step = STEP_DATA;
				break;
			case STEP_DATA:
				m_stRx.data[m_stRx.indx_d++] = temp;
				m_stRx.cs += temp;
				if (m_stRx.indx_d == m_stRx.len)
				{
					sttEven = FALSE;
					sttCS = 0;
					step = STEP_BCC;
				}
				break;
			case STEP_BCC:
				if (sttEven == FALSE)
				{
//					sttCS = temp;
//					sttCS <<= 8;
					sttEven = TRUE;
				}
				else
				{
//					sttCS += temp;
//					if (m_stRx.cs == sttCS)
//					{
//						sttEven = FALSE;
//						step = STEP_ETX;
//					}
					step = STEP_ETX;
				}
				break;
			case STEP_ETX:
				break;
			default:
				step = STEP_STX;
				break;
		}
		sttPre = temp;
	}

	return 0;
}
#endif

void CserialDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	CWnd* pCtl = GetDlgItem(IDC_RICHEDIT_RX);
//
//	if (!pCtl){ return; }
//
//	CRect rectCtl;
//	pCtl->GetWindowRect(&rectCtl);
//	ScreenToClient(&rectCtl);
//
//	pCtl->MoveWindow(rectCtl.left, rectCtl.top, cx - 2 * rectCtl.left, cy - rectCtl.top - rectCtl.left, TRUE);

	return;
}


void CserialDlg::OnBnClickedBtnGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

#if (1)
	m_CGraph.DestroyWindow();
	m_CGraph.Create(IDD_GRAPH_DIALOG, this);
	m_CGraph.ShowWindow(SW_SHOW);

	m_CGraph.m_LineChartCtrl.m_iGrphN = m_iGrph_i + 1;
	m_CGraph.m_LineChartCtrl.m_iItemN = m_iItem_i + 1;
#endif

#if (0)
	//if (m_pTHRsim == NULL)
	//{
	//	m_pTHRsim = AfxBeginThread(thread_DATsimul, this);
	//	if (m_pTHRsim == NULL)
	//	{
	//		AfxMessageBox(_T("Thread_SendFile Error"));
	//	}
	//	m_pTHRsim->m_bAutoDelete = FALSE;
	//	m_eTHRtxWork = THREAD_RUNNING;
	//}
	//else
	//{
	//	if (m_eTHRtxWork == THREAD_PAUSE)
	//	{
	//		m_pTHRsim->ResumeThread();
	//		m_eTHRtxWork = THREAD_RUNNING;
	//	}
	//}

	CWinThread *p1 = NULL;
	p1 = AfxBeginThread(Thread_ProgressBar, this);
	if (p1 == NULL)
	{
		AfxMessageBox(_T("Thread_4 Error"));
	}
#endif

	OnCbnSelchangeCombo3();
	//SetTimer(T_TIMER, tick_ms, NULL);
}


void CserialDlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_iGrphN = m_iGrph_i + 1;
	m_CGraph.m_LineChartCtrl.m_iItemN = m_iItem_i + 1;

	for (int i = 0; i < 8; i++)
	{
		if (i <= m_iItem_i)
		{
			m_bItemChk[i] = TRUE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = TRUE;
		}
		else
		{
			m_bItemChk[i] = FALSE;
			m_CGraph.m_LineChartCtrl.m_bItemEn[i] = FALSE;
		}			
	}
	UpdateData(FALSE);
}


void CserialDlg::OnCbnSelchangeComboPort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CserialDlg::OnBnClickedBtnFileopen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	/* ▶ File Path 얻기 */
	char szFilters[] = "Data(*.dat,*.txt) | *.dat;*.txt; | All Files(*.*)|*.*||";
	//	char szFilters[] = "All Files(*.*)|*.*||";	
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);
	if (fileDlg.DoModal() == IDOK)					// 선택하고 확인하면 IDOK 로 넘어옵니다
	{
		m_STRdatPath = fileDlg.GetPathName();		// 디렉토리+파일이름+확장자
	}
	else
	{
		return;
	}
	if (m_SFsim.Open(m_STRdatPath, CFile::modeRead | CFile::typeText)){}
	else{}


	if (m_pTHRsim == NULL)
	{
		m_pTHRsim = AfxBeginThread(thread_DATsimul, this);
		if (m_pTHRsim == NULL)
		{
			AfxMessageBox(_T("Thread_SendFile Error"));
		}
		m_pTHRsim->m_bAutoDelete = FALSE;
		m_eTHRtxWork = THREAD_RUNNING;
	}
	else
	{
		if (m_eTHRtxWork == THREAD_PAUSE)
		{
			m_pTHRsim->ResumeThread();
			m_eTHRtxWork = THREAD_RUNNING;
		}
	}


}

UINT CserialDlg::thread_DATsimul(LPVOID pParam)
{
	CserialDlg *aa = (CserialDlg*)pParam;
	CserialDlg *pDlg = (CserialDlg*)AfxGetApp()->m_pMainWnd;


	CString strLine = _T("");
	CString strData = _T("");
	BYTE aHexStr[2048];
	BYTE aHexBin[1024];

	while (1)
	{
		Sleep(0);

		if (pDlg->m_CGraph.m_bWindow == TRUE)
		{
			if (pDlg->m_SFsim.ReadString(strLine) == FALSE)
			{
//				pDlg->SendMessage(DAT_SIM_STOP, 0, 0);
				pDlg->SendMessage(DAT_SIM_FINISH, 0, 0);
			}
			else
			{
				int pos = strLine.Find(_T("#OK"));
				strData = strLine.Mid(9, pos - 9);

				strcpy((char*)aHexStr, (CStringA)strData.GetBuffer(strData.GetLength()));	// 전저리기에 _CRT_SECURE_NO_WARNINGS 추가
				pDlg->m_CUtils.hexStr2hexBin(aHexStr, aHexBin, strData.GetLength() / 2);

				pDlg->SendMessage(DAT_SIMUL, 0, (LPARAM)aHexBin);
			}
		}
	}
	return 0;
}

afx_msg LRESULT CserialDlg::OnDatSimul(WPARAM wParam, LPARAM lParam)
{
	BYTE* ptr;

	ptr = (BYTE*)lParam;

	int i = 0;
	int iData = 0;
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		for (int grph_i = 0; grph_i < m_CGraph.m_LineChartCtrl.m_iGrphN; grph_i++)
		{
			for (int item_i = 0; item_i <m_CGraph.m_LineChartCtrl.m_iItemN; item_i++)
			{
				iData = (int)((int)(*(ptr + i * 2) << 8) + (int)(*(ptr + i * 2 + 1) << 0));
				m_CGraph.m_LineChartCtrl.m_aData[grph_i * m_CGraph.m_LineChartCtrl.m_iItemN + item_i].Add(iData, 0, 0);
				i++;
			}
		}
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}
	return 0;
}

afx_msg LRESULT CserialDlg::OnDatSimFinish(WPARAM wParam, LPARAM lParam)
{
	if (m_pTHRsim != NULL)
	{
		m_SFsim.Close();
		m_pTHRsim->SuspendThread();
		DWORD dwResult;
		::GetExitCodeThread(m_pTHRsim->m_hThread, &dwResult);

		delete m_pTHRsim;
		m_pTHRsim = NULL;

		m_eTHRtxWork = THREAD_STOP;
	}
	return 0;
}

void CserialDlg::OnBnClickedBtnGpause()
{
	static bool bPausePlay = FALSE;

	if (m_CGraph.m_bWindow == FALSE)
	{
		bPausePlay = FALSE;
	}

	if (bPausePlay == FALSE)
	{
		if (m_pTHRsim == NULL)
		{
			AfxMessageBox(_T("Thread Not Started !"));
		}
		else
		{
			m_pTHRsim->SuspendThread();
			m_eTHRtxWork = THREAD_PAUSE;
		}
		bPausePlay = TRUE;
	}
	else
	{
		if (m_pTHRsim == NULL)
		{
			//m_pTHRsim = AfxBeginThread(thread_DATsimul, this);
			//if (m_pTHRsim == NULL)
			//{
			//	AfxMessageBox(_T("Thread_SendFile Error"));
			//}
			//m_pTHRsim->m_bAutoDelete = FALSE;
			//m_eTHRtxWork = THREAD_RUNNING;
		}
		else
		{
			if (m_eTHRtxWork == THREAD_PAUSE)
			{
				m_pTHRsim->ResumeThread();
				m_eTHRtxWork = THREAD_RUNNING;
			}
		}
		bPausePlay = FALSE;
	}

}

void CserialDlg::OnBnClickedBtnGclose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pTHRsim == NULL)
	{
		AfxMessageBox(_T("Thread Not Started !"));
	}
	else
	{
		m_SFsim.Close();
		m_CGraph.m_bWindow = FALSE;
		m_CGraph.DestroyWindow();
		m_pTHRsim->SuspendThread();
		DWORD dwResult;
		::GetExitCodeThread(m_pTHRsim->m_hThread, &dwResult);

		delete m_pTHRsim;
		m_pTHRsim = NULL;

		m_eTHRtxWork = THREAD_STOP;
	}
}

void CserialDlg::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_iGrphN = m_iGrph_i + 1;
	m_CGraph.m_LineChartCtrl.m_iItemN = m_iItem_i + 1;
}

//
//UINT CserialDlg::Thread_SendFile(LPVOID pParam)
//{
//	CserialDlg *aa = (CserialDlg*)pParam;
//	CserialDlg *pDlg = (CserialDlg*)AfxGetApp()->m_pMainWnd;
//
//	CString strLine = _T("");
//	CString strData = _T("");
//	BYTE aHexStr[2048];
//	BYTE aHexBin[1024];
//
//	while (pDlg->m_eTHRtxWork == THREAD_RUNNING)
//	{
//		Sleep(100);
//
//		if (pDlg->m_SFsim.ReadString(strLine) == FALSE)
//		{
//			pDlg->PostMessage(TX_THREAD_STOP, 0, 0);
//		}
//		else
//		{
//			int pos = strLine.Find(_T("#OK"));
//			strData = strLine.Mid(9, pos - 9);
//
//			strcpy((char*)aHexStr, (CStringA)strData.GetBuffer(strData.GetLength()));	// 전저리기에 _CRT_SECURE_NO_WARNINGS 추가
//			aHexBin[0] = 0x02;
//			aHexBin[1] = strData.GetLength() / 2;
//			pDlg->m_CUtils.hexStr2hexBin(aHexStr, &aHexBin[2], strData.GetLength() / 2);
//
//			aHexBin[2 + strData.GetLength() / 2] = 0;
//			for (int i = 0; i < strData.GetLength() / 2 + 1; i++)
//			{
//				aHexBin[2 + strData.GetLength() / 2] ^= aHexBin[1 + i];
//			}
//			aHexBin[2 + strData.GetLength() / 2 + 1] = 0x03;
//
//			pDlg->m_CSerial.WriteComm((BYTE *)aHexBin, strData.GetLength() / 2 + 4);
//		}
//	}
//	
//	return 0;
//}
//
//void CserialDlg::OnBnClickedBtnTxstart()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	if (m_pTHRsim == NULL)
//	{
//		m_pTHRsim = AfxBeginThread(Thread_SendFile, this);
//		if (m_pTHRsim == NULL)
//		{
//			AfxMessageBox(_T("Thread_SendFile Error"));
//		}
//		m_pTHRsim->m_bAutoDelete = FALSE;
//		m_eTHRtxWork = THREAD_RUNNING;
//	}
//	else
//	{
//		if (m_eTHRtxWork == THREAD_PAUSE)
//		{
//			m_pTHRsim->ResumeThread();
//			m_eTHRtxWork = THREAD_RUNNING;
//		}
//	}
//}
//
//
//void CserialDlg::OnBnClickedBtnTxpause()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	if (m_pTHRsim == NULL)
//	{
//		AfxMessageBox(_T("Thread Not Started !"));
//	}
//	else
//	{
//		m_pTHRsim->SuspendThread();
//		m_eTHRtxWork = THREAD_PAUSE;
//	}
//}
//
//
//void CserialDlg::OnBnClickedBtnTxstop()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	if (m_pTHRsim == NULL)
//	{
//		AfxMessageBox(_T("Thread Not Started !"));
//	}
//	else
//	{
//		m_SFsim.Close();
//		m_pTHRsim->SuspendThread();
//		DWORD dwResult;
//		::GetExitCodeThread(m_pTHRsim->m_hThread, &dwResult);
//
//		delete m_pTHRsim;
//		m_pTHRsim = NULL;
//
//		m_eTHRtxWork = THREAD_STOP;
//	}
//}
//
//
//afx_msg LRESULT CserialDlg::OnTxThreadStop(WPARAM wParam, LPARAM lParam)
//{
//	if (m_pTHRsim != NULL)
//	{
//		m_SFsim.Close();
//		m_pTHRsim->SuspendThread();
//		DWORD dwResult;
//		::GetExitCodeThread(m_pTHRsim->m_hThread, &dwResult);
//
//		delete m_pTHRsim;
//		m_pTHRsim = NULL;
//
//		m_eTHRtxWork = THREAD_STOP;
//	}
//	return 0;
//}


BOOL CserialDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(140, 170, 230));
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CserialDlg::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[0] = m_bItemChk[0];
}


void CserialDlg::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[1] = m_bItemChk[1];
}


void CserialDlg::OnBnClickedCheck3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[2] = m_bItemChk[2];
}


void CserialDlg::OnBnClickedCheck4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[3] = m_bItemChk[3];
}


void CserialDlg::OnBnClickedCheck5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[4] = m_bItemChk[4];
}


void CserialDlg::OnBnClickedCheck6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[5] = m_bItemChk[5];
}


void CserialDlg::OnBnClickedCheck7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[6] = m_bItemChk[6];
}


void CserialDlg::OnBnClickedCheck8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_bItemEn[7] = m_bItemChk[7];
}


BOOL CserialDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			UpdateData(TRUE);
			switch (xGraph)
			{
				case 3:
					OnBnClickedBtnGrphDataClr();
					UpdateData(TRUE);
					//dblZRe[3] = 0;
					//dblZIm[3] = 0;
					//dblZRe[4] = 0;
					//dblZIm[4] = 0;
					//dblZRe[5] = 0;
					//dblZIm[5] = 0;
					for (int i = 0; i < 2; i++)
					{
						dblZRe[i] = atof(strZRe[i]);
						dblZIm[i] = atof(strZIm[i]);
						dblZRotStep[i] = atof(strZRotStep[i]);
					}
					Renewal_Z();
					SetTimer(T_TIMER, tick_ms, NULL);
					break;			
				case 4:
					break;
			}
			dblNatualE = pow((1 + (1 / (double)nNaturalE)), (double)nNaturalE);
			
			//dblEulerZRe[0] = dblEulerZRe[1] = 1.0;
			//dblEulerZIm[0] = dblEulerZIm[1] = (dblEuler_x*PI) / nEuler_m;
			//double ZRe, ZIm;
			//for (int i = 0; i < nEuler_m - 1; i++)
			//{
			//	ZRe = (dblEulerZRe[0]*dblEulerZRe[1]) - (dblEulerZIm[0]*dblEulerZIm[1]);
			//	ZIm = (dblEulerZRe[0]*dblEulerZIm[1]) + (dblEulerZRe[1]*dblEulerZIm[0]);
			//	dblEulerZRe[1] = ZRe;
			//	dblEulerZIm[1] = ZIm;
			//}
			//strEulerZRe[0].Format("%f", dblEulerZRe[0]);
			//strEulerZIm[0].Format("%f", dblEulerZIm[0]);
			//strEulerZRe[1].Format("%f", dblEulerZRe[1]);
			//strEulerZIm[1].Format("%f", dblEulerZIm[1]);
			UpdateData(FALSE);
			return TRUE;
//		case VK_F2:
//		case VK_F3:
//			return TRUE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CserialDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT); // 글자색과 배경색이 일치하게 만듬

	pDC->SetTextColor(RGB(0, 0, 0));	// 글자색

	if (nCtlColor == CTLCOLOR_STATIC)	// 정적 스테틱
	{
		return CreateSolidBrush(RGB(140, 170, 230));
	}

//	if (nCtlColor == CTLCOLOR_BTN)		// 버튼(체크버튼/라디오버튼)
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}

//	if (nCtlColor == CTLCOLOR_EDIT)		// 버튼(체크버튼/라디오버튼)
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}

//	if (nCtlColor == CTLCOLOR_LISTBOX)		// 버튼(체크버튼/라디오버튼)
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}

//	if (nCtlColor == CTLCOLOR_DLG)		// 다이얼로그 박스 바탕색
//	{
//		return CreateSolidBrush(RGB(140, 170, 230));
//	}


	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CserialDlg::OnBnClickedCheck9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (bAlOnTop == TRUE)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CserialDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.	
	if (nIDEvent == T_TIMER)
	{
		switch (xGraph)
		{
			case 0:		// sinx
				taskSinX();
				break;
			case 1:		// euler's e
				taskNatural_e();
				break;
			case 2:		// exp(x)
				taskExpX();
				break;
			case 3:		// z=a+bi
				break;
			case 5:		// coin/dice
				taskCoin();
				break;
		}
	}
	if (nIDEvent == T_ZROTATE)
	{
		taskZRotate();
	}	
	if (nIDEvent == T_ZMULTIPLY)
	{
		taskZMultiply();
	}
	if (nIDEvent == T_EULER_1)
	{
		taskEulerForm();
	}
	if (nIDEvent == T_EULER_2)
	{
		taskEulerForm2();
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CserialDlg::OnGrphDlgDestroy(WPARAM wParam, LPARAM lParam)
{
	KillTimer(T_TIMER);
	KillTimer(T_ZROTATE);
	return 0;
}


afx_msg LRESULT CserialDlg::OnGrphPlayPause(WPARAM wParam, LPARAM lParam)
{
	if (m_CGraph.m_LineChartCtrl.m_bLBtnDbClk == FALSE)
	{	
		switch(xGraph)
		{
			case 0:
			case 1:
			case 2:
				SetTimer(T_TIMER, tick_ms, NULL);
				break;
			case 3:
				SetTimer(T_ZROTATE, tick_ms, NULL);
				break;
			case 4:
				SetTimer(T_TIMER, tick_ms, NULL);
				break;
		}		
	}
	else
	{
		switch(xGraph)
		{
			case 0:
			case 1:
			case 2:
				KillTimer(T_TIMER);
				break;
			case 3:
				KillTimer(T_ZROTATE);
				break;
			case 4:
				KillTimer(T_TIMER);
				break;
		}
	}
	
	return 0;
}


//UINT CserialDlg::Thread_ProgressBar(LPVOID pParam)
//{
//	CserialDlg *aa = (CserialDlg*)pParam;
//	CserialDlg *pDlg = (CserialDlg*)AfxGetApp()->m_pMainWnd;
//	while (1)
//	{
//		Sleep(100);
//		for (int i = 0; i < 6; i++)
//		{
//			pDlg->progBar[i].SetPos(pDlg->iProgBar[i]);
//		}		
//	}
//	return 0;
//}

void CserialDlg::OnCbnSelchangeCombo3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (xGraph)
	{
		case 0: 
		case 1: 
		case 2:	
			KillTimer(T_TIMER);
			break;
		case 3:
			KillTimer(T_ZROTATE);
			KillTimer(T_ZMULTIPLY);
			break;
		case 4: 
			KillTimer(T_EULER_1); 
			break;
		case 5:
			nDiceTry = 0;
			for (int i = 0; i < 6; i++)
			{
				nDiceIndx[i] = 0;
			}
			nHeadTail[0] = 0;
			nHeadTail[1] = 0;
			KillTimer(T_TIMER);
			break;
	}

	UpdateData(TRUE);
	switch (xGraph)
	{
		case 0: 
			m_iItem_i = 2;
			tick_ms = 10; 
			nGrphDMax = 500;
			break;
		case 1: 
			m_iItem_i = 0;
			tick_ms = 50;
			nGrphDMax = 1000;
			break;
		case 2:	
			m_iItem_i = 1;
			tick_ms = 50; 
			nGrphDMax = 1000;
			break;
		case 3:
			m_iItem_i = 5;
			tick_ms = 50;
			nGrphDMax = 180;	
			m_CGraph.m_LineChartCtrl.iZMulStep = 0;
			break;
		case 4: 
			m_iItem_i = 0;
			tick_ms = 100;
			nGrphDMax = 500;
			break;
		case 5:
			m_iItem_i = 0;
			tick_ms = 10;
			nGrphDMax = 1000;
			break;
	}
	m_CGraph.m_LineChartCtrl.m_iItemN = m_iItem_i + 1;
	m_CGraph.m_LineChartCtrl.xGraph = xGraph;
	m_CGraph.m_LineChartCtrl.m_iViewMaxN = nGrphDMax;

	OnBnClickedBtnGrphDataClr();

	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		switch (xGraph)
		{
		case 0:
			m_CGraph.SetWindowPos(NULL, 0, 0, 1600, 500, SWP_NOREPOSITION);
			SetTimer(T_TIMER, tick_ms, NULL);
			break;
		case 1:
			m_CGraph.SetWindowPos(NULL, 0, 0, 800 + 2 * 8, 800 + 2 * 8 + 23, SWP_NOREPOSITION);
			SetTimer(T_TIMER, tick_ms, NULL);
			break;
		case 2:
			m_CGraph.SetWindowPos(NULL, 0, 0, 800 + 2 * 8, 800 + 2 * 8 + 23, SWP_NOREPOSITION);
			SetTimer(T_TIMER, tick_ms, NULL);
			break;
		case 3:
			m_CGraph.SetWindowPos(NULL, 0, 0, 800 + 2 * 8, 800 + 2 * 8 + 23, SWP_NOREPOSITION);
			Renewal_Z();
			break;
		case 4:
			m_CGraph.SetWindowPos(NULL, 0, 0, 800 + 2 * 8, 800 + 2 * 8 + 23, SWP_NOREPOSITION);
			break;
		case 5:
			m_CGraph.SetWindowPos(NULL, 0, 0, 800 + 2 * 8, 800 + 2 * 8 + 23, SWP_NOREPOSITION);
			SetTimer(T_TIMER, tick_ms, NULL);
			break;
		}		
	}

	UpdateData(FALSE);	
}


void CserialDlg::OnBnClickedBtnGrphDataClr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	for (int i = 0; i < m_iGrph_i+1; i++)
	{
		for (int j = 0; j < m_iItem_i+1; j++)
		{
			m_CGraph.m_LineChartCtrl.m_aData[i * (m_iItem_i + 1) + j].Clear();
		}
	}
	
	//nXDataTotal = 0;
	//m_iXth = 0;
	m_CGraph.m_LineChartCtrl.nXDataTotal = 0;
	m_CGraph.m_LineChartCtrl.m_iXth = 0;
	nNaturalE = 0;
	dblExpX_x = 0;
}


void CserialDlg::OnCbnSelchangeComboSincos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnCbnSelchangeComboSincos2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnCbnSelchangeComboOperator()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeFormA()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeFormA2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEditFormB()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEditFormC()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEditFormD()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEditFormB2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEditFormC2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEditFormD2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEdit7()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEdit10()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	dblExpX = exp(dblExpX_x);
	dblExpX2 = pow(1.0 + (dblExpX_x/ (double)nExpParam_m), nExpParam_m);
	CString str;
	str.Format("%f", dblExpX);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(str);
	str.Format("%f", dblExpX2);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(str);

//	UpdateData(FALSE);
}


void CserialDlg::OnEnChangeEdit11()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData(TRUE);

	////CString str;
	////GetDlgItem(IDC_EDIT11)->GetWindowTextA(str);
	////if ((str.GetLength() == 1) && (str.GetAt(0) == '-'))
	////{
	////	return;
	////}
	////if ((str.GetAt(str.GetLength()-1) == '.'))
	////{
	////	return;
	////}
	////UpdateData(TRUE);
	//////dblZRe[0] = _wtof(strZRe[0]);
	////dblZRe[0] = atof(strZRe[0]);
	////Renewal_Z();
}


void CserialDlg::OnEnChangeEdit12()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	////CString str;
	////GetDlgItem(IDC_EDIT12)->GetWindowTextA(str);
	////if ((str.GetLength() == 1) && (str.GetAt(0) == '-'))
	////{
	////	return;
	////}
	////if ((str.GetAt(str.GetLength() - 1) == '.'))
	////{
	////	return;
	////}
	////
	////UpdateData(TRUE);
	////Renewal_Z();
}


void CserialDlg::OnEnChangeEdit13()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString str;
	//GetDlgItem(IDC_EDIT13)->GetWindowTextA(str);
	//if ((str.GetLength() == 1) && (str.GetAt(0) == '-'))
	//{
	//	return;
	//}
	//if ((str.GetAt(str.GetLength() - 1) == '.'))
	//{
	//	return;
	//}
	//
	//UpdateData(TRUE);
	//Renewal_Z();
}


void CserialDlg::OnEnChangeEdit14()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString str;
	//GetDlgItem(IDC_EDIT14)->GetWindowTextA(str);
	//if ((str.GetLength() == 1) && (str.GetAt(0) == '-'))
	//{
	//	return;
	//}
	//if ((str.GetAt(str.GetLength() - 1) == '.'))
	//{
	//	return;
	//}
	//
	//UpdateData(TRUE);
	//Renewal_Z();
}


void CserialDlg::OnEnChangeEdit15()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData(TRUE);
	//m_CGraph.m_LineChartCtrl.dblZRe[2] = dblZRe[2];
	//m_CGraph.m_LineChartCtrl.RedrawWindow();
}


void CserialDlg::OnEnChangeEdit16()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//UpdateData(TRUE);
	//m_CGraph.m_LineChartCtrl.dblZIm[2] = dblZIm[2];
	//m_CGraph.m_LineChartCtrl.RedrawWindow();
}


#if (0)
void CserialDlg::OnCbnSelchangeComboOperator2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtnGrphDataClr();
	UpdateData(TRUE);

	if (cbxOper2 == 2)
	{
		m_iItem_i = 5;
		m_CGraph.m_LineChartCtrl.m_iItemN = 6;
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	}
	else
	{
		dblZRe[3] = 1;
		dblZIm[3] = 0;
		dblZRe[4] = 0;
		dblZIm[4] = 0;
		dblZRe[5] = 0;
		dblZIm[5] = 0;
		m_iItem_i = 2;
		m_CGraph.m_LineChartCtrl.m_iItemN = 3;
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	Renewal_Z();
}
#endif

void CserialDlg::OnEnChangeEdit5()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CserialDlg::Renewal_Z()
{
	//UpdateData(TRUE);
	switch (iZOper)
	{
		case 0:	// +
			dblZRe[2] = dblZRe[0] + dblZRe[1];
			dblZIm[2] = dblZIm[0] + dblZIm[1];
			break;
		case 1:	// -
			dblZRe[2] = dblZRe[0] - dblZRe[1];
			dblZIm[2] = dblZIm[0] - dblZIm[1];
			break;
		case 2:	// *
			dblZRe[2] = (dblZRe[0] * dblZRe[1]) - (dblZIm[0] * dblZIm[1]);
			dblZIm[2] = (dblZRe[0] * dblZIm[1]) + (dblZRe[1] * dblZIm[0]);
			break;
		case 3:	// /
			break;
	}
	for (int i = 0; i < 6; i++)
	{
		dblZDeg[i] = atan2(dblZIm[i], dblZRe[i]) * 180 / PI;	// 각도 계산
		if (dblZDeg[i] < 0)
		{
			dblZDeg[i] = 180 + (180 + dblZDeg[i]);
		}
		dblZMag[i] = sqrt(dblZRe[i] * dblZRe[i] + dblZIm[i] * dblZIm[i]);
		strZRe[i].Format("%f",dblZRe[i]);
		strZIm[i].Format("%f",dblZIm[i]);
		strZDeg[i].Format("%f",dblZDeg[i]);
		strZMag[i].Format("%f", dblZMag[i]);
	}
	strZRotStep[0].Format("%f", dblZRotStep[0]);
	strZRotStep[1].Format("%f", dblZRotStep[1]);

	switch (iZOper)
	{
		case 0:
		case 1:
			dblZMax = sqrt(dblZRe[0] * dblZRe[0] + dblZIm[0] * dblZIm[0]) + sqrt(dblZRe[1] * dblZRe[1] + dblZIm[1] * dblZIm[1]);
			break;
		case 2:
		case 3:
			dblZMax = sqrt(dblZRe[0] * dblZRe[0] + dblZIm[0] * dblZIm[0]) * sqrt(dblZRe[1] * dblZRe[1] + dblZIm[1] * dblZIm[1]);
			break;
	}		
	if ((dblZMax - (int)dblZMax) != 0)
	{
		dblZMax = (int)dblZMax + 1;
	}
	m_CGraph.m_LineChartCtrl.dblZMax = dblZMax;

	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		//for (int i=0; i<m_iBaudIndx+1; i++)
		for (int i = 0; m_iItem_i + 1; i++)
		{
			m_CGraph.m_LineChartCtrl.m_aData[i].Add(dblZRe[i], dblZIm[i], 0);
		}
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}
}


void CserialDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		if (!(vZRot & 0x01))
		{
			CButton * pButton = (CButton*)GetDlgItem(IDC_BUTTON1);
			if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
				pButton->SetWindowText(_T("▣"));
			}
			vZRot |= 0x01;
			if (!(vZRot & 0x02))
			{
				SetTimer(T_ZROTATE, tick_ms, NULL);
			}	
		}
		else
		{
			UpdateData(FALSE);
			CButton * pButton = (CButton*)GetDlgItem(IDC_BUTTON1);
			if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
				pButton->SetWindowText(_T("↙"));
			}
			vZRot &= ~0x01;
			if (!(vZRot & 0x02))
			{
				KillTimer(T_ZROTATE);
			}
		}
	}
	
}


void CserialDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		if (!(vZRot & 0x02))
		{
			CButton * pButton = (CButton*)GetDlgItem(IDC_BUTTON2);
			if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
				pButton->SetWindowText(_T("▣"));
			}
			vZRot |= 0x02;
			if (!(vZRot & 0x01))
			{
				SetTimer(T_ZROTATE, tick_ms, NULL);
			}
		}
		else
		{
			UpdateData(FALSE);
			CButton * pButton = (CButton*)GetDlgItem(IDC_BUTTON2);
			if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
				pButton->SetWindowText(_T("↙"));
			}
			vZRot &= ~0x02;
			if (!(vZRot & 0x01))
			{
				KillTimer(T_ZROTATE);
			}
		}
	}
}

#if (0)
void CserialDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static bool bTRUE = TRUE;

	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		if (bTRUE == TRUE)
		{
			//CButton * pButton = (CButton*)GetDlgItem(IDC_BUTTON3);
			//if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
			//	pButton->SetWindowText(_T("▣"));
			//}
			OnBnClickedBtnGrphDataClr();
			nEuler_m = 0;
			dblZMax = 0;
			m_CGraph.m_LineChartCtrl.dblZMax = dblZMax;
			SetTimer(T_EULER_1, 1000, NULL);
			bTRUE = FALSE;
		}
		else
		{
			//CButton * pButton = (CButton*)GetDlgItem(IDC_BUTTON3);
			//if (pButton && ::IsWindow(pButton->GetSafeHwnd())){
			//	pButton->SetWindowText(_T("▶"));
			//}
			KillTimer(T_EULER_1);
			bTRUE = TRUE;
		}
	}
}
#endif

void CserialDlg::OnEnChangeEditMaxN()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_CGraph.m_LineChartCtrl.m_iViewMaxN = nGrphDMax;
}


void CserialDlg::OnEnChangeEdit22()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	dblEulerZRe[0] = dblEulerZRe[1] = 1.0;
	dblEulerZIm[0] = dblEulerZIm[1] = (dblEuler_x*PI) / nEuler_m;
	double ZRe, ZIm;
	for (int i = 0; i < nEuler_m - 1; i++)
	{
		ZRe = (dblEulerZRe[0]*dblEulerZRe[1]) - (dblEulerZIm[0]*dblEulerZIm[1]);
		ZIm = (dblEulerZRe[0]*dblEulerZIm[1]) + (dblEulerZRe[1]*dblEulerZIm[0]);
		dblEulerZRe[1] = ZRe;
		dblEulerZIm[1] = ZIm;
	}
	strEulerZRe[0].Format("%f", dblEulerZRe[0]);
	strEulerZIm[0].Format("%f", dblEulerZIm[0]);
	strEulerZRe[1].Format("%f", dblEulerZRe[1]);
	strEulerZIm[1].Format("%f", dblEulerZIm[1]);

	UpdateData(FALSE);
}


void CserialDlg::OnEnChangeEdit25()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData(TRUE);
	//
	//double ZRe, ZIm;
	//double ZTmpRe, ZTmpIm;
	//ZRe = dblEulerZRe = 1.0;
	//ZIm = dblEulerZIm = (dblEuler_x*PI) / nEuler_m;
	//
	//for (int i = 0; i < nEuler_m - 1; i++)
	//{
	//	ZTmpRe = (dblEulerZRe*ZRe) - (dblEulerZIm*ZIm);
	//	ZTmpIm = (dblEulerZRe*ZIm) + (ZRe*dblEulerZIm);
	//	dblEulerZRe = ZTmpRe;
	//	dblEulerZIm = ZTmpIm;
	//}
	//
	//UpdateData(FALSE);
}


//afx_msg LRESULT CserialDlg::OnControlRenewal(WPARAM wParam, LPARAM lParam)
//{
//	//UpdateData(FALSE);
//	GetDlgItem(IDC_EDIT11)->SetWindowTextA(strZRe[0]);
//	GetDlgItem(IDC_EDIT12)->SetWindowTextA(strZIm[0]);
//	GetDlgItem(IDC_EDIT13)->SetWindowTextA(strZRe[1]);
//	GetDlgItem(IDC_EDIT14)->SetWindowTextA(strZIm[1]);
//	GetDlgItem(IDC_EDIT15)->SetWindowTextA(strZRe[2]);
//	GetDlgItem(IDC_EDIT16)->SetWindowTextA(strZIm[2]);
//	GetDlgItem(IDC_EDIT17)->SetWindowTextA(strZDeg[0]);
//	GetDlgItem(IDC_EDIT18)->SetWindowTextA(strZDeg[1]);
//	GetDlgItem(IDC_EDIT19)->SetWindowTextA(strZDeg[2]);
//	GetDlgItem(IDC_EDIT20)->SetWindowTextA(strZRotStep[0]);
//	GetDlgItem(IDC_EDIT21)->SetWindowTextA(strZRotStep[1]);
//	return 0;
//}


void CserialDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		if (!(vZRot & 0x03))
		{
			OnBnClickedBtnGrphDataClr();
			iZMulStep++;
			m_CGraph.m_LineChartCtrl.iZMulStep = iZMulStep;
			SetTimer(T_ZMULTIPLY, 10, NULL);				
		}
	}
}


int CserialDlg::taskSinX(void)
{
	double xpos[3];
	double item[3];
	double a, b, c, d, x;
	
	m_CGraph.m_LineChartCtrl.m_iXth++;
	
	for (int i = 0; i < 2; i++)
	{
		a = dblFxA[i];
		b = dblFxB[i];
		d = dblFxD[i];
		x = (double)m_CGraph.m_LineChartCtrl.m_iXth * 2.0 * PI / (double)n2PiCycle;
		c = (cbxSinX[i] * 0.5 + dblFxC[i]) * PI;
		item[i] = a * sin(b*x + c) + d;
		xpos[i] = a * sin(b*x + c + 0.5*PI) + d;
		m_CGraph.m_LineChartCtrl.m_aData[i].Add(item[i], xpos[i], 0);
	}
	switch (cbxOper)
	{
		case 0:
			item[2] = item[0] + item[1]; 
			break;
		case 1:
			item[2] = item[0] - item[1]; 
			break;
		case 2:
			item[2] = item[0] * item[1]; 
			break;
		case 3:
			if (item[1] != 0)	item[2] = item[0] / item[1];
			else				item[2] = 0;
			if (item[2] > 10)	item[2] = 10;
			if (item[2] < -10)	item[2] = -10;
			break;
	}
	xpos[2] = xpos[0] + xpos[1];
	m_CGraph.m_LineChartCtrl.m_aData[2].Add(item[2], xpos[2], 0);
	m_CGraph.m_LineChartCtrl.nXDataTotal++;
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}

	return 0;
}


int CserialDlg::taskNatural_e(void)
{
	if (nNaturalE < nGrphDMax)
	{
		nNaturalE++;
		dblNatualE = pow((1 + 1 / (double)nNaturalE), (double)nNaturalE);
		m_CGraph.m_LineChartCtrl.m_aData[0].Add(dblNatualE, 0, 0);
		m_CGraph.m_LineChartCtrl.m_aData[1].Add(dblNatualE, 0, 0);
		m_CGraph.m_LineChartCtrl.m_aData[2].Add(dblNatualE, 0, 0);
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
	}
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}

	return 0;
}


int CserialDlg::taskExpX(void)
{
	if (dblExpX_x < 0.01 * nGrphDMax)
	{
		dblExpX_x += 0.01;
		dblExpX = exp(dblExpX_x);
		dblExpX2 = pow(1.0 + (dblExpX_x / (double)nExpParam_m), (double)nExpParam_m);
		double diff = dblExpX - dblExpX2;
		m_CGraph.m_LineChartCtrl.m_aData[0].Add(dblExpX, diff, 0);
		m_CGraph.m_LineChartCtrl.m_aData[1].Add(dblExpX2, diff, 0);
		m_CGraph.m_LineChartCtrl.m_aData[2].Add(0, 0, 0);
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
	}
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}

	return 0;
}

int CserialDlg::taskZRotate(void)
{
	double dblA;
		
	switch (vZRot)
	{
		case 1:
			dblA = sqrt((dblZRe[0] * dblZRe[0]) + (dblZIm[0] * dblZIm[0]));
			dblZDeg[0] += dblZRotStep[0];
			if (dblZDeg[0] > 360.0)
			{
				dblZDeg[0] = -360.0;
			}
			dblZRe[0] = dblA*cos(2 * PI * dblZDeg[0] / 360.0);
			dblZIm[0] = dblA*sin(2 * PI * dblZDeg[0] / 360.0);
			break;
		case 2:
			dblA = sqrt((dblZRe[1] * dblZRe[1]) + (dblZIm[1] * dblZIm[1]));
			dblZDeg[1] += dblZRotStep[1];
			if (dblZDeg[1] > 360.0)
			{
				dblZDeg[1] = -360.0;
			}
			dblZRe[1] = dblA*cos(2 * PI * dblZDeg[1] / 360.0);
			dblZIm[1] = dblA*sin(2 * PI * dblZDeg[1] / 360.0);
			break;
		case 3:
			for (int i = 0; i < 2; i++)
			{
				dblA = sqrt((dblZRe[i] * dblZRe[i]) + (dblZIm[i] * dblZIm[i]));
				dblZDeg[i] += dblZRotStep[i];
				if (dblZDeg[i] > 360.0)
				{
					dblZDeg[i] = -360.0;
				}
				dblZRe[i] = dblA*cos(2.0 * PI * dblZDeg[i] / 360.0);
				dblZIm[i] = dblA*sin(2.0 * PI * dblZDeg[i] / 360.0);
			}
			break;
	}
	dblZRe[3] = 0;
	dblZIm[3] = 0;
	Renewal_Z();

	return 0;
}


int CserialDlg::taskZMultiply(void)
{
	bool bEnd;

	switch (iZMulStep)
	{
		case 1:
		case 5:	// 삼각형 외곽
			dblZRe[3] = 1;				
			dblZIm[3] = 0;
			dblZRe[4] = dblZRe[0];
			dblZIm[4] = dblZIm[0];
			dblZRe[5] = dblZRe[3];
			dblZIm[5] = dblZIm[3];			
			Renewal_Z();
			KillTimer(T_ZMULTIPLY);
			break;
		case 2:	// 삼각형 내부
		case 6:
			Renewal_Z();
			dblDegInc = 0;
			KillTimer(T_ZMULTIPLY);
			break;
		case 3:
		case 8:	// Rotation
			bEnd = FALSE;
			if (dblDegInc < dblZDeg[1])
			{
				dblDegInc  += 0.5;
				dblZDeg[4] += 0.5;
				dblZDeg[5] += 0.5;
			}
			else
			{
				dblZDeg[4] += dblZDeg[1]-dblDegInc;
				dblZDeg[5] += dblZDeg[1]-dblDegInc;
				bEnd = TRUE;
			}
			dblZRe[4] = dblZMag[4] * cos(2 * PI * (dblZDeg[4]) / 360.0);
			dblZIm[4] = dblZMag[4] * sin(2 * PI * (dblZDeg[4]) / 360.0);
			dblZRe[5] = dblZMag[5] * cos(2 * PI * (dblZDeg[5]) / 360.0);
			dblZIm[5] = dblZMag[5] * sin(2 * PI * (dblZDeg[5]) / 360.0);
			Renewal_Z();

			if (bEnd == TRUE)
			{
				OnBnClickedBtnGrphDataClr();
				KillTimer(T_ZMULTIPLY);
				if (iZMulStep == 8)
				{
					iZMulStep = 0;
				}
			}
			break;
		case 4:
		case 7:	// Enlarge
			bEnd = FALSE;
			if ((dblZMag[5] > dblZMag[1] * 1.01) || (dblZMag[5] < dblZMag[1] * 0.99))
			{
				if (dblZMag[5] < dblZMag[1]) {
					dblZMag[4] *= 1.01;
					dblZMag[5] *= 1.01;
				} else {
					dblZMag[4] *= 0.99;
					dblZMag[5] *= 0.99;
				}
			}
			else
			{		
				dblZMag[4] = dblZMag[4] * dblZMag[1] / dblZMag[5];
				dblZMag[5] = dblZMag[5] * dblZMag[1] / dblZMag[5];
				bEnd = TRUE;
			}
			dblZRe[4] = dblZMag[4] * cos(2 * PI * dblZDeg[4] / 360.0);
			dblZIm[4] = dblZMag[4] * sin(2 * PI * dblZDeg[4] / 360.0);
			dblZRe[5] = dblZMag[5] * cos(2 * PI * dblZDeg[5] / 360.0);
			dblZIm[5] = dblZMag[5] * sin(2 * PI * dblZDeg[5] / 360.0);
			Renewal_Z();

			if (bEnd == TRUE)
			{
				KillTimer(T_ZMULTIPLY);
			}
			break;
		default:
			break;
	}
	return 0;
}

int CserialDlg::taskCoin(void)
{
	int event;
	double perc[4];

	if (!m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		return 0;
	}

	m_CGraph.m_LineChartCtrl.m_iXth++;
	nDiceTry++;

	event = (rand() % 6);
	nDiceIndx[event]++;
	if (event % 2)	nHeadTail[0]++;
	else			nHeadTail[1]++;

	for (int i = 0; i < 2; i++)
	{
		dblCPerc[i] = (double)nHeadTail[i] / (double)nDiceTry;
	}
	for (int i = 0; i < 6; i++)
	{
		dblDPerc[i] = (double)nDiceIndx[i] / (double)nDiceTry;
	}

	perc[0] = abs(0.5 - dblCPerc[0]) * 100;			// 100배 
	perc[1] = abs(0.5 - dblCPerc[0]) * 10000;		// 10000배 
	perc[2] = abs(0.5 - dblCPerc[0]) * 1000000;		// 1000000배 
	perc[3] = abs(0.5 - dblCPerc[0]) * 100000000;	// 100000000배 

	m_CGraph.m_LineChartCtrl.m_aData[0].Clear();

	for (int i = 0; i < 6; i++)
	{
		m_CGraph.m_LineChartCtrl.m_aData[0].Add(dblDPerc[i], 0, 0);
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
	}
	for (int i = 0; i < 2; i++)
	{
		m_CGraph.m_LineChartCtrl.m_aData[0].Add(dblCPerc[i], 0, 0);
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
	}
	for (int i = 0; i < 4; i++)
	{
		m_CGraph.m_LineChartCtrl.m_aData[0].Add(perc[i], 0, 0);
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
	}
	m_CGraph.m_LineChartCtrl.RedrawWindow();

	return 0;
}


void CserialDlg::OnEnChangeEditSampleN()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
}


void CserialDlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
}


void CserialDlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtnGrphDataClr();
	for (int i = 0; i < 3; i++)
	{
		dblZRe[3+i] = 0;
		dblZIm[3+i] = 0;
	}
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);	
	iZMulStep = 0;
	m_CGraph.m_LineChartCtrl.iZMulStep = iZMulStep;

	iZOper = 0;
	Renewal_Z();
	UpdateData(FALSE);
}


void CserialDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtnGrphDataClr();
	for (int i = 0; i < 3; i++)
	{
		dblZRe[3 + i] = 0;
		dblZIm[3 + i] = 0;
	}
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	iZMulStep = 0;
	m_CGraph.m_LineChartCtrl.iZMulStep = iZMulStep;

	iZOper = 1;
	Renewal_Z();
	UpdateData(FALSE);
}

void CserialDlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtnGrphDataClr();
	for (int i = 0; i < 3; i++)
	{
		dblZRe[3 + i] = 0;
		dblZIm[3 + i] = 0;
	}
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	iZMulStep = 0;
	m_CGraph.m_LineChartCtrl.iZMulStep = iZMulStep;

	iZOper = 2;
	Renewal_Z();
	UpdateData(FALSE);
}


void CserialDlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtnGrphDataClr();
	for (int i = 0; i < 3; i++)
	{
		dblZRe[3 + i] = 0;
		dblZIm[3 + i] = 0;
	}
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	iZMulStep = 0;
	m_CGraph.m_LineChartCtrl.iZMulStep = iZMulStep;

	iZOper = 3;
	Renewal_Z();
	UpdateData(FALSE);
}


int CserialDlg::taskEulerForm()
{
	if (nEuler_m > nGrphDMax)
	{
		return 0;
	}
	nEuler_m++;
	if (bEulerMFix == FALSE)
	{
		szEuler_m = nEuler_m;
	}
	dblEulerZRe[0] = dblEulerZRe[1] = 1.0;
	dblEulerZIm[0] = dblEulerZIm[1] = (dblEuler_x*PI) / nEuler_m;
	double ZRe, ZIm;
	for (int i = 0; i < nEuler_m - 1; i++)
	{
		ZRe = (dblEulerZRe[0]*dblEulerZRe[1]) - (dblEulerZIm[0]*dblEulerZIm[1]);
		ZIm = (dblEulerZRe[0]*dblEulerZIm[1]) + (dblEulerZRe[1]*dblEulerZIm[0]);
		dblEulerZRe[1] = ZRe;
		dblEulerZIm[1] = ZIm;
	}
	strEulerZRe[0].Format("%f", dblEulerZRe[0]);
	strEulerZIm[0].Format("%f", dblEulerZIm[0]);
	strEulerZRe[1].Format("%f", dblEulerZRe[1]);
	strEulerZIm[1].Format("%f", dblEulerZIm[1]);

	double dlbtmp = sqrt(dblEulerZRe[1] * dblEulerZRe[1] + dblEulerZIm[1] * dblEulerZIm[1]);
	if (dlbtmp > dblZMax)
	{
		dblZMax = dlbtmp;
	}

	if ((dblZMax - (int)dblZMax) != 0)
	{
		dblZMax = (int)dblZMax + 1;
	}
	m_CGraph.m_LineChartCtrl.dblZMax = dblZMax;

	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		m_CGraph.m_LineChartCtrl.m_aData[0].Add(dblEulerZRe[1], dblEulerZIm[1], 0);
		m_CGraph.m_LineChartCtrl.nXDataTotal++;
		m_CGraph.m_LineChartCtrl.RedrawWindow();
	}

	UpdateData(FALSE);
	return 0;
}


void CserialDlg::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		if (bEulerBtn == FALSE)
		{
			OnBnClickedBtnGrphDataClr();
			nEuler_m = 0;
			dblZMax = 0;
			m_CGraph.m_LineChartCtrl.dblZMax = dblZMax;
			m_CGraph.m_LineChartCtrl.m_aData[0].Add(0, 0, 0);
			m_CGraph.m_LineChartCtrl.nXDataTotal++;
			m_CGraph.m_LineChartCtrl.RedrawWindow();
			tick_ms = 100;
			if (bEulerMFix == TRUE)
			{	
				SetTimer(T_EULER_2, tick_ms, NULL);
			}
			else
			{
				SetTimer(T_EULER_1, tick_ms, NULL);
			}
			bEulerBtn = TRUE;
		}
		else
		{
			KillTimer(T_EULER_1);
			KillTimer(T_EULER_2);
			bEulerBtn = FALSE;
		}
	}
}


int CserialDlg::taskEulerForm2(void)
{
	static int step = 0;

	if (!m_CGraph.m_LineChartCtrl.m_hWnd)
	{
		return 1;
	}

	double ZRe, ZIm;
		
	switch(step)
	{
		case 0:
			OnBnClickedBtnGrphDataClr();
			dblZMax = 1;
			dblEulerZRe[0] = dblEulerZRe[1] = 0;
			dblEulerZIm[0] = dblEulerZIm[1] = 0;
			m_CGraph.m_LineChartCtrl.nXDataTotal = 0;
			nEuler_m = 0;
			step++;
			break;
		case 1:
			dblEulerZRe[0] = dblEulerZRe[1] = 1.0;
			dblEulerZIm[0] = dblEulerZIm[1] = (dblEuler_x*PI) / szEuler_m;
			step++;
			break;
		case 2:		
			ZRe = (dblEulerZRe[0]*dblEulerZRe[1]) - (dblEulerZIm[0]*dblEulerZIm[1]);
			ZIm = (dblEulerZRe[0]*dblEulerZIm[1]) + (dblEulerZRe[1]*dblEulerZIm[0]);
			dblEulerZRe[1] = ZRe;
			dblEulerZIm[1] = ZIm;
			strEulerZRe[0].Format("%f", dblEulerZRe[0]);
			strEulerZIm[0].Format("%f", dblEulerZIm[0]);
			strEulerZRe[1].Format("%f", dblEulerZRe[1]);
			strEulerZIm[1].Format("%f", dblEulerZIm[1]);		
			break;
		default :
			step = 0;
			break;
	}
	
	double dlbtmp = sqrt(dblEulerZRe[1] * dblEulerZRe[1] + dblEulerZIm[1] * dblEulerZIm[1]);
	if (dlbtmp > dblZMax)
	{
		dblZMax = dlbtmp;
	}
	if ((dblZMax - (int)dblZMax) != 0)
	{
		dblZMax = (int)dblZMax + 1;
	}
	m_CGraph.m_LineChartCtrl.dblZMax = dblZMax;
	
	m_CGraph.m_LineChartCtrl.m_aData[0].Add(dblEulerZRe[1], dblEulerZIm[1], 0);
	m_CGraph.m_LineChartCtrl.nXDataTotal++;
	m_CGraph.m_LineChartCtrl.RedrawWindow();

	if (nEuler_m >= szEuler_m)
	{
		KillTimer(T_EULER_2);
		bEulerBtn = FALSE;
		step = 0;
	}
	else
	{
		nEuler_m++;		
	}
	UpdateData(FALSE);

	return 0;
}


void CserialDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//@TN
	//	*pResult = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	*pResult = 0;
}


void CserialDlg::OnBnClickedCheck10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	dblEulerZIm[0] = (dblEuler_x * PI)/szEuler_m;
	strEulerZIm[0].Format("%f", dblEulerZIm[0]);

	UpdateData(FALSE);
}




void CserialDlg::OnEnChangeEdit31()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

//#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	dblEulerZIm[0] = (dblEuler_x * PI)/szEuler_m;
	strEulerZIm[0].Format("%f", dblEulerZIm[0]);

	UpdateData(FALSE);
}


void CserialDlg::OnBnClickedButton11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	dblZRe[0] = dblZRe[2];
	dblZIm[0] = dblZIm[2];
	strZRe[0].Format("%f", dblZRe[0]);
	strZIm[0].Format("%f", dblZIm[0]);
	UpdateData(FALSE);
}


void CserialDlg::OnBnClickedButton10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dblZRe[0] = atof(strEulerZRe[0]);
	dblZIm[0] = atof(strEulerZIm[0]);
	dblZRe[1] = atof(strEulerZRe[0]);
	dblZIm[1] = atof(strEulerZIm[0]);

	strZRe[0].Format("%f", dblZRe[0]);
	strZIm[0].Format("%f", dblZIm[0]);
	strZRe[1].Format("%f", dblZRe[1]);
	strZIm[1].Format("%f", dblZIm[1]);
	UpdateData(FALSE);
}
