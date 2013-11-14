/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef CS_CLICMD_PORTING_H
#define CS_CLICMD_PORTING_H

#ifdef __cplusplus
extern "C" {
#endif 

void CSCLIRegisterCommands_TOOLSET(void);

void CSCLIRegisterCommands_IFRAME(void);

void CSCLIRegisterCommands_MEM(void);

void CSCLIRegisterCommands_SMC(void);

void CSCLIRegisterCommands_FLASH(void);

void CSCLIRegisterCommands_GRAPHICS(void);

void CSCLIRegisterCommands_SCREEN(void);

void CSCLIRegisterCommands_PLAY(void);

void CSCLIRegisterCommands_PCMPLAY(void);

void CSCLIRegisterCommands_TUNER(void);

void CSCLIRegisterCommands_AUDIO(void);

void CSCLIRegisterCommands_CASECURE(void);

void CSCLIRegisterCommands_REC(void);

void CSCLIRegisterCommands_PANEL(void);

void CSCLIRegisterCommands_HDMI(void);

void CSCLIRegisterCommands_VIDEO(void);

void CSCLIRegisterCommands_DEMUX(void);

void CSCLIRegisterCommands_REG(void);

void CSCLIRegisterCommands_I2C(void);

void CSCLIRegisterCommands_TABLE(void);

void CSCLIRegisterCommands_GPIO(void);

/* function for register all porting CLI command */
#define CSCLIRegisterCommands_AllPorting() do { \
	CSCLIRegisterCommands_TOOLSET(); \
	CSCLIRegisterCommands_IFRAME(); \
	CSCLIRegisterCommands_MEM(); \
	CSCLIRegisterCommands_SMC(); \
	CSCLIRegisterCommands_FLASH(); \
	CSCLIRegisterCommands_GRAPHICS(); \
	CSCLIRegisterCommands_SCREEN(); \
	CSCLIRegisterCommands_PLAY(); \
	CSCLIRegisterCommands_PCMPLAY(); \
	CSCLIRegisterCommands_TUNER(); \
	CSCLIRegisterCommands_AUDIO(); \
	CSCLIRegisterCommands_CASECURE(); \
	CSCLIRegisterCommands_REC(); \
	CSCLIRegisterCommands_PANEL(); \
	CSCLIRegisterCommands_HDMI(); \
	CSCLIRegisterCommands_VIDEO(); \
	CSCLIRegisterCommands_DEMUX(); \
	CSCLIRegisterCommands_REG(); \
	CSCLIRegisterCommands_I2C(); \
	CSCLIRegisterCommands_TABLE(); \
	CSCLIRegisterCommands_GPIO(); \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* !CS_CLICMD_PORTING_H */

/* end of file */
