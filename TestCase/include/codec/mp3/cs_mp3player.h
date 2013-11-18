/******************************************************************************
FileName:  cs_pvrmgrconfig.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
MP3播放器接口

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create
1.1         2008-08-25      罗金华           增加对mo3数据注入播放的支持
1.2         2009-07-31      903571           增加频谱分析接口
*******************************************************************************/
#ifndef _CS_MP3_PLAYER_H_
#define _CS_MP3_PLAYER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"

#define  CSMP3_APIC_SIZE 256



typedef enum _GetMp3PicType_E
{
	GETMP3PICTYPE_POINTER,
	GETMP3PICTYPE_PATH,
	GETMP3PICTYPE_NONEED,
	GETMP3PICTYPE_UNKNOW
}GetMp3PicType_E;

typedef void  (*Cb_Mp3Pic_Free)(char *);
/*!
	Mp3信息
*/
typedef struct _CSMP3Info_S
{
	BYTE m_cTAG[3];		/*! 标签*/	
	BYTE m_cSong[30];	/*! 歌曲名(从文件中解析出来的歌曲名)*/
	BYTE m_cArtist[30];	/*! 演唱家名*/
	BYTE m_cAlbum[30];	/*! 专集*/   
	BYTE m_cYear[4];		/*! 出版年代*/
	BYTE m_cComment[30];/*!备注*/      
	BYTE m_cGenre;
	DWORD m_dwTimeLen;/*!歌曲可播放长度*/
	int m_nNumberOfCh; //声道数
	UINT m_unSampRate;  //采样率
    BYTE    m_ApicPath[CSMP3_APIC_SIZE];      /*MP3 海报保存的路径*/
	
	char                  *m_pcPicStr;
	unsigned int         m_nPicSize;
	Cb_Mp3Pic_Free   m_fnPicFree;
} CSMP3Info_S;

/*!
	回调通知类型
*/
typedef enum _MP3NotifyType_E
{
	EM_MP3_PLAY_END = 0,  /*表示歌曲播放结束*/
	EM_MP3_PLAY_INVALID_FILE ,   /*表示歌曲歌曲不能播放，比如如不是MP3文件*/
	EM_MP3_PLAY_INTERRUPT, /*表示歌曲播放过程中停止，如U盘掉了，文件中有损坏等*/
	EM_MP3_PLAY_FLUSH_FINISH,    //流式播放flush完成
	EM_MP3_PLAY_UNKOWN/*其它*/
}MP3NotifyType_E;

/*!
	回调通知函数定义
*/
typedef void (*MP3PlayerNotify_F)(HCSHANDLE hMp3Player,MP3NotifyType_E eType,DWORD dwNotifyData,void* pUserData);

/*!
	启动播放MP3文件
@param [in] hMp3Player 播放器对象句柄
@param [in] szFile 需要播放的mp3文件
\return 操作成功返回TRUE, 否则返回FALSE
\note 本函数内会启动一个异步线程用于播放
*/
BOOL CSMP3PlayerStart(HCSHANDLE hMp3Player, const char *szFile);

/*!
	创建MP3播放器对象
@param [in] fnCb 播放器回调响应函数(允许为空)
@param [in] pUserData 用户自定义数据，回调时作为参数回传给用户
\return 创建成功则返回播放器句柄，否则返回NULL
\note 创建出来的播放器句柄既可用于mp3文件播放，也可以用于mp3数据注入播放
*/
HCSHANDLE CSMP3PlayerCreate(MP3PlayerNotify_F fnCb, void *pUserData);

/*!
	停止MP3播放
@param [in] hMp3Player 播放器对象句柄
\return 操作成功返回TRUE, 否则返回FALSE
\note 停止播放是同步停止的，即必须等到异步播放线程结束才返回
*/
BOOL CSMP3PlayerStop(HCSHANDLE hMp3Player);

/*!
	暂停MP3播放
@param [in] hMp3Player 播放器对象句柄
\return 操作成功返回TRUE, 否则返回FALSE
*/
BOOL CSMP3PlayerPause(HCSHANDLE hMp3Player);

/*!	
	恢复MP3播放
@param [in] hMp3Player 播放器对象句柄
\return 操作成功返回TRUE, 否则返回FALSE
*/
BOOL CSMP3PlayerResume(HCSHANDLE hMp3Player);


/* 新增接口
    +--------------------------------------------------------------------------------------------------
    输入参数 bHaspic 设为1时，返回psOutInfo 内的图片指针数据，同时返回一个释放
               该指针的函数，在使用该图片指针后，调用该函数释放该指针。
               
   [注意] 

   输入参数 bHaspic 设为0时，返回psOutInfo 内的图片路径，该路径应该为内存空间
               文件系统，以保证MP3封面图片的解析速度。  
               
             请注意  APIC_JPG_PATH 和APIC_PNG_PATH 定义的路径

  \return 操作成功返回TRUE, 否这返回FALSE
     --------------------------------------------------------------------------------------------------+*/
BOOL CSMP3PlayerGetMp3InfoWithPic(const char *szFile,CSMP3Info_S *psOutInfo, GetMp3PicType_E eGetpic);

/*!
	获取MP3文件信息
@param [in] szFile mp3文件
@param [out] psOutInfo 文件的mp3信息
\return 操作成功返回TRUE, 否则返回FALSE
*/
BOOL CSMP3PlayerGetMp3Info(const char *szFile,CSMP3Info_S *psOutInfo);

/*!
	获取当前播放位置
@param [in] hMp3Player 播放器对象句柄
\return 当前播放位置
\note 播放器未启动时,返回0
*/
DWORD CSMP3PlayerGetTime(HCSHANDLE hMp3Player);

/*!
	获取当前正在播放的文件的路径
@param [in] hMp3Player 播放器对象句柄
\return 返回播放器正在播放的文件路径
\note 仅对mp3文件播放适用
\note 如果播放器未启动或者mp3数据注入播放时返回NULL
*/
const char* CSMP3PlayerGetFilePath(HCSHANDLE hMp3Player);

/*!
	启动mp3数据注入播放
@param [in] hMp3Player 播放器对象句柄
\return 操作成功返回TRUE, 否则返回FALSE
\note 函数内会启动一个异步解码线程
\note 请每次播放新的一首mp3歌曲时,调用本函数重新启动mp3数据注入播放
*/
BOOL CSMP3PlayerStartInject(HCSHANDLE hMp3Player);

/*!
	Mp3数据注入
@param [in] hMp3Player 播放器对象句柄
@param [in] pMp3Buffer mp3数据
@param [in] nBufLen 注入的mp3数据长度
\note mp3数据注入前先调用CSMP3PlayerGetFreeSize获取可注入的数据长度
\note 每次注入的数据长度不得大于播放器可注入的长度
\note 新的一首mp3歌曲数据注入前,请先调用CSMP3PlayerStartInject重新启动mp3注入播放
\note 不允许多首mp3歌曲数据连续注入
\note 本函数是非堵塞的,立即返回
\note 返回值表明注入成功与否
*/
BOOL CSMP3PlayerInject(HCSHANDLE hMp3Player, BYTE *pMp3Buffer, int nBufLen);

/*! 
	获取可注入的数据长度
@param [in] hMp3Player 播放器对象句柄
\return 播放器可注入的数据长度
\note 本接口仅对mp3数据注入播放适用
\note 如果播放器未启动或者mp3文件播放,则返回0
*/
int CSMP3PlayerGetFreeSize(HCSHANDLE hMp3Player);

/*!
	播放定位
@param [in] hMp3Player 播放器对象句柄
@param [in] dwSec 需要定位的目标位置( 单位为秒)
\return 操作成功返回TRUE, 否则返回FALSE
\note 目前本接口仅对数据源为文件的方式适用
\note 文件seek，调用前请先调用CSMP3PlayerGetMp3Info()来获得总时间，以便传入一个合法的时间。
\note 注意该函数是异步执行的，真正的seek是在读文件线程中实现的
*/
BOOL CSMP3PlayerSeek(HCSHANDLE hMp3Player,DWORD dwSec);

/*!
	获得版本号
@param [in] pcVersion 接收版本字串的buffer
@param [in] nSize buffer可用大小
\return 操作成功返回实际拷贝成功的数量
*/
int CSMP3GetVersion( char * pcVersion, int nSize );

/*!
   让播放器将缓冲区的数据全部播放完，并在数据播完后发送一个消息EM_MP3_PLAY_FLUSH_FINISH
@param[in]     hMp3Player   有效的播放器句柄
\return: 表明执行成功与否
\note: 本函数只应用在流式播放，文件播放不应该使用该接口
\note: 本函数是异步执行，返回值成功不代表数据播完，只有应用在注册的回调函数中收到EM_MP3_PLAY_FLUSH_FINISH时数据才真正播放完
\note: 在流式播放时一般用该函数来确认一首歌曲播放完成，可以切换到下一首歌曲
\note: 只有正在播放的状态下才能flush,如果是其它状态下，flush无效
*/
BOOL CSMP3PlayerFlush(HCSHANDLE hMp3Player);

/*!
	添加一个回调
@param[in]    hMp3Player   有效的播放器句柄
@param [in]   fnCb 回调函数
@param [in]   pUserData 用户自定义数据，回调时作为参数回传给用户，可以传空
\return 添加成功返回TRUE，失败FALSE
\note  回调函数和用户数据两者结合确定唯一的回调，只要其中一个不一样便认为是新的回调
\note  同一个回调再次注册返回失败
\note  pUserData是用户分配的内存，通常是需要全局保存，本函数内部不做拷贝
\note  如果已达最大回调数，则不能再添加，新的添加会失败
*/
BOOL CSMP3PlayerAddCallback(HCSHANDLE hMp3Player, MP3PlayerNotify_F fnCb, void *pUserData);

/*!
	移除一个回调
@param[in]    hMp3Player   有效的播放器句柄
@param [in]   fnCb 回调函数
@param [in]   pUserData 用户自定义数据，可以传空
\return 添加成功返回TRUE，失败FALSE
\note  回调函数和用户数据两者结合确定唯一的回调，只要其中一个不一样便认为是不同的回调
\note  移除不存在的回调返回失败
\note  pUserData和fnCb都必须和注册时相同
*/
BOOL CSMP3PlayerRemoveCallback(HCSHANDLE hMp3Player, MP3PlayerNotify_F fnCb, void *pUserData);

/*!
	销毁一个mp3播放器
@param[in]    hMp3Player   有效的播放器句柄
*/
void CSMP3PlayerDestroy(HCSHANDLE hMp3Player);
/*!
获取当前正在使用的MP3句柄
return 当前MP3的句柄
*/

HCSHANDLE  CSMP3PlayerGetUsedHandle();
/*!
获取当前MP3 所使用驱动的DEVICE ID
@param[in]    hMp3Player   有效的播放器句柄
*/
int  CSMP3PlayerGetDeviceId(HCSHANDLE hMp3Player);

/*!
	获取HTTP 注入播放MP3文件的duration 和当前播放时间
@param [in] http mp3文件locator
@param [in] nHTTPFilePos 文件的mp3播放位置
\return mp3播放时间或duration，单位是秒
*/
DWORD CSMP3PlayerGetHttpMp3TimeInfo(const char *pcURL,int nHTTPFilePos);

/*!
	获取HTTP 注入播放MP3文件的当前播放时间
@param [in] hMp3Player   有效的播放器句柄
\return mp3播放时间单位是秒
*/
DWORD CSMP3PlayerGetHttpMp3CurTime(HCSHANDLE hMp3Player);

/*!
	http播放MP3根据时间获取对应文件位置
@param [in] http mp3文件locator
@param [in] dwSec 需要定位的目标时间(单位为秒)
\return 目标时间对应的文件位置(单位为Byte)
\note 本接口仅对数据源为http文件的方式适用
\note 调用本接口前先通过相关接口来获得总时间，以便传入一个合法的时间，如果传入的时间超过总时间则返回结果未知
*/
int CSMP3PlayerHttpMp3GetPosByTime(const char* pcUrl, DWORD dwSec);

/*!
	清空放MP3注入播放相关缓冲区
@param [in] hMp3Player 播放对象句柄
\return 操作成功返回TRUE, 否则返回FALSE
\note 本接口清空的缓冲区包括解码缓冲区和PCM注入缓冲区
*/
BOOL CSMP3PlayerResetBuffer(HCSHANDLE hMp3Player);

#ifdef __cplusplus
}
#endif

#endif

