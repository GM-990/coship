/* ***** N2T008.H - Verifier - Decoder interface ***** */
/* All functions and data types in this file are intended to support
N2-T008 interface between CA2.0 verifier and DVB IRD core software */

#ifndef H_N3T008
#define H_N3T008

#ifndef H_NDSTYPES
#define H_NDSTYPES
typedef unsigned char               NDS_BYTE;
typedef unsigned short              NDS_STATUS;
typedef unsigned short              NDS_USHORT;
typedef unsigned long               NDS_ULONG;
typedef unsigned char               NDS_BOOLEAN;
#endif

#ifdef DSS_FORMAT_FOR_X_SERVICE_REF
typedef struct _X_SERVICE_REF {
   NDS_ULONG service_id;
   NDS_BYTE xponder_id;
   NDS_BYTE network_id;
} X_SERVICE_REF;
#else
typedef struct _X_SERVICE_REF {
   NDS_USHORT service_id;
   NDS_USHORT xponder_id;
   NDS_USHORT network_id;
} X_SERVICE_REF;
#endif

typedef struct _X_APP_ID {
   unsigned long application_id;
   unsigned long provider_id;
} X_APP_ID;

typedef struct _X_CA_DESCRIPTOR {
   NDS_USHORT ca_pid;
   NDS_BYTE cat_private_data_len;
   NDS_BYTE *cat_private_data;
} X_CA_DESCRIPTOR;

typedef struct _X_ES_DESCRIPTOR {
   NDS_USHORT es_pid;
   NDS_USHORT ecm_pid;
   NDS_BYTE es_private_data_len;
   NDS_BYTE *es_private_data;
} X_ES_DESCRIPTOR;

#ifndef H_N2T008

typedef enum {
   NDS_ADD,
   NDS_UPDATE,
   NDS_REMOVE,
   NDS_REMOVE_ALL,
   NDS_ENABLE,
   NDS_ENABLE_ALL,
   NDS_DISABLE,
   NDS_DISABLE_ALL
} STREAM_ACTION;

typedef enum {
   API_PRIORITY_IMMEDIATE,
   API_PRIORITY_HIGH,
   API_PRIORITY_NORMAL
} API_PRIORITY;

typedef enum {
   RUNTIME,           /* Runtime software */
   APPLICATION_TYPE1, /* Resident */
   APPLICATION_TYPE2, /* Autoexec, App certficates */
   APPLICATION_TYPE3,  /* Interactive app */
   RUNTIME_S64,	   /* Runtime software - fixed 64+8 byte signature */
   RUNTIME_S128	   /* Runtime software - fixed 128+8 byte signature */
}CHECK_REASON;

#endif

typedef enum {
   OP_PLAY_LIVE,
   OP_RECORD,
   OP_PLAY_BACK,
   OP_PREPARE_PLAY_LIVE,
   OP_PREPARE_RECORD,
   OP_PREPARE_PLAY_BACK
} STREAM_OPERATION;

typedef struct _X_FECM_KEY {
   NDS_USHORT fecm_id;
   X_SERVICE_REF service;
}X_FECM_KEY;

#define MAX_NUM_FECM_TABLE			10

#define DEFAULT_IRD_NUMBER			0xF0F0F0F0

#define CA_AREA_NVRAM				1
#define EMAIL_AREA_NVRAM			2
#define CHAINING_AREA_NVRAM         3   /* Added Raquel (ca2) */
#define IVG_AREA_NVRAM              4   /* Added Raquel (ca2) */
#define XTV_AREA_NVRAM              5

#define VALID_CW_MASK				0x01
#define VALID_TAPING_CW_MASK		0x02
#define VALID_DIG_TAPING_CW_MASK	0x04
#define VALID_RECORDING_CW_MASK	0x08

#define CLEAR_MODE_VIEWING_ALLOWED			0x01
#define CLEAR_MODE_VIEWING_NOT_ALLOWED		0x00

#define STANDBY_MODE_OFF 0
#define STANDBY_MODE_ON 1


/*#define WAIT_FOR_CWSTATUS			3*/

/* MODEM & CALLBACK functions and data */

/* Modem status byte flags */
#define CARRIER_PRESENT				0x01
#define PHONE_IS_IN_USE				0x02
#define PARITY_FAILURE				0x04
#define TIMEOUT_FAILURE				0x08
#define OVERRUN_ERROR				0x10
#define FRAMING_ERROR				0x20
#define OS_ERROR						0x40

#define ACCESS_MODEM					1
#define RELEASE_MODEM				0

#define ACCESS_FAILURE				1
#define ACCESS_TIMEOUT				2

/* Modem command codes */
#define DIAL_COMMAND					0
#define ONHOOK_COMMAND				1
#define RESET_COMMAND				2

/* Dial methods */
#define PULSE_DIAL_METHOD			0
#define TONE_DIAL_METHOD			1
#define DEFAULT_DIAL_METHOD			2 /* IRD uses a method defined by a setup */

/* Dial responses */
#define NO_CARRIER					3
#define INVALID_COMMAND				4
#define NO_DIALTONE					6
#define BUSY							7
#define NO_ANSWER						8
#define CONNECT						9
#define OS_FAILURE					0xFF

/* On-hook responses */
#define ONHOOK_SUCCESS				0
#define ONHOOK_FAILURE				1

/* Reset responses */
#define RESET_SUCCESS				0
#define RESET_FAILURE				1

/* Area for Software download functions and data definitions */

#define BYTE_WILDCARD		0xFF
#define SHORT_WILDCARD		0xFFFF

typedef struct {
   NDS_ULONG           sw_component_id;
   NDS_ULONG           sw_component_provider_id;
}X_SW_COMPONENT_ID;

typedef struct {
   NDS_BYTE            manufacturer_id;
   NDS_BYTE            model_type;
   NDS_BYTE            hardware_type;
}X_RX_TYPE;

/* any field in RX_IDENT may be used as wildcard */
typedef struct {
   X_RX_TYPE			receiver_type;
   NDS_BYTE            auxilary_version;
   NDS_USHORT          ird_core_version;
}X_RX_IDENTIFIER;

typedef struct {
   X_RX_IDENTIFIER	receiver_identifier;
   NDS_USHORT          new_sw_version;
   NDS_BYTE                download_type;
   NDS_BYTE                user_acceptance;
   NDS_BYTE                switch_condition;
   NDS_BYTE                high_priority;
   X_SW_COMPONENT_ID	component_id;
}X_SW_DOWNLOAD_EMM;

/* values for user_acceptance */
#define IMMEDIATE_DL            0
#define USER_ACCEPTANCE_DL      1

typedef struct {
   X_RX_IDENTIFIER	receiver_identifier;
   NDS_USHORT          new_sw_version;
   NDS_BYTE                switch_condition;
   X_SW_COMPONENT_ID	component_id;
}X_SW_SWITCH_EMM;

typedef struct {
   NDS_ULONG           component_id;
   NDS_ULONG           component_provider_id;
   NDS_USHORT      component_version;
   NDS_BYTE            component_type;
   NDS_BYTE            component_action;
}X_SW_COMPONENT_ITEM;

typedef struct {
   X_RX_IDENTIFIER	receiver_identifier;
   NDS_BYTE            action_type;
   NDS_BYTE            switch_condition;
   NDS_BYTE            user_acceptance;
   NDS_BYTE            num_components;
   X_SW_COMPONENT_ITEM	*items;
}X_DOWNLOAD_ACTION;

#define SIGNATURE_OK			0
#define SIGNATURE_FAIL		1
#define PARAMETER_FAIL		2

#define CA_DATA_SOURCE        0
#define CA_DATA_DESTINATION   1

#define CA_INVALID_HANDLE     1
#define CA_INVALID_REQUEST    2


NDS_STATUS X_InitVerifier (NDS_USHORT ird_software_version, NDS_ULONG *ver_version);
NDS_STATUS X_RunVerifier (void);
NDS_STATUS X_GetIrdSerialNumber (NDS_ULONG *ird_serial_number);
NDS_STATUS X_InitNvramArea (int nvram_area_size, NDS_USHORT nvram_area_type, NDS_USHORT *nvram_area_access_handle);
NDS_STATUS X_DataToIrd (int length, NDS_BYTE *data);
NDS_STATUS X_SetMaxNumComponents(int max_components);
NDS_STATUS X_VerAllocateRam (int ram_size, NDS_BYTE **ram_adr);
NDS_STATUS X_VerDeallocateRam (NDS_BYTE *ram_adr);
NDS_STATUS X_WriteNvramData(NDS_USHORT nvram_area_access_handle, int location, int length, NDS_BYTE *data);
NDS_STATUS X_ReadNvramData(NDS_USHORT nvram_area_access_handle, int location, int length, NDS_BYTE *data);
NDS_STATUS X_SetCasId (NDS_USHORT cas_id);
NDS_STATUS X_NewCATDescriptor(NDS_ULONG x_connection, X_CA_DESCRIPTOR *ca_descriptor);
NDS_STATUS X_CATInvalid (NDS_ULONG x_connection);
NDS_STATUS X_ServiceComponentInfo (NDS_ULONG x_connection, NDS_ULONG data_destination_handle,
                                   X_APP_ID app, X_SERVICE_REF service,
                                   NDS_ULONG stream_number, NDS_ULONG stream_type,
                                   STREAM_OPERATION operation,
                                   STREAM_ACTION action,
                                   X_ES_DESCRIPTOR *es_description);
NDS_STATUS X_CALoadFecm(NDS_ULONG x_connection, X_FECM_KEY fecm, API_PRIORITY api_apriority);
NDS_STATUS X_CAFreeFecm(NDS_ULONG x_connection, X_FECM_KEY fecm);
NDS_STATUS X_CAFecmArrived(NDS_ULONG x_connection, X_FECM_KEY fecm);
NDS_STATUS X_CAReadFecm(NDS_ULONG x_connection, X_FECM_KEY fecm,
                        NDS_ULONG stream_number, NDS_ULONG stream_type,
                        NDS_BYTE *fecm_buffer, NDS_USHORT *length);
NDS_STATUS X_SetCwStatus(NDS_ULONG x_connection,
                         X_SERVICE_REF service,
                         NDS_ULONG stream_number,
                         NDS_ULONG stream_type,
                         STREAM_OPERATION operation,
                         NDS_BYTE cw_status );

NDS_STATUS X_SetCciData(NDS_ULONG x_connection ,NDS_BYTE cci_byte);

NDS_STATUS X_GetHandleProperties (int request,
                                  NDS_ULONG data_handle,
                                  NDS_ULONG *flags);
NDS_STATUS X_SendApplicationEvent(X_APP_ID application, NDS_ULONG event_id, NDS_BYTE payload_length, NDS_BYTE *payload);
NDS_STATUS X_SetClearModePermission(NDS_BYTE clear_mode_status);
int X_CompareServiceRef(X_SERVICE_REF *, X_SERVICE_REF *);
NDS_STATUS X_SetStandbyStatus(NDS_BYTE standby_status);
NDS_STATUS GetModemStatus(void);
NDS_STATUS AccessModem(NDS_BYTE modem_access, NDS_USHORT timeout);
NDS_STATUS IssueModemCommand(NDS_BYTE *command_string);
/* GetModemString returns the same status byte as GetModemStatus */
NDS_STATUS GetModemString(NDS_BYTE *string, NDS_USHORT length, NDS_USHORT timeout, NDS_USHORT *action_len);
/* PutModemString returns the same status byte as GetModemStatus */
NDS_STATUS PutModemString(NDS_BYTE *string, NDS_USHORT length, NDS_USHORT *actual_length);
/* Callback function that informs the verifier that the carrier
was dropped when there wasn't any outstanding requests to modem.
After getting this callback the verifier shouldn't issue any
modem I/O requests */
NDS_STATUS CarrierDropped(void);
NDS_STATUS CarrierDroped(void);

/* Obtain additional data for the callback. This data will be transferred to
the ACC/SMS */
NDS_STATUS GetSmsCallbackData ( NDS_BYTE *data, int *data_length );

NDS_STATUS X_InstructSwDownload (X_SW_DOWNLOAD_EMM *start_download);
NDS_STATUS X_SwitchSwCode (X_SW_SWITCH_EMM *switch_param);
NDS_STATUS X_ProcessSoftwareDownloadAction (X_DOWNLOAD_ACTION *action);

NDS_STATUS X_GetIrdIdentifier (X_RX_IDENTIFIER *ird_ident);

NDS_STATUS X_CheckSwSignature(CHECK_REASON reason, NDS_BYTE *data, NDS_ULONG data_length,
                              NDS_BYTE *security_field);

/* Internal verifier functions area */
NDS_STATUS SendApplicationEventVer(X_APP_ID application, NDS_ULONG event_id, NDS_BYTE payload_length, NDS_BYTE *payload);

/* Note that X_DATETIME structure is specially create to be the same as
VER_DATE_TIME container in order to avoid complicated copying in the verifier. */
typedef	struct
{
  NDS_USHORT year;     /* starts from zero */
  NDS_USHORT month;    /* from 0 to 11 */
  NDS_USHORT day;      /* from 1 to 31 */
  NDS_USHORT hours;    /* from 0 to 23 */
  NDS_USHORT minutes;  /* from 0 to 59 */
  NDS_USHORT seconds;  /* from 0 to 59 */
} X_DATETIME;

NDS_STATUS X_GetCurrentGMTDateTime(X_DATETIME *dt );

/* Send a CA messages between different nodes (Udi) */
extern void SendRemoteCaMsg(NDS_BYTE *message, NDS_USHORT len);
void CaDebugPrintLine(unsigned char *text);

/* STB Control messages API */

#define STB_CONTROL_VALID_SIGNATURE_MASK  0x00000001

typedef int X_STB_CONTROL_MESSAGE_CLIENT (unsigned char *data, int data_length, int flags);

int X_RegisterSTBControlMessageClient (X_STB_CONTROL_MESSAGE_CLIENT *func);

typedef int X_SET_CARD_STATUS (unsigned char card_status);

int X_RegisterSetCardStatus (X_SET_CARD_STATUS *func);

#endif

