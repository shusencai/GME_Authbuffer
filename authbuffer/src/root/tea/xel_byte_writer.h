/*
 * xel_byte_writer.h
 *
 *  Created on: 2017年6月5日
 *      Author: XELEMENTLIU
 */

#ifndef _XEL_BYTE_WRITER_H_
#define _XEL_BYTE_WRITER_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>

namespace xel {

class byte_writer {

public:
	byte_writer(void* buf, int32_t length);
	~byte_writer();
public:
	int32_t write_int32(int32_t i);
	int16_t write_int16(int16_t s);
	int32_t write_long(long l);
	int32_t write_uint64(uint64_t v);
	int32_t write_byte(int8_t c);
	int32_t write_bytes(const void* pBuf, int32_t count);
	int32_t skip(int32_t count);
	bool is_end();
	int32_t bytes_write();
	int32_t remaining();
	void attach(void* buf , int32_t size);
	void detach();

private:
	uint8_t* m_buf;
	int32_t m_bytes_write;
	int32_t m_size;
};

}

#endif /* _XEL_BYTE_WRITER_H_ */
