package com.hacknife.lua;


public class LuaManagedValue extends LuaBaseObject {
    private LuaValue _source;
    private LuaContext _context;


    public LuaManagedValue(LuaValue value, LuaContext context) {
        _source = value;
        _context = context;
        context.retainValue(_source);
    }

    public LuaValue getSource() {
        return _source;
    }

    @Override
    protected void finalize() throws Throwable {
        _context.releaseValue(_source);
        super.finalize();
    }
}
