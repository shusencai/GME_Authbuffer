/*
 * xel_hex_string.cpp
 *
 *  Created on: 2017年6月8日
 *      Author: XELEMENTLIU
 */

#include "xel_hex_util.h"
#include "xel_hex_string.h"

namespace xel {


hex_string::hex_string(const void* pInBytesBuf,int32_t nInBytesLen){
	if(nInBytesLen <=0){
		m_hex_len = 0;
		m_hex_buf = new uint8_t[1];
		m_hex_buf[0] = '\0';
	}else{
		m_hex_len = nInBytesLen << 1;
		m_hex_buf = (uint8_t*)toHex(pInBytesBuf,nInBytesLen);
	}
}


hex_string::~hex_string() {
	m_hex_len = 0;
	delete[] m_hex_buf;
	m_hex_buf = NULL;
}

const uint8_t* hex_string::hex_str() {
	return m_hex_buf;
}

int32_t hex_string::hex_lenght(){
	return m_hex_len;
}

int32_t hex_string::bytes_str(void* pOutbuf,int nInMaxSize) {
	int32_t len = 0;
	xel::toBytes(m_hex_buf,m_hex_len,pOutbuf,nInMaxSize,&len);
	return len;
}

}
