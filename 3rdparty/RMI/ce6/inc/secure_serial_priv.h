/* <LIC_AMD_STD>
 * Copyright (C) 2003-2005 Advanced Micro Devices, Inc.  All Rights Reserved.
 * 
 * Unless otherwise designated in writing, this software and any related 
 * documentation are the confidential proprietary information of AMD. 
 * THESE MATERIALS ARE PROVIDED "AS IS" WITHOUT ANY
 * UNLESS OTHERWISE NOTED IN WRITING, EXPRESS OR IMPLIED WARRANTY OF ANY 
 * KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE AND IN NO 
 * EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER. 
 * 
 * AMD does not assume any responsibility for any errors which may appear 
 * in the Materials nor any responsibility to support or update the
 * Materials.  AMD retains the right to modify the Materials at any time, 
 * without notice, and is not obligated to provide such modified 
 * Materials to you. AMD is not obligated to furnish, support, or make
 * any further information available to you.
 * </LIC_AMD_STD>  */
/* <CTL_AMD_STD>
 * </CTL_AMD_STD>  */
/* <DOC_AMD_STD>
 * </DOC_AMD_STD>  */


#ifndef SECURE_SERIAL_PRIV_H
#define SECURE_SERIAL_PRIV_H

#ifdef __cplusplus
extern "C" {
#endif


#include <tchar.h>

#define SHA1_BLOCK_LENGTH   64
#define SHA1_DIGEST_LENGTH    20
#define SHA1_DIGEST_STRING_LENGTH (SHA1_DIGEST_LENGTH * 2 + 1)

// This strings needs t be obfuscated 

#define SEC_PORT 104
#define COMMAND_OFFSET 33
#define COMMAND_SIZE 48 
#define SER_BUF_SIZE 256
#define RETRY_COUNT 2

// server command
#define GET_SERIAL_NUM 's'
#define GET_DIVX_KEY 'd'
#define GET_HARDWARE_SECRET 'H'
#ifndef CHECK_SLEEP
#define CHECK_SLEEP 60*5				// seconds between check
#endif

#define SERIAL_DELIMITER ':'
#define KEY_SIZE 32
#define MAX_DIXV_KEY_SIZE 32
#define HARDWARE_SECRET_SIZE 8*4


typedef struct {
	int 		frontKey[KEY_SIZE];
	TCHAR	  serialNum[MAX_SERIAL_NUM_SIZE];
	int			endKey[KEY_SIZE];
} serialMsg;

typedef struct {
	TCHAR	 serialNum[MAX_SERIAL_NUM_SIZE*4];  // 4x because is is b64 encoded
	long	 hostid;
	long	 status;
	TCHAR	 sig[MAX_SERIAL_NUM_SIZE*4];
} netMsg;

typedef struct {
  TCHAR   serialNum[MAX_SERIAL_NUM_SIZE*4];  // 4x because is is b64 encoded
  long   hostid;
  long   status;
  TCHAR   sig[MAX_SERIAL_NUM_SIZE*4];
} divxKeyMsg;


#ifdef NEED_HW_SECRET
static  uint32  salt[8] = { 0x9defc909, 0x301ec998, 0xcde43ceb,
                     0x66dd8015, 0xf5086c66, 0x268d0a3a, 0x89af5557, 
                     0xd19c7d87};  
#endif
                     
#if 0
// obfuscate names using strings common to all executables
#ifndef DEBUG_REALNAME
#define SHA1Init bss
#define SHA1Pad ABCD
#define SHA1Transform BCD
#define SHA1Final CDEF
#define SHA1Update EFGH
#define getMyHostid libc
#define hashSerial lib
#define getSerialNum GN
#define getDivxKey DEFG
#endif

#endif // if 0 net to research more 

long getMyHostid(char *serialNum);
TCHAR *hashSerial(TCHAR *serialNum);

#ifdef __linux__
int  getDivxKey(u_int8_t **key,int *errorCode);
#include <sys/cdefs.h>
#endif

typedef struct {
    u_int32_t state[5];
    u_int64_t count;
    u_int8_t buffer[SHA1_BLOCK_LENGTH];
} SHA1_CTX;


void SHA1Transform(u_int32_t state[5], const u_int8_t buffer[SHA1_BLOCK_LENGTH]);
void SHA1Final(u_int8_t digest[SHA1_DIGEST_LENGTH], SHA1_CTX *context);	
void SHA1Pad(SHA1_CTX *context);
void SHA1Init(SHA1_CTX *context);
void SHA1Update(SHA1_CTX *context, const u_int8_t *data, size_t len);



#define HTONDIGEST(x) do {                                              \
        x[0] = htonl(x[0]);                                             \
        x[1] = htonl(x[1]);                                             \
        x[2] = htonl(x[2]);                                             \
        x[3] = htonl(x[3]);                                             \
        x[4] = htonl(x[4]); } while (0)

#define NTOHDIGEST(x) do {                                              \
        x[0] = ntohl(x[0]);                                             \
        x[1] = ntohl(x[1]);                                             \
        x[2] = ntohl(x[2]);                                             \
        x[3] = ntohl(x[3]);                                             \
        x[4] = ntohl(x[4]); } while (0)

#ifdef __cplusplus
}
#endif

#endif /* _SHA1_H */
