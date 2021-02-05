package com.hacknife.lua;

public interface LuaMethodHandler {

    public LuaValue onExecute(LuaValue[] arguments);
}
