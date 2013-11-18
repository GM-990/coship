#ifndef __WEBKIT_PUBLIC_H_
#define __WEBKIT_PUBLIC_H_

#ifdef  __cplusplus
extern "C" {
#endif
void CSRegistHTML5Video();
void CSInitQNTProtocolEX();

/**
@brief  用户键值与网页键值转换表，用于网页使用的键值与UDI定义的键值不一致的情况

@param[in] pnKeyTableAddr 键值对应表
@param[in] nFirstDimension 第一维数组长度
@return 如果成功返回GT_RET_SUCCESS，如果失败返回GT_RET_FAILURE
@note ?
例子:
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

