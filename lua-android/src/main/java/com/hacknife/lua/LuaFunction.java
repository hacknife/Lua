package com.hacknife.lua;


public class LuaFunction extends LuaBaseObject {
    private LuaContext _context;


    protected LuaFunction(int nativeId, LuaContext context) {
        super(nativeId);
        _context = context;
    }


    public LuaValue invoke(LuaValue[] arguments) {
        return invoke(arguments, null);
    }

    public LuaValue invoke(LuaValue[] arguments, LuaScriptController scriptController) {
        return LuaNativeUtil.invokeFunction(_context, this, arguments, scriptController);
    }

    public LuaContext getContext() {
        return _context;
    }
}
