#if !defined(_CSTTS_H)
#define _CSTTS_H

#ifdef __cplusplus
extern "C" 
{
#endif

#define DBG_THIRDPART_MSC "thirdparty.msc"

int CSCMMTTSGetSpeakers(char *text, int textLen);
int CSCMMTTSSetParam(char *option, char *value);
int CSCMMTTSGetParam(const char* option, char* value, int valueLen);
int CSCMMTTSRestoreDefaultParam();
int CSCMMTTSPlay(const char *text, int textLen);
int CSCMMTTSPause();
int CSCMMTTSResume();
int CSCMMTTSStop();

#ifdef __cplusplus
}
#endif

#endif
