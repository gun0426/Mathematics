

#define RX_EVENT			(WM_USER + 1)
#define BUFF_SIZE	4096		//4192


//	��� Ŭ����	CCommThread 

/***********************************************************************
* class CQueue
*
* * Read  - Port�� Open �� �Ŀ� ����Ÿ�� ������ RX_EVENT �޽����� 
*			Main Window �� ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿��Ͽ� �Լ��� 
*			�����ϰ� m_ReadData String�� ����� ����Ÿ�� �̿��Ͽ� �д´�.
 
* * Write - WriteComm(buff, 30) �� ���� ���ۿ� �� ũ�⸦ �ָ� �ȴ�. 
*
***********************************************************************/
class CQueue
{
public:
	BYTE buff[BUFF_SIZE];	// ť ���� ������� 4192�� ���� 
	int m_iHead, m_iTail;	// ť ���ۿ� �����͸� �ְ� �� �ö� ����� ���� 
	CQueue();
	void Clear();			// ���۸� �ʱ�ȭ ��Ŵ
	int GetSize();			// ���� ���ۿ� ����ִ� �������� size�� ����
	BOOL PutByte(BYTE b);	// ť���ۿ� 1����Ʈ�� ����
	BOOL GetByte(BYTE *pb);	// ť���ۿ��� 1����Ʈ�� �� ��
};

class	CSerialThread
{
public:
	CSerialThread();
	~CSerialThread();
	//--------- ȯ�� ���� -----------------------------------------//
	BOOL        check;
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	BOOL		m_bPortOpen;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// RX_EVENT�� �Բ� ������ �μ�.
	CQueue      m_QueueRead;			// ť����
	//--------- �ܺ� ��� �Լ� ------------------------------------//
	BOOL	OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//��Ʈ ���� 
	void	ClosePort();				// ��Ʈ �ݱ�
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//��Ʈ�� ������ ����

	//--------- ���� ��� �Լ� ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//��Ʈ���� ������ �о����
};

// Thread�� ����� �Լ� 
DWORD	ThreadWatchComm(CSerialThread* pComm);

