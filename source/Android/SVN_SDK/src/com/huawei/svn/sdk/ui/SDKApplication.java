package com.huawei.svn.sdk.ui;

import java.io.File;

import android.app.Application;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.AsyncTask;

import com.huawei.anyoffice.sdk.SDKContext;
import com.huawei.anyoffice.sdk.login.LoginAgent;
import com.huawei.anyoffice.sdk.login.LoginParam;
import com.huawei.anyoffice.sdk.login.LoginParam.AutoLoginType;
import com.huawei.shield.WedgeClass;
import com.huawei.shield.WrappingConfig;
import com.huawei.svn.sdk.thirdpart.URLConnectionFactoryHelper;

@WedgeClass(value="Landroid/app/Application;")
public class SDKApplication extends Application
{

    private static boolean isInited = false;
    
    private static boolean isLoginSucc = false;

    @Override
    public void onCreate()
    {
        super.onCreate();
        init();
    }

    private synchronized void init()
    {
        /* load so library and initialize SDK environment */
        if (!isInited)
        {
            isInited = true;
            
            try
            {
                System.loadLibrary("svnapi");
                System.loadLibrary("anyofficesdk");
                System.loadLibrary("jniapi");
                
            }
            catch(Exception e)
            {
                
            }
            
            String workPath = "/data/data/" + getPackageName();
            File f = new File(workPath);
            if (!f.exists())
            {
                f.mkdir();
            }

            SDKContext.getInstance().init(SDKApplication.this, workPath);
            
            doWrapping();
            
        }
    }
    
    
    private void doWrapping()
    {
        WrappingConfig cnf = WrappingConfig.getWrappingConfig();
        
        if (cnf.isWebviewWrap() || cnf.isHttpOverL4Wrap() || cnf.isSocketOverL4Wrap())
        {
            if (!isLoginSucc)
            {
                doLogin();
            }
            
            if (cnf.isHttpOverL4Wrap())
            {
                URLConnectionFactoryHelper.setURLStreamHandlerFactory();
            }
        }
        else if (cnf.isFileSandboxWrap())
        {
            doAppAuth();

        }
    }

    private void doLogin()
    {

        // login task
        AsyncTask<Object, Integer, Integer> loginTask = new AsyncTask<Object, Integer, Integer>()
        {
            @Override
            protected Integer doInBackground(Object... paramVarArgs)
            {
                // NetStatusManager.getInstance().setNetChangeCallback(callback);

                String appName = "ISVApp";
                try
                {
                    PackageInfo pkg = getPackageManager().getPackageInfo(getPackageName(), 0);
                    appName = pkg.applicationInfo.loadLabel(getPackageManager()).toString();

                }
                catch (NameNotFoundException e)
                {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }

                // 初始化登录参数
                LoginParam loginParam = new LoginParam();

                // 设置应用的业务类型，
                loginParam.setServiceType(appName);
                loginParam.setLoginTitle(appName);
                loginParam.setAutoLoginType(AutoLoginType.auto_login_enable);
                loginParam.setLoginBackground(false);

                // 设置用户信息
//                LoginParam.UserInfo userInfo = loginParam.new UserInfo();
//                if (null != inputUsername)
//                {
//                    userInfo.userName = inputUsername;
//                }
//
//                loginParam.setUserInfo(userInfo);
                loginParam.setUseSecureTransfer(true);

                //System.out.println("first time to login with " + userInfo.userName);
                int ret = LoginAgent.getInstance().loginSync(getApplicationContext(), loginParam);

                return ret;
            }
            
            @Override
            protected void onPostExecute(Integer ret)
            {
                if (0 == ret)
                {
                    isLoginSucc = true;
                }
            }

        };

        // start login task
        loginTask.execute(new Object());

    }
    
    private void doAppAuth()
    {
        AsyncTask<Object, Integer, Integer> loginTask = new AsyncTask<Object, Integer, Integer>()
        {
            @Override
            protected Integer doInBackground(Object... paramVarArgs)
            {
                //do gateway authentication
                int res = LoginAgent.getInstance().doAppAuthentication();

                return res;
            }
            
            @Override
            protected void onPostExecute(Integer ret)
            {
            }

        };

        // start login task
        loginTask.execute(new Object());

    
    }

}
