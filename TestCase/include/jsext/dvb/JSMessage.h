/********************************************************************************
* JSMessage.h									
* Copyright ? Shenzhen Coship Electronics Co.#,LTD.  2008.11.26			
* All rights reserved.								
*										
* Author: y_junjie,heansong,zdm,yyj,afei,zsm 					
* Date:  2008.11.26								
* Compiler:									
*										
* Description:	js porting							
* Notes:									
* Update:    2008.11.26  y_junjie create
  Modify     2008.12.1   y_junjie ,keep one propoty ,and add the order interface	 						
             2008.12.17  y_junjie,add the nvod module,message notify,and modify the
			          video postion(x,y,w,h) to RECT struct),add http server
			          address,add DHCP notify
             2008.12.22  y_junjie,remove DHCP notify,change to NETWORK 
             2008.12.27  y_junjie,add some comment to startsever ,change the message set/get method,use the propoty;
			             add the JSShowInfo() for test,and the user also can define there private message
	      2008.1.14 heansong, add OTA message
*********************************************************************************/

#ifndef __JSMESSAGE_H_
#define __JSMESSAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************JSCallback type 属性定义开始********************************************************/
#define  JSCALL_MSG                                     40000

//搜索过程中向界面发送的消息
#define  JSCALL_START_SCAN                              ( JSCALL_MSG + 1  ) //开始搜索  jsext负责抛出
#define  JSCALL_START_DELIVER                           ( JSCALL_MSG + 2  ) //开始搜索某个频点  jsext负责抛出
#define  JSCALL_DELIVER_FINISH                          ( JSCALL_MSG + 3  ) //某个频点搜索结束  jsext负责抛出
#define  JSCALL_START_NIT                               ( JSCALL_MSG + 4  ) //开始搜索NIT表  jsext负责抛出
#define  JSCALL_NIT_FINISH                              ( JSCALL_MSG + 5  ) //NIT表搜索成功  jsext负责抛出
#define  JSCALL_NIT_FAILED                              ( JSCALL_MSG + 6  ) //NIT表搜索失败  jsext负责抛出
#define  JSCALL_POP_SERVICE                             ( JSCALL_MSG + 7  ) //搜索到Service  jsext负责抛出
#define  JSCALL_ALL_FINISHED                            ( JSCALL_MSG + 8  ) //搜索全部结束  jsext负责抛出
#define  JSCALL_SEARCH_EXIT                             ( JSCALL_MSG + 9  ) //搜索任务退出  jsext负责抛出

//EPG接收/更新过程中向界面发送的消息
#define  JSCALL_PRESENT_EPG_CHANGED                     ( JSCALL_MSG + 11 ) //当前Event需要刷新  jsext负责抛出
#define  JSCALL_FOLLOW_EPG_CHANGED                      ( JSCALL_MSG + 12 ) //下一个Event需要刷新  jsext负责抛出
#define  JSCALL_SCHEDULE_EPG_FINISHED                   ( JSCALL_MSG + 13 ) //详细EPG可能需要刷新  jsext负责抛出
#define  JSCALL_SCHEDULE_EPG_ALL_FINISHED               ( JSCALL_MSG + 14 ) //所有的详细EPG搜索成功  jsext负责抛出

//播放过程事件消息
#define  JSCALL_CHANNEL_LOCKED                          ( JSCALL_MSG + 21 ) //频道加锁/解锁  jsext负责抛出
#define  JSCALL_PG_LOCKED                               ( JSCALL_MSG + 22 ) //频道父母锁/解锁  jsext负责抛出
#define  JSCALL_TUNE_LOST                               ( JSCALL_MSG + 23 ) //播放信号丢失  jsext负责抛出
#define  JSCALL_TUNE_LOCKED                             ( JSCALL_MSG + 24 ) //播放信号锁定  jsext负责抛出
#define  JSCALL_PLAY_SUCCESS                            ( JSCALL_MSG + 25 ) //播放成功  jsext负责抛出
#define  JSCALL_SI_UPDATE                               ( JSCALL_MSG + 26 ) //SI更新  CFG负责抛出
#define  JSCALL_NO_CHANNEL                              ( JSCALL_MSG + 27 ) //无频道  jsext负责抛出
#define  JSCALL_PLAY_STOP                               ( JSCALL_MSG + 28 ) //前端节目停播  jsext负责抛出
#define  JSCALL_PLAY_START                              ( JSCALL_MSG + 29 ) //前端节目恢复播放  jsext负责抛出
#define  JSCALL_CHANNEL_UNLOCKED                        ( JSCALL_MSG + 30 ) //频道加锁/解锁  jsext负责抛出
#define  JSCALL_PG_UNLOCKED                             ( JSCALL_MSG + 31 ) //频道父母锁/解锁  jsext负责抛出
#define  JSCALL_SI_TIME_UPDATE                          ( JSCALL_MSG + 32 ) //SI同步时间  jsext负责抛出
#define  JSCALL_DATA_CONNECT_FAIL                       ( JSCALL_MSG + 42 ) // 连接数据源失败  jsext负责抛出
#define  JSCALL_DATA_CONNECT_SUCCESS                    ( JSCALL_MSG + 43 ) // 连接数据源成功  jsext负责抛出
#define  JSCALL_MEDIA_NOT_SUPPORT                       ( JSCALL_MSG + 44 ) // 媒体类型不支持  jsext负责抛出
#define  JSCALL_MEDIA_STOP_SUCCESS                      ( JSCALL_MSG + 45 ) // 媒体播放停止成功  jsext负责抛出


//NVOD搜索/播放过程消息

//{NVOD模块支持消息
#define  JSCALL_NVOD_EVENT_PLAY_OVER                    ( JSCALL_MSG + 33 ) //NVOD时移事件播放结束  jsext负责抛出
#define	 JSCALL_NVOD_SEARCH_PROGRESS                    ( JSCALL_MSG + 34 ) //NVOD搜索进度		参数1-搜索到的ReferenceEvent个数,参数2-搜索进度值  jsext负责抛出
#define	 JSCALL_NVOD_GROUP_LIST_CHANGED                 ( JSCALL_MSG + 35 ) //NVOD分组列表个数改变	  jsext负责抛出
#define	 JSCALL_NVOD_REFEVT_LIST_CHANGED                ( JSCALL_MSG + 36 ) //NVOD参考事件列表个数改变	  jsext负责抛出
#define	 JSCALL_NVOD_SHTEVT_LIST_CHANGED                ( JSCALL_MSG + 37 ) //NVOD时移事件列表个数改变  jsext负责抛出
#define	 JSCALL_NVOD_SHTEVT_TIME_CHANGED                ( JSCALL_MSG + 38 ) //NVOD时移事件倒计时时间改变  jsext负责抛出
#define  JSCALL_NVOD_ALL_COMPLETE                       ( JSCALL_MSG + 39 ) //NVOD搜索结束  jsext负责抛出
#define  JSCALL_NVOD_REFEVT_COMPLETE                    ( JSCALL_MSG + 40 ) //NVOD参考事件搜索结束  jsext负责抛出
#define  JSCALL_NVOD_SHTEVT_COMPLETE                    ( JSCALL_MSG + 41 ) //NVOD时移事件搜索结束  jsext负责抛出
//}新SVS-NVOD模块支持消息

//预定和定时器相关消息
#define  JSCALL_SCHEDULE_PREV                           ( JSCALL_MSG + 51 ) //预定提前提醒  jsext负责抛出
#define  JSCALL_SCHEDULE_ONTIME                         ( JSCALL_MSG + 52 ) //预定节目到期  jsext负责抛出
#define  JSCALL_SCHEDULE_POST                           ( JSCALL_MSG + 53 ) //预定节目播放完毕  jsext负责抛出
#define  JSCALL_SCHEDULE_OVERTIME                       ( JSCALL_MSG + 54 ) //预定节目已经过期  jsext负责抛出
#define  JSCALL_SCHEDULE_CHANGED                        ( JSCALL_MSG + 55 ) //预定节目数据改变  jsext负责抛出

#define  JSCALL_TIMESWITCH_ON                           ( JSCALL_MSG + 61 ) //定时开机  暂没用到
#define  JSCALL_TIMESWITCH_OFF                          ( JSCALL_MSG + 62 ) //定时待机  暂没用到
#define  JSCALL_TIMESWITCH_CHANGED                      ( JSCALL_MSG + 63 ) //定时开关机数据改变  暂没用到
//表监视相关消息,起始值不妥
#define  JSCALL_PMT_COMPLETE                            ( JSCALL_MSG + 64 ) //PMT搜索完成  暂没用到

//智能卡相关消息
#define  JSCALL_SMART_IN                                ( JSCALL_MSG + 70 ) //智能卡已插入  CFG负责抛出
#define  JSCALL_SMART_OUT                               ( JSCALL_MSG + 71 ) //智能卡已拔出  CFG负责抛出

//CA相关消息
#define  JSCALL_CA_INFO                                 ( JSCALL_MSG + 80 ) //CA 的提示信息,为CA指定  CFG负责抛出
#define  JSCALL_CA_ALARM                                ( JSCALL_MSG + 81 ) //CA 的警告信息,为CA指定  CFG负责抛出
#define  JSCALL_CA_COMMAND                              ( JSCALL_MSG + 82 ) //CA 的命令信息,为CA指定  CFG负责抛出
#define  JSCALL_CA_STATUS                               ( JSCALL_MSG + 83 ) //CA 的命令信息,为CA指定  CFG负责抛出

//NETWORK 消息
#define  JSCALL_GET_IP_ADDR                             ( JSCALL_MSG + 90 ) //need update the ipaddr  CFG负责抛出   ??
#define  JSCALL_ETH_CONNECT                             ( JSCALL_MSG + 91 ) //network connect   CFG负责抛出 ??
#define  JSCALL_ETH_DISCONNECT                          ( JSCALL_MSG + 92 ) //network diconnect  CFG负责抛出
#define  JSCALL_CM_ON_LINE                              ( JSCALL_MSG + 93 ) //CM online  CFG负责抛出
#define  JSCALL_CM_OFF_LINE                             ( JSCALL_MSG + 94 ) //CM offline  CFG负责抛出
#define  JSCALL_WIFI_DOWN                               ( JSCALL_MSG + 95 ) //无线网卡拔出 
#define  JSCALL_WIFI_UP                                 ( JSCALL_MSG + 96 ) //无线网卡插上

//OTA消息
#define  JSCALL_OTA_FORCE_UPGRADE                       ( JSCALL_MSG + 110 ) //OTA强制升级信息  CFG负责抛出
#define  JSCALL_OTA_MANUAL_UPGRADE                      ( JSCALL_MSG + 111 ) //OTA手动升级提示  CFG负责抛出
#define  JSCALL_OTA_GET_ANALYSIS_DATA                   ( JSCALL_MSG + 112 ) //OTA 获取并分析升级数据  CFG负责抛出
#define  JSCALL_OTA_GET_NIT_SUCCESS                     ( JSCALL_MSG + 113 ) //OTA 获取并分析NIT成功  CFG负责抛出
#define  JSCALL_OTA_GET_NIT_FAILED                      ( JSCALL_MSG + 114 ) //OTA 获取并分析NIT失败  CFG负责抛出
#define  JSCALL_OTA_GET_PAT_SUCCESS                     ( JSCALL_MSG + 115 ) //OTA 获取并分析PAT成功  CFG负责抛出
#define  JSCALL_OTA_GET_PAT_FAILED                      ( JSCALL_MSG + 116 ) //OTA 获取并分析PAT失败  CFG负责抛出
#define  JSCALL_OTA_GET_PMT_SUCCESS                     ( JSCALL_MSG + 117 ) //OTA 获取并分析PMT成功  CFG负责抛出
#define  JSCALL_OTA_GET_PMT_FAILED                      ( JSCALL_MSG + 118 ) //OTA 获取并分析PMT失败  CFG负责抛出
#define  JSCALL_OTA_WRITE_PARAM_FLASH_START             ( JSCALL_MSG + 119 ) //OTA 烧录升级参数到flash  CFG负责抛出
#define  JSCALL_OTA_WRITE_PARAM_FLASH_SUCESS            ( JSCALL_MSG + 120 ) //OTA 烧录升级参数到flash成功  CFG负责抛出
#define  JSCALL_OTA_WRITE_PARAM_FLASH_FAILED            ( JSCALL_MSG + 121 ) //OTA 烧录升级参数到flash失败  CFG负责抛出
#define  JSCALL_OTA_REBOOT                              ( JSCALL_MSG + 122 ) //OTA 升级参数获取成功，重启升级  CFG负责抛出
#define  JSCALL_OTA_NO_UPGRADE_INFO                     ( JSCALL_MSG + 123 ) //未检测到OTA升级信息  CFG负责抛出

//节目自动更新消息
#define  JSCALL_NIT_AUTO_UPDATE                         ( JSCALL_MSG + 150 ) //NIT表自动更新  CFG负责抛出
#define  JSCALL_BAT_AUTO_UPDATE                         ( JSCALL_MSG + 151 ) //BAT表自动更新  CFG负责抛出
#define  JSCALL_PMT_AUTO_UPDATE                         ( JSCALL_MSG + 152 ) //PMT表自动更新  CFG负责抛出

//广告消息
#define  JSCALL_AD_CURRENT_CONTENT_CHANGE               ( JSCALL_MSG + 160 ) //当前节目的某广告位内容变化  jsext负责抛出
#define  JSCALL_AD_FIND_STB_CTRL_INFO                   ( JSCALL_MSG + 161 ) //发现与用户STB匹配的控制信息  jsext负责抛出

//Mosaic业务相关消息
#define  JSCALL_MOSAIC_COLLECT_MOSAIC_PAGE              (JSCALL_MSG + 170)//  jsext负责抛出
#define  JSCALL_MOSAIC_COLLECT_ERROR                    (JSCALL_MSG + 171)//  jsext负责抛出
#define  JSCALL_MOSAIC_COLLECT_COMPLETE                 (JSCALL_MSG + 172)//  jsext负责抛出
#define  JSCALL_MOSAIC_COLLECT_UPDATE                   (JSCALL_MSG + 173)//  jsext负责抛出


//定时器相关消息
#define  JSCALL_TIMER_ONTIME                            (JSCALL_MSG + 180)//  jsext负责抛出

//媒体播放过程中消息
#define  JSCALL_MEDIA_END                               (JSCALL_MSG + 200)//  jsext负责抛出
#define  JSCALL_MEDIA_BEGINING                          (JSCALL_MSG + 201)//  jsext负责抛出
#define  JSCALL_MEDIA_ERROR                             (JSCALL_MSG + 202)//  jsext负责抛出
#define  JSCALL_MEDIA_PLAYMODE_CHANGE                   (JSCALL_MSG + 203)//  jsext负责抛出
#define  JSCALL_MEDIA_FRONTTS_STOP                      (JSCALL_MSG + 204) //播放过程中，前端码流停播  jsext负责抛出
#define  JSCALL_MEDIA_FRONTTS_RUN                       (JSCALL_MSG + 205) //播放过程中，前端码流在播  jsext负责抛出
#define  JSCALL_MEDIA_PREV_ITEM_STEPPED                 (JSCALL_MSG + 206) //快退到上一段节目  jsext负责抛出
#define  JSCALL_MEDIA_NEXT_ITEM_STEPPED                 (JSCALL_MSG + 207) //快进到下一段节目  jsext负责抛出
#define  JSCALL_MEDIA_NEW_FRAME                         (JSCALL_MSG + 208) //新帧到来  jsext负责抛出
#define  JSCALL_MEDIA_SHOW_SUBTITLE                     (JSCALL_MSG + 209) //字幕数据  jsext负责抛出
#define  JSCALL_MEDIA_CLEAR_SUBTITLE                    (JSCALL_MSG + 210) //字幕消失  jsext负责抛出

//设备发现消息
#define  JSCALL_STORAGE_PLUGIN                          ( JSCALL_MSG + 220 ) //发现新设备插入  jsext负责抛出
#define  JSCALL_STORAGE_PLUGOUT                         ( JSCALL_MSG + 221 ) //设备被拔出  jsext负责抛出
#define  JSCALL_PARTITION_FOUND                         ( JSCALL_MSG + 226 ) //  jsext负责抛出
#define  JSCALL_PARTITION_MOUNTED                       ( JSCALL_MSG + 227 ) //  jsext负责抛出
#define  JSCALL_PARTITION_MOUNT_FAILED                  ( JSCALL_MSG + 228 ) //  jsext负责抛出
#define  JSCALL_PARTITION_UNINSTALL                     ( JSCALL_MSG + 229 ) //  jsext负责抛出

//Teletext消息
#define  JSCALL_TELETEXT_START_DISPLAY                  ( JSCALL_MSG + 250 ) //页面开始显示 ??
#define  JSCALL_TELETEXT_PAGE_UPDATE                    ( JSCALL_MSG + 251 ) //页面更新
#define  JSCALL_TELETEXT_LINK_FLASH                     ( JSCALL_MSG + 252 ) //Link页闪烁
#define  JSCALL_TELETEXT_PAGE_FLASH                     ( JSCALL_MSG + 253 ) //一般页数据闪烁
#define  JSCALL_TELETEXT_CAPTION_PAGE                   ( JSCALL_MSG + 254 ) //显示Close Caption页

//PVR消息
#define  JSCALL_PVRFILE_DEL_PROGRESS                    ( JSCALL_MSG + 255 ) //文件进度通知删除  jsext负责抛出
#define  JSCALL_PVRFILE_UPDATE_FILELIST                 ( JSCALL_MSG + 256 ) //文件列表更新通知  jsext负责抛出
#define  JSCALL_PVRFILE_HDD_FULL                        ( JSCALL_MSG + 257 ) //存储设备已满通知  jsext负责抛出
#define  JSCALL_PVRFILE_PLUGIN_STORAGE                  ( JSCALL_MSG + 258 )  //识别新存储设备通知  jsext负责抛出
#define  JSCALL_PVRFILE_PLUGOUT_STORAGE                 ( JSCALL_MSG + 259 ) //存储设备被拔除通知  jsext负责抛出
#define  JSCALL_PVRRSV_BEFORE_START                     ( JSCALL_MSG + 260 ) //预订录制即将启动通知  jsext负责抛出
#define  JSCALL_PVRRSV_START_SUCCES                     ( JSCALL_MSG + 261 ) //预订录制已成功启动通知  jsext负责抛出
#define  JSCALL_PVRRSV_START_FAILED                     ( JSCALL_MSG + 262 ) //预订录制启动失败通知  jsext负责抛出
#define  JSCALL_PVRRSV_BEFORE_STOP                      ( JSCALL_MSG + 263 ) //预订录制即将结束通知  jsext负责抛出
#define  JSCALL_PVRRSV_OUTOF_DATE                       ( JSCALL_MSG + 264 ) //预订录制项过期被删除的通知  jsext负责抛出
#define  JSCALL_PVRPLAY_TSHIFT_SWITCH                   ( JSCALL_MSG + 265 ) //直播与TShift播放切换通知  jsext负责抛出
#define  JSCALL_PVRPLAY_SPEED_CHANGE                    ( JSCALL_MSG + 266 ) //播放速度变化通知  jsext负责抛出
#define  JSCALL_PVRPLAY_END_OF_FILE                     ( JSCALL_MSG + 267 ) //播放到文件末尾通知  jsext负责抛出
#define  JSCALL_PVRPLAY_BEGIN_OF_FILE                   ( JSCALL_MSG + 268 ) //播放到文件开头通知  jsext负责抛出
#define  JSCALL_PVRREC_NOTIFY_STOP                      ( JSCALL_MSG + 269 ) //录制停止通知  jsext负责抛出

#define  JSCALL_PANEL_CHANNEL                           ( JSCALL_MSG + 280 )   //前面板节目显示 ??

//USB升级下载消息
#define  JSCALL_USB_UPGRADE_RS232_START                 ( JSCALL_MSG + 281 ) //开始升级  jsext负责抛出
#define  JSCALL_USB_DOWNLOAD_RS232_START                ( JSCALL_MSG + 282 ) //开始下载  jsext负责抛出
#define  JSCALL_USB_RS232_HEAD_MANU_ERROR               ( JSCALL_MSG + 283 ) //加头信息中的厂商描述不匹配  jsext负责抛出
#define  JSCALL_USB_RS232_HEAD_MACHINE_ERROR            ( JSCALL_MSG + 284 ) //加头信息中的机型描述不匹配  jsext负责抛出
#define  JSCALL_USB_RS232_HEAD_VER_ERROR                ( JSCALL_MSG + 285 )  //加头信息中的软件版本不匹配  jsext负责抛出
#define  JSCALL_USB_RS232_HEAD_CRC_ERROR                ( JSCALL_MSG + 286 ) //加头信息CRC校验失败  jsext负责抛出
#define  JSCALL_USB_RS232_FILE_CRC_ERROR                ( JSCALL_MSG + 287 ) //rs232文件CRC校验失败  jsext负责抛出
#define  JSCALL_USB_RS232_HEAD_OK                       ( JSCALL_MSG + 288 ) //加头信息正确  jsext负责抛出
#define  JSCALL_USB_READ_RS232_ERROR                    ( JSCALL_MSG + 289 ) //读rs232文件失败  jsext负责抛出
#define  JSCALL_USB_WRITE_RS232_ERROR                   ( JSCALL_MSG + 290 ) //写rs232文件失败  jsext负责抛出
#define  JSCALL_USB_READ_FLASH_ERROR                    ( JSCALL_MSG + 291 ) //读flash失败  jsext负责抛出
#define  JSCALL_USB_WRITE_FLASH_ERROR                   ( JSCALL_MSG + 292 ) //写flash失败  jsext负责抛出
#define  JSCALL_USB_TRANS_DATA_64K                      ( JSCALL_MSG + 293 ) //从USB中读取64K数据到内存  jsext负责抛出
#define  JSCALL_USB_WRITE_DATA_64K                      ( JSCALL_MSG + 294 ) //把64k数据从内存写入flash  jsext负责抛出
#define  JSCALL_USB_UPGRADE_RS232_END                   ( JSCALL_MSG + 295) //升级成功  jsext负责抛出
#define  JSCALL_USB_DOWNLOAD_RS232_END                  ( JSCALL_MSG + 296 ) //下载成功  jsext负责抛出
#define  JSCALL_USB_MEM_ALLOC_FAILED                    ( JSCALL_MSG + 297 ) //内存分配失败  jsext负责抛出
#define  JSCALL_USB_UNKNOW_ERROR                        ( JSCALL_MSG + 298 ) //未知错误  jsext负责抛出

//网络升级消息
#define  JSCALL_IP_DOWNLOADING                          ( JSCALL_MSG + 301 )//启动升级，正在下载数据  jsext负责抛出
#define  JSCALL_IP_DL_FAILED                            ( JSCALL_MSG + 302 )//下载失败  jsext负责抛出
#define  JSCALL_IP_DL_CONNECT_FAILED                    ( JSCALL_MSG + 303 )//网络信号丢失  jsext负责抛出
#define  JSCALL_IP_DL_CONNECT_RECOVER                   ( JSCALL_MSG + 304 )//网络连接恢复  jsext负责抛出
#define  JSCALL_IP_NETWORK_LOST                         ( JSCALL_MSG + 305 )//网络连接失败  jsext负责抛出
#define  JSCALL_IP_DL_OVERTIME                          ( JSCALL_MSG + 306 )//请求网络数据超时  jsext负责抛出
#define  JSCALL_IP_WRITING                              ( JSCALL_MSG + 307 )//正在写falsh  jsext负责抛出
#define  JSCALL_IP_WRITE_FAILED                         ( JSCALL_MSG + 308 )//写flash失败	  jsext负责抛出
#define  JSCALL_IP_VERIFYING                            ( JSCALL_MSG + 309 )//正在校验数据  jsext负责抛出
#define  JSCALL_IP_RS232_HEAD_MANU_ERROR                ( JSCALL_MSG + 310 )//加头信息中的manufacture  description 错误  jsext负责抛出
#define  JSCALL_IP_RS232_HEAD_MACHINE_ERROR             ( JSCALL_MSG + 311 )//加头信息中的machine description 错误  jsext负责抛出
#define  JSCALL_IP_RS232_HEAD_CRC_ERROR                 ( JSCALL_MSG + 312 )//加头信息CRC校验失败  jsext负责抛出
#define  JSCALL_IP_RS232_FILE_CRC_ERROR                 ( JSCALL_MSG + 313 )//升级文件CRC校验失败  jsext负责抛出
#define  JSCALL_IP_RS232_HEAD_OK                        ( JSCALL_MSG + 314 )//加头信息正确  jsext负责抛出
#define  JSCALL_IP_UPGRADE_SUCCESS                      ( JSCALL_MSG + 315 )//升级成功  jsext负责抛出
#define  JSCALL_IP_MEMORY_FULL                          ( JSCALL_MSG + 316 )//内存分配失败  jsext负责抛出
#define  JSCALL_IP_UNKNOWN_ERROR                        ( JSCALL_MSG + 317 )//未知错误  jsext负责抛出

// tts消息
#define JSCALL_TTS_END_OF_PLAY                          (JSCALL_MSG + 320)	// tts文本播音结束消息 ??

//蓝牙消息
#define JSCALL_BT_SCAN_END                              (JSCALL_MSG + 330)	//扫描蓝牙设备完成
#define JSCALL_BT_PAIR_END                              (JSCALL_MSG + 331)	//蓝牙设备配对完成
#define JSCALL_BT_CONNECT_END                           (JSCALL_MSG + 332)	//蓝牙设备连接完成
#define JSCALL_BT_FOUND_DEVICE                          (JSCALL_MSG + 333)	//发现蓝牙设备
#define JSCALL_BT_CONNECT_LOST                          (JSCALL_MSG + 334)	//发现蓝牙设备连接断开


//多page相关消息
#define  JSCALL_PAGE_HIDE                               (JSCALL_MSG + 340)//当前page即将被隐藏  jsext负责抛出
#define  JSCALL_PAGE_SHOW                               (JSCALL_MSG + 341)//当前page即将被显示  jsext负责抛出

//语音识别
#define JSCALL_SR_REC_START                             (JSCALL_MSG + 350)	// 开始录音
#define JSCALL_SR_SPEECH_END                            (JSCALL_MSG + 351)	// 检测到语音结束
#define JSCALL_SR_ERROR                                 (JSCALL_MSG + 352)	// 出错
#define JSCALL_SR_RESULT                                (JSCALL_MSG + 353)	// 识别结果
#define JSCALL_SR_VOLUME                                (JSCALL_MSG + 354)	// 读取音量

//media 播放相关消息
#define  JSCALL_MEDIA_STATE_LOADING                     (JSCALL_MSG + 400)//缓存开始  jsext负责抛出
#define  JSCALL_MEDIA_STATE_LOADED                      (JSCALL_MSG + 401)//缓存结束  jsext负责抛出
#define  JSCALL_MEDIA_STATE_HAVEMETADATA                (JSCALL_MSG + 401)//能够获取一些基本信息，如片长  jsext负责抛出
#define  JSCALL_MEDIA_STATE_ENOUGHDATA_FORPLAY          (JSCALL_MSG + 403)//有足够的数据进行播放，如进行快进快退都操作  jsext负责抛出
#define  JSCALL_MEDIA_STATE_DURATIONCHANGE              (JSCALL_MSG + 404)//节目总时长发生变化，需要播放器更新时长  jsext负责抛出
#define  JSCALL_MEDIA_STATE_RATECHANGE                  (JSCALL_MSG + 405)//节目总播放倍数发生变化 jsext负责抛出
#define  JSCALL_MEDIA_STATE_LOADING_PROGRESS            (JSCALL_MSG + 406)//等待播放，缓存数据的进度值  jsext负责抛出
#define  JSCALL_MEDIA_STATE_STOP                        (JSCALL_MSG + 407)//停止播放器操作成功  jsext负责抛出
#define  JSCALL_MEDIA_STATE_START                       (JSCALL_MSG + 408)//启动播放器操作成功  jsext负责抛出
#define  JSCALL_MEDIA_STATE_PAUSE                       (JSCALL_MSG + 409)//暂停播放器操作成功  jsext负责抛出
#define  JSCALL_MEDIA_SEEK_FINISH                       (JSCALL_MSG + 410)//选时播放定位成功  jsext负责抛出

//voip消息
#define  JSCALL_VOIP_LOGINOK                            ( JSCALL_MSG + 500 )//登陆成功  CFG负责抛出
#define  JSCALL_VOIP_LOGINFAILED                        ( JSCALL_MSG + 501 )//登陆失败  CFG负责抛出
#define  JSCALL_VOIP_LCLLOGOUT                          ( JSCALL_MSG + 502 )//本地注销成功  CFG负责抛出
#define  JSCALL_VOIP_SRVLOGOUT                          ( JSCALL_MSG + 503 )//服务器注销成功  CFG负责抛出
#define  JSCALL_VOIP_SESSCALLIN                         ( JSCALL_MSG + 504 )//收到远端电话  CFG负责抛出
#define  JSCALL_VOIP_SESSCALLOUT                        ( JSCALL_MSG + 505 )//电话已拨出  CFG负责抛出
#define  JSCALL_VOIP_SESSALERTED                        ( JSCALL_MSG + 506 )//对方已响铃  CFG负责抛出
#define  JSCALL_VOIP_SESSTALKING                        ( JSCALL_MSG + 507 )//开始通话中  CFG负责抛出
#define  JSCALL_VOIP_SESSTERM                           ( JSCALL_MSG + 508 )//终止通话  CFG负责抛出
#define  JSCALL_VOIP_SESSHOLDOK                         ( JSCALL_MSG + 509 )//保持通话成功  CFG负责抛出
#define  JSCALL_VOIP_SESSUNHOLDOK                       ( JSCALL_MSG + 510 )//解除保持通话成功  CFG负责抛出
#define  JSCALL_VOIP_INVALIDADDRESS                     ( JSCALL_MSG + 511 )//  CFG负责抛出
#define  JSCALL_VOIP_POSTSRVEVNT                        ( JSCALL_MSG + 512 )//  CFG负责抛出
#define  JSCALL_VOIP_PHONEHOOKOFF                       ( JSCALL_MSG + 513 )//电话机摘机  CFG负责抛出
#define  JSCALL_VOIP_PHONEHOOKON                        ( JSCALL_MSG + 514 )//电话机挂机  CFG负责抛出
#define  JSCALL_VOIP_AUDIODEVERROR                      ( JSCALL_MSG + 515 )//音频外设异常  CFG负责抛出
#define  JSCALL_VOIP_BLUETOOTHNOTCONNECT                ( JSCALL_MSG + 516 )//蓝牙设备未连接
#define  JSCALL_VOIP_PUSH_PHONEKEY                      ( JSCALL_MSG + 517 )//电话机按键

//dlna消息
#define  JSCALL_DLNA_SERVER_UPDATE                      ( JSCALL_MSG + 530 )//Server信息有变化
#define  JSCALL_DLNA_RENDER_PLAY                        ( JSCALL_MSG + 531 )//要播放指定的文件
#define  JSCALL_DLNA_RENDER_PAUSE                       ( JSCALL_MSG + 532 )//暂停播放
#define  JSCALL_DLNA_RENDER_STOP                        ( JSCALL_MSG + 533 )//停止播放
#define  JSCALL_DLNA_RENDER_SEEK                        ( JSCALL_MSG + 534 )//从指定时间点开始播放
#define  JSCALL_DLNA_RENDER_NEXT                        ( JSCALL_MSG + 535 )//播放下一个
#define  JSCALL_DLNA_RENDER_PREV                        ( JSCALL_MSG + 536 )//播放上一个
#define  JSCALL_DLNA_RENDER_MUTE                        ( JSCALL_MSG + 537 )//设置静音
#define  JSCALL_DLNA_RENDER_VOLU                        ( JSCALL_MSG + 538 )//修改音量
#define  JSCALL_DLNA_RENDER_RESUME                    ( JSCALL_MSG + 539 )//恢复播放


//消息提示,HTML上收到此消息后再决定是否显示或隐藏
#define  JSCALL_VIRTUAL_KEY                             0x300//
#define  JSCALL_INFO_SHOW                               ( JSCALL_MSG + 1000 ) //提示信息显示  jsext负责抛出
#define  JSCALL_INFO_HIDE                               ( JSCALL_MSG + 1001 ) //提示信息显示  jsext负责抛出

//应用管理器相关消息
#define JSCALL_AMS_DBUPDATE                             (JSCALL_MSG + 4001)//  jsext负责抛出
#define JSCALL_AMS_DBADDED                              (JSCALL_MSG + 4002)//  jsext负责抛出
#define JSCALL_AMS_DBDELETE                             (JSCALL_MSG + 4003)//  jsext负责抛出

#define JSCALL_AMS_DEPLOY_REQUEST_APPLIST_START         (JSCALL_MSG + 4004)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_REQUEST_APPLIST_OK            (JSCALL_MSG + 4005)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_REQUEST_APPLIST_FAILED        (JSCALL_MSG + 4006)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_UPDATE_START                  (JSCALL_MSG + 4007)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_UPDATE_OK                     (JSCALL_MSG + 4008)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_UPDATE_FAILED                 (JSCALL_MSG + 4009)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_DOWNLOAD_START                (JSCALL_MSG + 4010)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_DOWNLOAD_OK                   (JSCALL_MSG + 4011)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_DOWNLOAD_FAILED               (JSCALL_MSG + 4012)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_DELETE_START                  (JSCALL_MSG + 4013)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_DELETE_OK                     (JSCALL_MSG + 4014)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_DELETE_FAILED                 (JSCALL_MSG + 4015)//  jsext负责抛出
#define JSCALL_AMS_DEPLOY_FINISH                        (JSCALL_MSG + 4016)//  jsext负责抛出
#define JSCALL_AMS_APP_LOAD_OK                          (JSCALL_MSG + 4017)//  jsext负责抛出
#define JSCALL_AMS_APP_LOAD_FAILED                      (JSCALL_MSG + 4018)//  jsext负责抛出

#define  JSCALL_HTMLXLET_ACTIVE                         41002
#define  JSCALL_HTMLXLET_STOP                           41003


//自定义消息
#define  JSCALL_USERDEF_MSG_START                       (JSCALL_MSG + 5000)

//网络相关的消息
#define JSCALL_NETWORK_MSG_LINKUP                   (JSCALL_MSG + 5100)  //物理连接UP
#define JSCALL_NETWORK_MSG_LINKDOWN              (JSCALL_MSG + 5101)  //物理连接DOWN
#define JSCALL_NETWORK_MSG_CONNECTSUC          (JSCALL_MSG + 5102)  //网络连接Ok
#define JSCALL_NETWORK_MSG_IPCONFLIC              (JSCALL_MSG + 5103)  // IP地址冲突，静态IP模式下的MSG
#define JSCALL_NETWORK_MSG_IPERR                     (JSCALL_MSG + 5104)  //IP地址错误，静态IP模式下的MSG，IP地址，网关，掩码不匹配
#define JSCALL_NETWORK_MSG_NETMASKERR           (JSCALL_MSG + 5105)  //掩码错误，暂没有使用
#define JSCALL_NETWORK_MSG_GETEWAYERR           (JSCALL_MSG + 5106)  //网关错误，暂没有使用
#define JSCALL_NETWORK_MSG_DNSERR                   (JSCALL_MSG + 5107)  //DNS 错误,静态IP模式下的MSG。
#define JSCALL_NETWORK_MSG_DHCPFAIL                (JSCALL_MSG + 5108)  //DHCP失败，DHCP模式下的MSG，失败原因不明
#define JSCALL_NETWORK_MSG_DHCPTIMEOUT          (JSCALL_MSG + 5109)  //DHCP超时，DHCP模式下的MSG，超时未收到服务器回应
#define JSCALL_NETWORK_MSG_DHCPPARAMERR        (JSCALL_MSG + 5110) //DHCP参数错误，DHCP+模式下的MSG
#define JSCALL_NETWORK_MSG_PPPOEFAIL               (JSCALL_MSG + 5111)  //PPPOE失败，PPPOE模式下的MSG，失败原因不明
#define JSCALL_NETWORK_MSG_PPPOETIMEOUT         (JSCALL_MSG + 5112)  //PPPOE超时，PPPOE模式下的MSG，超时没有收到服务器回应
#define JSCALL_NETWORK_MSG_PPPOEAUTHFAIL        (JSCALL_MSG + 5113)  //PPPOE授权失败，PPPOE模式下的MSG，用户名，密码不匹配
#define JSCALL_NETWORK_MSG_PPPOEPARAMERR       (JSCALL_MSG + 5114)  //PPPOE参数错误，PPPOE模式下的MSG，用户名，密码为空
#define JSCALL_NETWORK_MSG_WLANFAIL                 (JSCALL_MSG + 5115)  //无线失败，无线连接特有MSG，失败原因不明
#define JSCALL_NETWORK_MSG_WLANINVALPARA       (JSCALL_MSG + 5116)  //无线参数错误，无线连接特有MSG，
#define JSCALL_NETWORK_MSG_WLANAPERR               (JSCALL_MSG + 5117)  //无线AP错误，无线连接特有MSG，
#define JSCALL_NETWORK_MSG_CONNECTING              (JSCALL_MSG + 5118)  //网络连接中,物理连接正常，网络连接由于某种原因从正常到异常，需要应用主动重连网络
#define JSCALL_NETWORK_MSG_CONNECTDOWN          (JSCALL_MSG + 5119)  //物理连接断开时间过长导致网络连接异常，此消息供应用处理提示消息
#define JSCALL_NETWORK_MSG_DOUBLUESTACKFAIL   (JSCALL_MSG + 5120)  //双栈失败
#define JSCALL_NETWORK_MSG_IPRENEW                   (JSCALL_MSG + 5121)  //IP地址更新消息

//NTP 相关的消息
#define JSCALL_NTP_MSGTYPE_GETTIME                     (JSCALL_MSG + 5150)  //NTP获取到时间
#define JSCALL_NTP_MSGTYPE_TIMEOUT                     (JSCALL_MSG + 5151)  //NTP获取时间超时
#define JSCALL_NTP_MSGTYPE_UNRESOLVED               (JSCALL_MSG + 5152)  //NTP主机无法解析
#define JSCALL_NTP_MSGTYPE_FAILURE                      (JSCALL_MSG + 5153)  //NTP失败，原因未知

#define  JSCALL_USERDEF_MSG_END                         (JSCALL_MSG + 6000)

#define  JSCALL_JAVA_MSG_START                          (JSCALL_MSG + 7000)
#define  JSCALL_JAVA_MSG_END                            (JSCALL_MSG + 9000)

//DVB调谐解调模块
#define NGB_MSG_DVB_SCAN_FINISHED  		        (10026)
#define NGB_MSG_DVB_SCAN_FAILED  		        (10027)

//ngb dtv协议模块
#define JSCALL_NGB_MSG_DVB_NIT_TIMEOUT          (11016)     //NIT超时
#define JSCALL_NGB_MSG_DVB_BAT_TIMEOUT          (11017)     //BAT超时

//ngb ca模块
#define JSCALL_NGB_MSG_CA_INVALID_AREA          (21052)     //区域码锁定

//ngb ota
#define JSCALL_NGB_MSG_CHECK_OTA_FAIL           (22103)     //手动检测OTA，未发现升级提示

//ngb 开机
#define JSCALL_NGB_MSG_CS_START_APP_FINISH      (30001)     //开机流程结束

//ngb nds 广告消息	 
#define JSCALL_NDS_AD_UPDATE  			  	(23001)
#define JSCALL_NDS_AD_SEARCH_COMPLETE  	    (23002)

#define NGB_MSG_VOD_BARKER_SUCCESS              (23101)     //成功获取VOD QAM Barker 频点
#define MSG_VOD_BARKER_TIMEOUT                  (23102)     //获取VOD QAM Barker 频点超时
#define NGB_MSG_VOD_BARKER_FAIL                 (23103)     //获取不到VOD QAM Barker 频点信息
#define NGB_MSG_VOD_SG_SUCCESS                  (23104)     //成功获取VOD Service Group ID
#define NGB_MSG_VOD_SG_TIMEOUT                  (23105)     //获取VOD Service Group ID 超时



/***********************JSCallback type 属性定义结束********************************************************/
typedef int (*JSCallback) (int type, void* pVoid, void* pvUserData);

#ifdef __cplusplus
}
#endif

#endif
