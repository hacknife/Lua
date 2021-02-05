//
// Created by 冯鸿杰 on 17/1/13.
//

#ifndef ANDROID_LUAJAVAEXCEPTIONHANDLER_H
#define ANDROID_LUAJAVAEXCEPTIONHANDLER_H

#include <jni.h>

/**
 * Java异常处理器
 */
class JavaExceptionHandler
{
private:
    jobject _jcontext;

public:
    JavaExceptionHandler(JNIEnv *env, jobject jcontext);
    virtual ~JavaExceptionHandler();
};


#endif //ANDROID_LUAJAVAEXCEPTIONHANDLER_H
