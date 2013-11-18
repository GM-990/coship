  /******************************************************************************
 FileName:  cs_nes.h
Copyright (c) 2009�� Shen Zhen Coship Electronics CO.,LTD.��
 
 Version:       Ver 1.0
 Author:        yinliming(yinliming@coship.com)
 
 File Description:
         Support NES (Nintendo Entertainment System) Interface
 
 Modify History:
 Version     	Date        		Author          	Description
 --------    	----------  	--------        ------------
 1.0         		2009-10-28 	yinliming         	Create
 *******************************************************************************/
 #ifndef CSNES_H
#define CSNES_H
 
#ifdef __cplusplus
 extern "C" {
#endif

#define CSNES "CSNES"

/*
@brief ����NESģ���ʼ������
*/
typedef struct _NESInitParam
 {
     int nClientId;                  ///<GrOpen���ص�ClientID
     int nParentWinId;          ///<nes ���ڵĸ�����ID, С�ڵ���0ʱ����Ϊ�Ǹ�Window
     int nX;
     int nY; 
     int nWidth;
     int nHeight;                     ///<nes���ڵ�λ�úʹ�С,λ��������丸����
 
     int nEvtFromFlag;     ///<nesϵͳ�����¼��ķ�ʽ
                                      ///<0 -nes auto get event form microwin
                                     ///<1 caller post event to nes by CSNESHandEvent
 }CSNESInitParams_S;

typedef enum
{
	EM_NES_UNKNOW	///
}CSNesOption_E;

/**
@brief ģ���ʼ��

@param[in] ��ʼ�����������CSNESInitParams_S�Ķ���
@return �ɹ�����NES�Ĵ���ID,ʧ�ܷ���-1
@note ����NES�ǵ���
*/
int CSNESInit(CSNESInitParams_S *pNesInitParams);

/**
@brief ����������Ϸ

@param[in] pNesFileName ������Ϸ��·��
@return -1��ʾʧ��
@note ��
*/
int CSNESLoadGame(const char*pNesFileName); 

/**
@brief ͨ���˽ӿڽ�������Ϣ���ݸ�NES

@param [in] unKeyType: �������ͣ�
					EM_UDIINPUT_KEY_PRESS		��������	
					EM_UDIINPUT_KEY_RELEASE	�����ͷ�
@param [in] unKeyCode  ��Ӧ��ֵ,����udi�ļ�ֵ
					CSUDI_VK_UP				��Ӧ��Ϸ�ֱ���������� 
					CSUDI_VK_RIGHT			��Ӧ��Ϸ�ֱ������ϼ���
					CSUDI_VK_DOWN			��Ӧ��Ϸ�ֱ������Ҽ���
					CSUDI_VK_LEFT			��Ӧ��Ϸ�ֱ������¼���
					
					CSUDI_VK_6				��Ӧ��Ϸ�ֱ��Ұ벿��������İ���
					CSUDI_VK_8				��Ӧ��Ϸ�ֱ��Ұ벿�����ұߵİ���
					CSUDI_VK_4				��Ӧ��Ϸ�ֱ��Ұ벿��������İ���
					CSUDI_VK_2				��Ӧ��Ϸ�ֱ��Ұ벿��������İ���
					
					???(��Ҫȷ��)				��Ӧ��Ϸ�ֱ���L1����
					???(��Ҫȷ��)				��Ӧ��Ϸ�ֱ���R1����
					???(��Ҫȷ��)				��Ӧ��Ϸ�ֱ���L2����
					???(��Ҫȷ��)				��Ӧ��Ϸ�ֱ���L2����

					???(��Ҫȷ��)				��Ӧ��Ϸ�ֱ���SELECT����
					???(��Ҫȷ��)				��Ӧ��Ϸ�ֱ���START����
					
@param [in] pReserve  �������������봫NULL	
@return  �ɹ�����0�����򷵻�����ֵ
@note �����Ҫ֧�ֶ����Ϸ�ֱ�����˽ӿڿ�����Ҫ�޸���Ӳ���
*/
int CSNESHandEvent(unsigned int unKeyType, unsigned int unKeyCode,void * pReserve);

/**
@brief ����ѡ��

@param[in] eOption ���CSNesOption_E����
@param[in] wParam key-value���е�key
@param[in] lParam 	key-value���е�value
@return ��
@note��
*/
void CSNESSetOption(CSNesOption_E eOption, void *wParam, void *lParam);

/**
@brief �˳���Ϸ

@param[in] ��
@return ��
@note �˽ӿ���ͬ���ģ�ֻ����Ϸ�����˳��Ժ�ŷ���
*/
void CSNESExitGame();

#ifdef __cplusplus
 }
#endif
#endif

