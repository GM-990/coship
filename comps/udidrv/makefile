DIR_LOCAL	= udidrv

#***********************************************************************
# Do not change the following include
#***********************************************************************
include $(_TMROOT)/sde/environment.mk

#-----------------------------------------------------------------------
# Source environment variables
#-----------------------------------------------------------------------
CXX_SOURCES 	=

C_SOURCES = \
      $(wildcard src/*.c) \
#-----------------------------------------------------------------------
# Required components
#-----------------------------------------------------------------------
REQUIRES = tmplatHandle  tmplatDma tmplatVsmDma tmplatIic tmplatEeprom tmplatGpio tmplatPanelButtons  \
	   tmplatPanelLeds  tmplatIrrx tmplatIrblaster tmplatSpi tmplatSerial tmplatModem tmplatIrblaster \
	   lnxnotifyqUsr lnxnotifyqDrv lnxcbdb lnxplatDrv lnxplatIO \
	   tmpvrPipeline tmpvrPipeTuner tmfssDemod  tmpvrPipeDemux tmcssDemux \
	   tmcssCbuf  tmpvrPipeAudio tmpvrAudioPres tmpvrPipeVideo tmvssDDVideo  tmpvrVideoPres  tmpvrRecord  tmpvrDataManager tmpvrPlayback\
	   tmvssDDImage tmvssDDGraphicsHelper tmasDDAudio tmasDDPCM tmvssDDVPP tmvssDDGraphics tmvssDDTVEnc tmvssDDRFMod tmvssDDVBI tmcssSmartcard tmavcontrol \
	   tmscsScd tmvssDDGraphics vpmfbDrv tmplatIsa

LIBS = lnxUKAL tmplatVersion lnxplatUsr lnxpvrUsr lnxcssUsr lnxtmvssUsr lnxnotifyqUsr lnxscsUsr tmpvrDataManager tmpvrIndexStorage 

TARGET = udidrv.bin

#-----------------------------------------------------------------------
# Directory where the 3rdparty includes are stored
#-----------------------------------------------------------------------
DIR_INCLUDE = $(_TMROOT)/sd/apps/coship/comps/udidrv/inc
DIR_INCLUDE += $(_TMROOT)/sd/apps/coship/comps/udilib/inc

UDIDRVLIBS_DIR=$(_TMROOT)/sd/apps/coship/comps/udilib

#-----------------------------------------------------------------------
# local CFLAGS
#-----------------------------------------------------------------------
#include $(_TMROOT)/comps/coship/make/coship_path_inc.mak
#include $(_TMROOT)/comps/coship/make/COption.mak
#LOCAL_CFLAGS  = $(CPP_INCLUDE_PATH)
#LOCAL_CFLAGS += $(CSFLAGS)
#ifeq ($(CSBoard),YiGouYu)
#LOCAL_CFLAGS += -DUSE_OTHER_SCHED
#endif
#LOCAL_CFLAGS = -fPIC
#EXPORTS=yes
LOCAL_CFLAGS  += -mcpu=cortex-a9 -mfpu=vfpv3-d16 -mfloat-abi=softfp
LOCAL_CFLAGS  += -Wall -Wcomment -mno-sched-prolog -fno-common -fno-builtin -fdollars-in-identifiers -fno-strict-aliasing
LOCAL_CFLAGS  += -O0 -g

LOCAL_DLLFLAGS = --sysroot=$(_TMSYSROOT)
LOCAL_LDFLAGS   += -L$(UDIDRVLIBS_DIR)/lib  -lmaintest -lostest -lplayertest -ltestkittest -ludiplus -ldemuxtest



#-----------------------------------------------------------------------
# local CPPFLAGS
#-----------------------------------------------------------------------
LOCAL_CXXFLAGS	=

#***********************************************************************
# Do not change this
#***********************************************************************
all: configuration target install

#***********************************************************************
# Do not change the following include
#***********************************************************************
ifneq ($(DIR_CONFIG),_)
#include $(DIR_SDE)/$(DIR_CONFIG)/makelib.mk
include $(DIR_SDE)/$(DIR_CONFIG)/maketarget.mk
endif

