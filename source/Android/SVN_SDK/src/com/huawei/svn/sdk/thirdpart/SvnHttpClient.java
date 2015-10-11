package com.huawei.svn.sdk.thirdpart;

import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.ClientConnectionOperator;
import org.apache.http.conn.routing.HttpRoutePlanner;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.conn.DefaultHttpRoutePlanner;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.HttpParams;

import com.huawei.shield.ProxyConstruct;
import com.huawei.shield.WedgeClass;

@WedgeClass(value="Lorg/apache/http/impl/client/DefaultHttpClient;")
@ProxyConstruct(value="Lorg/apache/http/impl/client/DefaultHttpClient;")
public class SvnHttpClient extends DefaultHttpClient
{
    @Override
    protected HttpRoutePlanner createHttpRoutePlanner()
    {
        //return super.createHttpRoutePlanner();
        return new DefaultHttpRoutePlanner(getConnectionManager().getSchemeRegistry());
        
    }

    
    @Override
    protected ClientConnectionManager createClientConnectionManager()
    {
       
        SchemeRegistry schemeRegistry = new SchemeRegistry();
        schemeRegistry.register(new Scheme("http",
                new SvnHttpSocketFactory(), 80));
        HttpParams params = getParams();
     
        ClientConnectionManager clientConnectionManager = new ThreadSafeClientConnManager(params, schemeRegistry)
        {
            /* overide this for dns parse */
            @Override
            protected ClientConnectionOperator createConnectionOperator(
                    SchemeRegistry schreg)
            {
                return new SvnClientConnectionOperator(schreg);
            }
        };
        return clientConnectionManager;
    }

    public SvnHttpClient()
    {
        super();
    }

    public SvnHttpClient(ClientConnectionManager conman, HttpParams params)
    {
        super(null, params);
    }

    public SvnHttpClient(HttpParams params)
    {
        super(params);
    }

    
}
