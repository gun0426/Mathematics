#include "stdafx.h"
#include "serial_thread.h"

extern HWND hCommWnd;

/***********************************************************************
* CQueue::Clear
*
*
***********************************************************************/
void CQueue::Clear()
{
	m_iHead = m_iTail =0;
	memset(buff,0,BUFF_SIZE);
}
/***********************************************************************
* CQueue::Clear
*
* * ������
*
***********************************************************************/
CQueue::CQueue()
{
	Clear();
}
/***********************************************************************
* CQueue::GetSize
*
*
***********************************************************************/
int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}
/***********************************************************************
* CQueue::PutByte
*
*
***********************************************************************/
BOOL CQueue::PutByte(BYTE b)
{
	if(GetSize() == (BUFF_SIZE-1)) return FALSE;
	buff[m_iHead++] =b;
	m_iHead %= BUFF_SIZE;
	return TRUE;
}
/***********************************************************************
* CQueue::GetByte
*
*
***********************************************************************/
BOOL CQueue::GetByte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= BUFF_SIZE;
	return TRUE;
}
/***********************************************************************
* CSerialThread::CSerialThread
*
* * ������ 
*
***********************************************************************/
CSerialThread::CSerialThread()
{
	//--> �ʱ�� �翬��..��Ʈ�� ������ ���� ���¿��߰���?
	m_bPortOpen = FALSE;
}
/***********************************************************************
* CSerialThread::~CSerialThread
*
* * �Ҹ���  
*
***********************************************************************/
CSerialThread::~CSerialThread()
{

}
/***********************************************************************
* CSerialThread::OpenPort
*
* * Port sPortName �� dwBaud �ӵ��� ����. 
* * ThreadWatchComm �Լ����� ��Ʈ�� �����ΰ��� ������ �� MainWnd �� 
*	�˸��� ���� RX_EVENT �޽����� ���� �� ���� ���� wPortID ���� 
*	���� �޴´�. 
*
***********************************************************************/
BOOL CSerialThread::OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity )
{

	// Local ����.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;
		
	// overlapped structure ���� �ʱ�ȭ.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;

	//--> Read �̺�Ʈ ������ ����..
	if ( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 	
	{
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;

	//--> Write �̺�Ʈ ������ ����..
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}
		
	//--> ��Ʈ�� ����..
	m_sPortName = strPortName;
	
	//--> ��������...RS 232 ��Ʈ ����..
	m_hComm = CreateFile( m_sPortName, 
						GENERIC_READ | GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						NULL);

	//--> ��Ʈ ���⿡ �����ϸ�..
	if (m_hComm == (HANDLE) -1)
	{
		AfxMessageBox(_T("fail port-open !"));
		return FALSE;
	}
	
	//===== ��Ʈ ���� ����. =====

	// EV_RXCHAR event ����...�����Ͱ� ������.. ���� �̺�Ʈ�� �߻��ϰԲ�..
	SetCommMask( m_hComm, EV_RXCHAR);	

	// InQueue, OutQueue ũ�� ����.
	SetupComm( m_hComm, BUFF_SIZE, BUFF_SIZE);	

	// ��Ʈ ����.
	PurgeComm( m_hComm,					
			   PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout ����.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts( m_hComm, &timeouts);

	// dcb ����.... ��Ʈ�� ��������..��� ����ϴ� DCB ����ü�� ����..
	dcb.DCBlength = sizeof(DCB);

	//--> ���� ������ �� �߿���..
	GetCommState( m_hComm, &dcb);	
	
	//--> ���巹��Ʈ�� �ٲٰ�..
	dcb.BaudRate = dwBaud;

	//--> Data 8 Bit
	dcb.ByteSize = byData;

	//--> Noparity
	dcb.Parity = NOPARITY;

	//--> 1 Stop Bit
	dcb.StopBits = byStop;

	//--> ��Ʈ�� ��..����������.. �����غ���..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}

	// ��Ʈ ���� ������ ����.
	m_bPortOpen = TRUE;

	//--> ��Ʈ ���� ������ ����.
	m_hThreadWatchComm = CreateThread( NULL, 0, 
									   (LPTHREAD_START_ROUTINE)ThreadWatchComm, 
									   this, 0, &dwThreadID);

	//--> ������ ������ �����ϸ�..
	if (! m_hThreadWatchComm)
	{
		//--> ���� ��Ʈ�� �ݰ�..
		ClosePort();
		return FALSE;
	}
	check = FALSE;

	return TRUE;
}
/***********************************************************************
* CSerialThread::ClosePort
*
*
***********************************************************************/
void CSerialThread::ClosePort()
{
	//--> ������� �ʾ���.
	m_bPortOpen = FALSE;
	
	//--> ����ũ ����..
	SetCommMask( m_hComm, 0);
	
	//--> ��Ʈ ����.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	//--> �ڵ� �ݱ�
	CloseHandle(m_hComm);		
}
/***********************************************************************
* CSerialThread::WriteComm
*
* * Port �� pBuff �� ������ nToWrite ��ŭ ����. 
* * ������ ������ Byte ���� �����Ѵ�. 
*
***********************************************************************/
DWORD CSerialThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

	// if Port is Closed, Then return
	if( !m_bPortOpen ){
		return 0;
	}

	//--> ���ڷ� ���� ������ ������ nToWrite ��ŭ ����.. �� ������.,dwWrite �� �ѱ�.
	if( !WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite)){
		//--> ���� ������ ���ڰ� ������ ���..
		if (GetLastError() == ERROR_IO_PENDING){
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeouts�� ������ �ð���ŭ ��ٷ��ش�.
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE)){
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE){
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}else{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}

	//--> ���� ��Ʈ�� ������ ������ ����..
	return dwWritten;
}
/***********************************************************************
* CSerialThread::ReadComm
*
* * Port �κ��� pBuff �� nToRead ��ŭ �д´�. 
* * ������ ������ Byte ���� �����Ѵ�. 
*
***********************************************************************/
DWORD CSerialThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead,dwError, dwErrorFlags;
	COMSTAT comstat;

	//--- system queue�� ������ byte���� �̸� �д´�.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);

	//--> �ý��� ť���� ���� �Ÿ��� ������..
	dwRead = comstat.cbInQue;
	if(dwRead > 0)
	{
		//--> ���ۿ� �ϴ� �о���̴µ�.. ����..�о���ΰ��� ���ٸ�..
		if( !ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead) )
		{
			//--> ���� �Ÿ��� ��������..
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts�� ������ �ð���ŭ ��ٷ��ش�.
				while (! GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}

	//--> ���� �о���� ������ ����.
	return dwRead;

}
/***********************************************************************
* ThreadWatchComm
*
* * ��Ʈ�� �����ϰ�, ���� ������ ������ m_ReadData�� ������ �ڿ� 
*   MainWnd �� �޼����� ������ Buffer �� ������ �о��� ��ȣ�Ѵ�. 
*
***********************************************************************/
DWORD	ThreadWatchComm(CSerialThread* pComm)
{
   DWORD           dwEvent;
   OVERLAPPED      os;
   BOOL            bOk = TRUE;
   BYTE            buff[2048];	// �б� ����
   DWORD           dwRead;		// ���� ����Ʈ��.
 
   // Event, OS ����.
   memset( &os, 0, sizeof(OVERLAPPED));
   
   //--> �̺�Ʈ ����..
   if( !(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)) ){
		bOk = FALSE;
   }

   //--> �̺�Ʈ ����ũ..
   if( !SetCommMask( pComm->m_hComm, EV_RXCHAR) ){
	   bOk = FALSE;
   }

   //--> �̺�Ʈ��..����ũ ������ ������..
   if( !bOk ){
		//AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		AfxMessageBox(_T("Error while creating ThreadWatchComm, " + pComm->m_sPortName));
		return FALSE;
   }
  
   //��Ʈ�� ����Ǹ� ���� ������ ��
   while (pComm ->m_bPortOpen)
   {
 		dwEvent = 0;
	
        WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);	// �̺�Ʈ�߻� ��� 
	
		//--> �����Ͱ� ���ŵǾ��ٴ� �޼����� �߻��ϸ�..
        if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
            // ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			//--> buff �� �޾Ƴ���..
			Sleep(0);
			do{
				//���� ������ �о� ����
				dwRead = pComm->ReadComm(buff, 2048);  
				if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for( WORD i = 0; i < dwRead; i++ )
					{
						//ť ���ۿ�  ���� ������ �ֱ�
						pComm->m_QueueRead.PutByte(buff[i]); 
					}
				}
				else
				{
					//ť������ ũ�⸦ �ʰ��ϸ� ��� �޽��� ����
					//pComm->m_QueueRead.Clear();
					AfxMessageBox(_T("buff full"));
				}
			}while(dwRead);

			//TestAppDlg�� �����Ͱ� ���Դٴ� �޽����� ����
			::PostMessage(hCommWnd, RX_EVENT, 0, 0);
		}


		/*	
		���� �����͸� ȭ�鿡 ������ �ð��� ���� ����.
		�����͸� �������� ������ cpu�������� 100%�� �Ǿ� 
		ȭ�鿡 �ѷ��ִ� �۾��� �� �ȵǰ�, 
		��������� ť ���ۿ� �����Ͱ� ���̰� �� 
		*/
		Sleep(0);			
	}
   
	CloseHandle( os.hEvent);

	//--> ������ ���ᰡ �ǰ���?
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;
}
#if (0)
/***********************************************************************
* ThreadWatchComm
*
* * ��Ʈ�� �����ϰ�, ���� ������ ������ m_ReadData�� ������ �ڿ� 
*   MainWnd �� �޼����� ������ Buffer �� ������ �о��� ��ȣ�Ѵ�. 
*
***********************************************************************/
DWORD	ThreadWatchComm(CSerialThread* pComm)
{
   DWORD           dwEvent;
   OVERLAPPED      os;
   BOOL            bOk = TRUE;
   BYTE            buff[2048];	// �б� ����
   DWORD           dwRead;		// ���� ����Ʈ��.
 
   // Event, OS ����.
   memset( &os, 0, sizeof(OVERLAPPED));
   
   //--> �̺�Ʈ ����..
   if( !(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)) ){
		bOk = FALSE;
   }

   //--> �̺�Ʈ ����ũ..
   if( !SetCommMask( pComm->m_hComm, EV_RXCHAR) ){
	   bOk = FALSE;
   }

   //--> �̺�Ʈ��..����ũ ������ ������..
   if( !bOk ){
		//AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		AfxMessageBox(_T("Error while creating ThreadWatchComm, " + pComm->m_sPortName));
		return FALSE;
   }
  
   //��Ʈ�� ����Ǹ� ���� ������ ��
   while (pComm ->m_bPortOpen){
 		dwEvent = 0;
	
        WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);	// �̺�Ʈ�߻� ��� 
	
		//--> �����Ͱ� ���ŵǾ��ٴ� �޼����� �߻��ϸ�..
        if ((dwEvent & EV_RXCHAR) == EV_RXCHAR){
            // ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			//--> buff �� �޾Ƴ���..
			Sleep(100);
			do{
				//���� ������ �о� ����
				dwRead = pComm->ReadComm( buff, 2048);  
				if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead){
					for( WORD i = 0; i < dwRead; i++ ){
						//ť ���ۿ�  ���� ������ �ֱ�
						pComm->m_QueueRead.PutByte(buff[i]); 
					}
				}else{
					//ť������ ũ�⸦ �ʰ��ϸ� ��� �޽��� ����
					AfxMessageBox(_T("buff full"));
				}
			}while(dwRead);

			//TestAppDlg�� �����Ͱ� ���Դٴ� �޽����� ����
			::PostMessage(hCommWnd, RX_EVENT, 0, 0 );
		}


		/*	
		���� �����͸� ȭ�鿡 ������ �ð��� ���� ����.
		�����͸� �������� ������ cpu�������� 100%�� �Ǿ� 
		ȭ�鿡 �ѷ��ִ� �۾��� �� �ȵǰ�, 
		��������� ť ���ۿ� �����Ͱ� ���̰� �� 
		*/
		Sleep(0);			
	}
   
	CloseHandle( os.hEvent);

	//--> ������ ���ᰡ �ǰ���?
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;
}
#endif