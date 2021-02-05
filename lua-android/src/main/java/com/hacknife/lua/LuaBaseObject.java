package com.hacknife.lua;


public class LuaBaseObject {
    protected int _nativeId;


    public LuaBaseObject() {
        _nativeId = 0;
    }

    protected LuaBaseObject(int nativeId) {
        _nativeId = nativeId;
    }

    @Override
    protected void finalize() throws Throwable {
        if (_nativeId > 0) {
            LuaNativeUtil.releaseNativeObject(_nativeId);
        }
        super.finalize();
    }
}
