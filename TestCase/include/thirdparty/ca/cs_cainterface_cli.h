#ifndef _CS_CAINTERFACE_CLI_H_
#define _CS_CAINTERFACE_CLI_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

typedef void (*pfCLIprint)(const CHAR * format,...);
typedef INT (*pfLibprint)(DWORD ,INT, pfCLIprint);

#define CA_CLI          0xF0 //  CLI ��Ϣ���ͣ���Ҫ��CA_ALARM����Ϣ���

typedef enum
{
	CA_CLI_SRV=0,	//multi service
	CA_CLI_CAT,		//cat,
	CA_CLI_PMT,		//pmt
	CA_CLI_NIT,		//nit,
	CA_CLI_BAT,		//bat
	
	CA_CLI_DMX,		//filter
	CA_CLI_MSG,		//the lastest msg	
	CA_CLI_CARD,	//card status
	CA_CLI_CW,		//original cw
	CA_CLI_ECM,     //ecm
	
	CA_CLI_EMM,     //emm
	CA_CLI_MENU,	//irdeto 
	
	CA_CLI_TM   ,   //��ȡĳ����Ϣ�ĵ�ǰʱ�䣬��������Ƶ������Ϣ֮�����ڲ�������ʱ��
	CA_CLI_MAX
}T_CaCli;

/**************************************************************************
*Function Name: CS_CACLIVIEW_SRV
*   �鿴CA�ڲ��Ķ�·��Ϣ
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_SRV(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_CAT
*   �鿴CA�ڲ���CAT��
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_CAT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_PMT
*   �鿴CA�ڲ���PMT��
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_PMT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_NIT
*   �鿴CA�ڲ���NIT��
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_NIT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_BAT
*   �鿴CA�ڲ���BAT��
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_BAT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_DMX
*   �鿴CA�ڲ���ȫ��filter����
*
*Parameters
*		   nSrv:  �����������룬����Ҫ�������������0
*          nParm: �����������룬����Ҫ�������������0
*          Fprint:��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_DMX(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_DMX
*   �鿴CA�׳��������Ϣ�����ڹ��ϲ鿴
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_MSG(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_CARD
*   �鿴CA�ڲ�������Ϣ
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_CARD(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_CW
*   �鿴CA���趨�����֣��Ǹ߼���ȫ���ֽڴ�������֡��߼���ȫ�����CW1
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_CW(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_ECM
*   �鿴CA��ECM��Ϣ
*
*Parameters
*		   dwHandle:  ������PID/���������Ǿ�����Ͱѵ�ǰ����ECM����ӡ����
*          nParm:     1:����PID�ľʹ�ӡ����  0:�رմ�ӡ
*          Fprint:��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_ECM(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_EMM
*   �鿴CA��EMM��Ϣ
*
*Parameters
*		   dwHandle:  ������PID/���������Ǿ�����Ͱѵ�ǰ����EMM����ӡ����
*          nParm:     1:����PID�ľʹ�ӡ����  0:�رմ�ӡ
*          Fprint:��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_EMM(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_MENU
*   ���CA�����е�MENU
*
*Parameters
*		   dwHandle:��Ŀ���/����0x10000������Ϊ��Index
*          nParm:   �����������룬����Ҫ�������������0
*          Fprint:  ��ӡ����
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_MENU(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

#ifdef  __cplusplus
}
#endif

#endif

