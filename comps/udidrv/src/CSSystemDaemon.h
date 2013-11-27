
#define CS_SYSTEM_DAEMON_PATH  "/etc/CSSystemDaemon.conf"
#define CS_SYSTEM_DAEMON_SEM_ID 0xFF
#define CS_SYSTEM_DAEMON_SHMEM_ID 0xFE
#define CS_SYSTEM_DAEMON_ACCESS_MODE 0600 /*°Ë½øÖÆ*/
#define CS_SYSTEM_DAEMON_CMD_SIZE 500

enum
{
	EM_SYSTEM_DAEMON_SEM_CMD_END = 0,
	EM_SYSTEM_DAEMON_SEM_CMD_START,
	EM_SYSTEM_DAEMON_SEM_PROXY_RUN,
	EM_SYSTEM_DAEMON_SEM_COUNT
}CSSystemDaemonSem_E;





typedef struct _CSSystemDaemonCmd_S
{
	int nRet;
	char szCmd[CS_SYSTEM_DAEMON_CMD_SIZE];
}CSSystemDaemonCmd_S;

typedef union _CSLinuxSem_U {
	int              nVal;    /* Value for SETVAL */
	struct semid_ds *psSemDs;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  * pwArray;  /* Array for GETALL, SETALL */
	struct seminfo  *psSemInfo;  /* Buffer for IPC_INFO  (Linux specific) */
}CSSystemDaemonLinuxSem_U;


