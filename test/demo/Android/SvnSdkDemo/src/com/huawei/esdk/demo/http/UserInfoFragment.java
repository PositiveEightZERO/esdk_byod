/**
 * 
 */
package com.huawei.esdk.demo.http;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
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

/**
 * @author cWX223941
 *
 */
public class UserInfoFragment extends Fragment
{
    private static final String TAG = "UserInfoFragment";
    private View v;
    private MyEditText etUserName, etLastTime, etUrl, etSubUrl;
    private TextView btnUserInfo;
    //    private UserInfoEntity                      userInfo;
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
        v = inflater.inflate(R.layout.fragment_http_userinfo, container, false);
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
        etUserName = (MyEditText) v
                .findViewById(R.id.et_http_userinfo_username);
        etLastTime = (MyEditText) v
                .findViewById(R.id.et_http_userinfo_lasttime);
        etUrl = (MyEditText) v.findViewById(R.id.et_http_userinfo_url);
        etSubUrl = (MyEditText) v.findViewById(R.id.et_http_userinfo_suburl);
        btnUserInfo = (TextView) v.findViewById(R.id.btn_http_userinfo_request);
        resultBlock = (LinearLayout) v
                .findViewById(R.id.layout_http_userinfo_result_block);
        tvRequest = (TextView) v.findViewById(R.id.tv_http_userinfo_request);
        tvResponse = (TextView) v.findViewById(R.id.tv_http_userinfo_response);
        
        etUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etSubUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etUserName.setItemBlockBackground(R.drawable.http_edit_bg);
        etLastTime.setItemBlockBackground(R.drawable.http_edit_bg);
        etUrl.setHintText(R.string.http_http);
        etSubUrl.setHintText(R.string.http_subhttp);
        etUserName.setHintText(R.string.http_userinfo_username);
        etLastTime.setHintText(R.string.http_userinfo_lastlogintime);
        //        UserInfoController.setUserInfoData(this);
        btnUserInfo.setEnabled(false);
        //no user name and last time
        etUserName.setVisibility(View.GONE);
        etLastTime.setVisibility(View.GONE);

        
        etUrl.setTextChangedListener(textWatcher);
        btnUserInfo.setOnClickListener(onClickListener);
        

    }

    private void initData()
    {
        etUrl.setText(Constants.HTTP_TEST_BASE_URL);
        etSubUrl.setText(Constants.HTTP_TEST_USERINFO_URL);
        etUserName.setText(Constants.LOGIN_USERNAME);
    }
   
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnUserInfo.getId())
            {
                btnUserInfo.setText(R.string.ing);
                btnUserInfo.setEnabled(false);
                tvRequest.setText("");
                tvResponse.setText("");
                
                if (Constants.ACTIVITY_SEND_HTTPTYPE_URLCONNECTION
                        .equals(((HttpActivity) getActivity()).getHttpType()))
                {
                    urlConnectionGetUserInfo();
                }
                else if (Constants.ACTIVITY_SEND_HTTPTYPE_HTTPCLIENT
                        .equals(((HttpActivity) getActivity()).getHttpType()))
                {
                    httpClientGetUserInfo();
                }
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
                ThemeUtil.setBtnToEnable(btnUserInfo, getActivity());
            }
            else
            {
                //set login button to be not enable
                ThemeUtil.setBtnToUnable(btnUserInfo, getActivity());
            }
        }
    };

    /**
     * if all edit text have content,then return true,otherwise return false
     * @return true:all have content;false:otherwise
     * */
    private boolean checkEditContent()
    {
        if (etUrl.hasContent())
        {
            return true;
        }
        return false;
    }

    private void handleUserInfoResult(boolean status, String request,
            String response)
    {
        if (status)
        {
            Log.e(TAG, "get user info success.");
            //            userInfo = new UserInfoEntity("zero","2012-03-04");
            //            setUserInfoData(userInfoFragment);
            btnUserInfo.setText(R.string.userinfo);
            btnUserInfo.setEnabled(true);
            resultBlock.setVisibility(View.VISIBLE);
            tvRequest.setText(request);
            tvResponse.setText(response);
        }
        else
        {
            Log.e(TAG, "get user info false.");
            btnUserInfo.setText(R.string.userinfo);
            btnUserInfo.setEnabled(true);
        }
    }

    private void urlConnectionGetUserInfo()
    {
        AsyncTask<Object, Integer, Boolean> userInfoTask = new AsyncTask<Object, Integer, Boolean>()
        {
            @Override
            protected Boolean doInBackground(Object... paramVarArgs)
            {
                boolean result = true;
                try
                {
                    URLConnectionFactoryHelper.setURLStreamHandlerFactory();
                    //String firstCookie = "";
                    StringBuilder sb = new StringBuilder();
                    URL url = new URL(etUrl.getText().trim()
                            + etSubUrl.getText().trim());
                    Log.d(TAG, "url = " + url.toString());
                    HttpURLConnection connection = (HttpURLConnection) url
                            .openConnection();
                    //connection.setRequestProperty("Cookie", firstCookie);
                    httpRequest = connection.getHeaderFields().toString();
                    connection.connect();
                    httpResponse = getResponseFronUrlConnection(connection);
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                    result = false;
                }
                
                return result;
            }

            @Override
            protected void onPostExecute(Boolean result)
            {
                handleUserInfoResult(result, httpRequest, httpResponse);
            }
        };
        userInfoTask.execute(new Object());
    }

    private void httpClientGetUserInfo()
    {
        AsyncTask<Object, Integer, Boolean> userInfoTask = new AsyncTask<Object, Integer, Boolean>()
        {
            @Override
            protected Boolean doInBackground(Object... paramVarArgs)
            {
                boolean result = true;
                try
                {
                    HttpGet getMethod = new HttpGet(etUrl.getText().trim()
                            + etSubUrl.getText().trim());
                    HttpClient hc = HttpClientHelp.getInstance();
                    HttpResponse response = hc.execute(getMethod);
                    httpRequest = getMethod.getRequestLine().toString();
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
                handleUserInfoResult(result, httpRequest, httpResponse);
            }
        };
        userInfoTask.execute(new Object());
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
