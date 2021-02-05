package com.hacknife.lua;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Deprecated
public @interface LuaExportTypeConfig {

    public String[] excludeExportClassMethodNames() default {};

    public String[] excludeExportInstanceMethodsNames() default {};

    public String[] excludeExportFieldNames() default {};
}
