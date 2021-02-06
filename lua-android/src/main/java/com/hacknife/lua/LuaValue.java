package com.hacknife.lua;

import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class LuaValue extends LuaBaseObject {
    private Object _valueContainer;
    private LuaValueType _type;
    private LuaContext _context;
    private int _tableId;

    @Override
    protected void finalize() throws Throwable {
        if (_tableId > 0) {
             LuaNativeUtil.releaseNativeObject(_tableId);
        }
        super.finalize();
    }

    public LuaValue() {
        super();
        setNilValue();
    }


    protected LuaValue(int nativeId) {
        super(nativeId);
        setNilValue();
    }

    private void setNilValue() {
        _type = LuaValueType.Nil;
        _valueContainer = null;
    }


    public LuaValue(Integer value) {
        super();
        setLongValue(Long.valueOf(value));
    }

    protected LuaValue(int nativeId, Integer value) {
        super(nativeId);
        setLongValue(Long.valueOf(value));
    }

    public LuaValue(Long value) {
        super();
        setLongValue(value);
    }


    private void setLongValue(Long value) {
        _type = LuaValueType.Integer;
        _valueContainer = value;
    }

    public LuaValue(Double value) {
        super();
        setDoubValue(value);

    }

    protected LuaValue(int nativeId, Double value) {
        super(nativeId);
        setDoubValue(value);
    }

    private void setDoubValue(Double value) {
        _type = LuaValueType.Number;
        _valueContainer = value;
    }

    public LuaValue(Boolean value) {
        super();
        setBoolValue(value);
    }

    protected LuaValue(int nativeId, Boolean value) {
        super(nativeId);
        setBoolValue(value);
    }

    private void setBoolValue(Boolean value) {
        _type = LuaValueType.Boolean;
        _valueContainer = value;
    }

    public LuaValue(String value) {
        super();
        setStringValue(value);
    }


    protected LuaValue(int nativeId, String value) {
        super(nativeId);
        setStringValue(value);
    }

    private void setStringValue(String value) {
        _type = LuaValueType.String;
        _valueContainer = value;
    }

    public LuaValue(byte[] value) {
        super();
        setByteArrayValue(value);
    }

    protected LuaValue(int nativeId, byte[] value) {
        super(nativeId);
        setByteArrayValue(value);
    }

    private void setByteArrayValue(byte[] value) {
        _type = LuaValueType.Data;
        _valueContainer = value;
    }

    public LuaValue(Byte[] value) {
        super();
        setByteArrayValue(value);
    }


    private void setByteArrayValue(Byte[] value) {
        _type = LuaValueType.Data;
        _valueContainer = value;
    }

    public LuaValue(List<?> value) {
        super();
        setArrayListValue(value);
    }

    protected LuaValue(int nativeId, List<?> value) {
        super(nativeId);
        setArrayListValue(value);
    }

    private void setArrayListValue(List<?> value) {
        _type = LuaValueType.Array;
        _valueContainer = value;
    }

    public LuaValue(Map<?, ?> value) {
        super();
        setHasMapValue(value);
    }

    protected LuaValue(int nativeId, Map<?, ?> value) {
        super(nativeId);
        setHasMapValue(value);
    }

    private void setHasMapValue(Map<?, ?> value) {
        _type = LuaValueType.Map;
        _valueContainer = value;
    }

    public LuaValue(LuaPointer value) {
        super();
        setPointerValue(value);
    }

    protected LuaValue(int nativeId, LuaPointer value) {
        super(nativeId);
        setPointerValue(value);
    }

    private void setPointerValue(LuaPointer value) {
        _type = LuaValueType.Pointer;
        _valueContainer = value;
    }

    public LuaValue(LuaFunction value) {
        super();
        setFunctionValue(value);
    }

    protected LuaValue(int nativeId, LuaFunction value) {
        super(nativeId);
        setFunctionValue(value);
    }

    private void setFunctionValue(LuaFunction value) {
        _type = LuaValueType.Function;
        _valueContainer = value;
    }

    public LuaValue(LuaTuple value) {
        super();
        setTupleValue(value);
    }

    protected LuaValue(int nativeId, LuaTuple value) {
        super(nativeId);
        setTupleValue(value);
    }

    private void setTupleValue(LuaTuple value) {
        _type = LuaValueType.Tuple;
        _valueContainer = value;
    }

    public LuaValue(Object value) {
        super();
        setObjectValue(value);
    }

    protected LuaValue(int nativeId, Object value) {
        super(nativeId);
        setObjectValue(value);
    }


    private void setObjectValue(Object value) {
        if (value != null) {
            if (value instanceof Character) {
                setLongValue(Long.valueOf((Character) value));
            } else if (value instanceof Byte) {
                setLongValue(Long.valueOf((Byte) value));
            } else if (value instanceof Short) {
                setLongValue(Long.valueOf((Short) value));
            } else if (value instanceof Integer) {
                setLongValue(Long.valueOf((Integer) value));
            } else if (value instanceof Long) {
                setLongValue((Long) value);
            } else if (value instanceof Float) {
                setDoubValue(((Float) value).doubleValue());
            } else if (value instanceof Double) {
                setDoubValue((Double) value);
            } else if (value instanceof Boolean) {
                setBoolValue((Boolean) value);
            } else if (value instanceof String) {
                setStringValue(value.toString());
            } else if (value instanceof byte[]) {
                setByteArrayValue((byte[]) value);
            } else if (value instanceof List<?>) {
                setArrayListValue((List<?>) value);
            } else if (value instanceof Map<?, ?>) {
                setHasMapValue((Map<?, ?>) value);
            } else if (value instanceof LuaPointer) {
                setPointerValue((LuaPointer) value);
            } else if (value instanceof LuaFunction) {
                setFunctionValue((LuaFunction) value);
            } else if (value instanceof LuaTuple) {
                setTupleValue((LuaTuple) value);
            } else {
                _type = LuaValueType.Object;
                _valueContainer = value;
            }
        } else {
            _type = LuaValueType.Nil;
            _valueContainer = null;
        }
    }


    public LuaValueType valueType() {
        return _type;
    }

    public long toInteger() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof Number) {
                return ((Number) _valueContainer).longValue();
            } else if (_valueContainer instanceof String) {
                return Integer.parseInt((String) _valueContainer);
            }
        }

        return 0;
    }


    public double toDouble() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof Number) {
                return ((Number) _valueContainer).doubleValue();
            } else if (_valueContainer instanceof String) {
                return Double.parseDouble((String) _valueContainer);
            }
        }

        return 0.0;
    }


    public boolean toBoolean() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof Boolean) {
                return (Boolean) _valueContainer;
            } else if (_valueContainer instanceof String) {
                return Boolean.parseBoolean((String) _valueContainer);
            }
        }

        return false;
    }

    public String toString() {
        if (_valueContainer != null) {
            return _valueContainer.toString();
        }

        return "";
    }

    public byte[] toByteArray() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof byte[]) {
                return (byte[]) _valueContainer;
            } else if (_valueContainer instanceof Byte[]) {
                final Byte[] srcBytes = (Byte[]) _valueContainer;
                byte[] bytes = new byte[srcBytes.length];
                for (int i = 0; i < srcBytes.length; i++) {
                    bytes[i] = srcBytes[i];
                }

                return bytes;
            } else if (_valueContainer instanceof String) {
                return ((String) _valueContainer).getBytes();
            }
        }

        return null;
    }

    public ArrayList toArrayList() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof ArrayList) {
                return (ArrayList) _valueContainer;
            }
        }

        return null;
    }

    public List<?> toList() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof List<?>) {
                return (List<?>) _valueContainer;
            }
        }

        return null;
    }


    public HashMap toHashMap() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof HashMap) {
                return (HashMap) _valueContainer;
            }
        }

        return null;
    }


    public Map<?, ?> toMap() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof Map<?, ?>) {
                return (Map<?, ?>) _valueContainer;
            }
        }

        return null;
    }

    public LuaPointer toPointer() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof LuaPointer) {
                return (LuaPointer) _valueContainer;
            }
        }

        return null;
    }

    public LuaFunction toFunction() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof LuaFunction) {
                return (LuaFunction) _valueContainer;
            }
        }

        return null;
    }

    public LuaTuple toTuple() {
        if (_valueContainer != null) {
            if (_valueContainer instanceof LuaTuple) {
                return (LuaTuple) _valueContainer;
            }
        }

        return null;
    }


    public Object toObject() {
        return _valueContainer;
    }

    public void setObject(String keyPath, Object object) {
        if (valueType() == LuaValueType.Map) {
            _valueContainer = LuaNativeUtil.luaValueSetObject(_context, this, keyPath, new LuaValue(object));
        }
    }
}
