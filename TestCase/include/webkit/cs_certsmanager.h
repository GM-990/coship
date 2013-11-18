#ifndef  CS_CERTSMANAGER_H
#define CS_CERTSMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif
/**
@brief 设置证书配置文件路径，该文件记录了ca证书路径、需要双向认证的网站对应个人证书及其私钥路径，
@param[in] pcPath 配置文件的全路径
@param[in] nLen 配置文件路径的长度
@return 
- 成功返回CSUDI_SUCCESS。
- 失败返回CSUDI_FAILURE。

具体配置文件格式如下:
        [cacerts]                                              //表示ca证书存放在c:/cert目录。所有用到的ca证书都要保存在此
        path=/cert                                          //目录下，可在此目录中创建子目录。
        
        [10.10.116.13]          		                 //表示要求双向认证的网站10.10.116.13个人证书全路径
        localcert=/cert/13/client.cer    	   //为"client.cer"，其对应私钥为"client.key"，
        privatekey=/cert/13/client.key	   //添加个人证书记录是这三项必须同时添加
*/
int CSCERTMgrSetCfgPath(const char *pcPath, int nLen);

#ifdef __cplusplus
}
#endif
#endif

