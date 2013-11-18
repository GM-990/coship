/***************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_TRCA_Interface.h  
* Author:      qiaozhenghui
* Date:        Jan 2013
* Description: TR CA Interface 
*            
****************************************************************************************************/
#ifndef __CS_TRCA_INTERFACE_H__
#define __CS_TRCA_INTERFACE_H__

#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************** 
  结构体作用:  定义了腾锐CA抛给应用的所有的消息类型
  
  特殊约定:    CA_ALARM:传递CA lib上抛的显示消息,具体请参考TRCaAlarm_E
               CA_STATUS:上抛卡的状态,具体显示和动作由应用定义
               CA_COMMAND:上抛业务消息,最后一个参数附带业务信息的地址
********************************************************************/
typedef enum _TRCaEventGroup
{
	CA_ALARM = 0,                    //ALARM消息 
	CA_STATUS,                       //STATUS消息 
	CA_COMMAND,                      //COMMAND消息   
	CA_INFO,                         //reserve for future
	CA_MMI,                          //reserve for future
    CA_MAX                           //无效字段  
}TRCaEventGroup_E;

/********************************************************************  
  结构体作用:  定义了腾锐CA抛给应用的所有的业务消息
  
  特殊约定:    ANNOUNCEMENT为紧急事件,用户收到后应当立即显示,可以退出和删除,
           进入公告箱可以查看和删除
               NOTIFICATION为缴费通知,用户收到后应当调用CSCACheckEntitleInfo进行判断是否需要显示
               MAIL:为普通优惠信息,用户收到后需要在屏幕上显示提示信息,
           进入邮箱可以查看和删除 
               COMMAND消息在回调中最后的一个参数为响应文本信息的地址，ForcePlay同时包含有节目三要素
********************************************************************/
typedef enum _TRCaCommand
{
    CA_COMMAND_MAIL  =  100,                             //邮件
    CA_COMMAND_ANNOUNCEMENT,                             //公告
    CA_COMMAND_NOTIFICATION,                             //通知
    CA_COMMAND_FINGERPRINT,                              //指纹,收到此消息后需要立即来取数据,并需要显示5-10秒,不能够被其他界面覆盖显示
    CA_COMMAND_FORCEPLAY,                                //强制切台
    CA_COMMAND_MAX                                       //无效字段
}TRCaCommand_E;

/******************************************************************** 
  结构体作用:  定义了腾锐CA抛给应用的所有卡消息
  
  特殊约定:    显示字段和动作可由应用自行决定
********************************************************************/
typedef enum  _TRCaStatus 
{ 
	CA_STATUS_CARD_INSERT = 200,                        //卡插入并复位成功,
	CA_STATUS_CARD_REMOVE,                              //卡拔出,
	CA_STATUS_CARD_ERROR,                               //卡插入但复位不成功,错误卡,
	CA_STATUS_CARD_UNKNOWN,                             //未识别卡,
	CA_STATUS_CARD_MAX                                  //无效字段  
}TRCaStatus_E; 

/********************************************************************  
  结构体作用:  定义了腾锐CA抛给应用的所有Alarm消息,需要应用在屏幕上显示
  
  特殊约定:    显示字段为腾锐要求,不可更改;
               ALARM消息显示的时候需要关闭音视频,
               CA_ALARM_CANCEL需要打开音视频
********************************************************************/
typedef enum _TRCaAlarm
{
	CA_ALARM_INPUT_CA_MOUDLE = 300,             //显示 "E01 请插入CA模块"
	CA_ALARM_CA_EEPROM_FAILURE,                 //显示 "E02 CA模块EEPROM失败" 
	CA_ALARM_CA_FAILURE1,                       //显示 "E03 CA模块失败" 
	CA_ALARM_SC_INPUT,                          //显示 "E04 请插入智能卡"
	CA_ALARM_SC_UNKNOWN ,                       //显示 "E05 系统不识别此卡"
	CA_ALARM_SC_FAILURE,                        //显示 "E06 智能卡失败"
	CA_ALARM_SC_CHECKING,                       //显示 "E07 正在检测智能卡"
	CA_ALARM_CA_FAILURE2,                       //显示 "E08 CA模块失败"
	CA_ALARM_SC_EEPROM_FAILURE,                 //显示 "E09 智能卡EEPROM失败"
	CA_ALARM_PMT_FORMAT_ERROR ,                 //显示 "E10 PMT格式错误"
	CA_ALARM_SC_NOT_MATCHED,                    //显示 "E11 机卡不匹配"	
	CA_ALARM_SC_FEED ,                          //显示 "E12 请喂养子卡"
	CA_ALARM_SERVICE_NOT_AVIALABLE,             //显示 "E13 没有有效节目"
	CA_ALARM_SERVICE_NOT_AUTHORIZED,            //显示 "E14 节目没有授权"
	CA_ALARM_SERVICE_AUTHORIZED ,               //显示 "E15 接收到授权"
	CA_ALARM_SERVICE_IS_SCRAMBLED,              //显示 "E16 当前节目已加密"
	CA_ALARM_SERVICE_CURRENTLY_SCRAMBLED,       //显示 "E17 当前节目已加密"
	CA_ALARM_NOT_ALLOWED_IN_BROADCASTER1,       //显示 "E18"
	CA_ALARM_NOT_ALLOWED_IN_BROADCASTER2,       //显示 "E19"
	CA_ALARM_NOT_ALLOWED_IN_BROADCASTER3 ,      //显示 "E20 运营商无授权"
	CA_ALARM_SERVICE_IS_DESCRAMBLED1,           //显示 "E21"    
	CA_ALARM_SERVICE_IS_DESCRAMBLED2,           //显示 "E22"
	CA_ALARM_NOT_ALLOWED_IN_REGION,             //显示 "E24 无本区域授权"
	CA_ALARM_SC_NOT_COMPATIABLE ,               //显示 "E25 智能卡不兼容"
	CA_ALARM_SERVICE_UNKONWN,                   //显示 "E26 系统不识别当前节目"
	CA_ALARM_SERVICE_NOT_RUNNING,               //显示 "E27 当前节目不在运行"   
	CA_ALARM_SC_LOCKED,                         //显示 "E28 智能卡被锁定"
	CA_ALARM_PARENTAL_LOCKED1,                  //显示 "E29"
	CA_ALARM_PARENTAL_LOCKED2 ,                 //显示 "E30"
	CA_ALARM_PARENTAL_LOCKED3,                  //显示 "E31 父母控制锁"
	CA_ALARM_COUNTRY_NOT_ALLOWED,               //显示 "E32 国家代码没有授权" 
	CA_ALARM_NO_AUTHORIZE_DATA,                 //显示 "E33 没有授权数据"
	CA_ALARM_ILLEGAL_BOX,                       //显示 "E34 非授权机顶盒"
	CA_ALARM_NO_SIGNAL ,                        //显示 "E35 没有信号"
	CA_ALARM_CANCEL,                            //播放清流或正常解扰,取消显示
	CA_ALARM_MAX                                //无效字段
}TRCaAlarm_E;

/********************************************************************
  枚举作用:  定义了腾锐CA抛给应用的的ALARM消息第三个参数的值
  
  特殊约定:  提示ALARM消息时,应用需要根据第三个参数来确定是否关闭音视频
********************************************************************/
typedef enum _TRCaAVControl
{
	EM_TRCA_STOPAV,     //关闭音视频
	EM_TRCA_STOPAUDIO,  //关闭音频
	EM_TRCA_STOPVIDEO,  //关闭视频
	EM_TRCA_OPENAV,     //打开音视频
	EM_TRCA_OPENAUDIO,  //打开音频
	EM_TRCA_OPENVIDEO,  //打开视频
	EM_TRCA_MAX         //无效字段
}TRCaAVControl_E;

/********************************************************************
  枚举作用:  定义了腾锐CA抛给应用的文本信息的三种类型
  
  特殊约定:    ANNOUNCEMENT为紧急事件,用户收到后应当立即显示,可以退出和删除,
           进入公告箱可以查看和删除
               NOTIFICATION为缴费通知,用户收到后应当立即显示,不可删除
               MAIL:为普通优惠信息,用户收到后需要在屏幕上显示提示信息,
           进入邮箱可以查看和删除 
********************************************************************/
typedef enum _TRCaMsgType
{
	CA_MSG_MAIL,        //邮件
	CA_MSG_ANNOUNCE,    //公告
	CA_MSG_NOTIFICATION //通知
}TRCaMsgType_E;

/******************************************************************** 
  枚举作用:  定义了腾锐CA抛给应用的文本信息的三种显示模式
  
  特殊约定: CA_MSG_NORMAL,需要通知用户,用户可以进入邮箱或公告箱中进行察看
            CA_MSG_TIMED ,需要立即显示,时间在文本信息结构体中 
            CA_MSG_FORCED,需要立即强制显示
********************************************************************/
typedef enum _TRCaMsgClass
{
	CA_MSG_NORMAL,
	CA_MSG_TIMED,      /* Automatic erase after wRemainTime seconds. */
	CA_MSG_FORCED      /* Forced display message. */
}TRCaMsgClass_E;

/********************************************************************  
  枚举作用:  配置CA Flash相关地址返回值
  
  特殊约定:   
********************************************************************/
typedef enum _TRCaFlashAddrErr
{
	EM_FLASH_FATAL_ERROR = -4,        
    EM_FLASH_PARAM_ERROR = -3,
	EM_FLASH_LENGTH_ERROR = -2,
	EM_OTHER_FAILURE = -1,
	EM_FLASH_SET_SUCCESS = 0
}TRCaFlashAddrErr_E;

/********************************************************************  
  枚举作用:  获取授权信息返回值
  
  特殊约定:   
********************************************************************/
typedef enum _TRCaEntitleRet
{
    EM_ENTITLE_VALID  = 0,          //有效授权
    EM_ENTITLE_PARA_ERR,            //参数错误
    EM_ENTITLE_NO_ENTITLE,          //没有授权
    EM_ENTITLE_VALID_LIMITED,       //有效授权,但有限制
    EM_ENTITLE_CLOSED,              //授权关闭
    EM_ENTITLE_OPEN                 //暂停授权
}TRCaEntitleRet_E;

/********************************************************************  
  枚举作用:  函数CSCAReadDataFromMaster返回值
  
  特殊约定:   
********************************************************************/
typedef enum _TRCaReadMasterRet
{
    EM_READ_MASTER_OK = 0,              //读取成功
    EM_READ_MASTER_SC_ERROR = 1,        //智能卡错误
    EM_READ_MASTER_DATA_ERROR = 3,      //没有获取到有效数据
    EM_READ_MASTER_NOT_SUPPORT = 5,     //智能卡属性错误
    EM_READ_MASTER_STATE_ERROR = 6,     //没有获取到有效数据
    EM_READ_MASTER_UNKNOW_ERROR = 9     //未知错误
}TRCaReadMasterRet_E;

/********************************************************************  
  枚举作用:  函数CSCAWriteDataToSlaver返回值
  
  特殊约定:   
********************************************************************/
typedef enum _TRCaWriteSlaverRet
{
    EM_WRITE_SLAVER_OK = 0,             //操作成功
    EM_WRITE_SLAVER_NOT_OK = 1,         //智能卡失败或喂养数据失败
    EM_WRITE_SLAVER_DATA_ERROR = 3,     //喂养数据失败
    EM_WRITE_SLAVER_WRITE_ERROR = 4,    //写智能卡失败
    EM_WRITE_SLAVER_NOT_SUPPORT = 5,    //智能卡属性错误
    EM_WRITE_SLAVER_STATE_ERROR = 6,    //没有获取到有效数据
    EM_WRITE_SLAVER_MATCH_ERROR = 7,    //子母卡不匹配
    EM_WRITE_SLAVER_UNKNOE_ERROR = 9    //未知错误
}TRCaWriteSlaverRet_E;

/******************************************************************** 
  结构体作用:  ForcePlay相关信息
  
  特殊约定:   锁定遥控器之后又两种办法解除锁定: 1.重启机顶盒；2.前端发m_ucLockedFlag=0的指令
                若切播的节目不在节目列表中，应用忽略此消息
********************************************************************/
typedef struct _TRCaForcePlay
{
    BYTE    m_ucLockedFlag;  //TRUE:遥控器锁定，不允许用户切台；FALSE:允许用户切台

    WORD    m_wNetWorkId;   //节目三要素
    WORD    m_wTsId;
    WORD    m_wServiceId;
    
    WORD    m_wContentLen;  //reserved
    BYTE*   m_pucContent;   //reserved
}TRCaForcePlay_S;

/******************************************************************** 
  结构体作用:  CA Flash地址信息
  
  特殊约定:   
********************************************************************/
typedef struct _TRCaFlashAddress
{
    BOOL    m_bCaFlashAddressFlag;  //TRUE: 正式app;FALSE: TestApp
    DWORD   m_dwMainFlashAddr;      //主Flash 地址
    DWORD   m_dwMainFlashLength;    //主Flash 长度 
    DWORD   m_dwBackupFlashAddr;    //备份Flash  地址若不使用直接传递0
    DWORD   m_dwBackupFlashLength;  //备份Flash  长度若不使用直接传递0
    
    DWORD   m_dwTxtMsgFlashAddr;    //文本信息Flash地址
    DWORD   m_dwTxtMsgFlashLenth;   //文本信息Flash长度
    DWORD   m_dwReserve3;           //保留，针对某些CA 特殊需求的地址
    DWORD   m_dwReserve4;           //保留，针对某些CA 特殊需求的地址
}TRCaFlashAddress_S;

/********************************************************************  
  结构体作用:  定义了腾锐CA抛给应用的文本信息附带的时间戳
  
  特殊约定:   
********************************************************************/
typedef struct _TRCaTimeStamp
{
	WORD    m_wYear;         // for example 1998,  m_wYear = 1998
	BYTE    m_ucMonth;       // 1 to 12 
	BYTE    m_ucDay;         // 1 to 31 
	BYTE    m_ucHour;        // 0 to 23 
	BYTE    m_ucMinute;      // 0 to 59 
	BYTE    m_ucSecond;      // 0 to 59 
}TRCaTimeStamp_S;

/******************************************************************** 
  结构体作用:  定义了腾锐CA抛给应用的文本信息结构,包括邮件,公告,通知
  
  特殊约定:    m_wPeriod仅对通知类型的文本有效
********************************************************************/
typedef struct _TRCaTxtMsg
{
   WORD             m_wIndex;                           //索引,邮件和公告各有各自的索引
   BYTE             m_ucPriority;                       //reserve
   WORD             m_wPeriod;                          //

   TRCaMsgClass_E   m_eClass;                           //级别,一般,定时显示,强制显示
   TRCaMsgType_E    m_eType;                            //信息类型,邮件,公告或是通知
   
   TRCaTimeStamp_S  m_sMsgTime;                         //reserve
   TRCaTimeStamp_S  m_sCreateTime;                      //创建时间
   
   WORD             m_wTitleLen;                        //标题长度
   BYTE             *m_pucMsgTitl;                      //标题内容
   WORD             m_wDataLen;                         //内容长度
   BYTE             *m_pucTxtMsgData;                   //信息内容
}TRCaTxtMsg_S;

/******************************************************************** 
  结构体作用:  授权信息
  
  特殊约定:   无 
********************************************************************/
typedef struct _TREntitleInfo
{
    WORD                 m_wProductId;      //产品ID,0--319

    TRCaEntitleRet_E     m_eEntitleStatus;  //此产品的授权状态

    TRCaTimeStamp_S      m_sStartTime;      //开始时间
    TRCaTimeStamp_S      m_sEndTime;        //结束时间
}TREntitleInfo_S;

/******************************************************************** 
  结构体作用:  指纹信息的结构
  
  特殊约定:    无
********************************************************************/
typedef struct _TRFingerPrintInfo
{
    BYTE    m_ucNumLen;         //指纹内容长度
    BYTE   *m_pucNumData;       //指纹内容
    BYTE    m_ucContentLen;     //reserved
    BYTE   *m_puccontentData;   //reserved
}TRFingerPrintInfo_S;

/**************************************************************************
*Function Name: CSCAGetSmartCardVersion
*   获取智能卡版本信息
*
*Parameters
*   pucVerBuf[out]:指向存储CA版本信息的空间
*   ucBufSize[in]:给定一个最大长度值,不能够小于7个字节
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
*
*Note:  可在需要显示智能卡的版本号时调用
**************************************************************************/
INT CSCAGetSmartCardVersion(BYTE* pucVerBuf,  BYTE ucBufSize);

/**************************************************************************
* Function Name: CSCAGetSmartCardID
*   获取智能卡的卡号
*
*Parameters	
*    pcCardId[out]:以字符串的形式返回智能卡卡号，
*    ucCardIdBuff[in]:为智能卡卡号所准备的Buff大小,长度为不少于27个字节
*         
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
*Note:用户需要读取卡号时调用
**************************************************************************/
INT	CSCAGetSmartCardID(char* pcCardId, BYTE ucCardIdBuff);


/**************************************************************************
*Function Name: CSCASetDebugMode
*  设置打印模式,用于应用调试
*
*Parameters 
*	dwMode[in]:每个模块都有相应的打印开关		
*   
*Note:调试时在CA初始化之前调用
**************************************************************************/
void CSCASetDebugMode(DWORD dwMode);

/**************************************************************************
* Function Name: CSCADelOneTxtMsg
*   删除邮件或公告的某一个文本信息
*
*Parameters	
*  eTxtMsgType[in]:删除消息的类型,具体请参考TRCaMsgType_E,
* wTxtMsgIndex[in]:所要删除消息的索引,邮件0--31；公告0--29；
*         
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
* Note:用户删除某一类型的某一消息的时候调用,例如删除邮件列表的第一封邮件
*   则eTxtMsgType = CA_MSG_MAIL, wTxtMsgIndex = 0
**************************************************************************/
INT CSCADelOneTxtMsg(TRCaMsgType_E eTxtMsgType, WORD wTxtMsgIndex);

/**************************************************************************
* Function Name: CSCADelTxtMsg
*   删除邮件或公告下的所有文本信息
*
*Parameters	
*    eTxtMsgType[in]:删除消息的类型,具体请参考TRCaMsgType_E,仅对邮件和公告有效
*         
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:用户删除邮件或公告所有的信息时调用,例如清空邮箱,eTxtMsgType = CA_MSG_MAIL
**************************************************************************/
INT CSCADelAllTxtMsg(TRCaMsgType_E eTxtMsgType);

/**************************************************************************
* Function Name: CSCAGetTxtMsgInfo
*   获取信息内容
*
*Parameters	
*    eTxtMsgType[in]:获取信息的类型
*        ucIndex[in]:所需要获取信息的索引,从0开始，邮箱0--31，共32封；公告0--29，共30封；通知为0
*      psTxtMsg[out]:将信息存储到以psMsg为开始的一段空间
*Return Values
*	0 : SUCCESS 
*	-1: FAILURE
*
*Note:用户主动读取邮件或公告的时候调用,获得邮件或公告或通知信息
**************************************************************************/
INT CSCAGetTxtMsgInfo(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, TRCaTxtMsg_S *psTxtMsg);

/**************************************************************************
*Function Name:CSCAGetTxtMsgNum
* 获取邮件或公告信息总数和剩余可存储的个数
*
*Parameters 
*	eMsgType[in]:信息类型,
*   pucTotalNum[out]:文本信息总数,邮件1--32;公告1--30
*   pucEmptyNum[out]:剩余空间数目,邮件1--32;公告1--30
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:用户进入公告箱或邮件箱,需要显示存储信息时候调用
**************************************************************************/
INT CSCAGetTxtMsgNum(TRCaMsgType_E eTxtMsgType, BYTE *pucTotalNum, BYTE *pucEmptyNum);

/**************************************************************************
*Function Name:CSCASetTxtMsgReadFlag
* 设置某一类的某一个信息是否已读标志
*
*Parameters 
*   eMsgType[in]:信息类型
*	ucIndex[in]:某一类信息存储的下标，从0开始,邮箱0--31，共32封; 公告0--29，共30封.
*	bRead[in]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:用户对单条邮箱或公告进行设置的时候使用
**************************************************************************/
INT CSCASetTxtMsgReadFlag(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, BOOL bRead);

/**************************************************************************
*Function Name:CSCASetTxtMsgReadFlag
* 查询某一类的某一个文本信息是否已读
*
*Parameters 
*   eMsgType[in]:信息类型
*	ucIndex[in]:某一类信息存储的下标，从0开始,邮箱0--31，共32封; 公告0--29，共30封.
*	pbRead[OUT]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:用户进入邮箱或公告箱查看文本信息时调用
**************************************************************************/
INT CSCAGetTxtMsgReadFlag(TRCaMsgType_E eTxtMsgType, BYTE ucIndex, BOOL *pbRead);

/**************************************************************************
*Function Name:CSCAHaveNewTxtMsgNum
* 返回当前某一类型信息还有多少封未读信息
*
*Parameters 
*	eMsgType[in]:信息类型
*   pucNotReadNum[out]:未读信息数目,范围为:邮件1--32; 公告1--30
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:用户查询邮箱或公告存储信息时候调用
**************************************************************************/
INT CSCAHaveNewTxtMsgNum(TRCaMsgType_E eTxtMsgType, BYTE *pucNotReadNum);

/**************************************************************************
*Function Name:CSCAGetEntitleTotalNum
* 获取授权信息的总条数
*
*Parameters 
*	pwTotalNum[out]:获取授权信息的总条数,320
*   
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:用户进入授权信息界面查看授权信息时,先调用此函数获取授权信息总数
**************************************************************************/
INT CSCAGetEntitleTotalNum(WORD *pwTotalNum);

/**************************************************************************
*Function Name:CSCAGetEntitleInfo
*   获取单条授权信息
*
*Parameters 
*	wIndex[in]:授权信息的索引,最大值通过INT 
*       CSCAGetEntitleTotalNum(WORD *pwTotalNum)获取, 范围为索引范围为0--319
*   psEntitleInfo[out]:指向索引为wIndex的授权信息的存储空间
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       用户进入授权界面查看授权信息时调用
**************************************************************************/
INT CSCAGetEntitleInfo(WORD wIndex, TREntitleInfo_S *psEntitleInfo);

/**************************************************************************
*Function Name:CSCAGetRating
*   获取智能卡的观看级别控制信息,级别为0--10
*
*Parameters 
*	pucRating[out]:返回给应用所查询的级别信息,级别范围0--10
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       用户查询观看级别时调用
**************************************************************************/
INT CSCAGetRating(BYTE *pucRating);

/**************************************************************************
*Function Name:CSCASetRating
*   设置智能卡的观看级别控制信息,级别为0--10
*
*Parameters 
*	        pucPin[in]:设置观看级别输入的6位pin码的首地址
*           ucPinLen[in]:输入Pin码的长度，为6，若错误则返回失败
*   
*           ucRating[out]:所需要设置的观看级别,范围为0--10
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       用户设置观看级别时调用
*       必须输入正确的PIN码之后才能够设置成功
**************************************************************************/
INT CSCASetRating(BYTE* pucPin, BYTE ucPinLen, BYTE ucRating);

/**************************************************************************
*Function Name:CSCAOpenParentalControl
*   进入有父母锁控制的节目
*
*Parameters 
*	pucPin
        [in]:进入父母锁控制节目时输入位的pin码的首地址
*   ucPinLen
        [in]:PIN码长度必须为6位,否则返回失败
*   
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       用户进入父母控制的节目之前调用
*       必须输入正确的PIN码之后才能够进入节目,否则CA lib不会进行解扰,返回失败
**************************************************************************/
INT CSCAOpenParentalControl(BYTE* pucPin, BYTE ucPinLen);

/**************************************************************************
*Function Name:CSCAChangePin
*   更改智能卡的PIN码
*
*Parameters 
*	aucOldPin
        [in]:旧的pin码,如果错误则返回失败,PIN码长度,必须为6位
*   pucNewPin
        [in]:新的PIN码,必须为6位,否则返回失败
*   ucPinLen
        [in]:输入的新Pin码和旧Pin码的长度，必须为6为,否则返回失败
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
*Note:
*       用户进入父母控制的节目之前调用
*       必须输入正确的PIN码之后才能够进入节目,否则CA lib不会进行解扰,返回失败
**************************************************************************/
INT CSCAChangePin(BYTE* pucOldPin, BYTE* pucNewPin, BYTE ucPinLen);

/**************************************************************************
*Function Name:CSCAReadDataFromMaster
*   从母卡中获取喂养数据
*
*Parameters 
*   
*Return Values: 
*	详见 TRCaReadMasterRet_E
*Note:
*       子母卡喂养时，从母卡读取数据时调用
**************************************************************************/
TRCaReadMasterRet_E CSCAReadDataFromMaster(void);

/**************************************************************************
*Function Name:CSCAWriteDataToSlaver
*   从母卡中获取的喂养数据通过此接口写入相配对的子卡中
*
*Parameters 
*	
*Return Values: 
*	详见 TRCaWriteSlaverRet_E
*Note:
*       子母卡喂养时，从母卡读取的数据写入子卡中时调用
**************************************************************************/
TRCaWriteSlaverRet_E CSCAWriteDataToSlaver(void);

/**************************************************************************
*Function Name:CSCASetFlashAddress
* 设置CA 内核存储数据所需的FLASH 地址
*
*Parameters
*  pstFlashAddress: CA Flash 相关信息
                                
pstFlashAddress->m_bCaFlashAddressFlag:1 正常模式,正常读写FLash
                                        0 TESTAPP 模式,数据不写入flash

pstFlashAddress->m_dwMainFlashAddr: 分配给CA内核主数据区地址,必须为一个非零值
pstFlashAddress->m_dwMainFlashLength: CA内核主数据区大小,为64K

pstFlashAddress->m_dwBackupFlashAddr:分配给CA内核备份数据区地址
pstFlashAddress->m_dwBackupFlashLength:CA内核备份数据区大小,为64K 

pstFlashAddress->m_dwTxtMsgFlashAddr:分配给CA 文本信息的数据区域地址
pstFlashAddress->m_dwTxtMsgFlashLenth:文本信息数据区域的长度,为64k.
*                     
*Return Values: 
*	见CaFlashAddrErr_E定义

Note:
        在CA初始化之前调用此函数进行Flash地址配置  
        若该函数返回错误，将导致CA初始化失败!!
**************************************************************************/
TRCaFlashAddrErr_E CSCASetFlashAddress(TRCaFlashAddress_S *pstFlashAddress);

#ifdef  __cplusplus
}
#endif

#endif //__CS_TRCA_INTERFACE_H__

