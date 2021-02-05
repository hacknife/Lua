//
// Created by 冯鸿杰 on 16/10/31.
//

#include "JavaObjectDescriptor.h"
#include "LuaExportTypeDescriptor.hpp"
#include "JavaEnv.h"
#include "AndroidLog.h"
#include "JavaType.h"
#include "LuaPointer.h"
#include "lua.hpp"

JavaObjectDescriptor::JavaObjectDescriptor(LuaContext *context, JNIEnv *env, jobject object)
    : LuaObjectDescriptor(context, (void *)(env -> NewGlobalRef(object)))
{

}

JavaObjectDescriptor::JavaObjectDescriptor(LuaContext *context, JNIEnv *env, jobject object, LuaExportTypeDescriptor *typeDescriptor)
    : LuaObjectDescriptor(context, (void *)(env -> NewGlobalRef(object)), typeDescriptor)
{

}

JavaObjectDescriptor::~JavaObjectDescriptor()
{
    JNIEnv *env = JavaEnv::getEnv();
    //移除引用
    env -> DeleteGlobalRef((jobject)getObject());
    JavaEnv::resetEnv(env);
}

jobject JavaObjectDescriptor::getJavaObject()
{
    return (jobject)getObject();
}