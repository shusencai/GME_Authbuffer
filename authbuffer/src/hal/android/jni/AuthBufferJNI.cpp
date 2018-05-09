#include <jni.h>
#include "auth_buffer.h"
#include <time.h>
#include <cstring>


#undef	__MODULE__
#define __MODULE__ "SDKJNI"

bool Java2Native(JNIEnv *env, char **nativeStr, jstring &javaStr)
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
													jint appId, jint roomId, jstring account,
													jint accountType,
													jstring key, jint expTime, jint authBits) {

	char *nativeAccount = NULL;
	if (account != NULL) {
		Java2Native(env, &nativeAccount, account);
	}

	if (nativeAccount == NULL) {
		return NULL;
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

	QAVSDK_AuthBuffer_GenAuthBuffer(appId, roomId, nativeAccount, accountType,
									nativeKey, expTime, authBits, retAuthBuff,
									&bufferLen);

	jbyteArray jarr = env->NewByteArray(bufferLen);
	jbyte *arr = env->GetByteArrayElements(jarr, NULL);

	for (int i = 0; i < bufferLen; i++) {
		arr[i] = retAuthBuff[i];
	}

	env->ReleaseByteArrayElements(jarr, arr, 0);

	delete[] nativeAccount;
	delete[] nativeKey;

	return jarr;
}
}