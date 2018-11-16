/*
 * xel_hex_util.cpp
 *
 *  Created on: 2017年6月8日
 *      Author: XELEMENTLIU
 */

#include "xel_hex_util.h"

namespace xel {


static char hexTab[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static char toAscii(char ch) {
	if (ch >= '0' && ch <= '9') {
		return (char) (ch - '0');
	} else if (ch >= 'a' && ch <= 'f') {
		return (char) (ch - 'a' + 10);
	} else if (ch >= 'A' && ch <= 'F') {
		return (char) (ch - 'A' + 10);
	} else {
		return ch;
	}
}

void* toHex(const void* pInBytesBuf, int nInBytesLen) {

	const char* bytes = (const char*)pInBytesBuf;
	char* hexBuf = new char[(nInBytesLen << 1) + 1];
	int index = 0;
	int i;
	for (i = 0; i < nInBytesLen; ++i) {
		hexBuf[index++] = hexTab[(bytes[i] & 0xf0) >> 4]; //high
		hexBuf[index++] = hexTab[bytes[i] & 0x0f]; //low
	}
	hexBuf[index] = '\0';
	return hexBuf;
}

void* toBytes(const void* pInHexBuf, int nInHexLen) {
	const char* hex = (const char*)pInHexBuf;
	int bytesLen = nInHexLen >> 1;
	char* bytesBuf = new char[bytesLen];

	for (int i = 0; i < bytesLen; i++) {
		char high = hex[i << 1];
		char low = hex[(i << 1) + 1];
		bytesBuf[i] = (char) ((toAscii(high) << 4) | toAscii(low));
	}
	return bytesBuf;
}

void toHex(const void* pInBytesBuf, int nInBytesLen,void* pOutBuf,int nOutBufMaxLen,int* nOutLen) {

	const char* bytes = (const char*)pInBytesBuf;
	char* outBuf = (char*)pOutBuf;

	int hexLen = (nInBytesLen << 1) , i;
	if (hexLen > nOutBufMaxLen) {
		if(nOutLen){
			*nOutLen = 0;
		}
		return;
	} else if (hexLen < nOutBufMaxLen) {
		outBuf[hexLen] = '\0';
	}

	hexLen = 0; 
	i = 0;

	for (; i < nInBytesLen; ++i) {
		outBuf[hexLen++] = hexTab[(bytes[i] & 0xf0) >> 4]; //high
		outBuf[hexLen++] = hexTab[bytes[i] & 0x0f]; //low
	}

	if(nOutLen){
		*nOutLen = hexLen;
	}

}

void toBytes(const void* pInHexBuf, int nInHexLen, void* pOutBuf,int nOutBufMaxLen, int* nOutLen) {

	if(nInHexLen %2 != 0){
		if(nOutLen){
			*nOutLen = 0;
		}
		return;
	}

	const char* hex = (const char*)pInHexBuf;
	char* outBuf = (char*)pOutBuf;

	int bytesLen = nInHexLen >> 1;

	for (int i = 0; i < bytesLen; i++) {
		char high = hex[i << 1];
		char low = hex[(i << 1) + 1];
		outBuf[i] = (char) ((toAscii(high) << 4) | toAscii(low));
	}

	if(bytesLen < nOutBufMaxLen){
		outBuf[bytesLen] = '\0';
	}

	if(nOutLen){
		*nOutLen = bytesLen;
	}
}

}
