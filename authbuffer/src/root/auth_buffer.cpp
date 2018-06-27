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


QAVSDK_AUTHBUFFER_API int QAVSDK_AUTHBUFFER_CALL QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int dwSdkAppId, unsigned int dwRoomID, const char* strAccount, const char* key, unsigned int expTime, unsigned int nAuthBits, unsigned char* retAuthBuff, unsigned int bufferLength) {
    printf("dwSdkAppid = %u, dwRoomID = %u, dwExpTime = %u, dwAuthBits = %u\n",
        dwSdkAppId, dwRoomID, expTime, nAuthBits);
    
    if (strAccount == NULL || key == NULL || strlen(key) != 16){
        return 0;
    }
    
    unsigned char pInBuf[512]={0};
    xel::byte_writer bw(pInBuf, sizeof(pInBuf));
    
    char cVer=0;
    unsigned short wAccountLen = (unsigned short)strlen((const char *)strAccount);
    
    bw.write_byte(cVer);
    bw.write_int16(wAccountLen);
    bw.write_bytes(strAccount, wAccountLen);
    bw.write_int32(dwSdkAppId);
    bw.write_int32(dwRoomID);
    bw.write_int32(expTime);
    bw.write_int32(nAuthBits);
    bw.write_int32(0 /*dwAccountType*/);
    
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
