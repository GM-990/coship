/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "udi2_error.h"
#include "udi2_demux.h"
#include "udi2_tuner.h"
#include "udi2_player.h"
#include "udi2_video.h"

#include "udi2_os.h"
#include "cs_testkit.h"
#include "cs_demuxtestcase.h"
#include "../cs_udi2testcase.h" 
#include "udi2_demux.h"

#include "cs_workermanager.h"


CSUDI_BOOL CSTC_DEMUX_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_DEMUX_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDEMUXGetCount 
//@DESCRIPTION: ���Ժ���CSUDIDEMUXGetCount��pnDemuxCount������ȷ����µ�ִ�н��
//@PRECONDITION:None
//@INPUT:pnDemuxCount=����ָ��
//@EXPECTATION:�ɹ���ȡ�ײ��������ڵ�Demux��Դ��Ŀ��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount ��ȡDemux��Դ��Ŀ
//@EXECUTIONFLOW: 2���жϻ�ȡ��Demux��Ŀ�Ƿ����0
//@EXECUTIONFLOW: 3���õ�Config�е�Demux��Ŀ
//@EXECUTIONFLOW: 4�����õ���Demux��Ŀ��config��Demux�Ƚϣ������ȷ���CSUDI_SUCCESS���������ʧ��
CSUDI_BOOL CSTC_DEMUX_IT_GetCount_0001(void)
{	
	char *pcSection = "DEMUX";
	char *pckey =  "DEMUX_NUMBER" ;
	char cResult[16];
	int  nResultSize = 16;
       int nDemuxCount = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey,cResult,nResultSize), "���ز���ʧ��\n");		
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount == CSTKGetIntFromStr(cResult,10),"���ز���ʧ��\n");
      	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
	

}

//@CASEGROUP:CSUDIDEMUXGetCount 
//@DESCRIPTION: ����CSUDIDEMUXGetCount��pnDemuxCount����Ϊ������µ�ִ�н��
//@PRECONDITION:None
//@INPUT:1��pnDemuxCount=CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1����pnDemuxCountΪNULL������CSUDIDEMUXGetCount����
//@EXECUTIONFLOW: 2���жϺ��������Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:pnDemuxCountΪNULL������£�ϵͳ���ܳ����������ײ�����ȷ�ֱ��������
CSUDI_BOOL CSTC_DEMUX_IT_GetCount_0002(void)
{
       CSUDI_BOOL bRet = CSUDI_FALSE;
	   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXGetCount(CSUDI_NULL),"����1ʧ��\n");
       bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

static int GetConfigCapability(int nIndex,CSUDIDEMUXCapability_S *capability)
{
	char *pcSection = "DEMUX";
	char cResult[16];
	int  nResultSize = 16;
	char buffer[80];
	int nRet = -1;

      sprintf(buffer,"DEMUX%d_CONNECT_TUNER",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_dwDMAConnectSupportTunersMask = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_dwDMAConnectSupportTunersMask is %d\n",capability->m_dwDMAConnectSupportTunersMask);
	
	sprintf(buffer,"DEMUX%d_FilterNum",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_nFilterNum = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_nFilterNum is %d\n",capability->m_nFilterNum);
       
	sprintf(buffer,"DEMUX%d_keyNum",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_nkeyNum = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_nkeyNum is %d\n",capability->m_nkeyNum);
     
	sprintf(buffer,"DEMUX%d_WorkTypeMask",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_dwWorkTypeMask= CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_dwWorkTypeMask is %ld\n",capability->m_dwWorkTypeMask);
      
	sprintf(buffer,"DEMUX%d_ConTypeMask",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize); 
	capability->m_dwConTypeMask = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_dwConTypeMask is %ld\n",capability->m_dwConTypeMask);
       
       CSTK_ASSERT_TRUE_FATAL((capability !=CSUDI_NULL),"������ȡ����!"); 
       nRet = 0;
	  
	CSTK_FATAL_POINT;

	return nRet;
}


//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: ����CSUDIDEMUXGetCapability�����в�������ȷ������£�����ִ�еĽ�������Է��ص�����������ȷ�Լ��
//@PRECONDITION:Demux�豸����Ŀ��Ϊ0
//@INPUT:1��nDemuxIndex = [0..nDemux-1]
//@INPUT:2��psCapabilityInfo = CSUDIDEMUXCapability_S�ṹָ��
//@EXPECTATION: ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�Demux����ĿnDemux.
//@EXECUTIONFLOW: 2��nDemuxIndexȡֵ[0..nDemux-1]�ڸ�ֵ��ִ��3-6
//@EXECUTIONFLOW: 3������CSUDIDEMUXGetCapability����ȡ��nDemuxIndex��Demux���������жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 4����ȡconfig���е�nDemuxIndex��Demux������
//@EXECUTIONFLOW: 5���Ƚ�3��4���õ������������Ƿ���ͬ
//@EXECUTIONFLOW: 6���������ͬ�������ʧ��
//@REMARK:��ȡ��Demux�������ݽṹ���ֶ���ο�CSUDIDEMUXCapability_S�Ķ���
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0001(void)
{
       int nDemuxCount =0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i =0;
       CSUDIDEMUXCapability_S capability;

       memset(&capability,0,sizeof(CSUDIDEMUXCapability_S));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"����1ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{     
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIDEMUXGetCapability(i,&sCapabilityInfo),"����3ʧ��\n");
		GetConfigCapability(i,&capability);
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_nFilterNum== capability.m_nFilterNum, "�Ƚϴ���!");
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_nkeyNum== capability.m_nkeyNum, "�Ƚϴ���!");
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_dwWorkTypeMask== capability.m_dwWorkTypeMask, "�Ƚϴ���!");
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_dwConTypeMask== capability.m_dwConTypeMask, "�Ƚϴ���!");	
              CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_dwDMAConnectSupportTunersMask==capability.m_dwDMAConnectSupportTunersMask, "�Ƚϴ���!");
       }	
       bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
	
}


//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: ����CSUDIDEMUXGetCapability��nDemuxIndex������������µ�ִ�н��
//@PRECONDITION:Demux�豸����Ŀ��Ϊ0
//@INPUT:1��nDemuxIndex={-1,nDemux,nDemux+1}
//@INPUT:2��psCapabilityInfo = CSUDIDEMUXCapability_S�ṹָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�Demux����ĿnDemux
//@EXECUTIONFLOW: 2��nDemuxIndexȡֵ{-1,nDemux,nDemux+1}���ֱ����CSUDIDEMUXGetCapability���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:��ȡnDemuxIndex={-1,nDemux,nDemux+1}�ĸ�Demux��������CSUDIDEMUXGetCapability������Ӧ���ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0002(void)
{
	int nDemuxCount =0;
	CSUDIDEMUXCapability_S sCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"����1ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetCapability(-1,&sCapabilityInfo),"����2ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetCapability(nDemuxCount,&sCapabilityInfo),"����2ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetCapability(nDemuxCount+1,&sCapabilityInfo),"����2ʧ��\n");

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}


//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: ����CSUDIDEMUXGetCapability��psCapabilityInfo����Ϊ������µ�ִ�н��
//@PRECONDITION:Demux�豸����Ŀ��Ϊ0
//@INPUT:1��nDemuxIndex =  [0..nDemux-1]
//@INPUT:2��psCapabilityInfo = CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�Demux����ĿnDemux
//@EXECUTIONFLOW: 2����ȡ[0..nDemux-1]�ڸ�Demux��������
//@EXECUTIONFLOW: 3������CSUDIDEMUXGetCapability���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:���psCapabilityInfo����Ϊ��,����������������Σ�CSUDIDEMUXGetCapability���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0003(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int  i =0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"����1ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{ 
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXGetCapability(i,CSUDI_NULL),"����3ʧ��\n");
	}

	 bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;     	 
}

//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: ����CSUDIDEMUXGetCapability��ȡ����demux����Ӧ��������СӦ�õ�Ҫ��
//@PRECONDITION:Demux�豸����Ŀ��Ϊ0
//@INPUT:1��nDemuxIndex =  [0..nDemux-1]
//@EXPECTATION: 1.�ܳɹ���ȡ��ÿ��demux������
//@EXPECTATION: 2.������һ��demux�������������(��������Ϊ����demux)
//@EXPECTATION: 2.1 ����EM_UDI_DEMUX_PLAY������
//@EXPECTATION: 2.2 ����EM_UDI_DEMUX_DMA�����ԣ������������ⲿӲ��tuner
//@EXPECTATION: 2.3 �ܺ��ⲿӲ��tuner����(��m_dwDMAConnectSupportTunersMaskӦ�ô���0)
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�Demux����ĿnDemux
//@EXECUTIONFLOW: 2����ȡ[0..nDemux-1]�ڸ�Demux���������������سɹ�
//@EXECUTIONFLOW: 3������������������2��demux����,�������ٴ���1��
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0004(void)
{
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	 int nDemuxCount =0;
	 int i = 0;
	 BOOL nFindRightDemux = FALSE;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i=0;i<nDemuxCount;i++)
	{
		memset(&sCapabilityInfo,0,sizeof(sCapabilityInfo));
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "step 2 ��ȡDemux����ʧ��!");
		if (sCapabilityInfo.m_dwWorkTypeMask&(EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_DMA))
		{
			if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask > 0)
			{
				CSTCPrint("find player demux:%d\n",i);
				nFindRightDemux = TRUE;
				break;
			}
		}
	}

	CSTK_FATAL_POINT;

	if (nFindRightDemux)
	{
		return CSUDI_TRUE;
	}

	return CSUDI_FALSE;
}

static int GetConfigTunerInfo(int nIndex, int *pnTunerInfo)
{
  	char *pcSection = "DEMUX";
	char cResult[16];
	char buffer[80];
	int nRet = -1;
	
	sprintf(buffer,"DEMUX%d_CONNECT_TUNER",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,sizeof(cResult));
	*pnTunerInfo = CSTKGetIntFromStr(cResult,10);
	/*��Щע��demux�ǲ�֧������tuner��,����Ӧ���Ǵ��ڵ���0*/
       CSTK_ASSERT_TRUE_FATAL((*pnTunerInfo >=0),"������ȡ����!");
	nRet = 0;
	  
	CSTK_FATAL_POINT;

	return nRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����Demux�豸��Tuner�豸�����ӺͶϿ����ӵ����
//@PRECONDITION:����Demux�豸
//@PRECONDITION:����Tuner�豸
//@PRECONDITION:��config�ļ�������Demux��Tuner�����Ӿ����
//@INPUT:1��nTunerIndex����ֵ= [0..nTuner-1]
//@INPUT:2��nDemuxIndex����ֵ= [0..nDemux-1]
//@EXPECTATION: 1��Demux��Tuner�����Ӿ����У��߱�����������һ��Demux��Tuner�豸��һ�������ӳɹ����ܱ���ȷ�Ͽ���
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��ͨ��config�ļ��õ�Demux��Tuner�����Ӿ����
//@EXECUTIONFLOW: 3��������Ӿ�����е�ÿһ��Demux��Tuner�豸���������豸�߱�������������ִ��4-5
//@EXECUTIONFLOW: 4������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisconnectTuner(nTunerIndex,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@REMARK:���ñ��о߱�����������Demux��Tuner�豸�ԣ�һ������ȷ����(CSUDIDEMUXConnectTuner)�ͶϿ�����(CSUDIDEMUXDisconnectTuner)
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0001(void)
{
       int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0;
       int   nTunerInfo = 0;
	int   j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{
             CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "��ȡDemux����ʧ��!");
             
	      if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
	      {
                  GetConfigCapability(i,&sCapabilityInfo);
                  if(sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA	)
                  {
                        GetConfigTunerInfo(i,&nTunerInfo);
                        for (j=0; j< 16;j++)
                        {
                            int pos = 1 << j;
                            if (nTunerInfo & pos)
                            {
                                       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"����4ʧ��!");	
                            	    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"����8 ʧ��!");
                            }
                        }
                   }
	      }
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����Demux�豸��Tuner�豸�������ӵ����
//@PRECONDITION:����Demux�豸
//@PRECONDITION:����Tuner�豸
//@PRECONDITION:��config�ļ�������Demux��Tuner�����Ӿ����
//@INPUT:1��nTunerIndex����ֵ= [0..nTuner-1]
//@INPUT:2��nDemuxIndex����ֵ= [0..nDemux-1]
//@EXPECTATION: 1��Demux��Tuner�����Ӿ����У����߱�����������һ��Demux��Tuner�豸��һ���������ӳɹ���
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��ͨ��config�ļ��õ�Demux��Tuner�����Ӿ����
//@EXECUTIONFLOW: 3��������Ӿ�����е�ÿһ��Demux��Tuner�豸���������豸���߱���������������ִ��4
//@EXECUTIONFLOW: 4������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)���ж��ܷ���ȷ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisConnectTuner(nTunerIndex,nDemuxIndex),�ָ��ֳ�
//@REMARK:���ñ��в��߱�����������Demux��Tuner�豸�ԣ�һ��������ȷ����
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0002(void)
{
       int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i =0;
       int   nTunerInfo = 0;
	int   j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{
		GetConfigTunerInfo(i,&nTunerInfo);
		
		for (j=0; j< 16;j++)
		{
			int pos = 1 << j;
			if (!(nTunerInfo & pos))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXConnectTuner(j, i),"����4ʧ��!");	
			}
		}
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����һ��Demux�豸ͬʱ���ֻ����һ��Tuner�豸����
//@PRECONDITION:����Demux�豸
//@PRECONDITION:���ڲ�ֹһ��Tuner�豸
//@PRECONDITION:��config�ļ�������Demux��Tuner�����Ӿ����
//@PRECONDITION:��Demux��Tuner�����Ӿ�����У�����ĳ��Demux�豸�ܹ�����Tuner���ӵ�����
//@INPUT:1��nTunerIndex����ֵ= [0..nTuner-1]
//@INPUT:2��nDemuxIndex����ֵ= [0..nDemux-1]
//@EXPECTATION: һ��Demux�豸���ֻ��ͬʱ��һ��Tuner�豸���ӣ�
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��ͨ��config�ļ��õ�Demux��Tuner�����Ӿ����ѡȡһ��nDemuxIndex����Demux������Tuner����
//@EXECUTIONFLOW: 3������CSUDIDEMUXConnectTuner(nTunerIndex0,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIDEMUXConnectTuner(nTunerIndex1,nDemuxIndex)���ж��ܷ���ȷ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisConnectTuner(nTunerIndex0,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIDEMUXConnectTuner(nTunerIndex1,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIDEMUXDisConnectTuner(nTunerIndex1,nDemuxIndex)���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0003(void)
{
       int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0;
       int   nTunerInfo = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{
              CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "��ȡDemux����ʧ��!");
             
	       if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
             {
                GetConfigCapability(i,&sCapabilityInfo);
                  if(sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA	)
    		    {
                         GetConfigTunerInfo(i,&nTunerInfo);
            		if (nTunerInfo == 3)
            		{		
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, i),"����3ʧ��!");
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(1, i),"����4ʧ��!");	 
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(0, i),"����5ʧ��!");
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(1, i),"����6ʧ��!");
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(1, i),"����7ʧ��!");
            		}
                  }
	       }
	}

      bRet = CSUDI_TRUE;
		 
      CSTK_FATAL_POINT
	  	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22
	
	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����һ��Tuner�豸ͬʱ�ܹ�����Demux����
//@PRECONDITION:���ڲ�ֹһ��Demux�豸
//@PRECONDITION:����Tuner�豸
//@PRECONDITION:��config�ļ�������Demux��Tuner�����Ӿ����
//@PRECONDITION:��Demux��Tuner�����Ӿ�����У�����ĳ��Tuner�豸�ܹ�����Demux���ӵ�����
//@INPUT:1��nTunerIndex����ֵ= [0..nTuner-1]
//@INPUT:2��nDemuxIndex����ֵ= [0..nDemux-1]
//@EXPECTATION: һ��Demux�豸���ֻ��ͬʱ��һ��Tuner�豸����
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��ͨ��config�ļ��õ�Demux��Tuner�����Ӿ����ѡȡһ��nTunerIndex����Tuner������Demux����
//@EXECUTIONFLOW: 3������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex0)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex1)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisConnectTuner(nTunerIndex,nDemuxIndex0)���ָ��ֳ�
//@EXECUTIONFLOW: 6������CSUDIDEMUXDisConnectTuner(nTunerIndex,nDemuxIndex1)���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0004(void)
{
       int nDemuxCount =0;
	int nTunerInfo1=0;
	int nTunerInfo2=0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       CSUDIDEMUXCapability_S  sCapabilityInfo1;
       CSUDIDEMUXCapability_S  sCapabilityInfo2;
	int i =0;
	int j =0;
	int z =0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for(i = 1; i < nDemuxCount; i++)
	{    
              CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo2)), "��ȡDemux����ʧ��!");
             
	       if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo2.m_dwDMAConnectedTunerId)
		{
                   for(j = 0; j < nDemuxCount; j++)
    		     {
                          GetConfigCapability(j,&sCapabilityInfo1);
                          GetConfigCapability(i,&sCapabilityInfo2);
                          if((sCapabilityInfo1.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA)&&(sCapabilityInfo2.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA))
                          {
                                 GetConfigTunerInfo(j,&nTunerInfo1);
        			     GetConfigTunerInfo(i,&nTunerInfo2);

        			     if ((nTunerInfo1 == nTunerInfo2)&&(i !=j))	
        			     {  
        			           for (z=0; z< 16;z++)
        			           {
        			               int pos = 1 << z;
        	                             if (nTunerInfo1& pos)
                		               {
                                           	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(z, j),"����3ʧ��!");
                				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(z, i),"����4ʧ��!");	 
                				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(z, j),"����5ʧ��!");
                				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(z, i),"����6ʧ��!");	 
                		               }
        			           }
        			      }
                              else
                              {
                                   break; 
                              }
                          }
    		      }
	       }
	}
	
	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT
	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22
	
	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ���ԷǷ���nDemuxIndex�������κ�Tuner�豸(�����ǺϷ��Ļ��ǷǷ���nTunerIndex)������
//@INPUT:1��nTunerIndex����ֵ= [0..nTuner-1]
//@INPUT:2��nDemuxIndex����ֵ= {-1,nDemux,nDemux+1}
//@EXPECTATION: �Ƿ���nDemuxIndex�������κ�Tuner�豸����
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��nTunerIndexȡ[0..nTuner-1]�ڵ�ֵ
//@EXECUTIONFLOW: 3��nDemuxIndexȡֵ{-1,nDemux,nDemux+1}
//@EXECUTIONFLOW: 4������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex),�ж��ܷ���ȷ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 5���ظ�ִ��4, ֱ��nDemuxIndexȡ��{-1,nDemux,nDemux+1}�е�����ֵ
//@EXECUTIONFLOW: 6���ظ�ִ��2, ֱ��nTunerIndexȡ��[0..nTuner-1]�ڵ�����ֵ
//@REMARK:ĳ���Ƿ�Demux�豸�������κκϷ���Tuner�豸��ȷ����
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0005(void)
{
       int nDemuxCount = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i =0;
       int   nTunerInfo = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{
	       GetConfigTunerInfo(i,&nTunerInfo);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(nTunerInfo,-1),"����4ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(nTunerInfo,nDemuxCount),"����4ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(nTunerInfo,nDemuxCount+1),"����4ʧ��\n");     	
	}	
	
	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@CASEGROUP:CSUDIDEMUXGetCapability
//@DESCRIPTION: m_dwDMAConnectedTunerId��ΪCSUDIDEMUX_NOT_CONNECTED_ID���ܷ�Ͽ��ɹ�
//@INPUT:1��nDemuxIndex����ֵ= [0..nDemux-1]
//@INPUT:2��nTunerIndex����ֵ= m_dwDMAConnectedTunerId
//@EXPECTATION: �ܶϿ��ɹ�
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��nDemuxIndexȡ[0..nDemux-1]�ڵ�ֵ
//@EXECUTIONFLOW: 3������CSUDIDEMUXGetCapability�ж�sCapabilityInfo�е�m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 4�����sCapabilityInfo.m_dwDMAConnectedTunerId��ΪCSUDIDEMUX_NOT_CONNECTED_ID,��ִ������Ĳ���
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisconnectTuner�Ͽ�nDemuxIndex��m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 6���ظ�ִ��2, ֱ��nDemuxIndexȡ��[0..nDemux-1]�ڵ�����ֵ
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0006(void)
{
       int nDemuxCount = 0;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_Error_Code bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)), "��ȡDemux����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for(; nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "��ȡDemux����ʧ��!");
		
		if(CSUDIDEMUX_NOT_CONNECTED_ID != sCapabilityInfo.m_dwDMAConnectedTunerId)
		{
			CSTCPrint("DemuxID:%d, TunerID:%d\n\n", nDemuxIndex, sCapabilityInfo.m_dwDMAConnectedTunerId);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(sCapabilityInfo.m_dwDMAConnectedTunerId, nDemuxIndex), "�Ͽ�����ʧ��");
                     CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "��ȡDemux����ʧ��!"); 
                     CSTK_ASSERT_TRUE_FATAL(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId,"�Ƚ�ʧ��!");
		}
             else
             {
                break;
             }         
	}

       bResult = CSUDI_TRUE;
      
	CSTK_FATAL_POINT;
 	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22
    
	return bResult;
}


//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@CASEGROUP:CSUDIDEMUXGetCapability
//@DESCRIPTION: m_dwDMAConnectedTunerId��ΪCSUDIDEMUX_NOT_CONNECTED_ID���ܷ�Ͽ��ɹ�
//@INPUT:1��nDemuxIndex����ֵ= [0..nDemux-1]
//@INPUT:2��nTunerIndex����ֵ= m_dwDMAConnectedTunerId
//@EXPECTATION: �ܶϿ��ɹ�
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2��nDemuxIndexȡ[0..nDemux-1]�ڵ�ֵ
//@EXECUTIONFLOW: 3������CSUDIDEMUXGetCapability�ж�sCapabilityInfo�е�m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 4�����sCapabilityInfo.m_dwDMAConnectedTunerIdΪCSUDIDEMUX_NOT_CONNECTED_ID,��ִ������Ĳ���
//@EXECUTIONFLOW: 5������CSUDIDEMUXConnectTuner����nDemuxIndex��m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisconnectTuner�Ͽ�nDemuxIndex��m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 7���ظ�ִ��2, ֱ��nDemuxIndexȡ��[0..nDemux-1]�ڵ�����ֵ
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0007(void)
{
       int nDemuxCount = 0;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_Error_Code bResult = CSUDI_FALSE;
       int   nTunerInfo = 0;
       int j = 0;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)), "��ȡDemux����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux����Ӧ�ô���0");

	for(; nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "��ȡDemux����ʧ��!");
		
		if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
		{
			 GetConfigCapability(nDemuxIndex,&sCapabilityInfo);
                      if(sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA	)
                      {
                            GetConfigTunerInfo(nDemuxIndex,&nTunerInfo);
                            for (j=0; j< 16;j++)
                            {
                                int pos = 1 << j;
                                if (nTunerInfo & pos)
                                {
                                          CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, nDemuxIndex),"����4ʧ��!");	
                                          CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "��ȡDemux����ʧ��!"); 
                                          CSTK_ASSERT_TRUE_FATAL(CSUDIDEMUX_NOT_CONNECTED_ID != sCapabilityInfo.m_dwDMAConnectedTunerId,"�Ƚ�ʧ��!");
                                		CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, nDemuxIndex),"����8 ʧ��!");
                                          CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "��ȡDemux����ʧ��!"); 
                                          CSTK_ASSERT_TRUE_FATAL(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId,"�Ƚ�ʧ��!");
                                }
                            }
                       }
		}
             else 
            {
                break;
            }
	}

       bResult = CSUDI_TRUE;
      
	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22
    
	return bResult;
}



//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����Demux�豸��Tuner�豸���������ӵ����,��ͨ��demux��������m_dwDMAConnectedTunerIdȥ�ж��Ƿ���Ҫ����demux��tunerʱ,������߱���������,��������
//@PRECONDITION:����Demux�豸
//@PRECONDITION:����Tuner�豸
//@PRECONDITION:��config�ļ�������Demux��Tuner�����Ӿ����
//@INPUT:1��nTunerIndex����ֵ= [0..nTuner-1]
//@INPUT:2��nDemuxIndex����ֵ= [0..nDemux-1]
//@EXPECTATION: 1��Demux��Tuner�����Ӿ����У��߱�����������һ��Demux��Tuner�豸��һ�������ӳɹ����ܱ���ȷ�Ͽ���
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ
//@EXECUTIONFLOW: 2������CSUDIDEMUXGetCapability��ȡָ��Demux�豸������
//@EXECUTIONFLOW: 3������CSUDIDEMUXDisconnectTuner���Ͽ�ָ��demux��ָ��tuner������
//@EXECUTIONFLOW: 4��������Ӿ�����е�ÿһ��Demux��Tuner�豸���������豸û�жϿ������ظ�����3
//@EXECUTIONFLOW: 5��ͨ��config�ļ��õ�Demux��Tuner�����Ӿ����
//@EXECUTIONFLOW: 6��������Ӿ�����е�ÿһ��Demux��Tuner�豸���������豸�߱�������������ִ��7-8
//@EXECUTIONFLOW: 7������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIDEMUXDisconnectTuner(nTunerIndex,nDemuxIndex)���ж��ܷ���ȷ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 9��������Ӿ�����е�ÿһ��Demux��Tuner�豸���������豸���߱���������,��ִ��8
//@EXECUTIONFLOW: 10������CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)������CSUDI_SUCCESS�����ʧ��,������������ɹ�
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0008(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i = 0;
	int nTunerInfo = 0;
	int j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��!");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0, "���ز���ʧ��!");
    
    for (i = 0; i < nDemuxCount; i++)
    {
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "����2��ȡDemux����ʧ��!");

        if (CSUDIDEMUX_NOT_CONNECTED_ID != sCapabilityInfo.m_dwDMAConnectedTunerId)
        {    
			if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask)
			{
				for (j = 0; j < 16; j++)
				{
					if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask&(1<<j))
					{
						if (sCapabilityInfo.m_dwDMAConnectedTunerId == j)
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i), "����3ʧ��!");
						}
					}
				}
			}
        }
	}
    
    for (i = 0; i < nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "��ȡDemux����ʧ��!");
             
		if (CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == GetConfigCapability(i, &sCapabilityInfo), "����5ʧ��!");
            
			if (sCapabilityInfo.m_dwWorkTypeMask & EM_UDI_DEMUX_DMA)
			{
				GetConfigTunerInfo(i, &nTunerInfo);
				for (j = 0; j < 16; j++)
				{
					int pos = 1 << j;
					if (nTunerInfo & pos)
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i), "����7ʧ��!");	
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i), "����8ʧ��!");
					}
					else
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXConnectTuner(j, i), "����10ʧ��!");						
					}
				}
			}
		}
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0), "�ָ�Ĭ������ʧ��");

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����demux ��tuner ���Ѿ����ӵ�����ٽ���������,�������Ѿ��Ͽ���������ٶϿ��ĳ���
//@PRECONDITION:����demux�豸
//@INPUT:1������tuner���ӵ�demux id
//@EXPECTATION: 1. demux ��tuner ���Ѿ����ӵ�����ٽ���������Ӧ�÷��سɹ�
//@EXPECTATION: 2. demux ��tuner �Ѿ��Ͽ���������ٶϿ�Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ,��demux [0,nDemux-1]�����²���
//@EXECUTIONFLOW: 2������CSUDIDEMUXGetCapability��ȡdemux n�����������ɹ�
//@EXECUTIONFLOW: 3�������demux��m_dwDMAConnectSupportTunersMask����0,���ҵ����������ӵ�tuner m
//@EXECUTIONFLOW: 4�����demux n�Ѿ���tuner����,��Ͽ�demux��tuner������,�������سɹ�
//@EXECUTIONFLOW: 5������CSUDIDEMUXConnectTuner(m,n)���������سɹ�
//@EXECUTIONFLOW: 6���ٴε���CSUDIDEMUXConnectTuner(m,n)���������سɹ�
//@EXECUTIONFLOW: 7������CSUDIDEMUXDisconnectTuner(m,n)���������سɹ�
//@EXECUTIONFLOW: 8���ٴε���CSUDIDEMUXDisconnectTuner(m,n)���������سɹ�
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0009(void)
{
	 int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0,j=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "step 2 ��ȡDemux����ʧ��!");
		if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask)
		{
			for (j=0;j<16;j++)
			{
				if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask&(1<<j))
				{
					if (sCapabilityInfo.m_dwDMAConnectedTunerId == j)
					{
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 4 ʧ��!");
					}

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"step 5 ʧ��!");	
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"step 6 ʧ��!");	
					CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 7 ʧ��!");
					CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 8 ʧ��!");
					bRet = CSUDI_TRUE;
				}
			}
		}
	}

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22
	
	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����demux ��tuner ���з��������ӺͶϿ�
//@INPUT:1������tuner���ӵ�demux id
//@EXPECTATION: 1. demux ��tuner ���з��������ӺͶϿ�500��ÿ�ζ��ܷ��سɹ�
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�nDemux����Ŀ,��demux [0,nDemux-1]�����²���
//@EXECUTIONFLOW: 2������CSUDIDEMUXGetCapability��ȡdemux n�����������ɹ�
//@EXECUTIONFLOW: 3�������demux��m_dwDMAConnectSupportTunersMask����0,�����ÿ�������ӵ�tuner m�����²���
//@EXECUTIONFLOW: 4�����demux n�Ѿ���tuner����,��Ͽ�demux��tuner������,�������سɹ�
//@EXECUTIONFLOW: 5������CSUDIDEMUXConnectTuner(m,n)���������سɹ�
//@EXECUTIONFLOW: 6������CSUDIDEMUXDisconnectTuner(m,n)���������سɹ�
//@EXECUTIONFLOW: 7���ظ�����5��6һ��500��,ÿ�ζ��������سɹ�
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0010(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0,j=0,m=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "����1ʧ��\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "step 2 ��ȡDemux����ʧ��!");
		if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask)
		{
			for (j=0;j<16;j++)
			{
				if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask&(1<<j))
				{
					if (sCapabilityInfo.m_dwDMAConnectedTunerId == j)
					{
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 4 ʧ��!");
					}

					for(m=0;m<500;m++)
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"step 5 ʧ��!");	
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 8 ʧ��!");
					}
				}
			}
		}
	}

	bRet = CSUDI_TRUE;

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"�ָ�Ĭ������ʧ��");//�ָ�Ĭ������Modified by qLuo 2011-8-22
	
	return bRet;
}

static int GetConfigFilterCount(int nIndex, int  *pnFilterCount)
{
  	char *pcSection = "DEMUX";
	char cResult[16];
	int  nResultSize = 16;
	char buffer[80];
	int nRet = -1;
	
	sprintf(buffer,"DEMUX%d_FilterNum",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	*pnFilterCount = CSTKGetIntFromStr(cResult,10);
       CSTK_ASSERT_TRUE_FATAL((pnFilterCount !=CSUDI_NULL),"������ȡ����!");
	nRet = 0;
	  
	CSTK_FATAL_POINT;

	return nRet;
}

//@CASEGROUP:CSUDIDEMUXGetFreeFilterCount
//@DESCRIPTION: ����ָ��Demux�豸�Ŀ���Filter����
//@PRECONDITION:��config�ļ��У���������Demux�������ñ�
//@INPUT:1��nDemuxIndex= [0..nDemux-1]
//@INPUT:2��pnFreeFilterCount=�޷�������ָ��
//@EXPECTATION: �ɹ���ȡָ��Demux�豸�Ŀ���Filter������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetFreeFilterCount�õ�pnFreeFilterCount����Ŀ
//@EXECUTIONFLOW: 2��ͨ��config�ļ����õ���Demux��Filter����
//@EXECUTIONFLOW: 3����ͨ��CSUDIDEMUXGetFreeFilterCount�õ��Ŀ�����Ŀ���кϷ��Լ��
//@EXECUTIONFLOW: 4�������Ŀ�Ϸ���������CSUDI_SUCCESS���������ʧ��
CSUDI_BOOL CSTC_DEMUX_IT_GetFreeFilterCount_0001(void)
{
	int nDemuxCount =0;
	int nFreeFilterCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int  i =0;
       int nFilterCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"����1ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");

	for (i = 0; i < nDemuxCount; i++)
	{ 
        	GetConfigFilterCount(i,&nFilterCount);
	 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(i,&nFreeFilterCount),"����1ʧ��\n");
	 	CSTK_ASSERT_TRUE_FATAL(nFreeFilterCount <= nFilterCount, "�Ƚϴ���!");
	 }

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXGetFreeFilterCount
//@DESCRIPTION: ����CSUDIDEMUXGetFreeFilterCount��nDemuxIndex������������µ�ִ�н��
//@PRECONDITION:None
//@INPUT:1��nDemuxIndex= {-1,nDemux,nDemux+1}
//@INPUT:2��pnFreeFilterCount=�޷�������ָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount�õ�Demux����ĿnDemux
//@EXECUTIONFLOW: 2��ȡֵnDemuxIndex= {-1,nDemux,nDemux+1}
//@EXECUTIONFLOW: 3������CSUDIDEMUXGetFreeFilterCount�õ�pnFreeFilterCount����Ŀ
//@EXECUTIONFLOW: 4���ж��ܷ���ȷ���ط�CSUDI_SUCCESS
//@REMARK:nDemuxIndex={-1,nDemux,nDemux+1}��CSUDIDEMUXGetFreeFilterCount������Ӧ���ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetFreeFilterCount_0002(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nFreeFilterCount=0;
	
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"����1ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetFreeFilterCount(-1,&nFreeFilterCount),"����3ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetFreeFilterCount(nDemuxCount,&nFreeFilterCount),"����3ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetFreeFilterCount(nDemuxCount+1,&nFreeFilterCount),"����3ʧ��\n");

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXGetFreeFilterCount
//@DESCRIPTION: ����CSUDIDEMUXGetFreeFilterCount�ڲ���pnFreeFilterCountΪ�������ִ�н��
//@PRECONDITION:None
//@INPUT:1��nDemuxIndex= [0..nDemux-1]
//@INPUT:2��pnFreeFilterCount=CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetFreeFilterCount
//@EXECUTIONFLOW: 2���жϷ���ֵ�Ƿ��Ƿ�CSUDI_SUCCESS
//@REMARK:1��pnFreeFilterCountΪNULL������£�ϵͳ���ܳ����������ײ�����ȷ�ֱ��������
//@REMARK:2�����psFreeFilterCount����Ϊ��,����������������Σ�CSUDIDEMUXGetFreeFilterCount���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetFreeFilterCount_0003(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int  i =0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"����1ʧ��\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"���ز���ʧ��\n");
		
	for (i = 0; i < nDemuxCount; i++)
	{ 
	 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXGetFreeFilterCount(i,CSUDI_NULL),"����2ʧ��\n");
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}


