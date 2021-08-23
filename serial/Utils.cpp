// Utils.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "Yubi.h"
#include "Utils.h"
#include "math.h"
//#include "YubiDlg.h"


// CUtils

IMPLEMENT_DYNAMIC(CUtils, CWnd)

CUtils::CUtils()
{

}

CUtils::~CUtils()
{
}


BEGIN_MESSAGE_MAP(CUtils, CWnd)
END_MESSAGE_MAP()



// CUtils 메시지 처리기입니다.
#if (1)
/***********************************************************************
* * CUtils::Time_SixArray2FiveArray()
*
* *	  "12 10 15 12 34 56" --> "31 79 12 34 56"
* *   Data[0] = (biYear << 1) | ((biMonth & 0x08) >> 3);
* *   Data[1] = ((biMonth & 0x07) << 5) | biDay;
* *   Data[2] = biHour;
* *   Data[3] = biMinute;
* *   Data[4] = biSecond;
*
***********************************************************************/
BOOL CUtils::Time_SixArray2FiveArray(BYTE* pSrc, BYTE* pDes)
{
	BYTE i;
	BYTE j;
//	BYTE temp;
	BYTE aFive[5];
	BYTE aSix[6];
	BYTE aStr[3];

	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 2; j++)
		{
			aStr[j] = *(pSrc + i*2 + j);
		}
		aStr[j] = NULL;
		aSix[i] = atoi((char*)aStr);
	}

	aFive[0] = (aSix[0] << 1) | ((aSix[1] & 0x08) >> 3);
	aFive[1] = ((aSix[1] & 0x07) << 5) | aSix[2];
	aFive[2] = aSix[3];
	aFive[3] = aSix[4];
	aFive[4] = aSix[5];

	/* 16진수 표시 */
	hexBin2hexStr(aFive, pDes, 5);

	return true;
}
/***********************************************************************
* * CUtils::Time_SixArray2FiveArray()
*
*
* *   biSecond= data4 & 0x3F;
* *   biMinute= data3 & 0x3F;
* *   biHour= data2 & 0x1F;
* *   biDay= data1 & 0x1F;
* *   biMonth = (data0 & 0x01) << 3)|((readCpu->data1 & 0xE0) >> 5);
* *   biYear= (data0 & 0xFE) >> 1;
*
***********************************************************************/
BOOL CUtils::Time_FiveArray2SixArray(BYTE* pSrc, BYTE* pDes)
{
	int i;
	BYTE aHex[6];
	BYTE aSix[6];

	CString CStr1 = _T("");
	CString CStr2 = _T("");

	hexStr2hexBin(pSrc, aHex, 5);

	aSix[0] = (aHex[0] & 0xfe) >> 1;
	aSix[1] = ((aHex[0] & 0x01) << 3) | ((aHex[1] & 0xe0) >> 5);
	aSix[2] = aHex[1] & 0x1f;
	aSix[3] = aHex[2] & 0x1f;
	aSix[4] = aHex[3] & 0x3f;
	aSix[5] = aHex[4] & 0x3f;

	/* 10진수 표시 */
	for (i = 0; i < 6; i++)
	{
		CStr1.Format(_T("%02d "), aSix[i]);
		CStr2 += CStr1;
	}

	for (i = 0; i < 3*6; i++)
	{
		pDes[i] = CStr2.GetAt(i);
	}
	pDes[i] = NULL;

	return true;
}
#endif
/***********************************************************************
* * CYubiDlg::hexStr2hexBin()
*
* * Process Rx Receive Event
*
* * "31" --> 0x31
* * "1f" --> 0x1f
* 
***********************************************************************/
BOOL CUtils::hexStr2hexBin(BYTE* pHexStr, BYTE* pHex, WORD bytecnt )	
{
	BYTE temp;
	WORD i = 0, j= 0;

	for(i=0; i<bytecnt*2; i++)
	{

		if( pHexStr[i] >= '0' && pHexStr[i] <= '9' )
		{
			temp = pHexStr[i] - '0';
		}
		else if( pHexStr[i] >= 'A' && pHexStr[i] <= 'F' )
		{
			temp = pHexStr[i] - 'A'+10;
		}
		else if( pHexStr[i] >= 'a' && pHexStr[i] <= 'f' )
		{
			temp = pHexStr[i] - 'a'+10;
		}
		else
		{
			return false;
		}

		if(i%2 == 0)
		{
			pHex[j] = temp << 4;
		}
		else
		{
			pHex[j] |= temp & 0x0f;
			j++;
		}
	}

	return true;
}

//static const BYTE Hexasc_Tbl[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
static const BYTE Hexasc_Tbl[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
/***********************************************************************
* * CYubiDlg::hexBin2hexStr()
*
* * Process Rx Receive Event
*
* * 0x31,0x33,0x34 --> "313334"
* 
***********************************************************************/
BOOL CUtils::hexBin2hexStr(BYTE* pHex, BYTE* pHexStr, WORD bytecnt)
{
	BYTE i=0,j=0;

	for( i = 0; i < bytecnt; i++ ){
		*( pHexStr+j ) = Hexasc_Tbl[*(pHex+i) >> 4];
		j++;
		*( pHexStr+j ) = Hexasc_Tbl[*(pHex+i) & 0x0f];
		j++;
	}

	return true;
}

/***********************************************************************
* * CUtils::Cal_CS()
*
* * 각 바이트를 더한다. 
* * 바이트를 넘어선 부분을 버린다.. 
* 
***********************************************************************/
BYTE CUtils::Cal_CS( BYTE * buff, WORD len )
{
	BYTE sum = 0;

	while(len--)	sum += (*buff++);
	sum %= 256;
	return sum;
}

/***********************************************************************
* * CUtils::Cal_BCC()
*
* * 각 바이트를 XOR 한다. 
* 
***********************************************************************/
BYTE CUtils::Cal_BCC(BYTE * buff,WORD len)
{
	BYTE bcc = 0;

	while(len--) bcc ^= *buff++;
	return bcc;
}

/* CRC16 implementation acording to CCITT standards */
const WORD crc16tab[256]= {
								0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
								0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
								0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
								0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
								0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
								0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
								0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
								0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
								0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
								0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
								0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
								0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
								0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
								0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
								0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
								0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
								0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
								0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
								0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
								0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
								0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
								0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
								0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
								0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
								0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
								0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
								0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
								0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
								0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
								0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
								0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
								0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
							};
/***********************************************************************
* * CUtils::CRC16_CCITT()
*
* *
* 
***********************************************************************/
WORD CUtils::CRC16_CCITT(BYTE * buf, WORD size)
{
	WORD cnt;
	WORD crc = 0;
	
	for(cnt = 0; cnt < size; cnt++){
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *buf++)&0x00ff];
	}
		
	return crc;
}

/***********************************************************************
* * CUtils::Chnge_BitOrder()
*
* * 0x41 : 0100 0001 --> 1000 0010 : 0x82
* 
***********************************************************************/
BYTE CUtils::Chnge_BitOrder(BYTE msb)
{
	BYTE i;
	BYTE lsb = 0;

	for(i=0; i<8; i++){
		lsb |= (msb & 0x01)<<(7-i);
		msb >>= 0x01;
	}

	return lsb;
}

/***********************************************************************
* * CUtils::Cal_DigitNum()
*
* * 자릿수(Digit Number) 를 계산해준다.
*   - WORD --> 4294967296 --> 10-Digit Number
*
***********************************************************************/
BYTE CUtils::Cal_DigitNum(WORD i_value)
{
	BYTE k;
	static WORD temp;

	for (k = 0; k < 9; k++)
	{
		temp = (i_value / (WORD)pow((long double)10, (int)9-k));	// 1000000000 ~ 1
		if (temp != 0)						
		{
			break;
		}
	}

	return (10-k);
}

/***********************************************************************
* * CUtils::modified_atoi()
*
* * '1' '2' '3' '0' '0' '0' --> 321
*
***********************************************************************/
WORD CUtils::modified_atoi(BYTE* i_pdat, BYTE i_len)
{
	BYTE k;
	BYTE aHexStr[10];
	WORD value;

	/* length limit check */
	if (i_len > 10)
	{
		return 0;
	}
	
	/* ascii-digit check */
	for (k = 0; k < i_len; k++)
	{
		if( (*(i_pdat+k) > '9') || (*(i_pdat+k) < '0') )
		{
			return 0;
		}
	}

	/* change order */
	for (k = 0; k < i_len; k++)
	{
		 aHexStr[k] = *(i_pdat+i_len-1-k);
	}
	aHexStr[k] = 0x00;
	
#if (0)
	/* reject head '0's */
	for (k = 0; k < i_len; k++)
	{

	}
#endif
	value = atoi((char const*)aHexStr);	// #include <stdlib.h>

	return value;
}

#if (0)
/***********************************************************************
* * CUtils::itoa()
*
* * 12345 --> "12345"
*
***********************************************************************/
void CUtils::itoa(WORD i_num, char* i_pstr)
{ 
	WORD k = 0; 
	WORD radix = 10; 	// 진수 
	WORD deg = 1; 
	WORD cnt = 0; 

	if (i_num == 0)		// GUN-MARK
	{
		*i_pstr++ = '0';
		*i_pstr = '\0';
	}

	while(1)			// 자리수의 수를 뽑는다 
	{	 
		if( (i_num/deg) > 0 ) 
		{
			cnt++; 
		}
		else 
		{
			break; 
		}
		deg *= radix; 
	} 
	deg /= radix;						// deg가 기존 자리수보다 한자리 높게 카운트 되어서 한번 나누어줌  
										// EX) 1241 ->	  cnt = 4; deg = 1000; 
	for(k = 0; k < cnt; k++)			// 자리수만큼 순회 
	{			
		*(i_pstr+k) = i_num/deg + '0';	// 가장 큰 자리수의 수부터 뽑음 
		i_num -= ((i_num/deg) * deg);	// 뽑은 자리수의 수를 없엠 
		deg /= radix;					// 자리수 줄임 
	} 
	*(i_pstr+k) = '\0'; 				// 문자열끝널.. 
}  

/***********************************************************************
* * CUtils::modified_itoa()
*
* * 12345 --> "12345"
*
***********************************************************************/
#define DIGIT_NUM	6
BYTE CUtils::modified_itoa(WORD i_value, BYTE* i_buff)
{
	BYTE k;
	BYTE j;
	BYTE digit;
	BYTE aStr1[DIGIT_NUM];
	BYTE aStr2[DIGIT_NUM];

	digit = Cal_DigitNum(i_value);			// ex) 123 --> digit = 3
	if (digit > DIGIT_NUM)
	{
		return 0;
	}
	
	itoa(i_value, (char*)aStr1);			// ex) 123 --> '1' '2' '3' '\0' X X

	for (k = 0; k < DIGIT_NUM-digit; k++)
	{
		aStr2[k] = '0';
	}

	for (j = 0; j < digit; j++)
	{
		aStr2[k+j] = aStr1[j];
	}

	for (k = 0; k < DIGIT_NUM; k++)
	{
		*(i_buff+k) = aStr2[DIGIT_NUM-1-k];
	}

	return 1;
}
#endif


/********************************************************************************
Hex File을 읽어서 m_fileBuffer 에 저장한다

리턴값 :
0 :  정상
-1 : HEX_FILE_LARGER_THAN_ROM_SIZE
-2 : HEX_FILE_SMALL_THAN_ROM_SIZE
-3 : file not found
-4 : invalid HEX file
*********************************************************************************/
int CUtils::ReadHexFile(CString fn, int file_type)	// GUN_DOWN
{
	FILE		*fp;
	int			ch;
	int			i;
	int			err_code = 0;

	for (i = 0; i < MAX_RECEIVE_NUM; i++)
	{
		m_fileBuffer[i] = 0x00;		// 버퍼를 클리어 시킨다	
	}

	//---------------------------------------------------------------------------------------------------
	// 모토롤라 포맷인지 인텔 포맷인지 체크하여 각 Read루틴에서 읽어들인다
	//---------------------------------------------------------------------------------------------------
	//fp = fopen ((char *)(LPCTSTR) fn, "r");
	fopen_s(&fp, fn, "r");
	if (!fp)	return -3;

	ch = fgetc(fp);
	if (ch == ':')
	{
		fclose(fp);
		err_code = ReadHexFile_IntelFormat(fn);
	}
	else if (ch == 'S')
	{
		fclose(fp);
		//		err_code = ReadHexFile_MotorolaFormat(fn);
		err_code = -4;
	}
	else
	{
		err_code = -4;
	}

	return err_code;
}
/********************************************************************************
Hex File을 읽어서 m_fileBuffer 에 저장한다 (인텔 포맷)

리턴값 :
0 :  정상
-1 : HEX_FILE_LARGER_THAN_ROM_SIZE
-2 : HEX_FILE_SMALL_THAN_ROM_SIZE
-3 : file not found
-4 : invalid HEX file
*********************************************************************************/
int CUtils::ReadHexFile_IntelFormat(CString fn)
{
	FILE	*fp;
	char	line_str[1024];
	char	tmp_str[1024];
	int		i;
	int		index;
	int		ch;
	int		val;
	int		num;
	int		high_addr = 0;
	int		prev_high_addr = 0;
	int		addr;
	int		total_addr;
	int		type;
	int		loop_cnt = 0;
	int		first_high_addr_flag = 1;


	//fp = fopen((char *)(LPCTSTR) fn, "r");
	fopen_s(&fp, fn, "r");
	if (!fp)	return FALSE;

	m_HEXFileSize = 0;

	high_addr = 0;
	addr = 0;
	total_addr = 0;
	while (1)
	{
		//---------------------------------------------
		// get one line
		//---------------------------------------------
		index = 0;
		loop_cnt = 0;
		do
		{
			if (feof(fp))	break;

			ch = fgetc(fp);

			if (isalnum(ch))
			{
				line_str[index++] = (char)ch;
			}
			else
			{
				if ((ch == '\n') || (ch == 0x0d) || (ch == 0x0a))
					break;
			}
		} while (1);

		line_str[index] = '\0';

		if (feof(fp))					break;
		if (line_str[0] == '\0')		continue;

		tmp_str[0] = line_str[0];					// get data length
		tmp_str[1] = line_str[1];
		tmp_str[2] = '\0';
		num = xtol((char *)tmp_str);

		if (num > 0)
		{
			tmp_str[0] = line_str[2];				// get addr
			tmp_str[1] = line_str[3];
			tmp_str[2] = line_str[4];
			tmp_str[3] = line_str[5];
			tmp_str[4] = '\0';
			addr = xtol((char *)tmp_str);
		}

		tmp_str[0] = line_str[6];					// get type
		tmp_str[1] = line_str[7];
		tmp_str[2] = '\0';
		type = xtol((char *)tmp_str);

		switch (type)
		{
		case 0:
			//--------------------------------------------------------------------	
			// 롬코드 데이터
			//--------------------------------------------------------------------	
			for (i = 0; i < num * 2; i++)
			{
				tmp_str[i % 2] = line_str[i + 8];

				if ((i % 2) == 1)
				{
					tmp_str[2] = '\0';
					val = xtol((char *)tmp_str);

					total_addr = (high_addr << 16) | addr;
					m_fileBuffer[total_addr] = val;

					if (total_addr > m_HEXFileSize)
					{
						m_HEXFileSize = total_addr;
					}
					addr++;
//					m_HEXFileSize++;
				}
			}
			break;

		case 4:
			//------------------------------------------------------------------------------------	-------------------
			// 상위 2바이트 어드레스 지정
			// 2바이트중 하위 바이트만 어드레스로 이용. 
			//------------------------------------------------------------------------------------	-------------------
			tmp_str[0] = line_str[8 + 2];
			tmp_str[1] = line_str[8 + 3];
			tmp_str[2] = '\0';
			val = xtol((char *)tmp_str);

			if (first_high_addr_flag)
			{
				high_addr = 0;
				prev_high_addr = val;
				first_high_addr_flag = 0;
			}
			else
			{
				high_addr = val - prev_high_addr;
			}
			break;
		}
	}

	fclose(fp);

	return	0;
}


int CUtils::xtol(char* str)
{
	int    i, val, po;

	val = 0;
	po = 1;
	for (i = (int)strlen(str) - 1; i >= 0; i--)
	{
		if (str[i] != '_')
		{
			if (iswdigit(str[i]))
				val += (str[i] - '0') * po;
			else
				val += (toupper(str[i]) - 'A' + 10) * po;

			po = po << 4;
		}
	}
	return(val);
}

#if (0)
int CUtils::ReadLzwFile(CString fn)
{
	int i;
	int index;
	int size;
	FILE	*fp;

	for (i = 0; i < MAX_RECEIVE_NUM; i++)
	{	/* 버퍼를 클리어 시킨다 */
		m_fileBuffer[i] = 0x00;		
	}

	fopen_s(&fp, fn, "rb");		// rb:read binary
	if (!fp)
	{
		return FALSE;
	}

#if (1)	
	fseek(fp, 0L, SEEK_END);	// file의 끝에서 0바이트 이동
	size = ftell(fp);			// file의 현재 위치를 반환
	fseek(fp, 0L, SEEK_SET);	// file의 처음에서 0바이트 이동

	for (index = 0; index < size; index++)
	{
		if (feof(fp))
		{
			break;
		}
		m_fileBuffer[APP_START+index] = fgetc(fp);
	}
#else	/* 맨뒤에 0xff : why? */
	fseek(fp, 0L, SEEK_SET);
	index = 0;
	while (1)
	{
		if (feof(fp))
		{
			break;
		}
		m_fileBuffer[index++] = fgetc(fp);
	}
#endif
	fclose(fp);

	return index;
}
#endif

#if (0)
#define CRC16 0x8005
unsigned short CUtils::Cal_CRC16(unsigned char* data, unsigned int size)
{
    unsigned short crc = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
	{
        return 0;
	}

    while(size > 0)
    {
        bit_flag = crc >> 15;

        /* Get next bit: */
        crc <<= 1;
        crc |= (*data >> (7 - bits_read)) & 1;

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
		{
            crc ^= CRC16;
		}
    }
    return crc;
}
#endif

#if (0)
/*
//                                      16   12   5
// this is the CCITT CRC 16 polynomial X  + X  + X  + 1.		1000 0100 0000 1000
// This works out to be 0x1021, but the way the algorithm works
// lets us use 0x8408 (the reverse of the bit pattern).  The high
// bit is always assumed to be set, thus we only use 16 bits to
// represent the 17 bit value.
*/
//unsigned short crc16(char *data_p, unsigned short length)
#define POLY_NORM 0x8408
unsigned short CUtils::Cal_CRC16(unsigned char* data_p, unsigned int length)
{
	unsigned char i;
	unsigned int data;
	unsigned int crc = 0xffff;
	
	if (length == 0)
	      return (~crc);
	
	do
	{
		for (i=0, data=(unsigned int)0xff & *data_p++;
		     i < 8; 
		     i++, data >>= 1)
		{
		      if ((crc & 0x0001) ^ (data & 0x0001))
		            crc = (crc >> 1) ^ POLY_NORM;
		      else  crc >>= 1;
		}
	} while (--length);
	
	crc = ~crc;
	data = crc;
	crc = (crc << 8) | (data >> 8 & 0xff);
	
	return (crc);
}
#endif

#if (0)
/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * This document is the property of Apple Inc.
 * It is considered confidential and proprietary.
 *
 * This document may not be reproduced or transmitted in any form,
 * in whole or in part, without the express written permission of
 * Apple Inc.
 *
 * CRC-16-ANSI (aka CRC-16-IBM) Polynomial: x^16 + x^15 + x^2 + 1
 * Derived from Craig Marciniak's "Craig's Portable CRC16 Library."
 */
static unsigned int crc16_tab[256] = {
	0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
	0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
	0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
	0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
	0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
	0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
	0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
	0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
	0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
	0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
	0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
	0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
	0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
	0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
	0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
	0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
	0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
	0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
	0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
	0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
	0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
	0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
	0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
	0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
	0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
	0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
	0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
	0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
	0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
	0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
	0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
	0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
};
//unsigned short crc16(unsigned short crc, const void *buf, size_t size)
unsigned short CUtils::Cal_CRC16(unsigned char* data_p, unsigned int length)
{
	const unsigned char *p;
	unsigned short crc = 0;

	p = data_p;

	while (length--)
	{
		crc = crc16_tab[(crc ^ (*p++)) & 0xFF] ^ (crc >> 8);
	}

    return crc;
}
#endif

#if (0)
#define POLY_NORM	0x1021
unsigned short CUtils::Cal_CRC16(unsigned char* addr, unsigned int num)		// 0x0000  ???
{
/* On entry, addr=>start of data
             num = length of data
             crc = incoming CRC     */
//int crc16(char *addr, int num, int crc)
//{
	int i;
	unsigned short crc = 0;

	for (; num>0; num--)							/* Step through bytes in memory */
	{
		crc = crc ^ (*addr++ << 8);					/* Fetch byte from memory, XOR into CRC top byte*/
		for (i=0; i<8; i++)							/* Prepare to rotate 8 bits */
		{
			crc = crc << 1;							/* rotate */
			if (crc & 0x10000)						/* bit 15 was set (now bit 16)... */
			{
				crc = (crc ^ POLY_NORM) & 0xFFFF;	/* XOR with XMODEM polynomic */
			}
													/* and ensure CRC remains 16-bit value */
		}											/* Loop for 8 bits */
	}												/* Loop until num=0 */
	
	return(crc);									/* Return updated CRC */
}
#endif

#if (0)	// #define POLY_NORM	0x1021
static const unsigned short crc16_tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
}; 
unsigned short CUtils::Cal_CRC16(unsigned char* crcdata, unsigned int len)		// 0xF176
{

    unsigned int i;

    unsigned short crc = 0xffff;	// 0x0000 이면 0x5E7F, 0xffff 이면 0xF176

    for(i = 0; i < len; i++)  
	{  
		crc = (crc << 8) ^ crc16_tab[((crc >> 8) ^ *crcdata++) & 0x00FF];
    }              

    return crc;
}
#endif

#if (1)	
#define POLY_NORM	0x1021
unsigned short CUtils::Cal_CRC16(unsigned char* crcdata, unsigned int len)		// 0x47a1
{
	unsigned int i,j;
	unsigned short crc = 0xffff; //crc=0;
	
	for(i=0;i<len;i++)
	{
		crc = crc ^ ((*crcdata)<<8);
		crcdata++;                 
		for(j=0;j<8;j++)
		{
			if(crc & 0x8000)
			{
				crc = (crc<<1) ^ POLY_NORM;              
			}
			else
			{
				crc = crc<<1;    
			}           
		}
	}
	return crc;                        
}
#endif


unsigned short CUtils::cal_crc16_byteby(unsigned short m_crc, int m_data)
{
	unsigned short crc;

	crc = m_crc;

	crc = crc ^ (m_data<<8);               
	for(int j=0; j<8; j++)
	{
		if(crc & 0x8000)
		{
			crc = (crc<<1) ^ POLY_NORM;              
		}
		else
		{
			crc = crc<<1;    
		}           
	}

	return crc;       
}



void CUtils::Lempel_Ziv_Decompression(unsigned char* p_pBuff, unsigned int p_u32Size)
{
	UINT8	u8Q_Length;
	UINT8	u8Index;
	UINT8	u8Queue_Data;
	UINT16	u16Q_SP;
	UINT32	u32Total_Cnt = 0;	// origine : u16Total_Cnt
	UINT8	u8LZW_State = STATE_NORMAL;
	UINT32	tIndex = 0;
	UINT16	u16Q_Buffer_Index = 0;
	UINT8	u8Queue[Q_SIZE];
	UINT8	temp;
	CFile file;

	file.Open(_T("decompress.bin"), CFile::modeCreate | CFile::modeWrite);
    
	// Lempel ziv Analysis
	while(u32Total_Cnt < p_u32Size)								// packet에서 수신한 데이터 수 
	{
//		HAL_CLEAR_WATCHDOG();
		switch (u8LZW_State) 
		{
			case STATE_NORMAL:
				u8Queue_Data = p_pBuff[u32Total_Cnt++];			// u32Total_Cnt 호출 후 1증가 
				if (u8Queue_Data == ESCAPE)						// 0xB4
				{
//file.Write(&u8Queue_Data, 1);
					u8LZW_State = STATE_B4_FIND;
				} 
				else
				{
#if (0)
					WriteToDataFlashAndQBuffer(u8Queue_Data);	// Q 입력,	
#else
					u8Queue[u16Q_Buffer_Index] = u8Queue_Data;
					++u16Q_Buffer_Index;	
					u16Q_Buffer_Index = u16Q_Buffer_Index % Q_SIZE;	
					file.Write(&u8Queue_Data, 1);
#endif
				}
				break;
			case STATE_B4_FIND: // XX B4 XX 		 XX
				u8Queue_Data = p_pBuff[u32Total_Cnt++];			// u32Total_Cnt 호출 후 1증가 
				if (u8Queue_Data == 0x00) 
				{
#if (0)
					WriteToDataFlashAndQBuffer(ESCAPE); 		// Queue Buffer  = 0xB4 입력
#else
					u8Queue[u16Q_Buffer_Index] = ESCAPE;
					++u16Q_Buffer_Index;	
					u16Q_Buffer_Index = u16Q_Buffer_Index % Q_SIZE;	
					temp = ESCAPE;
					file.Write(&temp, 1);
#endif
					u8LZW_State = STATE_NORMAL;
				} 
				else
				{
					u8LZW_State = STATE_B4_NONZERO_FIND;
				}
				break;
			case STATE_B4_NONZERO_FIND:
				u8Q_Length = p_pBuff[u32Total_Cnt++];										// Length 읽어오기
				u16Q_SP = (UINT16)QUEUE_RANGE(u16Q_Buffer_Index - (UINT16)u8Queue_Data);	// u8Data을 start Pointer로 대입 
				for (u8Index = 0; u8Index < u8Q_Length; u8Index++)							// SP로 가서 Length만큼 복사하여 Queue넣기
				{
					u8Queue_Data = u8Queue[QUEUE_RANGE(u16Q_SP + (UINT16)u8Index)];
#if (0)
					WriteToDataFlashAndQBuffer(u8Queue_Data);	
#else
					u8Queue[u16Q_Buffer_Index] = u8Queue_Data;
					++u16Q_Buffer_Index;	
					u16Q_Buffer_Index = u16Q_Buffer_Index % Q_SIZE;	
					file.Write(&u8Queue_Data, 1);
#endif
				}
				u8LZW_State = STATE_NORMAL;
				break;
			default:
				break;
				
		}
	}	 

	file.Close();
}

