#include "cs_recordtest.h"
#include <stdio.h>
#include <string.h>
#include "cs_testkit.h"

int FindDemuxSupportRec(void)
{
	int nDemuxId = -1;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nDemuxCount = 0;
	int i = 0;
	CSUDIDEMUXCapability_S sCapabilityInfo;
	
	eRet = CSUDIDEMUXGetCount(&nDemuxCount);
	if ((CSUDI_SUCCESS == eRet) && (nDemuxCount > 0))
	{
		for (i=0; i<nDemuxCount; i++)
		{
			memset(&sCapabilityInfo, 0, sizeof(sCapabilityInfo));
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo))
			{
				if (sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_RECORD)
				{
					nDemuxId = i;
					break;
				}
			}
		}
	}

	CSTCPrint("����¼�Ƶ�demuxId=%d\n", nDemuxId);

	return nDemuxId;
}

CSUDI_BOOL FindAllDemuxSupportRec(int* pOutDemuxArray, int nArraySize, int* pnActualCnt)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nDemuxCount = 0;
	int i = 0;
	int j = 0;
	CSUDIDEMUXCapability_S sCapabilityInfo;

	CSASSERT(pOutDemuxArray != CSUDI_NULL);
	CSASSERT(nArraySize > 0);
	CSASSERT(pnActualCnt != CSUDI_NULL);

	if (pOutDemuxArray == CSUDI_NULL || nArraySize <= 0 || pnActualCnt == CSUDI_NULL)
	{
		CSTCPrint("��������\n");
		return bRet;
	}
	
	eRet = CSUDIDEMUXGetCount(&nDemuxCount);
	if ((CSUDI_SUCCESS == eRet) && (nDemuxCount > 0))
	{
		for (i=0; i<nDemuxCount; i++)
		{
			memset(&sCapabilityInfo, 0, sizeof(sCapabilityInfo));
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo))
			{
				if (sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_RECORD)
				{
					if (i < nArraySize)
					{
						pOutDemuxArray[j] = i;
						j ++;
						CSTCPrint("DemuxId=%d֧��¼��\n", i);
					}
					else
					{
						CSTCPrint("���������С����\n");
						break;
					}
				}
			}
			else
			{
				CSTCPrint("��ȡ��%d��demux������ʧ��\n", i);
			}
		}
	}
	else
	{
		CSTCPrint("��ȡdemux����ʧ��, eRet=0x%x, nDemuxCount=%d\n", eRet, nDemuxCount);
	}

	if (j > 0)
	{
		*pnActualCnt = j;
		bRet = CSUDI_TRUE;
	}

	return bRet;
}

int FindDemuxNotSupportRec(void)
{
	int nDemuxId = -1;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nDemuxCount = 0;
	int i = 0;
	CSUDIDEMUXCapability_S sCapabilityInfo;
	
	eRet = CSUDIDEMUXGetCount(&nDemuxCount);
	if ((CSUDI_SUCCESS == eRet) && (nDemuxCount > 0))
	{
		for (i=0; i<nDemuxCount; i++)
		{
			memset(&sCapabilityInfo, 0, sizeof(sCapabilityInfo));
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo))
			{
				if ((sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_RECORD) == 0)
				{
					nDemuxId = i;
					break;
				}
				else
				{
					CSTCPrint("��ȡ��%d��demux������ʧ��\n", i);
				}
			}
		}
	}
	else
	{
		CSTCPrint("��ȡdemux����ʧ��, eRet=0x%x, nDemuxCount=%d\n", eRet, nDemuxCount);
	}	

	return nDemuxId;
}

int FindDemuxSupportPvrPlay(void)
{
	int nDemuxId = -1;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nDemuxCount = 0;
	int i = 0;
	CSUDIDEMUXCapability_S sCapabilityInfo;
	
	eRet = CSUDIDEMUXGetCount(&nDemuxCount);
	if ((CSUDI_SUCCESS == eRet) && (nDemuxCount > 0))
	{
		for (i=0; i<nDemuxCount; i++)
		{
			memset(&sCapabilityInfo, 0, sizeof(sCapabilityInfo));
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo))
			{
				if (sCapabilityInfo.m_dwConTypeMask&EM_UDI_DEMUX_INJECT_TS
					&& (sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY && sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_INJECT))
				{
					nDemuxId = i;
					break;
				}
			}
			else
			{
				CSTCPrint("��ȡ��%d��demux������ʧ��\n", i);
			}
		}
	}
	else
	{
		CSTCPrint("��ȡdemux����ʧ��, eRet=0x%x, nDemuxCount=%d\n", eRet, nDemuxCount);
	}

	return nDemuxId;
}

int FindDemuxSupportLivePlay(void)
{
	int nDemuxId = -1;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nDemuxCount = 0;
	int i = 0;
	CSUDIDEMUXCapability_S sCapabilityInfo;
	
	eRet = CSUDIDEMUXGetCount(&nDemuxCount);
	if ((CSUDI_SUCCESS == eRet) && (nDemuxCount > 0))
	{
		for (i=0; i<nDemuxCount; i++)
		{
			memset(&sCapabilityInfo, 0, sizeof(sCapabilityInfo));
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo))
			{
				if (sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY 
					&& sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA)
				{
					nDemuxId = i;
					break;
				}
			}
			else
			{
				CSTCPrint("��ȡ��%d��demux������ʧ��\n", i);
			}
		}
	}
	else
	{
		CSTCPrint("��ȡdemux����ʧ��, eRet=0x%x, nDemuxCount=%d\n", eRet, nDemuxCount);
	}

	CSTCPrint("����ֱ����demuxId=%d\n", nDemuxId);

	return nDemuxId;
}

int FindAudioDecoder(CSUDIAUDStreamType_E eAudioType)
{
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nAudioId = -1;
	int nAudioCount = 0;
	int i = 0;
	int j = 0;
	CSUDIAUDIOCapability_S sCapability;

	if (eAudioType <= EM_UDI_AUD_STREAM_UNKNOWN 
		|| eAudioType >= EM_UDI_AUD_STREAMTYPE_NUM)
	{
		CSTCPrint("[%s, %d] Error param, eAudioType=%d\n", __FUNCTION__, __LINE__, eAudioType);
		return -1;
	}

	eRet = CSUDIAUDIOGetCount(&nAudioCount);

	if ((CSUDI_SUCCESS != eRet) || (nAudioCount <= 0))
	{
		CSTCPrint("��ȡ��Ƶ����������ʧ��, eRet=0x%x, nAudioCount=%d\n", eRet, nAudioCount);
		return -1;
	}

	for (i=0; i<nAudioCount; i++)
	{
		memset(&sCapability, 0, sizeof(sCapability));

		if (CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sCapability))
		{
			for (j=0; j<EM_UDI_AUD_STREAMTYPE_NUM; j++)
			{
				if ((EM_UDI_AUD_STREAM_UNKNOWN == sCapability.m_sSupportDecode[j])
					|| (EM_UDI_AUD_STREAMTYPE_NUM == sCapability.m_sSupportDecode[j]))
				{
					break;
				}
				
				if (eAudioType == sCapability.m_sSupportDecode[j])
				{
					nAudioId = i;
					break;
				}
			}
		}
		else
		{
			CSTCPrint("��ȡ��%d��audio������ʧ��\n", i);
		}

		if (nAudioId != -1)
		{
			break;
		}
	}

	return nAudioId;
}

int FindVideoDecoder(CSUDIVIDStreamType_E eVideoType)
{
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nVideoId = -1;
	int nVideoCount = 0;
	int i = 0;
	int j = 0;
	CSUDIVIDEOCapability_S sCapability;

	eRet = CSUDIVIDEOGetCount(&nVideoCount);

	if ((CSUDI_SUCCESS != eRet) || (nVideoCount <= 0))
	{
		CSTCPrint("��ȡ��Ƶ����������ʧ��, eRet=0x%x, nVideoCount=%d\n", eRet, nVideoCount);
		return -1;
	}

	for (i=0; i<nVideoCount; i++)
	{
		memset(&sCapability, 0, sizeof(sCapability));
		if (CSUDI_SUCCESS == CSUDIVIDEOGetCapability(i, &sCapability))
		{
			for (j=0; j<EM_UDI_VID_STREAMTYPE_NUM; j++)
			{
				if ((EM_UDI_VID_STREAM_UNKNOWN == sCapability.m_eStreamType[j])
					|| (EM_UDI_VID_STREAMTYPE_NUM == sCapability.m_eStreamType[j]))
				{
					break;
				}

				if (eVideoType == sCapability.m_eStreamType[j])
				{
					nVideoId = i;
					break;
				}
			}
		}
		else
		{
			CSTCPrint("��ȡ��%d��video������ʧ��\n", i);
		}

		if (nVideoId != -1)
		{
			break;
		}
	}

	return nVideoId;
}

unsigned long GetRecFileSize(const char* pcFileName)
{
	CSUDIFSFileStat_S sFileStat;
	CSUDI_HANDLE hDir = CSUDI_NULL;
	CSUDIFSDirent_S *psDirent = CSUDI_NULL;
	unsigned long lTotalSize = 0;
	char acFullFileName[CSUDI_MAX_FILE_NAME_LEN] = {0};

	if (pcFileName == CSUDI_NULL || pcFileName[0] == 0)
	{
		CSTCPrint("�ļ�������Ϊ��\n");
		return 0;
	}
	
	if (CSUDIFSStat(pcFileName, &sFileStat) != 0)
	{
		CSTCPrint("¼�Ƶ��ļ�:%s ������\n", pcFileName);
		return 0;
	}

	if (CSUDIFS_ISDIR(sFileStat.m_lMode))
	{
		hDir = CSUDIFSOpenDir(pcFileName);

		if (hDir == CSUDI_NULL)
		{
			CSTCPrint("���ļ���ʧ��\n");
			return 0;
		}
		
		while (1)
		{
			psDirent = CSUDIFSReadDir(hDir);
			if (psDirent != CSUDI_NULL)
			{
				if (0 != strcmp(psDirent->m_cDirName, ".") && 0 != strcmp(psDirent->m_cDirName, ".."))
				{
					sprintf(acFullFileName, "%s/%s", pcFileName, psDirent->m_cDirName);
					
					memset(&sFileStat, 0, sizeof(sFileStat));
					if (CSUDIFSStat(acFullFileName, &sFileStat) == 0)
					{
						lTotalSize += sFileStat.m_lSize;
						//CSTCPrint("���ļ�: %s, ��С: %ld, �ܴ�С: %ld\n", psDirent->m_cDirName, sFileStat.m_lSize, lTotalSize);
					}
				}
			}
			else
			{
				break;
			}
		}

		CSUDIFSCloseDir(hDir);
	}
	else if (CSUDIFS_ISREG(sFileStat.m_lMode))
	{
		lTotalSize = sFileStat.m_lSize;
	}
	else
	{
		CSTCPrint("�ļ����Դ���\n");
	}
	
	CSTCPrint("�ļ��ܴ�СΪ: %ld\n", lTotalSize);

	return lTotalSize;
}

CSUDI_BOOL CheckRecResult(CSUDIRecStartParam_S sRecStartPar)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_MAXPLAYERTYPE;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIVIDStreamType_E eVidStreamType = EM_UDI_VID_STREAM_UNKNOWN;
	CSUDIAUDStreamType_E eAudStreamType = EM_UDI_AUD_STREAM_UNKNOWN;
	int i = 0;

	if (GetRecFileSize(sRecStartPar.m_szFileName) <= 0)
	{
		CSTCPrint("¼���ļ���С��鲻ͨ��\n");
		return CSUDI_FALSE;
	}

	switch(sRecStartPar.m_eRecType)
	{
		case EM_UDIRECORD_TYPE_CHNL:
		{
			ePlayerType = EM_UDIPLAYER_PVR;
			break;
		}

		case EM_UDIRECORD_TYPE_TSHIFT:
		{
			ePlayerType = EM_UDIPLAYER_TSHIFT;
			break;
		}

		default:
		{
			CSTCPrint("�����¼������:%d\n", sRecStartPar.m_eRecType);
			return CSUDI_FALSE;
		}
	}

	for (i=0; i<sRecStartPar.m_nCount; i++)
	{
		if (sRecStartPar.m_psPids[i].m_eContentType == EM_UDI_CONTENT_VIDEO)
		{
			eVidStreamType = sRecStartPar.m_psPids[i].m_uStreamType.m_eVideoType;
		}
		else if (sRecStartPar.m_psPids[i].m_eContentType == EM_UDI_CONTENT_AUDIO)
		{
			eAudStreamType = sRecStartPar.m_psPids[i].m_uStreamType.m_eAudioType;
		}
		else
		{
			CSTCPrint("StreamInfo[%d].ContentType = %d\n", i, sRecStartPar.m_psPids[i].m_eContentType);
		}
	}

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	if (sPlayerChnl.m_nDemux < 0)
	{
		CSTCPrint("δ�ҵ�֧��¼�Ƶ�demux\n");
		return CSUDI_FALSE;
	}
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(eAudStreamType);
	if (sPlayerChnl.m_nAudioDecoder < 0)
	{
		CSTCPrint("δ�ҵ�֧�� %d ��Ƶ�����͵���Ƶ������\n", eAudStreamType);
		return CSUDI_FALSE;
	}
	
	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(eVidStreamType);
	if (sPlayerChnl.m_nVideoDecoder < 0)
	{
		CSTCPrint("δ�ҵ�֧�� %d ��Ƶ�����͵���Ƶ������\n", eVidStreamType);
		return CSUDI_FALSE;
	}	

	eRet = CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer);
	if (eRet != CSUDI_SUCCESS || hPlayer == CSUDI_NULL)
	{
		CSTCPrint("����������ʧ��\n");
		return CSUDI_FALSE;
	}

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sRecStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	memcpy(&(sPlaybackPar.m_stEncryptParam), &(sRecStartPar.m_stEncryptParam), sizeof(CSUDIStreamEncryptParam_S));
	
	eRet = CSUDIPLAYERSetStream(hPlayer, sRecStartPar.m_psPids, sRecStartPar.m_nCount, &sPlaybackPar);
	if (eRet != CSUDI_SUCCESS)
	{
		CSTCPrint("���ý�Ŀ��Ϣʧ��, eRet=0x%x\n", eRet);
	}

	eRet = CSUDIPLAYERStart(hPlayer);
	if (eRet != CSUDI_SUCCESS)
	{
		CSTCPrint("��������ʧ��, eRet=0x%x\n", eRet);

		if (hPlayer != CSUDI_NULL)
		{
			if (CSUDI_SUCCESS != CSUDIPLAYERClose(hPlayer))
			{
				CSTCPrint("����¼����ʧ��\n");
			}
		}

		return CSUDI_FALSE;
	}
	else
	{
		CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);
	}

	CSTCPrint("�����Ƿ�����\n");
	bRet = CSTKWaitYes();

	eRet = CSUDIPLAYERStop(hPlayer);
	if (eRet != CSUDI_SUCCESS)
	{
		CSTCPrint("ֹͣ����ʧ��\n");
		bRet = CSUDI_FALSE;
	}

	if (hPlayer != CSUDI_NULL)
	{
		if (CSUDI_SUCCESS != CSUDIPLAYERClose(hPlayer))
		{
			CSTCPrint("����¼����ʧ��\n");
			bRet = CSUDI_FALSE;
		}
	}

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	return bRet;
}

CSUDI_HANDLE LivePlayStart(CSUDIPlayerChnl_S* psPlayerChnl, CSUDIStreamInfo_S* psAudioStream, CSUDIStreamInfo_S* psVideoStream)
{
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[2];

	if (psAudioStream == CSUDI_NULL || psVideoStream == CSUDI_NULL)
	{
		CSTCPrint("����Ƶ��Ϣ����Ϊ��\n");
		return CSUDI_NULL;
	}

	if (psPlayerChnl == CSUDI_NULL)
	{
		CSTCPrint("����ͨ���豸��Ϣ����Ϊ��\n");
		return CSUDI_NULL;
	}

	if (psPlayerChnl->m_nDemux < 0)
	{
		CSTCPrint("�Ƿ���demuxId: %d\n", psPlayerChnl->m_nDemux);
		return CSUDI_NULL;
	}

	if (CSUDI_SUCCESS != CSUDIDEMUXConnectTuner(UDI2_TUNER0_ID, psPlayerChnl->m_nDemux))
	{
		CSTCPrint("֧��ֱ����demux��tuner����ʧ��, demuxId=%d, tunerId=%d\n", psPlayerChnl->m_nDemux, UDI2_TUNER0_ID);
		return CSUDI_NULL;
	}

	eRet = CSUDIPLAYEROpen(psPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer);
	if ((CSUDI_SUCCESS != eRet) || (hPlayer == CSUDI_NULL))
	{
		CSTCPrint("����������ʧ��, eRet=0x%x\n", eRet);
		
		CSUDIDEMUXDisconnectTuner(UDI2_TUNER0_ID, psPlayerChnl->m_nDemux);
		
		return CSUDI_NULL;
	}

	memcpy(&asStreamInfo[0], psAudioStream, sizeof(CSUDIStreamInfo_S));
	memcpy(&asStreamInfo[1], psVideoStream, sizeof(CSUDIStreamInfo_S));

	eRet = CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL);
	if (CSUDI_SUCCESS == eRet)
	{
		eRet = CSUDIPLAYERStart(hPlayer);
		if (CSUDI_SUCCESS != eRet)
		{
			CSTCPrint("������Ŀ����ʧ��, eRet=0x%x\n", eRet);
		}
		else
		{
			CSUDIVIDEOShow(psPlayerChnl->m_nVideoDecoder, CSUDI_TRUE);
		}
	}
	else
	{
		CSTCPrint("���ò��Ž�Ŀ��Ϣʧ��, eRet=0x%x\n", eRet);
	}
	
	if ((CSUDI_SUCCESS != eRet) && (hPlayer != CSUDI_NULL))
	{
		CSUDIPLAYERClose(hPlayer);
		hPlayer = CSUDI_NULL;

		CSUDIDEMUXDisconnectTuner(UDI2_TUNER0_ID, psPlayerChnl->m_nDemux);
	}

	return hPlayer;
}

void ClearFile(const char* pcFileName)
{
	CSUDIFSFileStat_S sFileStat;
	CSUDI_HANDLE hDir = CSUDI_NULL;
	CSUDIFSDirent_S *psDirent = CSUDI_NULL;
	char acFullFileName[CSUDI_MAX_FILE_NAME_LEN] = {0};
	
	if (pcFileName == CSUDI_NULL || pcFileName[0] == 0)
	{
		CSTCPrint("�ļ�����Ӧ��Ϊ��\n");
		return;
	}

	memset(&sFileStat, 0, sizeof(sFileStat));
	if (CSUDIFSStat(pcFileName, &sFileStat) == 0)
	{
		if (CSUDIFS_ISDIR(sFileStat.m_lMode))
		{
			hDir = CSUDIFSOpenDir(pcFileName);

			if (hDir == CSUDI_NULL)
			{
				CSTCPrint("���ļ���ʧ��\n");
				return;
			}
			
			while (1)
			{
				psDirent = CSUDIFSReadDir(hDir);
				if (psDirent != CSUDI_NULL)
				{
					if (0 != strcmp(psDirent->m_cDirName, ".") && 0 != strcmp(psDirent->m_cDirName, ".."))
					{
						if (psDirent->m_ucDirType == 0)
						{
							sprintf(acFullFileName, "%s/%s", pcFileName, psDirent->m_cDirName);
							if (CSUDIFSRemove(acFullFileName) != 0)
							{
								CSTCPrint("ɾ���ļ�(%s)ʧ��\n", psDirent->m_cDirName);
							}
						}
						else
						{
							CSTCPrint("�������ļ���\n");
						}
					}
				}
				else
				{
					break;
				}
			}

			CSUDIFSCloseDir(hDir);

			if (CSUDIFSRmdir(pcFileName) != 0)
			{
				CSTCPrint("ɾ���ļ���(%s)ʧ��\n", pcFileName);
			}
		}
		else if (CSUDIFS_ISREG(sFileStat.m_lMode))
		{
			if (CSUDIFSRemove(pcFileName) != 0)
			{
				CSTCPrint("ɾ���ļ�(%s)ʧ��\n", pcFileName);
			}
		}
		else
		{
			CSTCPrint("�ļ����Դ���\n");
		}
	}
	else
	{
		CSTCPrint("��ȡ�ļ�(%s)����ʧ��\n", pcFileName);
	}
}

CSUDI_BOOL PreparePlayTestFile(CSUDIRecType_E eRecType, const char* pcFileName, RecordTestServiceInfo_S* psServiceInfo)
{
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIFSFileStat_S sFileStat;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = LONG_REC_TIME;

	if (((eRecType != EM_UDIRECORD_TYPE_CHNL) && (eRecType != EM_UDIRECORD_TYPE_TSHIFT))
		|| (pcFileName == CSUDI_NULL) || (psServiceInfo == CSUDI_NULL))
	{
		CSTCPrint("��������\n");
		return CSUDI_FALSE;
	}

	memset(&sFileStat, 0, sizeof(sFileStat));
	if (CSUDIFSStat(pcFileName, &sFileStat) == 0)
	{
		CSTCPrint("�����ļ��Ѵ��ڣ�ֱ�ӽ��벥�Ų���\n");
		return CSUDI_TRUE;
	}
	
	CSTCPrint("�����ļ������ڣ��Ƚ���¼��\n");

	if (!CSTC_UDI2PortingLock(dwTunerId, psServiceInfo->m_pcTsFilename))
	{
		CSTCPrint("��Ƶʧ��\n");
		return CSUDI_FALSE;
	}

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	if (sRecChnl.m_nDemux < 0)
	{
		CSTCPrint("����֧��¼�Ƶ�demuxʧ��\n");
		return CSUDI_FALSE;
	}

	if (CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux) != CSUDI_SUCCESS)
	{
		CSTCPrint("tuner��demux����ʧ�ܣ�tunerId=%d, demuxId=%d\n", dwTunerId, sRecChnl.m_nDemux);
		return CSUDI_FALSE;
	}

	eRet = CSUDIRECOpen(&sRecChnl, &hRec);
	if (eRet == CSUDI_SUCCESS && hRec != CSUDI_NULL)
	{
		memset(&sStartPar, 0, sizeof(sStartPar));	
		sStartPar.m_eRecType = eRecType;
		sStartPar.m_bAppend = CSUDI_FALSE;
		sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
		sStartPar.m_nCount = 2;
		sStartPar.m_psPids[0].m_nPid = psServiceInfo->m_nAudioPid;
		sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
		sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = psServiceInfo->m_eAudStreamType;
		sStartPar.m_psPids[1].m_nPid = psServiceInfo->m_nVideoPid;
		sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
		sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = psServiceInfo->m_eVidStreamType;
		memcpy(sStartPar.m_szFileName, pcFileName, sizeof(sStartPar.m_szFileName));
		if (eRecType == EM_UDIRECORD_TYPE_TSHIFT)
		{
			sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
		}

		if (CSUDIRECStart(hRec, &sStartPar) == CSUDI_SUCCESS)
		{
			CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
			CSUDIOSThreadSleep(nRecTime);

			CSUDIRECStop(hRec);
		}
	}
	else
	{
		CSTCPrint("����¼����ʧ��, eRet=0x%x\n", eRet);
	}
	
	if (hRec != CSUDI_NULL)
	{
		CSUDIRECClose(hRec);
		hRec = CSUDI_NULL;
	}

	CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux);

	if (CSUDIFSStat(pcFileName, &sFileStat) != 0)
	{
		CSTCPrint("¼�Ʋ����ļ�ʧ��\n");
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

CSUDI_BOOL LivePlayStop(CSUDI_HANDLE hPlayer, int nDemuxId)
{	
	if (hPlayer == CSUDI_NULL)
	{
		CSTCPrint("�������������ΪCSUDI_NULL\n");
		return CSUDI_FALSE;
	}

	if (CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer))
	{
		CSTCPrint("ֹͣ����ʧ��\n");
	}

	if (CSUDI_SUCCESS != CSUDIPLAYERClose(hPlayer))
	{
		CSTCPrint("���ٲ�����ʧ��\n");
		return CSUDI_FALSE;
	}

	if (nDemuxId < 0)
	{
		CSTCPrint("�Ƿ���demuxId: %d\n", nDemuxId);
		return CSUDI_FALSE;
	}
	else
	{
		if (CSUDI_SUCCESS != CSUDIDEMUXDisconnectTuner(UDI2_TUNER0_ID, nDemuxId))
		{
			CSTCPrint("�Ͽ�demux��tuner������ʧ�ܣ�demuxId=%d, tunerId=%d\n", nDemuxId, UDI2_TUNER0_ID);
			return CSUDI_FALSE;
		}
	}
	
	return CSUDI_TRUE;
}

