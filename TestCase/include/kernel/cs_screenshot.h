#ifndef CS_SCREENSHOT_H
#define CS_SCREENSHOT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	EM_SCREENSHOT_VIDEO = 0,           ///<视频层
	EM_SCREENSHOT_OSG = 1                ///<OSG层	
}CSScreenshotType_E;


/**
@brief 获取当前系统截屏，保存为一个jpg格式图片文件
@param[in] pcFileName 保存的图片全路径文件名
@param[in] eType 抓取截屏的图层，定义见CSScreenshotType_E
@return 
- 成功返回CSUDI_SUCCESS。
- 失败返回CSUDI_FAILURE。
*/

int CSScreenshotSaveToFile(CSScreenshotType_E eType, const char *pcFileName, int nFileNameLen);

#ifdef __cplusplus
}
#endif

#endif 


