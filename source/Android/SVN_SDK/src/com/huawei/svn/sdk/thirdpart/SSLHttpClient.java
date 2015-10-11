package com.huawei.svn.sdk.thirdpart;

import java.security.KeyStore;

import org.apache.http.HttpVersion;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.ClientConnectionOperator;
import org.apache.http.conn.routing.HttpRoutePlanner;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.conn.DefaultHttpRoutePlanner;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.protocol.HTTP;

import com.huawei.svn.sdk.thirdpart.ssl.SvnHttpsSocketFactory;

public class SSLHttpClient extends DefaultHttpClient
{
    @Override
    protected HttpRoutePlanner createHttpRoutePlanner()
    {
        // return super.createHttpRoutePlanner();
        return new DefaultHttpRoutePlanner(getConnectionManager()
                .getSchemeRegistry());

    }

    @Override
    protected ClientConnectionManager createClientConnectionManager()
    {

        KeyStore trustStore;
        try
        {
            trustStore = KeyStore.getInstance(KeyStore.getDefaultType());

            trustStore.load(null, null);

            SSLSocketFactory sf = new SSLSocketFactoryEx(trustStore);
            sf.setHostnameVerifier(SSLSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER);

            // SchemeRegistry registry = new SchemeRegistry();
            // registry.register(new Scheme("http",
            // PlainSocketFactory.getSocketFactory(), 80));
            // registry.register(new Scheme("https", sf, 443));
            //
            // ClientConnectionManager ccm = new
            // ThreadSafeClientConnManager(params, registry);
            //
            //

            SchemeRegistry schemeRegistry = new SchemeRegistry();
            schemeRegistry.register(new Scheme("http",
                    new SvnHttpSocketFactory(), 80));

             schemeRegistry.register(new Scheme("https", SvnHttpsSocketFactory
             .getSocketFactory(), 443));

//            schemeRegistry.register(new Scheme("https", sf, 443));

            HttpParams params = getParams();

            HttpProtocolParams.setVersion(params, HttpVersion.HTTP_1_1);
            HttpProtocolParams.setContentCharset(params, HTTP.UTF_8);
            HttpConnectionParams.setTcpNoDelay(params, true);

            ClientConnectionManager clientConnectionManager = new ThreadSafeClientConnManager(
                    params, schemeRegistry)
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
        catch (Exception e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        }
    }

    public SSLHttpClient()
    {
        super();
    }

    public SSLHttpClient(ClientConnectionManager conman, HttpParams params)
    {
        super(null, params);
    }

    public SSLHttpClient(HttpParams params)
    {
        super(params);
    }

}
