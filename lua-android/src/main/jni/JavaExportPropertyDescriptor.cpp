//
// Created by 冯鸿杰 on 2017/12/1.
//

#include "JavaExportPropertyDescriptor.h"
#include "LuaSession.h"
#include "JavaEnv.h"
#include "JavaType.h"
#include "JavaExportTypeDescriptor.h"
#include "JavaObjectDescriptor.h"
#include "JavaConverter.h"
#include <jni.h>


JavaExportPropertyDescriptor::JavaExportPropertyDescriptor(std::string name,
                                                           bool canRead,
                                                           bool canWrite)
    : LuaExportPropertyDescriptor(name, canRead, canWrite)
{

}

LuaValue* JavaExportPropertyDescriptor::invokeGetter(LuaSession *session, LuaObjectDescriptor *instance)
{
    if (canRead())
    {
        LuaContext *context = session -> getContext();

        JNIEnv *env = JavaEnv::getEnv();

        jobject jExportTypeManager = JavaEnv::getExportTypeManager(env);
        jmethodID invokeMethodId = env -> GetMethodID(JavaType::exportTypeManagerClass(env), "getterMethodRoute", "(Lcom/hacknife/lua/LuaContext;Ljava/lang/Object;Ljava/lang/String;)Lcom/hacknife/lua/LuaValue;");

        jobject jContext = JavaEnv::getJavaLuaContext(env, context);

        jstring methodName = env -> NewStringUTF(name().c_str());

        JavaObjectDescriptor *objectDescriptor = (JavaObjectDescriptor *)instance;
        jobject jReturnValue = env -> CallObjectMethod(jExportTypeManager, invokeMethodId, jContext, objectDescriptor -> getJavaObject(), methodName);

        env -> DeleteLocalRef(methodName);

        LuaValue *returnValue = JavaConverter::convertToLuaValueByJLuaValue(env, context, jReturnValue);

        env -> DeleteLocalRef(jReturnValue);

        JavaEnv::resetEnv(env);

        return returnValue;
    }

    return NULL;

}


void JavaExportPropertyDescriptor::invokeSetter(LuaSession *session, LuaObjectDescriptor *instance, LuaValue *value)
{
    if (canWrite())
    {
        LuaContext *context = session -> getContext();

        JNIEnv *env = JavaEnv::getEnv();

        jobject jExportTypeManager = JavaEnv::getExportTypeManager(env);
        jmethodID invokeMethodId = env -> GetMethodID(JavaType::exportTypeManagerClass(env), "setterMethodRoute", "(Lcom/hacknife/lua/LuaContext;Ljava/lang/Object;Ljava/lang/String;Lcom/hacknife/lua/LuaValue;)V");

        jobject jContext = JavaEnv::getJavaLuaContext(env, context);
        jstring methodName = env -> NewStringUTF(name().c_str());

        JavaObjectDescriptor *objectDescriptor = (JavaObjectDescriptor *)instance;

        jobject jLuaValue = JavaConverter::convertToJavaLuaValueByLuaValue(env, context, value);
        env -> CallVoidMethod(jExportTypeManager, invokeMethodId, jContext, objectDescriptor -> getJavaObject(), methodName, jLuaValue);

        env -> DeleteLocalRef(jLuaValue);
        env -> DeleteLocalRef(methodName);

        JavaEnv::resetEnv(env);
    }
}