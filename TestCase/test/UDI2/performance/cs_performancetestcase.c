/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
//#include "cs_performancetestcase.h"
#include <string.h>
#include <stdio.h>
#include "cs_assert.h"
#include "udiplus_os.h"
#include "udiplus_debug.h"
#include "cs_testkit.h"
#include "cs_testkit_porting.h"

#include "decError.h"
//#include "cs_portingtestentry.h"

#include "zlib.h"
#include "LzmaDec.h"
#include "udi2_os.h"

CSUDI_BOOL CSTC_PERFORMANCE_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PERFORMANCE_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

#define	ZALLOC_ALIGNMENT	16
#define MAX_WBITS   15
#define HEAD_CRC	2
#define EXTRA_FIELD	4
#define ORIG_NAME	8
#define COMMENT		0x10
#define RESERVED	0xe0

#define DEFLATED	8
//#define   SPACE_SIZE (100 * 1024)


static void* UnzipAlloc(void* pOpaque, unsigned int dwItems, unsigned int dwSize)
{
	void* pAllocAddr;

	pAllocAddr = CSUDIOSMalloc (dwSize*dwItems);

	return (pAllocAddr);
}

static void UnzipFree(void* pOpaque, void* pAddr, unsigned int dwSize)
{
	CSUDIOSFree(pAddr);
}

/**
@brief ��ѹ����gzipѹ��������

@param[in] pDstAddr Ŀ��buffer�ĵ�ַ����ѹ������ݽ������õ��������Ϊ�ڴ��ַ
@param[in] dwDstLen Ŀ��buffer�ĳ���
@param[in] pSrcAddr Դbuffer�ĵ�ַ��ѹ�����ݴ��������
@param[in] dwSrcLen Դbuffer�ĳ���
@return ��ѹ���ɹ�ʱ���ؽ�ѹ������ݳ��ȣ���ѹ��ʧ��ʱ����BLErrType_E���͵Ĵ���
*/
int BLUnZip(unsigned char* pDstAddr,unsigned int dwDstLen, unsigned char* pSrcAddr, unsigned int dwSrcLen)
{
	z_stream s;

	int nZipRet;
	int nOffset;
	int nFlags;
	
	unsigned int dwRetLen = 0;
	
	if(pDstAddr == NULL || pSrcAddr == NULL)
	{
		return -EM_ERR_PARAM;
	}
	
	if(0 == dwDstLen || 0 == dwSrcLen)
	{
		return -EM_ERR_PARAM;
	}
	
	/* skip header */
	nOffset = 10;
	nFlags = pSrcAddr[3];

	if (pSrcAddr[2] != DEFLATED || (nFlags & RESERVED) != 0) 
	{
		return (-EM_ERR_DECOMP);
	}
	
	if ((nFlags & EXTRA_FIELD) != 0)
	{
		nOffset = 12 + pSrcAddr[10] + (pSrcAddr[11] << 8);
	}
	
	if ((nFlags & ORIG_NAME) != 0)
	{
		while (pSrcAddr[nOffset++] != 0)
		;
	}
	if ((nFlags & COMMENT) != 0)
	{
		while (pSrcAddr[nOffset++] != 0)
		;
	}
	
	if ((nFlags & HEAD_CRC) != 0)
	{
		nOffset += 2;
	}
	
	if (nOffset >= dwSrcLen) 
	{
		return (-EM_ERR_DECOMP);
	}

	s.zalloc = UnzipAlloc;
	s.zfree = UnzipFree;
	s.outcb = Z_NULL;

	nZipRet = cs_inflateInit2(&s, -MAX_WBITS);
	if (nZipRet != Z_OK) 
	{
		return (-EM_ERR_DECOMP);
	}

	s.next_in = pSrcAddr + nOffset;
	s.avail_in = dwSrcLen - nOffset;
	s.next_out = pDstAddr;
	s.avail_out = dwDstLen;
	nZipRet = cs_inflate(&s, Z_FINISH);

	if (nZipRet != Z_OK && nZipRet != Z_STREAM_END) 
	{
		return (-EM_ERR_DECOMP);
	}

	dwRetLen = s.next_out - (unsigned char*) pDstAddr;
	cs_inflateEnd(&s);

	return dwRetLen;
}

/************************lZMA******************/
static void *UnLzmaAlloc(void *p, size_t size)
{
	void* pAllocAddr;

	pAllocAddr = CSUDIOSMalloc (size);

	return (pAllocAddr);

}
static void UnLzmaFree(void *p, void *address)
{
	CSUDIOSFree(address);
}

static ISzAlloc s_UnLzmaAllocFree = { UnLzmaAlloc, UnLzmaFree };


/**
@brief ��ѹ����lzmaѹ��������

@param[in] pDstAddr Ŀ��buffer�ĵ�ַ����ѹ������ݽ������õ��������Ϊ�ڴ��ַ
@param[in] dwDstLen Ŀ��buffer�ĳ���
@param[in] pSrcAddr Դbuffer�ĵ�ַ��ѹ�����ݴ��������
@param[in] dwSrcLen Դbuffer�ĳ���
@return ��ѹ���ɹ�ʱ���ؽ�ѹ������ݳ��ȣ���ѹ��ʧ��ʱ����BLErrType_E���͵Ĵ���
*/
static int BLUnLzma(unsigned char* pDstAddr,unsigned int dwDstLen, unsigned char* pSrcAddr, unsigned int dwSrcLen)
{
    unsigned int dwRetLen = 0;
    unsigned int dwRealSrclen = 0;
    UInt64 unpackSize;
    
    SRes res = SZ_OK;
    int i = 0;
    unsigned char* pDst = NULL;
    unsigned char* pSrc = NULL;

    /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    unsigned char header[LZMA_PROPS_SIZE + 8];
    ELzmaStatus status;
    
    if(pDstAddr == NULL || pSrcAddr == NULL)
	{
		return -EM_ERR_PARAM;
	}
	
	if(0 == dwDstLen || 0 == dwSrcLen)
	{
		return -EM_ERR_PARAM;
	}
	
    pDst = pDstAddr;
    pSrc = pSrcAddr;


    unpackSize = 0;
    memset(header,0, sizeof(header));
    memcpy(header,pSrc,sizeof(header));

    pSrc += sizeof(header);
     
    for (i = 0; i < 8; i++)
    {
        unsigned char b = header[LZMA_PROPS_SIZE + i];
        unpackSize += (UInt64)b << (i * 8);
    }

    if(unpackSize <= 0 || unpackSize > dwDstLen)
    {
		return -EM_ERR_DECOMP;
    }
    else
    {
		dwRetLen = unpackSize;
    }
    
    dwRealSrclen = dwSrcLen;
    
    res = LzmaDecode(pDst, &dwRetLen, pSrc, &dwRealSrclen, header, 
    					LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &status, &s_UnLzmaAllocFree);
    					
    if(SZ_OK == res 
    	&& (LZMA_STATUS_FINISHED_WITH_MARK == status || LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK == status))
    {
    	return dwRetLen;
    }
    else
    {
    	return -EM_ERR_DECOMP;
    }
}

static int CSGetFileSize(FILE *fp)
{	
	int nFileLen;
	
	if (fp == NULL)
		return -1;
	
	CSTKP_FSeek(fp,0,CSTK_SEEK_END);
	nFileLen =CSTKP_FTell(fp);
	CSTKP_FSeek(fp,0,CSTK_SEEK_SET);
	
	return nFileLen;
}

#define VerifyUnit	(512)

static CSUDI_BOOL CSVerifyDecompressData(FILE *pfOri,char *pcDecData,int nDecDataLen)
{
	char 	buf[VerifyUnit];
	int 		nReadLen;
	int 		nHaveVerified = 0;
	
	if (pfOri == NULL ||pcDecData == NULL || nDecDataLen <= 0)
		return CSUDI_FALSE;

	if (nDecDataLen != CSGetFileSize(pfOri))
		return CSUDI_FALSE;
	
	while(1)
	{
		nReadLen = CSTKP_FRead(buf,1,VerifyUnit,pfOri);
	
		if (nReadLen > 0)
		{
			if (memcmp(buf,pcDecData,nReadLen))
			{
				break;
			}
		}

		if (nReadLen < VerifyUnit)
		{
			break;
		}	

		nHaveVerified += nReadLen;
		if (nHaveVerified >= nDecDataLen)
		{
			break;
		}

		pcDecData += nReadLen;
	}

	return (nHaveVerified == nDecDataLen);
}

#define ORIGINAL_FILE_SIZE	(1024*1024*5)

//@CASEGROUP:CS_PERFORMANCE
//@DESCRIPTION:����ƽ̨ʹ��LZMA��Gzip�㷨��5MԴ�ļ�ѹ�����ѹ��ʱ������
//@PRECONDITION:����Ŀ¼�������������Ҫ��δѹ��ԭ�ļ�original.bin���Լ�LZMAѹ��
//@PRECONDITION:�ļ�lzma.lz���Լ�gzipѹ���ļ�gzip.gz �����Կ����ڴ治������18M
//@INPUT:
//@EXPECTATION:�����ѹ��ʱ��ͳ��
//@EXECUTIONFLOW:1.����ѹ���ļ����ڴ�
//@EXECUTIONFLOW:2.��ѹ���ļ����ڴ�
//@EXECUTIONFLOW:3.У������
//@EXECUTIONFLOW:4.ʱ��ͳ���Լ����������Դ
CSUDI_BOOL CSTC_PERFORMANCE_MT_0001(void)
{
	CSUDI_HANDLE	pfOriginal = NULL;
	CSUDI_HANDLE	pfLZMA = NULL;
	CSUDI_HANDLE	pfGzip = NULL;

	char 	*pSrc = NULL,*pDest = NULL;
	int 		nFileLen ;
	CSUDI_BOOL	bRet;
	unsigned int		dwStart = 0,dwCostLZMA = 0,dwCostGzip = 0;


#define CSCLOSEFILE(fp)	\
	if ((fp) != NULL)  \
	{ \
		CSTKP_FClose(fp); \
	}
	

#define CSFREEMEM(p)   \
	if ((p) != NULL) \
	{ \
		CSUDIOSFree(p); \
		p = NULL; \
	}

	CSTCPrint("����ƽ̨��5M�ļ��ֱ���LZMA��Gzipѹ��������ݼ�ѹ������Ҫ��ʱ��\n");
	CSTCPrint("��ȷ����ǰĿ¼�´��ڲ����ļ�original.bin lzma.lz gzip.gz \n");

	
	pfOriginal = CSTKP_FOpen("original.bin","r");
	pfLZMA = CSTKP_FOpen("lzma.lz","r");
	pfGzip = CSTKP_FOpen("gzip.gz","r");


	if (pfOriginal == NULL || pfLZMA == NULL || pfGzip == NULL)
	{
		CSTRACE(ERROR_LEVEL,"�򿪲��������ļ�ʧ��\n");
		return CSUDI_FALSE;
	}

	nFileLen = CSGetFileSize(pfLZMA);

	CSTCPrint("lzma file length:%d\n",nFileLen);
	
	if (nFileLen > 0)
	{
		int nLen;
		pSrc = CSUDIOSMalloc(nFileLen);
		pDest = CSUDIOSMalloc(ORIGINAL_FILE_SIZE+1024);

		if (pSrc == NULL || pDest == NULL)
		{
			bRet = CSUDI_FALSE;
			//PORTINGTEST_GOTO((pSrc != NULL && pDest != NULL),done,"�����ڴ�ʧ��");
			CSTK_ASSERT_TRUE_FATAL((pSrc != NULL && pDest != NULL), "�����ڴ�ʧ��");
		}

		nLen = CSTKP_FRead(pSrc,1,nFileLen,pfLZMA);

		bRet = (nLen == nFileLen);
		//PORTINGTEST_GOTO((nLen == nFileLen),done,"��ȡָ�������ļ�ʧ��");
		CSTK_ASSERT_TRUE_FATAL((nLen == nFileLen), "��ȡָ�������ļ�ʧ��");
		dwStart = CSUDIPlusOSGetTickCount();
		nLen = BLUnLzma((unsigned char*)pDest,ORIGINAL_FILE_SIZE+1024,(unsigned char*)pSrc,nFileLen);
		dwCostLZMA = CSUDIPlusOSGetTickCount() - dwStart;
		bRet = (nLen > 0);
		//PORTINGTEST_GOTO((nLen > 0),done,"LZMA��ѹ������");
		CSTK_ASSERT_TRUE_FATAL((nLen > 0), "LZMA��ѹ������");
		CSFREEMEM(pSrc)
		bRet = CSVerifyDecompressData(pfOriginal,pDest,nLen);
		//PORTINGTEST_GOTO(bRet,done,"LZMAУ��ʧ��");
		CSTK_ASSERT_TRUE_FATAL((bRet), "LZMAУ��ʧ��");
		CSFREEMEM(pDest)
	}
	else
	{
		bRet = CSUDI_FALSE;
		//PORTINGTEST_GOTO((nFileLen > 0),done,"��ȡLZMA ѹ���ļ�����ʧ��");
		CSTK_ASSERT_TRUE_FATAL((nFileLen > 0), "��ȡLZMA ѹ���ļ�����ʧ��");
	}

	nFileLen = CSGetFileSize(pfGzip);

	CSTCPrint("gzip file length:%d\n",nFileLen);
	
	if (nFileLen > 0)
	{
		int nLen;
		pSrc = CSUDIOSMalloc(nFileLen);
		pDest = CSUDIOSMalloc(ORIGINAL_FILE_SIZE+1024);

		if (pSrc == NULL || pDest == NULL)
		{
			bRet = CSUDI_FALSE;
			//PORTINGTEST_GOTO((pSrc != NULL && pDest != NULL),done,"�����ڴ�ʧ��");
			CSTK_ASSERT_TRUE_FATAL((pSrc != NULL && pDest != NULL), "�����ڴ�ʧ��");
		}

		nLen =CSTKP_FRead(pSrc,1,nFileLen,pfGzip);
		
		bRet = (nLen == nFileLen);
		//PORTINGTEST_GOTO(nLen == nFileLen,done,"��ȡָ�������ļ�ʧ��");
		CSTK_ASSERT_TRUE_FATAL((nLen == nFileLen), "��ȡָ�������ļ�ʧ��");
		dwStart = CSUDIPlusOSGetTickCount();
		nLen = BLUnZip((unsigned char*)pDest,ORIGINAL_FILE_SIZE+1024,(unsigned char*)pSrc,nFileLen);
		dwCostGzip = CSUDIPlusOSGetTickCount() - dwStart;
		bRet = (nLen > 0);
		//PORTINGTEST_GOTO(nLen > 0,done,"Gzip��ѹ������");
		CSTK_ASSERT_TRUE_FATAL((nLen > 0), "Gzip��ѹ������");
		CSFREEMEM(pSrc)
		bRet = CSVerifyDecompressData(pfOriginal,pDest,nLen);
		//PORTINGTEST_GOTO(bRet,done,"GzipУ��ʧ��");
		CSTK_ASSERT_TRUE_FATAL((bRet), "GzipУ��ʧ��");
		CSFREEMEM(pDest)
	}
	else
	{
		bRet = CSUDI_FALSE;
		//PORTINGTEST_GOTO((nFileLen > 0),done,"��ȡLZMA ѹ���ļ�����ʧ��");
		CSTK_ASSERT_TRUE_FATAL((nFileLen > 0), "��ȡLZMA ѹ���ļ�����ʧ��");
	}

        CSTK_FATAL_POINT

	CSCLOSEFILE(pfOriginal)
	CSCLOSEFILE(pfLZMA)
	CSCLOSEFILE(pfGzip)

	CSFREEMEM(pSrc)
	CSFREEMEM(pDest)

	if (bRet)
	{
		CSTCPrint("lzma cost %d gzip cost:%d\n",dwCostLZMA,dwCostGzip);
	}
	return bRet;
}

//@CASEGROUP:CS_PERFORMANCE
//@DESCRIPTION:����1 M ��С���ڴ棬����ִ�� memset ��memcpy ��Ҫ��ʱ��
//@PRECONDITION:ϵͳ��ʼ���ɹ�
//@INPUT:��������
//@EXPECTATION:����TRUE
CSUDI_BOOL CSTC_PERFORMANCE_MT_0002(void)
{
	unsigned int 		dwTimeStart = 0;
	unsigned int 		dwMemsetCost,dwMemcpyCost;
	unsigned char 		*memBlock1;
	unsigned char 		*memBlock2;
	
   	int 			nCount = 0; 

#define TEST_MEM_SPEED_SIZE (1024*1024) /*�����ڴ��С*/
#define TEST_MEM_SPEED_MAX_COUNT  100 /*���Դ���*/

	CSTCPrint("����ƽ̨memset��memcpy 1M �ڴ��ʱ��\n");

	memBlock1 = (unsigned char*)CSUDIOSMalloc( TEST_MEM_SPEED_SIZE );

	if (memBlock1 == NULL) 
	{
		CSTCPrint(" CSUDIOSMalloc memBlock1 return Null!\n\n");
		CSTCPrint("[RESULT] Fail\n\n");
		return CSUDI_FALSE;
	}

	memBlock2 = (unsigned char*)CSUDIOSMalloc( TEST_MEM_SPEED_SIZE );

	if (memBlock2 == NULL)
	{
		CSTCPrint(" CSUDIOSMalloc memBlock2  return Null!\n\n");
		CSTCPrint("[RESULT] Fail\n\n");

		/*release memBlock1*/
		CSUDIOSFree(memBlock1);
		
		return CSUDI_FALSE;
	}

	CSTCPrint("*************************Test Memset Begin**********************\n\n");

	dwTimeStart = CSUDIPlusOSGetTickCount();
		
	for(nCount = 0;nCount < TEST_MEM_SPEED_MAX_COUNT; nCount++)
	{
		memset(memBlock1, 0, TEST_MEM_SPEED_SIZE);
	}

	dwMemsetCost = CSUDIPlusOSGetTickCount() - dwTimeStart;
	
	CSTCPrint("*************************Test Memset End************************\n\n");

	CSTCPrint("*************************Test Memcpy Begin**********************\n\n");

	dwTimeStart = CSUDIPlusOSGetTickCount();

	for(nCount = 0;nCount< TEST_MEM_SPEED_MAX_COUNT; nCount++)
	{
		memcpy(memBlock2, memBlock1, TEST_MEM_SPEED_SIZE);
	}	

	dwMemcpyCost = CSUDIPlusOSGetTickCount() - dwTimeStart;
	
	CSTCPrint("*************************Test Memcpy End************************\n\n");

	CSTCPrint("%s: memset 1M %d times cost time is %d ms\n\n",__FUNCTION__,TEST_MEM_SPEED_MAX_COUNT,dwMemsetCost);
	CSTCPrint("%s: memcpy 1M %d times cost time is %d ms\n\n",__FUNCTION__,TEST_MEM_SPEED_MAX_COUNT,dwMemcpyCost);

	CSTCPrint("%s: memset 1M one time cost time is %d ms\n\n",__FUNCTION__,dwMemsetCost/TEST_MEM_SPEED_MAX_COUNT);
	CSTCPrint("%s: memcpy 1M one time cost time is %d ms\n\n",__FUNCTION__,dwMemcpyCost/TEST_MEM_SPEED_MAX_COUNT);

	CSUDIOSFree(memBlock1);
	CSUDIOSFree(memBlock2);
	
	return CSUDI_TRUE;
	
}


/*���� = 2 + 2/3 + 2/3*2/5 + 2/3*2/5*3/7 + ...*/
static double  CalculatePI()
{
	double x=2;
	double z=2;
	int a=1;
	int b=3;

	while(z>1e-15)
	{
		z = z*a/b;
		x += z;
		a++;
		b+=2;
	}
	return x;
}
/*����PI,��ȷ��С����100*/
/*���ھ����ϵ�����,Ŀǰ����PI��ֵ����15λ���ϵĶ���
ʹ������ֵ���㷨,û��ʹ�ø��������㷨*/
static void CalculatePI_Very_long()
{
	#define  ARRSIZE (1010) /*���������С����ʾλ��*/
	char x[ARRSIZE], z[ARRSIZE]; /*x[0] x[1] . x[2] x[3] x[4] .... x[ARRSIZE-1]*/
	
	int a=1;
	int b=3;
	int c =0;
	int d =0; 
	int Run=1;
	int Cnt=0;
	int i =0;
	
	memset(x,0,ARRSIZE);
	memset(z,0,ARRSIZE);

	x[1] = 2;
	z[1] = 2;

	while(Run && (++Cnt< 200000000))
	{
		d = 0;

		for(i=ARRSIZE-1; i>0; i--)
		{
			c = z[i]*a + d;
			z[i] = c % 10;
			d = c / 10;
		}
		
		d = 0;
		for(i=0; i< ARRSIZE; i++)
		{
			c = z[i]+d*10;
			z[i] = c / b;
			d = c % b;
		}
		
		Run = 0;
		for(i=ARRSIZE-1; i>0; i--)
		{
			c = x[i] + z[i];
			x[i] = c%10;
			x[i-1] += c/10;
			Run |= z[i];
		}
		a++;
		b+=2;
	}
}

//@CASEGROUP:CS_PERFORMANCE
//@DESCRIPTION:������double�ͼ���1000��PI��ʱ�����ĺ�������ģ��
//@DESCRIPTION:�������PI��ȷ��С�����100λ��ʱ�����ġ���32λ��
//@DESCRIPTION:��float�ľ�����С�����8λ,double��С�����15λ
//@PRECONDITION:ϵͳ��ʼ���ɹ�
//@INPUT:��
//@EXPECTATION:����TRUE
CSUDI_BOOL CSTC_PERFORMANCE_MT_0003(void)
{
	unsigned int 		dwValueStart = 0;
	unsigned int 		dwDoubleCost,dwLongCost;
	
	unsigned int 		i = 0;
	double 		PI = 0;
	
	dwValueStart = CSUDIPlusOSGetTickCount();
	for(; i<1000; i++)
	{
		PI = CalculatePI();
	}
	dwDoubleCost = CSUDIPlusOSGetTickCount() - dwValueStart;
	
	/*CSTCPrint("PI=%.15f   \r\n",PI);*/

	dwValueStart = CSUDIPlusOSGetTickCount();
	CalculatePI_Very_long();
	dwLongCost = CSUDIPlusOSGetTickCount() - dwValueStart;
	
	CSTCPrint("%s:�ø���������1000 ��PI ����ʱ��:%d  ms\r\n",__FUNCTION__,dwDoubleCost);
	CSTCPrint("%s:��������ģ�����PI��ȷ��С����100λ����ʱ��:%d  ms\r\n",__FUNCTION__,dwLongCost);

	return CSUDI_TRUE;
}

#include "csusp_crc_ut.h"
//@CASEGROUP:CS_PERFORMANCE
//@DESCRIPTION:����ƽ̨��MPEGCRC32�㷨����5M�ռ��CRCֵ��ʱ������
//@PRECONDITION:ϵͳ��ʼ���ɹ�
//@INPUT:��
//@EXPECTATION:����TRUE
CSUDI_BOOL CSTC_PERFORMANCE_MT_0004(void)
{
	char 	*pData ;
	unsigned int	dwStart,dwCRCCost;
	int		i;
	int		*pTmp;
	unsigned int	dwCRC;

	#define CRC_DATA_LENGTH	(1024*1024*5)

	CSTCPrint("����ƽ̨��MPEGCRC32�㷨����5M�ռ��CRCֵ��ʱ������\n");

	pData = CSUDIOSMalloc(CRC_DATA_LENGTH);

	if (pData == NULL)
	{
		CSTCPrint("%f �����ڴ�ʧ��%s\n",__FUNCTION__,__LINE__);
		return CSUDI_FALSE;
	}

	pTmp = (int *)pData;
	for(i=0;i<CRC_DATA_LENGTH/4;i++)
	{
		*pTmp++ = i;
	}
	
	dwStart =  CSUDIPlusOSGetTickCount();
	dwCRC = CSUSPMPEGCRC32_UT(pData,CRC_DATA_LENGTH);
	dwCRCCost = CSUDIPlusOSGetTickCount() - dwStart;

	CSTCPrint("����õ���CRC :0x%x\n",dwCRC);
	CSTCPrint("��MPEGCRC32�㷨����5M�ռ��CRCʱ������:%d ms\n",dwCRCCost);

	CSUDIOSFree(pData);
	
	return CSUDI_TRUE;
}

