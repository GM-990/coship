#ifndef  CS_CERTSMANAGER_H
#define CS_CERTSMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif
/**
@brief ����֤�������ļ�·�������ļ���¼��ca֤��·������Ҫ˫����֤����վ��Ӧ����֤�鼰��˽Կ·����
@param[in] pcPath �����ļ���ȫ·��
@param[in] nLen �����ļ�·���ĳ���
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ�ܷ���CSUDI_FAILURE��

���������ļ���ʽ����:
        [cacerts]                                              //��ʾca֤������c:/certĿ¼�������õ���ca֤�鶼Ҫ�����ڴ�
        path=/cert                                          //Ŀ¼�£����ڴ�Ŀ¼�д�����Ŀ¼��
        
        [10.10.116.13]          		                 //��ʾҪ��˫����֤����վ10.10.116.13����֤��ȫ·��
        localcert=/cert/13/client.cer    	   //Ϊ"client.cer"�����Ӧ˽ԿΪ"client.key"��
        privatekey=/cert/13/client.key	   //��Ӹ���֤���¼�����������ͬʱ���
*/
int CSCERTMgrSetCfgPath(const char *pcPath, int nLen);

#ifdef __cplusplus
}
#endif
#endif

