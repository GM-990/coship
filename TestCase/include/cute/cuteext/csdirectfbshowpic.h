#ifndef CSDIRECTFBSHOWPIC_H
#define CSDIRECTFBSHOWPIC_H

#ifdef __cplusplus
extern "C"{
#endif
/**
@brief  功能描述:
                使用DirectFB显示图片

@param[in]
const char* pcPicPath 图像文件路径，会自动删除路径前置的"/workdir"

@return
int     QT未初始化              返回 -4
        传入参数指针为空        返回 -3
        传入的路径文件不存在    返回 -2
        传入路径为"/workdir"    返回 -1
        成功                    返回 0

@exception
@note
*/
int CSDfbShowPicture(const char* pcPicPath);

/**
@brief  功能描述:
                清除使用CSDfbShowPicture显示的图像数据并释放内存

@param[in]
        无
@return
int      成功                   返回0

@exception
@note
*/
int CSDfbShowClear(void);
#ifdef __cplusplus
}
#endif
#endif // CSDIRECTFBSHOWPIC_H
