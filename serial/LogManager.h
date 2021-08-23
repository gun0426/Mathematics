#pragma once

#if !defined(_LOGMANAGER_H_)
#define _LOGMANAGER_H_
#endif

#include "afx.h"

class CLogManager
{
public:
	~CLogManager(void);
	void Log(LPCSTR  mesg, ... );
	static CLogManager * GetInstance();

//protected:
	CLogManager();
	CStdioFile * Open();
	void	Close();

	static CLogManager * me;
	static BOOL instanceFlag;
	CStdioFile * logFile;
};
