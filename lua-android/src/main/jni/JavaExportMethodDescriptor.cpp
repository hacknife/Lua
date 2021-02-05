//
// Created by 冯鸿杰 on 2017/11/16.
//

#include <StringUtils.h>
#include "JavaExportMethodDescriptor.h"
#include "JavaEnv.h"
#include "JavaType.h"
#include "JavaConverter.h"
#include "JavaExportTypeDescriptor.h"
#include "JavaObjectDescriptor.h"

JavaExportMethodDescriptor::JavaExportMethodDescriptor(std::string name, std::string methodSignature, LuaJavaMethodType type)
    : LuaExportMethodDescriptor(name, methodSignature)
{
    _type = type;
}

LuaValue* JavaExportMethodDescriptor::invoke(LuaSession *session, LuaArgumentList arguments)
{
    switch (_type)
    {
        case LuaJavaMethodTypeStatic:
            //类方法
            return invokeClassMethod(session, arguments);
        case LuaJavaMethodTypeInstance:
            //实例方法
            return invokeInstanceMethod(session, arguments);
        default:
            return NULL;
    }
}

LuaValue* JavaExportMethodDescriptor::invokeClassMethod(LuaSession *session, LuaArgumentList arguments)
{
    LuaContext *context = session -> getContext();

    JNIEnv *env = JavaEnv::getEnv();

    jobject jExportTypeManager = JavaEnv::getExportTypeManager(env);
    jmethodID invokeMethodId = env -> GetMethodID(JavaType::exportTypeManagerClass(env), "classMethodRoute", "(Lcom/hacknife/lua/LuaContext;Ljava/lang/Class;Ljava/lang/String;[Lcom/hacknife/lua/LuaValue;)Lcom/hacknife/lua/LuaValue;");

    jobject jContext = JavaEnv::getJavaLuaContext(env, context);

    std::string methodNameString = StringUtils::format("%s_%s", name().c_str(), methodSignature().c_str());
    jstring methodName = env -> NewStringUTF(methodNameString.c_str());

    int index = 0;
    jobjectArray jArgs = env -> NewObjectArray((jsize)arguments.size(), JavaType::luaValueClass(env), NULL);
    for (LuaArgumentList::iterator it = arguments.begin(); it != arguments.end() ; ++it)
    {
        LuaValue *argItem = *it;
        jobject jArgItem = JavaConverter::convertToJavaLuaValueByLuaValue(env, context, argItem);
        env -> SetObjectArrayElement(jArgs, index, jArgItem);
        env -> DeleteLocalRef(jArgItem);

        index++;
    }

    JavaExportTypeDescriptor *javaTypeDescriptor = (JavaExportTypeDescriptor *)typeDescriptor;
    jobject jReturnValue = env -> CallObjectMethod(jExportTypeManager, invokeMethodId, jContext, javaTypeDescriptor -> getJavaType(), methodName, jArgs);

    env -> DeleteLocalRef(methodName);
    env -> DeleteLocalRef(jArgs);

    LuaValue *returnValue = JavaConverter::convertToLuaValueByJLuaValue(env, context, jReturnValue);

    env -> DeleteLocalRef(jReturnValue);

    JavaEnv::resetEnv(env);

    return returnValue;
}

LuaValue* JavaExportMethodDescriptor::invokeInstanceMethod(LuaSession *session, LuaArgumentList arguments)
{
    LuaContext *context = session -> getContext();

    JNIEnv *env = JavaEnv::getEnv();

    jobject jExportTypeManager = JavaEnv::getExportTypeManager(env);
    jmethodID invokeMethodId = env -> GetMethodID(JavaType::exportTypeManagerClass(env), "instanceMethodRoute", "(Lcom/hacknife/lua/LuaContext;Ljava/lang/Object;Ljava/lang/String;[Lcom/hacknife/lua/LuaValue;)Lcom/hacknife/lua/LuaValue;");

    jobject jContext = JavaEnv::getJavaLuaContext(env, context);

    std::string methodNameString = StringUtils::format("%s_%s", name().c_str(), methodSignature().c_str());
    jstring methodName = env -> NewStringUTF(methodNameString.c_str());

    LuaArgumentList::iterator it = arguments.begin();
    JavaObjectDescriptor *objectDescriptor = (JavaObjectDescriptor *)((*it) -> toObject());
    it ++;

    int index = 0;
    jobjectArray jArgs = env -> NewObjectArray((jsize)arguments.size() - 1, JavaType::luaValueClass(env), NULL);
    for (; it != arguments.end() ; ++it)
    {
        LuaValue *argItem = *it;
        jobject jArgItem = JavaConverter::convertToJavaLuaValueByLuaValue(env, context, argItem);
        env -> SetObjectArrayElement(jArgs, index, jArgItem);
        env -> DeleteLocalRef(jArgItem);

        index++;
    }

    JavaExportTypeDescriptor *javaTypeDescriptor = (JavaExportTypeDescriptor *)typeDescriptor;
    jobject jReturnValue = env -> CallObjectMethod(jExportTypeManager, invokeMethodId, jContext, objectDescriptor -> getJavaObject(), methodName, jArgs);

    env -> DeleteLocalRef(methodName);
    env -> DeleteLocalRef(jArgs);

    LuaValue *returnValue = JavaConverter::convertToLuaValueByJLuaValue(env, context, jReturnValue);

    env -> DeleteLocalRef(jReturnValue);

    JavaEnv::resetEnv(env);

    return returnValue;
}