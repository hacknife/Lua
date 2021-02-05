package com.hacknife.lua;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Pattern;

import static android.os.Environment.MEDIA_MOUNTED;


public class LuaContext extends LuaBaseObject {
    private Context _context;
    private LuaExceptionHandler _exceptionHandler;
    private HashMap<String, LuaMethodHandler> _methods;
    private ArrayList<Class<? extends LuaExportType>> _regTypes = null;


    private static boolean _isSetupLuaFolder = false;


    Context getApplicationContext() {
        return _context.getApplicationContext();
    }


    private void setupLuaFolder() {
        if (!_isSetupLuaFolder) {
            copyLuaFileFromAssets("");
            _isSetupLuaFolder = true;
        }
    }


    private File getCacheDir() {
        File appCacheDir = null;

        int perm = _context.checkCallingOrSelfPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if (MEDIA_MOUNTED.equals(Environment.getExternalStorageState()) && perm == PackageManager.PERMISSION_GRANTED) {
            appCacheDir = _context.getExternalCacheDir();
        }

        if (appCacheDir == null) {

            appCacheDir = _context.getCacheDir();
        }

        return appCacheDir;
    }


    private void copyLuaFileFromAssets(String path) {
        try {
            String[] paths = _context.getAssets().list(path);
            if (paths.length > 0) {
                for (String subPath : paths) {
                    copyLuaFileFromAssets(String.format("%s%s/", path, subPath));
                }
            } else {
                String fileName = path.substring(0, path.length() - 1);
                if (fileName.toLowerCase().endsWith(".lua")) {
                    ByteArrayOutputStream dataStream = new ByteArrayOutputStream();
                    try {
                        String filePath = String.format("%s/lua/%s", getCacheDir(), fileName);
                        File file = new File(filePath);
                        File parentFile = file.getParentFile();
                        if (!parentFile.exists()) {
                            parentFile.mkdirs();
                        }

                        readAssetFileContent(fileName, dataStream);
                        writeToFile(file, dataStream);
                        dataStream.close();

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    protected LuaContext(int nativeId) {
        super(nativeId);

        this._regTypes = new ArrayList<>();
        this._methods = new HashMap<>();
    }


    public static LuaContext create(LuaEnv env) {
        LuaContext luaContext = LuaNativeUtil.createContext();
        luaContext._context = env.getAndroidApplicationContext();

        File cacheDir = new File(String.format("%s/lua", luaContext.getCacheDir()));
        if (!cacheDir.exists()) {
            cacheDir.mkdirs();
        }
        luaContext.addSearchPath(cacheDir.toString());

        return luaContext;
    }


    public static LuaContext create(Context context) {
        LuaEnv env = LuaEnv.defaultEnv();
        env.setAndroidApplicationContext(context);
        return LuaContext.create(env);
    }


    public void onException(LuaExceptionHandler handler) {
        _exceptionHandler = handler;
        LuaNativeUtil.catchException(this, _exceptionHandler != null);
    }

    public void raiseException(String message) throws Error {
        LuaNativeUtil.raiseException(this, message);
    }

    public void addSearchPath(String path) {
        String regExp = "/([^/]+)[.]([^/]+)$";
        if (!Pattern.matches(regExp, path)) {
            if (!path.endsWith("/")) {
                path = path + "/";
            }

            path = path + "?.lua";
        }

        LuaNativeUtil.addSearchPath(_nativeId, path);
    }


    public void setGlobal(String name, LuaValue value) {
        LuaNativeUtil.setGlobal(_nativeId, name, value);
    }


    public LuaValue getGlobal(String name) {
        return LuaNativeUtil.getGlobal(_nativeId, name);
    }


    public void retainValue(LuaValue value) {
        LuaNativeUtil.retainValue(this, value);
    }


    public void releaseValue(LuaValue value) {
        LuaNativeUtil.releaseValue(this, value);
    }


    public LuaValue evalScript(String script) {
        return evalScript(script, null);
    }


    public LuaValue evalScript(String script, LuaScriptController scriptController) {

        setupLuaFolder();

        return LuaNativeUtil.evalScript(_nativeId, script, scriptController);
    }

    public LuaValue evalScriptFromFile(String filePath) {
        return evalScriptFromFile(filePath, null);
    }


    public LuaValue evalScriptFromFile(String filePath, LuaScriptController scriptController) {

        setupLuaFolder();

        LuaValue retValue = null;

        String AssetsPathPrefix = "/android_asset";
        if (!filePath.startsWith("/") || filePath.startsWith(AssetsPathPrefix)) {
            if (filePath.startsWith(AssetsPathPrefix)) {
                filePath = filePath.substring(AssetsPathPrefix.length() + 1);
            }

            //转换路径为Lua文件目录路径
            filePath = String.format("%s/lua/%s", getCacheDir(), filePath);
        }

        File f = new File(filePath);
        retValue = LuaNativeUtil.evalScriptFromFile(_nativeId, filePath, scriptController);

        if (retValue == null) {
            retValue = new LuaValue();
        }

        return retValue;
    }

    public LuaValue callMethod(String methodName, LuaValue[] arguments) {
        return callMethod(methodName, arguments, null);
    }


    public LuaValue callMethod(String methodName, LuaValue[] arguments, LuaScriptController scriptController) {
        return LuaNativeUtil.callMethod(_nativeId, methodName, arguments, scriptController);
    }

    public void registerMethod(String methodName, LuaMethodHandler handler) {
        if (!_methods.containsKey(methodName)) {
            _methods.put(methodName, handler);
            LuaNativeUtil.registerMethod(_nativeId, methodName);
        } else {
            throw new Error("Method for the name already exists");
        }
    }


    public void runThread(LuaFunction handler, LuaValue[] arguments) {
        runThread(handler, arguments, null);
    }

    public void runThread(LuaFunction handler, LuaValue[] arguments, LuaScriptController scriptController) {
        LuaNativeUtil.runThread(this, handler, arguments, scriptController);
    }


    private LuaValue methodInvoke(String methodName, LuaValue[] arguments) {

        LuaValue retValue = null;
        if (_methods.containsKey(methodName)) {
            retValue = _methods.get(methodName).onExecute(arguments);
        } else {
            retValue = new LuaValue();
        }

        return retValue;

    }


    private void exportsNativeType(String typeName) {
        //查找类型
        if (typeName.startsWith("_")) {
            //无效类型
            return;
        }

        try {
            String targetName = typeName.replace("_", ".");
            Class type = Class.forName(targetName);
            exportsNativeType(type);
        } catch (ClassNotFoundException e1) {
            List<PackageInfo> packageInfos = _context.getPackageManager().getInstalledPackages(0);
            for (PackageInfo pi : packageInfos) {
                try {
                    Class type = Class.forName(String.format("%s.%s", pi.packageName, typeName));
                    exportsNativeType(type);
                    break;
                } catch (ClassNotFoundException e2) {

                }
            }
        }

    }


    private void exportsNativeType(Class type) {
        if (!type.isInterface() && LuaExportType.class.isAssignableFrom(type) && !_regTypes.contains(type)) {
            Class baseType = type.getSuperclass();
            exportsNativeType(baseType);

            Log.d("luascriptcore", String.format("Register type %s", type.getName()));

            _regTypes.add(type);
            LuaExportTypeManager.getDefaultManager().exportType(this, type, baseType);
        }
    }


    private void readAssetFileContent(String fileName, ByteArrayOutputStream outputStream) {
        InputStream stream = null;
        try {
            stream = _context.getAssets().open(fileName);
            byte[] buffer = new byte[1024];

            int hasRead = 0;
            while ((hasRead = stream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, hasRead);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }


    private void writeToFile(File file, ByteArrayOutputStream dataStream) {
        try {
            FileOutputStream stream = new FileOutputStream(file);
            dataStream.writeTo(stream);
            stream.flush();
            stream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
