/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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
	//在本测试用例集执行前调用
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_DESCRAMBLE_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}


//寻找视频解码器
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

//寻找音频解码器
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

//回调函数
static void CSUDISECTIONCallback( CSUDISECTIONRequestType_E eType, void * pvParam , void * pvUserData)
{
	(void)(eType);
	(void)(pvParam);
	(void)(pvUserData);
}


//为请求PID为0x100的section 分配一个通道
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
//@DESCRIPTION:测试接口第2个参数handle值传空的非法参数情况
//@PRECONDITION:
//@INPUT:1、demuxID = 0
//@INPUT:2、输出handle地址= NULL
//@EXPECTATION:返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:调用CSUDIDSMAllocate(0,NULL)，期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0001( void )
{
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDIDSMAllocate(0, CSUDI_NULL);

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == bResult),"参数判断错误");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	 
}

//@CASEGROUP:CSUDIDSMAllocate 
//@DESCRIPTION:测试接口第1个参数demuxID为-1非法值的情况
//@PRECONDITION:
//@INPUT:1.  demuxID = -1
//@INPUT:2.  输出handle地址= 合法地址
//@EXPECTATION:返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:调用CSUDIDSMAllocate(-1,&hDescramble),期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0002( void )
{
	CSUDI_Error_Code bResult = CSUDI_SUCCESS;

	bResult = CSUDIDSMAllocate(-1, CSUDI_NULL);

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == bResult),"参数判断错误");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;	 
}

//@CASEGROUP:CSUDIDSMAllocate
//@DESCRIPTION:测试接口第1个参数demuxID超出demux数量的非法情况
//@PRECONDITION:CSUDIDEMUXGetCount获取到正确的demux数量
//@INPUT:1.  demuxID= CSUDIDEMUXGetCount();
//@INPUT:2.  输出handle地址= 合法地址
//@EXPECTATION:返回错误值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCount获取平台demux数量
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate(平台demux数量,合法handle地址),期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	int nDemuxCount = 0;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),"获得Demux数量失败");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux数量不大于0");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIDSMAllocate(nDemuxCount, &hDescramble)),"不应该分配成功");

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@DESCRIPTION:在所有支持descramble的demux上正常分配解扰器
//@PRECONDITION:
//@INPUT:1.  demuxID= 全部合法的Demux Index
//@INPUT:2.  输出handle地址= 合法地址
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1.CSUDIDEMUXGetCount获取demux数量
//@EXECUTIONFLOW:2.调用CSUDIDEMUXGetCapability依次获取demux的能力
//@EXECUTIONFLOW:3.如果该demux支持的解扰数量大于0,则调用CSUDIDSMAllocate在该demux上分配解扰器,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.如果分配descramble成功则调用CSUDIDSMFree释放分配的descramble
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMAllocate_0004( void )
{
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_HANDLE hDescramble = CSUDI_NULL; 
	int nDemuxCount = 0;
	int nSumIndex = 0;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),"获得Demux数量失败");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux数量不大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i,&sCapabilityInfo)),"获得demux属性失败");

		if (sCapabilityInfo.m_nkeyNum > 0) 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hDescramble), "分配通道失败\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");

			nSumIndex++;
		}
	}

	if (0 == nSumIndex)
	{
		CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"没有找到解扰器大于0的demux\n");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@DESCRIPTION:在所有支持descramble的demux上正常分配到指定数量的解扰器
//@PRECONDITION:
//@INPUT:1.  demuxID= 全部合法的Demux Index
//@INPUT:2.  输出handle地址= 合法地址
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1.CSUDIDEMUXGetCount获取demux数量
//@EXECUTIONFLOW:2.调用CSUDIDEMUXGetCapability依次获取demux的能力
//@EXECUTIONFLOW:3.如果该demux支持的解扰数量等于N,则循环调用CSUDIDSMAllocate在该demux上分配N个解扰器,期望每次都返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMFree，循环将该demux分配好的解扰通道释放
//@EXECUTIONFLOW:5.循环(步骤3~4)demux个数次
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),"获得Demux数量失败");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux数量不大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i,&sCapabilityInfo)),"获得demux属性失败");

		if(sCapabilityInfo.m_nkeyNum <= 0)
		{
			continue;
		}

        for (j = 0; j < sCapabilityInfo.m_nkeyNum; j++) 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hDescramble), "分配通道失败\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

			hDsmHandle[k++] = hDescramble;
		}

        for(n = 0; n < k; n++)
        {
            if(hDsmHandle[n] != NULL)
            {
                CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIDSMFree(hDsmHandle[n]), "释放通道失败\n");

                hDsmHandle[n] = NULL;
            }
        }
	}

	if (0 == k)
	{
		CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"没有找到解扰器大于0的demux\n");
	}

	CSTK_FATAL_POINT;

	for(n=0; n<k; n++)
	{
		if(hDsmHandle[n] != NULL)
		{
			CSTK_ASSERT_TRUE(CSUDI_SUCCESS == CSUDIDSMFree(hDsmHandle[n]), "释放通道失败\n");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试释放的handle为非法handle(NULL)的情况
//@PRECONDITION:
//@INPUT:输入handle = NULL
//@EXPECTATION:返回错误值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:调用CSUDIDSMFree(NULL),期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMFree_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMFree(CSUDI_NULL)), "参数判断错误");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试释放的handle为CSUDIDSMAllocate分配的合法句柄的正常情况
//@PRECONDITION:CSUDIDSMAllocate成功分配
//@INPUT:1.  输入handle = CSUDIDSMAllocate分配的handle
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCapability获取解扰器数量大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1获取出的demux上分配一个descramble,期望成功
//@EXECUTIONFLOW:3.调用CSUDIDSMFree释放分配得到的descramble 句柄,期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMFree_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int nDemuxCount = 0 ;
	int i = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获得Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0),"demux数量不大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "获得demux属性失败\n");

		if (sCapabilityInfo.m_nkeyNum > 0)
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"没有找到解扰器大于0的demux\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hDescramble), "分配通道失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMSetPID
//@DESCRIPTION:测试传入的handle为非法handle(NULL)的情况
//@PRECONDITION:
//@INPUT:1.  输入handle = NULL
//@INPUT:2.  pid = 0x100
//@EXPECTATION:返回错误(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:调用CSUDIDSMSetPID(NULL,0x100),期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_SMSetPID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetPID(CSUDI_NULL, 0x100)), "参数判断错误");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetPID
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试传入的PID为非法值0x1fff的情况
//@PRECONDITION:
//@INPUT:1.  输入handle = CSUDIDSMAllocate分配的合法句柄
//@INPUT:2.  pid = 0x1fff
//@EXPECTATION:返回错误CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道
//@EXECUTIONFLOW:1.1 调用CSUDIDEMUXGetCapability获取过滤器数量和解扰器数量均大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:1.2 调用CSUDIFILTERAllocate分配一个测试PID的section过滤器
//@EXECUTIONFLOW:1.3 调用CSUDIFILTERStart启动该section
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配一个descramble
//@EXECUTIONFLOW:3.调用CSUDIDSMSetPID传入非法PID 0x1fff,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4.调用CSUDIDSMFree释放句柄
//@EXECUTIONFLOW:5.调用CSUDIFILTERFree释放测试申请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_SMSetPID_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble), "分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetPID(hDescramble, 0x1fff), "步骤3失败 \n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetPID
//@CASEGROUP:CSUDIDSMClearPID
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试所有参数均合法的正常情况
//@PRECONDITION:CSUDIDSMAllocate分配成功
//@INPUT:1.  输入handle = CSUDIDSMAllocate分配的合法句柄
//@INPUT:2.  pid = 0x100
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配一个descramble
//@EXECUTIONFLOW:3.调用CSUDIDSMSetPID,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMClearPID清除descramble关联的PID
//@EXECUTIONFLOW:5.调用CSUDIDSMFree释放解扰器句柄
//@EXECUTIONFLOW:6.调用CSUDIFILTERFree释放测试申请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_SMSetPID_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败 \n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMClearPID
//@DESCRIPTION:测试传入的handle为非法handle(NULL)的情况
//@PRECONDITION:
//@INPUT:1.  输入handle = NULL
//@INPUT:2. 0x100 
//@EXPECTATION:返回错误(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:调用CSUDIDSMClearPID(NULL,0x100),期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_SMClearPID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMClearPID(CSUDI_NULL, 0x100)),"参数判断错误");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMClearPID
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试传入的PID为非法值0x1fff的情况
//@PRECONDITION:CSUDIDSMAllocate分配成功
//@INPUT:1.  输入handle = CSUDIDSMAllocate分配的descramble
//@INPUT:2. 0x1fff 
//@EXPECTATION:返回错误(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配descramble
//@EXECUTIONFLOW:3.调用CSUDIDSMSetPID关联解扰器的PID:0x100
//@EXECUTIONFLOW:4.调用CSUDIDSMClearPID传入0x1fff,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.调用CSUDIDSMFree释放descramble
//@EXECUTIONFLOW:6.调用CSUDIFILTERFree释放测试声请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_SMClearPID_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMClearPID(hDescramble, 0x1fff), "步骤4执行失败 \n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMSetOddKey
//@DESCRIPTION:测试传入的handle为非法handle(NULL)的情况
//@PRECONDITION:
//@INPUT:1.  输入handle = NULL
//@INPUT:2. pucOddKey = 8 Bytes的数组首地址
//@INPUT:3. ucOddLength = 8
//@EXPECTATION:返回错误(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:调用CSUDIDSMSetOddKey,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetOddKey_0001( void )
{
	const unsigned char  scOddKey[8] = {0};

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetOddKey(CSUDI_NULL, scOddKey, 8)),"参数判断错误");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetOddKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试oddKey值指针为NULL的情况
//@PRECONDITION:CSUDIDSMAllocate成功分配descramble
//@INPUT:1.  输入handle = CSUDIDSMAllocate的句柄
//@INPUT:2. pucOddKey = NULL
//@INPUT:3. ucOddLength = 8
//@EXPECTATION:返回错误CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配descramble
//@EXECUTIONFLOW:3. 调用CSUDIDSMSetPID关联解扰器的PID
//@EXECUTIONFLOW:4.调用CSUDIDSMSetOddKey第2个参数传入NULL,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.调用CSUDIDSMClearPID清除关联PID
//@EXECUTIONFLOW:6.调用CSUDIDSMFree释放descramble
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试申请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetOddKey_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetOddKey(hDescramble, CSUDI_NULL, 8), "步骤4失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetOddKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试key长传入0的异常参数情况
//@PRECONDITION:CSUDIDSMAllocate成功分配descramble
//@INPUT:1.  输入handle = CSUDIDSMAllocate的句柄
//@INPUT:2. pucOddKey = 8 字节数组首地址
//@INPUT:3. ucOddLength = 0
//@EXPECTATION:返回错误CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配descramble
//@EXECUTIONFLOW:3. 调用CSUDIDSMSetPID关联解扰器的PID
//@EXECUTIONFLOW:4.调用CSUDIDSMSetOddKey第3个参数传入0,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.调用CSUDIDSMFree释放descramble
//@EXECUTIONFLOW:6.调用CSUDIFILTERFree释放测试声请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetOddKey_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	const unsigned char  scOddKey[8];
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetOddKey(hDescramble, scOddKey, 0), "步骤4失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMSetEvenKey
//@DESCRIPTION:测试传入的handle为非法handle(NULL)的情况
//@PRECONDITION:
//@INPUT:1.  输入handle = NULL
//@INPUT:2. pucEvenKey = 8 bytes的数组首地址
//@INPUT:3. ucEvenLength = 8
//@EXPECTATION:返回错误(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:调用CSUDIDSMSetEvenKey,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetEvenKey_0001( void )
{
	const unsigned char  scEvenKey[8];

	CSTK_ASSERT_TRUE_FATAL((CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetEvenKey(CSUDI_NULL, scEvenKey, 8)),"参数判断错误");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetEvenKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试oddKey值指针为NULL的情况
//@PRECONDITION:CSUDIDSMAllocate成功分配descramble
//@INPUT:1.  输入handle = CSUDIDSMAllocate的句柄
//@INPUT:2. pucEvenKey = NULL
//@INPUT:3. ucEvenLength = 8
//@EXPECTATION:返回错误CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配descramble
//@EXECUTIONFLOW:3. 调用CSUDIDSMSetPID关联解扰器的PID
//@EXECUTIONFLOW:4.调用CSUDIDSMSetEvenKey第2个参数传入NULL,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.调用CSUDIDSMClearPID清除关联PID
//@EXECUTIONFLOW:6.调用CSUDIDSMFree释放descramble
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试申请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetEvenKey_0002( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetEvenKey(hDescramble, CSUDI_NULL, 0), "步骤4失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetEvenKey
//@CASEGROUP:CSUDIDSMFree
//@DESCRIPTION:测试key长传入0的异常参数情况
//@PRECONDITION:CSUDIDSMAllocate成功分配descramble
//@INPUT:1.  输入handle = CSUDIDSMAllocate的句柄
//@INPUT:2. pucEvenKey = 8 字节数组首地址
//@INPUT:3. ucEvenLength = 0
//@EXPECTATION:返回错误CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配descramble
//@EXECUTIONFLOW:3.调用CSUDIDSMSetPID关联解扰器的PID
//@EXECUTIONFLOW:4.调用CSUDIDSMSetEvenKey第3个参数传入0,期望返回CSUDIDSM_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5.调用CSUDIDSMClearPID清除关联PID
//@EXECUTIONFLOW:6.调用CSUDIDSMFree释放descramble
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试申请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetEvenKey_0003( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	const unsigned char  scEvenKey[8];
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDIDSM_ERROR_BAD_PARAMETER == CSUDIDSMSetEvenKey(hDescramble, scEvenKey, 0), "步骤4失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIDSMAllocate
//@CASEGROUP:CSUDIDSMSetEvenKey CSUDIDSMSetOddKey
//@DESCRIPTION:测试key长度超过8字节的情况
//@PRECONDITION:CSUDIDSMAllocate成功分配descramble
//@INPUT:1.  输入handle = CSUDIDSMAllocate的句柄
//@INPUT:2. pucEvenKey = 16 字节数组首地址
//@INPUT:3. ucEvenLength = 16
//@EXPECTATION:CSUDIDSMSetEvenKey和CSUDIDSMSetOddKey都返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1.调用demux接口请求PID为0x100的section,此操作的目的只是为了打开一个PID通道,详情请参考CSTC_DSM_TEST_IT_SMSetPID_0002的步骤1
//@EXECUTIONFLOW:2.调用CSUDIDSMAllocate在步骤1上的demux分配descramble
//@EXECUTIONFLOW:3.调用CSUDIDSMSetPID关联解扰器的PID
//@EXECUTIONFLOW:4.调用CSUDIDSMSetEvenKey第3个参数传入16,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMSetOddKey第3个参数传入16,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6.调用CSUDIDSMClearPID清除关联PID
//@EXECUTIONFLOW:7.调用CSUDIDSMFree释放descramble
//@EXECUTIONFLOW:8.调用CSUDIFILTERFree释放测试申请的filter
CSUDI_BOOL CSTC_DSM_TEST_IT_DSMSetKey_0001( void )
{
	CSUDI_HANDLE hDescramble = CSUDI_NULL;
	CSUDI_HANDLE hFilterHandle = CSUDI_NULL;
	unsigned char  scKey[16];
	int nDemuxIndex = 0;
	BOOL bRet;

	bRet = ForStartSection(&hFilterHandle,&nDemuxIndex);  //为请求PID为0x100的section 分配一个通道
	CSTK_ASSERT_TRUE_FATAL(bRet,"分配filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 16), "设置16字节偶key失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 16), "设置16字节奇key失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
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
//@DESCRIPTION:1. 测试整个descramble模块功能与流程
//@DESCRIPTION:2. 测试解扰功能是否正常
//@PRECONDITION:1.player模块能够正常播放
//@PRECONDITION:2.CSUDIAUDIOGetStatus能正确的获取已解码的音频帧数
//@PRECONDITION:3.CSUDIVIDEOGetStreamStatus能正确的获取已解码的视频帧数
//@INPUT:1. 加扰码流
//@INPUT:2. 加扰码流对应KEY
//@INPUT:3. 各个接口正常参数
//@EXPECTATION:1. descramble各个接口正常返回
//@EXPECTATION:2. descramble成功分配且PID关联正常
//@EXPECTATION:3. 节目KEY对应部分音视频解扰成功,音视频帧数有增加
//@REMARK:测试过程中由于无法实时的知道当前KEY的情况,所以只设置了一个KEY,在测试过程只要获取出的
//@REMARK:音视频帧数均有增加即可认为测试通过。
//@EXECUTIONFLOW:1. 调用CSUDIVIDEOGetCount获取video解码器数量
//@EXECUTIONFLOW:2. 调用CSUDIVIDEOGetCapability依次寻找第一个支持MPEG2的decoder,并将其作为节目播放的视频解码器
//@EXECUTIONFLOW:3. 调用CSUDIAUDIOGetCount获取audio解码器的数量
//@EXECUTIONFLOW:4. 调用CSUDIAUDIOGetCapability依次寻找第一个支持MPEG2的decoder,并将其作为节目播放的音频解码器
//@EXECUTIONFLOW:5. 调用CSUDIDEMUXGetCount获取demux的数量
//@EXECUTIONFLOW:6. 调用CSUDIDEMUXGetCapability依次寻找第一个支持EM_UDI_DEMUX_PLAY并且解扰器数量大于0的demux用于节目播放的demux
//@EXECUTIONFLOW:7. 锁测试码流频点
//@EXECUTIONFLOW:8. 调用CSUDIPLAYEROpen打开一个上面获取出来设备的播放器
//@EXECUTIONFLOW:9. 调用CSUDIPLAYERSetStream设置测试节目的音视频播放参数(PID和类型)
//@EXECUTIONFLOW:10. 调用CSUDIPLAYERStart播放测试节目
//@EXECUTIONFLOW:11. 调用CSUDIDSMAllocate在选定的demux上分配2个解扰器,用于音视频PID
//@EXECUTIONFLOW:12. 调用CSUDIDSMSetPID分别关联音视频的PID
//@EXECUTIONFLOW:13. 调用CSUDIDSMSetOddKey分别设置音视频的oddKey
//@EXECUTIONFLOW:14. 调用CSUDIDSMSetEvenKey分别设置音视频的EvenKey
//@EXECUTIONFLOW:15. 调用CSUDIAUDIOGetStatus和CSUDIVIDEOGetStreamStatus获取音视频解码帧数
//@EXECUTIONFLOW:16. 每隔40ms重复步骤15，重复25次
//@EXECUTIONFLOW:17. 每隔10S重复步骤12到15,重复6次,期望音视频被解码数量大于0
//@EXECUTIONFLOW:18. 测试结束,调用CSUDIDSMClearPID分别清除音视频PID关联
//@EXECUTIONFLOW:19. 调用CSUDIDSMFree分别释放视音频的descramble
//@EXECUTIONFLOW:20. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:21. 调用CSUDIPLAYERClose关闭播放器 
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
	//设置key值
	unsigned char  scAudioOddKey[8] = {0x24 ,0x5e ,0x0d ,0x8f ,0x06 ,0xb7 ,0x47 ,0x04};
	unsigned char  scVideoOddKey[8] = {0x24 ,0x5e ,0x0d ,0x8f ,0x06 ,0xb7 ,0x47 ,0x04};

	unsigned char  scAudioEvenKey[8] = {0x29, 0x23, 0xbe, 0x0a, 0xe1, 0x6c, 0xd6, 0x23};									
	unsigned char  scVideoEvenKey[8] ={0x29, 0x23, 0xbe, 0x0a, 0xe1, 0x6c, 0xd6, 0x23};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoDecCount), "获取Video数量失败!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoDecCount > 0), "没有可用的视频解码器\n");

	nVideoIndex = FindVideoDecoder(nVideoDecCount, EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL((nVideoIndex != -1),"步骤2失败，没有支持MPEG2类型的视频解码器\n");
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount), "获取Audio数量失败!\n");

	CSTK_ASSERT_TRUE_FATAL((nAudioDecCount > 0), "没有可用的音频解码器\n");

	nAudioIndex = FindAudioDecoder(nAudioDecCount, EM_UDI_AUD_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"步骤4失败，没有支持MPEG2类型的音频解码器\n");
	sPlayerChnl.m_nAudioDecoder = nAudioIndex;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获取Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo), "获取Demux属性失败\n");
		if ((EM_UDI_DEMUX_PLAY == (sDemuxCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY)) && (sDemuxCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount),"没有找到支持EM_UDI_DEMUX_PLAY并且解扰器数量大于0的demux \n");
	sPlayerChnl.m_nDemux = i;

	CSTCPrint("请把码流的播出码率设置为38M\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(0, "SmartcardInit_V2.3.mpg"), "锁频失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer),"打开播放器失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "打开播放器成功，返回句柄不能为空\n");

	scStreamInfo[0].m_nPid = 529;
	scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	scStreamInfo[1].m_nPid = 528;
	scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL),"步骤9失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"播放节目失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hAudioDescramble), "分配音频通道失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hAudioDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(i, &hVideoDescramble), "分配视频通道失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hVideoDescramble, "分配通道成功，返回句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hAudioDescramble, 529), "指定音频PID 失败 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hVideoDescramble, 528), "指定视频PID 失败 \n");

	memset (&sStreamStatus, 0, sizeof (CSUDIVIDEOStreamStatus_S));
	memset (&sStatus, 0, sizeof (CSUDIAUDIOStatus_S));

	for (i=0; i<6; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hAudioDescramble, scAudioEvenKey, 8), "设置Audio的奇key失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hVideoDescramble, scVideoEvenKey, 8), "设置Video的奇key失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hAudioDescramble, scAudioOddKey, 8), "设置Audio的偶key失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hVideoDescramble, scVideoOddKey, 8), "设置Video的偶key失败\n");

		CSTCPrint("设置KEY，请等待10秒...\n");
		CSUDIOSThreadSleep(10000);

		for (j=0; j<25; j++)
		{
			CSUDIOSThreadSleep(40);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex, &sStatus), "获取Audio状态信息失败\n");
			nSumAudioFrameCount += sStatus.m_uFrameCount;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStreamStatus(nVideoIndex, &sStreamStatus), "获取解码器状态信息失败\n");
			nSumVideoFrameCount += sStreamStatus.m_dwFrameCount;
		}
	}

	CSTCPrint("[UDI2DESCRAMBLETEST]0:Yes,解扰成功，能正常播放约10秒节目\r\n");
	CSTCPrint("[UDI2DESCRAMBLETEST]1:No,解扰失败，不能播放节目 \r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "步骤15失败\n");

	CSTK_ASSERT_TRUE_FATAL(((nSumAudioFrameCount >0) && (nSumVideoFrameCount>0)), "音视频被解码数量不大于0 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hAudioDescramble, 529), "清除指定音频PID 失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hVideoDescramble, 528), "清除指定视频PID 失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hVideoDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hVideoDescramble), "释放视频通道失败\n");
			hVideoDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hAudioDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hAudioDescramble), "释放音频通道失败\n");
			hAudioDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
			hPlayer = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:整个descramble模块
//@DESCRIPTION:测试反复多次申请 、关联PID、清除PID、释放descramble
//@PRECONDITION:CSUDIDSMAllocate成功分配
//@EXPECTATION:反复申请、关联PID、清除PID、释放1000次,每次都返回成功
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCapability获取解扰器数量和filter数量都大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:2.调用CSUDIFILTERAllocate在测试demux上申请一个PID=0的filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3.调用CSUDIFILTERStart启动该filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMAllocate在测试demux上分配一个descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMSetPID 设置descramble的关联PID为0,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMClearPID清除关联的PID,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6.调用CSUDIDSMFree释放测试descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8.重复步骤2~7  1000次
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获取Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "获取Demux属性失败\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(i<nDemuxCount,"查找同时支持filter和descramble的demux失败\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = i;

	for (i=0;i<1000;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"申请filter失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filter失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 8), "设置8字节奇key失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 8), "设置8字节偶key失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
		hFilterHandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:整个descramble模块
//@DESCRIPTION:测试在不调用CSUDIDSMClearPID直接调用CSUDIDSMFree的情况
//@PRECONDITION:CSUDIDSMAllocate成功分配
//@EXPECTATION:反复申请、关联PID、释放descramble 1000次,每次都返回成功
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCapability获取解扰器数量和filter数量都大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:2.调用CSUDIFILTERAllocate在测试demux上申请一个PID=0的filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3.调用CSUDIFILTERStart启动该filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMAllocate在测试demux上分配一个descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMSetPID 设置descramble的关联PID为0,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6.不调用CSUDIDSMClearPID而直接调用CSUDIDSMFree释放测试descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8.重复步骤2~7  1000次
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获取Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "获取Demux属性失败\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((i<nDemuxCount), "查找同时支持filter和descramble的demux失败\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = i;

	for (i=0;i<1000;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"申请filter失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filter失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 8), "设置8字节奇key失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 8), "设置8字节偶key失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
		hFilterHandle = CSUDI_NULL;
	}

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:整个descramble模块
//@DESCRIPTION:测试先关闭descramble关联的PID通道,然后再调用CSUDIDSMSetPID清除关联PID以及调用CSUDIDSMFree释放descramble
//@PRECONDITION:1.CSUDIDSMAllocate成功分配
//@PRECONDITION:2.section模块能够正常的分配filter并且start
//@INPUT:1.  section模块打开的PID=0x100的通道
//@EXPECTATION:1. 调用CSUDIFILTERAllocate和CSUDIFILTERStart能够正常的分配启动filter
//@EXPECTATION:2. 先调用CSUDIFILTERFree释放掉PID通道,再调用CSUDIDSMSetPID清除关联PID以及调用CSUDIDSMFree释放descramble，期望都返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCapability获取解扰器数量和filter数量都大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:2.调用CSUDIFILTERAllocate在测试demux上申请一个PID=0x100的filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3.调用CSUDIFILTERStart启动该filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMAllocate在测试demux上分配一个descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMSetPID 设置descramble的关联PID为0x100,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10. 调用CSUDIFILTERStop停止测试filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11.调用CSUDIFILTERFree先释放测试filter以及PID通道,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8. 调用CSUDIDSMClearPID清除关联PID 0x100,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9.调用CSUDIDSMFree释放测试descramble,期望返回CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获取Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for (i=0; i<nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo), "获取Demux属性失败\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(i<nDemuxCount,"查找同时支持filter和descramble的demux失败\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSetction.m_nPID = 0x100;
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
		"申请filter失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filter失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, 0x100), "指定PID失败 \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetEvenKey(hDescramble, scKey, 8), "设置8字节奇key失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetOddKey(hDescramble, scKey, 8), "设置8字节偶key失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStop(hFilterHandle),"停止接收数据 \n");
	//hFilterHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, 0x100), "清除关联PID失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");		
	hDescramble = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
	hFilterHandle = CSUDI_NULL;

	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hDescramble)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:整个descramble模块
//@DESCRIPTION:测试反复多次申请 、关联清除不同PID、释放descramble
//@PRECONDITION:CSUDIDSMAllocate成功分配
//@EXPECTATION:反复申请、关联PID、清除PID、释放1000次,每次都返回成功
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCapability获取解扰器数量和filter数量都大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:2.调用CSUDIFILTERAllocate在测试demux上申请一个合法PID的filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3.调用CSUDIFILTERStart启动该filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMAllocate在测试demux上分配一个descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMSetPID 设置descramble的关联PID为申请filter的PID,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMClearPID清除关联的PID,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6.调用CSUDIDSMFree释放测试descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8.重复步骤2~7  1000次
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获取Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for (nDemuxIndex=0; nDemuxIndex<nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo), "获取Demux属性失败\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nDemuxIndex<nDemuxCount,"查找同时支持filter和descramble的demux失败\n");

	

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;	
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = nDemuxIndex;

	for (nCycCount=0; nCycCount<1000; nCycCount++, nPid++)
	{
		stSetction.m_nPID = nPid;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"申请filter失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filter失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, nPid), "指定PID失败 \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMClearPID(hDescramble, nPid), "清除关联PID失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:整个descramble模块
//@DESCRIPTION:测试反复多次申请 、关联不同PID、在不清除PID的情况下释放descramble
//@PRECONDITION:CSUDIDSMAllocate成功分配
//@EXPECTATION:反复申请、关联PID、清除PID、释放1000次,每次都返回成功
//@EXECUTIONFLOW:1.调用CSUDIDEMUXGetCapability获取解扰器数量和filter数量都大于0的demux ID，作为测试ID
//@EXECUTIONFLOW:2.调用CSUDIFILTERAllocate在测试demux上申请一个合法PID的filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3.调用CSUDIFILTERStart启动该filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4.调用CSUDIDSMAllocate在测试demux上分配一个descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5.调用CSUDIDSMSetPID 设置descramble的关联PID为申请filter的PID,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6.调用CSUDIDSMFree释放测试descramble,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7.调用CSUDIFILTERFree释放测试filter,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8.重复步骤2~7  1000次
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "获取Demux数量失败\n");

	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for (nDemuxIndex=0; nDemuxIndex<nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo), "获取Demux属性失败\n");

		if ((sCapabilityInfo.m_nFilterNum > 0) && (sCapabilityInfo.m_nkeyNum > 0)) 
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(nDemuxIndex<nDemuxCount,"查找同时支持filter和descramble的demux失败\n");

	stSetction.m_eReqType = EM_UDI_REQ_SECTION_DATA;	
	stSetction.m_bCRCCheck = CSUDI_FALSE;
	memset(&stSetction.m_sMMFilter, 0, sizeof(stSetction.m_sMMFilter));
	stSetction.m_nDemuxID = nDemuxIndex;
	
	for (nCycCount=0; nCycCount<1000; nCycCount++, nPid++)
	{
		stSetction.m_nPID = nPid;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSetction, CSUDISECTIONCallback, CSUDI_NULL, &hFilterHandle),
			"申请filter失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"start filter失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMAllocate(nDemuxIndex,&hDescramble),"分配descramble句柄失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMSetPID(hDescramble, nPid), "指定PID失败 \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
		hDescramble = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDSMFree(hDescramble), "释放descramble通道失败\n");
			hDescramble = CSUDI_NULL;
		}

		if (CSUDI_NULL != hFilterHandle)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle), "释放Filter资源失败\n");
			hFilterHandle = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

