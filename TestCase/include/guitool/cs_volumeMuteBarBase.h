#ifndef CS_VolumeMuteBarBase_H
#define CS_VolumeMuteBarBase_H

#include "mmcp_typedef.h"

typedef enum
{
	EM_UNDEFINE,
	EM_USER_MUTE,
	EM_USER_VOLUME
}eVolMuteType;

typedef void* (*registerVolMuteCallBack_F)(eVolMuteType type, void *pUserData);

typedef struct _stRegVolMute 
{
	eVolMuteType m_eType;
	registerVolMuteCallBack_F m_fnVolMuteCallBack;
}stRegVolMute;

class VolumeMuteBarBase{
public:
	virtual ~VolumeMuteBarBase() {};
	virtual void setVol(int vol) {};
	virtual void setVolInit(int initVol, int minVol, int maxVol) {};
public:
	 int getWinId() const;
	 int getClientId() const;
	 int getGcId() const;
	 bool isHDWin() const;
	 
};

#endif

