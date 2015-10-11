/**
 * 
 */
package com.huawei.esdk.demo.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.utils.FileUtil;
import com.huawei.esdk.demo.utils.ThemeUtil;
import com.huawei.esdk.demo.widget.MyEditText;
import com.huawei.svn.sdk.socket.SvnSocket;

/**
 * @author cWX223941
 *
 */
public class HttpLoginFragment extends Fragment
{
    private static final String TAG = "HttpLoginFragment";
    private View v;
    private MyEditText etUrl, etSubUrl, etUserName, etPassword;
    private TextView btnLogin;
    private LinearLayout resultBlock;
    private TextView tvRequest, tvResponse;
    private String httpRequest;
    private String httpResponse;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState)
    {
        v = inflater.inflate(R.layout.fragment_http_login, container, false);
        init();
        return v;
    }

    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
        etUrl = (MyEditText) v.findViewById(R.id.et_http_login_url);
        etSubUrl = (MyEditText) v.findViewById(R.id.et_http_login_suburl);
        etUserName = (MyEditText) v.findViewById(R.id.et_http_login_username);
        etPassword = (MyEditText) v.findViewById(R.id.et_http_login_password);
        btnLogin = (TextView) v.findViewById(R.id.btn_http_login_login);
        resultBlock = (LinearLayout) v
                .findViewById(R.id.layout_http_login_result_block);
        tvRequest = (TextView) v.findViewById(R.id.tv_http_login_request);
        tvResponse = (TextView) v.findViewById(R.id.tv_http_login_response);
        
        
        etUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etSubUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etUserName.setItemBlockBackground(R.drawable.http_edit_bg);
        etPassword.setItemBlockBackground(R.drawable.http_edit_bg);
        etUrl.setHintText(R.string.http_http);
        etSubUrl.setHintText(R.string.http_subhttp);
        etUserName.setHintText(R.string.http_login_name);
        etPassword.setHintText(R.string.http_login_password);
        btnLogin.setEnabled(false);
        
        etUrl.setTextChangedListener(textWatcher);
        etUserName.setTextChangedListener(textWatcher);
        etPassword.setTextChangedListener(textWatcher);
        btnLogin.setOnClickListener(loginListener);
    }


    private void initData()
    {
        etUrl.setText(Constants.HTTP_TEST_BASE_URL);
        etSubUrl.setText(Constants.HTTP_TEST_LOGIN_URL);
        etUserName.setText("test");
        etPassword.setText("password");
    }
    private OnClickListener loginListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            btnLogin.setText(R.string.ing);
            btnLogin.setEnabled(false);
            tvRequest.setText("");
            tvResponse.setText("");
            
            if (Constants.ACTIVITY_SEND_HTTPTYPE_URLCONNECTION
                    .equals(((HttpActivity) getActivity()).getHttpType()))
            {
                doLoginThroughURLConnection();
            }
            else if (Constants.ACTIVITY_SEND_HTTPTYPE_HTTPCLIENT
                    .equals(((HttpActivity) getActivity()).getHttpType()))
            {
                doLoginThroughHttpClient();
            }
        }
    };
    private TextWatcher textWatcher = new TextWatcher()
    {
        @Override
        public void onTextChanged(CharSequence s, int start, int before,
                int count)
        {
        }

        @Override
        public void beforeTextChanged(CharSequence s, int start, int count,
                int after)
        {
        }

        @Override
        public void afterTextChanged(Editable s)
        {
            if (checkEditContent())
            {
                //set login button to be enable
                ThemeUtil.setBtnToEnable(btnLogin, getActivity());
            }
            else
            {
                //set login button to be not enable
                ThemeUtil.setBtnToUnable(btnLogin, getActivity());
            }
        }
    };

    /**
     * if all edit text have content,then return true,otherwise return false
     * @return true:all have content;false:otherwise
     * */
    private boolean checkEditContent()
    {
        if (etUrl.hasContent() && etUserName.hasContent()
                && etPassword.hasContent())
        {
            return true;
        }
        return false;
    }

    private void handleLoginResult(Boolean status, String request,
            String response)
    {
        if (status)
        {
            btnLogin.setText(R.string.login);
            btnLogin.setEnabled(true);
            resultBlock.setVisibility(View.VISIBLE);
            tvRequest.setText(request);
            tvResponse.setText(response);
        }
        else
        {
            btnLogin.setText(R.string.login);
            btnLogin.setEnabled(true);
        }
    }

    private void doLoginThroughURLConnection()
    {
        Log.i(TAG, "in urlConnectionLogin.");
        final String urlinfo = etUrl.getText().trim()
                + etSubUrl.getText().trim();
        String username = etUserName.getText().trim();
        String password = etPassword.getText().trim();
        final List<BasicNameValuePair> params = new LinkedList<BasicNameValuePair>();
        params.add(new BasicNameValuePair("username", username));
        params.add(new BasicNameValuePair("password", password));
        AsyncTask<Object, Integer, Boolean> loginTask = new AsyncTask<Object, Integer, Boolean>()
        {
            @Override
            protected Boolean doInBackground(Object... paramVarArgs)
            {
                boolean result = true;
                
                URLConnectionFactoryHelper.setURLStreamHandlerFactory();
                try
                {
                    //init the request data
                    URL url = new URL(urlinfo);
                    HttpURLConnection connection = (HttpURLConnection) url
                            .openConnection();
                    connection.setDoOutput(true);
                    connection.setDoInput(true);
                    connection.setRequestMethod("POST");
                    connection
                            .setFixedLengthStreamingMode(null != params ? URLEncodedUtils
                                    .format(params, "UTF-8").length() : 0);
                    connection.setUseCaches(false); // Post 请求不能使用缓存，因为要保证post数据安全
                    connection.setConnectTimeout(5000);// （单位：毫秒）jdk
                    connection.setReadTimeout(5000);// （单位：毫秒）jdk 1.5换成这个,读操作超时
                    Log.d(TAG, "url = " + url.toString());
                    connection.connect();
                    String param = URLEncodedUtils.format(params, "UTF-8");
                    connection.getOutputStream().write(param.getBytes(), 0,
                            param.getBytes().length);
                    connection.getOutputStream().flush();
                    connection.getOutputStream().close();
                    
                    //get request and response
                    httpRequest = connection.getHeaderFields().toString();
                    httpResponse = getResponseFronUrlConnection(connection);
                   
                    Log.i(TAG, "urlConnection login success.");
                }
                catch (Exception e)
                {
                    result = false;
                    e.printStackTrace();
                    Log.e(TAG,
                            "urlConnection login false.resCode = "
                                    + e.getMessage());
                }
                return result;
            }

            @Override
            protected void onPostExecute(Boolean result)
            {
                handleLoginResult(result, httpRequest, httpResponse);
            }
        };
        loginTask.execute(new Object());
    }

    private void doLoginThroughHttpClient()
    {
        Log.i(TAG, "in httpClientLogin.");
        
        final String urlinfo = etUrl.getText().trim()
                + etSubUrl.getText().trim();
        
        String username = etUserName.getText().trim();
        String password = etPassword.getText().trim();
        final List<BasicNameValuePair> params = new LinkedList<BasicNameValuePair>();
        params.add(new BasicNameValuePair("username", username));
        params.add(new BasicNameValuePair("password", password));

        
        AsyncTask<Object, Integer, Boolean> loginTask = new AsyncTask<Object, Integer, Boolean>()
        {
            @Override
            protected Boolean doInBackground(Object... paramVarArgs)
            {
                boolean result = true;
                try
                {
                    //新建SvnHttpClient对象
                    //HttpClient hc = new SvnHttpClient();
                    HttpClient hc = HttpClientHelp.getInstance();//for share cookie, use a singleton
                    HttpPost postMethod = new HttpPost(urlinfo);
                    UrlEncodedFormEntity param = new UrlEncodedFormEntity(
                            params, "utf-8");
                    postMethod.setEntity(param);
                    
                    //发送请求
                    HttpResponse response = hc.execute(postMethod);
                    httpRequest = postMethod.getRequestLine().toString();
                    Log.i(TAG, "resCode = "
                            + response.getStatusLine().getStatusCode());
                    httpResponse = EntityUtils.toString(response.getEntity(),
                            "utf-8");
                    //get response and request
                    Log.i(TAG, "httpClient request success.");
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                    Log.e(TAG,
                            "httpClient request fail. exception = "
                                    + e.getMessage());
                    result = false;
                }
                return result;
            }

            @Override
            protected void onPostExecute(Boolean result)
            {
                handleLoginResult(result, httpRequest, httpResponse);
            }
        };
        loginTask.execute(new Object());
    }

    /**
     * get response from connection
     * @param conn HttpURLConnection
     * */
    private String getResponseFronUrlConnection(HttpURLConnection conn)
            throws Exception
    {
        StringBuffer builder = new StringBuffer();
        InputStream stream = conn.getInputStream();
        builder.append(FileUtil.read(stream));
        return builder.toString();
    }
}
