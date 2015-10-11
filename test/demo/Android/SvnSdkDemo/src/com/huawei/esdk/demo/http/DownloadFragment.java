/**
 * 
 */
package com.huawei.esdk.demo.http;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;

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
import com.huawei.esdk.demo.utils.BaseUtil;
import com.huawei.esdk.demo.utils.FileUtil;
import com.huawei.esdk.demo.utils.ThemeUtil;
import com.huawei.esdk.demo.widget.MyEditText;

/**
 * @author cWX223941
 *
 */
public class DownloadFragment extends Fragment
{
    private static final String TAG = "DownloadFragment";
    private View v;
    private MyEditText etUrl, etSubUrl, etFileName;
    private TextView btnDownload;
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
        v = inflater.inflate(R.layout.fragment_http_download, container, false);
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
        etUrl = (MyEditText) v.findViewById(R.id.et_http_download_url);
        etSubUrl = (MyEditText) v.findViewById(R.id.et_http_download_suburl);
        etFileName = (MyEditText) v
                .findViewById(R.id.et_http_download_filename);
        btnDownload = (TextView) v.findViewById(R.id.btn_http_download_request);
        resultBlock = (LinearLayout) v
                .findViewById(R.id.layout_http_download_result_block);
        tvRequest = (TextView) v.findViewById(R.id.tv_http_download_request);
        tvResponse = (TextView) v.findViewById(R.id.tv_http_download_response);
        
        etUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etSubUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etFileName.setItemBlockBackground(R.drawable.http_edit_bg);
        etUrl.setHintText(R.string.http_http);
        etSubUrl.setHintText(R.string.http_subhttp);
        etFileName.setHintText(R.string.http_download_filename);
        btnDownload.setEnabled(false);
        
        etUrl.setTextChangedListener(textWatcher);
        etFileName.setTextChangedListener(textWatcher);
        btnDownload.setOnClickListener(onClickListener);
    }

    private void initData()
    {
        etUrl.setText(Constants.HTTP_TEST_BASE_URL);
        etSubUrl.setText(Constants.HTTP_TEST_DOWNLOAD_URL);
        etFileName.setText("aa.jpg");
    }


    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            btnDownload.setText(R.string.ing);
            btnDownload.setEnabled(false);
            tvRequest.setText("");
            tvResponse.setText("");
            
            if (Constants.ACTIVITY_SEND_HTTPTYPE_URLCONNECTION
                    .equals(((HttpActivity) getActivity()).getHttpType()))
            {
                urlConnectionDownload();
            }
            else if (Constants.ACTIVITY_SEND_HTTPTYPE_HTTPCLIENT
                    .equals(((HttpActivity) getActivity()).getHttpType()))
            {
                httpClientDownload();
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
                ThemeUtil.setBtnToEnable(btnDownload, getActivity());
            }
            else
            {
                //set login button to be not enable
                ThemeUtil.setBtnToUnable(btnDownload, getActivity());
            }
        }
    };

    /**
     * if all edit text have content,then return true,otherwise return false
     * @return true:all have content;false:otherwise
     * */
    private boolean checkEditContent()
    {
        if (etUrl.hasContent() && etFileName.hasContent())
        {
            return true;
        }
        return false;
    }

    private void handleDownloadResult(boolean status, String request,
            String response)
    {
        if (status)
        {
            BaseUtil.showToast(R.string.download_ok, getActivity());
            //            downloadFragment.btnDownload.setVisibility(View.VISIBLE);
            btnDownload.setText(R.string.download);
            btnDownload.setEnabled(true);
            resultBlock.setVisibility(View.VISIBLE);
            tvRequest.setText(request);
            tvResponse.setText(response);
        }
        else
        {
            btnDownload.setText(R.string.download);
            btnDownload.setEnabled(true);
            BaseUtil.showToast(R.string.download_error, getActivity());
        }
    }

    private void urlConnectionDownload()
    {
        final String fileName = etFileName.getText().trim();
        String encodedFileName = fileName;
        try
        {
            encodedFileName = URLEncoder.encode( fileName, "utf-8");
        }
        catch (UnsupportedEncodingException e1)
        {
            encodedFileName = fileName;
        }
        
        
        final String urlStr = etUrl.getText().trim()
                + etSubUrl.getText().trim() + "?fileName=" + encodedFileName;
       
        
        AsyncTask<Object, Integer, Boolean> downloadTask = new AsyncTask<Object, Integer, Boolean>()
        {
            @Override
            protected Boolean doInBackground(Object... paramVarArgs)
            {
                boolean result = false;
                try
                {
                    URLConnectionFactoryHelper.setURLStreamHandlerFactory();
                    InputStream stream = null;
                    FileOutputStream fstream = null;
                    //init the request data
                    URL url = new URL(urlStr);
                    HttpURLConnection connection = (HttpURLConnection) url
                            .openConnection();
                    connection.connect();
                    stream = connection.getInputStream();
                    Log.e(TAG, "URLConnection request success, res = "
                            + connection.getResponseCode());
                    if (connection.getResponseCode() == 200)
                    {
                        String localPath = Constants.FOLDER_PATH_DOWNLOAD + "/"
                                + fileName;
                        File folder = new File(Constants.FOLDER_PATH_DOWNLOAD);
                        if (!folder.exists())
                        {
                            folder.mkdirs();
                        }
                        Log.d(TAG, "Down Success,filePath = " + localPath);
                        fstream = new FileOutputStream(new File(localPath));
                        FileUtil.streamCopy(stream, fstream);
                        result = true;
                    }
                    //get response and request
                    httpRequest = urlStr;
                    httpResponse = connection.getHeaderFields().toString();
                }
                catch (Exception e)
                {
                    result = false;
                    e.printStackTrace();
                    Log.e(TAG,
                            "URLConnection request fail, res = "
                                    + e.getMessage());
                }
                return result;
            }

            @Override
            protected void onPostExecute(Boolean result)
            {
                handleDownloadResult(result, httpRequest, httpResponse);
            }
        };
        downloadTask.execute(new Object());
    }

    private void httpClientDownload()
    {
        final String fileName = etFileName.getText().trim();
        
        String encodedFileName = fileName;
        try
        {
            encodedFileName = URLEncoder.encode( fileName, "utf-8");
        }
        catch (UnsupportedEncodingException e1)
        {
            encodedFileName = fileName;
        }
        
        
        final String urlStr = etUrl.getText().trim()
                + etSubUrl.getText().trim() + "?fileName=" + encodedFileName;
        AsyncTask<Object, Integer, Boolean> downloadTask = new AsyncTask<Object, Integer, Boolean>()
        {
            @Override
            protected Boolean doInBackground(Object... paramVarArgs)
            {
                boolean result = false;
                try
                {
                    InputStream fileInStream = null;
                    FileOutputStream fileOutStream = null;
                    //HttpClient hc = new SvnHttpClient();
                    HttpClient hc = HttpClientHelp.getInstance();
                    HttpGet getMethod = new HttpGet(urlStr);
                    HttpResponse response = hc.execute(getMethod);
                    Log.e(TAG, "HttpClient request success, res = "
                            + response.getStatusLine().getStatusCode());
                    if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK)
                    {
                        HttpEntity respEnt = response.getEntity();
                        Boolean isStream = respEnt.isStreaming();
                        if (isStream)
                        {
                            fileInStream = respEnt.getContent();
                            String localPath = Constants.FOLDER_PATH_DOWNLOAD
                                    + "/" + fileName;
                            File folder = new File(
                                    Constants.FOLDER_PATH_DOWNLOAD);
                            if (!folder.exists())
                            {
                                folder.mkdirs();
                            }
                            fileOutStream = new FileOutputStream(new File(
                                    localPath));
                            FileUtil.streamCopy(fileInStream, fileOutStream);
                            //get response and request
                            
                            result = true;
                        }
                    }
                    httpRequest = urlStr;
                    httpResponse = response.getStatusLine().toString();
                }
                catch (Exception e)
                {
                    result = false;
                    e.printStackTrace();
                    Log.e(TAG,
                            "HttpClient request fail, res = " + e.getMessage());
                }
                return result;
            }

            @Override
            protected void onPostExecute(Boolean result)
            {
                handleDownloadResult(result, httpRequest, httpResponse);
            }
        };
        downloadTask.execute(new Object());
    }
}
