#ifndef CS_CLICMD_PORTING_H
#define CS_CLICMD_PORTING_H

#ifdef __cplusplus
extern "C" {
#endif 

void CSCLIRegisterCommands_SYSSM(void);

void CSCLIRegisterCommands_SCREEN(void);

void CSCLIRegisterCommands_DEMUX(void);

void CSCLIRegisterCommands_DMX(void);

void CSCLIRegisterCommands_TOOLSET(void);

void CSCLIRegisterCommands_CASECURE(void);

void CSCLIRegisterCommands_TUNER(void);

void CSCLIRegisterCommands_FLASH(void);

void CSCLIRegisterCommands_AUD(void);

void CSCLIRegisterCommands_IFRAME(void);

void CSCLIRegisterCommands_PLAY(void);

void CSCLIRegisterCommands_VID(void);

void CSCLIRegisterCommands_MEMORY(void);

void CSCLIRegisterCommands_I2C(void);

void CSCLIRegisterCommands_GPIO(void);

void CSCLIRegisterCommands_TABLE(void);

/* function for register all porting CLI command */
#define CSCLIRegisterCommands_AllPorting() do { \
	CSCLIRegisterCommands_SYSSM(); \
	CSCLIRegisterCommands_SCREEN(); \
	CSCLIRegisterCommands_DEMUX(); \
	CSCLIRegisterCommands_DMX(); \
	CSCLIRegisterCommands_TOOLSET(); \
	CSCLIRegisterCommands_CASECURE(); \
	CSCLIRegisterCommands_TUNER(); \
	CSCLIRegisterCommands_FLASH(); \
	CSCLIRegisterCommands_AUD(); \
	CSCLIRegisterCommands_IFRAME(); \
	CSCLIRegisterCommands_PLAY(); \
	CSCLIRegisterCommands_VID(); \
	CSCLIRegisterCommands_MEMORY(); \
	CSCLIRegisterCommands_I2C(); \
	CSCLIRegisterCommands_GPIO(); \
	CSCLIRegisterCommands_TABLE(); \
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* !CS_CLICMD_PORTING_H */

/* end of file */
