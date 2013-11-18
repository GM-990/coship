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

/***********************JSCallback type ���Զ��忪ʼ********************************************************/
#define  JSCALL_MSG                                     40000

//��������������淢�͵���Ϣ
#define  JSCALL_START_SCAN                              ( JSCALL_MSG + 1  ) //��ʼ����  jsext�����׳�
#define  JSCALL_START_DELIVER                           ( JSCALL_MSG + 2  ) //��ʼ����ĳ��Ƶ��  jsext�����׳�
#define  JSCALL_DELIVER_FINISH                          ( JSCALL_MSG + 3  ) //ĳ��Ƶ����������  jsext�����׳�
#define  JSCALL_START_NIT                               ( JSCALL_MSG + 4  ) //��ʼ����NIT��  jsext�����׳�
#define  JSCALL_NIT_FINISH                              ( JSCALL_MSG + 5  ) //NIT�������ɹ�  jsext�����׳�
#define  JSCALL_NIT_FAILED                              ( JSCALL_MSG + 6  ) //NIT������ʧ��  jsext�����׳�
#define  JSCALL_POP_SERVICE                             ( JSCALL_MSG + 7  ) //������Service  jsext�����׳�
#define  JSCALL_ALL_FINISHED                            ( JSCALL_MSG + 8  ) //����ȫ������  jsext�����׳�
#define  JSCALL_SEARCH_EXIT                             ( JSCALL_MSG + 9  ) //���������˳�  jsext�����׳�

//EPG����/���¹���������淢�͵���Ϣ
#define  JSCALL_PRESENT_EPG_CHANGED                     ( JSCALL_MSG + 11 ) //��ǰEvent��Ҫˢ��  jsext�����׳�
#define  JSCALL_FOLLOW_EPG_CHANGED                      ( JSCALL_MSG + 12 ) //��һ��Event��Ҫˢ��  jsext�����׳�
#define  JSCALL_SCHEDULE_EPG_FINISHED                   ( JSCALL_MSG + 13 ) //��ϸEPG������Ҫˢ��  jsext�����׳�
#define  JSCALL_SCHEDULE_EPG_ALL_FINISHED               ( JSCALL_MSG + 14 ) //���е���ϸEPG�����ɹ�  jsext�����׳�

//���Ź����¼���Ϣ
#define  JSCALL_CHANNEL_LOCKED                          ( JSCALL_MSG + 21 ) //Ƶ������/����  jsext�����׳�
#define  JSCALL_PG_LOCKED                               ( JSCALL_MSG + 22 ) //Ƶ����ĸ��/����  jsext�����׳�
#define  JSCALL_TUNE_LOST                               ( JSCALL_MSG + 23 ) //�����źŶ�ʧ  jsext�����׳�
#define  JSCALL_TUNE_LOCKED                             ( JSCALL_MSG + 24 ) //�����ź�����  jsext�����׳�
#define  JSCALL_PLAY_SUCCESS                            ( JSCALL_MSG + 25 ) //���ųɹ�  jsext�����׳�
#define  JSCALL_SI_UPDATE                               ( JSCALL_MSG + 26 ) //SI����  CFG�����׳�
#define  JSCALL_NO_CHANNEL                              ( JSCALL_MSG + 27 ) //��Ƶ��  jsext�����׳�
#define  JSCALL_PLAY_STOP                               ( JSCALL_MSG + 28 ) //ǰ�˽�Ŀͣ��  jsext�����׳�
#define  JSCALL_PLAY_START                              ( JSCALL_MSG + 29 ) //ǰ�˽�Ŀ�ָ�����  jsext�����׳�
#define  JSCALL_CHANNEL_UNLOCKED                        ( JSCALL_MSG + 30 ) //Ƶ������/����  jsext�����׳�
#define  JSCALL_PG_UNLOCKED                             ( JSCALL_MSG + 31 ) //Ƶ����ĸ��/����  jsext�����׳�
#define  JSCALL_SI_TIME_UPDATE                          ( JSCALL_MSG + 32 ) //SIͬ��ʱ��  jsext�����׳�
#define  JSCALL_DATA_CONNECT_FAIL                       ( JSCALL_MSG + 42 ) // ��������Դʧ��  jsext�����׳�
#define  JSCALL_DATA_CONNECT_SUCCESS                    ( JSCALL_MSG + 43 ) // ��������Դ�ɹ�  jsext�����׳�
#define  JSCALL_MEDIA_NOT_SUPPORT                       ( JSCALL_MSG + 44 ) // ý�����Ͳ�֧��  jsext�����׳�
#define  JSCALL_MEDIA_STOP_SUCCESS                      ( JSCALL_MSG + 45 ) // ý�岥��ֹͣ�ɹ�  jsext�����׳�


//NVOD����/���Ź�����Ϣ

//{NVODģ��֧����Ϣ
#define  JSCALL_NVOD_EVENT_PLAY_OVER                    ( JSCALL_MSG + 33 ) //NVODʱ���¼����Ž���  jsext�����׳�
#define	 JSCALL_NVOD_SEARCH_PROGRESS                    ( JSCALL_MSG + 34 ) //NVOD��������		����1-��������ReferenceEvent����,����2-��������ֵ  jsext�����׳�
#define	 JSCALL_NVOD_GROUP_LIST_CHANGED                 ( JSCALL_MSG + 35 ) //NVOD�����б�����ı�	  jsext�����׳�
#define	 JSCALL_NVOD_REFEVT_LIST_CHANGED                ( JSCALL_MSG + 36 ) //NVOD�ο��¼��б�����ı�	  jsext�����׳�
#define	 JSCALL_NVOD_SHTEVT_LIST_CHANGED                ( JSCALL_MSG + 37 ) //NVODʱ���¼��б�����ı�  jsext�����׳�
#define	 JSCALL_NVOD_SHTEVT_TIME_CHANGED                ( JSCALL_MSG + 38 ) //NVODʱ���¼�����ʱʱ��ı�  jsext�����׳�
#define  JSCALL_NVOD_ALL_COMPLETE                       ( JSCALL_MSG + 39 ) //NVOD��������  jsext�����׳�
#define  JSCALL_NVOD_REFEVT_COMPLETE                    ( JSCALL_MSG + 40 ) //NVOD�ο��¼���������  jsext�����׳�
#define  JSCALL_NVOD_SHTEVT_COMPLETE                    ( JSCALL_MSG + 41 ) //NVODʱ���¼���������  jsext�����׳�
//}��SVS-NVODģ��֧����Ϣ

//Ԥ���Ͷ�ʱ�������Ϣ
#define  JSCALL_SCHEDULE_PREV                           ( JSCALL_MSG + 51 ) //Ԥ����ǰ����  jsext�����׳�
#define  JSCALL_SCHEDULE_ONTIME                         ( JSCALL_MSG + 52 ) //Ԥ����Ŀ����  jsext�����׳�
#define  JSCALL_SCHEDULE_POST                           ( JSCALL_MSG + 53 ) //Ԥ����Ŀ�������  jsext�����׳�
#define  JSCALL_SCHEDULE_OVERTIME                       ( JSCALL_MSG + 54 ) //Ԥ����Ŀ�Ѿ�����  jsext�����׳�
#define  JSCALL_SCHEDULE_CHANGED                        ( JSCALL_MSG + 55 ) //Ԥ����Ŀ���ݸı�  jsext�����׳�

#define  JSCALL_TIMESWITCH_ON                           ( JSCALL_MSG + 61 ) //��ʱ����  ��û�õ�
#define  JSCALL_TIMESWITCH_OFF                          ( JSCALL_MSG + 62 ) //��ʱ����  ��û�õ�
#define  JSCALL_TIMESWITCH_CHANGED                      ( JSCALL_MSG + 63 ) //��ʱ���ػ����ݸı�  ��û�õ�
//����������Ϣ,��ʼֵ����
#define  JSCALL_PMT_COMPLETE                            ( JSCALL_MSG + 64 ) //PMT�������  ��û�õ�

//���ܿ������Ϣ
#define  JSCALL_SMART_IN                                ( JSCALL_MSG + 70 ) //���ܿ��Ѳ���  CFG�����׳�
#define  JSCALL_SMART_OUT                               ( JSCALL_MSG + 71 ) //���ܿ��Ѱγ�  CFG�����׳�

//CA�����Ϣ
#define  JSCALL_CA_INFO                                 ( JSCALL_MSG + 80 ) //CA ����ʾ��Ϣ,ΪCAָ��  CFG�����׳�
#define  JSCALL_CA_ALARM                                ( JSCALL_MSG + 81 ) //CA �ľ�����Ϣ,ΪCAָ��  CFG�����׳�
#define  JSCALL_CA_COMMAND                              ( JSCALL_MSG + 82 ) //CA ��������Ϣ,ΪCAָ��  CFG�����׳�
#define  JSCALL_CA_STATUS                               ( JSCALL_MSG + 83 ) //CA ��������Ϣ,ΪCAָ��  CFG�����׳�

//NETWORK ��Ϣ
#define  JSCALL_GET_IP_ADDR                             ( JSCALL_MSG + 90 ) //need update the ipaddr  CFG�����׳�   ??
#define  JSCALL_ETH_CONNECT                             ( JSCALL_MSG + 91 ) //network connect   CFG�����׳� ??
#define  JSCALL_ETH_DISCONNECT                          ( JSCALL_MSG + 92 ) //network diconnect  CFG�����׳�
#define  JSCALL_CM_ON_LINE                              ( JSCALL_MSG + 93 ) //CM online  CFG�����׳�
#define  JSCALL_CM_OFF_LINE                             ( JSCALL_MSG + 94 ) //CM offline  CFG�����׳�
#define  JSCALL_WIFI_DOWN                               ( JSCALL_MSG + 95 ) //���������γ� 
#define  JSCALL_WIFI_UP                                 ( JSCALL_MSG + 96 ) //������������

//OTA��Ϣ
#define  JSCALL_OTA_FORCE_UPGRADE                       ( JSCALL_MSG + 110 ) //OTAǿ��������Ϣ  CFG�����׳�
#define  JSCALL_OTA_MANUAL_UPGRADE                      ( JSCALL_MSG + 111 ) //OTA�ֶ�������ʾ  CFG�����׳�
#define  JSCALL_OTA_GET_ANALYSIS_DATA                   ( JSCALL_MSG + 112 ) //OTA ��ȡ��������������  CFG�����׳�
#define  JSCALL_OTA_GET_NIT_SUCCESS                     ( JSCALL_MSG + 113 ) //OTA ��ȡ������NIT�ɹ�  CFG�����׳�
#define  JSCALL_OTA_GET_NIT_FAILED                      ( JSCALL_MSG + 114 ) //OTA ��ȡ������NITʧ��  CFG�����׳�
#define  JSCALL_OTA_GET_PAT_SUCCESS                     ( JSCALL_MSG + 115 ) //OTA ��ȡ������PAT�ɹ�  CFG�����׳�
#define  JSCALL_OTA_GET_PAT_FAILED                      ( JSCALL_MSG + 116 ) //OTA ��ȡ������PATʧ��  CFG�����׳�
#define  JSCALL_OTA_GET_PMT_SUCCESS                     ( JSCALL_MSG + 117 ) //OTA ��ȡ������PMT�ɹ�  CFG�����׳�
#define  JSCALL_OTA_GET_PMT_FAILED                      ( JSCALL_MSG + 118 ) //OTA ��ȡ������PMTʧ��  CFG�����׳�
#define  JSCALL_OTA_WRITE_PARAM_FLASH_START             ( JSCALL_MSG + 119 ) //OTA ��¼����������flash  CFG�����׳�
#define  JSCALL_OTA_WRITE_PARAM_FLASH_SUCESS            ( JSCALL_MSG + 120 ) //OTA ��¼����������flash�ɹ�  CFG�����׳�
#define  JSCALL_OTA_WRITE_PARAM_FLASH_FAILED            ( JSCALL_MSG + 121 ) //OTA ��¼����������flashʧ��  CFG�����׳�
#define  JSCALL_OTA_REBOOT                              ( JSCALL_MSG + 122 ) //OTA ����������ȡ�ɹ�����������  CFG�����׳�
#define  JSCALL_OTA_NO_UPGRADE_INFO                     ( JSCALL_MSG + 123 ) //δ��⵽OTA������Ϣ  CFG�����׳�

//��Ŀ�Զ�������Ϣ
#define  JSCALL_NIT_AUTO_UPDATE                         ( JSCALL_MSG + 150 ) //NIT���Զ�����  CFG�����׳�
#define  JSCALL_BAT_AUTO_UPDATE                         ( JSCALL_MSG + 151 ) //BAT���Զ�����  CFG�����׳�
#define  JSCALL_PMT_AUTO_UPDATE                         ( JSCALL_MSG + 152 ) //PMT���Զ�����  CFG�����׳�

//�����Ϣ
#define  JSCALL_AD_CURRENT_CONTENT_CHANGE               ( JSCALL_MSG + 160 ) //��ǰ��Ŀ��ĳ���λ���ݱ仯  jsext�����׳�
#define  JSCALL_AD_FIND_STB_CTRL_INFO                   ( JSCALL_MSG + 161 ) //�������û�STBƥ��Ŀ�����Ϣ  jsext�����׳�

//Mosaicҵ�������Ϣ
#define  JSCALL_MOSAIC_COLLECT_MOSAIC_PAGE              (JSCALL_MSG + 170)//  jsext�����׳�
#define  JSCALL_MOSAIC_COLLECT_ERROR                    (JSCALL_MSG + 171)//  jsext�����׳�
#define  JSCALL_MOSAIC_COLLECT_COMPLETE                 (JSCALL_MSG + 172)//  jsext�����׳�
#define  JSCALL_MOSAIC_COLLECT_UPDATE                   (JSCALL_MSG + 173)//  jsext�����׳�


//��ʱ�������Ϣ
#define  JSCALL_TIMER_ONTIME                            (JSCALL_MSG + 180)//  jsext�����׳�

//ý�岥�Ź�������Ϣ
#define  JSCALL_MEDIA_END                               (JSCALL_MSG + 200)//  jsext�����׳�
#define  JSCALL_MEDIA_BEGINING                          (JSCALL_MSG + 201)//  jsext�����׳�
#define  JSCALL_MEDIA_ERROR                             (JSCALL_MSG + 202)//  jsext�����׳�
#define  JSCALL_MEDIA_PLAYMODE_CHANGE                   (JSCALL_MSG + 203)//  jsext�����׳�
#define  JSCALL_MEDIA_FRONTTS_STOP                      (JSCALL_MSG + 204) //���Ź����У�ǰ������ͣ��  jsext�����׳�
#define  JSCALL_MEDIA_FRONTTS_RUN                       (JSCALL_MSG + 205) //���Ź����У�ǰ�������ڲ�  jsext�����׳�
#define  JSCALL_MEDIA_PREV_ITEM_STEPPED                 (JSCALL_MSG + 206) //���˵���һ�ν�Ŀ  jsext�����׳�
#define  JSCALL_MEDIA_NEXT_ITEM_STEPPED                 (JSCALL_MSG + 207) //�������һ�ν�Ŀ  jsext�����׳�
#define  JSCALL_MEDIA_NEW_FRAME                         (JSCALL_MSG + 208) //��֡����  jsext�����׳�
#define  JSCALL_MEDIA_SHOW_SUBTITLE                     (JSCALL_MSG + 209) //��Ļ����  jsext�����׳�
#define  JSCALL_MEDIA_CLEAR_SUBTITLE                    (JSCALL_MSG + 210) //��Ļ��ʧ  jsext�����׳�

//�豸������Ϣ
#define  JSCALL_STORAGE_PLUGIN                          ( JSCALL_MSG + 220 ) //�������豸����  jsext�����׳�
#define  JSCALL_STORAGE_PLUGOUT                         ( JSCALL_MSG + 221 ) //�豸���γ�  jsext�����׳�
#define  JSCALL_PARTITION_FOUND                         ( JSCALL_MSG + 226 ) //  jsext�����׳�
#define  JSCALL_PARTITION_MOUNTED                       ( JSCALL_MSG + 227 ) //  jsext�����׳�
#define  JSCALL_PARTITION_MOUNT_FAILED                  ( JSCALL_MSG + 228 ) //  jsext�����׳�
#define  JSCALL_PARTITION_UNINSTALL                     ( JSCALL_MSG + 229 ) //  jsext�����׳�

//Teletext��Ϣ
#define  JSCALL_TELETEXT_START_DISPLAY                  ( JSCALL_MSG + 250 ) //ҳ�濪ʼ��ʾ ??
#define  JSCALL_TELETEXT_PAGE_UPDATE                    ( JSCALL_MSG + 251 ) //ҳ�����
#define  JSCALL_TELETEXT_LINK_FLASH                     ( JSCALL_MSG + 252 ) //Linkҳ��˸
#define  JSCALL_TELETEXT_PAGE_FLASH                     ( JSCALL_MSG + 253 ) //һ��ҳ������˸
#define  JSCALL_TELETEXT_CAPTION_PAGE                   ( JSCALL_MSG + 254 ) //��ʾClose Captionҳ

//PVR��Ϣ
#define  JSCALL_PVRFILE_DEL_PROGRESS                    ( JSCALL_MSG + 255 ) //�ļ�����֪ͨɾ��  jsext�����׳�
#define  JSCALL_PVRFILE_UPDATE_FILELIST                 ( JSCALL_MSG + 256 ) //�ļ��б����֪ͨ  jsext�����׳�
#define  JSCALL_PVRFILE_HDD_FULL                        ( JSCALL_MSG + 257 ) //�洢�豸����֪ͨ  jsext�����׳�
#define  JSCALL_PVRFILE_PLUGIN_STORAGE                  ( JSCALL_MSG + 258 )  //ʶ���´洢�豸֪ͨ  jsext�����׳�
#define  JSCALL_PVRFILE_PLUGOUT_STORAGE                 ( JSCALL_MSG + 259 ) //�洢�豸���γ�֪ͨ  jsext�����׳�
#define  JSCALL_PVRRSV_BEFORE_START                     ( JSCALL_MSG + 260 ) //Ԥ��¼�Ƽ�������֪ͨ  jsext�����׳�
#define  JSCALL_PVRRSV_START_SUCCES                     ( JSCALL_MSG + 261 ) //Ԥ��¼���ѳɹ�����֪ͨ  jsext�����׳�
#define  JSCALL_PVRRSV_START_FAILED                     ( JSCALL_MSG + 262 ) //Ԥ��¼������ʧ��֪ͨ  jsext�����׳�
#define  JSCALL_PVRRSV_BEFORE_STOP                      ( JSCALL_MSG + 263 ) //Ԥ��¼�Ƽ�������֪ͨ  jsext�����׳�
#define  JSCALL_PVRRSV_OUTOF_DATE                       ( JSCALL_MSG + 264 ) //Ԥ��¼������ڱ�ɾ����֪ͨ  jsext�����׳�
#define  JSCALL_PVRPLAY_TSHIFT_SWITCH                   ( JSCALL_MSG + 265 ) //ֱ����TShift�����л�֪ͨ  jsext�����׳�
#define  JSCALL_PVRPLAY_SPEED_CHANGE                    ( JSCALL_MSG + 266 ) //�����ٶȱ仯֪ͨ  jsext�����׳�
#define  JSCALL_PVRPLAY_END_OF_FILE                     ( JSCALL_MSG + 267 ) //���ŵ��ļ�ĩβ֪ͨ  jsext�����׳�
#define  JSCALL_PVRPLAY_BEGIN_OF_FILE                   ( JSCALL_MSG + 268 ) //���ŵ��ļ���ͷ֪ͨ  jsext�����׳�
#define  JSCALL_PVRREC_NOTIFY_STOP                      ( JSCALL_MSG + 269 ) //¼��ֹ֪ͣͨ  jsext�����׳�

#define  JSCALL_PANEL_CHANNEL                           ( JSCALL_MSG + 280 )   //ǰ����Ŀ��ʾ ??

//USB����������Ϣ
#define  JSCALL_USB_UPGRADE_RS232_START                 ( JSCALL_MSG + 281 ) //��ʼ����  jsext�����׳�
#define  JSCALL_USB_DOWNLOAD_RS232_START                ( JSCALL_MSG + 282 ) //��ʼ����  jsext�����׳�
#define  JSCALL_USB_RS232_HEAD_MANU_ERROR               ( JSCALL_MSG + 283 ) //��ͷ��Ϣ�еĳ���������ƥ��  jsext�����׳�
#define  JSCALL_USB_RS232_HEAD_MACHINE_ERROR            ( JSCALL_MSG + 284 ) //��ͷ��Ϣ�еĻ���������ƥ��  jsext�����׳�
#define  JSCALL_USB_RS232_HEAD_VER_ERROR                ( JSCALL_MSG + 285 )  //��ͷ��Ϣ�е�����汾��ƥ��  jsext�����׳�
#define  JSCALL_USB_RS232_HEAD_CRC_ERROR                ( JSCALL_MSG + 286 ) //��ͷ��ϢCRCУ��ʧ��  jsext�����׳�
#define  JSCALL_USB_RS232_FILE_CRC_ERROR                ( JSCALL_MSG + 287 ) //rs232�ļ�CRCУ��ʧ��  jsext�����׳�
#define  JSCALL_USB_RS232_HEAD_OK                       ( JSCALL_MSG + 288 ) //��ͷ��Ϣ��ȷ  jsext�����׳�
#define  JSCALL_USB_READ_RS232_ERROR                    ( JSCALL_MSG + 289 ) //��rs232�ļ�ʧ��  jsext�����׳�
#define  JSCALL_USB_WRITE_RS232_ERROR                   ( JSCALL_MSG + 290 ) //дrs232�ļ�ʧ��  jsext�����׳�
#define  JSCALL_USB_READ_FLASH_ERROR                    ( JSCALL_MSG + 291 ) //��flashʧ��  jsext�����׳�
#define  JSCALL_USB_WRITE_FLASH_ERROR                   ( JSCALL_MSG + 292 ) //дflashʧ��  jsext�����׳�
#define  JSCALL_USB_TRANS_DATA_64K                      ( JSCALL_MSG + 293 ) //��USB�ж�ȡ64K���ݵ��ڴ�  jsext�����׳�
#define  JSCALL_USB_WRITE_DATA_64K                      ( JSCALL_MSG + 294 ) //��64k���ݴ��ڴ�д��flash  jsext�����׳�
#define  JSCALL_USB_UPGRADE_RS232_END                   ( JSCALL_MSG + 295) //�����ɹ�  jsext�����׳�
#define  JSCALL_USB_DOWNLOAD_RS232_END                  ( JSCALL_MSG + 296 ) //���سɹ�  jsext�����׳�
#define  JSCALL_USB_MEM_ALLOC_FAILED                    ( JSCALL_MSG + 297 ) //�ڴ����ʧ��  jsext�����׳�
#define  JSCALL_USB_UNKNOW_ERROR                        ( JSCALL_MSG + 298 ) //δ֪����  jsext�����׳�

//����������Ϣ
#define  JSCALL_IP_DOWNLOADING                          ( JSCALL_MSG + 301 )//����������������������  jsext�����׳�
#define  JSCALL_IP_DL_FAILED                            ( JSCALL_MSG + 302 )//����ʧ��  jsext�����׳�
#define  JSCALL_IP_DL_CONNECT_FAILED                    ( JSCALL_MSG + 303 )//�����źŶ�ʧ  jsext�����׳�
#define  JSCALL_IP_DL_CONNECT_RECOVER                   ( JSCALL_MSG + 304 )//�������ӻָ�  jsext�����׳�
#define  JSCALL_IP_NETWORK_LOST                         ( JSCALL_MSG + 305 )//��������ʧ��  jsext�����׳�
#define  JSCALL_IP_DL_OVERTIME                          ( JSCALL_MSG + 306 )//�����������ݳ�ʱ  jsext�����׳�
#define  JSCALL_IP_WRITING                              ( JSCALL_MSG + 307 )//����дfalsh  jsext�����׳�
#define  JSCALL_IP_WRITE_FAILED                         ( JSCALL_MSG + 308 )//дflashʧ��	  jsext�����׳�
#define  JSCALL_IP_VERIFYING                            ( JSCALL_MSG + 309 )//����У������  jsext�����׳�
#define  JSCALL_IP_RS232_HEAD_MANU_ERROR                ( JSCALL_MSG + 310 )//��ͷ��Ϣ�е�manufacture  description ����  jsext�����׳�
#define  JSCALL_IP_RS232_HEAD_MACHINE_ERROR             ( JSCALL_MSG + 311 )//��ͷ��Ϣ�е�machine description ����  jsext�����׳�
#define  JSCALL_IP_RS232_HEAD_CRC_ERROR                 ( JSCALL_MSG + 312 )//��ͷ��ϢCRCУ��ʧ��  jsext�����׳�
#define  JSCALL_IP_RS232_FILE_CRC_ERROR                 ( JSCALL_MSG + 313 )//�����ļ�CRCУ��ʧ��  jsext�����׳�
#define  JSCALL_IP_RS232_HEAD_OK                        ( JSCALL_MSG + 314 )//��ͷ��Ϣ��ȷ  jsext�����׳�
#define  JSCALL_IP_UPGRADE_SUCCESS                      ( JSCALL_MSG + 315 )//�����ɹ�  jsext�����׳�
#define  JSCALL_IP_MEMORY_FULL                          ( JSCALL_MSG + 316 )//�ڴ����ʧ��  jsext�����׳�
#define  JSCALL_IP_UNKNOWN_ERROR                        ( JSCALL_MSG + 317 )//δ֪����  jsext�����׳�

// tts��Ϣ
#define JSCALL_TTS_END_OF_PLAY                          (JSCALL_MSG + 320)	// tts�ı�����������Ϣ ??

//������Ϣ
#define JSCALL_BT_SCAN_END                              (JSCALL_MSG + 330)	//ɨ�������豸���
#define JSCALL_BT_PAIR_END                              (JSCALL_MSG + 331)	//�����豸������
#define JSCALL_BT_CONNECT_END                           (JSCALL_MSG + 332)	//�����豸�������
#define JSCALL_BT_FOUND_DEVICE                          (JSCALL_MSG + 333)	//���������豸
#define JSCALL_BT_CONNECT_LOST                          (JSCALL_MSG + 334)	//���������豸���ӶϿ�


//��page�����Ϣ
#define  JSCALL_PAGE_HIDE                               (JSCALL_MSG + 340)//��ǰpage����������  jsext�����׳�
#define  JSCALL_PAGE_SHOW                               (JSCALL_MSG + 341)//��ǰpage��������ʾ  jsext�����׳�

//����ʶ��
#define JSCALL_SR_REC_START                             (JSCALL_MSG + 350)	// ��ʼ¼��
#define JSCALL_SR_SPEECH_END                            (JSCALL_MSG + 351)	// ��⵽��������
#define JSCALL_SR_ERROR                                 (JSCALL_MSG + 352)	// ����
#define JSCALL_SR_RESULT                                (JSCALL_MSG + 353)	// ʶ����
#define JSCALL_SR_VOLUME                                (JSCALL_MSG + 354)	// ��ȡ����

//media ���������Ϣ
#define  JSCALL_MEDIA_STATE_LOADING                     (JSCALL_MSG + 400)//���濪ʼ  jsext�����׳�
#define  JSCALL_MEDIA_STATE_LOADED                      (JSCALL_MSG + 401)//�������  jsext�����׳�
#define  JSCALL_MEDIA_STATE_HAVEMETADATA                (JSCALL_MSG + 401)//�ܹ���ȡһЩ������Ϣ����Ƭ��  jsext�����׳�
#define  JSCALL_MEDIA_STATE_ENOUGHDATA_FORPLAY          (JSCALL_MSG + 403)//���㹻�����ݽ��в��ţ�����п�����˶�����  jsext�����׳�
#define  JSCALL_MEDIA_STATE_DURATIONCHANGE              (JSCALL_MSG + 404)//��Ŀ��ʱ�������仯����Ҫ����������ʱ��  jsext�����׳�
#define  JSCALL_MEDIA_STATE_RATECHANGE                  (JSCALL_MSG + 405)//��Ŀ�ܲ��ű��������仯 jsext�����׳�
#define  JSCALL_MEDIA_STATE_LOADING_PROGRESS            (JSCALL_MSG + 406)//�ȴ����ţ��������ݵĽ���ֵ  jsext�����׳�
#define  JSCALL_MEDIA_STATE_STOP                        (JSCALL_MSG + 407)//ֹͣ�����������ɹ�  jsext�����׳�
#define  JSCALL_MEDIA_STATE_START                       (JSCALL_MSG + 408)//���������������ɹ�  jsext�����׳�
#define  JSCALL_MEDIA_STATE_PAUSE                       (JSCALL_MSG + 409)//��ͣ�����������ɹ�  jsext�����׳�
#define  JSCALL_MEDIA_SEEK_FINISH                       (JSCALL_MSG + 410)//ѡʱ���Ŷ�λ�ɹ�  jsext�����׳�

//voip��Ϣ
#define  JSCALL_VOIP_LOGINOK                            ( JSCALL_MSG + 500 )//��½�ɹ�  CFG�����׳�
#define  JSCALL_VOIP_LOGINFAILED                        ( JSCALL_MSG + 501 )//��½ʧ��  CFG�����׳�
#define  JSCALL_VOIP_LCLLOGOUT                          ( JSCALL_MSG + 502 )//����ע���ɹ�  CFG�����׳�
#define  JSCALL_VOIP_SRVLOGOUT                          ( JSCALL_MSG + 503 )//������ע���ɹ�  CFG�����׳�
#define  JSCALL_VOIP_SESSCALLIN                         ( JSCALL_MSG + 504 )//�յ�Զ�˵绰  CFG�����׳�
#define  JSCALL_VOIP_SESSCALLOUT                        ( JSCALL_MSG + 505 )//�绰�Ѳ���  CFG�����׳�
#define  JSCALL_VOIP_SESSALERTED                        ( JSCALL_MSG + 506 )//�Է�������  CFG�����׳�
#define  JSCALL_VOIP_SESSTALKING                        ( JSCALL_MSG + 507 )//��ʼͨ����  CFG�����׳�
#define  JSCALL_VOIP_SESSTERM                           ( JSCALL_MSG + 508 )//��ֹͨ��  CFG�����׳�
#define  JSCALL_VOIP_SESSHOLDOK                         ( JSCALL_MSG + 509 )//����ͨ���ɹ�  CFG�����׳�
#define  JSCALL_VOIP_SESSUNHOLDOK                       ( JSCALL_MSG + 510 )//�������ͨ���ɹ�  CFG�����׳�
#define  JSCALL_VOIP_INVALIDADDRESS                     ( JSCALL_MSG + 511 )//  CFG�����׳�
#define  JSCALL_VOIP_POSTSRVEVNT                        ( JSCALL_MSG + 512 )//  CFG�����׳�
#define  JSCALL_VOIP_PHONEHOOKOFF                       ( JSCALL_MSG + 513 )//�绰��ժ��  CFG�����׳�
#define  JSCALL_VOIP_PHONEHOOKON                        ( JSCALL_MSG + 514 )//�绰���һ�  CFG�����׳�
#define  JSCALL_VOIP_AUDIODEVERROR                      ( JSCALL_MSG + 515 )//��Ƶ�����쳣  CFG�����׳�
#define  JSCALL_VOIP_BLUETOOTHNOTCONNECT                ( JSCALL_MSG + 516 )//�����豸δ����
#define  JSCALL_VOIP_PUSH_PHONEKEY                      ( JSCALL_MSG + 517 )//�绰������

//dlna��Ϣ
#define  JSCALL_DLNA_SERVER_UPDATE                      ( JSCALL_MSG + 530 )//Server��Ϣ�б仯
#define  JSCALL_DLNA_RENDER_PLAY                        ( JSCALL_MSG + 531 )//Ҫ����ָ�����ļ�
#define  JSCALL_DLNA_RENDER_PAUSE                       ( JSCALL_MSG + 532 )//��ͣ����
#define  JSCALL_DLNA_RENDER_STOP                        ( JSCALL_MSG + 533 )//ֹͣ����
#define  JSCALL_DLNA_RENDER_SEEK                        ( JSCALL_MSG + 534 )//��ָ��ʱ��㿪ʼ����
#define  JSCALL_DLNA_RENDER_NEXT                        ( JSCALL_MSG + 535 )//������һ��
#define  JSCALL_DLNA_RENDER_PREV                        ( JSCALL_MSG + 536 )//������һ��
#define  JSCALL_DLNA_RENDER_MUTE                        ( JSCALL_MSG + 537 )//���þ���
#define  JSCALL_DLNA_RENDER_VOLU                        ( JSCALL_MSG + 538 )//�޸�����
#define  JSCALL_DLNA_RENDER_RESUME                    ( JSCALL_MSG + 539 )//�ָ�����


//��Ϣ��ʾ,HTML���յ�����Ϣ���پ����Ƿ���ʾ������
#define  JSCALL_VIRTUAL_KEY                             0x300//
#define  JSCALL_INFO_SHOW                               ( JSCALL_MSG + 1000 ) //��ʾ��Ϣ��ʾ  jsext�����׳�
#define  JSCALL_INFO_HIDE                               ( JSCALL_MSG + 1001 ) //��ʾ��Ϣ��ʾ  jsext�����׳�

//Ӧ�ù����������Ϣ
#define JSCALL_AMS_DBUPDATE                             (JSCALL_MSG + 4001)//  jsext�����׳�
#define JSCALL_AMS_DBADDED                              (JSCALL_MSG + 4002)//  jsext�����׳�
#define JSCALL_AMS_DBDELETE                             (JSCALL_MSG + 4003)//  jsext�����׳�

#define JSCALL_AMS_DEPLOY_REQUEST_APPLIST_START         (JSCALL_MSG + 4004)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_REQUEST_APPLIST_OK            (JSCALL_MSG + 4005)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_REQUEST_APPLIST_FAILED        (JSCALL_MSG + 4006)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_UPDATE_START                  (JSCALL_MSG + 4007)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_UPDATE_OK                     (JSCALL_MSG + 4008)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_UPDATE_FAILED                 (JSCALL_MSG + 4009)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_DOWNLOAD_START                (JSCALL_MSG + 4010)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_DOWNLOAD_OK                   (JSCALL_MSG + 4011)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_DOWNLOAD_FAILED               (JSCALL_MSG + 4012)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_DELETE_START                  (JSCALL_MSG + 4013)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_DELETE_OK                     (JSCALL_MSG + 4014)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_DELETE_FAILED                 (JSCALL_MSG + 4015)//  jsext�����׳�
#define JSCALL_AMS_DEPLOY_FINISH                        (JSCALL_MSG + 4016)//  jsext�����׳�
#define JSCALL_AMS_APP_LOAD_OK                          (JSCALL_MSG + 4017)//  jsext�����׳�
#define JSCALL_AMS_APP_LOAD_FAILED                      (JSCALL_MSG + 4018)//  jsext�����׳�

#define  JSCALL_HTMLXLET_ACTIVE                         41002
#define  JSCALL_HTMLXLET_STOP                           41003


//�Զ�����Ϣ
#define  JSCALL_USERDEF_MSG_START                       (JSCALL_MSG + 5000)

//������ص���Ϣ
#define JSCALL_NETWORK_MSG_LINKUP                   (JSCALL_MSG + 5100)  //��������UP
#define JSCALL_NETWORK_MSG_LINKDOWN              (JSCALL_MSG + 5101)  //��������DOWN
#define JSCALL_NETWORK_MSG_CONNECTSUC          (JSCALL_MSG + 5102)  //��������Ok
#define JSCALL_NETWORK_MSG_IPCONFLIC              (JSCALL_MSG + 5103)  // IP��ַ��ͻ����̬IPģʽ�µ�MSG
#define JSCALL_NETWORK_MSG_IPERR                     (JSCALL_MSG + 5104)  //IP��ַ���󣬾�̬IPģʽ�µ�MSG��IP��ַ�����أ����벻ƥ��
#define JSCALL_NETWORK_MSG_NETMASKERR           (JSCALL_MSG + 5105)  //���������û��ʹ��
#define JSCALL_NETWORK_MSG_GETEWAYERR           (JSCALL_MSG + 5106)  //���ش�����û��ʹ��
#define JSCALL_NETWORK_MSG_DNSERR                   (JSCALL_MSG + 5107)  //DNS ����,��̬IPģʽ�µ�MSG��
#define JSCALL_NETWORK_MSG_DHCPFAIL                (JSCALL_MSG + 5108)  //DHCPʧ�ܣ�DHCPģʽ�µ�MSG��ʧ��ԭ����
#define JSCALL_NETWORK_MSG_DHCPTIMEOUT          (JSCALL_MSG + 5109)  //DHCP��ʱ��DHCPģʽ�µ�MSG����ʱδ�յ���������Ӧ
#define JSCALL_NETWORK_MSG_DHCPPARAMERR        (JSCALL_MSG + 5110) //DHCP��������DHCP+ģʽ�µ�MSG
#define JSCALL_NETWORK_MSG_PPPOEFAIL               (JSCALL_MSG + 5111)  //PPPOEʧ�ܣ�PPPOEģʽ�µ�MSG��ʧ��ԭ����
#define JSCALL_NETWORK_MSG_PPPOETIMEOUT         (JSCALL_MSG + 5112)  //PPPOE��ʱ��PPPOEģʽ�µ�MSG����ʱû���յ���������Ӧ
#define JSCALL_NETWORK_MSG_PPPOEAUTHFAIL        (JSCALL_MSG + 5113)  //PPPOE��Ȩʧ�ܣ�PPPOEģʽ�µ�MSG���û��������벻ƥ��
#define JSCALL_NETWORK_MSG_PPPOEPARAMERR       (JSCALL_MSG + 5114)  //PPPOE��������PPPOEģʽ�µ�MSG���û���������Ϊ��
#define JSCALL_NETWORK_MSG_WLANFAIL                 (JSCALL_MSG + 5115)  //����ʧ�ܣ�������������MSG��ʧ��ԭ����
#define JSCALL_NETWORK_MSG_WLANINVALPARA       (JSCALL_MSG + 5116)  //���߲�������������������MSG��
#define JSCALL_NETWORK_MSG_WLANAPERR               (JSCALL_MSG + 5117)  //����AP����������������MSG��
#define JSCALL_NETWORK_MSG_CONNECTING              (JSCALL_MSG + 5118)  //����������,��������������������������ĳ��ԭ����������쳣����ҪӦ��������������
#define JSCALL_NETWORK_MSG_CONNECTDOWN          (JSCALL_MSG + 5119)  //�������ӶϿ�ʱ������������������쳣������Ϣ��Ӧ�ô�����ʾ��Ϣ
#define JSCALL_NETWORK_MSG_DOUBLUESTACKFAIL   (JSCALL_MSG + 5120)  //˫ջʧ��
#define JSCALL_NETWORK_MSG_IPRENEW                   (JSCALL_MSG + 5121)  //IP��ַ������Ϣ

//NTP ��ص���Ϣ
#define JSCALL_NTP_MSGTYPE_GETTIME                     (JSCALL_MSG + 5150)  //NTP��ȡ��ʱ��
#define JSCALL_NTP_MSGTYPE_TIMEOUT                     (JSCALL_MSG + 5151)  //NTP��ȡʱ�䳬ʱ
#define JSCALL_NTP_MSGTYPE_UNRESOLVED               (JSCALL_MSG + 5152)  //NTP�����޷�����
#define JSCALL_NTP_MSGTYPE_FAILURE                      (JSCALL_MSG + 5153)  //NTPʧ�ܣ�ԭ��δ֪

#define  JSCALL_USERDEF_MSG_END                         (JSCALL_MSG + 6000)

#define  JSCALL_JAVA_MSG_START                          (JSCALL_MSG + 7000)
#define  JSCALL_JAVA_MSG_END                            (JSCALL_MSG + 9000)

//DVB��г���ģ��
#define NGB_MSG_DVB_SCAN_FINISHED  		        (10026)
#define NGB_MSG_DVB_SCAN_FAILED  		        (10027)

//ngb dtvЭ��ģ��
#define JSCALL_NGB_MSG_DVB_NIT_TIMEOUT          (11016)     //NIT��ʱ
#define JSCALL_NGB_MSG_DVB_BAT_TIMEOUT          (11017)     //BAT��ʱ

//ngb caģ��
#define JSCALL_NGB_MSG_CA_INVALID_AREA          (21052)     //����������

//ngb ota
#define JSCALL_NGB_MSG_CHECK_OTA_FAIL           (22103)     //�ֶ����OTA��δ����������ʾ

//ngb ����
#define JSCALL_NGB_MSG_CS_START_APP_FINISH      (30001)     //�������̽���

//ngb nds �����Ϣ	 
#define JSCALL_NDS_AD_UPDATE  			  	(23001)
#define JSCALL_NDS_AD_SEARCH_COMPLETE  	    (23002)

#define NGB_MSG_VOD_BARKER_SUCCESS              (23101)     //�ɹ���ȡVOD QAM Barker Ƶ��
#define MSG_VOD_BARKER_TIMEOUT                  (23102)     //��ȡVOD QAM Barker Ƶ�㳬ʱ
#define NGB_MSG_VOD_BARKER_FAIL                 (23103)     //��ȡ����VOD QAM Barker Ƶ����Ϣ
#define NGB_MSG_VOD_SG_SUCCESS                  (23104)     //�ɹ���ȡVOD Service Group ID
#define NGB_MSG_VOD_SG_TIMEOUT                  (23105)     //��ȡVOD Service Group ID ��ʱ



/***********************JSCallback type ���Զ������********************************************************/
typedef int (*JSCallback) (int type, void* pVoid, void* pvUserData);

#ifdef __cplusplus
}
#endif

#endif
