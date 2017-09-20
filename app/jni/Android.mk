LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDED += \
	$(JNI_H_INCLUDE)
LOCAL_MODULE := native-lib
LOCAL_SRC_FILES := \
	../src/main/cpp/native-lib.cpp \
	../src/main/cpp/rb_tree.c
include $(BUILD_SHARED_LIBRARY)
