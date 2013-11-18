#ifndef MMCP_DEBUG_H
#define MMCP_DEBUG_H
#ifdef __cplusplus
extern "C" {
#endif

/**
UDI1.0与2.0使用相同的接口定义，因此这里只需要根据使用环境的不同，
引用不同的头文件，无需进行任何修改。
*/
#ifdef MMCP_UDI2
#include "udiplus_debug.h"
#else
#include "cs_assert.h"
#endif

//DTV模块
#define DBG_DTV_WM                     ("dtv.wm")
#define DBG_DTV_DM                     ("dtv.dm")
#define DBG_DTV_ADS                    ("dtv.ads")
#define DBG_DTV_CRC                    ("dtv.crc")
#define DBG_DTV_EPG                    ("dtv.epg")
#define DBG_DTV_EPGSORT                ("dtv.epgsort")
#define DBG_DTV_MEM                    ("dtv.mem")
#define DBG_DTV_NVOD                   ("dtv.nvod")
#define DBG_DTV_PUSHFILE               ("dtv.pushfile")
#define DBG_DTV_PVOD                   ("dtv.pvod")
#define DBG_DTV_SEARCH                 ("dtv.search")
#define DBG_DTV_SECTION                ("dtv.section")
#define DBG_DTV_SI                     ("dtv.si")
#define DBG_DTV_SICACHE                ("dtv.sicache")
#define DBG_DTV_SOURCE                 ("dtv.source")
#define DBG_DTV_SOURCETUNER            ("dtv.sourcetuner")
#define DBG_DTV_SUBTITLE               ("dtv.subtitle")
#define DBG_DTV_TELETEXT               ("dtv.teletext")
#define DBG_DTV_SM                     ("dtv.sm")
#define DBG_DTV_CAINJECT               ("dtv.cainject")
#define DBG_DTV_QUICK_SEARCH           ("dtv.qs")

//codec模块
#define DBG_CODEC_MP3                  ("codec.mp3")

//jsext模块
#define DBG_JSEXT                      ("jsext")
#define DBG_JSPRINT                    ("jsprint")
#define DBG_JSEXT_VOIP                 ("jsext.voip")
#define DBG_JSEXT_MSG                  ("jsext.msg")
#define DBG_JSEXT_2D                   ("jsext.2d")
#define DBG_JSEXT_3D                   ("jsext.3d")
#define DBG_JSEXT_MPC                  ("jsext.mpc")
#define DBG_JSEXT_SR                   ("jsext.sr")
#define DBG_JSEXT_TJBANK               ("jsext.tjbank")
#define DBG_JSEXT_WLAN                 ("jsext.wlan")

//java模块
#define DBG_JAVA					   ("Java")

//kernel模块
#define DBG_KERNEL_FS                  ("kernel.cfs")
#define DBG_KERNEL_FSHTTP              ("kernel.cfshttp")
#define DBG_KERNEL_UTIL                ("kernel.util")
#define DBG_KERNEL_UDIPROXY            ("kernel.udiproxy")
#define DBG_KERNEL_MEM                 ("kernel.mem")
#define DBG_KERNEL_MEM_DEBUG           ("kernel.mem.debug")
#define DBG_KERNEL_MEM_LEAK            ("kernel.mem.leak")
#define DBG_KERNEL_EVENT               ("kernel.event")
#define DBG_KERNEL_OC                  ("kernel.oc")
#define DBG_KERNEL_OCSECTION           ("kernel.ocsection")
#define DBG_KERNEL_OCCONTROL           ("kernel.occtrl")
#define DBG_KERNEL_JSON                ("kernel.json")
#define DBG_KERNEL_DOWNLOAD            ("kernel.download")
#define DBG_KERNEL_RMI                 ("kernel.rmi")
#define DBG_KERNEL_CMD                 ("kernel.cmdserver")
#define DBG_KERNEL_SCREENSHOT          ("kernel.screenshot")
#define DBG_KERNEL_CONDANALYZER	   	   ("kernel.condanalyzer")

//c应用管理器模块
#define DBG_AMSC_APP                   ("amsc.app")
#define DBG_AMSC_DB                    ("amsc.db")
#define DBG_AMSC_CTRL                  ("amsc.ctrl")
#define DBG_AMSC_DEPLOY                ("amsc.deploy")
#define DBG_AMSC_ASSOCIATE             ("amsc.associate")
#define DBG_AMSC_DOWNLOAD              ("amsc.download")

//mediaplayer模块
#define DBG_MP_FFMPEG                  ("mp.ffmpeg")
#define DBG_MP                         ("mp")
#define DBG_MP_SA                      ("mp.sa")
#define DBG_MP_AVCTRL                  ("mp.avctrl")
#define DBG_MP_TIME                    ("mp.time")
#define DBG_MP_PUMP                    ("mp.pump")
#define DBG_MP_TSMONITOR               ("mp.tsmonitor")

//guitool模块
#define DBG_GT_WEBKIT                  ("gt.webkit")
#define DBG_GT_PREFORMANCE             ("gt.performance")
#define DBG_GT_OCCONTROL               ("gt.occontrol")

//graph模块
#define DBG_GRAPH_MW                   ("graph.mw")
#define DBG_GRAPH_MWIMG                ("graph.mwimg")
#define DBG_GRAPH_MWKEY                ("graph.mwkey")
#define DBG_GRAPH_EFFECT               ("graph.effect")
#define DBG_GRAPH_ARPFONT              ("graph.afont")
#define DBG_GRAPH_ETRUMPFONT           ("graph.efont")
#define DBG_GRAPH_STFFONT              ("graph.stffont")
#define DBG_GRAPH_FONTADPT             ("graph.fontadapter")
#define DBG_GRAPH_FT                   ("graph.ft")
#define DBG_GRAPH_GBIME                ("graph.gbime")
#define DBG_GRAPH_IMEMGR               ("graph.imemgr")
#define DBG_GRAPH_FJPG                 ("graph.fjpg")
#define DBG_GRAPH_IMGSAVE              ("graph.imgsave")
#define DBG_GRAPH_OSG                  ("graph.osg")
#define DBG_GRAPH_IFRAME               ("graph.iframe")
#define DBG_GRAPH_CURSOR               ("graph.cursor")
#define DBG_GRAPH_COPYRIGHT            ("graph.copyright")
#define DBG_GRAPH_ICON                 ("graph.icon")


//protocol模块
#define DBG_PRTCL_DHCP                 ("protocol.dhcp")
#define DGB_PRTCL_TCPSETUP             ("protocol.tcpsetup")
#define DBG_PRTCL_HTTP                 ("protocol.http")
#define DBG_PRTCL_NTP                  ("protocol.ntp")
#define DBG_PRTCL_STACK                ("protocol.stack")
#define DBG_PRTCL_PING                 ("protocol.ping")
#define DBG_PRTCL_TFTP                 ("protocol.tftp")
#define DBG_PRTCL_TR069                ("protocol.tr069")
#define DBG_PRTCL_SOAP                 ("protocol.soap")
#define DBG_PRTCL_RTSP                 ("protocol.rtsp")
#define DBG_PRTCL_RTP                  ("protocol.rtp")
#define DBG_PRTCL_SDP                  ("protocol.sdp")
#define DBG_PRTCL_DNS                  ("protocol.dns")
#define DBG_PRTCL_DLNA                 ("protocol.dlna")
#define DBG_PRTCL_XMPP                 ("protocol.xmpp")
#define DBG_PRTCL_AIRPLAY             ("protocol.airplay")
#define DBG_PRTCL_EASYBUS              ("protocol.easybus")

//shell模块
#define DBG_SHELL						("shell")
#define DBG_SHELL_EW200                 ("shell.ew200")
#define DGB_SHELL_SAITION             	("shell.saition")
#define DBG_SHELL_APP                	("shell.appmanager")
#define DBG_SHELL_CA                  	("shell.ca")
#define DBG_SHELL_CONFIG                ("shell.config")
#define DBG_SHELL_DM                	("shell.dm")
#define DBG_SHELL_OTA                   ("shell.ota")
#define DBG_SHELL_IDONGGAME      		("shell.idonggame")
#define DBG_SHELL_IRCARD                ("shell.ircard")
#define DBG_SHELL_SAU                	("shell.sau")
#define DBG_SHELL_TR069                 ("shell.tr069")
#define DBG_SHELL_TTS                 	("shell.tts")
#define DBG_SHELL_EITLOADER             ("shell.eitloader")
#define DBG_SHELL_SEARCH                ("shell.search")
#define DBG_SHELL_ETH0                  ("shell.eth0")
#define DBG_SHELL_PANEL                 ("shell.panel")
#define DBG_SHELL_SYSTOOL               ("shell.systool")
#define DBG_SHELL_CMDSRVAVPLAYER		("shell.cmdsrvavplayer")
#define DBG_SHELL_CMDSRVDEVMNG			("shell.cmdsrvdevmng")
#define DBG_SHELL_BOOTSHOW              ("shell.bootshow")
#define DBG_SHELL_NGODSA                ("shell.ngodsa")

//midp模块
#define DBG_MIDP                        ("midp")
#define DGB_JMIDP                       ("midp.java")

//jvm模块
#define DBG_CVM                         ("cvm")
#define DBG_CVM_AWT                     ("cvm.awt")
#define DBG_CVM_BLUEZ                   ("cvm.bluez")

//qtmicrowin模块
#define DBG_QTMW                        ("qtmw")

//qtwebkitplugins模块
#define DBG_QTPLUGIN_MIDP               ("qtplugin.midp")
#define DBG_QT_WEBKIT                   ("qt.webkit")
#define DBG_QT_INPUTMETHOD              ("qt.inputmethod")
#define DBG_QT_CUTEEXT                  ("qt.cuteext")


//dtvmx native模块
#define DBG_DTVMX_MEDIA                 ("dtvmx.media")
#define DBG_DTVMX_COMMOM                ("dtvmx.common")
#define DBG_DTVMX_ADS                   ("dtvmx.ads")
#define DBG_DTVMX_APPMANAGER            ("dtvmx.appmanager")
#define DBG_DTVMX_CA                    ("dtvmx.ca")
#define DBG_DTVMX_DTV                   ("dtvmx.dtv")
#define DBG_DTVMX_GRAPHICS              ("dtvmx.graphics")
#define DBG_DTVMX_NETWORK               ("dtvmx.network")
#define DBG_DTVMX_KERNEL                ("dtvmx.kernel")
#define DBG_DTVMX_AVMSC                 ("dtvmx.avmsc")
#define DBG_DTVMX_DAVIC                 ("dtvmx.davic")
#define DBG_DTVMX_DVB                   ("dtvmx.dvb")
#define DBG_DTVMX_UART					("dtvmx.uart")

//cfg 模块
#define DBG_CFG_CITV                    ("cfg.citv")
#define DBG_CFG_NETMANAGER              ("cfg.netmanager")
#define DBG_CFG_NVOD                    ("cfg.nvod")
#define DBG_CFG_OTA                     ("cfg.ota")
#define DBG_CFG_TUNE                    ("cfg.TuneMainFreq")

#ifdef __cplusplus
}
#endif

#endif  /* MMCP_DEBUG_H */

