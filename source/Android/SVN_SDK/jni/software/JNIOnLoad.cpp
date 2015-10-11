#include <jni.h>
#include <android/log.h>
#include "nativehelper/JNIHelp.h"
#include <android_runtime/AndroidRuntime.h>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "SDK", __VA_ARGS__))

//jint JNI_OnLoad(JavaVM* vm, void* reserved)
//{
//    g_JavaVM = vm;
//    JNIEnv* env = NULL;
//
//    if ( JNI_OK != (*g_JavaVM)->GetEnv(g_JavaVM,(JNIEnv**)&env,JNI_VERSION_1_4) )
//    {
//        LOGE("JNI_OnLoad GetEnv failed!");
//    }
//
//    register_svn_sqlite_SQLiteConnection(env);
//    register_svn_sqlite_SQLiteDebug(env);
//    register_svn_sqlite_SQLiteGlobal(env);
//
//
//    return JNI_VERSION_1_4;
//}

extern JavaVM* g_JavaVM;

namespace android
{
    extern int register_svn_sqlite_SQLiteConnection(JNIEnv *env);
    extern int register_svn_sqlite_SQLiteDebug(JNIEnv *env);
    extern int register_svn_sqlite_SQLiteGlobal(JNIEnv *env);

    extern int register_svn_sqlite_CursorWindow(JNIEnv *env);
}

__attribute__((visibility("default"))) jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_JavaVM = vm;
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }

//    jniRegisterNativeMethods(env, "com/huawei/svn/sdk/thirdpart/SvnWebView",
//                                                    gMethods, NELEM(gMethods));
    android::register_svn_sqlite_SQLiteConnection(env);
    android::register_svn_sqlite_SQLiteDebug(env);
    android::register_svn_sqlite_SQLiteGlobal(env);

    android::register_svn_sqlite_CursorWindow(env);

    return JNI_VERSION_1_4;
}
