/*************************************************************************
* CoSpellright (c) 2004,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
* 
* File Name��CS_InputMethod.h  ��T9 release��  
* File Identify��reference to Configuration Manager
* Summary�� A chinese character typing methord 
* 
* Current Version��1.0
* Author(s)��
* 
/---------------------------------------------------------------------------------------------------------/
revision             author            reason             date
1.0                 jl.Huang          Original       September 8,2004
/--------------------------------------------------------------------------------------------------------/
************************************************************************** */

#ifndef _CS_IMET9_H_
#define _CS_IMET9_H_

#ifdef __cplusplus
extern "C" {
#endif

/*----------------- MICRO for controlling complier (start)----------- *
 * if you want to compile this file on PC platform, please define the *
 * CSSTRSAFE_PLATFORM 0, otherwise(under LSI,ST) define				  *
 * CSSTRSAFE_PLATFORM 1 or 2.											  *
 *----------------- MICRO for controlling complier (start)------------*/
#define CSIMET9_PLATFORM 1    //select PC Platform

//
#if (CSIMET9_PLATFORM == 0)
#define CSIME_EXPORT __declspec(dllexport)
typedef unsigned int  size_t;
//
#elif (CSIMET9_PLATFORM == 1)
#define CSIME_EXPORT
//
#elif(CSIMET9_PLATFORM == 2)
#define CSIME_EXPORT
#endif

#ifdef __cplusplus
#define _CSIME_EXTERN_C    extern "C"
#else
#define _CSIME_EXTERN_C    extern
#endif

#define CSIMET9API  _CSIME_EXTERN_C CSIME_EXPORT

/*==========================================================
//   Spell ��ƴ���ļ��   MB Ϊģ��ļ��    CharacterΪ���ֵļ��   //
//   Symbol  ���ŵļ��                                        // 
===========================================================*/          
//ƴ��ģ��һ����GB������������������������(�ֱ�Ϊ1��2) 
//ѡ��ģ�岢��ú���ģ��ĵ�ַ   
//�ɹ��򷵻�1��ʧ�ܷ���0
//��ע������ӳ��ģ����������չ��ģ��
//      nFlag��ѡ����ֵ��Ϊ��չ��־
CSIMET9API int CreateIMET9(int nFlag);
CSIMET9API int DeleteIMET9();


//�������ܣ��������ֲ����ض�Ӧ��ƴ���ĺ�ѡ���׵�ַ��ƴ������
//���룺��������    ���ظ�����ָ��  
//������ɹ����к�ѡ���������  ʧ�ܣ�û�У�����0   
//      ����ƴ����ѡ��ĸ������׵�ַ
//��ע�����صľ���Ǻ�ѡ���е�һ��ƴ�����׵�ַ
CSIMET9API int GetSpellCount(int nInputDigit,int* pnSpellCount);


//�������ܣ����ݻ�õ�ƴ����ѡ��õ���Ӧ��ƴ���ַ�
//���룺��ѡ��ľ��    ƴ����ַ����(ע����ݺ�ѡ������޶�)   
//      ����ѡ��ƴ����ָ��       buffer��С
//������ɹ�����1   ���򷵻�0      ��������ѡ��ƴ��
CSIMET9API int GetSpell(int nSpellHandle,int nIndex,char* pcSpellBuffer,int nSize);


//�������ܣ����ݻ�õ�ƴ���ַ��õ���Ӧ�ĺ��ֺ�ѡ��
//���룺ѡ����ƴ���ľ��     ָ���ֺ�ѡ��ĸ�����ָ��
//������ɹ�����ֵ   ���ֺ�ѡ��ľ��    ���غ�ѡ�����
CSIMET9API int GetCharacterCount(char* pcSpellBuffer,int* pnCharacterCount);


//�������ܣ����ݻ�õĺ��ֵĺ�ѡ��õ�Ҫ�ҵĺ���
//���룺���ֺ�ѡ��ľ��   ��������(ע����ݺ��ַ��صĺ�ѡ����������޶�)
//      ѡ���ĺ��ֵ�ָ��             buffer��С	
//������ɹ��򷵻�1   ���򷵻�0
CSIMET9API int GetCharacter(int nCharacterHandle,int nIndex,char* pcCharacterBuffer,int nSize);


//�������ܣ��������ֲ����ض�Ӧ�ķ��ŵĺ�ѡ���׵�ַ
//���룺��������(0,1)    ָ����ŵĸ�����ָ��    ��־λ��1Ϊ���ţ�0Ϊ��ĸ��
//������ɹ����к�ѡ�����ֵ  ʧ�ܣ�û�У�����0   
//      ���ط��ź�ѡ��ĸ������׵�ַ
//��ע�����ص��׵�ַ�Ǻ�ѡ���е�һ�����ŵ��׵�ַ
//      �����������ѡȡ��ģ������ѡ����ŵı�������
CSIMET9API int GetSymbolCount(int nInputDigit,int* pnSymbolCount,int nFlag);



//�������ܣ����ݻ�õķ��ź�ѡ��õ���Ӧ�ķ���
//���룺���ź�ѡ����׵�ַ    ���ŵ�ַ����(ע����ݺ�ѡ������޶�)   
//         ����ѡ�����ŵ�BUFFER    buffer��С
//������ɹ�����1   ���򷵻�0  ͬʱ����ѡ��ƴ�����׵�ַ
CSIMET9API int GetSymbol(int nSymbolHandle,int nIndex,char* pcSymbolBuffer,int nSize);

#ifdef __cplusplus
}

#endif

#endif
