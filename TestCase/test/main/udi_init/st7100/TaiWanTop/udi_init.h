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
void udi_init();
void hdi_init();
#endif /* __UDI_INIT_H */ 


/* End of File */
