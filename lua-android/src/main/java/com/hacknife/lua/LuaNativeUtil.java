package com.hacknife.lua;


public class LuaNativeUtil {
    static {
        System.loadLibrary("lua");
    }

    private LuaNativeUtil() {
        throw new Error("Not allowed to instantiate the class");
    }


    public static native LuaContext createContext();


    public static native void addSearchPath(int contextNativeId, String path);


    public static native void setGlobal(int contextNativeId, String name, LuaValue value);

    public static native LuaValue getGlobal(int contextNativeId, String name);


    public static native void retainValue(LuaContext context, LuaValue value);

    public static native void releaseValue(LuaContext context, LuaValue value);


    public static native void raiseException(LuaContext context, String message);

    public static native void catchException(LuaContext context, boolean enabled);


    public static native LuaValue evalScript(int contextNativeId, String script, LuaScriptController scriptController);

    public static native LuaValue evalScriptFromFile(int contextNativeId, String path, LuaScriptController scriptController);

    public static native LuaValue callMethod(int contextNativeId, String methodName, LuaValue[] arguments, LuaScriptController scriptController);

    public static native void registerMethod(int contextNativeId, String methodName);

    public static native void releaseNativeObject(int nativeId);

    public static native LuaValue invokeFunction(
            LuaContext context,
            LuaFunction func,
            LuaValue[] arguments,
            LuaScriptController scriptController);

    public static native boolean registerType(
            LuaContext context,
            String alias,
            String typeName,
            String parentTypeName,
            Class type,
            String[] fields,
            String[] instanceMethods,
            String[] classMethods);

    public static native void runThread(LuaContext context,
                                        LuaFunction handler,
                                        LuaValue[] arguments,
                                        LuaScriptController scriptController);

    public static native LuaScriptController createScriptController();


    public static native void scriptControllerSetTimeout(LuaScriptController controller, int timeout);

    public static native void scriptControllerForceExit(LuaScriptController controller);

    public static native Object luaValueSetObject(LuaContext context, LuaValue value, String keyPath, LuaValue object);

}
