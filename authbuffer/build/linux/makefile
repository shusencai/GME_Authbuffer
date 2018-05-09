all: libAuthBuffer
test:
	mkdir -p ../../bin/linux
	g++ -g -o ../Release/linux/test_tea ../test_tea.cpp ../Release/linux/libtea.a

libAuthBuffer:
	mkdir -p ../../bin/
	mkdir -p ../../bin/linux
	g++ ../../src/root/auth_buffer.cpp \
		../../src/root/tea/tea.cpp \
		../../src/root/tea/xel_byte_writer.cpp \
		../../src/root/tea/xel_hex_string.cpp \
		../../src/root/tea/xel_hex_util.cpp \
		-I../../src/root/ \
		-I../../src/root/tea \
		-fPIC -shared -o ../../bin/linux/libAuthbuffer.so

clean:
	rm -r ../../bin/

