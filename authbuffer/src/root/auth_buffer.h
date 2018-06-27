/****************************************************************************
CopyRight (c) Tencent Technology(Shenzhen) Co., Ltd
AuthBuffer Header
*****************************************************************************/

#ifndef qav_auth_buffer_h
#define qav_auth_buffer_h

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#ifdef _AV_IMPLEMENT_
#define QAVSDK_AUTHBUFFER_API extern "C" __declspec(dllexport)
#else
#define QAVSDK_AUTHBUFFER_API extern "C" __declspec(dllimport)
#endif
#define QAVSDK_AUTHBUFFER_CALL __cdecl
#elif defined(__APPLE__)
#define QAVSDK_AUTHBUFFER_API __attribute__((visibility("default"))) extern "C"
#define QAVSDK_AUTHBUFFER_CALL
#elif defined(ANDROID) || defined(__linux__) || defined(__linux)
#define QAVSDK_AUTHBUFFER_API extern "C"  __attribute__ ((visibility("default")))
#define QAVSDK_AUTHBUFFER_CALL
#else
#define QAVSDK_AUTHBUFFER_API extern "C"
#define QAVSDK_AUTHBUFFER_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
////Generate AuthBuffer to enter Room for Audio/Video Communication
//		[in]nAppId					--> User's AppID				
//		[in]nRoomID					--> Room number		
//		[in]strOpenID				--> User's OpenID
//		[in]strKey					--> Key from Tencent Cloud,you can get the information by the Tencent Cloud(strAccountType)	
//		[in]nExpTime				--> he expired time of the coming AuthBuffer 
//		[in]nAuthBits				--> Permissions of the player(defult value is -1)	
//		[out]strAuthBytes			--> Generated binary AuthBuffer
//		[in]bufferLength	        --> The length of the string AuthBuffer
//      [ret]                       --> Actual Length of AuthBuffer

QAVSDK_AUTHBUFFER_API int QAVSDK_AUTHBUFFER_CALL QAVSDK_AuthBuffer_GenAuthBuffer(unsigned int nAppId, unsigned int dwRoomID, const char* strOpenID,
	const char* strKey, unsigned int nExpTime, unsigned int nAuthBits, unsigned char* strAuthBuffer, unsigned int bufferLength);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* author_buffer_h */
