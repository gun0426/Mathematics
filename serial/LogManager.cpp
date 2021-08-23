#include "StdAfx.h"
#include "LogManager.h"
#include "afxinet.h"

BOOL CLogManager::instanceFlag = false;
CLogManager * CLogManager::me = NULL;

CLogManager * CLogManager::GetInstance()
{
	if(! instanceFlag)
	{
		me = new CLogManager();
		instanceFlag = true;
	}
	return me;
}

CLogManager::CLogManager(){
	this->logFile = NULL;
}

CLogManager::~CLogManager(void){
	instanceFlag = false;
	this->Close();
}

void CLogManager::Log(LPCSTR  mesg, ...)
{
	CStdioFile * pFile = this->logFile;
	CString buffer, buffer2, format;
	CTime t;
	va_list argptr;

	format.Format("%s", mesg);

//	if ( pFile == NULL ) {
		pFile = CLogManager::Open();
		if ( pFile == NULL ) {
			AfxMessageBox("Cannot Open Log!!");
			return;
		}
		this->logFile = pFile;
//	}

	t = CTime::GetCurrentTime();
	buffer.Format("%s\t", t.Format("%Y-%m-%d %H:%M:%S"));
	va_start( argptr, mesg );
	for(int i = 0; i < format.GetLength(); i++){
		if(format.GetAt(i) != '%'){
			buffer2 = " ";
			buffer2.SetAt(0, format.GetAt(i));
			buffer += buffer2;
		} else {
			i++;
			switch(format.GetAt(i)){
				case 'd':
					buffer2.Format("%d", va_arg(argptr,int) );
					buffer += buffer2;
					break;
				case 's':
					buffer2.Format("%s", va_arg(argptr, char*) );
					buffer += buffer2;
					break;
				case 'c':
					buffer2.Format("%c", va_arg(argptr, char) );
					buffer += buffer2;
					break;
			}
		}
	}
	va_end( argptr );
	if(buffer.Find("\r\n") < 0)
		buffer += "\r\n";

	TRACE(buffer);

	pFile->WriteString(buffer);
	pFile->Flush();
	pFile->Close();
}

CStdioFile * CLogManager::Open()
{
	CStdioFile * fp = new CStdioFile();
	CFileException e;
	
	CTime t;
	t = CTime::GetCurrentTime();
	CString filePath;
	CString fileName;
	CString progName;
	char csRet[255];   
	ZeroMemory(csRet,sizeof(csRet));

	int ret = ::GetPrivateProfileString( "FTP", "LOG_FLD", NULL, csRet, 255, ".\\ProgIni.ini" );

	if(ret > 0)
	{
		filePath.Format("%s\\", csRet);
	} 
	else 
	{
		filePath.Format(".\\\\LOG\\\\");
	}
	::CreateDirectory(filePath, NULL);
	CWnd * pMain = ::AfxGetApp()->m_pMainWnd;
	pMain->GetWindowText( progName );
	fileName.Format("%s %s.log", AfxGetAppName(), t.Format("%Y%m%d"));

	::CreateDirectory(filePath, NULL);

	if ( !fp->Open(filePath + fileName,  CFile::modeCreate | 
		                                 CFile::modeNoTruncate | 
		                                 CFile::modeWrite | 
										 CFile::shareDenyNone, &e))
	{
		//Error!!
//		if(e.m_cause == CFileException::sharingViolation)
			if(!fp->Open(filePath + fileName,  CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, &e)){
				TCHAR szError[1024];
				e.GetErrorMessage(szError, 1024);
				CString buffer;
				buffer.Format( "Failed Open Log!! (%d)\n%s\n", e.m_cause, szError );
//				AfxMessageBox( buffer );
				TRACE( buffer );
				return NULL;
			}
	}

	fp->SeekToEnd();

	return fp;
}

void CLogManager::Close(){
	this->logFile->Flush();
	this->logFile->Close();
	delete this->logFile;
}