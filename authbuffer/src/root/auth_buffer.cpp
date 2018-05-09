//
//  author_buffer.c
//  QAVSDK_SIG
//
//  Created by tobinchen on 08/08/2017.
//  Copyright © 2017 tobinchen. All rights reserved.
//

#include "auth_buffer.h"
#include "xel_byte_writer.h"
#include "xel_hex_string.h"
#include "tea.h"


QAVSDK_API int QAVSDK_CALL QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int appId, unsigned int authId, const char* account, unsigned int accountType, const char* key, unsigned int expTime, unsigned int privilegeMap, unsigned char* retAuthBuff, unsigned int* buffLenght) {
    
    printf("dwSdkAppid = %u , dwAuthId = %u , dwExpTime = %u  , dwPrivilegeMap = %u , dwAccountType = %u\n",
           appId, authId ,expTime , privilegeMap , accountType);
    
    if (!retAuthBuff || !account){
        return -1;
    }
    
    unsigned char pInBuf[512]={0};
    xel::byte_writer bw(pInBuf, sizeof(pInBuf));
    
    char cVer=0;
    unsigned short wAccountLen = strlen((const char *)account);
    
    bw.write_byte(cVer);
    bw.write_int16(wAccountLen);
    bw.write_bytes(account, wAccountLen);
    bw.write_int32(appId);
    bw.write_int32(authId);
    bw.write_int32(expTime);
    bw.write_int32(privilegeMap);
    bw.write_int32(accountType);
    
    int pInLen = bw.bytes_write();
    
	unsigned char pEncryptOutBuf[512] = { 0 };
    int iEncrptyLen = 0;
    
	symmetry_encrypt((const unsigned char*)pInBuf, pInLen, (const unsigned char*)key, (unsigned char*)pEncryptOutBuf, &iEncrptyLen);
    
    memcpy(retAuthBuff, pEncryptOutBuf, iEncrptyLen);
    retAuthBuff[iEncrptyLen]='\0';
    *buffLenght = iEncrptyLen;
    return 0;
}
