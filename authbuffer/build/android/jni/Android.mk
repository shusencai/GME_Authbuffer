LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SrcRootDir := $(LOCAL_PATH)/../../../src/root
SrcHalDir := $(LOCAL_PATH)/../../../src/hal

LOCAL_SRC_FILES := \
	$(SrcRootDir)/auth_buffer.cpp \
	$(SrcRootDir)/tea/xel_byte_writer.cpp \
	$(SrcRootDir)/tea/xel_hex_string.cpp \
	$(SrcRootDir)/tea/xel_hex_util.cpp\
	$(SrcRootDir)/tea/Tea.cpp \
	$(SrcHalDir)/android/jni/AuthBufferJNI.cpp

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(SrcRootDir)/ \
	$(SrcRootDir)/tea \
	$(SrcHalDir)/android/jni
	
LOCAL_MODULE := qav_authbuff

include $(BUILD_SHARED_LIBRARY)

