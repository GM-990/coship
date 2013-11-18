/*
 * Generic Hash and HMAC Program
 *
 * Copyright (C) 2009 Harald von Fellenberg <hvf@hvf.ch>
 *
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or 
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/*  fehashmac.h	Generic header file for common definitions
 *
 *	hvf 15.9.01
 *	add LASH160	hvf 19.10.2008
 *	hvf 29.12.2008 add bitwise test vectors
 *	hvf 31.01.2009 align with SHA3-C-API
 *  hvf 29.03.2011 add SHA512-224 and SHA512-256 test vectors
 *	hvf 05.04.2011 add BLAKE
 */

#ifndef _CSBL_HMAC_H_
#define _CSBL_HMAC_H_

 
#ifdef __cplusplus
extern "C" {
#endif

//hmac 长度为固定值20
#define HMAC_LEN 20


/*
cKeyType: 0: bin format, 1, hexstring format.
*/
typedef struct
{
	unsigned char cKeyType;
	char *pcKey;
	unsigned char ucKeyLen;
}HMAC_KEY;

int CSBLHMACDigest(const unsigned char *pucData, unsigned int unDataLen,const HMAC_KEY *phmacKey, unsigned char *pMAC);

#ifdef  __cplusplus
}
#endif

#endif
