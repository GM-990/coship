#ifndef CreateContainerInterface_H
#define CreateContainerInterface_H

#include "CSUSPDMCommonType.h"
#include "cs_fsapi.h"

#ifdef __cplusplus
extern "C"
{
#endif

//����Cable��������
HCSHANDLE CSUSPDMCreateCableContainer( HCSHANDLE hMemoryPool, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );
//����������������
HCSHANDLE CSUSPDMCreateTerrestriaContainer( HCSHANDLE hMemoryPool, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );
//����Satellite��������
HCSHANDLE CSUSPDMCreateSatelliteContainer( HCSHANDLE hMemoryPool, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );

//��������
/*CSUSPDMCreateContainer
1���ӿ�������
	���������������Ǵ�����һ���������󣬶�û�ж�Flash�д洢�����ݵ�DM
2������˵����
������ӿ�CSUSPDMCreateCableContainer��CSUSPDMCreateTerrestriaContainer���������һ������dwContainerType���������������ȫ��ͬ
dwContainerType ����DMͷ�ļ�CSUSPDMParseData.h�ж�����������ֱ��ʾ�����������ͣ�
#define 		USPDM_C_CONTAINER		(0x01)   //���߻�
#define 		USPDM_T_CONTAINER		(0x02)   //�����
#define 		USPDM_S_CONTAINER		(0x04)   //���ǻ�*/

HCSHANDLE CSUSPDMCreateContainer( HCSHANDLE hMemoryPool, DWORD dwContainerType, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );
//�ͷ���������
BOOL CSUSPDMFreeDataContainer( HCSHANDLE hDataContainer );


//�趨�û��Զ����Ŀ���ݵĳ��ȣ�ֻ����Container����֮ǰ���ã�����ֻ�ܱ�����һ�Σ�
//nBufLen[in]: �û��Զ������ݵĳ��ȣ�����ܳ���64�ֽڣ�
//nCoverOffset[in]: ������ظ�ʱ���ǵ�ƫ�Ƶ�ַ��������û��Զ��������׵�ַ��
//nCoverLen[in]: ���ǵĳ���
//nStoreFlashLen[in]: Ҫ�洢Flash���û��Զ������ݶεĳ��ȣ����Զ����ֶεĵ�һ���ֽ�����.��������õ�һ����������ô����ֶ�Ĭ��Ϊ��һ�������Ĵ�С��
BOOL CSUSPDMSetUserDataLen( int nBufLen, int nCoverOffset, int nCoverLen );
BOOL CSUSPDMSetUserDataLenEx( int nBufLen, int nCoverOffset, int nCoverLen, int nStoreFlashLen );



/*
	�趨�Ƿ��Service��ChannelID��Ϊ��Ŀ�Ĺؼ��֡���Ĭ������£�����ֶ��ǽ�Ŀ�Ĺؼ��֡�
*/
//�趨��Ŀ���߼�Ƶ���Ų���Ϊ��Ŀ�Ĺؼ��֣���Container����֮ǰ���ã�
BOOL CSUSPDMSetServiceChannelNumNotKey( void );
//�趨��Ŀ���߼�Ƶ������Ϊ��Ŀ�Ĺؼ��֣���Container����֮ǰ����,���ΪDM��Ĭ��ֵ��
BOOL CSUSPDMSetServiceChannelNumIsKey();



/*
	���º�������������ʼ��֮ǰ����
	�趨TP�Ĺؼ��֡�
	��Ĭ�ϵ�����£�����Ƶ����Ƶ�ʣ�������ʽ������������Ϊ�ؼ��֣����������Ƶ����TSID��OriNetworkID��Ϊ�ؼ���
*/
//�Ƿ�����û����õ�Ƶ��ؼ���
BOOL CSUSPDMSetTPKeyIsUserConfig( BOOL bIsUserConfig );
BOOL CSUSPDMGetTPKeyIsUserConfig();
//�Ƿ���TSID��ΪƵ��ؼ��֣�
BOOL CSUSPDMSetTPKeyTSID( BOOL bTSIDIsKey );
BOOL CSUSPDMGetTPKeyTSID();
//�Ƿ���OriNetworkID��ΪƵ��ؼ��֣�
BOOL CSUSPDMSetTPKeyOriNetworkID( BOOL bOriNetworkIDIsKey );
BOOL CSUSPDMGetTPKeyOriNetworkID();
//�Ƿ���Ƶ����ΪƵ��ؼ��֣�
BOOL CSUSPDMSetTPKeyFre( BOOL bFreIsKey );
BOOL CSUSPDMGetTPKeyFre();
//�Ƿ���Ƶƫ��ΪƵ��ؼ��ֵĵ���
BOOL CSUSPDMSetTPKeyFreOffset( BOOL bFreOffsetIsKey, DWORD dwFreOffset );
BOOL CSUSPDMGetTPKeyFreOffset( BOOL * pbFreOffsetIsKey, DWORD * pdwFreOffset );

//��ӽӿڣ��Ƿ���SymbolRate��ΪƵ��ؼ��֣�
BOOL CSUSPDMSetTPKeySymbolRate( BOOL bSymbolRateIsKey );
BOOL CSUSPDMGetTPKeySymbolRate();

int USPDMLocalSetServiceChannelKeyFlag(int nChannelNumKeySelectFlag);
int USPDMLocalGetServiceChannelKeyFlag();

//�˺�������������DM�ӿ�֮ǰ���ò���Ч
//Ϊ�˽��ATIƽ̨Flash��ȡ�ٶ�������������ӿ�
//������ƽ̨�����ļ�ϵͳ��Flash����Ҫ���ô˽ӿ�
//������
//	0: ��ͨFlash��д��Ĭ��ֵ��
//	1: �ļ�ϵͳFlash�ļ��ٷ�ʽ��д������ATIƽ̨��
//���أ�
//	nFlashType��ֵ
int CSUSPDMSetContainerFlashType( int nFlashType );


//���¹����趨���Ԫ�������ĺ��������û�û�е�����Ӧ����ʱ��
//��ʾû�������������

//�趨����Ŀ����
BOOL CSUSPDMSetMaxServiceNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨������Ŀ����
int CSUSPDMGetMaxServiceNum( HCSHANDLE hDataContainer );

//�趨���TPS����
BOOL CSUSPDMSetMaxTransponderSNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨�����TPS����
int CSUSPDMGetMaxTransponderSNum( HCSHANDLE hDataContainer );

//�趨���TPT����
BOOL CSUSPDMSetMaxTransponderTNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨�����TPT����
int CSUSPDMGetMaxTransponderTNum( HCSHANDLE hDataContainer );

//�趨���TPC����
BOOL CSUSPDMSetMaxTransponderCNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨�����TPC����
int CSUSPDMGetMaxTransponderCNum( HCSHANDLE hDataContainer );

//�趨������Ǹ���
BOOL CSUSPDMSetMaxSatelliteNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨��������Ǹ���
int CSUSPDMGetMaxSatelliteNum( HCSHANDLE hDataContainer );

//�趨������߸���
BOOL CSUSPDMSetMaxAntennaNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨��������߸���
int CSUSPDMGetMaxAntennaNum( HCSHANDLE hDataContainer );

//�趨����ϵ����
BOOL CSUSPDMSetMaxRelationNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨������ϵ����
int CSUSPDMGetMaxRelationNum( HCSHANDLE hDataContainer );

//�趨����Iterator����
BOOL CSUSPDMSetMaxIteratorNum( int nMaxNum, HCSHANDLE hDataContainer );
//��ȡ�û��趨�����Iterator����
int CSUSPDMGetMaxIteratorNum( HCSHANDLE hDataContainer );

BOOL CSUSPDMSetAutoServiceChannelIDDispatchFlag( BOOL bAutoChannelIDFlag, WORD wMinID, WORD wMaxID );
BOOL CSUSPDMGetAutoServiceChannelIDDispatchFlag( BOOL * pbAutoChannelIDFlag, WORD * pwMinID, WORD * pwMaxID );


//�����û��趨���������ݵ����ֵ��������û���Ҫ����
//�����ݹ���ĺ��ʵ��ڴ�ʹ������FLSʹ����
BOOL CSUSPDMGetMemFlsComfortableSize( int * pnMemSize, int * pnFlsSize, HCSHANDLE hDataContainer );

//��ȡ�汾��Ϣ
int CSUSPDMGetVersionInfo( char * pcVersionInfo, int nSize );
//��ȡDM���ڴ����ԣ���С�ڴ������ڴ棩
BOOL CSUSPDMGetMemInfo( int * pnMin, int * pnMax );

/*	�趨���ڴ�ռ任�����ٶȱ�ʶ
	�������ΪTRUEʱ���趨��ʶΪ�棬��ʹ������̨�ٶȼӿ�
	���ñȲ����ö�ռ��Ƶ�������4�ֽڵ��ڴ棬���ٶȾ����������Ƶ�������1000���Ļ�Ҫ��500ms+
	�˽ӿ�Ҫ�ڵ��ô��������ӿں�����ŵ��ô˺���  */
BOOL CSUSPDMSetMemSwapTimeFlag( HCSHANDLE hDataContainerHandle, BOOL bFlag );


/*CSUSPDMSetIsAttendUniNum
1���ӿ�������
��������Audio_Type�ֶα�־��������עService������UniNum�ֶΡ����ýӿ�CSUSPDMCreateContainer��ʼ��Container����������
���û�е��ô˺�����DM�ڲ���עUniNum�ֶΣ���ԭʼAudio_Type�ֶγ���Ϊ2��bit�Ĵ���ʽ
��������˴˺������Ҵ���Ĳ���ΪFALSE����ô˵���ڴ˹����в���עService�����е�UniNum�ֶ����ԣ������ʱ���ڹ����е�����UniNum��صĽӿھͻ��ж������������FALSE���������½ӿڣ�
CSUSPDMGetServiceUniNum��CSUSPDMSetServiceUniNum��CSUSPDMCreateIterateUniNumService
2������˵����
hDataContainerHandle����������������������������������ǵ��ýӿ�CSUSPDMCreateContainer�������������
bFlag�����������ΪFALSE��˵������ע��Ŀ�е�UniNum�ֶΣ���Ҫ��Audio_Type�ֶ����ӵ�8��bit��ΪTRUE��˵����ע��Ŀ�е�UniNum�ֶΣ���Audio_Type�ֶλ��ǰ�ԭʼ2��bit����
*/
BOOL CSUSPDMSetIsAttendUniNum( HCSHANDLE hDataContainerHandle, BOOL bIsAttendUniNum );

BOOL CSUSPDMSetClFlag( HCSHANDLE hDataContainer, BOOL bFlag );

HCSHANDLE CSUSPDMCreateFileSysContainer(HCSHANDLE hMemoryPool, DWORD dwContainerType, const char *pcDir);
BOOL CSUSPDMGetContainerHandle(HCSHANDLE **ppContainerHandle, int *pnContainerNum);

/*CSUSPDMSetCSFSFuns
1���ӿ�������
���DM ���ļ�ϵͳдFLASH ��Ϊ�����ò�������Ŀ�������Ӳ����������Ӵ˽ӿ�
�����ļ��Ĵ򿪡��رա���д����λ�ӿ��Բ�������ʽ���ý�DM

�õ��ļ�ϵͳ���ݹ���(CSUSPDMCreateFileSysContainer)����Ŀ������ô˷��������ã������дFlash��ʧ��
�˽ӿ���Ҫ�����ݹ����ʼ��֮ǰ����

2������˵����
pvFSP[] : ����������������飬�����coiship�ļ�ϵͳ����ֵ����Ϊvoid *pvFSP[] = {CSFSClose,CSFSOpen,CSFSRead,CSFSLseek,CSFSWrite};
nFucCount: ��������������ǵ�һ����������ĳ��ȣ���03.00.02�汾�����ݹ����У���ֵΪ5
*/
BOOL CSUSPDMSetCSFSFuns(CSFSOperation_t* pFucPort);

//added for yangjunjie 08.12.19
BOOL CSUSPDMLoadServiceFromFlash(HCSHANDLE hDataContainerHandle, BOOL bLoad );

/*CSUSPDMTakeSpace
1���ӿ�������
������hmc2.0���������DM ���ļ�ϵͳдFLASH ��Ϊ���⿪���޽�Ŀ����ռ���ܱ�Ӧ��ռ�������
��Ӵ˽ӿڣ�ͨ����DM��ʼ��ʱ���ô˽ӿ���ռס�㹻�ռ乩�����Ŀʹ��

�˽ӿ���CSUSPDMCreateFileSysContainer ֮����ã�����֮ǰ�����ȵ���CSUSPDMSetMaxServiceNum ��CSUSPDMSetMaxTransponderCNum�Ƚӿ�
���ý�Ŀ����������Ϣ�����������ú�����ֵ������������Դ�˷ѣ���Ϊ�ڲ�ʵ����ͨ������������Ԥ���ռ��

2������˵����
hDataContainerHandle : ������������������ͨ��CSUSPDMCreateFileSysContainer����
*/
BOOL CSUSPDMTakeSpace( HCSHANDLE hDataContainerHandle );
#ifdef __cplusplus
}
#endif

#endif
