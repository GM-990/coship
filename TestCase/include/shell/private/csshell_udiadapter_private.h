/**@defgroup PublicShellTF3CA PublicShellTF3CA ����ͬ��3 CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_UDIADAPTER_PRIVATE_H
#define CSSHELL_UDIADAPTER_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
��udi1.0�У�ʱ��ĽṹΪTTIME����udi2.0�У���ΪCSUDIPLUSOSTIME_S��
CSUDIPLUSOSTIME_S��TTIME�ǲ����ݵģ������г�Ա������λ�η��䲻һ�¡�
mmcp�м��Ϊudi1.0��udi2.0����ͬ�ķ�����caֻ����һ�׿⡣
Ϊ�˱�����ǰ���ݣ�ca��ʱ��ṹ��Ȼʹ��TTIME��ca�������ӿ�Ҳ���ֲ��䡣�м���Լ��Դ����䡣
*/
//#ifdef MMCP_UDI2
	typedef struct _TTIME
	{
		unsigned int m_dwHour_8 		:8; 	/**< Hours*/
		unsigned int m_dwMinute_8		:8; 	/**< Minutes*/
		unsigned int m_dwSecond_8		:8; /**< Seconds*/
		unsigned int m_dwMilSecond_8	:8; /**< ����,�˴����岻����1s=1000ms,�ð�λ��ʾ�ǲ�����*/
	}TTIME;
	
	typedef struct _TDATE
	{
		   unsigned int m_dwYear_13 		:13;   /**< Year*/
		   unsigned int m_dwMonth_8 		:8;    /**< Month*/
		   unsigned int m_dwDay_8			:8; 		   /**< Day*/
		   unsigned int m_dwWeek_3			:3;    /**< Week*/
	}TDATE;
//#endif



#ifdef __cplusplus
}
#endif
/** @} */
#endif


