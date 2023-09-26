#include "android/uni_jni_utils.h"
#include "NanoLog.hpp"


#ifdef __cplusplus
extern "C"
{
#endif 

//Global varaiables
UnionJNIEnvToVoid  g_uenv;
JavaVM *g_vm = NULL;
JNIEnv *g_env = NULL;
int    g_attached = 1;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_uenv.venv = 0;
    jint result = -1;
    JNIEnv *env = 0;

    nanolog::initialize(nanolog::GuaranteedLogger(), "/data/data/com.universe.unilauncher/files/", "nanolog", 1);
    nanolog::set_log_level(nanolog::LogLevel::INFO);
    LOG_INFO << "JNI_OnLoad";

    if(vm->GetEnv(&g_uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    LOG_INFO << "JNI_OnLoad Success";

    result = JNI_VERSION_1_4;
    g_vm = vm;
    g_env = env;
    return result;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    LOG_INFO << "JNI_OnUnload";
}

#ifdef __cplusplus
}
#endif 

