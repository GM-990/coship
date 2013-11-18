#ifndef CS_SCREENSHOT_H
#define CS_SCREENSHOT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	EM_SCREENSHOT_VIDEO = 0,           ///<��Ƶ��
	EM_SCREENSHOT_OSG = 1                ///<OSG��	
}CSScreenshotType_E;


/**
@brief ��ȡ��ǰϵͳ����������Ϊһ��jpg��ʽͼƬ�ļ�
@param[in] pcFileName �����ͼƬȫ·���ļ���
@param[in] eType ץȡ������ͼ�㣬�����CSScreenshotType_E
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ�ܷ���CSUDI_FAILURE��
*/

int CSScreenshotSaveToFile(CSScreenshotType_E eType, const char *pcFileName, int nFileNameLen);

#ifdef __cplusplus
}
#endif

#endif 


