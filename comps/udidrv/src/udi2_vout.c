#include "generic_include.h"
#include "udi2_vout.h"

#define MODULE_NAME "CS_VOUT"

CSUDI_Error_Code CSUDIVOUTEnable(CSUDIVOUTDevices_E eVoutDevice)
{
    CSUDI_Error_Code retcode = CSUDI_SUCCESS;
    PIPE_VP_DEVICE_HDMI_CONFIG hdmicfg;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s\n",__FUNCTION__);
     switch(eVoutDevice)
     	{
          case EM_UDIVOUT_CVBS:/**< CSBVS视频输出接口 */
		     retcode = cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_D_IDX),1);
		     break;
	    case EM_UDIVOUT_YUV:  /**< 分量视频输出接口 */
		     retcode = cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_A_IDX),1) ||\
		                    cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_B_IDX),1) ||\
		                    cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_C_IDX),1);
		     break;
	   case EM_UDIVOUT_HDMI: /**< HDMI视频输出接口 */
	   	     //retcode =  cnxt_tvenc_hdmi_set_standby(gDisplayConfig.hDIsplayHandle[0].hTVEnc, 0);
	   	     //hdmicfg.standby = 0;
             if(gTmVpDeviceObj.pHDDevice != NULL)
	   	     gTmVpDeviceObj.pHDDevice->hdmi_config(gTmVpDeviceObj.pHDDevice,&hdmicfg);
	   	     break;
	   case  EM_UDIVOUT_YC:     /**< S-Video 视频输出接口 */
	   	     retcode = cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_E_IDX),1) ||\
			 	      cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_F_IDX),1); 
		     break;	
         case  EM_UDIVOUT_DVO: 
	   case  EM_UDIVOUT_RF:      /**< 射频信号视频输出接口 */
	   case  EM_UDIVOUT_SCART:  /**< SCART视频输出接口 */
	   default:
	   	    retcode = CSUDI_FAILURE;
	}
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"exit Function :%s\n",__FUNCTION__);
	 return retcode; 
}

CSUDI_Error_Code CSUDIVOUTDisable(CSUDIVOUTDevices_E eVoutDevice)
{
    CSUDI_Error_Code retcode = CSUDI_SUCCESS;
    PIPE_VP_DEVICE_HDMI_CONFIG hdmicfg;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s\n",__FUNCTION__);
    cnxt_kal_memset(&hdmicfg,0,sizeof(hdmicfg));	   	
     switch(eVoutDevice)
     	{
          case EM_UDIVOUT_CVBS:/**< CSBVS视频输出接口 */
		     retcode = cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_D_IDX),0);
		     break;
	    case EM_UDIVOUT_YUV:   /**< 分量视频输出接口 */
		     retcode =  cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_A_IDX),0) ||\
		                    cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_B_IDX),0) ||\
		                    cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_C_IDX),0);
		     break;
	   case EM_UDIVOUT_HDMI:   /**< HDMI视频输出接口 */
             //hdmicfg.standby = 1;
             hdmicfg.ColorSpace = CNXT_TVENC_HDMI_COLOR_SPACE_RGB_FULL;
	   	     //retcode =  cnxt_tvenc_hdmi_set_standby(gDisplayConfig.hDIsplayHandle[0].hTVEnc, 1);
	   	     if(gTmVpDeviceObj.pHDDevice != NULL)
	   	     gTmVpDeviceObj.pHDDevice->hdmi_config(gTmVpDeviceObj.pHDDevice,&hdmicfg);
	   	     break;
	   case  EM_UDIVOUT_YC:     /**< S-Video 视频输出接口 */
	   	     retcode = cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_E_IDX),0) ||\
			 	      cnxt_tvenc_enable_dac(CNXT_TVENC_DAC_IDX_TO_FLAG(CNXT_TVENC_DAC_F_IDX),0);
                  break;
	   case  EM_UDIVOUT_DVO: 
	   case  EM_UDIVOUT_RF:      /**< 射频信号视频输出接口 */
	   case  EM_UDIVOUT_SCART:  /**< SCART视频输出接口 */
	   default:
	   	    retcode = CSUDI_FAILURE;
	}
	 CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s\n",__FUNCTION__);
	 return retcode;
}
CSUDI_Error_Code CSUDIVOUTSetOutputType(CSUDIVOUTDevices_E eVoutDevice, CSUDIVOUTOutputType_E eType)
{
    return (CSUDI_Error_Code)(0);
}


