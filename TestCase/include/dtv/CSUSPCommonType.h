#ifndef CSUSPCommonType_H
#define CSUSPCommonType_H


#include "mmcp_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif




#define CAPID_IRDETO            1     /*SystemID范围： 0x0600 to 0x06FF*/
#define CAPID_NDS               2     /*SystemID范围： 0x0900 to 0x09FF*/
#define CAPID_CONAX             3     /*SystemID范围： 0x0B00 to 0x0BFF*/
#define CAPID_MEDIAGUARD        4     /*/SystemID范围： 0x0100 to 0x01FF*/
#define CAPID_NAGRAVISION       5     /*SystemID范围： 0x1800 to 0x18FF*/
#define CAPID_VIACCESS          6     /*SystemID范围： 0x0500 to 0x05FF*/
#define CAPID_CRYPTOWORKS       7     /*SystemID范围： 0x0D00 to 0x0DFF*/
#define CAPID_TF                8     /*SystemID范围：目前同方CA的SystemID没有范围，只是一个经验值(0x4a02)*/


#ifdef  __cplusplus
}
#endif


#endif /* CSUSPCommonType_H */


