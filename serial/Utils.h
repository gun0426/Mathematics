#pragma once

//#define MAX_RECEIVE_NUM	0x8000	// 32k
#define MAX_RECEIVE_NUM		0x40000	// 256k	/* GUN_MARK */
// CUtils

enum 
{
	STATE_NORMAL = 0,
	STATE_B4_FIND,
	STATE_B4_NONZERO_FIND
};

/* Lemel_ziv 알고리즘 관련 DEFINE */
#define ESCAPE  				0xB4		//0910 odo
#define Q_SIZE					256			//0910 odo
#define DEFAULT_ARRAY_SIZE      1024
#define QUEUE_RANGE(uChr) (((uChr) + Q_SIZE) % Q_SIZE)

class CUtils : public CWnd
{
	DECLARE_DYNAMIC(CUtils)

public:
	CUtils();
	virtual ~CUtils();
	BOOL	Time_SixArray2FiveArray(BYTE* pSrc, BYTE* pDes);
	BOOL	Time_FiveArray2SixArray(BYTE* pSrc, BYTE* pDes);
	BOOL	hexBin2hexStr(BYTE* pHex, BYTE* pHexStr, WORD bytecnt);
	BOOL	hexStr2hexBin(BYTE* pHexStr, BYTE* pHex, WORD bytecnt);
	BYTE	Chnge_BitOrder(BYTE msb);
	WORD	CRC16_CCITT(BYTE * buf, WORD size);
	BYTE	Cal_BCC(BYTE * buff,WORD len);
	BYTE	Cal_CS( BYTE * buff, WORD len );
	BYTE	Cal_DigitNum(WORD i_value);
	WORD	modified_atoi(BYTE* i_pdat, BYTE i_len);
//	void	itoa(WORD i_num, char* i_pstr);
	BYTE	modified_itoa(WORD i_value, BYTE* i_buff);
	int		ReadHexFile(CString fn, int file_type);
	int		ReadHexFile_IntelFormat(CString fn);


protected:
	DECLARE_MESSAGE_MAP()
public:
	BYTE	m_fileBuffer[MAX_RECEIVE_NUM];
	int		m_HEXFileSize;
	int xtol(char* str);
	int ReadLzwFile(CString fn);
	unsigned short Cal_CRC16(unsigned char* data, unsigned int size);
	void Lempel_Ziv_Decompression(unsigned char* p_pBuff, unsigned int p_u32Size);
	unsigned short cal_crc16_byteby(unsigned short m_crc, int m_data);
};


