#ifndef _CS_EASYBUS_H_
#define _CS_EASYBUS_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*
*  错误值
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
  char acIp[EASYBUS_ADDR_MAX_LEN]; /*ip地址，格式: 192.168.1.2*/
  unsigned short ucPort; /*端口号*/
}CSEasybusAddr_S;

typedef struct {
  CSEasybusAddr_S sRemoteAddr; /*远端（如手机、PAD）应用的地址*/
  char acMode; /*模式：单播，广播，通知*/
  char acMsgType[EASYBUS_MSGTYPE_MAX_LEN + 1]; /*消息类型*/
  char acMsgData[EASYBUS_MSGDATA_MAX_LEN]; /*消息数据*/
  int nMsgDataSize; /*消息数据大小*/
}CSEasybusMsg_S;


/***************************************************************************
*  函      数:  int easybus_attach(void)
*  功      能:  attach到easybus总线上
*  参      数:  无
*  返  回  值:  <0 失败，0 成功
*  备      注:  
****************************************************************************/
int CSEasybusAttach(void);

/***************************************************************************
*  函      数:  int easybus_detach(void)
*  功      能:  从easybus总线上detach  
*  参      数:  无
*  返  回  值:  <0 失败，0 成功
*  备      注:  
****************************************************************************/
int CSEasybusDetach(void);

/***************************************************************************
*  函      数:  int easybus_interest(char *msgType)
*  功      能:  注册感兴趣的消息类型
*  参      数:  msgType 消息类型，ASCII编码，最大不能超过16字节
*  返  回  值:  <0 失败，0成功
*  备      注:  该接口为阻塞方式
****************************************************************************/
int CSEasybusInterest(char *pcMsgType);

/***************************************************************************
*  函      数:  int easybus_receive(EasybusMsg *revMsg)
*  功      能:  从easybus接收数据
*  参      数:  revMsg 接收的消息
*  返  回  值:  <0 失败，0 成功
*  备      注:  该接口为阻塞方式
****************************************************************************/
int CSEasybusReceive(CSEasybusMsg_S *psRevMsg);

/***************************************************************************
*  函      数:  int easybus_send(EasybusMsg *sendMsg)
*  功      能:  通过easybus发送数据
*  参      数:  sendMsg 发送的消息
*  返  回  值:  <0 失败，0 成功
*  备      注:   该接口为阻塞方式
****************************************************************************/
int CSEasybusSend(CSEasybusMsg_S *psSendMsg);

#ifdef  __cplusplus
}
#endif

#endif/*_CS_EASYBUS_H_*/
