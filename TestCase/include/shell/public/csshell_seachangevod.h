
#ifndef CSSHELL_SEACHANGE_VOD_H
#define CSSHELL_SEACHANGE_VOD_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"

typedef DWORD(*CSShellSeaChangeVodGetAreaCode_F)(void);

typedef struct _CSShellSeaChangeVodFunc_S
{
	int m_nSize;	//该结构大小，等于sizeof(CSShellSeaChangeVodFunc_S)

	CSShellSeaChangeVodGetAreaCode_F m_fnGetAreaCode;
}CSShellSeaChangeVodFunc_S;


/**
@brief shellvod用来进行特定需求数据的获取.在vod中，有一些特定数据，根据项目不同，获取方式而不同，但是用方式是相同的。
这些特殊需求值的获取应用来做，应用将获取接口注册给shell vod。因此，shell vod只起到传值作用，比如，思迁vod需
要用区域码进行认证，而区域码获取规则是根据地区不同而不同的，此时，可将区域码获取动作由应用实现，将此动作通过调用
CSShellSeaChangeVodSetFunc设置给应用，js或者java应用中，要点播获取区域码时，调用CSShellVodSeaChangeGetAreaCode
获取区域码。
*/

/**
@brief 设置思迁vod区域码。由应用调用
@param[in] dwAreaCode 要设置的区域码
*/
void CSShellSeaChangeVodSetFunc(const CSShellSeaChangeVodFunc_S* psVodFunc);

/**
@brief 获取思迁vod区域码。
@return 返回思迁vod区域码。如果之前未进行设置，则默认区域码为-1;
*/
DWORD CSShellSeaChangeVodGetAreaCode();

#ifdef __cplusplus
}
#endif
/** @} */
#endif   
