/****************************************************************************/
/* N3T009.h                                                                 */
/* --------                                                                 */
/* Conditional Access API                                                   */
/* All functions and data types in this file are intended to support        */
/* N3-T009 interface between Jupiter Verifier and Applications.             */
/*                                                                          */
/****************************************************************************/


#ifndef H_N3T009
#define H_N3T009

#ifndef H_NDSTYPES
#define H_NDSTYPES
typedef unsigned char               NDS_BYTE;
typedef unsigned short              NDS_STATUS;
typedef unsigned short              NDS_USHORT;
typedef unsigned long               NDS_ULONG;
typedef unsigned char               NDS_BOOLEAN;
#endif


/********************************************************************************/
/*                            DEFINITION OF EVENTS                              */
/********************************************************************************/
/* The event_ids defined below are not the real event id value.
   The real event id value is the value specified here + the base CA event value.
   The base CA event value needs to be coordinated with the application at integration stage. */

#define CA_EV_CARD_STATUS                        0
#define CA_EV_CA_MODULE_CONFIG                   1
#define CA_EV_USER_PARAMETER_UPDATE              2
#define CA_EV_DISPLAY_MSG                        3
#define CA_EV_CHANGE_IN_EMAIL_STATUS             4
#define CA_EV_CA_EVENT_STATUS                    5
#define CA_EV_PPV_CONFIGURATION                  6
#define CA_EV_NUMBER_OF_PURCHASE_OPTIONS         7
#define CA_EV_PURCHASE_OPTION_DATA               8
#define CA_EV_PURCHASE_ACTION_FAILURE            9
#define CA_EV_STORE_IPPV_RECORD                  10
#define CA_EV_CARD_SLOT_UPDATE                   11
#define CA_EV_CALLBACK_STATUS                    12
#define CA_EV_CHAINING_STATUS                    13
#define CA_EV_USER_PARAMETER_CERTIFICATE         14
#define CA_EV_RAW_DATA_TO_EPG                    15
#define CA_EV_DISPLAY_FINGERPRINT				 16 /*Added - David Mor 21.6.2004, for Overt FP*/
#define CA_EV_PURCHASE_PACKET_DATA               17

/********************************************************************************/
/*                      VERIFIER API VERSION ACCESS INTERFACE                   */
/********************************************************************************/
NDS_STATUS CaApi_GetCaApiVersion( unsigned short *ca_api_version);

/********************************************************************************/
/*                      EVENT INFORMATION ACCESS INTERFACE                      */
/********************************************************************************/

/* Requests the CA Event Payload Data Length. */
NDS_STATUS  CaApi_GetCaEventPayloadLength (unsigned long event_id, unsigned long payload_handle,
                                           int *data_length);
/* Requests the CA Event Payload Data. */
NDS_STATUS  CaApi_GetCaEventPayload (unsigned long event_id, unsigned long payload_handle,
                                     int buf_size, int *data_length, unsigned char *data_buffer);

/********************************************************************************/
/*                      Verifier Configuration and status                       */
/********************************************************************************/

/* N3T009 API version */
#define N3_T009_VERSION     0x01
#define N3_T009_DOC_VERSION 'A'

/* card_type - Possible values: */
#define UNKNOWN_CARD        0
#define VIEWING_CARD        1
#define OLD_VIEWING_CARD    2
#define INTERACTIVE_CARD    3
#define AUXILIARY_CARD      4

/* card_status - Possible values: */
#define CARD_REMOVED        0    /* Card removed */
#define CARD_READY          1    /* Card ready: the smart card is ready for operation */
#define CARD_FAILURE        2    /* Card failure: the card does not answer at all */
#define CARD_INVALID        3    /* Card invalid: the inserted card is not a viewing card */
#define CARD_CLOSED         4    /* Card failed on the authentication */
#define CARD_TRANSITION     5    /* New smart card in preparation stage */
#define CARD_INSERTED       6    /* The viewing card in inserted and it gets prepared to work */


/*finger_print_descriptor*/

typedef struct {
	unsigned char   descriptor_tag;
	unsigned char   descriptor_length;
	unsigned char char_size;
	unsigned char char_color;
	unsigned char reg_hight[2];
	unsigned char reg_width[2];
	unsigned char reg_color;
	unsigned char	X_Reg_Offset[2];
	unsigned char	Y_Reg_Offset[2];
	unsigned char	X_Text_Offset[2];
	unsigned char	Y_Text_Offset[2];
	unsigned char	duration[2];
	unsigned char text_length;
	unsigned char fs_text[9];
} OVERT_FINGERPRINT_DESCRIPTOR;

#ifndef MAX_ENTITLEMENT_INFO_BYTES
#define MAX_ENTITLEMENT_INFO_BYTES 4
#endif
#ifndef MAX_XTV_INFO_BYTES
#define MAX_XTV_INFO_BYTES         6
#endif

/* Additional Entitlement Info Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   num_of_entitlement_info_bytes;
   unsigned char   entitlement_info[MAX_ENTITLEMENT_INFO_BYTES];
   unsigned char   num_of_xtv_info_bytes;
   unsigned char   xtv_info[MAX_XTV_INFO_BYTES];
} ADDITIONAL_ENTITLEMENT_INFO_DESCRIPTOR;

/* IPPV Slot Extension Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   additional_entitlement_info[MAX_ENTITLEMENT_INFO_BYTES];
} IPPV_SLOT_EXTENSION_DESCRIPTOR;

/* OPPV Slot Extension Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   additional_entitlement_info[MAX_ENTITLEMENT_INFO_BYTES];
} OPPV_SLOT_EXTENSION_DESCRIPTOR;

/* Card Status Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   card_type;
   unsigned char   card_status ;
} CARD_STATUS_DESCRIPTOR;

/* Viewing Card Config Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   card_id[4];
   unsigned char   subscriber_id[4];
   unsigned char   paired;
   unsigned char   card_id_check_digit;
   unsigned char   card_authorized;
} VIEWING_CARD_CONFIG_DESCRIPTOR;
/* a typo too entrenched to just change */
#define VEIWING_CARD_CONFIG_DESCRIPTOR VIEWING_CARD_CONFIG_DESCRIPTOR

/* System Config Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_system_id[2];
   unsigned char   si_filtering[2];
} SYSTEM_CONFIG_DESCRIPTOR;

/* Verifier Config Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_api_version[2];
} VERIFIER_CONFIG_DESCRIPTOR;

/* Subscriber Config Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   region_code;
   unsigned char   country_code[3];
   unsigned char   currency[2];
   unsigned char   zipcode[8];
} SUBSCRIBER_CONFIG_DESCRIPTOR;

/* Requests region bytes data from the card. */
NDS_STATUS  CaApi_ReadRegionBytes (unsigned char action, int region_block, int *number_of_region_blocks,
                                   int *region_block_length, unsigned char *region_data);
/* Sets the subscriber-preferred language for OSDs and for Email reception. */
NDS_STATUS  CaApi_SetCurrentLanguage (unsigned char ca_ird_language);
/* Gets the current language for OSDs and for Email reception. */
NDS_STATUS  CaApi_GetCurrentLanguage (unsigned char *ca_ird_language);
/* Sets the subscriber-preferred customer ID for default settings. */
NDS_STATUS  CaApi_SetCustomerId (int customer_id);

#define GET_NUMBER_AND_LENGTH_OF_REGION_BLOCKS  1
#define GET_REGION_BLOCK_DATA                   2


/********************************************************************************/
/*                            Raw Data                                          */
/********************************************************************************/

typedef struct
{
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    data;
} CA_EV_RAW_DATA_TO_EPG_DESCRIPTOR;

/********************************************************************************/
/*                            User Parameters                                   */
/********************************************************************************/

#define USER_PARAMETERS_READY             0
#define USER_PARAMETERS_UPDATE            1

#define PERMANENT_UNLOCK_ALLOWED          0x01
#define TEMPORARY_UNLOCK_ALLOWED          0x02
#define PIN_ALWAYS_REQUIRED               0x04

typedef struct
{
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    limit_id;
   unsigned char    limit_type;
   unsigned char    value_len;
   unsigned char    description_len;
}LIMIT_PROPERTIES_DESCRIPTOR;

typedef struct
{
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    user_pin[4];
}USER_PIN_DESCRIPTOR;

/* Definition of Limits Types */
#define RANGE_T      1
#define BITMAP_T     2
#define VALUE_T      3

#define RATING_LIMIT_ID                   1
#define SPENDING_LIMIT_ID                 2
#define CUMULATIVE_SPENDING_LIMIT_ID      3
#define CUMULATIVE_BALANCE_LIMIT_ID       4
#define WATERSHED_LIMIT_ID                5

/* To set or change the PIN number for particular user. */
NDS_STATUS  CaApi_SetUserPin (unsigned char user_id, unsigned char *old_pin, unsigned char *master_pin,
                              unsigned char *new_pin);
/* To notify the Verifier of a change in the active user or to exit particular user mode. */
NDS_STATUS  CaApi_SetCurrentUser (unsigned char user_id,  unsigned char *user_pin);
/* To retrieve current number of user limits. */
NDS_STATUS  CaApi_GetUserLimitConfig (int *num_users, int *num_limits, int *limit_properties_desc_len);
/* To retrieve the configuration of a user limit from the CA module. */
NDS_STATUS  CaApi_GetLimitProperties (int limit_block, unsigned char requested_limit_id, unsigned char *limit);
/* To retrieve subscriber parameters from the CA module. */
NDS_STATUS  CaApi_GetLimitValue (unsigned char user_id, unsigned char *user_pin, unsigned char *master_pin, unsigned char limit_id, unsigned char value_len, void *limit_value);
/* To retrieve the description of a user limit parameter from the CA module. */
NDS_STATUS  CaApi_GetLimitDescription (unsigned char limit_id, unsigned char description_len, void *description);
/* Called by Application to set subscriber parameters for the CA module. */
NDS_STATUS  CaApi_SetLimitValue (unsigned char user_id, unsigned char *master_pin, unsigned char limit_id, unsigned char value_len, void *limit_value);
/* Called by the Application to recieve current lock and PIN code status */
NDS_STATUS  CaApi_GetLockStatus (unsigned char *lock_mode, unsigned char *pin_status);
/* Called by the Application to modify current Locking mode. */
NDS_STATUS  CaApi_SetLockMode (unsigned char *master_pin, unsigned char lock_mode);
/* To activate or deactivate temporary override of the limits-locked state. */
NDS_STATUS  CaApi_SetTemporaryUnlock (unsigned char *master_pin, unsigned char temp_unlock);
/* To set a safety preferences to the CA module. */
NDS_STATUS  CaApi_SetSafetyPreferences(unsigned short flags, unsigned char num_pins, unsigned short freeze_time);
/* To get the number of PIN entry code attempts remaining */
NDS_STATUS  CaApi_GetNumPinTriesLeft(unsigned char *num_pin);
/* To create PIN certificate descriptor */
NDS_STATUS  CaApi_CreateUserParameterCertificate (unsigned char *master_pin,
                                                  unsigned char *request_certificate_desc,
                                                  unsigned char *certificate_desc);

/* USER PARAMETER UPDATE Descriptor */
typedef struct {
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    user_id;
   unsigned char    user_parameter_status;
} USER_PARAMETER_UPDATE_DESCRIPTOR;

/* PIN CODE CERTIFICATE Descriptor */
typedef struct {
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    start_date[12];
   unsigned char    expiration_date[12];
   unsigned char    flags[1];
   unsigned char    signature[8];
} PIN_CODE_CERTIFICATE_DESCRIPTOR;


/* PIN CODE CERTIFICATE REQUEST Descriptor */
typedef struct {
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    start_date[12];
   unsigned char    expiration_date[12];
   unsigned char    flags[1];
} PIN_CODE_CERTIFICATE_REQUEST_DESCRIPTOR;

/********************************************************************************/
/*                            Exception and OSD Message                         */
/********************************************************************************/

#define CA_IDLE_EXCEPTION           0
#define CA_IDLE_PARAM               0

#define CA_SHOW_INFO_OSD            1
#define CA_SHOW_GENERAL_OSD         2
#define CA_SHOW_LIMIT_EXCEEDED      3

#define DSS_CA_OSD_TEXT_PRESENT     0xFF
#define CA_OSD_TEXT_PRESENT         0xFD

#define ECM_OSD 0
#define EMM_OSD 1
#define VER_OSD 2

typedef struct  {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   exception_code;
   unsigned char   exception_msg_number;
   unsigned char   exception_msg_language;
   unsigned char   exception_text_length;
   unsigned char   exception_text;
} EXCEPTION_DESCRIPTOR;

/* Exception Extra Descriptor */
typedef struct  {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   exception_source;
   unsigned char   priority;
} EXCEPTION_EXTRA_DATA_DESCRIPTOR;

/* OSD Msg Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   msg_number;
   unsigned char   msg_language;
   unsigned char   message_handle[4];
   unsigned char   msg_text_length;
   unsigned char   message_text;
} OSD_MSG_DESCRIPTOR;

/* OSD Extra Data Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   data_source[4];
   unsigned char   OSD_source;
   unsigned char   priority;
} OSD_EXTRA_DATA_DESCRIPTOR;

/* To notify the CA module that the message was already displayed */
NDS_STATUS CaApi_CancelMessage (unsigned long message_handle);


/********************************************************************************/
/*                            PPV                                               */
/********************************************************************************/

/* Purchase Actions Constants Definition */
#define PREPARE_CA_DATA                   0
#define GET_NUMBER_OF_PURCHASE_OPTIONS    1
#define GET_PURCHASE_OPTION_DATA          2
#define PURCHASE_VIEWING_ONLY             3
#define PURCHASE_VIEWING_AND_TAPING       4
#define GET_PURCHASE_PACKET_DATA          5

/* Hold_Info used in CaApi_FreezePurchaseOffer - constant definition */
#define HOLD_PURCHASE_INFO     1
#define RELEASE_PURCHASE_INFO  0

/* Entitlement Type Constants Definition */
#define SUBSCRIPTION                1
#define PPV                         2
#define PPE                         3
#define PPT                         4
#define PPCW                        5
#define XTV_RENT                    6
#define FREE_PREVIEW                7
#define INDIVIDUAL_FREE_PREVIEW     8
#define OPPV                        9
#define ISU                         10
#define ENTITLEMENT_TYPE_NOT_AVAILABLE    0xFF

/* Slot Type Constants Definition */
#define SLOT_SUBSCRIPTION           1
#define SLOT_IPPV                   2
#define SLOT_IFP                    3
#define SLOT_ACCOUNT                4
#define SLOT_OPPV_EXTENSION         5
#define SLOT_IPPV_EXTENSION         6
#define SLOT_TYPE_NOT_AVAILABLE     0xFF

/* Cost Type Constants Definition */
#define VIEW_ONLY                   0x01
#define VIEW_AND_TAPE               0x02

/* Cancel/Close (overwrite slot) action Definition */
#define CLOSE_CANCEL                0
#define CLOSE_OVERWRITE             1

/* APP_ID DESCRIPTOR */
typedef struct _APP_ID_DESCRIPTOR {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   application_id[4];
   unsigned char   provider_id[4];
} APP_ID_DESCRIPTOR;

/* FECM_KEY Descriptor */
typedef struct _FECM_KEY_DESCRIPTOR {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   fecm_id[4];
   unsigned char   auxiliary_filter_info[4];
} FECM_KEY_DESCRIPTOR;

/* PPV CONFIG DESCRIPTOR */
typedef struct {
   unsigned char descriptor_tag;
   unsigned char descriptor_length;
   unsigned char number_of_ippv_records[2];
   unsigned char ippv_record_length[2];
   unsigned char number_of_history_records[2];
   unsigned char history_record_length[2];
   unsigned char number_of_subscription_records[2];
   unsigned char subscription_record_length[2];
   unsigned char number_of_ifp_records[2];
   unsigned char ifp_record_length[2];
   unsigned char number_of_account_slots[2];
   unsigned char account_slot_length[2];
   unsigned char features_supported;
} PPV_CONFIG_DESCRIPTOR;

/* DATA_SOURCE DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   x_connection[4];
} DATA_SOURCE_DESCRIPTOR;

/* SERVICE DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   service_identifier[16];
} SERVICE_IDENTIFIER_DESCRIPTOR;

/* STREAM_REFERENCE DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   stream_num[4];
   unsigned char   stream_type[4];
} STREAM_REFERENCE_DESCRIPTOR;

/* STREAM_OPERATION_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
    unsigned char   stream_operation[4];
} STREAM_OPERATION_DESCRIPTOR;

/* PROGRAM_INFORMATION_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   type;
   unsigned char   length;
   unsigned char   operational_fields;
} PROGRAM_INFORMATION_DESCRIPTOR;

/* CA EVENT STATUS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_condition_identifier[4];
   unsigned char   cw_status ;
   unsigned char   extra_authorizations;
   unsigned char   purchase_info_byte;
   unsigned char   ca_service_id[4];
} CA_EVENT_STATUS_DESCRIPTOR;

/* REASON FOR CW DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   entitlement_type;
   unsigned char   slot_type;
   unsigned char   slot_number[2];
} REASON_FOR_CW_DESCRIPTOR;

/* VIEWING PROBLEMS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_module_status ;
   unsigned char   number_of_viewing_problem_bytes;
   unsigned char   data[20];
} VIEWING_PROBLEMS_DESCRIPTOR;

/* SI INFO DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   si_sync_token[4];
   unsigned char   promo_id[2];
   unsigned char   conditional_access_info;
   unsigned char   conditional_access_info_mask;
} SI_INFO_DESCRIPTOR;

/* TIME_BLACKOUT_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   end_of_blackout[12];
} TIME_BLACKOUT_DESCRIPTOR;

/* CCI_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   cci_byte;
} CCI_DESCRIPTOR;

/* RATING_LEVEL_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   rating;
} RATING_LEVEL_DESCRIPTOR;

/* NUMBER_OF_PURCHASE_OPTIONS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_module_status;
   unsigned char   purchase_problem_byte_0;
   unsigned char   number_of_options;
} NUMBER_OF_PURCHASE_OPTIONS_DESCRIPTOR;

/* PURCHASE OPTION NUMBER DESCRIPTOR */
typedef struct
{
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   purchase_option_number;
}PURCHASE_OPTION_NUMBER_DESCRIPTOR;

/* PURCHASE_OPTION_DATA DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   entitlement_type;
   unsigned char   available_payment_methods[2];
   unsigned char   expiration_date[12]; /* see DATETIME Format */
   unsigned char   ca_service_id[4];
   unsigned char   account_slot_id[2];
} PURCHASE_OPTION_DATA_DESCRIPTOR;

/* PURCHASE PROBLEMS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_module_status;
   unsigned char   ca_status;
   unsigned char   number_of_purchase_problem_bytes;
   unsigned char   data[20];
} PURCHASE_PROBLEMS_DESCRIPTOR;

/* COST DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   cost_type;
   unsigned char   cost[4];
   unsigned char   currency[2];
   unsigned char   cost_problem;
   unsigned char   number_of_limit_ids;
   unsigned char   data[20];
} COST_DESCRIPTOR;

/* PURCHASE_PACKET_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   purchase_packet_data;
} PURCHASE_PACKET_DESCRIPTOR;

/* NOTIFICATION DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   threshold_exceeded;
   unsigned char   ippv_slot_table_almost_full;
} NOTIFICATION_DESCRIPTOR;

/* RENT_OPTION_DATA_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   rent_duration[4];
} RENT_OPTION_DATA_DESCRIPTOR;

/* BONUS POINTS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   cost[4];
   unsigned char   reward[4];
   unsigned char   cost_problem;
} BONUS_POINTS_DESCRIPTOR;

/* MAXIMUM_EXTENSION_DATE_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   maximum_extension_date[12];
} MAXIMUM_EXTENSION_DATE_DESCRIPTOR;

/* FINAL_EXPIRY_DATE_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   final_expiry_date[12];
} FINAL_EXPIRY_DATE_DESCRIPTOR;

/* CANCEL ACTION DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   close_action;
} CANCEL_ACTION_DESCRIPTOR;

/* IPPV RECORD HANDLE DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   slot_number[2];
   unsigned char   ca_service_id[4];
} IPPV_RECORD_HANDLE_DESCRIPTOR;

/* EXTENDED IPPV RECORD HANDLE DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   IPPV_RECORD_HANDLE_DESCRIPTOR irhd;
   CANCEL_ACTION_DESCRIPTOR      cad;
} EXTENDED_IPPV_RECORD_HANDLE_DESCRIPTOR;

/* METADATA DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   metadata_data;        /* Length of metadata data
                                            is from 0 till descriptor_length */
} METADATA_DESCRIPTOR;

/* Format of DATETIME */
typedef struct {
   unsigned char   year[2];    /* starts from zero */
   unsigned char   month[2];   /* from 0 to 11 */
   unsigned char   day[2];     /* from 0 to 30 */
   unsigned char   hours[2];   /* from 0 to 23 */
   unsigned char   minutes[2]; /* from 0 to 59 */
   unsigned char   seconds[2]; /* from 0 to 59 */
} DATETIME;

/* COST LIMIT PROBLEM DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   cost_problem;
   unsigned char   number_of_limit_ids;
   unsigned char   data[20];
} COST_LIMIT_PROBLEM_DESCRIPTOR;

/* IPPV_SLOT_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_service_id[4];
   unsigned char   cost[4];
   unsigned char   entitlement_type;
   unsigned char   payment_method;
   unsigned char   account_slot_id[2];
   unsigned char   expiration_date[12]; /* see DATETIME Format */
   unsigned char   purchase_date[12];
   unsigned char   cancellation_date[12];
   unsigned char   viewed;
   unsigned char   cancelled;
   unsigned char   reported;
   unsigned char   analog_taping;
   unsigned char   digital_taping;
   unsigned char   maximum_extension_date[12];
} IPPV_SLOT_DESCRIPTOR;

/* SUBSCRIPTION_SLOT_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   ca_service_id[4];
   unsigned char   expiration_date[12]; /* see DATETIME Format */
   unsigned char   oppv_flag; /* =1 - OPPV, =0 - Subscription */
   unsigned char   analog_taping;
   unsigned char   digital_taping;
} SUBSCRIPTION_SLOT_DESCRIPTOR;


/* ACCOUNT_SLOT_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   account_slot_id[2];
   unsigned char   currency_id[2];
   unsigned char   decimal_point_place[2]; /* 0xFFFF -ignore field */
   unsigned char   account_limit[4];
   unsigned char   account_balance[4];
   unsigned char   bonus_points_balance[4]; /* 0xFFFFFFFF - not in use */
}ACCOUNT_SLOT_DESCRIPTOR;

/* CARD SLOT UPDATE DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   slot_type;
   unsigned char   slot_number[2];
}CARD_SLOT_UPDATE_DESCRIPTOR;

#define PURCHASE_VIA_SMS_SLOT          0
#define PURCHASE_VIA_CREDIT_CARD       1
#define PURCHASE_VIA_BONUS_POINTS      2

/* PURCHASE_ACTION_REQUEST_ID_DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   purchase_action_request_tag[4];
} PURCHASE_ACTION_REQUEST_ID_DESCRIPTOR;

/* PURCHASE_INFO DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   action;
   unsigned char   purchase_option_number;
   unsigned char   payment_method;
   unsigned char   purchase_action_info;
   unsigned char   purchase_info_request_tag[4];
} PURCHASE_INFO_DESCRIPTOR;


/* Requests the CA_EV_CA_STATUS enumeration and re-send. */
NDS_STATUS  CaApi_EnumerateCaEventStatus (unsigned char *app_id, int *num_events);
/* To request the CA module to hold all necessary data to proceed with last Purchase offer. */
NDS_STATUS  CaApi_FreezePurchaseOffer (unsigned char *data_source, unsigned char *service,
                                       unsigned char *stream_reference, unsigned char hold_info);
/* Make purchase action */
NDS_STATUS  CaApi_PurchaseAction (unsigned char *app_id, unsigned char *data_source, unsigned char *service,
                                  unsigned char *stream_reference, unsigned char *purchase_info,
                                  unsigned char *fecm_key, unsigned char *fecm, unsigned char *auxiliary_data);
/* Used to cancel a IPPV purchase. */
NDS_STATUS  CaApi_CancelPurchase (unsigned char *card_id, unsigned char *ippv_record_handle_descriptor);
/* Called by Application to read and store all IPPV records. */
NDS_STATUS  CaApi_ReadCardSlot (int slot_number, unsigned char slot_type, unsigned char *slot);
/* To specify if warning is to be given or not prior to purchase consummation.*/
NDS_STATUS  CaApi_SetIppvViewedWarning(unsigned char *data_source, unsigned char *service, unsigned char action);
/* To store a history data to a specific history slot. */
NDS_STATUS  CaApi_StoreHistoryRecord (int history_slot_number, void *data);
/* To retrieve a history data from a specific history slot. */
NDS_STATUS  CaApi_GetHistoryRecord (int history_slot_number, void *data);


/********************************************************************************/
/*                    Production and Maintenance                                */
/********************************************************************************/

/* Retrieves CA data for the diagnostic screen. */
NDS_STATUS  CaApi_GetCaDiagData( unsigned char *diag_data, unsigned short *card_sw_version);


/********************************************************************************/
/*                               Email                                          */
/********************************************************************************/

/* EMAIL_STATUS DESCRIPTOR */
typedef struct {
   unsigned char descriptor_tag;
   unsigned char descriptor_length;
   unsigned char num_unread_email[2];
   unsigned char total_num_email[2];
} EMAIL_STATUS_DESCRIPTOR;

/* EMAIL RECORD DESCRIPTOR */
typedef struct {
   unsigned char  descriptor_tag;
   unsigned char  descriptor_length;
   unsigned char  email_id[2];
   unsigned char  deletion_date[12];
   unsigned char  transmission_date[12];
   unsigned char  total_text_length[4];
   unsigned char  parameters[2];
   unsigned char  customer_params[2];
   unsigned char  compression_dictionary;
   unsigned char  language;
} EMAIL_RECORD_DESCRIPTOR;

#define LOCK_ALL  1
#define LOCK_ONE  2

/* Called by Application when opening Email message menu */
NDS_STATUS  CaApi_LockEmailDB (int action, unsigned long lock_parameter, unsigned long *request_handle);
/* Called by Application when Email interaction with subscriber is complete. */
NDS_STATUS  CaApi_UnlockEmailDB (unsigned long request_handle);
/* Checks number of available Email messages. */
NDS_STATUS CaApi_GetNumEmailMsg (int request_type, unsigned char *num_unread_email,
                                 unsigned char *total_num_emails, unsigned long request_handle);
/* To retrieve an Email message (only control information)*/
NDS_STATUS CaApi_GetEmailProperties (unsigned long request_handle, int message_index,
                                     char *email_record_size, char *email_record);
/* To retrieve an Email message (only text) */
NDS_STATUS CaApi_GetEmailText(unsigned long request_handle, unsigned long master_pin,
                              unsigned short email_id, unsigned long text_offset,
                              unsigned long text_length, void *email_text);
/* To delete an Email message. */
NDS_STATUS  CaApi_DeleteEmail (unsigned short email_id);
/* Requests that a specific email message be marked as having been read */
NDS_STATUS  CaApi_MarkEmailAsRead (unsigned short email_id);


/********************************************************************************/
/*                               Callback                                       */
/********************************************************************************/

/* Triggers a callback of the specified type. */
NDS_STATUS  CaApi_DoCallback (unsigned long request_tag, unsigned char callback_type, int data_length, void *data);
/* Requests the status of a previously-triggered callback. */
NDS_STATUS  CaApi_GetCallbackRequestStatus (unsigned long request_tag, unsigned char *cb_status);
/* Terminates a previously-triggered callback */
NDS_STATUS  CaApi_TerminateCallbackRequest (unsigned long request_tag);
/* Determines whether the CA module is capable of perfoming callback */
NDS_STATUS  CaApi_GetCallbackAbilityStatus (unsigned char *cb_status);


/* Callback status descriptor status field values */
#define CALLBACK_SUCCEEDED 0
#define CALLBACK_FAILED 1
#define VERIFIER_CALLBACK_SUCCEEDED 2

/* Callback status descriptor request tag field special value */
#define VERIFIER_REQUEST_TAG 0xffffffff

/* CALLBACK_STATUS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   request_tag[4];
   unsigned char   status;         /* = 0 Callback Succeeded,  = 1 Callback failed */
} CALLBACK_STATUS_DESCRIPTOR;

/* Receiver ID Descriptor */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   receiver_id[4];
} RECEIVER_ID_DESCRIPTOR;



/********************************************************************************/
/*                     Direct Smart Card Access                                 */
/********************************************************************************/

NDS_STATUS CaApi_IssueSmartCardCommand(void *data_to_card, int data_to_card_length,
                                       void *data_from_card, int* data_from_card_length,
                                       unsigned char data_direction, int timeout);


/********************************************************************************/
/*                            Chaining                                          */
/********************************************************************************/

/* CHAINING_STATUS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   chaining_type;
   unsigned char   chaining_stage;
} CHAINING_STATUS_DESCRIPTOR;

NDS_STATUS CaApi_CardChaining  (void);
NDS_STATUS CaApi_AbortChaining (void);

/********************************************************************************/
/*                            Recording                                          */
/********************************************************************************/

/* RECORDING STATUS DESCRIPTOR */
typedef struct {
   unsigned char   descriptor_tag;
   unsigned char   descriptor_length;
   unsigned char   recording_status;
} RECORDING_STATUS_DESCRIPTOR;

/* Extended Config Info Descriptor */
typedef struct {
   unsigned char    descriptor_tag;
   unsigned char    descriptor_length;
   unsigned char    info_byte_1;
} EXTENDED_CONFIG_INFO_DESCRIPTOR;

/********************************************************************************/
/*                     Stream Operation parameters                                */
/********************************************************************************/
/* STREAM OPERATION values,for the mode of operation that the STB performs on this stream */
#define CA_OP_PLAY_LIVE            0
#define CA_OP_RECORD               1
#define CA_OP_PLAY_BACK            2
#define CA_OP_PREPARE_PLAY_LIVE    3
#define CA_OP_UNKNOWN              0xFF


/********************************************************************************/
/*                      DESCRIPTOR TAGS DEFINITION                              */
/********************************************************************************/
#define CARD_STATUS_DESCRIPTOR_TAG                    0x01
#define EXCEPTION_DESCRIPTOR_TAG                      0x02
#define VIEWING_CARD_CONFIG_DESCRIPTOR_TAG            0x03
#define SYSTEM_CONFIG_DESCRIPTOR_TAG                  0x04
#define VERIFIER_CONFIG_DESCRIPTOR_TAG                0x05
#define SUBSCRIBER_CONFIG_DESCRIPTOR_TAG              0x06
#define USER_PARAMETER_UPDATE_DESCRIPTOR_TAG          0x07
#define OSD_MSG_DESCRIPTOR_TAG                        0x08
#define APP_ID_DESCRIPTOR_TAG                         0x09
#define FECM_KEY_DESCRIPTOR_TAG                       0x0A
#define DATA_SOURCE_DESCRIPTOR_TAG                    0x0B
#define SERVICE_IDENTIFIER_DESCRIPTOR_TAG             0x0C
#define STREAM_REFERENCE_DESCRIPTOR_TAG               0x0D
#define PPV_CONFIG_DESCRIPTOR_TAG                     0x0E
#define CA_EVENT_STATUS_DESCRIPTOR_TAG                0x0F
#define SI_INFO_DESCRIPTOR_TAG                        0x10
#define REASON_FOR_CW_DESCRIPTOR_TAG                  0x11
#define VIEWING_PROBLEMS_DESCRIPTOR_TAG               0x12
#define NUM_PURCHASE_OPTIONS_DESCRIPTOR_TAG           0x13
#define PURCHASE_OPTION_DATA_DESCRIPTOR_TAG           0x14
#define PURCHASE_PROBLEMS_DESCRIPTOR_TAG              0x15
#define COST_DESCRIPTOR_TAG                           0x16
#define COST_LIMIT_PROBLEM_DESCRIPTOR_TAG             0x17
#define METADATA_DESCRIPTOR_TAG                       0x18
#define PURCHASE_INFO_DESCRIPTOR_TAG                  0x19
#define IPPV_SLOT_DESCRIPTOR_TAG                      0x1A
#define CARD_SLOT_UPDATE_DESCRIPTOR_TAG               0x1B
#define EMAIL_STATUS_DESCRIPTOR_TAG                   0x1C
#define CALLBACK_STATUS_DESCRIPTOR_TAG                0x1D
#define CHAINING_STATUS_DESCRIPTOR_TAG                0x1E
#define ACCOUNT_SLOT_DESCRIPTOR_TAG                   0x1F
#define BONUS_POINTS_DESCRIPTOR_TAG                   0x20
#define IPPV_RECORD_HANDLE_DESCRIPTOR_TAG             0x21
#define SUBSCRIPTION_SLOT_DESCRIPTOR_TAG              0x22
#define PURCHASE_OPTION_NUMBER_DESCRIPTOR_TAG         0x23
#define LIMIT_PROPERTIES_DESCRIPTOR_TAG               0x24
#define USER_PIN_DESCRIPTOR_TAG                       0x25
#define EMAIL_RECORD_DESCRIPTOR_TAG                   0x26
#define STREAM_OPERATION_DESCRIPTOR_TAG               0x27
#define RECORDING_STATUS_DESCRIPTOR_TAG               0x28
#define RENT_OPTION_DATA_DESCRIPTOR_TAG               0x29
#define TIME_BLACKOUT_DESCRIPTOR_TAG                  0x2A
#define NOTIFICATION_DESCRIPTOR_TAG                   0x2B
#define PIN_CODE_CERTIFICATE_DESCRIPTOR_TAG           0x2C
#define PIN_CODE_CERTIFICATE_REQUEST_DESCRIPTOR_TAG   0x2D
#define DATA_TO_EPG_DESCRIPTOR_TAG                    0x2E
#define CCI_DESCRIPTOR_TAG                            0x2F
#define RATING_LEVEL_DESCRIPTOR_TAG                   0x30
#define PURCHASE_ACTION_REQUEST_ID_DESCRIPTOR_TAG     0x31
#define OSD_EXTRA_DATA_DESCRIPTOR_TAG                 0x32
#define EXCEPTION_EXTRA_DATA_DESCRIPTOR_TAG           0x33
#define PROGRAM_INFORMATION_DESCRIPTOR_TAG            0x34
#define RECEIVER_ID_DESCRIPTOR_TAG                    0x35
#define EXTENDED_CONFIG_INFO_DESCRIPTOR_TAG           0x36
#define ADDITIONAL_ENTITLEMENT_INFO_DESCRIPTOR_TAG    0x37
#define MAXIMUM_EXTENSION_DATE_DESCRIPTOR_TAG         0x38
#define FINAL_EXPIRY_DATE_DESCRIPTOR_TAG              0x39
#define PURCHASE_PACKET_DESCRIPTOR_TAG                0x3A
#define OVERT_FINGERPRINT_DESCRIPTOR_TAG			  0x3B
#define IPPV_SLOT_EXTENSION_DESCRIPTOR_TAG            0x3C
#define OPPV_SLOT_EXTENSION_DESCRIPTOR_TAG            0x3D
#define EXTENDED_IPPV_RECORD_HANDLE_DESCRIPTOR_TAG    0x3E /* DirecTV only */
#define GROUP_DESCRIPTOR_TAG                          0x3E /* Generalized case of group of descriptors */
#define CANCEL_ACTION_DESCRIPTOR_TAG                  0x3F
/********************************************************************************/
/*                            ADD BY WANSUANLIN(2008-09-10)                     */
/********************************************************************************/
#define CLIENT_STATUS_DESCRIPTOR_TAG                  0x40
#define ENTITLEMENT_SPECIFIC_INFO_DESCRIPTOR_TAG      0x41
#define FINGERPRINT_FROM_ECM_DESCRIPTOR_TAG           0x42
#define CHAINING_CARD_DESCRIPTOR_TAG                  0x43
#define HARDWARE_STATUS_DESCRIPTOR_TAG                0x4A
#define CONTENT_LICENSE_STATUS_DESCRIPTOR_TAG         0x4C
#define TOTAL_EMAIL_LIST_DESCRIPTOR_TAG               0x51
#define EMAIL_READ_STATUS_DESCRIPTOR_TAG              0x52
#define CA_STB_LABEL_DESCRIPTOR_TAG                   0x53
#define CARD_DATA_CHANGE_DESCRIPTOR_TAG               0x60

/********************************************************************************/
/*                            RETURN CODES                                      */
/********************************************************************************/
#ifdef NDS_OK
   #undef NDS_OK
#endif

enum {
   NDS_OK,
   CARD_ERROR,
   ACTION_FAILED,
   INVALID_PARAMETER,
   INVALID_PIN,
   INVALID_USER_ID,
   NO_OSD_MESSAGES,
   UNKNOWN_HANDLE,
   CALLBACK_NOT_ENABLED,
   CALLBACK_MODULE_BUSY,
   IPPV_MODULE_BUSY,
   INFO_NOT_AVAILABLE,
   WRONG_CARD_NEED_NEW,
   WRONG_CARD_NEED_OLD,
   CARD_NOT_CHAINABLE,
   CARD_IN_TRANSITION,
   INSUFFICIENT_MEMORY_AVAILABLE,
   INVALID_MAIL_MESSAGE_NUMBER,
   UNDELETABLE_EMAIL,
   INVALID_LIMIT_ID,
   INSUFFICIENT_BUFFER,
   WRONG_CARD_ID,
   WRONG_CUSTOMER_ID,
   WRONG_LANGUAGE,
   UNPAIRED_OLD_CARD
};


#endif    /* #ifndef H_N3T009 */

