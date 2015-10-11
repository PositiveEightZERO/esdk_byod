/*
       Licensed to the Apache Software Foundation (ASF) under one
       or more contributor license agreements.  See the NOTICE file
       distributed with this work for additional information
       regarding copyright ownership.  The ASF licenses this file
       to you under the Apache License, Version 2.0 (the
       "License"); you may not use this file except in compliance
       with the License.  You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing,
       software distributed under the License is distributed on an
       "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
       KIND, either express or implied.  See the License for the
       specific language governing permissions and limitations
       under the License.
 */
package io.cordova.hellocordova;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

import org.apache.cordova.CordovaActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Process;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;

import com.huawei.svn.sdk.mdm.DeviceIdInfo;
import com.huawei.svn.sdk.server.SvnApiService;
import com.huawei.svn.sdk.server.SvnCallBack;
import com.huawei.svn.sdk.webview.SvnWebViewProxy;

public class CordovaApp extends CordovaActivity
{
    public static final String VERSION = "Version";
    public static final String ENCCONTEXT = "EncContext";
    public static final String SVNSERVER = "SvnServer";
    public static final String SVNSERVERBACKUP = "SvnServerBackup";
    public static final String URLENCODED = "UrlEncoded";
    public static final String AUTH = "auth";
    public static final String SOURCE = "source";
    public static final String USERNAME = "user_name";
    public static final String PASSWORD = "password";
    public static final String SVNPARAMS = "SvnParams";
    public static final String RETURNCODE = "ReturnCode";
    public static final String PACKAGENAME = "PackageName";
    public static final String HOMEACTIVITY = "HomeActivity";
    public static final String TERMINALID = "TerminalID";
    private static final String TAG = "SSOIntent";
    private String username;
    private String password;
    private String svnServer;
    // 处理回调函数
    private SvnCallBack iCallBack = new SvnCallBack()
    {
        @Override
        public void statusNotify(int arg0, int arg1)
        {
            Log.d("CordovaApp", "VPNStatus = " + SvnApiService.getVPNStatus()
                    + ", iStatus = " + arg0 + ",iError=" + arg1);
        }

        @Override
        public void writeLog(String strLog, int iLevel)
        {
            Log.d("CordovaApp", "" + strLog);
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        super.init();
        appView.setOnKeyListener(new OnKeyListener()
        {
            public boolean onKey(View v, int keyCode, KeyEvent event)
            {
                if (keyCode == KeyEvent.KEYCODE_BACK
                        && event.getAction() == KeyEvent.ACTION_UP)
                {
                    // Do Here whatever you want 
                    CordovaApp.this.finish();
                    //Process.killProcess(Process.myPid());
                    return true;
                }
                return onKeyDown(keyCode, event);
            }
        });
        // Set by <content src="index.html" /> in config.xml
        Intent intent = this.getIntent();
        if (intent == null)
        {
            return;
        }
        String svnServer = null;
        String urlEncoded = null;
        String username = null;
        String password = null;
        Bundle bundle = intent.getExtras();
        if (bundle == null)
        {
            Log.e(TAG, "no sso info of intent!");
            svnServer = "10.170.103.70";
            username = "lzy";
            password = "Admin@123";
        }
        else
        {
            svnServer = bundle.getString(SVNSERVER);
            urlEncoded = bundle.getString(URLENCODED);
            username = bundle.getString(USERNAME);
            password = bundle.getString(PASSWORD);
            Log.i(TAG, "svnServer:" + svnServer + ",username:" + username
                    + ",password:" + password);
            if (username == null || password == null || svnServer == null)
            {
                Log.e(TAG, "some sso info of intent is null !");
            }
            if ("true".equals(urlEncoded))
            {
                try
                {
                    username = URLDecoder.decode(username, "UTF-8");
                    password = URLDecoder.decode(password, "UTF-8");
                }
                catch (UnsupportedEncodingException e)
                {
                    Log.e(TAG, "UnsupportedEncodingException !");
                }
            }
        }
        String[] svnServerIp = svnServer.split(":");
        this.svnServer = svnServerIp[0];
        this.username = username;
        this.password = password;
        //        String gateway = "10.170.103.70";
        //        String username = "lzy";
        //        String password = "Admin@123";
        // 文件加密路径等参数
        String deviceId = new DeviceIdInfo(getBaseContext()).getDeviceId();
        // 设置不校验证书
        SvnApiService.undoCAChecking();
        // 设置回调函数
        SvnApiService.setCallBack(iCallBack);
        // 登录SVN
        int ret = SvnApiService.login(this.username, this.password,
                this.svnServer, "/data/data/" + getPackageName(),
                getPackageName(), deviceId);//
        SvnWebViewProxy.getInstance().setWebViewUseSVN(this.appView);
        String url = "http://172.16.1.4:8080/";
        loadUrl(url);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        System.out.println("-----------------onkeydown");
        if (KeyEvent.KEYCODE_BACK == keyCode)
        {
            this.finish();
            //Process.killProcess(Process.myPid());
        }
        return super.onKeyDown(keyCode, event);
    }
}
