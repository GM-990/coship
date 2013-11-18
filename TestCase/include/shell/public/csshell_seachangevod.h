
#ifndef CSSHELL_SEACHANGE_VOD_H
#define CSSHELL_SEACHANGE_VOD_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"

typedef DWORD(*CSShellSeaChangeVodGetAreaCode_F)(void);

typedef struct _CSShellSeaChangeVodFunc_S
{
	int m_nSize;	//�ýṹ��С������sizeof(CSShellSeaChangeVodFunc_S)

	CSShellSeaChangeVodGetAreaCode_F m_fnGetAreaCode;
}CSShellSeaChangeVodFunc_S;


/**
@brief shellvod���������ض��������ݵĻ�ȡ.��vod�У���һЩ�ض����ݣ�������Ŀ��ͬ����ȡ��ʽ����ͬ�������÷�ʽ����ͬ�ġ�
��Щ��������ֵ�Ļ�ȡӦ��������Ӧ�ý���ȡ�ӿ�ע���shell vod����ˣ�shell vodֻ�𵽴�ֵ���ã����磬˼Ǩvod��
Ҫ�������������֤�����������ȡ�����Ǹ��ݵ�����ͬ����ͬ�ģ���ʱ���ɽ��������ȡ������Ӧ��ʵ�֣����˶���ͨ������
CSShellSeaChangeVodSetFunc���ø�Ӧ�ã�js����javaӦ���У�Ҫ�㲥��ȡ������ʱ������CSShellVodSeaChangeGetAreaCode
��ȡ�����롣
*/

/**
@brief ����˼Ǩvod�����롣��Ӧ�õ���
@param[in] dwAreaCode Ҫ���õ�������
*/
void CSShellSeaChangeVodSetFunc(const CSShellSeaChangeVodFunc_S* psVodFunc);

/**
@brief ��ȡ˼Ǩvod�����롣
@return ����˼Ǩvod�����롣���֮ǰδ�������ã���Ĭ��������Ϊ-1;
*/
DWORD CSShellSeaChangeVodGetAreaCode();

#ifdef __cplusplus
}
#endif
/** @} */
#endif   
