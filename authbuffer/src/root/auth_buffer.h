//
//  author_buffer.h
//  QAVSDK_SIG
//
//  Created by tobinchen on 08/08/2017.
//  Copyright © 2017 tobinchen. All rights reserved.
//

#ifndef author_buffer_h
#define author_buffer_h

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef QAVSDK_IMPLEMENT
#define QAVSDK_API extern "C" __declspec(dllexport)
#else
#define QAVSDK_API extern "C" __declspec(dllimport)
#endif
#define QAVSDK_CALL __stdcall
#elif defined(__APPLE__)
#define QAVSDK_API __attribute__((visibility("default"))) extern "C"
#define QAVSDK_CALL
#elif defined(ANDROID) || defined(__linux__) || defined(__linux)
#define QAVSDK_API extern "C"  __attribute__ ((visibility("default")))
#define QAVSDK_CALL
#else
#define AGORA_API extern "C"
#define AGORA_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
QAVSDK_API int QAVSDK_CALL QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int appId, unsigned int authId, const char* account, unsigned int accountType, const char* key, unsigned int expTime, unsigned int privilegeMap, unsigned char* retAuthBuff, unsigned int* buffLenght);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* author_buffer_h */
