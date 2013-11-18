/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_descramble.h"
#include "udi2_demux.h"
#include "udi2_section.h"
#include "udi2_player.h"
#include "udi2_video.h"
#include "udi2_audio.h"
#include "udi2_os.h"
#include "udi2_tuner.h"
#include "udiplus_debug.h"
#include "cs_descrambletestcase.h"
#include "../cs_udi2testcase.h"

CSUDI_BOOL CSTC_DESCRAMBLE_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_DESCRAMBLE_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}


//Ѱ����Ƶ������
static int FindVideoDecoder(int nVideoDecCount, CSUDIVIDStreamType_E eFindVideoType)
{
	CSUDIVIDEOCapability_S sVideoCapabilityInfo;
	int nVideoID = -1;
	int i = 0;
	int k = 0;
	
	for (i=0; i<nVideoDecCount; i++)
	{
		if (!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(i, &sVideoCapabilityInfo)))
		{
			for (k=0; k<EM_UDI_VID_STREAMTYPE_NUM; k++)
	              {
				if (eFindVideoType == sVideoCapabilityInfo.m_eStreamType[k])
				{
					nVideoID = i;
					break;
				}
				if (EM_UDI_VID_STREAM_UNKNOWN == sVideoCapabilityInfo.m_eStreamType[k])
				{
					break;
				}
			}
				  
			if (nVideoID != -1)
			{
				break;
			}
		}
	}
	return nVideoID;
}

//Ѱ����Ƶ������
static int FindAudioDecoder(int nAudioDecCount, CSUDIAUDStreamType_E eFindAudioType)
{
	CSUDIAUDIOCapability_S sAudioCapabilityInfo;
	int nAudioID = -1;
	int i = 0;
	int k = 0;

	for (i=0; i<nAudioDecCount; i++)
	{
		if (!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo)))
		{
			for (k=0; k< EM_UDI_AUD_STREAMTYPE_NUM; k++ )
			{
				if (eFindAudioType == sAudioCapabilityInfo.m_sSupportDecode[k]) 
				{
					nAudioID = i;
					break;

				}
				if (EM_UDI_AUD_STREAM_UNKNOWN == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
			}
				  
			if (nAudioID != -1)
			{
				break;
			}
		}
	}
       return nAudioID;
}

//�ص�����
static void CSUDISECTIONCallback( CSUDISECTIONRequestType_E eType, void * pvParam , void * pvUserData)
{
	(void)(eType);
	(void)(pvParam);
	(void)(pvUserData);
}


//Ϊ����PIDΪ0x100��section ����һ��ͨ��
static BOOL ForStartSection( CSUDI_HANDLE *phFilter,int *pnDemuxIndex)
{
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDISECTIONRequestInfo_S stSetction;
	CSUDI_Error_Code enRetCode;
	int nDemuxCount = 0 ; 
	int i = 0;
	CSUDI_BOOL bRet = FALSE;
	CSUDI_BOOL bFindedFilter = FALSE;

	if (phFilter == CSUDI_NULL || pnDemuxIndex == CSUDI_NULL)
	{
		return FALSE;
	}

	*phFilter = CSUDI_NULL;

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));

	if (!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)))
	{
		if (!CSASSERT_FAILED(nDemuxCount > 0))
		{
			for (i=0; i<nDemuxCount; i++)
			{
				if (!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)))
				{
					if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
					{
						break;
					}
				}
			}
			
			if (i == nDemuxCount)
			{
				return FALSE;
			}
			else
			{
				bFindedFilter = TRUE;
			}
		}
	}

	if (bFindedFilter)
	{
		*pnDemuxIndex = i;
		stSetction.m_nDemuxID = i;

		enRetCode = CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle);
		if ((enRetCode == CSUDI_SUCCESS) && (CSUDI_NULL != hFilterHandle))
		{
			*phFilter = hFilterHandle;
			enRetCode = CSUDIFILTERStart(hFilterHandle);
			if (enRetCode == CSUDI_SUCCESS)
			{
				bRet = TRUE;
			}
			else
			{
				CSTCPrint("[%s, %d] filter start failed, errorcode=0x%x\n", __FUNCTION__, __LINE__, enRetCode);
			}
		}
		else
		{
			CSTCPrint("[%s, %d] filter allocate failed, errorcode=0x%x\n", __FUNCTION__, __LINE__, enRetCode);
		}
	}

	return bRet;
}


//@CASEGROUP:CSUDIDSMAllocate 
//@DESCRIPTION:���Խӿڵ�2������handleֵ���յķǷ��������
//@PRECONDITION:
//@INPUT:1��demuxID = 0
//@INPUT:2�����handle��ַ= NULL
//@EXPECTATION:����CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:����CSUDIDSMAllocate(0,NULL)����������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0001( void )
{
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDIDSMAllocate(0, CSUDI_NULL);

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == bResult),"�����жϴ���");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	 
}

//@CASEGROUP:CSUDIDSMAllocate 
//@DESCRIPTION:���Խӿڵ�1������demuxIDΪ-1�Ƿ�ֵ�����
//@PRECONDITION:
//@INPUT:1.  demuxID = -1
//@INPUT:2.  ���handle��ַ= �Ϸ���ַ
//@EXPECTATION:����CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:����CSUDIDSMAllocate(-1,&hDescramble),��������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0002( void )
{
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDIDSMAllocate(-1, CSUDI_NULL);

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == bResult),"�����жϴ���");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	 
}

//@CASEGROUP:CSUDIDSMAllocate
//@DESCRIPTION:���Խӿڵ�1������demuxID����demux�����ķǷ����
//@PRECONDITION:CSUDIDEMUXGetCount��ȡ����ȷ��demux����
//@INPUT:1.  demuxID= CSUDIDEMUXGetCount();
//@INPUT:2.  ���handle��ַ= �Ϸ���ַ
//@EXPECTATION:���ش���ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCount��ȡƽ̨demux����
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate(ƽ̨demux����,�Ϸ�handle��ַ),�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	int nDemuxCount = 0;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),"���Demux����ʧ��");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux����������0");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIDSMAllocate(nDemuxCount, &hDescramble)),"��Ӧ�÷���ɹ�");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@DESCRIPTION:������֧��descramble��demux���������������
//@PRECONDITION:
//@INPUT:1.  demuxID= ȫ���Ϸ���Demux Index
//@INPUT:2.  ���handle��ַ= �Ϸ���ַ
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1.CSUDIDEMUXGetCount��ȡdemux����
//@EXECUTIONFLOW:2.����CSUDIDEMUXGetCapability���λ�ȡdemux������
//@EXECUTIONFLOW:3.�����demux֧�ֵĽ�����������0,�����CSUDIDSMAllocate�ڸ�demux�Ϸ��������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.�������descramble�ɹ������CSUDIDSMFree�ͷŷ����descramble
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0004( void )
{
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_HANDLE hDescramble = CSUDI_NULL; 
	int nDemuxCount = 0;
	int nSumIndex = 0;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),"���Demux����ʧ��");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux����������0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i,&sCapabilityInfo)),"���demux����ʧ��");

		if (sCapabilityInfo.m_nkeyNum > 0) 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hDescramble), "����ͨ��ʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");

			nSumIndex++;
		}
	}

	if (0 == nSumIndex)
	{
		CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"û���ҵ�����������0��demux\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@DESCRIPTION:������֧��descramble��demux���������䵽ָ�������Ľ�����
//@PRECONDITION:
//@INPUT:1.  demuxID= ȫ���Ϸ���Demux Index
//@INPUT:2.  ���handle��ַ= �Ϸ���ַ
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1.CSUDIDEMUXGetCount��ȡdemux����
//@EXECUTIONFLOW:2.����CSUDIDEMUXGetCapability���λ�ȡdemux������
//@EXECUTIONFLOW:3.�����demux֧�ֵĽ�����������N,��ѭ������CSUDIDSMAllocate�ڸ�demux�Ϸ���N��������,����ÿ�ζ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMFree��ѭ������demux����õĽ���ͨ���ͷ�
//@EXECUTIONFLOW:5.ѭ��(����3~4)demux������
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0005( void )
{
	CSUDIDEMUXCapability_S	sCapabilityInfo;
	CSUDI_HANDLE hDescramble = CSUDI_NULL; 
	int nDemuxCount = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 0;

	#define DSM_HANDLE_CNT (1024)
	CSUDI_HANDLE hDsmHandle[DSM_HANDLE_CNT];

	memset(hDsmHandle, 0, sizeof(hDsmHandle));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),"���Demux����ʧ��");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux����������0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i,&sCapabilityInfo)),"���demux����ʧ��");

		if(sCapabilityInfo.m_nkeyNum <= 0)
		{
			continue;
		}

        for (j = 0; j < sCapabilityInfo.m_nkeyNum; j++) 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hDescramble), "����ͨ��ʧ��\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

			hDsmHandle[k++] = hDescramble;
		}

        for(n = 0; n < k; n++)
        {
            if(hDsmHandle[n] != NULL)
            {
                CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIDSMFree(hDsmHandle[n]), "�ͷ�ͨ��ʧ��\n");

                hDsmHandle[n] = NULL;
            }
        }
	}

	if (0 == k)
	{
		CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"û���ҵ�����������0��demux\n");
	}

	CSTK_FATAL_POINT;

	for(n=0; n<k; n++)
	{
		if(hDsmHandle[n] != NULL)
		{
			CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIDSMFree(hDsmHandle[n]), "�ͷ�ͨ��ʧ��\n");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:�����ͷŵ�handleΪ�Ƿ�handle(NULL)�����
//@PRECONDITION:
//@INPUT:����handle = NULL
//@EXPECTATION:���ش���ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:����CSUDIDSMFree(NULL),��������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMFree_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMFree(CSUDI_NULL)), "�����жϴ���");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:�����ͷŵ�handleΪCSUDIDSMAllocate����ĺϷ�������������
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����
//@INPUT:1.  ����handle = CSUDIDSMAllocate�����handle
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCapability��ȡ��������������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1��ȡ����demux�Ϸ���һ��descramble,�����ɹ�
//@EXECUTIONFLOW:3.����CSUDIDSMFree�ͷŷ���õ���descramble ���,��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMFree_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int nDemuxCount = 0 ;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "���Demux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux����������0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "���demux����ʧ��\n");

		if (sCapabilityInfo.m_nkeyNum > 0)
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"û���ҵ�����������0��demux\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hDescramble), "����ͨ��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMSetPID
//@DESCRIPTION:���Դ����handleΪ�Ƿ�handle(NULL)�����
//@PRECONDITION:
//@INPUT:1.  ����handle = NULL
//@INPUT:2.  pid = 0x100
//@EXPECTATION:���ش���(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:����CSUDIDSMSetPID(NULL,0x100),��������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_SMSetPID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetPID(CSUDI_NULL, 0x100)), "�����жϴ���");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetPID
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:���Դ����PIDΪ�Ƿ�ֵ0x1fff�����
//@PRECONDITION:
//@INPUT:1.  ����handle = CSUDIDSMAllocate����ĺϷ����
//@INPUT:2.  pid = 0x1fff
//@EXPECTATION:���ش���CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��
//@EXECUTIONFLOW:1.1 ����CSUDIDEMUXGetCapability��ȡ�����������ͽ���������������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:1.2 ����CSUDIFILTERAllocate����һ������PID��section������
//@EXECUTIONFLOW:1.3 ����CSUDIFILTERStart������section
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����һ��descramble
//@EXECUTIONFLOW:3.����CSUDIDSMSetPID����Ƿ�PID 0x1fff,��������CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.����CSUDIDSMFree�ͷž��
//@EXECUTIONFLOW:5.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_SMSetPID_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble), "����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetPID(hDescramble, 0x1fff), "����3ʧ�� \n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetPID
//@CASEGROUP:CSUDIDSMClearPID
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:�������в������Ϸ����������
//@PRECONDITION:CSUDIDSMAllocate����ɹ�
//@INPUT:1.  ����handle = CSUDIDSMAllocate����ĺϷ����
//@INPUT:2.  pid = 0x100
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����һ��descramble
//@EXECUTIONFLOW:3.����CSUDIDSMSetPID,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMClearPID���descramble������PID
//@EXECUTIONFLOW:5.����CSUDIDSMFree�ͷŽ��������
//@EXECUTIONFLOW:6.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_SMSetPID_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ�� \n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMClearPID
//@DESCRIPTION:���Դ����handleΪ�Ƿ�handle(NULL)�����
//@PRECONDITION:
//@INPUT:1.  ����handle = NULL
//@INPUT:2. 0x100 
//@EXPECTATION:���ش���(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:����CSUDIDSMClearPID(NULL,0x100),��������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_SMClearPID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMClearPID(CSUDI_NULL, 0x100)),"�����жϴ���");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMClearPID
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:���Դ����PIDΪ�Ƿ�ֵ0x1fff�����
//@PRECONDITION:CSUDIDSMAllocate����ɹ�
//@INPUT:1.  ����handle = CSUDIDSMAllocate�����descramble
//@INPUT:2. 0x1fff 
//@EXPECTATION:���ش���(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����descramble
//@EXECUTIONFLOW:3.����CSUDIDSMSetPID������������PID:0x100
//@EXECUTIONFLOW:4.����CSUDIDSMClearPID����0x1fff,��������CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.����CSUDIDSMFree�ͷ�descramble
//@EXECUTIONFLOW:6.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_SMClearPID_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMClearPID(hDescramble, 0x1fff), "����4ִ��ʧ�� \n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMSetOddKey
//@DESCRIPTION:���Դ����handleΪ�Ƿ�handle(NULL)�����
//@PRECONDITION:
//@INPUT:1.  ����handle = NULL
//@INPUT:2. pucOddKey = 8 Bytes�������׵�ַ
//@INPUT:3. ucOddLength = 8
//@EXPECTATION:���ش���(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:����CSUDIDSMSetOddKey,��������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetOddKey_0001( void )
{
	const unsigned char  scOddKey[8] = {0};

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetOddKey(CSUDI_NULL, scOddKey, 8)),"�����жϴ���");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetOddKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:����oddKeyֵָ��ΪNULL�����
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����descramble
//@INPUT:1.  ����handle = CSUDIDSMAllocate�ľ��
//@INPUT:2. pucOddKey = NULL
//@INPUT:3. ucOddLength = 8
//@EXPECTATION:���ش���CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����descramble
//@EXECUTIONFLOW:3. ����CSUDIDSMSetPID������������PID
//@EXECUTIONFLOW:4.����CSUDIDSMSetOddKey��2����������NULL,��������CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.����CSUDIDSMClearPID�������PID
//@EXECUTIONFLOW:6.����CSUDIDSMFree�ͷ�descramble
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetOddKey_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetOddKey(hDescramble, CSUDI_NULL, 8), "����4ʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetOddKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:����key������0���쳣�������
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����descramble
//@INPUT:1.  ����handle = CSUDIDSMAllocate�ľ��
//@INPUT:2. pucOddKey = 8 �ֽ������׵�ַ
//@INPUT:3. ucOddLength = 0
//@EXPECTATION:���ش���CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����descramble
//@EXECUTIONFLOW:3. ����CSUDIDSMSetPID������������PID
//@EXECUTIONFLOW:4.����CSUDIDSMSetOddKey��3����������0,��������CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.����CSUDIDSMFree�ͷ�descramble
//@EXECUTIONFLOW:6.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetOddKey_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	const unsigned char  scOddKey[8];
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetOddKey(hDescramble, scOddKey, 0), "����4ʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMSetEvenKey
//@DESCRIPTION:���Դ����handleΪ�Ƿ�handle(NULL)�����
//@PRECONDITION:
//@INPUT:1.  ����handle = NULL
//@INPUT:2. pucEvenKey = 8 bytes�������׵�ַ
//@INPUT:3. ucEvenLength = 8
//@EXPECTATION:���ش���(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:����CSUDIDSMSetEvenKey,��������CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetEvenKey_0001( void )
{
	const unsigned char  scEvenKey[8];

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetEvenKey(CSUDI_NULL, scEvenKey, 8)),"�����жϴ���");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetEvenKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:����oddKeyֵָ��ΪNULL�����
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����descramble
//@INPUT:1.  ����handle = CSUDIDSMAllocate�ľ��
//@INPUT:2. pucEvenKey = NULL
//@INPUT:3. ucEvenLength = 8
//@EXPECTATION:���ش���CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����descramble
//@EXECUTIONFLOW:3. ����CSUDIDSMSetPID������������PID
//@EXECUTIONFLOW:4.����CSUDIDSMSetEvenKey��2����������NULL,��������CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.����CSUDIDSMClearPID�������PID
//@EXECUTIONFLOW:6.����CSUDIDSMFree�ͷ�descramble
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetEvenKey_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetEvenKey(hDescramble, CSUDI_NULL, 0), "����4ʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetEvenKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:����key������0���쳣�������
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����descramble
//@INPUT:1.  ����handle = CSUDIDSMAllocate�ľ��
//@INPUT:2. pucEvenKey = 8 �ֽ������׵�ַ
//@INPUT:3. ucEvenLength = 0
//@EXPECTATION:���ش���CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����descramble
//@EXECUTIONFLOW:3.����CSUDIDSMSetPID������������PID
//@EXECUTIONFLOW:4.����CSUDIDSMSetEvenKey��3����������0,��������CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.����CSUDIDSMClearPID�������PID
//@EXECUTIONFLOW:6.����CSUDIDSMFree�ͷ�descramble
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetEvenKey_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	const unsigned char  scEvenKey[8];
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetEvenKey(hDescramble, scEvenKey, 0), "����4ʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetEvenKey CSUDIDSMSetOddKey
//@DESCRIPTION:����key���ȳ���8�ֽڵ����
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����descramble
//@INPUT:1.  ����handle = CSUDIDSMAllocate�ľ��
//@INPUT:2. pucEvenKey = 16 �ֽ������׵�ַ
//@INPUT:3. ucEvenLength = 16
//@EXPECTATION:CSUDIDSMSetEvenKey��CSUDIDSMSetOddKey������CSUDI_SUCCESS
//@EXECUTIONFLOW:1.����demux�ӿ�����PIDΪ0x100��section,�˲�����Ŀ��ֻ��Ϊ�˴�һ��PIDͨ��,������ο�CSTC_DSM_TEST_IT_SMSetPID_0002�Ĳ���1
//@EXECUTIONFLOW:2.����CSUDIDSMAllocate�ڲ���1�ϵ�demux����descramble
//@EXECUTIONFLOW:3.����CSUDIDSMSetPID������������PID
//@EXECUTIONFLOW:4.����CSUDIDSMSetEvenKey��3����������16,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMSetOddKey��3����������16,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.����CSUDIDSMClearPID�������PID
//@EXECUTIONFLOW:7.����CSUDIDSMFree�ͷ�descramble
//@EXECUTIONFLOW:8.����CSUDIFILTERFree�ͷŲ��������filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetKey_0001( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	unsigned char  scKey[16];
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //Ϊ����PIDΪ0x100��section ����һ��ͨ��
	CSTK_ASSERT_TRUE_FATAL(bRet,"����filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 16), "����16�ֽ�żkeyʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 16), "����16�ֽ���keyʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�ͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetPID
//@CASEGROUP:CSUDIDSMSetOddKey
//@CASEGROUP:CSUDIDSMSetEvenKey
//@CASEGROUP: CSUDIDSMClearPID 
//@CASEGROUP: CSUDIDSMFree
//@DESCRIPTION:1. ��������descrambleģ�鹦��������
//@DESCRIPTION:2. ���Խ��Ź����Ƿ�����
//@PRECONDITION:1.playerģ���ܹ���������
//@PRECONDITION:2.CSUDIAUDIOGetStatus����ȷ�Ļ�ȡ�ѽ������Ƶ֡��
//@PRECONDITION:3.CSUDIVIDEOGetStreamStatus����ȷ�Ļ�ȡ�ѽ������Ƶ֡��
//@INPUT:1. ��������
//@INPUT:2. ����������ӦKEY
//@INPUT:3. �����ӿ���������
//@EXPECTATION:1. descramble�����ӿ���������
//@EXPECTATION:2. descramble�ɹ�������PID��������
//@EXPECTATION:3. ��ĿKEY��Ӧ��������Ƶ���ųɹ�,����Ƶ֡��������
//@REMARK:���Թ����������޷�ʵʱ��֪����ǰKEY�����,����ֻ������һ��KEY,�ڲ��Թ���ֻҪ��ȡ����
//@REMARK:����Ƶ֡���������Ӽ�����Ϊ����ͨ����
//@EXECUTIONFLOW:1. ����CSUDIVIDEOGetCount��ȡvideo����������
//@EXECUTIONFLOW:2. ����CSUDIVIDEOGetCapability����Ѱ�ҵ�һ��֧��MPEG2��decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:3. ����CSUDIAUDIOGetCount��ȡaudio������������
//@EXECUTIONFLOW:4. ����CSUDIAUDIOGetCapability����Ѱ�ҵ�һ��֧��MPEG2��decoder,��������Ϊ��Ŀ���ŵ���Ƶ������
//@EXECUTIONFLOW:5. ����CSUDIDEMUXGetCount��ȡdemux������
//@EXECUTIONFLOW:6. ����CSUDIDEMUXGetCapability����Ѱ�ҵ�һ��֧��EM_UDI_DEMUX_PLAY���ҽ�������������0��demux���ڽ�Ŀ���ŵ�demux
//@EXECUTIONFLOW:7. ����������Ƶ��
//@EXECUTIONFLOW:8. ����CSUDIPLAYEROpen��һ�������ȡ�����豸�Ĳ�����
//@EXECUTIONFLOW:9. ����CSUDIPLAYERSetStream���ò��Խ�Ŀ������Ƶ���Ų���(PID������)
//@EXECUTIONFLOW:10. ����CSUDIPLAYERStart���Ų��Խ�Ŀ
//@EXECUTIONFLOW:11. ����CSUDIDSMAllocate��ѡ����demux�Ϸ���2��������,��������ƵPID
//@EXECUTIONFLOW:12. ����CSUDIDSMSetPID�ֱ��������Ƶ��PID
//@EXECUTIONFLOW:13. ����CSUDIDSMSetOddKey�ֱ���������Ƶ��oddKey
//@EXECUTIONFLOW:14. ����CSUDIDSMSetEvenKey�ֱ���������Ƶ��EvenKey
//@EXECUTIONFLOW:15. ����CSUDIAUDIOGetStatus��CSUDIVIDEOGetStreamStatus��ȡ����Ƶ����֡��
//@EXECUTIONFLOW:16. ÿ��40ms�ظ�����15���ظ�25��
//@EXECUTIONFLOW:17. ÿ��10S�ظ�����12��15,�ظ�6��,��������Ƶ��������������0
//@EXECUTIONFLOW:18. ���Խ���,����CSUDIDSMClearPID�ֱ��������ƵPID����
//@EXECUTIONFLOW:19. ����CSUDIDSMFree�ֱ��ͷ�����Ƶ��descramble
//@EXECUTIONFLOW:20. ����CSUDIPLAYERStopֹͣ���Խ�Ŀ����
//@EXECUTIONFLOW:21. ����CSUDIPLAYERClose�رղ����� 
CSUDI_BOOL CSTC_DSM_TEST_IT_Descramble_0001( void )
{
	CSUDI_HANDLE hAudioDescramble = CSUDI_NULL;
	CSUDI_HANDLE hVideoDescramble = CSUDI_NULL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIDEMUXCapability_S  sDemuxCapabilityInfo;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S scStreamInfo[2];
	CSUDIAUDIOStatus_S sStatus;
	CSUDIVIDEOStreamStatus_S sStreamStatus;
	int nVideoDecCount = 0;
	int nVideoIndex = -1;
	int nAudioDecCount = 0;
	int nAudioIndex = -1;
	int nDemuxCount = 0 ; 
	unsigned int nSumAudioFrameCount = 0;
	unsigned int nSumVideoFrameCount = 0;
	int i = 0;
	int j = 0;
	//����keyֵ
	unsigned char  scAudioOddKey[8] = {0x24 ,0x5e ,0x0d ,0x8f ,0x06 ,0xb7 ,0x47 ,0x04};
	unsigned char  scVideoOddKey[8] = {0x24 ,0x5e ,0x0d ,0x8f ,0x06 ,0xb7 ,0x47 ,0x04};

	unsigned char  scAudioEvenKey[8] = {0x29, 0x23, 0xbe, 0x0a, 0xe1, 0x6c, 0xd6, 0x23};									
	unsigned char  scVideoEvenKey[8] ={0x29, 0x23, 0xbe, 0x0a, 0xe1, 0x6c, 0xd6, 0x23};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoDecCount), "��ȡVideo����ʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoDecCount > 0), "û�п��õ���Ƶ������\n");

	nVideoIndex = FindVideoDecoder(nVideoDecCount, EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL((nVideoIndex != -1),"����2ʧ�ܣ�û��֧��MPEG2���͵���Ƶ������\n");
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount), "��ȡAudio����ʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((nAudioDecCount > 0), "û�п��õ���Ƶ������\n");

	nAudioIndex = FindAudioDecoder(nAudioDecCount, EM_UDI_AUD_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"����4ʧ�ܣ�û��֧��MPEG2���͵���Ƶ������\n");
	sPlayerChnl.m_nAudioDecoder = nAudioIndex;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "��ȡDemux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo), "��ȡDemux����ʧ��\n");
		if ((EM_UDI_DEMUX_PLAY == (sDemuxCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY)) && (sDemuxCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"û���ҵ�֧��EM_UDI_DEMUX_PLAY���ҽ�������������0��demux \n");
	sPlayerChnl.m_nDemux = i;

	CSTCPrint("��������Ĳ�����������Ϊ38M\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(0, "SmartcardInit_V2.3.mpg"), "��Ƶʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer),"�򿪲�����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�򿪲������ɹ������ؾ������Ϊ��\n");

	scStreamInfo[0].m_nPid = 529;
	scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	scStreamInfo[1].m_nPid = 528;
	scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL),"����9ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"���Ž�Ŀʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hAudioDescramble), "������Ƶͨ��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hAudioDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hVideoDescramble), "������Ƶͨ��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hVideoDescramble, "����ͨ���ɹ������ؾ������Ϊ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hAudioDescramble, 529), "ָ����ƵPID ʧ�� \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hVideoDescramble, 528), "ָ����ƵPID ʧ�� \n");

	memset (&sStreamStatus, 0, sizeof (CSUDIVIDEOStreamStatus_S));
	memset (&sStatus, 0, sizeof (CSUDIAUDIOStatus_S));

	for (i=0; i<6; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hAudioDescramble, scAudioEvenKey, 8), "����Audio����keyʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hVideoDescramble, scVideoEvenKey, 8), "����Video����keyʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hAudioDescramble, scAudioOddKey, 8), "����Audio��żkeyʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hVideoDescramble, scVideoOddKey, 8), "����Video��żkeyʧ��\n");

		CSTCPrint("����KEY����ȴ�10��...\n");
		CSUDIOSThreadSleep(10000);

		for (j=0; j<25; j++)
		{
			CSUDIOSThreadSleep(40);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex, &sStatus), "��ȡAudio״̬��Ϣʧ��\n");
			nSumAudioFrameCount += sStatus.m_uFrameCount;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStreamStatus(nVideoIndex, &sStreamStatus), "��ȡ������״̬��Ϣʧ��\n");
			nSumVideoFrameCount += sStreamStatus.m_dwFrameCount;
		}
	}

	CSTCPrint("[UDI2DESCRAMBLETEST]0:Yes,���ųɹ�������������Լ10���Ŀ\r\n");
	CSTCPrint("[UDI2DESCRAMBLETEST]1:No,����ʧ�ܣ����ܲ��Ž�Ŀ \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "����15ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(((nSumAudioFrameCount >0) && (nSumVideoFrameCount>0)), "����Ƶ����������������0 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hAudioDescramble, 529), "���ָ����ƵPID ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hVideoDescramble, 528), "���ָ����ƵPID ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ���Ž�Ŀʧ��\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hVideoDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hVideoDescramble), "�ͷ���Ƶͨ��ʧ��\n");
			hVideoDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hAudioDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hAudioDescramble), "�ͷ���Ƶͨ��ʧ��\n");
			hAudioDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:����descrambleģ��
//@DESCRIPTION:���Է���������� ������PID�����PID���ͷ�descramble
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����
//@EXPECTATION:�������롢����PID�����PID���ͷ�1000��,ÿ�ζ����سɹ�
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCapability��ȡ������������filter����������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:2.����CSUDIFILTERAllocate�ڲ���demux������һ��PID=0��filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����CSUDIFILTERStart������filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMAllocate�ڲ���demux�Ϸ���һ��descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMSetPID ����descramble�Ĺ���PIDΪ0,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMClearPID���������PID,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.����CSUDIDSMFree�ͷŲ���descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ���filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8.�ظ�����2~7  1000��
CSUDI_BOOL CSTC_DSM_TEST_IT_Descramble_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDISECTIONRequestInfo_S stSetction;
	unsigned char  scKey[8];
	int nDemuxCount = 0 ; 
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "��ȡDemux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "��ȡDemux����ʧ��\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(i<nDemuxCount,"����ͬʱ֧��filter��descramble��demuxʧ��\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = i;

	for (i=0;i<1000;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"����filterʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filterʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 8), "����8�ֽ���keyʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 8), "����8�ֽ�żkeyʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
		hFilterHandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:����descrambleģ��
//@DESCRIPTION:�����ڲ�����CSUDIDSMClearPIDֱ�ӵ���CSUDIDSMFree�����
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����
//@EXPECTATION:�������롢����PID���ͷ�descramble 1000��,ÿ�ζ����سɹ�
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCapability��ȡ������������filter����������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:2.����CSUDIFILTERAllocate�ڲ���demux������һ��PID=0��filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����CSUDIFILTERStart������filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMAllocate�ڲ���demux�Ϸ���һ��descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMSetPID ����descramble�Ĺ���PIDΪ0,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.������CSUDIDSMClearPID��ֱ�ӵ���CSUDIDSMFree�ͷŲ���descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ���filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8.�ظ�����2~7  1000��
CSUDI_BOOL CSTC_DSM_TEST_IT_Descramble_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDISECTIONRequestInfo_S stSetction;
	unsigned char  scKey[8];
	int nDemuxCount = 0 ; 
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "��ȡDemux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "��ȡDemux����ʧ��\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount), "����ͬʱ֧��filter��descramble��demuxʧ��\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = i;

	for (i=0;i<1000;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"����filterʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filterʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 8), "����8�ֽ���keyʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 8), "����8�ֽ�żkeyʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
		hFilterHandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:����descrambleģ��
//@DESCRIPTION:�����ȹر�descramble������PIDͨ��,Ȼ���ٵ���CSUDIDSMSetPID�������PID�Լ�����CSUDIDSMFree�ͷ�descramble
//@PRECONDITION:1.CSUDIDSMAllocate�ɹ�����
//@PRECONDITION:2.sectionģ���ܹ������ķ���filter����start
//@INPUT:1.  sectionģ��򿪵�PID=0x100��ͨ��
//@EXPECTATION:1. ����CSUDIFILTERAllocate��CSUDIFILTERStart�ܹ������ķ�������filter
//@EXPECTATION:2. �ȵ���CSUDIFILTERFree�ͷŵ�PIDͨ��,�ٵ���CSUDIDSMSetPID�������PID�Լ�����CSUDIDSMFree�ͷ�descramble������������CSUDI_SUCCESS
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCapability��ȡ������������filter����������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:2.����CSUDIFILTERAllocate�ڲ���demux������һ��PID=0x100��filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����CSUDIFILTERStart������filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMAllocate�ڲ���demux�Ϸ���һ��descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMSetPID ����descramble�Ĺ���PIDΪ0x100,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:10. ����CSUDIFILTERStopֹͣ����filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11.����CSUDIFILTERFree���ͷŲ���filter�Լ�PIDͨ��,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8. ����CSUDIDSMClearPID�������PID 0x100,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:9.����CSUDIDSMFree�ͷŲ���descramble,��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_DSM_TEST_IT_Descramble_0004( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDISECTIONRequestInfo_S stSetction;
	unsigned char  scKey[8];
	int nDemuxCount = 0 ; 
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "��ȡDemux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "��ȡDemux����ʧ��\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(i<nDemuxCount,"����ͬʱ֧��filter��descramble��demuxʧ��\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
		"����filterʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filterʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "ָ��PIDʧ�� \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 8), "����8�ֽ���keyʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 8), "����8�ֽ�żkeyʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStop(hFilterHandle),"ֹͣ�������� \n");
	//hFilterHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "�������PIDʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");		
	hDescramble = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
	hFilterHandle = CSUDI_NULL;

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:����descrambleģ��
//@DESCRIPTION:���Է���������� �����������ͬPID���ͷ�descramble
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����
//@EXPECTATION:�������롢����PID�����PID���ͷ�1000��,ÿ�ζ����سɹ�
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCapability��ȡ������������filter����������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:2.����CSUDIFILTERAllocate�ڲ���demux������һ���Ϸ�PID��filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����CSUDIFILTERStart������filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMAllocate�ڲ���demux�Ϸ���һ��descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMSetPID ����descramble�Ĺ���PIDΪ����filter��PID,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMClearPID���������PID,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.����CSUDIDSMFree�ͷŲ���descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ���filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8.�ظ�����2~7  1000��
CSUDI_BOOL CSTC_DSM_TEST_IT_Descramble_0005( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDISECTIONRequestInfo_S stSetction;
	int nDemuxCount = 0 ;
	int nCycCount = 0;
	int nPid = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "��ȡDemux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for (nDemuxIndex=0; nDemuxIndex<nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo), "��ȡDemux����ʧ��\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nDemuxIndex<nDemuxCount,"����ͬʱ֧��filter��descramble��demuxʧ��\n");

	

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;	
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = nDemuxIndex;

	for (nCycCount=0; nCycCount<1000; nCycCount++, nPid++)
	{
		stSetction.m_nPID = nPid;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"����filterʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filterʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, nPid), "ָ��PIDʧ�� \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, nPid), "�������PIDʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
		hFilterHandle = CSUDI_NULL;

		if(nCycCount%100 == 0)
		{
			CSTCPrint("test process:%d%%\r\n", nCycCount/10);
		}
		
	}

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:����descrambleģ��
//@DESCRIPTION:���Է���������� ��������ͬPID���ڲ����PID��������ͷ�descramble
//@PRECONDITION:CSUDIDSMAllocate�ɹ�����
//@EXPECTATION:�������롢����PID�����PID���ͷ�1000��,ÿ�ζ����سɹ�
//@EXECUTIONFLOW:1.����CSUDIDEMUXGetCapability��ȡ������������filter����������0��demux ID����Ϊ����ID
//@EXECUTIONFLOW:2.����CSUDIFILTERAllocate�ڲ���demux������һ���Ϸ�PID��filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3.����CSUDIFILTERStart������filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4.����CSUDIDSMAllocate�ڲ���demux�Ϸ���һ��descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5.����CSUDIDSMSetPID ����descramble�Ĺ���PIDΪ����filter��PID,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6.����CSUDIDSMFree�ͷŲ���descramble,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7.����CSUDIFILTERFree�ͷŲ���filter,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8.�ظ�����2~7  1000��
CSUDI_BOOL CSTC_DSM_TEST_IT_Descramble_0006( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDISECTIONRequestInfo_S stSetction;
	int nDemuxCount = 0 ;
	int nCycCount = 0;
	int nPid = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "��ȡDemux����ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for (nDemuxIndex=0; nDemuxIndex<nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo), "��ȡDemux����ʧ��\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nDemuxIndex<nDemuxCount,"����ͬʱ֧��filter��descramble��demuxʧ��\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;	
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = nDemuxIndex;
	
	for (nCycCount=0; nCycCount<1000; nCycCount++, nPid++)
	{
		stSetction.m_nPID = nPid;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"����filterʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filterʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"����descramble���ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, nPid), "ָ��PIDʧ�� \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
		hFilterHandle = CSUDI_NULL;
		
		if(nCycCount%100 == 0)
		{
			CSTCPrint("test process:%d%%\r\n", nCycCount/10);
		}
	}

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "�ͷ�descrambleͨ��ʧ��\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "�ͷ�Filter��Դʧ��\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

