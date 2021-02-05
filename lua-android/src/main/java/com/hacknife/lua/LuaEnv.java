package com.hacknife.lua;

import android.content.Context;

public final class LuaEnv {

    private static final LuaEnv _env = new LuaEnv();

    public static LuaEnv defaultEnv() {
        return _env;
    }


    private Context _androidApplicationContext = null;

    public void setAndroidApplicationContext(Context context) {
        _androidApplicationContext = context.getApplicationContext();
    }

    public Context getAndroidApplicationContext() {
        return _androidApplicationContext;
    }
}
