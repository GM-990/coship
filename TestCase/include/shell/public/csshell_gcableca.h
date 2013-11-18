#ifndef CSSHELL_GCABLECA_H
#define CSSHELL_GCABLECA_H

#include "csshell_ca.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define G_MAX_OPERATOR_NUM       (4)         /*������Ӫ�̸���*/
#define G_MAX_SLOT_NUM       (20)   /*���洢�����Ǯ����*/
#define G_MAX_OPERATORINFO_LEN (20)
typedef struct _G_CaEntitle
{
	DWORD 	m_dwProductID;
	char 	m_acProductName[32];
	char 	m_cStartTime[64];
	char 	m_cEndTime[64];
} G_CaEntitle_S;

typedef struct _CaOperatorInfo_S
{
	WORD m_dwOperatorid;
	BYTE	m_acOperatorInfo[G_MAX_OPERATORINFO_LEN+1];   /*��Ӫ��˽����Ϣ*/	
}G_CaOperatorInfo_S;

typedef struct _CaOperatorIds_S               /*��Ӫ��ID*/
{
	WORD	m_wOperatorIdNum;
	G_CaOperatorInfo_S m_stOperatorsInfo[G_MAX_OPERATOR_NUM];
} G_CaOperatorIds_S;

typedef struct _CaWallet_S               /*Ǯ��*/
{
	WORD   m_nID;
	WORD	m_nBalance;
	WORD	m_nRemainder;
} G_CaWallet_S;

typedef struct _CaSlotId_S          /*Ǯ��ID*/
{
	WORD	m_wSlotNum;
	G_CaWallet_S m_acSlotIds[G_MAX_SLOT_NUM];
}G_CaSlotId_S;

typedef struct _CaViewedIPP_S
{
	int m_nOperatorId;
	char m_acProductName[64];
	char m_acStartTime[64];
	char m_acEndTime[64];
    int m_nBookedPrice;
	int m_nBookedPriceType;
}G_CaIPPViewedInfo_S;

typedef struct _CaIPPInfo_S
{
	int m_nOperatorId;
	int m_nWalletId;
	int m_nProductId;	
	int m_nType; //ippv,ippt
	int m_nEcmPid;	
	int m_nPriceNum;
	int m_nPrice[4]; //�۸�������ԡ�
	char m_acProductName[32];
	char m_acStartTime[32];
	char m_acEndTime[64];
}G_CaIPPInfo_S;


typedef struct 
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellGCableCAInitParam_S)
	
	/*	
	@brief ��ȡ��ǰCA���ƣ���������CA����
	@param[out] pcCAName �洢ca���̵�����
	@param[in/out] pnLen �������ʱ��ʾpcCANameָ���buffer���ȣ��������������� pcCAName��ʵ�ʳ���
	@return �ɹ�����0��ʧ��-1
	*/
	int (* m_fnGCableCAGetName)(char* pcCAName, int *pnLen);

	/*	
	@brief ��ȡ��ǰ�����ܿ�����
	@param[out] pcCardId �洢��ȡ�������ܿ�����
	@param[in/out] pnLen �������ʱ��ʾpcCardIdָ���buffer���ȣ��������������� pcCardId�п��ŵ�ʵ�ʳ���
	@return �ɹ�����0��ʧ��-1
	*/
	int (* m_fnGCableCAGetIcNo)(char* pcCardId, int *pnLen);

	/**
	@brief��ȡ��ǰ���ܿ�PIN���Ƿ�����
	@return  1Ϊ���ܿ���������0Ϊ���ܿ�δ������-1Ϊ���ն˵�CAϵͳ��֧��PIN����������.
	*/
	int (* m_fnGCableCAPinLocked)();

	/**
	@brief ��ȡ��ǰCA�汾��
	@param[out] pcVersion ��ǰca�İ汾��
	@param[in/out] pnLen �������ʱ��ʾpcVersionָ���buffer���ȣ��������������� pcVersion��ʵ�ʳ���
	@return  �ɹ�����0��ʧ��-1
	*/
	int (* m_fnGCableCAVersion)(char* pcVersion, int *pnLen);

	/**
	@brief ��ȡ���ܿ��е�������루ȡ��������еĵ�16λ��
	@return  ���ܿ��е�������,����0��ʾ��������Ч
	*/
	int (* m_fnGCableCAGetRegionCode)();

	/**
	@brief �޸�PIN��
	@param[in] nPinType: PIN������. 0Ϊ��ĸ����PIN�������ùۿ�ʱ��ͼ��𣩣�1ΪǮ������PIN����IPP����ʱ�����PIN����-1Ϊ���ն˵�CAϵͳ��֧�ֶ������͵�PIN�롣
	@param[in] nOldPin: ��PIN��
	@param[in] nNewPin: ��PIN��
	@return  �ɹ�����0��ʧ��-1
	*/
	int  (* m_fnGCableCAChangePin)(int nPinType, int nOldPin,  int nNewPin);

	/**
	@brief �޸����ܿ��ۿ�����
	@param[in] dwPin: ���ܿ�PIN�룬�����Ǹ�ĸ�������͵�PIN��
	@param[in] cRatingValue: ���ܿ��ۿ����𣬷�Χ3~18 
	@return  0��ʾ�ɹ���������ʾʧ�ܣ�-1��ʾ��֧�֡�
	*/
	int  (* m_fnGCableCASetRating)(DWORD dwPin, BYTE cRatingValue);

	/**
	@brief ��ȡ��ǰ���ܿ��ۿ����� 
	@return  ���ڵ���0����ȡ�ɹ����صĹۿ����𣨷�Χһ��Ϊ3-18����-1Ϊ���ն˵�CAϵͳ��֧��;-2Ϊ��ȡʧ��
	*/
	int (* m_fnGCableCAGetRating)();

	/**
	@brief �޸����ܿ��ۿ�����
	@param[in] dwPin: ���ܿ�PIN��.
	@param[in] pcWorkTime: ���ܿ��ۿ�ʱ��.��ʽΪ"hh:mm-hh:mm"����"07:30-22:59"��ʾ������7��30��������22��59��
	@param[in] nLen:�������pcWorkTime�ĳ���
	@return   0��ʾ�ɹ���������ʾʧ�ܣ�-1��ʾ��֧�֡�
	*/
	int  (* m_fnGCableCASetWorkTime)(DWORD dwPin,  char*pcWorkTime, int nLen);

	/**
	@brief ��ȡ��ǰ���ܿ��ۿ�ʱ��
	@param[out] pcWorkTime: ���ܿ��ۿ�ʱ��.��ʽΪ"hh:mm-hh:mm" ��"07:30-22:59"����ʾ������7��30��������22��59��
	@param[in/out] pnLen �������ʱ��ʾpcCardIdָ���buffer���ȣ��������������� pcCardId�п��ŵ�ʵ�ʳ���
	@return   0��ʾ�ɹ���������ʾʧ�ܣ�-1��ʾ��֧�֡�
	@note 
	*/
	int (* m_fnGCableCAGetWorkTime)(char *pcWorkTime, int* pnLen);

	/**
	@brief ȡ��������Ӫ����Ϣ
	@param[out] pstOperatorId: ��Ӫ����Ϣ
	@return  0��ʾ�ɹ���������ʾʧ�ܣ�-1��ʾ��֧�֡�
	@note
	*/
	int (* m_fnGCableCAGetOperators)(G_CaOperatorIds_S * pstOperatorId);

	/**
	@brief  ��ȡָ����Ӫ�̵�����Ǯ��
	@param[in] wOperatorsId:ָ����Ӫ�̵�ID��
	@param[out] pstSlotinfo: Ǯ����Ϣ��
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (* m_fnGCableCAGetWallets)(WORD wOperatorsId,  G_CaSlotId_S * pstSlotinfo);

	int (* m_fnGCableCAGetEntitlesCount)(WORD wOperatorsId);

	/**
	@brief  ������Ӫ��IDȡ��������ͨ��Ȩ��Ŀ����
	@param[in] nEntitleIndex:ָ����Ӫ�̵�ID��
	@param[out] pstEntitleinfo: ��Ȩ��Ϣ��
	@return 0��ʾ�ɹ���������ʾʧ��
	*/
	int (* m_fnGCableCAGetEntitle)(int nEntitleIndex, G_CaEntitle_S * pstEntitleinfo);

	int (* m_fnGCableCAGetViewdIPPCount)();

	/**
	@brief  ��ȡ�ѹۿ�����Ipp��Ŀ
	@param[out] pstViewedIPP: �Ѿ��ۿ�����ipp��Ŀ��
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (* m_fnGCableCAGetViewdIPP)(int nIndex, G_CaIPPViewedInfo_S *pstViewedIPP);


	/**
	@brief  ��ȡ��ʱ�����������IPP��Ŀ
	@param[out] pstIPP: ��ȡIpp��Ϣ
	@return 0��ʾ�ɹ���������ʾʧ��
	*/
	int (* m_fnGCableCAGetInquireIPP)(G_CaIPPInfo_S *pstIPP);

	/**
	@brief  ��ȡָ�����͵ļ۸�
	@param[int] nType: ʹ�ô�ֵָ������ļ۸�����.0  ���ش�����¼��ļ۸�;1  ���ش�������¼��۸�;2  Ҫ�ش�����¼��۸�;3  Ҫ�ش�������¼��۸�
	@param[int] pstCaIPPInfo:ipp��Ϣ
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (* m_fnGCableCAIppGetPrice)(G_CaIPPInfo_S *pstIPP, int nType);

	/**
	@brief ������ǰ��IPP��Ŀ
	@param[int] pcPin: ���ܿ�Pin��
	@param[int] nPriceType: �����ļ۸�����
	@param[int] pstCaIPPInfo:ipp��Ϣ
	@return  �ɹ�����0���Ѿ�������������1��ʧ�ܷ��ظ�ֵ��������ֵ���壩�� 
	@note ���� 0��¼��ļ۸�;1����¼��۸�;2 ������ն˼��ɵ�CAϵͳ��֧�ֶ��ּ۸����ͣ��򴫸�ֵ
	*/
	int (* m_fnGCableCAIppBook)(G_CaIPPInfo_S *pstIPP, DWORD dwPin, int nPriceType);

	/**
	@brief  �ܾ���ʱ������IPP��Ŀ��������
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (* m_fnGCableCACancelInquireIPP)();	

	/**
	@brief  ��ĸ������, ��ǰ������ĸ��ʱ���ã���ȡĸ������Ҫι���ӿ�������
	@param[int]:  ��Ӫ��ID
	@param[out]:  ĸ������Ҫι���ӿ�������(����ʱbuffer��С���ٷ���256�ֽ�)
	@param[out]:  ĸ������Ҫι���ӿ������ݵ�ʵ�ʳ���
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (*m_fnReadFeedDataFromParent)(WORD wOperatorId, BYTE* pcData, BYTE *pbLen);

	/**
	@brief  ����ĸ�������Ϣд���ӿ�
	@param[int]:  Ҫд���ӿ�������buffer
	@param[int]:  Ҫд���ӿ�������ʵ�ʳ���
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (*m_fnWriteFeedDataToChild)(WORD wOperatorId, BYTE* pcData, BYTE bLen);

	/**
	@brief  �õ�ĸ������
	@param[out]:  Ҫд��ĸ������buffer��0 ��ʾ��ǰΪĸ������Ϊ0 ��ʾ��ǰ��Ϊ�ӿ�������ֵΪ�ӿ���Ӧ��ĸ������
	@param[out]:  Ҫд��ĸ�����ŵ�ʵ�ʳ���
	@return  0��ʾ�ɹ���������ʾʧ��
	*/
	int (*m_fnGetMotherInfo)(BYTE* pcMotherInfo, int *pnLen);

	/**
	@brief  �������뿨���״̬
	@return 
	@ 0 ��ʾ���ܿ���Ӧ��ǰ������
	@ 1 ��ʾ�����ܿ�����Ч��
	@ 2 ��ʾ���ܿ�û�ж�Ӧ�κλ�����
	@ 3 ��ʾ���ܿ��Ѷ�Ӧ��Ļ�����
	@ -1 ��ʾ�˽ӿڵ���ʧ��
	*/
	int (*m_fnIsPaired)();

	/**
	@brief  �õ�������bouquetId
	@return 
	@ 0 ��ʾbouquetId��ȡʧ��
	*/
	int (*m_fnGCableCAGetBouquetID)();
	
	/**
	@brief	�õ������е�Ĭ��bouquetId(��m_fnGCableCAGetBouquetID ������ȡ�쳣��ʱ�����)
	@return 
	*/

	int (*m_fnGCableCAGetDefaultBouquetID)();

	/**
	@brief	�����Ƿ�����bouquetID�����ı�־
	@return 
	*/

	int (*m_fnGCableCASetMonitorFlag)(BOOL bNeedMonitor);
	
}CSShellGCableCAInitParam_S;

/**
@brief ���� CA �ӿڲ���
@param psParam  CA  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellGCableCASetParam(CSShellGCableCAInitParam_S * psParam);

/**
@brief ��ȡ CA �ӿڲ���
@param ��
@�ɹ�����CSShellGCableInitParam_S,ʧ�ܷ���NULL
*/
const CSShellGCableCAInitParam_S* CSShellGCableCAgetParam();

#ifdef __cplusplus
}
#endif

#endif

