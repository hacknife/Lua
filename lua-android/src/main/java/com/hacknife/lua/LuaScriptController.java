package com.hacknife.lua;

public class LuaScriptController extends LuaBaseObject {

    protected LuaScriptController(int nativeId) {
        super(nativeId);
    }

    public static LuaScriptController create() {
        return LuaNativeUtil.createScriptController();
    }

    public void setTimeout(int timeout) {
        LuaNativeUtil.scriptControllerSetTimeout(this, timeout);
    }

    public void forceExit() {
        LuaNativeUtil.scriptControllerForceExit(this);
    }
}
