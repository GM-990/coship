#ifndef sipmt_H
#define sipmt_H

#ifdef PRAGMA
#pragma interface "sipmt.h"
#endif

// {{{USR
#ifdef __cplusplus
extern "C" 
{
#endif

#include "mmcp_typedef.h"

#include "SICommonType.h"


/*
 *	音频节点
 */
typedef struct _SIAudioNode
{
	BYTE bAudioType;					// 音频类型，参考 iso-13818
	WORD wAudioPID;						//该音频流的 pid
	DWORD dwLanguage;					//音频语言，若不存在则为0，含义参考 iso_639 

} SIAudioInfo;


/*
函数名 SIPMTMessageCallback
	回调函数，不能被阻塞,否则后果严重

	void(*SIPMTMessageCallback)(
		NOTIFY_TYPE enmType,
		WORD wServiceID
	);	

Parameters
	
	enmType
		[out]枚举 表示回调状态有 COMPLETE:完成,TIMEOUT:超时,ABORT:用户取消（调用stop）
	wServiceID
		[out]所请求的service id

Return Values
		无
*/
typedef void (*SIPMTMessageCallback)(NOTIFY_TYPE enmType, WORD wServiceID);


/*
函数名 SIPMT_CreateInstance
	创建pmt搜索实例，该函数应在其他函数之前调用

	void SIPMT_CreateInstance();	

Parameters
	
	无

Return Values

HCSHANDLE:  创建成功返回实例的句柄，
            失败返回为NULL；
	
*/
HCSHANDLE SIPMT_CreateInstance();


/*
函数名 SIPMT_RegisterCallback
	注册回调函数，该函数应紧跟SIPMT_CreateInstance之后。

	void SIPMT_RegisterCallback(
		HCSHANDLE hInstance,
		SIPMTMessageCallback callback
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	callback
		[in] 回调函数的地址

Return Values

HCSHANDLE:  创建成功返回实例的句柄，
            失败返回为NULL；
	
*/
void SIPMT_RegisterCallback(HCSHANDLE hInstance, SIPMTMessageCallback callback);


/*
函数名 SIPMT_StartSearch
	开始pmt搜索

	void SIPMT_StartSearch(
		HCSHANDLE hInstance,
		WORD wPMTPID,
		WORD wServiceID,
		DWORD dwTimeout,
		DWORD dwDemuxID
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	wPMTPID
		[in] 要搜索的service 的PMT_ID
	wServiceID
		[in] 要搜索的service 的SEVICE_ID
	dwTimeout
		[in] 超时时间
	dwDemuxID
		[in] demux设备的ID，若只有一个demux设备可以设置成 0

Return Values

BOOL: 启动搜索成功返回为TRUE
      启动搜索失败返回为FALSE
	
*/
BOOL SIPMT_StartSearch(HCSHANDLE hInstance, WORD wPMTPID, WORD wServiceID, DWORD dwTimeout, DWORD dwDemuxID );


/*
函数名 SIPMT_Stop
	停止pmt搜索

	void SIPMT_Stop(
		HCSHANDLE hInstance,
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建

Return Values

BOOL: 停止搜索成功返回为TRUE
      停止搜索失败返回为FALSE
	
*/
BOOL SIPMT_Stop(HCSHANDLE hInstance);


/*
函数名 SIPMT_GetFirstAudio
	得到第一个音频流信息

	void SIPMT_GetFirstAudio(
		HCSHANDLE hInstance,
		SIAudioInfo* pAudio
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	pAudio
		[out]存放音频信息的结构体的地址

Return Values

HCSHANDLE:  成功返回第一个音频信息的句柄
          NULL表示该操作没有得到音频信息。
	
*/
HCSHANDLE SIPMT_GetFirstAudio(HCSHANDLE hInstance, SIAudioInfo* pAudio);


/*
函数名 SIPMT_GetNextAudio
	得到下一个音频流信息

	void SIPMT_GetNextAudio(
		HCSHANDLE hInstance,
		HCSHANDLE hPre,
		SIAudioInfo* pAudio
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	hPre
		[in] 前一个音频流的句柄
	pAudio
		[out]存放音频信息的结构体的地址

Return Values

HCSHANDLE:  成功返回下一个音频信息的句柄
          NULL表示该操作没有得到音频信息。
	
*/
HCSHANDLE SIPMT_GetNextAudio(HCSHANDLE hInstance, HCSHANDLE hPre, SIAudioInfo* pAudio);


/*
函数名 SIPMT_GetFirstStream
	得到pmt中第一个流的信息

	void SIPMT_GetFirstStream(
		HCSHANDLE hInstance,
		SIMpegInfo* pStream)
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	pStream
		[out]存放流信息的结构体对象的地址

Return Values

HCSHANDLE:  成功返回第一个流信息的句柄
          NULL表示该操作没有得到音频信息。
	
*/
HCSHANDLE SIPMT_GetFirstStream(HCSHANDLE hInstance, SIMpegInfo* pStream);


/*
函数名 SIPMT_GetNextStream
	得到pmt中下一个流的信息

	void SIPMT_GetNextStream(
		HCSHANDLE hInstance,
		HCSHANDLE hPreStream,
		SIMpegInfo* pStream
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	hPreStream
		[in] 前一个音频流的句柄
	pStream
		[out]存放流信息的结构体对象的地址

Return Values

HCSHANDLE:  成功返回下一个流信息的句柄
          NULL表示该操作没有得到流信息。
	
*/
HCSHANDLE SIPMT_GetNextStream(HCSHANDLE hInstance, HCSHANDLE hPreStream, SIMpegInfo* pStream);


/*
函数名 SIPMT_GetStreamDescriptor
	得到某一个流中的指定descriptor的原始数据

	void SIPMT_GetStreamDescriptor(
		HCSHANDLE hInstance,
		SIMpegInfo* pStream,
		BYTE tagValue,
		BYTE* pBuf,
		int* pnBuf
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	pStream
		[in]指向某个流的指针
	tagValue
		[in]指定的descriptor的标志
	pBuf
		[out]指定流中的descriptor的原始数据
	pnBuf
		[out] 指定流中的descriptor的原始数据长度

Return Values

BOOL:成功返回为TRUE
      失败返回为FALSE

*/
BOOL SIPMT_GetStreamDescriptor(HCSHANDLE hInstance, SIMpegInfo* pStream, BYTE tagValue, BYTE* pBuf, int* pnBuf);


/*
函数名 SIPMT_GetFirstStreamByType
	得到pmt中指定类型的第一个流信息

	void SIPMT_GetFirstStreamByType(
		HCSHANDLE hInstance,
		BYTE bType,
		SIMpegInfo* pStream
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	bType
		[in]指定流的类型
	pStream
		[out] 存放流信息的结构体对象的地址

Return Values

HCSHANDLE:  成功返回第一个流信息的句柄
          NULL表示该操作没有得到流信息。

*/
HCSHANDLE SIPMT_GetFirstStreamByType(HCSHANDLE hInstance, BYTE bType, SIMpegInfo* pStream);


/*
函数名 SIPMT_GetNextStreamByType
	得到pmt中指定类型的下一个流信息

	void SIPMT_GetNextStreamByType(
		HCSHANDLE hInstance,
		HCSHANDLE hPreStream,
		SIMpegInfo* pStream
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	hPreStream
		[in]前一个流信息的句柄
	pStream
		[out] 存放流信息的结构体对象的地址

Return Values

HCSHANDLE:  成功返回下一个流信息的句柄
          NULL表示该操作没有得到流信息。

*/
HCSHANDLE SIPMT_GetNextStreamByType(HCSHANDLE hInstance, HCSHANDLE hPreStream, SIMpegInfo* pStream);


/*
函数名 SIPMT_GetSectionData
	得到pmt的原始数据

	void SIPMT_GetSectionData(
		HCSHANDLE hInstance,
		BYTE* pBuffer,
		int *pnLength
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	pBuffer
		[out]指向原始数据的指针
	pnLength
		[out] 原始数据长度指针

Return Values

BOOL:成功返回为TRUE
      失败返回为FALSE

*/
BOOL SIPMT_GetSectionData(HCSHANDLE hInstance, BYTE* pBuffer, int *pnLength);


/*
函数名 SIPMT_GetFirstSubtitle
	得到第一个Subtitle的信息

	void SIPMT_GetFirstSubtitle(
		HCSHANDLE hInstance,
		SISubtitle* pSubtitle
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	pSubtitle
		[out]指向得到的Subtitle的信息的指针

Return Values

HCSHANDLE:  成功返回第一个Subtitle信息的句柄
          NULL表示该操作没有得到Subtitle信息。

*/
HCSHANDLE SIPMT_GetFirstSubtitle( HCSHANDLE hInstance, SISubtitle* pSubtitle );


/*
函数名 SIPMT_GetNextSubtitle
	得到第一个Subtitle的信息

	void SIPMT_GetNextSubtitle(
		HCSHANDLE hInstance,
		HCSHANDLE hPre,
		SISubtitle* pSubtitle
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	hPre
		[in]前一个Subtitle信息的句柄
	pSubtitle
		[out] 指向得到的Subtitle的信息的指针

Return Values

HCSHANDLE:  成功返回下一个Subtitle信息的句柄
          NULL表示该操作没有得到Subtitle信息。

*/
HCSHANDLE SIPMT_GetNextSubtitle( HCSHANDLE hInstance, HCSHANDLE hPre, SISubtitle* pSubtitle );


/*
函数名 SIPMT_GetFirstTeletext
	得到第一个Teletext的信息

	void SIPMT_GetFirstTeletext(
		HCSHANDLE hInstance,
		SITeletext* pTeletext
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	pTeletext
		[out]指向得到的Teletext的信息的指针

Return Values

HCSHANDLE:  成功返回第一个Teletext信息的句柄
          NULL表示该操作没有得到Teletext信息。

*/
HCSHANDLE SIPMT_GetFirstTeletext( HCSHANDLE hInstance, SITeletext* pTeletext );


/*
函数名 SIPMT_GetNextTeletext
	得到下一个Teletext的信息

	void SIPMT_GetNextTeletext(
		HCSHANDLE hInstance,
		HCSHANDLE hPre,
		SITeletext* pTeletext
	);	

Parameters
	
	hInstance
		[in] pmt搜索实例的句柄 SIPMT_CreateInstance所创建
	hPre
		[in]前一个Teletext信息的句柄
	pTeletext
		[out] 指向得到的Teletext的信息的指针

Return Values

HCSHANDLE:  成功返回下一个Teletext信息的句柄
          NULL表示该操作没有得到Teletext信息。

*/
HCSHANDLE SIPMT_GetNextTeletext( HCSHANDLE hInstance, HCSHANDLE hPre, SITeletext* pTeletext );


#ifdef __cplusplus
}
#endif
// }}}USR

// {{{USR

// }}}USR

#endif /* sipmt_H */
