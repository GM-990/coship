/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "udi2_demux.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_player.h" 
#include "udi2_inject.h" 
#include "cs_injectertest.h"

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
int searchDemuxID_Y_InInject(int nDemxType)
{
	int i = 0;
	int nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S sDemuxCapabilityInfo;

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)) //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo)) //ȡ���ӦID=i ��demux������
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask &nDemxType) != 0)
				{
					nDemuxID = i;
					break;
				}
			}	
		}
	}
	CSTCPrint("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return nDemuxID;
}

//����֧��nAudioType���͵�Audio Decoder
//nAudioType�μ�CSUDIAUDStreamType_E, ����ΪEM_UDI_AUD_STREAM_AC3���⴦��
int searchAudioID_Y_InInject(int nAudioType)
{
	CSUDIAUDStreamType_E eAudStreamTypeNum = EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIAUDIOCapability_S sAudioCapabilityInfo;
	int nAudioCount = 0;
	int nAudioID = -1;
	int i = 0;
	int j = 0;
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount)) //��ȡ����Ƶ������������
	{
		for (i=0; i<nAudioCount; i++)
		{
			
			if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo)) //�����ID����Ƶ������������
			{
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)
				{
					for (j=0; j<eAudStreamTypeNum; j++) //�����������������棺֧�ֽ����������֧��ֱ�����������
					{
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == EM_UDI_AUD_STREAM_UNKNOWN)
						{
							break;
						}
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == nAudioType)
						{
							nAudioID=i;
							break;
						}
					}
				}
				else
				{
					for (j=0; j<eAudStreamTypeNum; j++) //�����������������棺֧�ֽ����������֧��ֱ�����������
					{
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == EM_UDI_AUD_STREAM_UNKNOWN)
						{
                            //CSTKPrint("eAudStreamTypeNum=%d\n",sAudioCapabilityInfo.m_sSupportDecode[j]);
							break;
						}
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == nAudioType)
						{
							nAudioID=i;
							break;
						}
					}
				}
				
				if (nAudioID != -1)
					break;
			}
		}
	}	

	return nAudioID;
}

//���ݴ����index������Ҫ�����������͵���Ƶ������
//�������������ʾ���Ӧ����Ƶ����Ƶ����ö�����ȡֵ
int searchVideoID_Y_InInject(int type_index)
{
	int i;
	int j;
	CSUDIVIDStreamType_E eVidStreamTypeNum = EM_UDI_VID_STREAMTYPE_NUM ;
	int nVideoCount;
	int nVidioID=-1;
	CSUDIVIDEOCapability_S sVideoCapabilityInfo;	
	
	CSUDIVIDEOGetCount(&nVideoCount); //��ȡ����

	for ( i=0;i < nVideoCount; i++)
	{
		CSUDIVIDEOGetCapability( i, &sVideoCapabilityInfo);	//��ȡ�������������Ƶ��ͬ������Ƶֻ��֧�ֵ���Ƶ����һ��

		for (j=0; j < eVidStreamTypeNum; j++)
		{
			if (sVideoCapabilityInfo.m_eStreamType[j] == EM_UDI_VID_STREAM_UNKNOWN)
			{
				break;
			}
			if (sVideoCapabilityInfo.m_eStreamType[j] == type_index)
			{
				nVidioID=i;
				break;
			}
		}

		if (nVidioID!=-1)
		{
			break;
		}
	}

	return nVidioID;

}

void fnReqDataCallback(void *pvContext)
{
	return;
}

