#ifndef _CS_EASYBUS_H_
#define _CS_EASYBUS_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*
*  ����ֵ
*/
enum {
    EM_EASYBUSERR_NOERROR                  =   0,
    EM_EASYBUSERR_UNKNOWN                  =  -1,
    EM_EASYBUSERR_NOMEMORY                 =  -2,
    EM_EASYBUSERR_BADPARAM                 =  -3,
    EM_EASYBUSERR_INVALID                  =  -4,
    EM_EASYBUSERR_OPENCTRLCONNFAIL         =  -5,
    EM_EASYBUSERR_OPENMONITORCONNFAIL      =  -6,
    EM_EASYBUSERR_ATTACHFAIL               =  -7,
    EM_EASYBUSERR_SOCKETPAIRFAIL           =  -8,
    EM_EASYBUSERR_DETACHFAIL               =  -9,
    EM_EASYBUSERR_INTERESTFAIL             =  -10,
    EM_EASYBUSERR_CTRLCONNNOEXIST          =  -11,
    EM_EASYBUSERR_MONITORCONNNOEXIST       =  -12,
    EM_EASYBUSERR_RECEIVEFAIL              =  -13,
    EM_EASYBUSERR_RECEIVEEOF               =  -14,
    EM_EASYBUSERR_SENDFAIL                 =  -15,
}CSEasybusErr_E;

#define EASYBUS_ADDR_MAX_LEN (24)
#define EASYBUS_MSGTYPE_MAX_LEN (16)
#define EASYBUS_MSGDATA_MAX_LEN (1024)

typedef struct {
  char acIp[EASYBUS_ADDR_MAX_LEN]; /*ip��ַ����ʽ: 192.168.1.2*/
  unsigned short ucPort; /*�˿ں�*/
}CSEasybusAddr_S;

typedef struct {
  CSEasybusAddr_S sRemoteAddr; /*Զ�ˣ����ֻ���PAD��Ӧ�õĵ�ַ*/
  char acMode; /*ģʽ���������㲥��֪ͨ*/
  char acMsgType[EASYBUS_MSGTYPE_MAX_LEN + 1]; /*��Ϣ����*/
  char acMsgData[EASYBUS_MSGDATA_MAX_LEN]; /*��Ϣ����*/
  int nMsgDataSize; /*��Ϣ���ݴ�С*/
}CSEasybusMsg_S;


/***************************************************************************
*  ��      ��:  int easybus_attach(void)
*  ��      ��:  attach��easybus������
*  ��      ��:  ��
*  ��  ��  ֵ:  <0 ʧ�ܣ�0 �ɹ�
*  ��      ע:  
****************************************************************************/
int CSEasybusAttach(void);

/***************************************************************************
*  ��      ��:  int easybus_detach(void)
*  ��      ��:  ��easybus������detach  
*  ��      ��:  ��
*  ��  ��  ֵ:  <0 ʧ�ܣ�0 �ɹ�
*  ��      ע:  
****************************************************************************/
int CSEasybusDetach(void);

/***************************************************************************
*  ��      ��:  int easybus_interest(char *msgType)
*  ��      ��:  ע�����Ȥ����Ϣ����
*  ��      ��:  msgType ��Ϣ���ͣ�ASCII���룬����ܳ���16�ֽ�
*  ��  ��  ֵ:  <0 ʧ�ܣ�0�ɹ�
*  ��      ע:  �ýӿ�Ϊ������ʽ
****************************************************************************/
int CSEasybusInterest(char *pcMsgType);

/***************************************************************************
*  ��      ��:  int easybus_receive(EasybusMsg *revMsg)
*  ��      ��:  ��easybus��������
*  ��      ��:  revMsg ���յ���Ϣ
*  ��  ��  ֵ:  <0 ʧ�ܣ�0 �ɹ�
*  ��      ע:  �ýӿ�Ϊ������ʽ
****************************************************************************/
int CSEasybusReceive(CSEasybusMsg_S *psRevMsg);

/***************************************************************************
*  ��      ��:  int easybus_send(EasybusMsg *sendMsg)
*  ��      ��:  ͨ��easybus��������
*  ��      ��:  sendMsg ���͵���Ϣ
*  ��  ��  ֵ:  <0 ʧ�ܣ�0 �ɹ�
*  ��      ע:   �ýӿ�Ϊ������ʽ
****************************************************************************/
int CSEasybusSend(CSEasybusMsg_S *psSendMsg);

#ifdef  __cplusplus
}
#endif

#endif/*_CS_EASYBUS_H_*/
