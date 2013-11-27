DIR_LOCAL	= comps/udiplus

#***********************************************************************
# Do not change the following include
#***********************************************************************
include $(_TMROOT)/sde/environment.mk

#-----------------------------------------------------------------------
# Source environment variables
#-----------------------------------------------------------------------
CXX_SOURCES 	=

C_SOURCES = \
      $(wildcard src/*.c)
OUTPUT_PREFIX ?= $(_TMTGTBUILDROOT)/comps/generic_apps/usr
#-----------------------------------------------------------------------
# Required components
#-----------------------------------------------------------------------
REQUIRES = tmplatHandle  tmplatDma tmplatVsmDma tmplatIic tmplatEeprom tmplatGpio tmplatPanelButtons  \
	   tmplatPanelLeds  tmplatIrrx tmplatIrblaster tmplatSpi tmplatSerial tmplatModem tmplatIrblaster \
	   lnxnotifyqUsr lnxnotifyqDrv lnxcbdb lnxplatDrv lnxplatIO \
	   tmpvrPipeline tmpvrPipeTuner tmfssDemod  tmpvrPipeDemux tmcssDemux \
	   tmcssCbuf  tmpvrPipeAudio tmpvrAudioPres tmpvrPipeVideo tmvssDDVideo  tmpvrVideoPres  tmpvrRecord  tmpvrDataManager tmpvrPlayback\
	   tmvssDDImage tmvssDDGraphicsHelper tmasDDAudio   tmvssDDVPP tmvssDDGraphics tmvssDDTVEnc tmvssDDRFMod tmvssDDVBI tmcssSmartcard tmavcontrol \
	   tmscsScd tmvssDDGraphics vpmfbDrv tmplatIsa

LIBS  = 

#-----------------------------------------------------------------------
# Directory where the 3rdparty includes are stored
#-----------------------------------------------------------------------
DIR_INCLUDE = $(_TMROOT)/sd/apps/coship/comps/udidrv/inc
DIR_INCLUDE += $(_TMROOT)/sd/apps/coship/comps/udilib/inc

#-----------------------------------------------------------------------
# local CFLAGS
#-----------------------------------------------------------------------
LOCAL_CFLAGS = -fPIC
EXPORTS=yes
LOCAL_DLLFLAGS = --sysroot=$(_TMSYSROOT)

#-----------------------------------------------------------------------
# local CPPFLAGS
#-----------------------------------------------------------------------
LOCAL_CXXFLAGS	=

install_me:
	$(INSTALL) -d $(OUTPUT_PREFIX)
	$(INSTALL) -d $(OUTPUT_PREFIX)/lib
	cp -Pr $(MODULE_INSTALL_LOC)/lib/libudiplus.so $(OUTPUT_PREFIX)/lib/
	
#***********************************************************************
# Do not change this
#***********************************************************************
all: configuration lib install_me

#***********************************************************************
# Do not change the following include
#***********************************************************************
ifneq ($(DIR_CONFIG),_)
include $(DIR_SDE)/$(DIR_CONFIG)/makelib.mk
endif

