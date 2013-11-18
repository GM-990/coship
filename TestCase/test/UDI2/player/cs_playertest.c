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
#include "udi2_tuner.h"
#include "udi2_os.h"
#include "udi2_player.h" 
#include "cs_playertest.h"

int g_TunnerSuccess = 0; //�ж���Ƶ�Ƿ�ɹ�0Ϊʧ�ܣ�1Ϊ�ɹ�
int g_nFrequency =0;  //���Բ���������Ƶ�� 
int g_nModulation = 0; //���Ե��Ʒ�ʽ
int g_nSymbolrate = 0;  //���Է�����
int g_nCallBackNum = 0; //��ӵ�callback����Ŀ
int g_nCallBackTime = 0; //��ӵ�callback�ĸ���

//������Ƶ����Ƶ��������ʼ����Ӧ�Ĳ�����Ϣ
void initAVInfo(int nAudioId, int nVideoId)
{
	if(nAudioId != -1)
	{
		CSUDIAUDIOSetVolume(nAudioId, 31);
	}
	if(nVideoId != -1)
	{
		CSUDIVIDEOShow(nVideoId, CSUDI_TRUE);
		CSUDIVIDEOSetAspectRatio(nVideoId,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
		CSUDIVIDEOSetMatchMethod(nVideoId,EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);
	}
	return ;
}

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
int PLAYER_searchDemuxID_WithIndex(int nDemxType,int index)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;
	int nIndex = 0;

	if((nDemxType < EM_UDI_DEMUX_PLAY)||(nDemxType > (EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_RECORD|EM_UDI_DEMUX_DMA|EM_UDI_DEMUX_INJECT)))
	{
		CSTCPrint("[%s][line:%d] nDemxType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nDemxType);
		return  -1;
	}
	
	if(index<0)
	{
		CSTCPrint("[%s][line:%d] index Error!!!(%d)\n\r", __FUNCTION__, __LINE__, index);
		return  -1;
	}
	
	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType ) != 0)
				{					
					nIndex = nIndex +1;
					
					if (nIndex == index)
					{
						nDemuxID=i;
						
						break;
					}
				}
			}
			else
			{
				CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCapability failed!!! Demux Index:%d\n\r", __FUNCTION__, __LINE__, i);
			}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	CSTCPrint("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return  nDemuxID;
}

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
int PLAYER_searchDemuxID_Y(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if((nDemxType < EM_UDI_DEMUX_PLAY)||(nDemxType > (EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_RECORD|EM_UDI_DEMUX_DMA|EM_UDI_DEMUX_INJECT)))
	{
		CSTCPrint("[%s][line:%d] nDemxType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nDemxType);
		return  -1;
	}
	
	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType ) != 0)
				{
					nDemuxID=i;
					break;
				}
			}	
			else
			{
				CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCapability failed!!! Demux Index:%d\n\r", __FUNCTION__, __LINE__, i);
			}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	CSTCPrint("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return  nDemuxID;
}

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
int PLAYER_searchDemuxID_N(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if((nDemxType < EM_UDI_DEMUX_PLAY)||(nDemxType > (EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_RECORD|EM_UDI_DEMUX_DMA|EM_UDI_DEMUX_INJECT)))
	{
		CSTCPrint("[%s][line:%d] nDemxType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nDemxType);
		return  -1;
	}
	
	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType ) == 0)
				{
					nDemuxID=i;
					break;
				}
			}	
			else
			{
				CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCapability failed!!! Demux Index:%d\n\r", __FUNCTION__, __LINE__, i);
			}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	CSTCPrint("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return  nDemuxID;
}

//�Ƿ�֧����Ӧ�����͵�bypass���
//nType ΪCSUDIAUDStreamType_Eö�ٶ���֧�ֵ�����
static CSUDI_BOOL iIsAudioSupportBypassType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;

	if(nAudioIndex<0)
	{
		CSTCPrint("[%s][line:%d] nAudioIndex Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nAudioIndex);
		return CSUDI_FALSE;
	}
	
	if((nType <= EM_UDI_AUD_STREAM_UNKNOWN)||(nType >= EM_UDI_AUD_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Audio Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nType);
		return  CSUDI_FALSE;
	}
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportByPass[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportByPass[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}
//�Ƿ�֧����Ӧ�����͵Ľ������
//nType ΪCSUDIAUDStreamType_Eö�ٶ���֧�ֵ�����
static CSUDI_BOOL iIsAudioSupportDecodeType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;

	if(nAudioIndex<0)
	{
		CSTCPrint("[%s][line:%d] nAudioIndex Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nAudioIndex);
		return CSUDI_FALSE;
	}
	
	if((nType <= EM_UDI_AUD_STREAM_UNKNOWN)||(nType >= EM_UDI_AUD_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Audio Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nType);
		return  CSUDI_FALSE;
	}
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportDecode[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportDecode[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

//����֧��nAudioType���͵�Audio Decoder
//nAudioType�μ�CSUDIAUDStreamType_E, ����ΪEM_UDI_AUD_STREAM_AC3���⴦��
int PLAYER_searchAudioID_WithIndex(int nAudioType,int index)
{
	int nAudioCount = 0;
	int nAudioID = -1;
	int nAudioIndex = 0;
	int nIndex = 0;

	if(index<0)
	{
		CSTCPrint("[%s][line:%d] index Error!!!(%d)\n\r", __FUNCTION__, __LINE__, index);
		return -1;
	}
	
	if((nAudioType <= EM_UDI_AUD_STREAM_UNKNOWN)||(nAudioType >= EM_UDI_AUD_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Audio Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nAudioType);
		return  -1;
	}
	
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //��ȡ����Ƶ������������
	{
		for (nAudioIndex = 0; nAudioIndex<nAudioCount; nAudioIndex++)
		{
			
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)//AC3�ż��bypassģʽ
				{
					if (iIsAudioSupportBypassType(nAudioIndex,nAudioType))
					{
						nIndex = nIndex+1;
						
						if (nIndex == index)
						{
							nAudioID = nAudioIndex;
							break;
						}
					}
				}
				else
				{
					if (iIsAudioSupportDecodeType(nAudioIndex,nAudioType))
					{
						nIndex = nIndex+1;						
						
						if (nIndex == index)
						{
							nAudioID = nAudioIndex;
							break;
						}
					}
				}
		}
	}	
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIAUDIOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}

	return nAudioID;
}

//����֧��nAudioType���͵�Audio Decoder
//nAudioType�μ�CSUDIAUDStreamType_E, ����ΪEM_UDI_AUD_STREAM_AC3���⴦��
int PLAYER_searchAudioID_Y(int nAudioType)
{
	int nAudioCount = 0;
	int nAudioID = -1;
	int nAudioIndex = 0;

	if((nAudioType <= EM_UDI_AUD_STREAM_UNKNOWN)||(nAudioType >= EM_UDI_AUD_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Audio Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nAudioType);
		return  -1;
	}
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //��ȡ����Ƶ������������
	{
		for (nAudioIndex = 0; nAudioIndex<nAudioCount; nAudioIndex++)
		{
			if(nAudioType == EM_UDI_AUD_STREAM_AC3)//AC3�ż��bypassģʽ
			{
				if (iIsAudioSupportBypassType(nAudioIndex,nAudioType))
				{					
					nAudioID = nAudioIndex;
					
					break;
				}
			}
			else
			{
				if (iIsAudioSupportDecodeType(nAudioIndex,nAudioType))
				{					
					nAudioID = nAudioIndex;
					
					break;
				}
			}
		}
	}	
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIAUDIOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}

	return nAudioID;
}
static CSUDI_BOOL iIsVideoSupportDecodeType(int nVideoIndex,int nType)
{
	int nSupportIndex;
	CSUDIVIDEOCapability_S  sCapability;

	if(nVideoIndex<0)
	{
		CSTCPrint("[%s][line:%d] nVideoIndex Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nVideoIndex);
		return CSUDI_FALSE;
	}
	
	if((nType <= EM_UDI_VID_STREAM_UNKNOWN)||(nType >= EM_UDI_VID_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Video Type Error!!!\n\r", __FUNCTION__, __LINE__);
		return  CSUDI_FALSE;
	}
	
	memset(&sCapability,EM_UDI_VID_STREAM_UNKNOWN,sizeof(CSUDIVIDEOCapability_S));
	if(CSUDIVIDEOGetCapability(nVideoIndex, &sCapability) != CSUDI_SUCCESS)
	{
		CSTCPrint("[%s][line:%d] Get Video (%d) capability failure !!\n\r", __FUNCTION__, __LINE__, nVideoIndex);
		return CSUDI_FALSE;
	}
	
	for (nSupportIndex=0; 
			nSupportIndex < EM_UDI_VID_STREAMTYPE_NUM && sCapability.m_eStreamType[nSupportIndex]!= EM_UDI_VID_STREAM_UNKNOWN;
				nSupportIndex++)
	{
		if((sCapability.m_eStreamType[nSupportIndex] == nType))
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

//����֧��nVideoType���͵�Video Decoder
//nVideoType�μ�CSUDIVIDStreamType_E
int PLAYER_searchVideoID_WithIndex(int nVideoType,int index)
{	
	int nVideoCount = 0;
	int nVidioID = -1;
	int nVideoIndex = 0;
	int nIndex = 0;

	if(index<0)
	{
		CSTCPrint("[%s][line:%d] index Error!!!(%d)\n\r", __FUNCTION__, __LINE__, index);
		return -1;
	}
	
	if((nVideoType <= EM_UDI_VID_STREAM_UNKNOWN)||(nVideoType >= EM_UDI_VID_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Video Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nVideoType);
		return  -1;
	}
			
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)) //��ȡ����
	{
		for (nVideoIndex = 0; nVideoIndex<nVideoCount; nVideoIndex++)
		{
				if(iIsVideoSupportDecodeType(nVideoIndex,nVideoType))
				{
					nIndex = nIndex+1;
										
					if (nIndex == index)
					{
						nVidioID = nVideoIndex;
						break;
					}
				}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}

	return nVidioID;
}

//����֧��nVideoType���͵�Video Decoder
//nVideoType�μ�CSUDIVIDStreamType_E
int PLAYER_searchVideoID_Y(int nVideoType)
{	
	int nVideoCount = 0;
	int nVidioID = -1;
	int nVideoIndex = 0;
	
	if((nVideoType <= EM_UDI_VID_STREAM_UNKNOWN)||(nVideoType >= EM_UDI_VID_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Video Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nVideoType);
		return  -1;
	}
			
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)) //��ȡ����
	{
		for (nVideoIndex = 0; nVideoIndex<nVideoCount; nVideoIndex++)
		{
			if(iIsVideoSupportDecodeType(nVideoIndex,nVideoType))
			{					
				nVidioID = nVideoIndex;
				break;
			}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}

	return nVidioID;
}

//���Ҳ�֧��nAudioType���͵�Audio Decoder[�ҵ�һ������]
//nAudioType�μ�CSUDIAUDStreamType_E, ����ΪEM_UDI_AUD_STREAM_AC3���⴦��
int PLAYER_searchAudioID_N(unsigned int *pnAudioType)         
{
	int nAudioCount = 0;
	int nAudioID = -1;
	int nAudioIndex = 0;

	if (pnAudioType == CSUDI_NULL)
	{
		CSTCPrint("[%s][line:%d] pnAudioType is NULL!!!\n\r", __FUNCTION__, __LINE__);
		return -1;
	}
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //��ȡ����Ƶ������������
	{
		for (nAudioIndex = 0; nAudioIndex<nAudioCount; nAudioIndex++)
		{
			unsigned int enAudType = EM_UDI_AUD_STREAM_UNKNOWN+1;
			for(;enAudType<EM_UDI_AUD_STREAMTYPE_NUM;enAudType++)
			{
				if ((!iIsAudioSupportDecodeType(nAudioIndex,enAudType)) && (!iIsAudioSupportBypassType(nAudioIndex,enAudType)))//��֧�ֽ��뼰͸��
				{
					nAudioID = nAudioIndex;
					break;
				}
			}
			if (nAudioID != -1)
			{
				*pnAudioType = enAudType;
				break;
			}
		}
	}	
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIAUDIOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	return nAudioID;
}

//���Ҳ�֧��nVideoType���͵�Video Decoder
//nVideoType�μ�CSUDIVIDStreamType_E
int PLAYER_searchVideoID_N(unsigned int *pnVideoType)   
{
	int nVideoCount = 0;
	int nVidioID = -1;
	int nVideoIndex = 0;
	//CSUDIVIDStreamType_E enmVid = EM_UDI_VID_STREAM_UNKNOWN;
	
	if (pnVideoType == CSUDI_NULL)
	{
		CSTCPrint("[%s][line:%d] pnVideoType is NULL!!!\n\r", __FUNCTION__, __LINE__);
		return -1;
	}
			
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)) //��ȡ����
	{
		for (nVideoIndex = 0; nVideoIndex<nVideoCount; nVideoIndex++)
		{
				unsigned int nVidType = EM_UDI_VID_STREAM_UNKNOWN+1;
				for (;nVidType<EM_UDI_VID_STREAMTYPE_NUM;nVidType++)
				{
					if(!iIsVideoSupportDecodeType(nVideoIndex,nVidType))
					{
						nVidioID = nVideoIndex;
						break;
					}
				}
				if (nVidioID != -1)
				{
					*pnVideoType = nVidType;
					break;
				}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	return nVidioID;
}

//����֧��nVideoSupport1��nVideoSupport2���͵�Video Decoder
//nVideoSupport1��nVideoSupport2��ֵ�μ�CSUDIAUDStreamType_E
int PLAYER_searchVideoID_Y_Y(int nVideoSupport1, int nVideoSupport2)             
{
	int nVideoCount = 0;
	int nVidioID = -1;
	int nVideoIndex = 0;

	if(nVideoSupport1<0)
	{
		CSTCPrint("[%s][line:%d] index Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nVideoSupport1);
		return -1;
	}

	if(nVideoSupport2<0)
	{
		CSTCPrint("[%s][line:%d] index Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nVideoSupport2);
		return -1;
	}
			
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)) //��ȡ����
	{
		for (nVideoIndex = 0; nVideoIndex<nVideoCount; nVideoIndex++)
		{
				if(iIsVideoSupportDecodeType(nVideoIndex,nVideoSupport1)  &&
					iIsVideoSupportDecodeType(nVideoIndex,nVideoSupport2))
				{
					nVidioID = nVideoIndex;
					break;
				}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	return nVidioID;
}

//����������
//����1 ��ʾdemux֧�ֵĹ������ͣ�CSUDIDEMUXWorkType_E���������
//����2 ��ʾ��Ƶ������֧�ֵ���Ƶ����
//����3 ��ʾ��Ƶ������֧�ֵ���Ƶ����
//���� 4 ��ʾ��������������ֱ����ע�룬ʱ�Ʋ��Ż���PVR����
CSUDI_Error_Code  PLAYER_openPlayerWithType(unsigned int dwStreamType,int uDemuxType, int eAudStreamType, int eVidStreamType, int ePlayerType,  CSUDI_HANDLE *phPlayer)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	int nDemuxID = -1;
	int nAudioID = -1;
	int nVideoID = -1;

	if((dwStreamType < EM_STREAM_VIDEO)||(dwStreamType > (EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR|EM_STREAM_SUBTITLE|EM_STREAM_TELETEXT)))
	{
		CSTCPrint("[%s][line:%d] dwStreamType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, dwStreamType);
		return  CSUDI_FAILURE;
	}

	if((uDemuxType < EM_UDI_DEMUX_PLAY)||(uDemuxType > (EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_RECORD|EM_UDI_DEMUX_DMA|EM_UDI_DEMUX_INJECT)))
	{
		CSTCPrint("[%s][line:%d] uDemuxType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, uDemuxType);
		return CSUDI_FAILURE;
	}

	if((ePlayerType < EM_UDIPLAYER_LIVE)||(ePlayerType > EM_UDIPLAYER_MAXPLAYERTYPE))
	{
		CSTCPrint("[%s][line:%d] Live Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, ePlayerType);
		return CSUDI_FAILURE;
	}

	if (phPlayer == CSUDI_NULL)
	{
		CSTCPrint("[%s][line:%d] phPlayer is NULL!!!\n\r", __FUNCTION__, __LINE__);
		return CSUDI_FAILURE;
	}
	
	switch(dwStreamType)
	{
		case (EM_STREAM_VIDEO):
		{
			nDemuxID = PLAYER_searchDemuxID_Y(uDemuxType);
			nVideoID = PLAYER_searchVideoID_Y(eVidStreamType);
			if (nDemuxID == -1 || nVideoID == -1)
			{
				*phPlayer=CSUDI_NULL;
				return CSUDI_FAILURE;
			}
		}
		break;
		case (EM_STREAM_AUDIO):
		{
			nDemuxID = PLAYER_searchDemuxID_Y(uDemuxType);
			nAudioID = PLAYER_searchAudioID_Y(eAudStreamType);
			if (nDemuxID == -1 || nAudioID == -1)
			{
				*phPlayer=CSUDI_NULL;
				return CSUDI_FAILURE;
			}
		}
		break;
		case (EM_STREAM_VIDEO|EM_STREAM_AUDIO):
		{
			nDemuxID = PLAYER_searchDemuxID_Y(uDemuxType);
			nAudioID = PLAYER_searchAudioID_Y(eAudStreamType);
			nVideoID = PLAYER_searchVideoID_Y(eVidStreamType);
			if (nDemuxID == -1 || nAudioID == -1 || nVideoID == -1)
			{
				*phPlayer=CSUDI_NULL;
				return CSUDI_FAILURE;
			}
		}
		break;
		default:
			CSTCPrint("ERROR play type !!!!!\r\n");
			*phPlayer=CSUDI_NULL;
			return CSUDI_FAILURE;
	}
	
	CSTCPrint("[in PLAYER_openPlayerWithType ..........]\n\r");
	CSTCPrint("nDemuxID=%d ----------\n\r",nDemuxID);
	CSTCPrint("nAudioID=%d ----------\n\r",nAudioID);
	CSTCPrint("nVideoID=%d ----------\n\r",nVideoID);

	sPlayerChnl.m_nDemux = nDemuxID;
	sPlayerChnl.m_nAudioDecoder = nAudioID;
	sPlayerChnl.m_nVideoDecoder = nVideoID;

	initAVInfo(nAudioID, nVideoID);

	nRet = CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer);
	if (CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("[%s][line:%d] CSUDIPLAYEROpen failed!!!\n\r", __FUNCTION__, __LINE__);
	}
	return nRet;
}
//����������
//����1 ��ʾdemux֧�ֵĹ������ͣ�CSUDIDEMUXWorkType_E���������
//����2 ��ʾ��Ƶ������֧�ֵ���Ƶ����
//����3 ��ʾ��Ƶ������֧�ֵ���Ƶ����
//���� 4 ��ʾ��������������ֱ����ע�룬ʱ�Ʋ��Ż���PVR����
CSUDI_HANDLE PLAYER_openPlayer(int uDemuxType, int eAudStreamType, int eVidStreamType, int ePlayerType)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nDemuxID = -1;
	int nAudioID = -1;
	int nVideoID = -1;

	if((uDemuxType < EM_UDI_DEMUX_PLAY)||(uDemuxType > (EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_RECORD|EM_UDI_DEMUX_DMA|EM_UDI_DEMUX_INJECT)))
	{
		CSTCPrint("[%s][line:%d] uDemuxType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, uDemuxType);
		return CSUDI_NULL;
	}

	if((ePlayerType < EM_UDIPLAYER_LIVE)||(ePlayerType > EM_UDIPLAYER_MAXPLAYERTYPE))
	{
		CSTCPrint("[%s][line:%d] Live Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, ePlayerType);
		return CSUDI_NULL;
	}
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_Y(eAudStreamType);
	nVideoID=PLAYER_searchVideoID_Y(eVidStreamType);

	CSTCPrint("[in PLAYER_openPlayer ..........]\n\r");

	CSTCPrint("nDemuxID=%d ----------\n\r",nDemuxID);
	CSTCPrint("nAudioID=%d ----------\n\r",nAudioID);
	CSTCPrint("nVideoID=%d ----------\n\r",nVideoID);

	if (nDemuxID==-1 ||( (nAudioID==-1) && (nVideoID==-1)))
	{
		hPlayer=CSUDI_NULL;
		CSTCPrint("[%s][line:%d] �����Ҳ������ʵĽ������������ļ�����\n\r", __FUNCTION__, __LINE__);
	}
	else
	{
		sPlayerChnl.m_nDemux=nDemuxID;
		sPlayerChnl.m_nAudioDecoder=nAudioID;
		sPlayerChnl.m_nVideoDecoder=nVideoID;

		initAVInfo(nAudioID, nVideoID);
		
		if (CSUDI_SUCCESS != CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,&hPlayer))
		{
			hPlayer =CSUDI_NULL;
			CSTCPrint("[%s][line:%d] CSUDIPLAYEROpen failed!!!\n\r", __FUNCTION__, __LINE__);
		}
	}
	return hPlayer;
	
}

///////////////�����������������治ͬ����Щ����ͬʱ���ش������
CSUDI_HANDLE  secondOpenPlayer(int uDemuxType,int eAudStreamType,int eVidStreamType,int ePlayerType,CSUDI_Error_Code * pnOpenError)
{
	CSUDIPlayerChnl_S  psPlayerChnl;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int nDemuxID = -1;
	int nAudioID = -1;
	int nVideoID = -1;

	if((uDemuxType < EM_UDI_DEMUX_PLAY)||(uDemuxType > (EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_RECORD|EM_UDI_DEMUX_DMA|EM_UDI_DEMUX_INJECT)))
	{
		CSTCPrint("[%s][line:%d] uDemuxType Error!!!(%d)\n\r", __FUNCTION__, __LINE__, uDemuxType);
		return CSUDI_NULL;
	}

	if((eAudStreamType < EM_UDI_AUD_STREAM_UNKNOWN)||(eAudStreamType > EM_UDI_AUD_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Audio Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, eAudStreamType);
		return CSUDI_NULL;
	}

	if((eVidStreamType < EM_UDI_VID_STREAM_UNKNOWN)||(eVidStreamType > EM_UDI_VID_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Video Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, eVidStreamType);
		return CSUDI_NULL;
	}

	if((ePlayerType < EM_UDIPLAYER_LIVE)||(ePlayerType > EM_UDIPLAYER_MAXPLAYERTYPE))
	{
		CSTCPrint("[%s][line:%d] Live Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, ePlayerType);
		return CSUDI_NULL;
	}

	if (pnOpenError == CSUDI_NULL)
	{
		CSTCPrint("[%s][line:%d] pnOpenError is NULL!!!\n\r", __FUNCTION__, __LINE__);
		return CSUDI_NULL;
	}
		
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_Y(eAudStreamType);
	nVideoID=PLAYER_searchVideoID_Y(eVidStreamType);

	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

	if (nDemuxID==-1||nAudioID==-1||nVideoID==-1)
	{
		CSTCPrint("[%s][line:%d] �����Ҳ������ʵĽ������������ļ�����\n\r", __FUNCTION__, __LINE__);
		return CSUDI_NULL;
	}
	
	*pnOpenError=CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer);

	return hPlayer;
}
static CSUDI_BOOL g_sSourceEventChange = CSUDI_FALSE;
void  PLAYER_iInitCallbackPara()
{
	g_sSourceEventChange = CSUDI_FALSE;
}
CSUDI_BOOL PLAYER_iIsSourceEventCallback(int nWaitTime)
{
	CSUDIOSThreadSleep(nWaitTime);
	
	return g_sSourceEventChange;
}
//ָ���¼����͵Ļص�����
void fnPlayerCallback(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{

	int tmp;
	tmp=*((int *)pvUserData);
	
	if( eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{
		g_nCallBackNum=g_nCallBackNum+tmp;
		g_nCallBackTime++;
	}
}

//ָ���¼����͵Ļص�����
void fnPlayerCallback1(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)
{
	if (EM_UDIPLAYER_VIDEO_SOURCE_CHANGE == eEvent)
	{
		CSTCPrint("EM_UDIPLAYER_VIDEO_SOURCE_CHANGE.........\r\n");
		g_sSourceEventChange = CSUDI_TRUE;
	}
	return;
}

//ָ���¼����͵Ļص�����
void fnPlayerCallback2(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)
{
	if (EM_UDIPLAYER_BEGIN_OF_STREAM == eEvent)
	{
		CSTCPrint("EM_UDIPLAYER_BEGIN_OF_STREAM.........!!!!!\r\n");
	
	}
	return;
}

CSUDI_BOOL PLAYER_iIsAudioSupportDecodeType(int nStreamType)
{
	int nAudioDecCount = 0;
	int ii;
	
	if((nStreamType <= EM_UDI_AUD_STREAM_UNKNOWN)||(nStreamType >= EM_UDI_AUD_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Audio Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nStreamType);
		return  CSUDI_FALSE;
	}
	
	if(CSUDI_SUCCESS != CSUDIAUDIOGetCount(&nAudioDecCount) && nAudioDecCount != 0)
	{
		CSTCPrint("[%s][line:%d] Audio Count Failure !!!\n\r", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}

	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		if(iIsAudioSupportDecodeType(ii,nStreamType))	
			return CSUDI_TRUE;
	}
	return CSUDI_FALSE;
}

CSUDI_BOOL PLAYER_iIsVideoSupportDecodeType(int nStreamType)
{
	int nVIdDecCount = 0;
	int ii;
	if((nStreamType <= EM_UDI_VID_STREAM_UNKNOWN)||(nStreamType >= EM_UDI_VID_STREAMTYPE_NUM))
	{
		CSTCPrint("[%s][line:%d] Video Type Error!!!(%d)\n\r", __FUNCTION__, __LINE__, nStreamType);
		return  CSUDI_FALSE;
	}
	
	if(CSUDI_SUCCESS != CSUDIVIDEOGetCount(&nVIdDecCount) || nVIdDecCount == 0)
	{
		CSTCPrint("[%s][line:%d] Audio Count Failure !!!\n\r", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}

	for(ii = 0; ii < nVIdDecCount; ii++)
	{
		if(iIsVideoSupportDecodeType(ii,nStreamType))	
			return CSUDI_TRUE;
	}
	return CSUDI_FALSE;
}

