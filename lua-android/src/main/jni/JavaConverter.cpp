//
// Created by 冯鸿杰 on 16/9/30.
//

#include <stdint.h>
#include <LuaTuple.h>
#include "JavaConverter.h"
#include "JavaType.h"
#include "JavaObjectDescriptor.h"
#include "LuaExportsTypeManager.hpp"
#include "JavaEnv.h"
#include "LuaObjectManager.h"
#include "LuaFunction.h"
#include "LuaContext.h"
#include "LuaValue.h"
#include "LuaPointer.h"
#include "LuaTable.hpp"

LuaContext *JavaConverter::convertToContextByJLuaContext(JNIEnv *env, jobject context) {
    if (context != NULL) {
        jfieldID nativeIdFieldId = env->GetFieldID(JavaType::contextClass(env), "_nativeId", "I");
        jint nativeId = env->GetIntField(context, nativeIdFieldId);

        return (LuaContext *) LuaObjectManager::SharedInstance()->getObject(nativeId);
    }

    return NULL;
}

LuaScriptController *
JavaConverter::convertToScriptControllerByJScriptController(JNIEnv *env, jobject scriptController) {
    if (scriptController != NULL) {
        jfieldID nativeIdFieldId = env->GetFieldID(JavaType::scriptControllerClass(env),
                                                   "_nativeId", "I");
        jint nativeId = env->GetIntField(scriptController, nativeIdFieldId);

        return (LuaScriptController *) LuaObjectManager::SharedInstance()->getObject(nativeId);
    }

    return NULL;
}

LuaValue *
JavaConverter::convertToLuaValueByJObject(JNIEnv *env, LuaContext *context, jobject object) {
    LuaValue *value = NULL;

    if (env->IsSameObject(object, NULL) == JNI_TRUE) {
        value = LuaValue::NilValue();
    } else if (env->IsInstanceOf(object, JavaType::stringClass(env)) == JNI_TRUE) {
        //String类型
        jstring str = (jstring) object;
        const char *cstr = env->GetStringUTFChars(str, NULL);
        std::string valueStr = cstr;
        value = new LuaValue(valueStr);
        env->ReleaseStringUTFChars(str, cstr);
    } else if (env->IsInstanceOf(object, JavaType::integerClass(env)) == JNI_TRUE) {
        //Integer类型
        jmethodID intValueMethodId = env->GetMethodID(JavaType::integerClass(env), "intValue",
                                                      "()I");
        value = new LuaValue((long) env->CallIntMethod(object, intValueMethodId));
    } else if (env->IsInstanceOf(object, JavaType::longClass(env)) == JNI_TRUE) {
        //Long类型
        jmethodID valueMethodId = env->GetMethodID(JavaType::longClass(env), "longValue", "()J");
        value = new LuaValue((long) env->CallLongMethod(object, valueMethodId));
    } else if (env->IsInstanceOf(object, JavaType::doubleClass(env)) == JNI_TRUE) {
        //Double类型
        jmethodID doubleValueMethodId = env->GetMethodID(JavaType::doubleClass(env), "doubleValue",
                                                         "()D");
        value = new LuaValue(env->CallDoubleMethod(object, doubleValueMethodId));
    } else if (env->IsInstanceOf(object, JavaType::booleanClass(env)) == JNI_TRUE) {
        //Boolean类型
        jmethodID boolValueMethodId = env->GetMethodID(JavaType::booleanClass(env), "booleanValue",
                                                       "()Z");
        value = new LuaValue((bool) env->CallBooleanMethod(object, boolValueMethodId));
    } else if (env->IsInstanceOf(object, JavaType::bytesClass(env)) == JNI_TRUE) {
        //byte数组
        jbyteArray byteArr = (jbyteArray) object;
        jsize len = env->GetArrayLength(byteArr);
        jbyte bytes[len];
        env->GetByteArrayRegion(byteArr, 0, len, bytes);

        value = new LuaValue((const char *) bytes, (size_t) len);
    } else if (env->IsInstanceOf(object, JavaType::byteArrayClass(env)) == JNI_TRUE) {
        //byte数组
        jmethodID byteValueMethodId = env->GetMethodID(JavaType::byteClass(env), "byteValue",
                                                       "()B");

        jobjectArray byteArr = (jobjectArray) object;
        jsize len = env->GetArrayLength(byteArr);

        jbyte bytes[len];
        for (int i = 0; i < len; ++i) {
            jobject byteItem = env->GetObjectArrayElement(byteArr, i);
            bytes[i] = env->CallByteMethod(byteItem, byteValueMethodId);
            env->DeleteLocalRef(byteItem);
        }

        value = new LuaValue((const char *) bytes, (size_t) len);
    } else if (env->IsInstanceOf(object, JavaType::intsClass(env)) == JNI_TRUE) {
        //int数组
        jintArray intArr = (jintArray) object;
        jsize len = env->GetArrayLength(intArr);

        LuaValueList list;
        jint *intElms = env->GetIntArrayElements(intArr, NULL);
        for (int i = 0; i < len; i++) {
            LuaValue *elm = new LuaValue((long) *(intElms + i));
            list.push_back(elm);
        }
        env->ReleaseIntArrayElements(intArr, intElms, 0);

        value = new LuaValue(list);

    } else if (env->IsInstanceOf(object, JavaType::intArrayClass(env)) == JNI_TRUE) {
        //int数组
        jmethodID intValueMethodId = env->GetMethodID(JavaType::integerClass(env), "intValue",
                                                      "()I");

        jobjectArray intArr = (jobjectArray) object;
        jsize len = env->GetArrayLength(intArr);

        LuaValueList list;
        for (int i = 0; i < len; ++i) {
            jobject intItem = env->GetObjectArrayElement(intArr, i);

            jint val = env->CallIntMethod(intItem, intValueMethodId);
            LuaValue *elm = new LuaValue((long) val);
            list.push_back(elm);

            env->DeleteLocalRef(intItem);
        }

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::longsClass(env)) == JNI_TRUE) {
        //long数组
        jlongArray longArr = (jlongArray) object;
        jsize len = env->GetArrayLength(longArr);

        LuaValueList list;
        jlong *longElms = env->GetLongArrayElements(longArr, NULL);
        for (int i = 0; i < len; i++) {
            LuaValue *elm = new LuaValue((long) *(longElms + i));
            list.push_back(elm);
        }
        env->ReleaseLongArrayElements(longArr, longElms, 0);

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::longArrayClass(env)) == JNI_TRUE) {
        //long数组
        jmethodID valueMethodId = env->GetMethodID(JavaType::longClass(env), "longValue", "()J");

        jobjectArray intArr = (jobjectArray) object;
        jsize len = env->GetArrayLength(intArr);

        LuaValueList list;
        for (int i = 0; i < len; ++i) {
            jobject item = env->GetObjectArrayElement(intArr, i);

            jlong val = env->CallLongMethod(item, valueMethodId);
            LuaValue *elm = new LuaValue((long) val);
            list.push_back(elm);

            env->DeleteLocalRef(item);
        }

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::floatsClass(env)) == JNI_TRUE) {
        //float数组
        jfloatArray arr = (jfloatArray) object;
        jsize len = env->GetArrayLength(arr);

        LuaValueList list;
        jfloat *elms = env->GetFloatArrayElements(arr, NULL);
        for (int i = 0; i < len; i++) {
            LuaValue *elm = new LuaValue((double) *(elms + i));
            list.push_back(elm);
        }
        env->ReleaseFloatArrayElements(arr, elms, 0);

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::floatArrayClass(env)) == JNI_TRUE) {
        //float数组
        jmethodID valueMethodId = env->GetMethodID(JavaType::floatClass(env), "floatValue", "()F");

        jobjectArray arr = (jobjectArray) object;
        jsize len = env->GetArrayLength(arr);

        LuaValueList list;
        for (int i = 0; i < len; ++i) {
            jobject item = env->GetObjectArrayElement(arr, i);

            jfloat val = env->CallFloatMethod(item, valueMethodId);
            LuaValue *elm = new LuaValue((double) val);
            list.push_back(elm);

            env->DeleteLocalRef(item);
        }

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::doublesClass(env)) == JNI_TRUE) {
        //double数组
        jdoubleArray arr = (jdoubleArray) object;
        jsize len = env->GetArrayLength(arr);

        LuaValueList list;
        jdouble *elms = env->GetDoubleArrayElements(arr, NULL);
        for (int i = 0; i < len; i++) {
            LuaValue *elm = new LuaValue((double) *(elms + i));
            list.push_back(elm);
        }
        env->ReleaseDoubleArrayElements(arr, elms, 0);

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::doubleArrayClass(env)) == JNI_TRUE) {
        //double数组
        jmethodID valueMethodId = env->GetMethodID(JavaType::doubleClass(env), "doubleValue",
                                                   "()D");

        jobjectArray arr = (jobjectArray) object;
        jsize len = env->GetArrayLength(arr);

        LuaValueList list;
        for (int i = 0; i < len; ++i) {
            jobject item = env->GetObjectArrayElement(arr, i);

            jdouble val = env->CallDoubleMethod(item, valueMethodId);
            LuaValue *elm = new LuaValue((double) val);
            list.push_back(elm);

            env->DeleteLocalRef(item);
        }

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::booleansClass(env)) == JNI_TRUE) {
        //boolean数组
        jbooleanArray arr = (jbooleanArray) object;
        jsize len = env->GetArrayLength(arr);

        LuaValueList list;
        jboolean *elms = env->GetBooleanArrayElements(arr, NULL);
        for (int i = 0; i < len; i++) {
            LuaValue *elm = new LuaValue((bool) *(elms + i));
            list.push_back(elm);
        }
        env->ReleaseBooleanArrayElements(arr, elms, 0);

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::booleanArrayClass(env)) == JNI_TRUE) {
        //boolean数组
        jmethodID valueMethodId = env->GetMethodID(JavaType::booleanClass(env), "booleanValue",
                                                   "()Z");

        jobjectArray arr = (jobjectArray) object;
        jsize len = env->GetArrayLength(arr);

        LuaValueList list;
        for (int i = 0; i < len; ++i) {
            jobject item = env->GetObjectArrayElement(arr, i);

            jboolean val = env->CallBooleanMethod(item, valueMethodId);
            LuaValue *elm = new LuaValue((bool) val);
            list.push_back(elm);

            env->DeleteLocalRef(item);
        }

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::listClass(env)) == JNI_TRUE) {
        //ArrayList
        static jclass jListClass = JavaType::listClass(env);
        static jmethodID getMethodId = env->GetMethodID(jListClass, "get", "(I)Ljava/lang/Object;");
        static jmethodID sizeMethodId = env->GetMethodID(jListClass, "size", "()I");

        LuaValueList list;
        jint len = env->CallIntMethod(object, sizeMethodId);
        for (int i = 0; i < len; ++i) {
            jobject item = env->CallObjectMethod(object, getMethodId, i);
            LuaValue *valueItem = JavaConverter::convertToLuaValueByJObject(env, context, item);
            list.push_back(valueItem);
            env->DeleteLocalRef(item);
        }

        value = new LuaValue(list);
    } else if (env->IsInstanceOf(object, JavaType::mapClass(env)) == JNI_TRUE) {
        //HashMap
        static jclass jHashMapClass = JavaType::mapClass(env);
        static jmethodID getMethodId = env->GetMethodID(jHashMapClass, "get",
                                                        "(Ljava/lang/Object;)Ljava/lang/Object;");
        static jmethodID sizeMethodId = env->GetMethodID(jHashMapClass, "size", "()I");
        static jmethodID keySetMethodId = env->GetMethodID(jHashMapClass, "keySet",
                                                           "()Ljava/util/Set;");

        static jclass jSetClass = (jclass) env->NewGlobalRef(
                JavaEnv::findClass(env, "java/util/Set"));
        static jmethodID toArrayMethodId = env->GetMethodID(jSetClass, "toArray",
                                                            "()[Ljava/lang/Object;");

        LuaValueMap map;
        jint len = env->CallIntMethod(object, sizeMethodId);

        jobject keySet = env->CallObjectMethod(object, keySetMethodId);
        jobjectArray keys = (jobjectArray) env->CallObjectMethod(keySet, toArrayMethodId);

        for (int i = 0; i < len; ++i) {
            jobject key = env->GetObjectArrayElement(keys, i);
            jobject item = env->CallObjectMethod(object, getMethodId, key);

            const char *keyStr = env->GetStringUTFChars((jstring) key, NULL);
            LuaValue *valueItem = JavaConverter::convertToLuaValueByJObject(env, context, item);
            map[keyStr] = valueItem;
            env->ReleaseStringUTFChars((jstring) key, keyStr);

            env->DeleteLocalRef(item);
            env->DeleteLocalRef(key);
        }

        value = new LuaValue(map);

        env->DeleteLocalRef(keys);
        env->DeleteLocalRef(keySet);
    } else if (env->IsInstanceOf(object, JavaType::luaValueClass(env)) == JNI_TRUE) {
        //LuaValue类型
        value = JavaConverter::convertToLuaValueByJLuaValue(env, context, object);
    } else if (env->IsInstanceOf(object, JavaType::pointerClass(env)) == JNI_TRUE) {
        //LuaPointer类型
        //先查找LuaPointer是否有对应的本地对象
        jfieldID nativeIdFieldId = env->GetFieldID(JavaType::pointerClass(env), "_nativeId", "I");
        jint nativeId = env->GetIntField(object, nativeIdFieldId);
        LuaPointer *pointer = (LuaPointer *) LuaObjectManager::SharedInstance()->getObject(
                nativeId);
        if (pointer != NULL) {
            value = new LuaValue(pointer);
        } else {
            //没找到相关的Pointer则使用nil代替
            value = new LuaValue();
        }
    } else if (env->IsInstanceOf(object, JavaType::functionClass(env)) == JNI_TRUE) {
        //LuaFunction类型
        jfieldID nativeIdFieldId = env->GetFieldID(JavaType::functionClass(env), "_nativeId", "I");
        jint nativeId = env->GetIntField(object, nativeIdFieldId);
        LuaFunction *function = (LuaFunction *) LuaObjectManager::SharedInstance()->getObject(
                nativeId);
        if (function != NULL) {
            value = new LuaValue(function);
        } else {
            //没找到相关的Function则使用nil代替
            value = new LuaValue();
        }

    } else if (env->IsInstanceOf(object, JavaType::tupleClass(env)) == JNI_TRUE) {
        //LuaTuple类型
        jmethodID countMethodId = env->GetMethodID(JavaType::tupleClass(env), "count", "()I");
        jmethodID returnValueMethodId = env->GetMethodID(JavaType::tupleClass(env),
                                                         "getReturnValueByIndex",
                                                         "(I)Ljava/lang/Object;");
        jint count = env->CallIntMethod(object, countMethodId);

        LuaTuple *tuple = new LuaTuple();
        for (int i = 0; i < count; ++i) {
            jobject returnValue = env->CallObjectMethod(object, returnValueMethodId, i);
            LuaValue *value = JavaConverter::convertToLuaValueByJObject(env, context, returnValue);
            if (value != NULL) {
                tuple->addReturnValue(value);
                value->release();
            }
            env->DeleteLocalRef(returnValue);
        }

        value = new LuaValue(tuple);
        tuple->release();
    } else if (env->IsSameObject(object, NULL) != JNI_TRUE) {
        //对象类型
        //查找对象是否存在ObjectDescriptor
        bool needRelease = false;
        LuaObjectDescriptor *objDesc = JavaEnv::getAssociateInstanceRef(env, object);
        if (objDesc == NULL) {
            //不存在则创建对象
            jclass objType = env->GetObjectClass(object);
            jclass exportTypeCls = JavaType::luaExportTypeClass(env);
            if (env->IsAssignableFrom(objType, exportTypeCls)) {
                //为导出类型
                std::string typeName = JavaEnv::getJavaClassName(env, objType, false);
                LuaExportTypeDescriptor *typeDescriptor = context->getExportsTypeManager()->_getMappingType(
                        typeName);
                objDesc = new JavaObjectDescriptor(context, env, object, typeDescriptor);
            } else {
                objDesc = new JavaObjectDescriptor(context, env, object);
            }

            env->DeleteLocalRef(objType);

            needRelease = true;
        }

        value = new LuaValue(objDesc);

        if (needRelease) {
            objDesc->release();
        }
    }

    return value;
}

LuaValue *
JavaConverter::convertToLuaValueByJLuaValue(JNIEnv *env, LuaContext *context, jobject value) {
    //构造调用参数
    env = JavaEnv::getEnv();

    static jclass jLuaValueTypeClass = JavaType::luaValueTypeClass(env);
    static jmethodID typeValueMethodId = env->GetMethodID(jLuaValueTypeClass, "value", "()I");

    static jclass jLuaValueClass = JavaType::luaValueClass(env);
    static jmethodID typeMethodId = env->GetMethodID(jLuaValueClass, "valueType",
                                                     "()Lcom/hacknife/lua/LuaValueType;");
    static jmethodID toIntMethodId = env->GetMethodID(jLuaValueClass, "toInteger", "()J");
    static jmethodID toNumMethodId = env->GetMethodID(jLuaValueClass, "toDouble", "()D");
    static jmethodID toBoolMethodId = env->GetMethodID(jLuaValueClass, "toBoolean", "()Z");
    static jmethodID toStrMethodId = env->GetMethodID(jLuaValueClass, "toString",
                                                      "()Ljava/lang/String;");
    static jmethodID toByteArrMethodId = env->GetMethodID(jLuaValueClass, "toByteArray", "()[B");
    static jmethodID toListMethodId = env->GetMethodID(jLuaValueClass, "toList",
                                                       "()Ljava/util/List;");
    static jmethodID toMapMethodId = env->GetMethodID(jLuaValueClass, "toMap", "()Ljava/util/Map;");
    static jmethodID toPointerId = env->GetMethodID(jLuaValueClass, "toPointer",
                                                    "()Lcom/hacknife/lua/LuaPointer;");
    static jmethodID toFunctionId = env->GetMethodID(jLuaValueClass, "toFunction",
                                                     "()Lcom/hacknife/lua/LuaFunction;");
    static jmethodID toObjectId = env->GetMethodID(jLuaValueClass, "toObject",
                                                   "()Ljava/lang/Object;");
    static jmethodID toTupleId = env->GetMethodID(jLuaValueClass, "toTuple",
                                                  "()Lcom/hacknife/lua/LuaTuple;");

    static jfieldID tableIdFieldId = env->GetFieldID(jLuaValueClass, "_tableId", "I");

    jobject itemType = env->CallObjectMethod(value, typeMethodId);
    jint valueType = env->CallIntMethod(itemType, typeValueMethodId);
    env->DeleteLocalRef(itemType);

    LuaValue *retValue = NULL;
    switch (valueType) {
        case LuaValueTypeNil:
            retValue = LuaValue::NilValue();
            break;
        case LuaValueTypeInteger:
            retValue = LuaValue::IntegerValue(env->CallLongMethod(value, toIntMethodId));
            break;
        case LuaValueTypeNumber:
            retValue = LuaValue::NumberValue(env->CallDoubleMethod(value, toNumMethodId));
            break;
        case LuaValueTypeBoolean:
            retValue = LuaValue::BooleanValue(env->CallBooleanMethod(value, toBoolMethodId));
            break;
        case LuaValueTypeString: {
            jstring strValue = (jstring) env->CallObjectMethod(value, toStrMethodId);
            const char *charStr = env->GetStringUTFChars(strValue, NULL);
            retValue = LuaValue::StringValue(charStr);
            env->ReleaseStringUTFChars(strValue, charStr);
            env->DeleteLocalRef(strValue);
            break;
        }
        case LuaValueTypeData: {
            jbyteArray byteArr = (jbyteArray) env->CallObjectMethod(value, toByteArrMethodId);
            jsize len = env->GetArrayLength(byteArr);

            jbyte *buffer = new jbyte[len];
            env->GetByteArrayRegion(byteArr, 0, len, buffer);

            retValue = LuaValue::DataValue((const char *) buffer, (size_t) len);

            delete[] buffer;

            env->DeleteLocalRef(byteArr);

            break;
        }
        case LuaValueTypeArray: {
            //先检查是否存在tableId
            int tableId = env->GetIntField(value, tableIdFieldId);
            if (tableId > 0) {
                //直接获取LuaTable对象
                LuaTable *table = (LuaTable *) LuaObjectManager::SharedInstance()->getObject(
                        tableId);
                retValue = LuaValue::TableValue(table);
            } else {
                static jclass jListClass = JavaType::listClass(env);
                static jmethodID getMethodId = env->GetMethodID(jListClass, "get",
                                                                "(I)Ljava/lang/Object;");
                static jmethodID sizeMethodId = env->GetMethodID(jListClass, "size", "()I");

                LuaValueList list;
                jobject arrayList = env->CallObjectMethod(value, toListMethodId);
                jint len = env->CallIntMethod(arrayList, sizeMethodId);
                for (int i = 0; i < len; ++i) {
                    jobject item = env->CallObjectMethod(arrayList, getMethodId, i);
                    LuaValue *valueItem = JavaConverter::convertToLuaValueByJObject(env, context,
                                                                                    item);
                    list.push_back(valueItem);
                    env->DeleteLocalRef(item);
                }

                retValue = LuaValue::ArrayValue(list);

                env->DeleteLocalRef(arrayList);
            }
            break;
        }
        case LuaValueTypeMap: {
            int tableId = env->GetIntField(value, tableIdFieldId);
            if (tableId > 0) {
                //直接获取LuaTable对象
                LuaTable *table = (LuaTable *) LuaObjectManager::SharedInstance()->getObject(
                        tableId);
                retValue = LuaValue::TableValue(table);
            } else {
                static jclass jHashMapClass = JavaType::mapClass(env);
                static jmethodID getMethodId = env->GetMethodID(jHashMapClass, "get",
                                                                "(Ljava/lang/Object;)Ljava/lang/Object;");
                static jmethodID sizeMethodId = env->GetMethodID(jHashMapClass, "size", "()I");
                static jmethodID keySetMethodId = env->GetMethodID(jHashMapClass, "keySet",
                                                                   "()Ljava/util/Set;");

                static jclass jSetClass = (jclass) env->NewGlobalRef(
                        JavaEnv::findClass(env, "java/util/Set"));
                static jmethodID toArrayMethodId = env->GetMethodID(jSetClass, "toArray",
                                                                    "()[Ljava/lang/Object;");

                LuaValueMap map;
                jobject hashMap = env->CallObjectMethod(value, toMapMethodId);
                jint len = env->CallIntMethod(hashMap, sizeMethodId);

                jobject keySet = env->CallObjectMethod(hashMap, keySetMethodId);
                jobjectArray keys = (jobjectArray) env->CallObjectMethod(keySet, toArrayMethodId);

                for (int i = 0; i < len; ++i) {
                    jobject key = env->GetObjectArrayElement(keys, i);
                    jobject item = env->CallObjectMethod(hashMap, getMethodId, key);

                    const char *keyStr = env->GetStringUTFChars((jstring) key, NULL);
                    LuaValue *valueItem = JavaConverter::convertToLuaValueByJObject(env, context,
                                                                                    item);
                    map[keyStr] = valueItem;
                    env->ReleaseStringUTFChars((jstring) key, keyStr);

                    env->DeleteLocalRef(item);
                    env->DeleteLocalRef(key);
                }

                retValue = LuaValue::DictonaryValue(map);

                env->DeleteLocalRef(keys);
                env->DeleteLocalRef(keySet);
                env->DeleteLocalRef(hashMap);
            }

            break;
        }
        case LuaValueTypePtr: {
            jobject jPointer = env->CallObjectMethod(value, toPointerId);

            //先查找是否有对应的本地LuaPointer
            jfieldID nativeIdFieldId = env->GetFieldID(JavaType::pointerClass(env), "_nativeId",
                                                       "I");
            jint nativeId = env->GetIntField(jPointer, nativeIdFieldId);

            LuaPointer *pointer = (LuaPointer *) LuaObjectManager::SharedInstance()->getObject(
                    nativeId);
            if (pointer != NULL) {
                retValue = new LuaValue(pointer);
            } else {
                retValue = new LuaValue();
            }

            env->DeleteLocalRef(jPointer);

            break;
        }
        case LuaValueTypeFunction: {
            jobject jFunction = env->CallObjectMethod(value, toFunctionId);

            //先查找是否有对应的本地LuaFunction
            jfieldID nativeIdFieldId = env->GetFieldID(JavaType::functionClass(env), "_nativeId",
                                                       "I");
            jint nativeId = env->GetIntField(jFunction, nativeIdFieldId);

            LuaFunction *function = (LuaFunction *) LuaObjectManager::SharedInstance()->getObject(
                    nativeId);
            if (function != NULL) {
                retValue = new LuaValue(function);
            } else {
                retValue = new LuaValue();
            }

            env->DeleteLocalRef(jFunction);

            break;
        }
        case LuaValueTypeTuple: {
            //元组
            jobject jTuple = env->CallObjectMethod(value, toTupleId);

            jmethodID countMethodId = env->GetMethodID(JavaType::tupleClass(env), "count", "()I");
            jmethodID returnValueMethodId = env->GetMethodID(JavaType::tupleClass(env),
                                                             "getReturnValueByIndex",
                                                             "(I)Ljava/lang/Object;");
            jint count = env->CallIntMethod(jTuple, countMethodId);

            LuaTuple *tuple = new LuaTuple();
            for (int i = 0; i < count; ++i) {
                jobject returnValue = env->CallObjectMethod(jTuple, returnValueMethodId, i);
                LuaValue *item = JavaConverter::convertToLuaValueByJObject(env, context,
                                                                           returnValue);
                if (value != NULL) {
                    tuple->addReturnValue(item);
                    item->release();
                }

                env->DeleteLocalRef(returnValue);
            }

            retValue = new LuaValue(tuple);
            tuple->release();

            env->DeleteLocalRef(jTuple);

            break;
        }
        case LuaValueTypeObject: {
            jobject obj = env->CallObjectMethod(value, toObjectId);
            if (env->IsSameObject(obj, NULL) != JNI_TRUE) {
                bool needRelease = false;
                LuaObjectDescriptor *objDesc = JavaEnv::getAssociateInstanceRef(env, obj);
                if (objDesc == NULL) {
                    //不存在则创建对象
                    //判断是否为导出类型
                    jclass objType = env->GetObjectClass(obj);
                    jclass exportTypeCls = JavaType::luaExportTypeClass(env);
                    if (env->IsAssignableFrom(objType, exportTypeCls)) {
                        //为导出类型
                        std::string typeName = JavaEnv::getJavaClassName(env, objType, false);
                        LuaExportTypeDescriptor *typeDescriptor = context->getExportsTypeManager()->_getMappingType(
                                typeName);
                        objDesc = new JavaObjectDescriptor(context, env, obj, typeDescriptor);
                    } else {
                        objDesc = new JavaObjectDescriptor(context, env, obj);
                    }

                    env->DeleteLocalRef(objType);

                    needRelease = true;
                }

                retValue = new LuaValue(objDesc);

                if (needRelease) {
                    objDesc->release();
                }

                env->DeleteLocalRef(obj);
            }
            break;
        }
        default:
            break;
    }

    JavaEnv::resetEnv(env);

    return retValue;
}

jobject
JavaConverter::convertToJavaObjectByLuaValue(JNIEnv *env, LuaContext *context, LuaValue *luaValue) {
    jobject retObj = NULL;
    if (luaValue != NULL) {
        switch (luaValue->getType()) {
            case LuaValueTypeNumber: {
                static jmethodID initMethodId = env->GetMethodID(JavaType::doubleClass(env),
                                                                 "<init>", "(D)V");
                retObj = env->NewObject(JavaType::doubleClass(env), initMethodId,
                                        luaValue->toNumber());

                break;
            }
            case LuaValueTypeBoolean: {
                static jmethodID initMethodId = env->GetMethodID(JavaType::booleanClass(env),
                                                                 "<init>", "(Z)V");
                retObj = env->NewObject(JavaType::booleanClass(env), initMethodId,
                                        luaValue->toBoolean());

                break;
            }
            case LuaValueTypeString: {
                retObj = env->NewStringUTF(luaValue->toString().c_str());
                break;
            }
            case LuaValueTypeData: {
                jsize size = (jsize) luaValue->getDataLength();
                jbyteArray byteArray = env->NewByteArray(size);
                env->SetByteArrayRegion(byteArray, 0, size, (const jbyte *) luaValue->toData());

                retObj = byteArray;

                break;
            }
            case LuaValueTypeArray: {
                LuaValueList *list = luaValue->toArray();
                if (list != NULL) {
                    static jclass jArrayListClass = JavaType::arrayListClass(env);
                    static jmethodID initMethodId = env->GetMethodID(jArrayListClass, "<init>",
                                                                     "()V");
                    static jmethodID addMethodId = env->GetMethodID(jArrayListClass, "add",
                                                                    "(Ljava/lang/Object;)Z");

                    retObj = env->NewObject(jArrayListClass, initMethodId);
                    for (LuaValueList::iterator i = list->begin(); i != list->end(); ++i) {
                        LuaValue *item = *i;
                        jobject itemObj = JavaConverter::convertToJavaObjectByLuaValue(env, context,
                                                                                       item);
                        env->CallBooleanMethod(retObj, addMethodId, itemObj);
                        if (itemObj != NULL) {
                            env->DeleteLocalRef(itemObj);
                        }
                    }
                }
                break;
            }
            case LuaValueTypeMap: {
                LuaValueMap *map = luaValue->toMap();
                if (map != NULL) {
                    static jclass jHashMapClass = JavaType::hashMapClass(env);
                    static jmethodID initMethodId = env->GetMethodID(jHashMapClass, "<init>",
                                                                     "()V");
                    static jmethodID putMethodId = env->GetMethodID(jHashMapClass, "put",
                                                                    "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

                    retObj = env->NewObject(jHashMapClass, initMethodId);
                    for (LuaValueMap::iterator i = map->begin(); i != map->end(); ++i) {
                        std::string key = i->first;
                        LuaValue *item = i->second;

                        jstring keyStr = env->NewStringUTF(key.c_str());
                        jobject itemObj = JavaConverter::convertToJavaObjectByLuaValue(env, context,
                                                                                       item);
                        if (keyStr != NULL && itemObj != NULL) {
                            jobject tmpObj = env->CallObjectMethod(retObj, putMethodId, keyStr,
                                                                   itemObj);
                            env->DeleteLocalRef(tmpObj);
                        }

                        env->DeleteLocalRef(keyStr);
                        env->DeleteLocalRef(itemObj);
                    }
                }
                break;
            }
            case LuaValueTypePtr: {
                LuaPointer *pointer = luaValue->toPointer();
                if (pointer != NULL) {
                    int nativeId = LuaObjectManager::SharedInstance()->putObject(pointer);
                    //创建Java层的LuaPointer对象
                    jmethodID initMethodId = env->GetMethodID(JavaType::pointerClass(env), "<init>",
                                                              "(I)V");
                    retObj = env->NewObject(JavaType::pointerClass(env), initMethodId, nativeId);
                }
                break;
            }
            case LuaValueTypeFunction: {
                LuaFunction *function = luaValue->toFunction();
                if (function != NULL) {
                    int nativeId = LuaObjectManager::SharedInstance()->putObject(function);
                    jmethodID initMethodId = env->GetMethodID(JavaType::functionClass(env),
                                                              "<init>",
                                                              "(ILcom/hacknife/lua/LuaContext;)V");
                    jobject jcontext = JavaEnv::getJavaLuaContext(env, context);
                    retObj = env->NewObject(JavaType::functionClass(env), initMethodId, nativeId,
                                            jcontext);
                }
                break;
            }
            case LuaValueTypeTuple: {
                //元组
                LuaTuple *tuple = luaValue->toTuple();
                if (tuple != NULL) {
                    jmethodID initMethodId = env->GetMethodID(JavaType::tupleClass(env), "<init>",
                                                              "()V");
                    jmethodID addReturnValueMethodId = env->GetMethodID(JavaType::tupleClass(env),
                                                                        "addReturnValue",
                                                                        "(Ljava/lang/Object;)V");

                    retObj = env->NewObject(JavaType::tupleClass(env), initMethodId);

                    //填充数据
                    for (int i = 0; i < tuple->count(); ++i) {
                        LuaValue *retValue = tuple->getReturnValueByIndex(i);
                        jobject jRetValue = JavaConverter::convertToJavaObjectByLuaValue(env,
                                                                                         context,
                                                                                         retValue);
                        if (jRetValue != NULL) {
                            env->CallVoidMethod(retObj, addReturnValueMethodId, jRetValue);
                        }
                        env->DeleteLocalRef(jRetValue);
                    }
                }
                break;
            }
            case LuaValueTypeObject: {
                LuaObjectDescriptor *objDesc = luaValue->toObject();
                if (dynamic_cast<JavaObjectDescriptor *>(objDesc) != NULL) {
                    //如果为LuaJavaObjectDescriptor则转换为jobject类型
                    retObj = env->NewLocalRef((jobject) objDesc->getObject());
                }
                break;
            }
            default:
                break;
        }
    }

    return retObj;
}

jobject JavaConverter::convertToJavaLuaValueByLuaValue(JNIEnv *env, LuaContext *context,
                                                       LuaValue *luaValue) {
    jobject retObj = NULL;
    if (luaValue != NULL) {
        static jclass jLuaValue = JavaType::luaValueClass(env);
        static jmethodID jNilInitMethodId = env->GetMethodID(jLuaValue, "<init>", "(I)V");
        static jfieldID jContextFieldId = env->GetFieldID(jLuaValue, "_context",
                                                          "Lcom/hacknife/lua/LuaContext;");
        jmethodID initMethodId = jNilInitMethodId;

        int tableId = 0;

        switch (luaValue->getType()) {
            case LuaValueTypeNumber: {
                static jmethodID numberInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                       "(ILjava/lang/Double;)V");
                initMethodId = numberInitMethodId;
                break;
            }
            case LuaValueTypeBoolean: {
                static jmethodID boolInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                     "(ILjava/lang/Boolean;)V");
                initMethodId = boolInitMethodId;
                break;
            }
            case LuaValueTypeString: {
                static jmethodID stringInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                       "(ILjava/lang/String;)V");
                initMethodId = stringInitMethodId;
                break;
            }
            case LuaValueTypeData: {
                static jmethodID byteArrInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                        "(I[B)V");
                initMethodId = byteArrInitMethodId;
                break;
            }
            case LuaValueTypeArray: {
                static jmethodID arrayInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                      "(ILjava/util/List;)V");
                initMethodId = arrayInitMethodId;

                LuaTable *table = luaValue->toTable();
                tableId = LuaObjectManager::SharedInstance()->putObject(table);
                break;
            }
            case LuaValueTypeMap: {
                static jmethodID mapInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                    "(ILjava/util/Map;)V");
                initMethodId = mapInitMethodId;

                LuaTable *table = luaValue->toTable();
                tableId = LuaObjectManager::SharedInstance()->putObject(table);
                break;
            }
            case LuaValueTypePtr: {
                static jmethodID pointerInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                        "(ILcom/hacknife/lua/LuaPointer;)V");
                initMethodId = pointerInitMethodId;
                break;
            }
            case LuaValueTypeFunction: {
                static jmethodID functionInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                         "(ILcom/hacknife/lua/LuaFunction;)V");
                initMethodId = functionInitMethodId;
                break;
            }
            case LuaValueTypeTuple: {
                static jmethodID tupleInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                      "(ILcom/hacknife/lua/LuaTuple;)V");
                initMethodId = tupleInitMethodId;
                break;
            }
            case LuaValueTypeObject: {
                static jmethodID objectInitMethodId = env->GetMethodID(jLuaValue, "<init>",
                                                                       "(ILjava/lang/Object;)V");
                initMethodId = objectInitMethodId;
                break;
            }
            default:
                break;
        }

        if (luaValue->getType() == LuaValueTypeNil) {
            retObj = env->NewObject(jLuaValue, initMethodId, luaValue->objectId());
        } else {
            jobject jObj = JavaConverter::convertToJavaObjectByLuaValue(env, context, luaValue);
            retObj = env->NewObject(jLuaValue, initMethodId, luaValue->objectId(), jObj);
            env->DeleteLocalRef(jObj);

            if (tableId > 0) {
                //设置TableID到luaValue中
                static jfieldID tableIdFieldID = env->GetFieldID(jLuaValue, "_tableId", "I");
                env->SetIntField(retObj, tableIdFieldID, tableId);
            }
        }

        jobject jContext = JavaEnv::getJavaLuaContext(env, context);
        env->SetObjectField(retObj, jContextFieldId, jContext);
    }

    return retObj;
}
