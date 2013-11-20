DIR_LOCAL	= udi_test

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

LIBS  = 

#-----------------------------------------------------------------------
# Directory where the 3rdparty includes are stored
#-----------------------------------------------------------------------
DIR_INCLUDE +=
DIR_INCLUDE += $(_TMROOT)/sd/apps/coship/comps/udidrv/inc
DIR_INCLUDE += $(_TMROOT)/sd/apps/coship/comps/udilib/inc

#-----------------------------------------------------------------------
# local CFLAGS
#-----------------------------------------------------------------------
LOCAL_DLLFLAGS = --sysroot=$(_TMSYSROOT) -ludidrv

#-----------------------------------------------------------------------
# local CPPFLAGS
#-----------------------------------------------------------------------
LOCAL_CXXFLAGS	=

#***********************************************************************
# Do not change this
#***********************************************************************
all: configuration target

#***********************************************************************
# Do not change the following include
#***********************************************************************
ifneq ($(DIR_CONFIG),_)
include $(DIR_SDE)/$(DIR_CONFIG)/maketarget.mk
endif

