//
// Created by 冯鸿杰 on 16/9/28.
//

#include <stdint.h>
#include "JavaType.h"
#include "JavaEnv.h"

jclass JavaType::fieldClass(JNIEnv *env)
{
    static jclass fieldClass = NULL;

    if (fieldClass == NULL)
    {
        jclass jTmpClass = JavaEnv::findClass(env, "java/lang/reflect/Field");
        fieldClass = (jclass)env -> NewGlobalRef(jTmpClass);
        env -> DeleteLocalRef(jTmpClass);
    }

    return fieldClass;
}

jclass JavaType::methodClass(JNIEnv *env)
{
    static jclass methodClass = NULL;

    if (methodClass == NULL)
    {
        jclass jTmpClass = JavaEnv::findClass(env, "java/lang/reflect/Method");
        methodClass = (jclass)env -> NewGlobalRef(jTmpClass);
        env -> DeleteLocalRef(jTmpClass);
    }

    return methodClass;
}

jclass JavaType::contextClass(JNIEnv *env)
{
    static jclass jLuaContext = NULL;

    if (jLuaContext == NULL)
    {
        jclass jLuaContextCls = JavaEnv::findClass(env, "com/hacknife/lua/LuaContext");
        jLuaContext = (jclass)env -> NewGlobalRef(jLuaContextCls);
        env -> DeleteLocalRef(jLuaContextCls);
    }

    return jLuaContext;
}

jclass JavaType::scriptControllerClass(JNIEnv *env)
{
    static jclass jLuaScriptController = NULL;

    if (jLuaScriptController == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaScriptController");
        jLuaScriptController = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jLuaScriptController;
}

jclass JavaType::threadClass(JNIEnv *env)
{
    static jclass jLuaThread = NULL;

    if (jLuaThread == NULL)
    {
        jclass jLuaThreadCls = JavaEnv::findClass(env, "com/hacknife/lua/LuaThread");
        jLuaThread = (jclass)env -> NewGlobalRef(jLuaThreadCls);
        env -> DeleteLocalRef(jLuaThreadCls);
    }

    return jLuaThread;
}

jclass JavaType::moduleClass(JNIEnv *env)
{
    static jclass jLuaModule = NULL;

    if (jLuaModule == NULL)
    {
        jclass jTmpClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaModule");
        jLuaModule = (jclass)env -> NewGlobalRef(jTmpClass);
        env -> DeleteLocalRef(jTmpClass);
    }

    return jLuaModule;
}

jclass JavaType::luaValueClass(JNIEnv *env)
{
    static jclass jLuaValue = NULL;

    if (jLuaValue == NULL)
    {
        jclass jLuaValueCls = JavaEnv::findClass(env, "com/hacknife/lua/LuaValue");
        jLuaValue = (jclass)env -> NewGlobalRef(jLuaValueCls);
        env -> DeleteLocalRef(jLuaValueCls);
    }

    return jLuaValue;
}

jclass JavaType::luaValueTypeClass(JNIEnv *env)
{
    static jclass jLuaValueType = NULL;

    if (jLuaValueType == NULL)
    {
        jclass jLuaValueCls = JavaEnv::findClass(env, "com/hacknife/lua/LuaValueType");
        jLuaValueType = (jclass)env -> NewGlobalRef(jLuaValueCls);
        env -> DeleteLocalRef(jLuaValueCls);
    }

    return jLuaValueType;
}

jclass JavaType::stringClass(JNIEnv *env)
{
    static jclass jStringCls = NULL;

    if (jStringCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/String");
        jStringCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jStringCls;
}

jclass JavaType::shortClass(JNIEnv *env)
{
    static jclass jShortCls = NULL;

    if (jShortCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Short");
        jShortCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jShortCls;
}

jclass JavaType::integerClass(JNIEnv *env)
{
    static jclass jIntegerCls = NULL;

    if (jIntegerCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Integer");
        jIntegerCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jIntegerCls;
}

jclass JavaType::doubleClass(JNIEnv *env)
{
    static jclass jDoubleCls = NULL;

    if (jDoubleCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Double");
        jDoubleCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jDoubleCls;
}

jclass JavaType::floatClass(JNIEnv *env)
{
    static jclass jFloatCls = NULL;

    if (jFloatCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Float");
        jFloatCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jFloatCls;
}

jclass JavaType::longClass(JNIEnv *env)
{
    static jclass jLongCls = NULL;

    if (jLongCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Long");
        jLongCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jLongCls;
}

jclass JavaType::booleanClass(JNIEnv *env)
{
    static jclass jBooleanCls = NULL;

    if (jBooleanCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Boolean");
        jBooleanCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jBooleanCls;
}

jclass JavaType::byteClass(JNIEnv *env)
{
    static jclass jByteCls = NULL;

    if (jByteCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "java/lang/Byte");
        jByteCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jByteCls;
}

jclass JavaType::bytesClass(JNIEnv *env)
{
    static jclass jByteArrayCls = NULL;

    if (jByteArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[B");
        jByteArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jByteArrayCls;
}

jclass JavaType::byteArrayClass(JNIEnv *env)
{
    static jclass jByteArrayCls = NULL;

    if (jByteArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Ljava/lang/Byte;");
        jByteArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jByteArrayCls;
}

jclass JavaType::intsClass(JNIEnv *env)
{
    static jclass jIntArrayCls = NULL;

    if (jIntArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[I");
        jIntArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jIntArrayCls;
}

jclass JavaType::intArrayClass(JNIEnv *env)
{
    static jclass jIntArrayCls = NULL;

    if (jIntArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Ljava/lang/Integer;");
        jIntArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jIntArrayCls;
}

jclass JavaType::longsClass(JNIEnv *env)
{
    static jclass jLongArrayCls = NULL;

    if (jLongArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[J");
        jLongArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jLongArrayCls;
}

jclass JavaType::longArrayClass(JNIEnv *env)
{
    static jclass jLongArrayCls = NULL;

    if (jLongArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Ljava/lang/Long;");
        jLongArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jLongArrayCls;
}

jclass JavaType::floatsClass(JNIEnv *env)
{
    static jclass jFloatArrayCls = NULL;

    if (jFloatArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[F");
        jFloatArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jFloatArrayCls;
}

jclass JavaType::floatArrayClass(JNIEnv *env)
{
    static jclass jFloatArrayCls = NULL;

    if (jFloatArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Ljava/lang/Float;");
        jFloatArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jFloatArrayCls;
}

jclass JavaType::doublesClass(JNIEnv *env)
{
    static jclass jDoubleArrayCls = NULL;

    if (jDoubleArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[D");
        jDoubleArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jDoubleArrayCls;
}

jclass JavaType::doubleArrayClass(JNIEnv *env)
{
    static jclass jDoubleArrayCls = NULL;

    if (jDoubleArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Ljava/lang/Double;");
        jDoubleArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jDoubleArrayCls;
}

jclass JavaType::booleansClass(JNIEnv *env)
{
    static jclass jBoolArrayCls = NULL;

    if (jBoolArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Z");
        jBoolArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jBoolArrayCls;
}

jclass JavaType::booleanArrayClass(JNIEnv *env)
{
    static jclass jBoolArrayCls = NULL;

    if (jBoolArrayCls == NULL)
    {
        jclass tmpClass = JavaEnv::findClass(env, "[Ljava/lang/Boolean;");
        jBoolArrayCls = (jclass)env -> NewGlobalRef(tmpClass);
        env -> DeleteLocalRef(tmpClass);
    }

    return jBoolArrayCls;
}

jclass JavaType::arrayListClass(JNIEnv *env)
{
    static jclass jArrayList = NULL;

    if (jArrayList == NULL)
    {
        jclass  jArrayListCls = JavaEnv::findClass(env, "java/util/ArrayList");
        jArrayList = (jclass)env -> NewGlobalRef(jArrayListCls);
        env -> DeleteLocalRef(jArrayListCls);
    }

    return jArrayList;
}

jclass JavaType::listClass(JNIEnv *env)
{
    static jclass jList = NULL;

    if (jList == NULL)
    {
        jclass  jListCls = JavaEnv::findClass(env, "java/util/List");
        jList = (jclass)env -> NewGlobalRef(jListCls);
        env -> DeleteLocalRef(jListCls);
    }

    return jList;
}

jclass JavaType::hashMapClass(JNIEnv *env)
{
    static jclass jHashMap = NULL;

    if (jHashMap == NULL)
    {
        jclass jHashMapCls = JavaEnv::findClass(env, "java/util/HashMap");
        jHashMap = (jclass)env -> NewGlobalRef(jHashMapCls);
        env -> DeleteLocalRef(jHashMapCls);
    }

    return jHashMap;
}

jclass JavaType::mapClass(JNIEnv *env)
{
    static jclass jMap = NULL;

    if (jMap == NULL)
    {
        jclass jMapCls = JavaEnv::findClass(env, "java/util/Map");
        jMap = (jclass)env -> NewGlobalRef(jMapCls);
        env -> DeleteLocalRef(jMapCls);
    }

    return jMap;
}

jclass JavaType::luaBaseObjectClass(JNIEnv *env)
{
    static jclass jLuaBaseObject = NULL;

    if (jLuaBaseObject == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaBaseObject");
        jLuaBaseObject = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jLuaBaseObject;
}

jclass JavaType::luaObjectClass(JNIEnv *env)
{
    static jclass jLuaObject = NULL;

    if (jLuaObject == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/modules/oo/LuaObjectClass");
        jLuaObject = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jLuaObject;
}

jclass JavaType::pointerClass(JNIEnv *env)
{
    static jclass jPointer = NULL;

    if (jPointer == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaPointer");
        jPointer = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jPointer;
}

jclass JavaType::functionClass(JNIEnv *env)
{
    static jclass jFunction = NULL;

    if (jFunction == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaFunction");
        jFunction = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jFunction;
}

jclass JavaType::tupleClass(JNIEnv *env)
{
    static jclass jTuple = NULL;

    if (jTuple == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaTuple");
        jTuple = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jTuple;
}

jclass JavaType::exportTypeManagerClass(JNIEnv *env)
{
    static jclass jExportTypeManagerCls = NULL;

    if (jExportTypeManagerCls == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaExportTypeManager");
        jExportTypeManagerCls = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jExportTypeManagerCls;
}

jclass JavaType::luaExportTypeClass(JNIEnv *env)
{
    static jclass jExportTypeCls = NULL;

    if (jExportTypeCls == NULL)
    {
        jclass jTempClass = JavaEnv::findClass(env, "com/hacknife/lua/LuaExportType");
        jExportTypeCls = (jclass)env -> NewGlobalRef(jTempClass);
        env -> DeleteLocalRef(jTempClass);
    }

    return jExportTypeCls;
}