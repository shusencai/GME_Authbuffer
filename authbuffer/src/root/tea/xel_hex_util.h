/*
 * hex_util.h
 *
 *  Created on: 2017年6月8日
 *      Author: XELEMENTLIU
 */

#ifndef _XEL_HEX_UTIL_H_
#define _XEL_HEX_UTIL_H_

#include <stdlib.h>

namespace xel {

void *toHex(const void* pInBytesBuf, int nInBytesLen);
void toHex(const void* pInBytesBuf, int nInBytesLen,void* pOutBuf,int nOutBufMaxLen,int* nOutLen);

void *toBytes(const void* pInHexBuf, int nInHexLen);
void toBytes(const void* pInHexBuf, int nInHexLen, void* pOutBuf,int nOutBufMaxLen, int* nOutLen);

}

#endif /* _XEL_HEX_UTIL_H_ */
