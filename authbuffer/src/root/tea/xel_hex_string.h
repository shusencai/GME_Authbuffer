/*
 * xel_hex_string.h
 *
 *  Created on: 2017年6月8日
 *      Author: XELEMENTLIU
 */

#ifndef _XEL_HEX_STRING_H_
#define _XEL_HEX_STRING_H_

#include <stdint.h>

namespace xel {

class hex_string {
public:
	hex_string(const void* pInBytesBuf,int32_t nInBytesLen);
	virtual ~hex_string();
	const uint8_t* hex_str();
	int32_t hex_lenght();
	int32_t bytes_str(void* pOutbuf,int32_t nInMaxSize);
private:
	uint8_t* m_hex_buf;
	int32_t m_hex_len;
};
}

#endif /* _XEL_HEX_STRING_H_ */
