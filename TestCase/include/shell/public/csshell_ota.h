#ifndef CSSHELL_OTA_H
#define CSSHELL_OTA_H
#include "mmcp_typedef.h"

#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define OTA_DELIVER_COUNT				(0X8)

/**
@brief ota����ö��
@note ����ֻ�Ƕ��˼��ֳ�����ota��׼���ͣ�����������������Ŀ������Ŀ���Զ����Լ���ota���͡�
��Ŀ����ota����ʱһ��Ҫע��:
1.����ota���͵Ľ��ͣ�һ��Ҫ��java�㱣��һ��
2.�Զ���ota����ֵһ��Ҫ����EM_CSSHELL_OTA_TYPE_USERSTART
@note �����ota������java��OTAType������java��OTAType��������׼ȷ��������ʷԭ����
*/
typedef enum
{
    EM_CSSHELL_OTA_TYPE_NDS = 1,
    EM_CSSHELL_OTA_TYPE_IRDETO,
    EM_CSSHELL_OTA_TYPE_TF,
    EM_CSSHELL_OTA_TYPE_COSHIP,
    EM_CSSHELL_OTA_TYPE_OTHER,
    EM_CSSHELL_OTA_TYPE_COMVENIENT_CA,
    EM_CSSHELL_OTA_TYPE_FTASAT,
    EM_CSSHELL_OTA_TYPE_FTATER,

    EM_CSSHELL_OTA_TYPE_USERSTART = 0x100
}CSShellOtaType_E;

/**
@brief ota��Ϣ���嶨��
@note �ײ�ota��java��ota����ͨ�ţ�������Щota��Ϣ��
@note ���ײ�C�����⵽ota������Ϣʱ��ͨ������MMCP_EvtBroadcastEvent���㲥����Ϊ����ö��ֵ��MMCP_EvtEventEntry��Ϣ������Щ��Ϣ���ݸ�java�㡣
@code ��⵽��ǿ��������ota��Ϣʱ����Ҫ�㲥��Ϣ֪ͨjavaӦ��:

	MMCP_EvtEventEntry sEventEntry = {0};

	sEventEntry.type = EVT_OTA_EVENT;
	sEventEntry.code = EM_CSSHELL_OTA_MESSAGE_FORCE;
	sEventEntry.data = (void  *)0;	//�ò����Լ����壬java���ȡ��֮���գ����а���Ŀ����
	nRet = MMCP_EvtBroadcastEvent(&sEventEntry, NULL);

*/
typedef enum
{
    EM_CSSHELL_OTA_MESSAGE_FORCE = 0,
    EM_CSSHELL_OTA_MESSAGE_NORMAL ,
    EM_CSSHELL_OTA_MESSAGE_REFRESHLOADER,
    EM_CSSHELL_OTA_MESSAGE_CANCEL,
    EM_CSSHELL_OTA_MESSAGE_MANUAL
} CSShellOtaMessageType_E;

/**
@brief Comvenient CA OTA �����������
@aDelivers ����Ƶ���б�
@nDeliverCount ����Ƶ��ĸ���
@bDownUpgrade �Ƿ�֧�ֵͰ汾����
*/
typedef struct _CSSHELLOTA_ComvenientCAStartParam_S
{
    Deliver aDelivers[OTA_DELIVER_COUNT];
    int nDeliverCount;
    BOOL bDownUpgrade;
} CSSHELLOTA_ComvenientCAStartParam_S;

/**
@brief FTA������OTA �����������
@nSatIndex ��������
@nTPIndex  TP����
@nDownLoadPID ���������DownLoadPID
@nTableID ���������TableID
*/
typedef struct _CSSHELLOTA_FtaSatStartParam_S
{
    int nSatIndex;
    int nTPIndex;
    int nDownLoadPID;
    int nTableID;
} CSSHELLOTA_FtaSatStartParam_S;

/**
@brief FTA�ĵ���OTA �����������
@aDelivers ����Ƶ���б�
@nDeliverCount  ����Ƶ��ĸ���
@nDownLoadPID ���������DownLoadPID
@nTableID ���������TableID
*/
typedef struct _CSSHELLOTA_FtaTerStartParam_S
{
    Deliver aDelivers[OTA_DELIVER_COUNT];
    int nDeliverCount;
    int nDownLoadPID;
    int nTableID;
} CSSHELLOTA_FtaTerStartParam_S;
/*
*@brief ota��ʼ������,������һϵ�к���ָ�룬��Щ����ָ����Ӧ�õ�otaʵ��(C����)��Ȼ��ע���shell��ÿ������ָ���ӦOTAInvoke��һ���ӿ�
@see OTAInvoke*/
typedef struct _CSShellOtaInitParam
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellOtaInitParam_S)
	CSShellOtaType_E m_eType;	//ota������
	/**
	@brief ��ʼ����ota��������,��OTAInvoke.start��Ӧ.
	@note ���øýӿں󣬽��ɼ�������е�������Ϣ����⵽������Ϣ֮��Ҫ����Ϣ֪ͨ��Ӧ�á����´�����ʱ��loaderʵʩӦ��д��flash��������
	@note loader��������Ӧ�ò�д��flashʵ�������Ĺ��̲��Ǹýӿ������ĵġ�
	*/
	void (* m_fnStart)();

	/**
	@brief ��ͣota����,��OTAInvoke.userwait��Ӧ
	*/
	void (* m_fnUserWait)();

	/**
	@brief ���ô���״̬,��OTAInvoke.setStandByStatus��Ӧ
	@param bStandby ������1  ��������0
	@return �ɹ���0 ʧ�ܣ�-1
	*/
	int (* m_fnSetStandbyStatus)(BOOL bStandby);

	/**
	@brief ��ȡloader״̬,��OTAInvoke.getLoaderStatus��Ӧ
	@return loader��״̬������������ota������loader״̬����������Ŀ���ֱ�ӷ���-1��Ҳ����ע��ʱ����m_fnGetLoaderStatusΪNULL��
	������ĵĻ���״ֵ̬����������ɾ���ota������
	@note ע��״ֵ̬�ĺ��壬��һ��������������ʲô״̬��Ҫ��OTAInvoke.getLoaderStatus����ֵ����һ��
	*/
	int (* m_fnGetLoaderStatus)();

	/**
	@brief �����ֶ�OTA����,��OTAInvoke.enterManualOTA��Ӧ
	*/
	void (* m_fnEnterManualOta)(int nFreq,int nSymbol,int nModulation);

	/**
	@brief ��ȡOTA����,��OTAInvoke.getOTAName��Ӧ
	*/
	const char* (* m_fnGetOtaName)();

	/**
	@brief�ж��Ƿ���������Ϣ,��OTAInvoke.hasUpdateInfo��Ӧ
	@return ��������TRUE   ��������FALSE
	*/
	BOOL (*m_fnHasUpdateInfo)();

	/**
	@brief��ȡ����״̬
	@return ����״̬��TRUE �Ǵ���״̬��FALSE
	@note nds�����ڼٴ���״̬Ҳ��ota��ota��Ϻ�����������֮ǰ�Ǵ���״̬������֮��ҲҪ����תΪ����״̬��Ӧ������Ҫ��ȡ����״̬���ܽ��д˲�����
	@note ֻ�Լٴ�����Ч�����˲�֧��������ĺ����⣬nds��Ҫ��Ӧ���ϼ�⵽������־����ʾ������û�����ȡ������֮���û��ٰ��´�������ҲҪ�ٴ�����
	*/
	BOOL (* m_fnGetStandbyStatus)();

	 /**
	 @brief����USB����
	 @return �����ɹ���TRUE ����ʧ�ܷ��أ�FALSE
	 @pcPathName:��ʾ���������ļ����ڵ�·��������
	 */
	BOOL (* m_fnStartUSBUpgrade)(const char *pcPathName);

	 /**
	 @brief����USB����
	 @return �����ɹ����أ�TRUE ����ʧ�ܷ��أ�FALSE
	 */
	BOOL (*m_fnStartUSBDownLoad)();
	/**
	@brief ������ǿ�ƽ���Loader����OTA����
	*/
	
	void (* m_fnEnterForceOta)(int nFreq,int nSymbol,int nModulation, int nPid, int nTableId);
	/**
	@brief ������������
	@param pParam
					OTATypeΪEM_CSSHELL_OTA_TYPE_COMVENIENT_CA ʱ��pParamΪCSSHELLOTA_ComvenientCAStartParam_S*
					OTATypeΪEM_CSSHELL_OTA_TYPE_FTASAT ʱ��pParamΪCSSHELLOTA_FtaSatStartParam_S*
					OTATypeΪEM_CSSHELL_OTA_TYPE_FTATER ʱ��pParamΪCSSHELLOTA_FtaTerStartParam_S*
	*/
	BOOL (* m_fnSetStartParam)(void* pParam);

 	/**
     @brief����OTA����
     @return �����ɹ����أ�TRUE ����ʧ�ܷ��أ�FALSE
     @note �м���޸�flash�е�Booter���ݣ����÷�������Ĳ�����Ϣд��flash��loaderģ��ʹ�á�
     */
	BOOL (*m_fnStartGDOTA)(int nFrequency,int nSymbolRate, int nModulation,DWORD nServiceID, int nManu_code, int nMajor_hw_version,int nMinor_hw_version,int nHw_batch_no);

	/**
     @brief�м��֪ͨ�������޸�flash�е�Booter���ݣ�д����ɺ��м���������������С����������´�����ʱ��ֱ�ӽ���loaderģ�����OTA������
     @return �����ɹ����أ�TRUE ����ʧ�ܷ��أ�FALSE
     @note 
     */
	BOOL (*m_fnStartLater)(void);
	
	/**
     @brief������������
     @param pcURL: ���������URL��ַ nURLLen: URL�ַ�������
     @return �����ɹ����أ�TRUE ����ʧ�ܷ��أ�FALSE
     @note 
     */
	BOOL (*m_fnStartInternetUpgrade)(char *pcURL, int nURLLen);

}CSShellOtaInitParam_S;


/**
@brief ����ota�����ӿ�
@param psParam ota����shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellOtaSetParam(CSShellOtaInitParam_S * psParam);


#ifdef __cplusplus
}
#endif

#endif

