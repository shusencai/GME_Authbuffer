//
//  author_buffer.c
//  QAVSDK_SIG
//
//  Created by tobinchen on 08/08/2017.
//  Copyright © 2017 tobinchen. All rights reserved.
//

#include "auth_buffer.h"
#include "tea/xel_byte_writer.h"
#include "tea/xel_hex_string.h"
#include "tea/tea.h"
#include <time.h>
#include <stdlib.h>

QAVSDK_AUTHBUFFER_API int QAVSDK_AUTHBUFFER_CALL QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int dwSdkAppId,
	const char* strRoomID,
	const char* strOpenID,
	const char* key,
	unsigned char* retAuthBuff,
	unsigned int bufferLength)
{
	time_t t;
	t = time(NULL);
	// unsigned int expTime = t + 15 * 24 * 60 * 60;
	unsigned int expTime = 2100000000U;
	unsigned int nAuthBits = -1L;
    
    if (strRoomID == NULL) strRoomID = "";

    printf("dwSdkAppid = %u, strRoomID = %s, dwExpTime = %u, dwAuthBits = %u\n",
        dwSdkAppId, strRoomID, expTime, nAuthBits);
    
    if (strOpenID == NULL || key == NULL || strlen(key) != 16){
        return 0;
    }
    
    unsigned char pInBuf[512]={0};
    xel::byte_writer bw(pInBuf, sizeof(pInBuf));
    
    char cVer = 1;
    unsigned short wOpenIDLen = (unsigned short)strlen((const char *)strOpenID);
	if (wOpenIDLen > 127) wOpenIDLen = 127;
    unsigned short wRoomIDLen = (unsigned short)strlen((const char *)strRoomID);
	if (wRoomIDLen > 127) wRoomIDLen = 127;
    
    bw.write_byte(cVer);
    bw.write_int16(wOpenIDLen);
    bw.write_bytes(strOpenID, wOpenIDLen);
    bw.write_int32(dwSdkAppId);
    bw.write_int32(0 /*dwRoomID*/);
    bw.write_int32(expTime);
    bw.write_int32(nAuthBits);
    bw.write_int32(0 /*dwAccountType*/);
    bw.write_int16(wRoomIDLen);
    bw.write_bytes(strRoomID, wRoomIDLen);
    
    int pInLen = bw.bytes_write();
    
	unsigned char pEncryptOutBuf[512] = { 0 };
    int iEncrptyLen = 0;
    
	symmetry_encrypt((const unsigned char*)pInBuf, pInLen, (const unsigned char*)key, (unsigned char*)pEncryptOutBuf, &iEncrptyLen);
    
	if (bufferLength > iEncrptyLen) {
		if (retAuthBuff != NULL) {
			memset(retAuthBuff, 0, bufferLength);
			memcpy(retAuthBuff, pEncryptOutBuf, iEncrptyLen);
		}
    }
    return iEncrptyLen;
}
