/*@defgroup TR069�������͵Ķ��壬�Լ��ӿڵ�˵��
@brief tr069��Ӧ��ͷ�ļ�cs_tr069.h��tr069ģ����һ���������ϵͳ���նˣ�������ʱ��
��������ϵͳע�ᣬע��֮������ϵͳ�Ϳ���Զ�̿�������ն��ˣ�����ϵͳ��Ҫ�Ĳ�����
���ò����ͻ�ȡ�������֣���Щ���úͻ�ȡ����ͨ��tr069Э�����Զ�̿��ƣ�tr069ģ����Ҫ
ʵ����tr069Э���ͨ�ţ���������ϵͳ�����������ݰ�������Ӧ�Ĳ���֮���ٽ���Ϣ��������
��ϵͳ��
tr069ģ���ʹ��:
�û�ͨ������CSTR069Init������Ȼ���ٵ���CSTR069Start�Ϳ�������tr069ģ�飬����֮�󣬻�
��ǰ������ϵͳע�ᣬ֮��˫���Ϳ���ͨ���ˡ�

@version 2.0.1 2010/08/01
@version 2.0.2 2011/01/10
@{
*/
#ifndef _CS_TR069_H
#define _CS_TR069_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "udiplus_os.h"
#include "udiplus_typedef.h"
/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/

#define CSTR069_NAME_LEN 256       ///< tr069�ж���������ֶ����Ƶ���󳤶�
#define CSTR069_VLAUE_LEN    2048  ///< tr069�ж���������ֶ�ֵ����󳤶�


/**@brief tr069�¼�����*/
typedef enum {
	EM_TR069_MSG_BOOTSTRAP=0,      ///<����tr069ģ��
	EM_TR069_MSG_GETPARAM_VALUE, ///<��ȡ����
	EM_TR069_MSG_SETPARAM_VALUE, ///<���ò���
	EM_TR069_MSG_REBOOT,         ///<�����豸
	EM_TR069_MSG_FACTORYRESET,   ///<�豸��λ
	EM_TR069_MSG_DOWNLOAD,       ///<�����ļ�
	EM_TR069_MSG_UPLOAD,         ///<�ϴ��ļ�
	EM_TR069_MSG_GETPARAM_ATTRI, ///<��ȡ��������
	EM_TR069_MSG_SETPARAM_ATTRI, ///<���ò�������
	EM_TR069_MSG_GETPRCMETHOD,   ///<tr069 ��ȡ����RPC����
	EM_TR069_MSG_GETPARAM_NAME=10,  ///<��ȡ��������
	EM_TR069_MSG_COMMON_FAULT,   ///<ͨ�ų���
	EM_TR069_MSG_SETPARAM_FAULT, ///<���ò�������
	EM_TR069_MSG_END_SESSION,    ///<�����Ự
	EM_TR069_MSG_PING,           ///<ping����
	EM_TR069_MSG_TRACERT,        ///<trace·�ɲ���
	EM_TR069_MSG_TRAPE,          ///<trape����
	EM_TR069_MSG_PERIODIC,       ///<��ʱ�ϱ�
	EM_TR069_MSG_OPENEPG,        ///<��EPG
	EM_TR069_MSG_RESTARTSERVER ,  ///<����http������
	EM_TR069_MSG_SETPARAM_VALUEA=20,
	EM_TR069_MSG_GETCONFIGFILE,
	EM_TR069_MSG_GETERRORLOG,
	EM_TR069_MSG_ADDOBJECT,
	EM_TR069_MSG_DELOBJECT,
	EM_TR069_MSG_STARTAPP,
	EM_TR069_MSG_EMERGENCYRADIO,
	EM_TR069_MSG_UNKNOW
}CSTR069_EVENT_CODE_E;


/**@brief data���� */
typedef enum {
	EM_TR069_STRING = 0, ///string
	EM_TR069_INT = 1,    ///int
	EM_TR069_UNSIGNEDINT = 2,     ///unsignedInt
	EM_TR069_BOOL = 3,    ///<boolean
	EM_TR069_DATETIME = 4, ///dateTime
	EM_TR069_OBJECT = 5,  ///object
	EM_TR069_OTHER
}CSTR069_DATATYPE_E;


/**@brief tr069����ֵ����*/
typedef enum {
	EM_TR069_SUCCESS = 0,   ///<�ɹ�
	EM_TR069_FAILURE,       ///<ʧ��
	EM_TR069_INVALPARA      ///<��������
}CSTR069_RET_E;

/**@brief inform���� */
typedef enum {
	EM_TR069_INFONOMAL = 0, ///<����inform��Ϣ
	EM_TR069_INFOPERIOD,    ///<�������ϱ�������Ϣ
	EM_TR069_INFOTRAPE,     ///<trape ������Ϣ
	EM_TR069_INFODEVICEID   ///<����deviceid������Ϣ
}CSTR069_INFOTYPE_E;

/**@brief tr069ģ��״̬ */
typedef enum {
	EM_TR069_STATE_INIT ,    ///<��ʼ״̬,���ûص�����CSOnTR069StateChange_Fʱ��dwPara1��dwPara2Ϊ��
	EM_TR069_STATE_READY,    ///<����״̬������֮�������ӽڵ���Ϣ,���ûص�����CSOnTR069StateChange_Fʱ��dwPara1��dwPara2Ϊ��
	EM_TR069_STATE_END      ///<����״̬,���ûص�����CSOnTR069StateChange_Fʱ��dwPara1��dwPara2Ϊ��
}CSTR069State_E;

/**@brief tr069�¼��ṹ�壬����CSTR069PostMsg�ᴫ���ֵ */
typedef struct{
 	int nEventCode;         ///<�¼������룬ȡֵΪCSTR069_EVENT_CODE_E�ж��������
  	int nEventData;         ///<�¼�ֵ
	HCSHANDLE hTr069handle; ///<tr069���
}CSTR069_EVENT_S;


/*
@brief ����ָ�룬������ȡ�û���Ϣ

@param[out] pcBuffer:�������������û���Ϣ
@param[in] nLen:�û���Ϣ����
@param[in] pvUserdata:�û����������û��Զ���ʹ�ã��û��ڳ�ʼ��tr069������ӽڵ���Ϣʱ����CSTR069_Item_S�д���pvUserdata��
tr069����CSOnTR069Read_F ��ʱ�򽫸ò����ص����û�
*/
typedef int (*CSOnTR069Read_F)(char *pcBuffer,int nLen,void* pvUserdata);  


/*
@brief ����ָ�룬���������û���Ϣ

@param[in] pcBuffer:���Ҫ���õ��û���Ϣ
@param[in] nLen:�û���Ϣ����
@param[in] pvUserdata:�û����������û��Զ���ʹ�ã��û��ڳ�ʼ��tr069������ӽڵ��ʱ����CSTR069_Item_S�д���pvUserdata��
tr069����CSOnTR069Write_F ��ʱ�򽫸ò����ص����û�
*/
typedef int (*CSOnTR069Write_F)(char *pcBuffer,int nLen,void* pvUserdata);


/**@brief TR069�ڵ���Ϣ����

@note ����CSTR069Init������ʱ��Ҫ����CSTR069_Item_S���͵����飬�ýṹ���ж����˽ڵ�Ĳ���
����:nType,���÷�Ϊ����:
1����ǰ������ϵͳ��ȡ����ʱ:
nType =0����ʾ����CSOnTR069GetPara_F����(���û�ʵ��)����ȡֵ
nType =1 ,��ʾ ��CSTR069_Item_S���͵Ľڵ�acData��Ϊ""����Ĭ��ֵ��Ϊ���ַ���ʱ��ȡĬ��ֵacData��
���acDataΪ�գ���pfRead��Ϊ�գ������pfRead��ȡֵ
nType =2����ʾʹ��tr069�ڲ�ʵ������ȡ���������û�ע��ĺ���ָ��
2����ǰ������ϵͳ���ò���ʱ:
nType =0����ʾ����CSOnTR069SetPara_F����(���û�ʵ��)������
nType =1 ,��ʾ ��CSTR069_Item_S���͵Ľڵ�pfWrite��Ϊ�գ�����pfWrite������
nType =2����ʾʹ��tr069�ڲ�ʵ�������ã��������û�ע��ĺ���ָ��
*/
typedef struct
{
	char acFullName[CSTR069_NAME_LEN];  ///< �����ڵ�ȫ·��������"Device.LAN.1.IPAddress"
	int  nType;                     ///< �����ڵ��������,��ϸ˵��������ṹ��˵��
	int nWritable;	
	char acData[CSTR069_VLAUE_LEN]; ///< �ڵ�Ĭ��ֵ	
	CSOnTR069Read_F pfRead;            ///< ��ȡ�û�������Ϣ	
	CSOnTR069Write_F pfWrite;          ///< �����û�������Ϣ
    void* pvUserdata;               ///< �����û��Զ�����Ϣ������:�����ڵ���Ϣ��ȫ��ͬ��Ҷ�ӽڵ㣬
                                    ///< ��pvUserdata�������������Ҷ�ӽڵ�Ĳ�����,�����һ��������һ��Ҷ�ӵ���
                                    ///< ����:{"IPAddress",0,1,"",CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)1},
                                    ///< �ڶ���������������:{"IPAddress",0,1,"",CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)2}, 
                                    ///< �û����ú�֮��tr069ģ�����pfRead��pfWrite��ʱ��Ὣ�����õ�pvUserdata�ص���������֪�û���
                                    ///<ȡ�Ŀ���������Ϣ
 	CSTR069_DATATYPE_E eDataType;   ///<�ڵ���������ͣ�����ȡֵ��CSTR069_DATATYPE_E�Ķ���
}CSTR069_Item_S;


/*
@brief TR069��ǰ״ָ̬�룬��tr069ģ��״̬�仯��ʱ��ᴥ���ûص�������CSTR069AddCallback��ʱ����Ҫ��������͵ĺ���ָ��

@param[in] dwUserData:���������û�����
@param[in] eMsg:����tr069��ǰ״ֵ̬����Ϊ����״̬��ʱ���ʾtr069�Ѿ�������������ӽڵ���Ϣ
@param[in] dwPara1: ����1
@param[in] dwPara2: ����2
*/
typedef void (*CSOnTR069StateChange_F)(DWORD dwUserData, CSTR069State_E eMsg, DWORD dwPara1, DWORD dwPara2);


/*
@brief �����û���������ָ�룬����CSTR069Init��ʱ����Ҫ��������͵ĺ���ָ��

@param[in] pcParaName:�����õĲ��������ƣ���ȫ·�����ƣ���:Device.DeviceInfo.Manufacturer
@param[in] pcParaValue:�����õĲ�����ֵ
@param[in] nParaLen: �����õĲ����ĳ���
@param[in] unUserData:���������û�����
*/
typedef int (*CSOnTR069SetPara_F)(char* pcParaName,char *pcParaValue,int nParaLen,unsigned int unUserData);


/*
@brief ��ȡ�û���������ָ�룬����CSTR069Init��ʱ����Ҫ��������͵ĺ���ָ��

@param[in] pcParaName:��Ҫ��ȡ�ĵĲ��������ƣ���ȫ·�����ƣ���:Device.DeviceInfo.Manufacturer
@param[out] pcParaValue:�������������������Ҫ��ȡ�Ĳ�����ֵ
@param[in] nParaLen: ��ȡ�Ĳ����ĳ���
@param[in] unUserData:���������û�����
*/
typedef int (*CSOnTR069GetPara_F)(char* pcParaName,char *pcParaValue,int *pnParaLen,unsigned int unUserData);


/*
@brief  tr069 ģ���ʼ��

@param[in] 	psItem:	 CSTR069_Item_S���͵Ľṹ������.
@param[in] 	nItemNum: psItem�ĸ���.
@param[in]  fnSetPara: ���ò����ĺ���ָ��.
@param[in]  fnGetPara: ��ȡ�����ĺ���ָ��.
@param[in]  unUserData:  ���������û�����
@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069Init(CSTR069_Item_S *psItem,int nItemNum,CSOnTR069SetPara_F fnSetPara,CSOnTR069GetPara_F fnGetPara,unsigned int unUserData);


/*
@brief  tr069 ����inform��Ϣ

@note ����֮����tr069INFORM���н�Я����Щ��Ϣ���ͳ�ȥ��ǰ��
@param[in]	Inform: Ҫ���õ�INFORM��Ϣ������.
@param[in]	nInformNum: ���õ�INFORM��Ϣ�ĸ���.
@param[in]	eInfoType:	INFORM��Ϣ���ͣ�ȡֵΪCSTR069_INFOTYPE_E�ж��������
@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069SetInform(char *ppInform[],int nInformNum,CSTR069_INFOTYPE_E eInfoType);


/*
@brief  ����tr069

@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069Start(void);


/*
@brief  ������Ϣ��tr069ģ��

@param[in] 	pEvent: ��Ҫ���͸�tr069ģ�����Ϣ����Ϣȡֵ��CSTR069_EVENT_S�еĶ���.
@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069PostMsg(CSTR069_EVENT_S *psEvent);


/*
@brief ��ȡ�״���������ϵͳ�ɹ���ʱ��

@return  �����״���������ϵͳ�ɹ���ʱ��
*/ 
CSUDIPLUS_TIME_T CSTR069GetFirstLinkTimes(void);

/*
@brief  ����ǰ������ϵͳ��URL���û���������

@note �ò���Ӧ���ڵ���CSTR069Start֮ǰ����������Ч��CSTR069Start֮����INFORM
����ʱ����õ���URL���û���������
@param[in] 	pcUrl: ǰ������ϵͳ��URL
@param[in] 	pcUserName: ǰ������ϵͳ���û���.
@param[in] 	pcPassWord:  ǰ������ϵͳ������.
@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069SetServerInform(char *pcUrl,char *pcUserName,char *pcPassWord);


/*
@brief  ���tr069��һ��ڵ�

@note �ýӿڵĵ���Ӧ����tr069״̬ΪEM_TR069_STATE_READY
@param[in] 	psItem:      ��Ҫ��ӵ�һ��ڵ����Ϣ.
�������һ��ڵ���Ϣ:
    CSTR069_Item_S tr069LanArr_s[] = {
    {"Device.LAN.1.AddressingType", 0,"GetAddrType",CSUDI_NULL,CSUDI_NULL,(void*)1},
    {"Device.LAN.1.IPAddress", 1, "", CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)1},
    {"Device.LAN.1.SubnetMask", 1, "", CSNetWork_GetUsrMask, CSNetWork_SetUsrMask,(void*)1},
    {"Device.LAN.1.DefaultGateway",1, "", CSNetWork_GetUsrGateway,CSNetWork_SetUsrGateway,(void*)1},
    {"Device.LAN.1.DNSServers", 1, "",CSNetWork_GetUsrDNSServer,CSNetWork_SetUsrDNSServer,(void*)1},
    {"Device.LAN.1.MACAddress",1, "",CSNetWork_GetUsrMac,CSUDI_NULL,(void*)1}
};
@param[in] 	nItemNum:      �ܹ�Ҫ��ӵĽڵ�������ΪpsItem����Ĵ�С.
@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069AddItem(CSTR069_Item_S *psItem,int nItemNum);

/*
@brief  ɾ��tr069��һ��ڵ�

@note �ýӿڵĵ���Ӧ����tr069״̬ΪEM_TR069_STATE_READY
@param[in] 	psItem:      ��Ҫɾ����һ��ڵ����Ϣ.
�������һ��ڵ���Ϣ:
    CSTR069_Item_S tr069LanArr_s[] = {
    {"Device.LAN.1.AddressingType", 0,"GetAddrType",CSUDI_NULL,CSUDI_NULL,(void*)1},
    {"Device.LAN.1.IPAddress", 1, "", CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)1},
    {"Device.LAN.1.SubnetMask", 1, "", CSNetWork_GetUsrMask, CSNetWork_SetUsrMask,(void*)1},
    {"Device.LAN.1.DefaultGateway",1, "", CSNetWork_GetUsrGateway,CSNetWork_SetUsrGateway,(void*)1},
    {"Device.LAN.1.DNSServers", 1, "",CSNetWork_GetUsrDNSServer,CSNetWork_SetUsrDNSServer,(void*)1},
    {"Device.LAN.1.MACAddress",1, "",CSNetWork_GetUsrMac,CSUDI_NULL,(void*)1}
};
@param[in] 	nItemNum:      �ܹ�Ҫɾ���Ľڵ�������ΪpsItem����Ĵ�С.
@return  �ɹ����� EM_TR069_SUCCESS  ʧ�ܷ��� EM_TR069_FAILURE ������
*/ 
CSTR069_RET_E CSTR069DelItem(CSTR069_Item_S *psItem,int nItemNum);
/*
@brief  ��TR069ע��һ��callback

@param[in] dwUserData: �����û�������Ϣ
@param[in] Callback:  Ҫ��ӵĻص�	
*/ 
BOOL CSTR069AddCallback(DWORD dwUserData, CSOnTR069StateChange_F fnOnTR069StateChange);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

