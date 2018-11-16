#include <jni.h>
#include "auth_buffer.h"
#include <time.h>
#include <cstring>


#undef	__MODULE__
#define __MODULE__ "SDKJNI"

static bool Java2Native(JNIEnv *env, char **nativeStr, jstring &javaStr)
{
	if(nativeStr == NULL)
	{
		return false;
	}

	const char *nativeStrTmp = env->GetStringUTFChars(javaStr, 0);
	if (nativeStrTmp == NULL) 
	{
		return false; /* out of memory */
	}

	int length = env->GetStringUTFLength(javaStr);
	char* nativeStrBuff = new char[length + 1];
	if(nativeStrBuff == NULL)
	{
		*nativeStr = 0;
		env->ReleaseStringUTFChars(javaStr, nativeStrTmp);
		return false;
	}
	
	nativeStrBuff[0] = 0;
	if (length > 0)
	{
		memcpy(nativeStrBuff, nativeStrTmp, length);
	}
	
	nativeStrBuff[length] = 0;
	*nativeStr = nativeStrBuff;
	env->ReleaseStringUTFChars(javaStr, nativeStrTmp);
	return true;
}


extern "C"
{
JNIEXPORT jbyteArray JNICALL
Java_com_tencent_av_sig_AuthBuffer_genAuthBuffer(JNIEnv *env, jobject instance,
													jint sdkAppID, jstring roomID, jstring openID,
													jstring key) {

	char *nativeOpenID = NULL;
	if (openID != NULL) {
		Java2Native(env, &nativeOpenID, openID);
	}

	if (nativeOpenID == NULL) {
		return NULL;
	}
    
    char *nativeRoomID = NULL;
    if (roomID != NULL) {
        Java2Native(env, &nativeRoomID, roomID);
    }
    
    if (nativeRoomID == NULL) {
        nativeRoomID = new char[1];
        nativeRoomID[1] = '\0';
    }

	char *nativeKey = NULL;
	if (key != NULL) {
		Java2Native(env, &nativeKey, key);
	}

	if (nativeKey == NULL) {
		return NULL;
	}

	unsigned int bufferLen = 512;
	unsigned char retAuthBuff[512] = {0};

	bufferLen = QAVSDK_AuthBuffer_GenAuthBuffer(sdkAppID, nativeRoomID, nativeOpenID,
									nativeKey, retAuthBuff, bufferLen);

	jbyteArray jarr = env->NewByteArray(bufferLen);
	jbyte *arr = env->GetByteArrayElements(jarr, NULL);

	for (int i = 0; i < bufferLen; i++) {
		arr[i] = retAuthBuff[i];
	}

	env->ReleaseByteArrayElements(jarr, arr, 0);

	delete[] nativeOpenID;
    delete[] nativeRoomID;
	delete[] nativeKey;

	return jarr;
}
}
