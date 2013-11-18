/**@defgroup RDI RDI Removable Device Interface�ṩ���ƶ��豸���ֵ�ͨ�ýӿ�
@brief RDIģ���Ӧͷ�ļ�<udi2_rdi.h>����Ҫ����ͳһ�Ŀ��ƶ��豸ͨ�÷��֡��Ƴ�������
�Լ�ͨ�ò����ӿں�������ݽṹ���豸Id��������ݣ��䱾�������κ���ʵ�ֵĽӿڡ�
Eastwin�м����ֲ����Ҫ��ע���ƶ��豸����Ŀ���������ע��ģ�顣

�������п��ƶ��豸�����Ƕ�����IdΪһ��32λ�޷����������и�16λ��ʾ�����豸�ţ�
һ���豸���ͣ���Ӧһ�����豸�š���16λ��ʾ����豸�ţ�һ��������豸��Ӧһ��
���豸�š�ͬһ���豸�������β壬��Ӧ�Ĵ��豸���ǲ�ͬ�ġ�\n
���п��ƶ��豸���뵽�ն�ʱ����Ҫ��USB�豸������Ӧ�豸��UDIģ���ͨ��ע��ı�ģ��
�ж����CSUDIRDICallback_F���͵Ļص�����֪ͨEastwin������Eastwin��û������������
�ɳ�ʼ��֮ǰ���Ϳ������豸���룬������ͬ���豸�����ܰ����ǿ��ƶ��豸��������Ӳ��
���豸��������ÿ�����ܰ������ƶ��豸��UDIģ�鶼�����һ����ȡ�����Ѵ����豸��Id��
�ӿڣ�����ζ�Ų��̶ֹ��豸������ǰ������豸��ʹ������֪ͨҲ�ǿ��Խ��ܵģ�ͬ����
��ע���֪ͨ�ص����������貹��֮ǰ�Ѳ����豸�������Ϣ��\n

- һ�������Ŀ��ƶ��豸�������Ϣ�׳�������Ӧ��Ϊ��Found->Ready������豸�а�
����ϵ����Ӧ���׳����豸��Ready��Ϣ�����׳����豸��Found��Ϣ����U��Ӧ����ɴ�
���豸����Ϣ�׳������׳��־��豸����Ϣ������豸��Ҫ�Լ죬��Found��Ready��Ϣ֮�䣬
�ɲ������ɸ�Checking��Ϣ������豸����ĳЩ�����޸��Ĺ��϶��޷�����������Ready��Ϣ
�ɱ�Error��Ϣ�����\n
- һ�������Ŀ��ƶ��豸���γ���Ӧ�׳���Ӧ�豸��Plugout��Ϣ�����豸�а�����ϵ��
��Ӧ���׳����豸��Plugout��Ϣ�����׳����豸��Plugout��Ϣ����U��Ӧ���׳��־��豸
�İγ���Ϣ�����׳��洢�豸�İγ���Ϣ��

@note ���еĿ��ƶ��豸�����԰���һ�׶�д���ƽӿڣ�IORead��IOWrite��IOCtrl������
���׽ӿڵ�ʵ�ֲ����Ǳ���ģ����������£����ǻ�ʹ�ÿ��ƶ��豸���豸Id��Ϊ������
ʹ�ö�Ӧ�̶��豸�Ľӿ���������Щ�豸��

@version 2.0.3 2009/09/15 ����IORead,IOWrite�ӿ�Ӧ���̷��ص�����
@version 2.0.2 2009/09/10 �޸�CSUDIRDIEvent_E�Ĳ��������֮ǰDeviceId�ظ�������
@version 2.0.1 2009/09/09 ���ioctrl,ioread,iowrite��ͨ���豸�����ӿڣ�����������ģ�鲻�ṩ�ù���
@version 2.0.0 2009/09/08 ����
@{
*/

#ifndef _UDI2_RDI_H_
#define _UDI2_RDI_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#ifdef __cplusplus
extern "C" {
#endif

/**@brief ���ƶ��豸���豸�����룬���п��ƶ��豸���豸�����ֵ����õ������豸�ţ����豸�ż������˸��豸�����ͣ����ƶ��豸�����豸�ž������豸����ģ�鶨�壬��ֵ��0x00010000��ʼ�������д���0x00010000���豸�Ŷ���ʾ���ƶ��豸��*/
#define CSUDI_RDI_MAJOR_DEVICEID_MASK		(0xFFFF0000)

/**@brief ���ƶ��豸���豸�����룬���п��ƶ��豸���豸�����ֵ����õ�����豸�ţ����豸�ż������˸������豸��һ�������豸�� �������͵Ŀ��ƶ��豸�Ĵ��豸�ž���0��ʼ�������豸ÿ�β���γ������豸�Ŷ��ᷢ���仯��*/
#define CSUDI_RDI_MINOR_DEVICEID_MASK		(0x0000FFFF)

/**@brief ͨ��DeviceId��ȡ��Ӧ�豸�Ƿ�Ϊ���ƶ��豸*/
#define CSUDI_ISREMOVEABLE(dwDeviceId)		((dwDeviceId) & CSUDI_RDI_MAJOR_DEVICEID_MASK)

enum
{
	CSUDI_RDI_STORAGE_MAJOR_DEVICEID		=  1 << 16,		///< �洢�豸���豸�ţ���udi2_fsģ����п���
	CSUDI_RDI_VOLUME_MAJOR_DEVICEID			=  2 << 16,		///< �־��豸���豸�ţ���udi2_fsģ����п���
	CSUDI_RDI_TUNER_MAJOR_DEVICEID			=  3 << 16		///< ��Ƶͷ�豸���豸�ţ���udi2_tunerģ����п���
};

typedef enum
{
    EM_UDIRDI_EVENT_UNKNOWN,       ///< δ֪�豸�¼�,�Ƿ�ֵ
    EM_UDIRDI_EVENT_FOUND,         ///< �����豸�ѿɻ���豸��Ϣ,���в��ɷ��ʷ���,pEventDataΪCSUDI_NULL
    EM_UDIRDI_EVENT_CHECKING,      ///< �������״̬ǰ��������豸�Լ졢��װ�Ⱥ�ʱ�ϳ���׼��������������Ϣ֪ͨӦ�����ĵȴ���pEventDataΪint���ͣ���ʾ�Լ���ȣ���ΧΪ0-100��-1��ʾ����δ֪�������������У�����޷�֪��״̬�����븳ֵΪ-1���Լ�����и���ϢӦԼÿ����1�ε�Ƶ�ʷ��������� 
    EM_UDIRDI_EVENT_READY,         ///< �豸����,�ѿɽ��з���,pEventDataΪCSUDI_NULL
    EM_UDIRDI_EVENT_ERROR,         ///< �豸���ɻָ�����,�豸���޷�����,pEventDataΪ�������(������������ģ�鶨�壬��δ�������������ʾ�ʹ����Ų飬�����ں������������)
    EM_UDIRDI_EVENT_PLUGOUT       ///< �豸�Ƴ�,pEventDataΪCSUDIRDIPlugOutType_E�����γ�����,����Ϣ֪ͨ���غ󽫲�����ͨ�����豸Id��ȡ�豸��Ϣ
}CSUDIRDIEvent_E;

/**@brief ����PLUGOUT�¼��Ķ�������

@note ������ද�������PLUGOUT�¼��������û������豸�Ƴ��ӿڰ�ȫ�Ƴ��豸���û�ֱ�Ӱγ��豸���Ƿ������û���������豸�����豸���ߵ���������ֵ��ϵͳ�Զ�ȥ�Ƴ����豸�ȵȡ�
*/
typedef enum
{
    EM_UDIRDI_PLUGOUT_UNKNOWN,       ///< δ֪plugout��������,�Ƿ�ֵ
    EM_UDIRDI_PLUGOUT_SAFE,          ///< �û������豸�Ƴ��ӿ�Remove��ȫ�Ƴ��豸�����İγ���Ϣ
    EM_UDIRDI_PLUGOUT_UNSAFE,        ///< �û�ֱ�ӷǷ��γ��豸�����İγ���Ϣ
    EM_UDIRDI_PLUGOUT_OVERFLOW      ///< �û������豸���ർ�³������ط�ֵ�����糬�����ص���ʱ�����һ��������豸��ϵͳ�Զ��Ƴ��������İγ���Ϣ
}CSUDIRDIPlugOutType_E;

/**
@brief �����豸֪ͨ����ԭ��

�����豸״̬�����仯(���롢�γ���)ʱ,��ֲ��ͨ�������͵Ļص�����֪ͨ�û���
@param[in] eType �¼�����,��ϸ�����CSUDIRDIEvent_E˵��.
@param[in] dwDeviceId���豸id�ţ��������˲�������Ϣ���豸�����������豸�š�
@param[in] pvEventData�����¼�����������,��ϸ�����CSUDIRDIEvent_E��ö��ֵ˵����
@param[in] pvUserData �û�����,����CSUDIRDIAddCallback_Fע��ûص�ʱ�����pvUserData��
@note pvEventData�ں������غ���ȫʧЧ(�������ڲ�ָ��ָ�������)����Ӧ�����ϲ����н��п�����
*/
typedef  void ( *CSUDIRDICallback_F )( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData);

/**
@brief ����豸֪ͨ�ص�����ԭ�Ρ�

���п��ƶ��豸����ģ�����Ҫʵ��һ������ʽ�ĺ�����������Ӹ������豸�ļ���������
 
@param[in] fnCallback���ص����������豸״̬�����仯ʱ���øú�����
@param[in] pvUserData���û����ݣ�����Ϊ�ص�������pvUserData�������ء�

@return �ɹ���ӷ���CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ��
@note ��ӻص�ǰ�������豸�仯��Ϣ�����Ჹ��֪ͨ����������ʱ��Щ�豸�ʹ��ڣ����ñ�����ʱ����õ�֪ͨ��
*/
typedef CSUDI_Error_Code ( *CSUDIRDIAddCallback_F )(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief ɾ���豸֪ͨ�ص�����ԭ��

���п��ƶ��豸����ģ�����Ҫʵ��һ������ʽ�ĺ���������ɾ���������豸�ļ���������
 
@param[in] fnCallback���ص�������֮ǰע��ĺ�����
@param[in] pvUserData���û����ݣ�������ע��ʱ��һ�¡�
@return �ɹ�ɾ������CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ��
@note fnCallback��pvUserDataһ��Ψһȷ��һ���ص�������
*/
typedef CSUDI_Error_Code ( *CSUDIRDIRemoveCallback_F )(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief ɾ���洢�豸ԭ�Ρ�

���п��ƶ��豸����ģ�����ʵ��һ������ʽ�ĺ����������Ƴ��������豸��ʵ����
@param[in] dwDeviceId��Ŀ���豸Id��
@return �ɹ��򷵻�CSUDI_SUCCESS;ʧ�ܷ��ش������ֵ
@note 
- ֻ��ɾ�����ƶ��豸���͵��豸����ͼɾ�������豸���ͽ����ش���
- ɾ���豸�󲻿���ͨ�����豸Id��ȡ�豸��Ϣ��
- �û����øýӿڻᴥ��EM_UDIRDI_PLUGOUT_SAFE���͵�EM_UDIRDI_EVENT_PLUGOUT��Ϣ��
*/
typedef CSUDI_Error_Code ( *CSUDIRDIRemoveDevice_F )(CSUDI_UINT32 dwDeviceId);

/**
@brief �ӿ��ƶ��豸��ȡ����ԭ��

���ƶ��豸����ģ���ʵ��һ������ʽ�ĺ��������ڴ�ָ�������豸ʵ���ж�ȡ���ݣ�
����ӿ��ǿ�ѡ�ģ��������Ҫ���豸�ж�ȡ���ݣ��ɲ�ʵ�ָýӿڡ�
 
@param[in] dwDeviceId��Ŀ���豸Id��
@param[in] pvData��ָ��������ݵ�Buffer��
@param[in] nBuffLength��pvData�ĳ��ȡ�
@param[out] pnReadLength��ʵ�ʶ��������ݳ��ȡ�
@return �ɹ��򷵻�CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ��
@note
- �ӿ�Ӧ���κ�����¾����̷��أ������ܶ����κ����ݣ���*pnReadLength==0�����Է��سɹ�
- ֻ�ܶ�ȡ�����Ϳ��ƶ��豸���͵��豸����ͼ��ȡ�����豸���ͽ����ش���
*/
typedef CSUDI_Error_Code ( *CSUDIRDIIORead_F)(CSUDI_UINT32 dwDeviceId,void* pvData,int nBuffLength,int* pnReadLength);

/**
@brief �ӿ��ƶ��豸д������ԭ��

���ƶ��豸����ģ���ʵ��һ������ʽ�ĺ�����������ָ�������豸ʵ����д�����ݣ�
����ӿ��ǿ�ѡ�ģ��������Ҫ���豸��д�����ݣ��ɲ�ʵ�ָýӿڡ�
@param[in]  dwDeviceId Ŀ���豸Id��
@param[in] pvData ָ��д�����ݵ�Buffer��
@param[in]  nBuffLength pvData�ĳ��ȡ�
@param[out]  pnWriteLength��ʵ��д������ݳ��ȡ�
@return �ɹ��򷵻�CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ��
@note
- �ӿ�Ӧ���κ�����¾����̷��أ������ݲ���һ��д�꣬��*pnWriteLength<nBuffLength������*pnWriteLength=0�����Է��سɹ�
- ֻ���������Ϳ��ƶ��豸���͵��豸д�����ݣ���ͼд�������豸���ͽ����ش���
*/
typedef CSUDI_Error_Code ( *CSUDIRDIIOWrite_F)(CSUDI_UINT32 dwDeviceId,void* pvData,int nBuffLength,int* pnWriteLength);

/**
@brief ���ƶ��豸���ƽӿ�ԭ�Ρ�

���ƶ��豸����ģ���ʵ��һ������ʽ�ĺ��������ڿ����豸��Ϣ������ӿ��ǿ�ѡ�ģ��������Ҫ�����豸���ƻ�ͨ������ר�ýӿڿ����豸���ɲ�ʵ�ָýӿڡ� 
@param[in] dwDeviceId Ŀ���豸Id��
@param[in] cmd ������������ɿ��ƶ��豸ģ�����ж��塣
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note
- �ӿ�Ӧ���κ�����¾����̷��أ������ܶ����κ����ݣ���*pnReadLength==0�����Է��سɹ�
- ֻ�ܿ��Ʊ����Ϳ��ƶ��豸���͵��豸����ͼ���������豸���ͽ����ش���
*/
typedef CSUDI_Error_Code ( *CSUDIRDIIOCtrl_F)(CSUDI_UINT32 dwDeviceId,int cmd,...);


#ifdef __cplusplus
}
#endif

/** @} */

#endif   
