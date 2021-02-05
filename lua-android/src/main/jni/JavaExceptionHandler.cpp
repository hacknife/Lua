//
// Created by 冯鸿杰 on 17/1/13.
//

#include "JavaExceptionHandler.h"
#include "JavaEnv.h"

JavaExceptionHandler::JavaExceptionHandler(JNIEnv *env, jobject jcontext)
    : _jcontext(env -> NewWeakGlobalRef(jcontext))
{

}

JavaExceptionHandler::~JavaExceptionHandler()
{
    JNIEnv *env = JavaEnv::getEnv();

    env -> DeleteWeakGlobalRef(_jcontext);

    JavaEnv::resetEnv(env);
}