#pragma once

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif 

typedef union {
    JNIEnv *env;
    void   *venv;
}UnionJNIEnvToVoid;

extern JavaVM *g_vm;
extern JNIEnv *g_env;
extern int    g_attatched;
extern UnionJNIEnvToVoid g_uenv;

#define ATTATCH_JNI_THREAD  g_attatched =  g_vm->AttachCurrentThread(&g_env, NULL);\
            if(g_attatched > 0)\
            {\
                g_vm->DetachCurrentThread();\
            }else{\
            }

#define DETATCH_JNI_THREAD if(g_attatched <=0)\
            {\
                g_vm->DetachCurrentThread();\
            }

#ifdef __cplusplus
}
#endif 
