/*
 * 
 */
package com.huawei.svn.sdk.thirdpart;

import java.lang.reflect.AccessibleObject;
import java.security.Policy;
import java.security.PrivilegedAction;
import java.security.Security;

/**
 * Helper class to avoid multiple anonymous inner class for <code>.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class PriviAction<T> implements PrivilegedAction<T>
{

    /** The arg1. */
    private Object arg1;

    /** The arg2. */
    private Object arg2;

    /** The action. */
    private int action;

    /** The Constant GET_SYSTEM_PROPERTY. */
    private static final int GET_SYSTEM_PROPERTY = 1;

    /** The Constant GET_SECURITY_POLICY. */
    private static final int GET_SECURITY_POLICY = 2;

    /** The Constant SET_ACCESSIBLE. */
    private static final int SET_ACCESSIBLE = 3;

    /** The Constant GET_SECURITY_PROPERTY. */
    private static final int GET_SECURITY_PROPERTY = 4;

    /**
     * * Creates a PrivilegedAction to get the security property with the given
     * name.
     * 
     * @param property
     *            the name of the property
     * @return the security property
     * @see Security#getProperty
     */
    public static PrivilegedAction<String> getSecurityProperty(String property)
    {
        return new PriviAction<String>(GET_SECURITY_PROPERTY, property);
    }

    /**
     * Instantiates a new privi action.
     * 
     * @param action
     *            the action
     * @param arg
     *            the arg
     */
    private PriviAction(int action, Object arg)
    {
        this.action = action;
        this.arg1 = arg;
    }

    /***
     * Creates a PrivilegedAction to get the current security policy object.
     * 
     * @see Policy#getPolicy
     */
    public PriviAction()
    {
        action = GET_SECURITY_POLICY;
    }

    /***
     * Creates a PrivilegedAction to disable the access checks to the given
     * object.
     * 
     * @param object
     *            the object whose accessible flag will be set to
     *            <code>true</code>
     * 
     * @see AccessibleObject#setAccessible(boolean)
     */
    public PriviAction(AccessibleObject object)
    {
        action = SET_ACCESSIBLE;
        arg1 = object;
    }

    /***
     * Creates a PrivilegedAction to return the value of the system property
     * with the given key.
     * 
     * @param property
     *            the key of the system property
     * 
     * @see System#getProperty(String)
     */
    public PriviAction(String property)
    {
        action = GET_SYSTEM_PROPERTY;
        arg1 = property;
    }

    /***
     * Creates a PrivilegedAction to return the value of the system property
     * with the given key.
     * 
     * @param property
     *            the key of the system property
     * @param defaultAnswer
     *            the return value if the system property does not exist
     * 
     * @see System#getProperty(String, String)
     */
    public PriviAction(String property, String defaultAnswer)
    {
        action = GET_SYSTEM_PROPERTY;
        arg1 = property;
        arg2 = defaultAnswer;
    }

    /**
     * * Performs the actual privileged computation as defined by the
     * constructor.
     * 
     * @return the t
     * @see java.security.PrivilegedAction#run()
     */
    @SuppressWarnings("unchecked")
    public T run()
    {
        switch (action)
        {
        case GET_SYSTEM_PROPERTY:
            return (T) System.getProperty((String) arg1, (String) arg2);
        case GET_SECURITY_PROPERTY:
            return (T) Security.getProperty((String) arg1);
        case GET_SECURITY_POLICY:
            return (T) Policy.getPolicy();
        case SET_ACCESSIBLE:
            ((AccessibleObject) arg1).setAccessible(true);
            break;
        default:
            break;
        }
        return null;
    }
}
