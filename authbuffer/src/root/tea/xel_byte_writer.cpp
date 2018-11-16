/*
 * xel_byte_writer.cpp
 *
 *  Created on: 2017年6月8日
 *      Author: XELEMENTLIU
 */



#include "xel_byte_writer.h"

namespace xel {

byte_writer::byte_writer(void* buf, int32_t length) {
	this->m_bytes_write = 0;
	this->m_buf = (uint8_t*)buf;
	this->m_size = length;
}


byte_writer::~byte_writer() {

}

int32_t byte_writer::bytes_write() {
	return m_bytes_write;
}

int32_t byte_writer::remaining() {
	return m_size - m_bytes_write;
}

int32_t byte_writer::write_int32(int32_t i) {
	int32_t pos = m_bytes_write;
	m_buf[pos++]  =  (i >> 24) & 0xff;
	m_buf[pos++]  =  (i >> 16) & 0xff;
	m_buf[pos++]  =  (i >> 8) & 0xff;
	m_buf[pos++]  =  (i) & 0xff;
	m_bytes_write = pos;
	return 4;
}

short byte_writer::write_int16(short s) {
	int32_t pos = m_bytes_write;
	m_buf[pos++]  =  (s >> 8) & 0xff;
	m_buf[pos++]  =  (s) & 0xff;
	m_bytes_write = pos;
	return 2;
}

int32_t byte_writer::write_long(long l) {
	return write_int32(l);
}

int32_t byte_writer::write_uint64(uint64_t ull){
	int32_t pos = m_bytes_write;
	m_buf[pos++] = (ull  >> 56 ) & 0xff;
	m_buf[pos++] = (ull  >> 48 ) & 0xff;
	m_buf[pos++] = (ull  >> 40 ) & 0xff;
	m_buf[pos++] = (ull  >> 32 ) & 0xff;
	m_buf[pos++] = (ull  >> 24 ) & 0xff;
	m_buf[pos++] = (ull  >> 16 ) & 0xff;
	m_buf[pos++] = (ull  >> 8 ) & 0xff;
	m_buf[pos++] = (ull  >> 0 ) & 0xff;
	m_bytes_write = pos;
	return 8;
}

int32_t byte_writer::write_byte(int8_t s) {
	m_buf[m_bytes_write++] = s;
	return 1;
}

int32_t byte_writer::write_bytes(const void* bytes, int32_t count) {
	memcpy(this->m_buf + m_bytes_write , bytes , count);
	m_bytes_write+=count;
	return count;
}

int32_t byte_writer::skip(int count) {
	m_bytes_write += count;
	return count;
}

bool byte_writer::is_end() {
	return m_bytes_write >= m_size;
}

void byte_writer::attach(void* buf , int32_t size){
	this->m_buf = (uint8_t*)buf;
	this->m_bytes_write = 0;
	this->m_size = size;
}

void byte_writer::detach(){
	this->m_buf = NULL;
	this->m_bytes_write = 0;
	this->m_size = 0;
}

}
