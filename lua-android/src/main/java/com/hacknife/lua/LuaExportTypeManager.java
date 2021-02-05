package com.hacknife.lua;

import java.lang.annotation.Annotation;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

class LuaExportTypeManager {

    private static LuaExportTypeManager _manager = new LuaExportTypeManager();

    private static HashMap<Class<? extends LuaExportType>, HashMap<String, Method>> _regClassMethods = new HashMap<>();

    private static HashMap<Class<? extends LuaExportType>, HashMap<String, Method>> _regInstanceMethods = new HashMap<>();

    private static HashMap<Class<? extends LuaExportType>, HashMap<String, Field>> _regFieldMethods = new HashMap<>();

    static LuaExportTypeManager getDefaultManager() {
        return _manager;
    }

    private LuaExportTypeConfig findExportTypeConfig(Class<? extends LuaExportType> t) {
        LuaExportTypeConfig exportTypeConfig = null;
        Annotation[] annotations = t.getDeclaredAnnotations();
        for (Annotation annotation : annotations) {
            if (annotation.annotationType().equals(LuaExportTypeConfig.class)) {
                exportTypeConfig = (LuaExportTypeConfig) annotation;
                break;
            }
        }

        return exportTypeConfig;
    }


    private boolean isExcludeField(Field field) {
        LuaExclude exclude = null;
        Annotation[] annotations = field.getAnnotations();
        for (Annotation annotation : annotations) {
            if (annotation.annotationType().equals(LuaExclude.class)) {
                return true;
            }
        }
        return false;
    }


    private boolean isExcludeMethod(Method method) {
        LuaExclude exclude = null;
        Annotation[] annotations = method.getAnnotations();
        for (Annotation annotation : annotations) {
            if (annotation.annotationType().equals(LuaExclude.class)) {
                return true;
            }
        }
        return false;
    }


    private boolean isExcludeConstructor(Constructor constructor) {
        LuaExclude exclude = null;
        Annotation[] annotations = constructor.getAnnotations();
        for (Annotation annotation : annotations) {
            if (annotation.annotationType().equals(LuaExclude.class)) {
                return true;
            }
        }

        return false;
    }

    private String getTypeSignature(Class type) {
        if (int.class.isAssignableFrom(type)) {
            return "i";
        } else if (short.class.isAssignableFrom(type)) {
            return "s";
        } else if (char.class.isAssignableFrom(type)) {
            return "c";
        } else if (long.class.isAssignableFrom(type)) {
            return "q";
        } else if (boolean.class.isAssignableFrom(type)) {
            return "B";
        } else if (float.class.isAssignableFrom(type)) {
            return "f";
        } else if (double.class.isAssignableFrom(type)) {
            return "d";
        }

        return "@";
    }

    private Constructor getConstructor(Class targetType, Class type, LuaValue[] arguments) {
        int targetMatchDegree = 0;
        Constructor targetConstructor = null;
        Constructor defaultConstructor = null;

        if (LuaExportType.class.isAssignableFrom(type)) {
            Constructor[] constructors = type.getConstructors();

            for (Constructor constructor : constructors) {
                if (isExcludeConstructor(constructor)) {
                    continue;
                }

                int matchDegree = 0;
                int index = 0;

                Class[] paramTypes = constructor.getParameterTypes();

                if (paramTypes.length == 0) {
                    defaultConstructor = constructor;
                }

                if (paramTypes.length != arguments.length) {
                    continue;
                }

                boolean hasMatch = true;
                for (Class paramType : paramTypes) {
                    LuaValue arg = arguments[index];

                    if (int.class.isAssignableFrom(paramType)
                            || long.class.isAssignableFrom(paramType)
                            || short.class.isAssignableFrom(paramType)
                            || Integer.class.isAssignableFrom(paramType)
                            || Short.class.isAssignableFrom(paramType)
                            || Long.class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.Integer || arg.valueType() == LuaValueType.Number) {
                            matchDegree++;
                        }
                    } else if (float.class.isAssignableFrom(paramType)
                            || double.class.isAssignableFrom(paramType)
                            || Float.class.isAssignableFrom(paramType)
                            || Double.class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.Number) {
                            matchDegree++;
                        }
                    } else if (boolean.class.isAssignableFrom(paramType)
                            || Boolean.class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.Boolean) {
                            matchDegree++;
                        }
                    } else if (String.class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.String) {
                            matchDegree++;
                        }
                    } else if (byte[].class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.Data || arg.valueType() == LuaValueType.String) {
                            matchDegree++;
                        } else {
                            hasMatch = false;
                        }
                    } else if (paramType.isArray() || List.class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.Array) {
                            matchDegree++;
                        } else {
                            hasMatch = false;
                        }
                    } else if (Map.class.isAssignableFrom(paramType)) {
                        if (arg.valueType() == LuaValueType.Map) {
                            matchDegree++;
                        } else {
                            hasMatch = false;
                        }
                    } else {
                        Object obj = arg.toObject();
                        if (obj.getClass().isAssignableFrom(paramType)) {
                            matchDegree++;
                        } else {
                            hasMatch = false;
                        }
                    }


                    if (!hasMatch) {
                        break;
                    }

                    index++;
                }

                if (hasMatch && matchDegree > targetMatchDegree) {
                    targetConstructor = constructor;
                    targetMatchDegree = matchDegree;
                }
            }

            if (targetConstructor == null) {
                targetConstructor = getConstructor(targetType, type.getSuperclass(), arguments);
            }

            if (targetConstructor == null && targetType == type) {
                targetConstructor = defaultConstructor;
            }
        }

        return targetConstructor;
    }

    LuaValue constructorMethodRoute(LuaContext context, Class<? extends LuaExportType> type, LuaValue[] arguments) {
        Constructor constructor = getConstructor(type, type, arguments);
        if (constructor != null) {
            ArrayList<Object> args = new ArrayList<>();
            Class[] paramTypes = constructor.getParameterTypes();
            for (int i = 0; i < paramTypes.length; i++) {
                args.add(getArgValue(paramTypes[i], arguments[i]));
            }

            try {
                Object instance = constructor.newInstance(args.toArray(new Object[0]));
                return new LuaValue(instance);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return new LuaValue();
    }


    LuaValue classMethodRoute(LuaContext context, Class<? extends LuaExportType> type, String methodName, LuaValue[] arguments) {
        try {
            if (_regClassMethods.containsKey(type) && _regClassMethods.get(type).containsKey(methodName)) {
                ArrayList argumentArray = new ArrayList();
                Method method = _regClassMethods.get(type).get(methodName);
                if (method == null) {
                    return new LuaValue();
                }

                Class<?>[] types = method.getParameterTypes();
                for (int i = 0; i < types.length; i++) {
                    LuaValue item = null;
                    if (arguments.length > i) {
                        item = arguments[i];
                    } else {
                        item = new LuaValue();
                    }

                    Class<?> paramType = types[i];
                    argumentArray.add(getArgValue(paramType, item));
                }

                Object retValue = method.invoke(type, argumentArray.toArray());

                return new LuaValue(retValue);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return new LuaValue();
    }


    LuaValue instanceMethodRoute(LuaContext context, Object instance, String methodName, LuaValue[] arguments) {
        try {
            Class type = instance.getClass();
            if (_regInstanceMethods.containsKey(type) && _regInstanceMethods.get(type).containsKey(methodName)) {
                ArrayList argumentArray = new ArrayList();
                Method method = _regInstanceMethods.get(type).get(methodName);
                if (method == null) {
                    return new LuaValue();
                }

                Class<?>[] types = method.getParameterTypes();
                for (int i = 0; i < types.length; i++) {
                    LuaValue item = null;
                    if (arguments.length > i) {
                        item = arguments[i];
                    } else {
                        item = new LuaValue();
                    }

                    Class<?> paramType = types[i];
                    argumentArray.add(getArgValue(paramType, item));

                }

                Object retValue = method.invoke(instance, argumentArray.toArray());

                return new LuaValue(retValue);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return new LuaValue();
    }

    LuaValue getterMethodRoute(LuaContext context, Object instance, String fieldName) {
        try {
            Class type = instance.getClass();
            if (_regFieldMethods.containsKey(type) && _regFieldMethods.get(type).containsKey(fieldName)) {
                Field field = _regFieldMethods.get(type).get(fieldName);
                if (field == null) {
                    return new LuaValue();
                }

                Object retValue = field.get(instance);

                return new LuaValue(retValue);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return new LuaValue();
    }

    void setterMethodRoute(LuaContext context, Object instance, String fieldName, LuaValue value) {
        try {
            Class type = instance.getClass();
            if (_regFieldMethods.containsKey(type) && _regFieldMethods.get(type).containsKey(fieldName)) {
                ArrayList argumentArray = new ArrayList();
                Field field = _regFieldMethods.get(type).get(fieldName);
                if (field == null) {
                    return;
                }

                field.set(instance, getArgValue(field.getType(), value));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private Object getArgValue(Class<?> argType, LuaValue value) {
        if (value == null || value.valueType() == LuaValueType.Nil) {
            return null;
        }

        if (int.class.isAssignableFrom(argType) || Integer.class.isAssignableFrom(argType)) {
            return (int) value.toInteger();
        } else if (long.class.isAssignableFrom(argType) || Long.class.isAssignableFrom(argType)) {
            return value.toInteger();
        } else if (float.class.isAssignableFrom(argType) || Float.class.isAssignableFrom(argType)) {
            return (float) value.toDouble();
        } else if (double.class.isAssignableFrom(argType) || Double.class.isAssignableFrom(argType)) {
            return value.toDouble();
        } else if (boolean.class.isAssignableFrom(argType) || Boolean.class.isAssignableFrom(argType)) {
            return value.toBoolean();
        } else if (String.class.isAssignableFrom(argType)) {
            return value.toString();
        } else if (byte[].class.isAssignableFrom(argType)) {
            return value.toByteArray();
        } else if (ArrayList.class.isAssignableFrom(argType)) {
            return value.toArrayList();
        } else if (HashMap.class.isAssignableFrom(argType)) {
            return value.toHashMap();
        } else if (argType.isArray()) {
            if (int[].class.isAssignableFrom(argType)) {
                ArrayList itemArr = value.toArrayList();
                int items[] = new int[itemArr.size()];
                for (int j = 0; j < itemArr.size(); j++) {
                    items[j] = ((Double) itemArr.get(j)).intValue();
                }
                return items;
            } else if (Integer[].class.isAssignableFrom(argType)) {
                ArrayList itemArr = value.toArrayList();
                Integer items[] = new Integer[itemArr.size()];
                for (int j = 0; j < itemArr.size(); j++) {
                    int item = ((Double) itemArr.get(j)).intValue();
                    items[j] = Integer.valueOf(item);
                }

                return items;
            } else if (Double[].class.isAssignableFrom(argType)) {
                return value.toArrayList().toArray(new Double[0]);
            } else if (double[].class.isAssignableFrom(argType)) {
                ArrayList itemArr = value.toArrayList();
                double items[] = new double[itemArr.size()];
                for (int j = 0; j < itemArr.size(); j++) {
                    items[j] = ((Double) itemArr.get(j)).doubleValue();
                }

                return items;
            } else if (Boolean[].class.isAssignableFrom(argType)) {
                return value.toArrayList().toArray(new Boolean[0]);
            } else if (boolean[].class.isAssignableFrom(argType)) {
                ArrayList itemArr = value.toArrayList();
                boolean items[] = new boolean[itemArr.size()];
                for (int j = 0; j < itemArr.size(); j++) {
                    items[j] = ((Boolean) itemArr.get(j)).booleanValue();
                }

                return items;
            } else {
                return value.toArrayList().toArray();
            }
        } else {
            return value.toObject();
        }
    }


    void exportType(LuaContext context, Class<? extends LuaExportType> t, Class<? extends LuaExportType> st) {
        String alias = t.getSimpleName();
        String typeName = t.getName();
        LuaExportTypeConfig typeConfig = findExportTypeConfig(t);

        String baseTypeName = null;
        if (st != null) {
            baseTypeName = st.getName();
        }

        HashSet<String> exportFieldSet = new HashSet<>();
        HashMap<String, Field> exportFields = new HashMap<>();
        HashMap<String, Method> exportClassMethods = new HashMap<>();
        HashMap<String, Method> exportInstanceMethods = new HashMap<>();

        Field[] fields = t.getFields();
        for (Field field : fields) {
            int modifier = field.getModifiers();
            if (Modifier.isStatic(modifier)) {
                continue;
            }

            if (!Modifier.isPublic(modifier)) {
                continue;
            }

            if (Modifier.isAbstract(modifier)) {
                continue;
            }

            String fieldName = field.getName();
            if (fieldName.startsWith("_")) {
                continue;
            }

            if (isExcludeField(field)) {
                continue;
            }

            if (typeConfig != null && typeConfig.excludeExportFieldNames().length > 0) {
                boolean isExclude = false;
                for (String name : typeConfig.excludeExportFieldNames()) {
                    if (name.equals(fieldName)) {
                        isExclude = true;
                        break;
                    }
                }

                if (isExclude) {
                    continue;
                }
            }

            exportFieldSet.add(String.format("%s_rw", fieldName));
            exportFields.put(fieldName, field);
        }

        Method[] methods = t.getMethods();
        for (Method method : methods) {
            int modifier = method.getModifiers();
            if (Modifier.isStatic(modifier)) {
                continue;
            }

            if (!Modifier.isPublic(modifier)) {
                continue;
            }

            if (Modifier.isAbstract(modifier)) {
                continue;
            }

            String methodName = method.getName();
            if (methodName.startsWith("_")) {
                continue;
            }

            if (methodName.equals("access$super")) {
                continue;
            }

            if (isExcludeMethod(method)) {
                continue;
            }

            if (typeConfig != null && typeConfig.excludeExportInstanceMethodsNames().length > 0) {
                boolean isExclude = false;
                for (String name : typeConfig.excludeExportInstanceMethodsNames()) {
                    if (name.equals(methodName)) {
                        isExclude = true;
                        break;
                    }
                }

                if (isExclude) {
                    continue;
                }
            }

            StringBuilder signatureStringBuilder = new StringBuilder();
            Class[] paramTypes = method.getParameterTypes();
            for (Class paramType : paramTypes) {
                signatureStringBuilder.append(getTypeSignature(paramType));
            }

            exportInstanceMethods.put(String.format("%s_%s", methodName, signatureStringBuilder.toString()), method);
        }

        methods = t.getMethods();
        for (Method method : methods) {
            String methodName = method.getName();

            if (methodName.startsWith("_")) {
                continue;
            }

            int modifier = method.getModifiers();
            if (Modifier.isStatic(modifier)
                    && Modifier.isPublic(modifier)
                    && !Modifier.isAbstract(modifier)
                    && !methodName.equals("access$super")
                    && !isExcludeMethod(method)) {
                if (typeConfig != null && typeConfig.excludeExportClassMethodNames().length > 0) {
                    boolean isExclude = false;
                    for (String name : typeConfig.excludeExportClassMethodNames()) {
                        if (name.equals(methodName)) {
                            isExclude = true;
                            break;
                        }
                    }

                    if (isExclude) {
                        continue;
                    }
                }

                StringBuilder signatureStringBuilder = new StringBuilder();
                Class[] paramTypes = method.getParameterTypes();
                for (Class paramType : paramTypes) {
                    signatureStringBuilder.append(getTypeSignature(paramType));
                }

                exportClassMethods.put(String.format("%s_%s", methodName, signatureStringBuilder.toString()), method);
            }
        }

        String[] exportClassMethodsArr = exportClassMethods.keySet().toArray(new String[0]);
        String[] exportInstanceMethodsArr = exportInstanceMethods.keySet().toArray(new String[0]);
        String[] exportFieldArr = exportFieldSet.toArray(new String[0]);

        if (LuaNativeUtil.registerType(
                context,
                alias,
                typeName,
                baseTypeName,
                t,
                exportFieldArr,
                exportInstanceMethodsArr,
                exportClassMethodsArr)) {
            if (exportClassMethods.size() > 0) {
                _regClassMethods.put(t, exportClassMethods);
            }
            if (exportInstanceMethods.size() > 0) {
                _regInstanceMethods.put(t, exportInstanceMethods);
            }
            if (exportFields.size() > 0) {
                _regFieldMethods.put(t, exportFields);
            }
        }
    }
}
