package com.hacknife.lua;

import java.util.ArrayList;

public class LuaTuple extends LuaBaseObject {
    private ArrayList<LuaValue> _returnValues;

    public LuaTuple() {
        _returnValues = new ArrayList<LuaValue>();
    }

    public int count() {
        return _returnValues.size();
    }

    public void addReturnValue(Object value) {
        _returnValues.add(new LuaValue(value));
    }

    public Object getReturnValueByIndex(int index) {
        return _returnValues.get(index).toObject();
    }
}
