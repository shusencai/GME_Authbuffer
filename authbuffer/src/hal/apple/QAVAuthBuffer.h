//
//  author_buffer.c
//  QAVSDK_SIG
//
//  Created by tobinchen on 08/08/2017.
//  Copyright © 2017 tobinchen. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface QAVAuthBuffer : NSObject

+ (NSData*) GenAuthBuffer:(unsigned int)appId roomId:(unsigned int)roomId identifier:(NSString*)identifier key:(NSString*)key expTime:(unsigned int)expTime authBits:(unsigned int) authBits;

@end
