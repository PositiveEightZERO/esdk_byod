# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#add for svn build
#include $(call all-subdir-makefiles)

#LOCAL_ARM_MODE := arm

LOCAL_PATH := $(call my-dir)
APP_ABI := armeabi-v7a


include $(CLEAR_VARS)

LOCAL_MODULE    := jniapi

LOCAL_CFLAGS += -DAPP_VERSION_DEFAULT
LOCAL_CFLAGS += -DHAVE_SYS_UIO_H

#add for head file search path
LOCAL_C_INCLUDES :=            \
        $(LOCAL_PATH)          \
        $(LOCAL_PATH)/software/include \
        $(LOCAL_PATH)/software/include/anyoffice \
        $(NDK_HOME)/sources/cxx-stl/stlport/stlport 
#        $(LOCAL_PATH)/software/utils \
#        $(LOCAL_PATH)/software/cutils \
#        $(LOCAL_PATH)/software/nativehelper

#LOCAL_ALLOW_UNDEFINED_SYMBOLS :=true

SOURCEPATH1 :=software
SOURCE +=$(SOURCEPATH1)/JNIOnLoad.cpp
#SOURCE +=$(SOURCEPATH1)/svn_http_jni.c
SOURCE +=$(SOURCEPATH1)/fsm_api_jni.c
SOURCE +=$(SOURCEPATH1)/svn_socket_jni.c
SOURCE +=$(SOURCEPATH1)/svn_server_jni.c
SOURCE +=$(SOURCEPATH1)/svn_dns_resolve.c
SOURCE +=$(SOURCEPATH1)/svn_sqlite_SQLiteCommon.cpp
SOURCE +=$(SOURCEPATH1)/svn_sqlite_SQLiteConnection.cpp
SOURCE +=$(SOURCEPATH1)/svn_sqlite_SQLiteDebug.cpp
SOURCE +=$(SOURCEPATH1)/svn_sqlite_SQLiteGlobal.cpp
#SOURCE +=$(SOURCEPATH1)/sqlite3_android.cpp
#SOURCE +=$(SOURCEPATH1)/OldPhoneNumberUtils.cpp
#SOURCE +=$(SOURCEPATH1)/PhoneNumberUtils.cpp
SOURCE +=$(SOURCEPATH1)/android_util_Log.cpp
SOURCE +=$(SOURCEPATH1)/android_util_Binder.cpp
SOURCE +=$(SOURCEPATH1)/android_os_Parcel.cpp
SOURCE +=$(SOURCEPATH1)/CursorWindow.cpp
SOURCE +=$(SOURCEPATH1)/svn_sqlite_CursorWindow.cpp
SOURCE +=$(SOURCEPATH1)/IPCThreadState.cpp
SOURCE +=$(SOURCEPATH1)/JniConstants.cpp
SOURCE +=$(SOURCEPATH1)/Parcel.cpp
SOURCE +=$(SOURCEPATH1)/Unicode.cpp
SOURCE +=$(SOURCEPATH1)/String8.cpp
SOURCE +=$(SOURCEPATH1)/String16.cpp
SOURCE +=$(SOURCEPATH1)/Static.cpp
LOCAL_SRC_FILES += $(SOURCE)

LOCAL_CFLAGS += -DANYOFFICE_ANDROID
LOCAL_CFLAGS += -DHAVE_CONFIG_H
LOCAL_CFLAGS += -DHAVE_PTHREADS

#LOCAL_SHARED_LIBRARIES = libsvnssl libsvncrypto libsvnapi

#LOCAL_SHARED_LIBRARIES := libandroid_runtime

LOCAL_LDLIBS := -ldl -llog -lz -L$(LOCAL_PATH)/libs -lsvnapi -lanyofficesdk -lnativehelper -lutils -lcutils -landroid_runtime -lbinder \
	$(call host-path, $(NDK_HOME)/sources/cxx-stl/stlport/libs/armeabi/libstlport_static.a)


include $(BUILD_SHARED_LIBRARY) 



