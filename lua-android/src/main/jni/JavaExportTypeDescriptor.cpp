//
// Created by 冯鸿杰 on 2017/11/16.
//

#include <LuaEngineAdapter.hpp>
#include "JavaExportTypeDescriptor.h"
#include "JavaEnv.h"
#include "JavaObjectDescriptor.h"
#include "LuaSession.h"
#include "JavaType.h"
#include "JavaConverter.h"

JavaExportTypeDescriptor::JavaExportTypeDescriptor (std::string &typeName, JNIEnv *env, jclass jType, LuaExportTypeDescriptor *parentTypeDescriptor)
    : LuaExportTypeDescriptor(typeName, parentTypeDescriptor)
{
    _jType = (jclass)env -> NewWeakGlobalRef(jType);
}

JavaExportTypeDescriptor::~JavaExportTypeDescriptor()
{
    JNIEnv *env = JavaEnv::getEnv();
    env -> DeleteWeakGlobalRef((jobject)_jType);
    JavaEnv::resetEnv(env);
}

jclass JavaExportTypeDescriptor::getJavaType()
{
    return _jType;
}

LuaObjectDescriptor* JavaExportTypeDescriptor::createInstance(LuaSession *session)
{
    JNIEnv *env = JavaEnv::getEnv();

    //获取传入参数
    LuaArgumentList args;
    session -> parseArguments(args, 2);

    jobject jExportTypeManager = JavaEnv::getExportTypeManager(env);
    jclass jExportTypeManagerCls = JavaType::exportTypeManagerClass(env);
    jmethodID invokeMethodId = env -> GetMethodID(jExportTypeManagerCls, "constructorMethodRoute", "(Lcom/hacknife/lua/LuaContext;Ljava/lang/Class;[Lcom/hacknife/lua/LuaValue;)Lcom/hacknife/lua/LuaValue;");

    jobject jContext = JavaEnv::getJavaLuaContext(env, session -> getContext());


    int index = 0;
    jobjectArray jArgs = env -> NewObjectArray((jsize)args.size(), JavaType::luaValueClass(env), NULL);
    for (LuaArgumentList::iterator it = args.begin(); it != args.end(); ++it)
    {
        LuaValue *argItem = *it;
        jobject jArgItem = JavaConverter::convertToJavaLuaValueByLuaValue(env, session -> getContext(), argItem);
        env -> SetObjectArrayElement(jArgs, index, jArgItem);
        env -> DeleteLocalRef(jArgItem);

        index++;
    }

    jobject jReturnValue = env -> CallObjectMethod(jExportTypeManager, invokeMethodId, jContext, this -> getJavaType(), jArgs);

    env -> DeleteLocalRef(jArgs);

    JavaObjectDescriptor *objectDescriptor = NULL;
    LuaValue *returnValue = JavaConverter::convertToLuaValueByJLuaValue(env, session -> getContext(), jReturnValue);
    env -> DeleteLocalRef(jReturnValue);

    if (returnValue -> getType() != LuaValueTypeNil)
    {
        objectDescriptor = dynamic_cast<JavaObjectDescriptor *>(returnValue -> toObject());
        objectDescriptor -> retain();
    }
    else
    {
        session -> reportLuaException("Unsupported constructor method");
    }

    //释放参数对象
    for (LuaArgumentList::iterator it = args.begin(); it != args.end() ; ++it)
    {
        LuaValue *value = *it;
        value -> release();
    }

    returnValue -> release();

    JavaEnv::resetEnv(env);

    return objectDescriptor;
}

void JavaExportTypeDescriptor::destroyInstance(LuaSession *session, LuaObjectDescriptor *objectDescriptor)
{
    LuaExportTypeDescriptor::destroyInstance(session, objectDescriptor);
}