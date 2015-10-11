/**
 * 
 */
package com.huawei.esdk.demo.http;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.View.OnClickListener;
import android.webkit.DownloadListener;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;
import android.widget.Toast;

import com.huawei.anyoffice.sdk.ui.ISDKWebViewSSO;
import com.huawei.anyoffice.sdk.ui.SDKWebview;
import com.huawei.anyoffice.sdk.web.WebApp;
import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.utils.ThemeUtil;
import com.huawei.esdk.demo.widget.MyEditText;
import com.huawei.svn.sdk.webview.SvnWebViewProxy;

/**
 * @author cWX223941
 *
 */
public class WebViewActivity extends BaseActivity implements ISDKWebViewSSO
{
    //    private static final String                     TAG = "WebViewActivity";
    private MyEditText etUrl;
    private TextView btnWebView, btnBack;
    private WebView webView;
    private SDKWebview sdkWebview;
    
    private TextView btnSdkWebView;
    private TextView btnWebApp;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_webview);
        init();
    }

    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
        etUrl = (MyEditText) findViewById(R.id.et_webview_url);
        btnWebView = (TextView) findViewById(R.id.btn_webview);
        btnBack = (TextView) findViewById(R.id.btn_webview_back);
        webView = (WebView) findViewById(R.id.wv_webview);
        sdkWebview = (SDKWebview) findViewById(R.id.wv_sdkwebview);
        
        btnSdkWebView = (TextView) findViewById(R.id.btn_sdkwebview);
        btnWebApp = (TextView) findViewById(R.id.btn_webapp);
        
        btnWebView.setOnClickListener(onClickListener);
        btnSdkWebView.setOnClickListener(onClickListener);
        btnWebApp.setOnClickListener(onClickListener);
        
        btnBack.setOnClickListener(onClickListener);
        etUrl.setTextChangedListener(textWatcher);
        webView.setDownloadListener(new DownLoadAction());
        
        etUrl.setItemBlockBackground(R.drawable.http_edit_bg);
        etUrl.setHintText(R.string.http_http);
        btnWebView.setEnabled(false);
        btnSdkWebView.setEnabled(false);
        btnWebApp.setEnabled(false);
        
        SvnWebViewProxy.getInstance().setExceptionAddressList(true, "*.huawei.com;10.170.*");
        
        //设置webview走安全隧道
        boolean res = SvnWebViewProxy.getInstance().setWebViewUseSVN(webView);
        System.out.println("setWebViewUseSVN:" + res);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.getSettings().setDomStorageEnabled(true);
        webView.getSettings()
                .setUserAgentString(
                        "Mozilla/5.0 (Linux; U; Android 2.2; en-gb; Nexus One Build/FRF50) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1");
        webView.setWebViewClient(new WebViewClient()
        {
            @SuppressWarnings("unused")
            final static int BUFFER_SIZE = 4096;

            @Override
            public void onPageStarted(WebView view, String url, Bitmap favicon)
            {
                super.onPageStarted(view, url, favicon);
            }

            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url)
            {
                System.out.println("shouldOverrideUrlLoading:" + url);
                view.loadUrl(url);
                return true;
            }

            @Override
            public void onPageFinished(WebView view, String url)
            {
                super.onPageFinished(view, url);
            }
        });
        
        SDKWebview.setSSOCallback(WebViewActivity.this);
        
        
    }
    

    @SuppressLint("SetJavaScriptEnabled")
    private void initData()
    {
        etUrl.setText(Constants.HTTP_TEST_WEBVIEW_URL);
    }


    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnWebView.getId())
            {
                //                WebViewController.doWebViewPrepareAction(WebViewActivity.this);
                webView.setVisibility(View.VISIBLE);
                sdkWebview.setVisibility(View.GONE);
                //load web view url
                String url = etUrl.getText().trim();
                webView.loadUrl(url);//must be called in UI thread
                
            }
            else if (v.getId() == btnBack.getId())
            {
                WebViewActivity.this.finish();
            }
            else if(v.getId() == btnSdkWebView.getId())
            {
                //                WebViewController.doWebViewPrepareAction(WebViewActivity.this);
                webView.setVisibility(View.GONE);
                sdkWebview.setVisibility(View.VISIBLE);
                //load web view url
                String url = etUrl.getText().trim();

                sdkWebview.loadUrl(url);//must be called in UI thread
                
            }
            else if(v.getId() == btnWebApp.getId())
            {
                String url = etUrl.getText().trim();
                
                //use address input view
                //WebApp.setUseInputView(true);
                WebApp.openUrl(WebViewActivity.this, url, true, true);
                
            }
        }
    };
    class DownLoadAction implements DownloadListener
    {
        @Override
        public void onDownloadStart(String url, String userAgent,
                String contentDisposition, String mimetype, long contentLength)
        {
            System.out.println("DownloadListener:" + url + ", mimeType:"
                    + mimetype + ",contentLength:" + contentLength);
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
                ThemeUtil.setBtnToEnable(btnWebView, WebViewActivity.this);
                ThemeUtil.setBtnToEnable(btnSdkWebView, WebViewActivity.this);
                ThemeUtil.setBtnToEnable(btnWebApp, WebViewActivity.this);
                
            }
            else
            {
                //set login button to be not enable
                ThemeUtil.setBtnToUnable(btnWebView, WebViewActivity.this);
                ThemeUtil.setBtnToUnable(btnSdkWebView, WebViewActivity.this);
                ThemeUtil.setBtnToUnable(btnWebApp, WebViewActivity.this);
            }
        }
    };

    @Override
    protected void onResume()
    {
        super.onResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    private boolean checkEditContent()
    {
        if (etUrl.hasContent())
        {
            return true;
        }
        return false;
    }

    @Override
    public void ssoCallback(final String url)
    {
    	Handler handler = new Handler(Looper.getMainLooper());
		handler.post(
			new Runnable()
			{
		        @Override
		        public void run()
		        {
		        	Toast.makeText(WebViewActivity.this, "callback:" + url, Toast.LENGTH_SHORT).show();
		        }
		    }
		);
    }
}
