package com.huawei.svn.sdk.thirdpart;

import java.net.URL;
import java.net.URLStreamHandlerFactory;

public class URLConnectionFactoryHelper
{
    private static URLStreamHandlerFactory factory;

    public synchronized static void setURLStreamHandlerFactory()
    {
        if (factory == null)
        {
            factory = new SvnURLStreamHandlerFactory();
            URL.setURLStreamHandlerFactory(factory);
        }
    }
}
