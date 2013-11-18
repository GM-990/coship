
#ifndef CS_JSENV_H
#define CS_JSENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

/******************************************************************************
Function Name:    CSJSLoadPropertyFile
	
Description:
	�����ļ����ּ���property�ļ�
Input:
    pcFileName ָ�����ļ���

Output:
    ��

Return Value:
    property�ľ��
Other:
    �ڵ��ô˷���֮ǰ���������úû�������"STB.systemPath"��"STB.innerPath"
*******************************************************************************/
HCSHANDLE CSJSLoadPropertyFile(const char *pcFileName);

/******************************************************************************
Function Name:    JSGetNonVolatileEnv
	
Description:
	��ȡ������Ļ���������
Input:
    pcFileName ָ�����ļ���
	pcKey ���������ļ�ֵ
	nLength �������pcValue�ĳ���

Output:
    pcValue ��key��Ӧ��ֵ

Return Value:
	����������ֵ�����keyû�ж�Ӧ�Ļ������������ؿ��ַ�����
Other:
*******************************************************************************/
int JSGetNonVolatileEnv(const char *pcFileName, const char * pcKey, char *pcValue, int nLength);

/******************************************************************************
Function Name:    JSGetNonVolatileEnvValueLen
	
Description:
	��ȡ���������ĳ��ȡ�
Input:
    pcFileName ָ�����ļ���
	pcKey ���������ļ�ֵ

Output:
    

Return Value:
	���������ĳ��ȣ�
	���keyû�ж�Ӧ�Ļ�������������0��
	�����ļ������ڡ�key�����ڣ�����-1��
Other:
*******************************************************************************/
int JSGetNonVolatileEnvValueLen(const char *pcFileName, const char * pcKey);

/******************************************************************************
Function Name:    JSSetNonVolatileEnv
	
Description:
	����������Ļ���������
Input:
    pcFileName ָ�����ļ���
	pcKey ���������ļ�ֵ
	pcValue  ����������ֵ 
Output:
    None

Return Value:
	
Other:
*******************************************************************************/
int JSSetNonVolatileEnv(const char *pcFileName, const char * pcKey, const char * pcValue);

/******************************************************************************
Function Name:    JSSaveEnv
	
Description:
	����ɹػ�����Ļ�������
Input:
	��

Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSSaveEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSRevertEnv
	
Description:
	�Ӵ洢�豸�лָ���������
Input:
	��
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSRevertEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSSetDefaultEnv
	
Description:
	�ѻ��������ָ���������
Input:
	��
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSSetDefaultEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSDeleteEnv
	
Description:
	�Ӵ洢�豸��ɾ����ӦpcKey�Ļ�������
Input:
	��
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSDeleteEnv(const char* pcFileName, char *pcKey);

/******************************************************************************
Function Name:    JSDeleteAllEnv
	
Description:
	�Ӵ洢�豸��ɾ�����л�������
Input:
	��
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSDeleteAllEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSGetNonVolatileEnvCount

Description:
��ȡ��������������
Input:
pcFileName ָ�����ļ���
Output:
None

Return Value:
	ָ�������ļ��е�key�ܸ���
Other:
*******************************************************************************/
int JSGetNonVolatileEnvCount(const char *pcFileName);

/******************************************************************************
Function Name:    JSGetNonVolatileEnvAt

Description:
��ȡָ����ŵĻ���������key��
Input:
pcFileName ָ�����ļ���
nIndex ���
pcKey ����������keyֵ���������
dwKeyLen  ����������keyֵ��buffer���� 
Output:
None

Return Value:
	��ȡָ�������ļ��ж�Ӧindex��key��ֵ
Other:
*******************************************************************************/
int JSGetNonVolatileEnvAt(const char *pcFileName, int nIndex, char * pcKey, DWORD dwKeyLen);

/******************************************************************************
Function Name: CSJSLoadPropertyFileFromEeprom

Description:
    �����ļ����ִ�eeprom�м���property�ļ�
    ��Ӧ�����ٸ��ݴ��ļ�����ȡ���ݵ�ʱ��ʵ���϶��Ǵ�eeprom�л�ȡ����
Input:
    pcFileName - ָ�����ļ���(ע:ֻ�����ļ���������·��)
    dwFlashAddress - �洢���ļ���eeprom����ʼ��ַ
    dwSize - �洢���ļ���eeprom�Ŀռ��С

Output:
    ��

Return Value:
    property�ľ��
Other:
    ���ô˽ӿ�ʱҪ��֤�˻���������eeprom�洢����
    ���ָ�����ļ�������̫�������������ƣ��򳬳��Ĳ��ֻᱻ����
*******************************************************************************/
HCSHANDLE CSJSLoadPropertyFileFromEeprom(const char *pcFileName, DWORD dwFlashAddress, DWORD dwSize);

/******************************************************************************
Function Name: CSJSClearPropertyFileEeprom

Description:
    �����ļ��������У��λ���Ա�ﵽ�ָ��������õ�Ч��
Input:
    pcFileName - ָ�����ļ���(ע:ֻ�����ļ���������·��)

Output:
    ��

Return Value:
    FALSE��ʾʧ�ܣ�TRUE��ʾ�ɹ�
Other:
    ���ô˽ӿ�ʱҪ��֤�˻���������eeprom�洢����
*******************************************************************************/
BOOL CSJSClearPropertyFileEeprom(const char *pcFileName);

/******************************************************************************
Function Name:    JSAddPropertyFile

Description:
	��property�ļ���ӵ��ṹ����ȥ
Input:
	pcFilePath:�ļ�ȫ·��
	nPropertyType:�ļ�����
Output:
None

Return Value:
	�ɹ���ӻ�����ӹ����ض�Ӧ��HCSHANDLE��ʧ�ܷ���0
Other:
*******************************************************************************/
HCSHANDLE JSAddPropertyFile(char *pcFilePath, int nPropertyType);

/**
�㶫 ���������仯֪ͨ
@param szKey - ��������.
@param szValue - �仯���ֵ
@param pUserData �û��Զ������(ע��ص�����ʱ����Ĳ���)
*/
typedef void (*JSGDOnEnvChanged_F)(const char* szKey,const char* szValue,void* pUserData);

/******************************************************************************
Function Name:    JSGDSetEnv
	
Description:
	���ù㶫��Ŀ������Ļ���������
Input:
    pcFileName ָ�����ļ���
	pcKey ���������ļ�ֵ
	pcValue  ����������ֵ 
Output:
    None

Return Value:
	
Other:
*******************************************************************************/
BOOL JSGDSetEnv(const char *pcFileName, const char *pcKey, const char *pcValue);


/******************************************************************************
Function Name:    JSGDGetEnv
	
Description:
	��ȡ�㶫��Ŀ������Ļ���������
Input:
    pcFileName ָ�����ļ���
	pcKey ���������ļ�ֵ
	pcValue  ����������ֵ 
	nLength  pcValue�ĳ���
Output:
    pcValue

Return Value:
	
Other:
*******************************************************************************/
BOOL JSGDGetEnv(const char *pcFileName, const char * pcKey, char *pcValue, int nLength);

/**
ע��㶫���������仯������
@param szKey - ��Ҫ�����Ļ���������(NULL��ʾ�������л��������仯)
@param fnCb - �����߻ص�����
@param pUserData �û��Զ������(�ص�֪ͨʱ�ش����ص�����)
*/
BOOL JSGDENVAddListner(const char* szKey,JSGDOnEnvChanged_F fnCb,void* pUserData);

/**
�Ƴ��㶫��������������
@param fnCb - �����߻ص�����
*/
void JSGDENVRemoveListner(JSGDOnEnvChanged_F fnCb);

/**
�Ƴ��㶫��������������
@param szKey - ��Ҫ�����Ļ���������(NULL��ʾ�������л��������仯)
@param fnCb - �����߻ص�����
*/
void JSGDENVRemoveKeyListner(const char* szKey, JSGDOnEnvChanged_F fnCb);


/******************************************************************************
Function Name:    JSDestoryAllEnv
	
Description:
	���ڴ���ɾ��ĳ�������ļ�(�����ݱ�)�������
Input:
	�ļ�����
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSDestoryAllEnv(const char* pcFileName);

/******************************************************************************
Function Name:    JSRemoveAllEnv
	
Description:
	�Ӵ洢�豸��ɾ�������ļ��������ڴ���ɾ��ĳ�������ļ�(�����ݱ�)�������
Input:
	�ļ�����
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������
Other:
*******************************************************************************/
int JSRemoveAllEnv(const char* pcFileName);

BOOL CSNGBSetEnv(const char *pcKey, const char *pcValue);

BOOL CSNGBGetEnv(const char *pcKey, char *pcValue, int nLength);


#ifdef __cplusplus
}
#endif

#endif  /* CS_JSENV_H */




