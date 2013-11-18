#ifndef _UDI_INIT_H
#define _UDI_INIT_H
#define  PANEL_GPIO_COM0  21
#define  PANEL_GPIO_COM1  24
#define  PANEL_GPIO_COM2  20
#define  PANEL_GPIO_COM3  23
#define  PANEL_GPIO_COM4  19
#define  PANEL_GPIO_COM5  255
#define  PANEL_GPIO_SCAN  18
#define  PANEL_GPIO_SEG0  37
#define  PANEL_GPIO_SEG1  25
#define  PANELRIORITY   220 
#define  UARTINDEX 	2
#define  BAUD 		9600
#define	 QAMPRIORITY    159
#define  QAMRESETPINNUM	0xff
#define  EPRINDEX 	I2C_3
#define  ERPDEVADD	0xa0
#define  ERPCHIPTYPE    AT24C64
#define  FLSCOUNT       1
#define  FLSINDEX	0
#define  FLSBASEADD	0X1E000000
#define	 FLSBUFFSIZE    0x2000000
#define	 FLSGPIO        13

#define  AUD_OUTPUT_HDMI  1
#define  AUD_OUTPUT_RCA  1
#define  AUD_OUTPUT_RF  1
#define  AUD_OUTPUT_SCART  0
#define  AUD_OUTPUT_SPDIF  1
#define  COLOR  3				//颜色模式，0：ARGB1555；1：ARGB4444；2：RGB888；3；ARGB8888
#define  EPR_DEV_ADDR  0xa0
#define  EPR_I2C_INDEX  3
#define  EPR_TYPE  6    //6-AT24C64
#define  FLASH0_BASEADDR  0x1E000000
#define  FLASH0_SIZE  0x2000000
#define  FLASH_COUNT  1
#define  FLASH_MAX_BLOCK_SIZE  0x10000
#define  FLASH_TEST_BASE  0x1FE00000
#define  FLASH_TEST_SIZE  0x160000
#define  FLASH_WP_GPIO  13
#define  FREQ_CNT  1			//频点总数
#define  GPIO_READ_INDEX  57		//测试读操作的GPIO
#define  GPIO_STAND_VALID_INDEX  3	//标准有效的GPIO
#define  GPIO_WRITE_INDEX  57 		//测试写操作的GPIO（可采用方便测试电压的引脚）
#define  I2C_EXIST_SLAVE_ADDRESS  0x38
#define  I2C_INDEX  2
#define  IR_PROTOCOL  0 
#define  PANEL_GPIO_COM0  21
#define  PANEL_GPIO_COM1  24
#define  PANEL_GPIO_COM2  20
#define  PANEL_GPIO_COM3  23
#define  PANEL_GPIO_COM4  19
#define  PANEL_GPIO_COM5  255
#define  PANEL_GPIO_SCAN  18
#define  PANEL_GPIO_SEG0  37
#define  PANEL_GPIO_SEG1  25
#define  QAM_TS0_FREQ  307000 	//频点,KHZ
#define  QAM_TS0_MODE  3		//调制方式 1-Qam16；2-Qam32；3-Qam64；4-Qam128；5-Qam256
#define  QAM_TS0_SR  6875 		//符号率,KHZ
#define  QAM_TS0_TSFILE  Audio&Video Test_27Mbps_20070524.ts		//频点所播放的码流文件(特别注意，该项暂时在与测试代码大量耦合，固不能修改此项目)
#define  RECT  2				//颜色模式，0：Res_720_576；1：Res_720_480；2：Res_1280_720；3；Res_1920_1080
#define  VID_OUTPUT_CVBS  1
#define  VID_OUTPUT_DVO  0
#define  VID_OUTPUT_HDMI  1
#define  VID_OUTPUT_RF  1
#define  VID_OUTPUT_RGB  1
#define  VID_OUTPUT_SCART  0
#define  VID_OUTPUT_YC  1
#define  VID_OUTPUT_YUV  1
void      	udi_init();
void 		hdi_init();
#endif /* __UDI_INIT_H */ 


/* End of File */
