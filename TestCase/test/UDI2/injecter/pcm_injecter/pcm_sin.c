/******************************************************************************
FileName:  pcm_sin.c
Copyright (c) 2010， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        wuzhengbing (wuzhengbing@coship.com)

File Description:
        File Description, Please add you description here.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2010-10-08  wuzhengbing     Create
*******************************************************************************/

/******************************************************************************
 *                                 Head file                                 *
 ******************************************************************************/
#include <math.h>
#include <string.h>

#include "pcm_sin.h"
#include "udiplus_debug.h"
#include "udiplus_typedef.h"
#include "udiplus_debug.h"
#include "udi2_os.h"

/******************************************************************************
 *                                 Local Define                                *
 ******************************************************************************/
typedef struct 
{
	CSUDIINJECTERPcmStartParam_S m_StartParam;//PCM播放参数,ts流时该项为0
    	CSUDIINJECTERPcmEndian_E m_enmEndian;//文件数据的大小端字节序特性 ,ts时该项为0   
	double m_dT;
	int m_nSampleLevel;
	int m_nSinFreq;
	CSUDI_BOOL m_bOpen;
} PcmSin;
/******************************************************************************
 *                                 Global Variables                                   *
 ******************************************************************************/

#define SAMPLE_LEVEL_FOR_SIN 127
#define SAMPLE_RATE_FOR_SIN 48000
#define BIT_WIDTH_FOR_SIN 8 /* 8位采样，没有大小端问题 */
#define CHANNEL_NUM_FOR_SIN 1
#define PI_2  ((double)6.2831853071796)

#define LE

static void Swap16(BYTE* word)
{
	BYTE t = word[0];
	word[0] = word[1];
	word[1] = t;
}

static void Swap32(BYTE* word)
{
	BYTE t = word[0];
	word[0] = word[3];
	word[1] = t;

	t = word[1];
	word[1] = word[2];
	word[2] = t;
}

static CSUDI_INT8 GenSample8(PcmSin * pPcmSin)
{
	CSUDI_INT8 sample = (CSUDI_INT8)(pPcmSin->m_nSampleLevel*sin(pPcmSin->m_dT));
	pPcmSin->m_dT += PI_2*pPcmSin->m_nSinFreq/pPcmSin->m_StartParam.m_uSampleRate;
	return sample;
}

static CSUDI_INT16 GenSample16(PcmSin * pPcmSin)
{
	CSUDI_INT16 sample = (CSUDI_INT16)(pPcmSin->m_nSampleLevel*sin(pPcmSin->m_dT));
	pPcmSin->m_dT += PI_2*pPcmSin->m_nSinFreq/pPcmSin->m_StartParam.m_uSampleRate;

#ifdef LE
	if (pPcmSin->m_enmEndian == EM_UDIINJECTER_BIG_ENDIAN)
	{
		Swap16((BYTE*)&sample);
	}
#else
	if (pPcmSin->m_enmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		Swap16((BYTE*)&sample);
	}
#endif

	return sample;
}

static CSUDI_INT32 GenSample32(PcmSin * pPcmSin)
{
	CSUDI_INT32 sample = (CSUDI_INT32)(pPcmSin->m_nSampleLevel*sin(pPcmSin->m_dT));
	pPcmSin->m_dT += PI_2*pPcmSin->m_nSinFreq/pPcmSin->m_StartParam.m_uSampleRate;

#ifdef LE
	if (pPcmSin->m_enmEndian == EM_UDIINJECTER_BIG_ENDIAN)
	{
		Swap32((BYTE*)&sample);
	}
#else
	if (pPcmSin->m_enmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		Swap32((BYTE*)&sample);
	}
#endif

	return sample;
}

static int FillSample(PcmSin * pPcmSin, BYTE* buf, int size)
{
	int i = 0;
	CSUDI_INT16 wSample;
	CSUDI_INT32 dwSample;
	
	int nBufferToFill = 0;
	int nSampleInterval = 0;

	nSampleInterval = (pPcmSin->m_StartParam.m_uBitsSample/8)*pPcmSin->m_StartParam.m_uChannels;
	nBufferToFill = size/nSampleInterval*nSampleInterval;
	
	if (pPcmSin->m_StartParam.m_uBitsSample == 8)
	{
		if (pPcmSin->m_StartParam.m_uChannels == 2)
		{
			for (i = 0; i < nBufferToFill; i += nSampleInterval)
			{
				buf[i + 1]  = buf[i] = GenSample8(pPcmSin);
			}
		}
		else /* 1 channel ? */
		{
			for (i = 0; i < nBufferToFill; i += nSampleInterval)
			{
				buf[i] = GenSample8(pPcmSin);
			}
		}
	}
	else if (pPcmSin->m_StartParam.m_uBitsSample == 16) //16
	{
		if (pPcmSin->m_StartParam.m_uChannels == 2)
		{
			for (i = 0; i < nBufferToFill; i += nSampleInterval)
			{
    				wSample = GenSample16(pPcmSin);
				memcpy(buf + i, &wSample, 2);
				memcpy(buf + i + 2, &wSample, 2);
			}
		}
		else /* 1 channel ? */
		{
			for (i = 0; i < nBufferToFill; i += nSampleInterval)
			{
    				wSample = GenSample16(pPcmSin);
				memcpy(buf + i, &wSample, 2);
			}
		}
	}
	else if (pPcmSin->m_StartParam.m_uBitsSample == 32) //32
	{
		if (pPcmSin->m_StartParam.m_uChannels == 2)
		{
			for (i = 0; i < nBufferToFill; i += nSampleInterval)
			{
    				dwSample = GenSample32(pPcmSin);
				memcpy(buf + i, &dwSample, 4);
				memcpy(buf + i + 4, &dwSample, 4);
			}
		}
		else /* 1 channel ? */
		{
			for (i = 0; i < nBufferToFill; i += nSampleInterval)
			{
    				dwSample = GenSample32(pPcmSin);
				memcpy(buf + i, &dwSample, 4);
			}
		}
	}
	else
	{
		CSASSERT(0);
	}	

	return nBufferToFill;
}

/******************************************************************************
 *                                 Local Variables                                   *
 ******************************************************************************/

/******************************************************************************
 *                                 Static Function Declare                               *
 ******************************************************************************/

/******************************************************************************
 *                                 Global Functions                                   *
 ******************************************************************************/
CSUDI_HANDLE CSPcmSin_Open(const CSUDIINJECTERPcmStartParam_S *pStartParam, CSUDIINJECTERPcmEndian_E eEndian, int nFreq)
{
	PcmSin* pPcmSin = NULL;

	//param check
	if (pStartParam == NULL)
	{
		CSASSERT(0);
		return NULL;
	}

	if (pStartParam->m_uBitsSample != 8
		&& pStartParam->m_uBitsSample != 16
		&& pStartParam->m_uBitsSample != 32)
	{
		CSASSERT(0);
		return NULL;
	}
	
	if (pStartParam->m_uSampleRate == 0)
	{
		CSASSERT(0);
		return NULL;
	}
	
	if (pStartParam->m_uChannels != 1
		&& pStartParam->m_uChannels != 2)
	{
		CSASSERT(0);
		return NULL;
	}

	pPcmSin = (PcmSin*)CSUDIOSMalloc(sizeof(PcmSin));
	pPcmSin->m_dT = 0;
	pPcmSin->m_enmEndian = eEndian;
	pPcmSin->m_StartParam = *pStartParam;

	switch (pPcmSin->m_StartParam.m_uBitsSample)
	{
	case 32: 
		pPcmSin->m_nSampleLevel = 0x7FFFFFFF;
		break;
	case 16: 
		pPcmSin->m_nSampleLevel = 0x7FFF;
		break;
	case 8:
	default:
		pPcmSin->m_nSampleLevel = 0x7F;
		break;
	}

	pPcmSin->m_nSinFreq = nFreq;
	pPcmSin->m_bOpen = TRUE;

	return (CSUDI_HANDLE)pPcmSin;
}

int CSPcmSin_Read(CSUDI_HANDLE hPcmSin, void* pBuffer, int nBufSize)
{
	PcmSin* pPcmSin = (PcmSin*)hPcmSin;

	if (pPcmSin == NULL)
	{
		CSASSERT(0);
		return -1;
	}
	if (!pPcmSin->m_bOpen)
	{
		CSASSERT(0);
		return -1;
	}
	
	return FillSample(pPcmSin, pBuffer, nBufSize);
}

void CSPcmSin_Close(CSUDI_HANDLE hPcmSin)
{
	PcmSin* pPcmSin = (PcmSin*)hPcmSin;
	memset(pPcmSin, 0, sizeof(PcmSin));
	CSUDIOSFree(pPcmSin);
}

/******************************************************************************
 *                                 Static Functions                                   *
 ******************************************************************************/

/* End of File */

