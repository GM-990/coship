#ifndef __WEBKIT_PUBLIC_H_
#define __WEBKIT_PUBLIC_H_

#ifdef  __cplusplus
extern "C" {
#endif
void CSRegistHTML5Video();
void CSInitQNTProtocolEX();

/**
@brief  �û���ֵ����ҳ��ֵת����������ҳʹ�õļ�ֵ��UDI����ļ�ֵ��һ�µ����

@param[in] pnKeyTableAddr ��ֵ��Ӧ��
@param[in] nFirstDimension ��һά���鳤��
@return ����ɹ�����GT_RET_SUCCESS�����ʧ�ܷ���GT_RET_FAILURE
@note ?
����:
static int nMapKey[][2] = {
{CSUDI_VK_UP, PAGE_UP},
{CSUDI_VK_DOWN, PAGE_DOWN}, 
{CSUDI_VK_LEFT, PAGE_LEFT}, 
{CSUDI_VK_RIGHT, PAGE_RIGHT}
}; 

CSBrwMapKeyTable(nMapKey, sizeof(nMapKey)/sizeof(nMapKey[0][0])/2);
*/
int CSBrwMapKeyTable(int(* pnKeyTableAddr)[2], int nFirstDimension);

#ifdef  __cplusplus
}
#endif
#endif

