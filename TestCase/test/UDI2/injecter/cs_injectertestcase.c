/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "udi2_demux.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_fs.h"
#include "udi2_os.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "cs_injectertest.h"



CSUDI_BOOL CSTC_INJECTER_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_INJECTER_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

