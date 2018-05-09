//
//  author_buffer.c
//  QAVSDK_SIG
//
//  Created by tobinchen on 08/08/2017.
//  Copyright © 2017 tobinchen. All rights reserved.
//

#import "QAVAuthBuffer.h"
#include "auth_buffer.h"



@implementation QAVAuthBuffer

+ (NSData*) GenAuthBuffer:(unsigned int)appId roomId:(unsigned int)roomId identifier:(NSString*)identifier accountType:(unsigned int)accountType key:(NSString*)key expTime:(unsigned int)expTime authBits:(unsigned int) authBits
{
    unsigned int bufferLen = 512;
    unsigned char retAuthBuff[512] = {0};
    
    QAVSDK_AuthBuffer_GenAuthBuffer(appId, roomId, [identifier UTF8String], accountType, [key UTF8String], expTime, authBits, retAuthBuff, &bufferLen);
    
    return [NSData dataWithBytes:retAuthBuff length:bufferLen];
}

@end
