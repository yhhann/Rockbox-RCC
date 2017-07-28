/*
 * 提供关机函数的调用 by zhkailing
 */

#include <jni.h>

#include "rockbox-jni.h"
#include "powermgmt.h"

JNIEXPORT void JNICALL Java_com_gaana_jni_RockboxNativeInterface_powerOff
  (JNIEnv *env, jclass thiz){
    (void)env;
    (void)thiz;
    sys_poweroff();
}
