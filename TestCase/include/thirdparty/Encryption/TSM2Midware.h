/*
TSM2Midware.h
定义了TSM提供给中间件调用的接口形式
*/

#ifndef _TSM2MIDWARE_H
#define _TSM2MIDWARE_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define SUMASECINFO_MODE "TSM"

int SumaSecure_NotifyExtraParam(const char* szExtraParam);//



int SumaSecure_GetTsmDeviceID(int type, char* szTsmDeviceID);  //



int SumaSecure_SetPDSEntry(const char* pszHost, unsigned short usPort);//



typedef struct _SBankEntry
{
unsigned long dwIP;	   // 入口IP，主机字节序
unsigned long dwPort;   // 入口端口，主机字节序
}SBankEntry;
int SumaSecure_NotifyBankEntry(int nEntryCount, const SBankEntry* pBankEntrys);//



int SumaSecure_DisplaySKB(const char *id, const char *pdata, int len,int x, int y, int type, int maxlen);//


int SumaSecure_GetSKBCipher(const char *id, char* buf,char* picid);//



int SumaSecure_Notify(int type);//



int SumaSecure_ChangePIN(const char* szOldPinID, const char* szNewPinID1, const char* szNewPinID2);//



int SumaSecure_SetPIN(int nFlag);//


int SumaSecure_IsExistPIN();//



int SumaSecure_IsCertExist();//



int SumaSecure_EncapCertApply(const char* szPinID1, const char* szPinID2, char* szReqBuf);//



int SumaSecure_EncapCertUpdate(const char *szOldPinID, const char* szNewPinID1,
															const char* szNewPinID2, char* szReqBuf);//
															

int SumaSecure_GetDisposedCert(unsigned short usCertType,char* szCert);//



int SumaSecure_NotifyResponse(const char *szResInfo);	//



int SumaSecure_EncapAuthReq(const char* szPinID, char* szReqBuf);//



int SumaSecure_GetToken(char* szToken);//



int  SumaSecure_Sign(const char* szPinID,unsigned char ucDigestAlgID,
									 	const char *szInData,int nInLen, char* szOutData);//


int SumaSecure_Verify(unsigned char ucDigestAlgID,unsigned short usCertType,
									  const char *szInData,int nInLen,
									  const char *szSignData,int nSignLen);//
									  
								  
int SumaSecure_GenRandom(char* szRandom, int nLen);//



int SumaSecure_EncAccountByBankPK(unsigned short usBankID, const char* szAccount,
																 char* szEncedData);//
																 
																 

int SumaSecure_GetVersion(char* pszVersion);//



int  SumaSecure_Verify_SSL_Sign(unsigned short usCertType,const unsigned char *szInData,
																int nInLen, unsigned char* szSignData, int nSignLen);//
																
															
int SumaSecure_Sign_SSL_Digest(const unsigned char *szInData,int nInLen,
															 unsigned char* szOutData,int *pnOutLen);//
															 

int SumaSecure_GenRSAKey(unsigned char ucType,unsigned char* szPublicKey,
												int *pnPukKeyLen,unsigned char* szPrivateKey,int *pnPriKeyLen);//
												
												
											
int SumaSecure_RsaEncrypt(const unsigned char *szPublicKey,int nKeyLen,
												const unsigned char *szInData,int nInLen,
												unsigned char* szOutData,int *pnOutLen);//
												

												
int SumaSecure_RsaDecrypt(const unsigned char *szPrivateKey,int nKeyLen,
												const unsigned char *szInData,int nInLen,
												unsigned char* szOutData,int *pnOutLen);//
												

											
int SumaSecure_GetRandom(unsigned char *szRandom, int nLen);//



int SumaSecure_RsaSign(unsigned char ucDigestAlgID,const unsigned char *szPrivateKey,
												int nKeyLen,const unsigned char *szInData,int nInLen,
												unsigned char *szOutData,int *pnOutLen);//
												


int SumaSecure_RsaVerify(unsigned char ucDigestAlgID,const unsigned char *szPublicKey,
												int nKeyLen,const unsigned char *szInData,int nInLen,
												const unsigned char *szSignData,int nSignLen);//
												


int SumaSecure_GetCert(unsigned short usCertType,unsigned char *pbyCert,int *pnLen);//



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _TSM2MIDWARE_H
