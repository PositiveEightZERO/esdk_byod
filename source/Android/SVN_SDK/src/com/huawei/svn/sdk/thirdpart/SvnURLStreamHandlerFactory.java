/*
 * 
 */
package com.huawei.svn.sdk.thirdpart;

import java.net.URLStreamHandler;
import java.net.URLStreamHandlerFactory;


/**
 * SvnURLStreamHandlerFactory 
 * 用来创建SvnURLStreamHandler.
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class SvnURLStreamHandlerFactory implements URLStreamHandlerFactory
{

    /* (non-Javadoc)
     * @see java.net.URLStreamHandlerFactory#createURLStreamHandler(java.lang.String)
     */
    @Override
    public URLStreamHandler createURLStreamHandler(String protocol)
    {
        if ("http".equals(protocol))
        {
            return new SvnURLStreamHandler();
        }

        return null;
    }

}
