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

+ (NSData*) GenAuthBuffer:(unsigned int)appId roomID:(NSString*)roomID openID:(NSString*)openID key:(NSString*)key
{
    unsigned int bufferLen = 512;
    unsigned char retAuthBuff[512] = {0};
    
    bufferLen = QAVSDK_AuthBuffer_GenAuthBuffer(appId, [roomID UTF8String], [openID UTF8String], [key UTF8String] ,retAuthBuff, bufferLen);
    
    return [NSData dataWithBytes:retAuthBuff length:bufferLen];
}

@end
