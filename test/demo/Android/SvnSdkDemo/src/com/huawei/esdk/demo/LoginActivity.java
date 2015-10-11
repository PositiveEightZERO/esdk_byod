/**
 * 
 */
package com.huawei.esdk.demo;

import java.io.File;
import java.net.InetSocketAddress;

import android.content.ClipboardManager;
import android.content.ClipboardManager.OnPrimaryClipChangedListener;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.media.MediaPlayer;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Process;
import android.text.Editable;
import android.text.Html;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.huawei.anyoffice.sdk.SDKContext;
import com.huawei.anyoffice.sdk.app.AppInfo;
import com.huawei.anyoffice.sdk.app.AppManager;
import com.huawei.anyoffice.sdk.login.LoginAgent;
import com.huawei.anyoffice.sdk.login.LoginParam;
import com.huawei.anyoffice.sdk.login.LoginParam.AutoLoginType;
import com.huawei.anyoffice.sdk.login.LoginParam.UserInfo;
import com.huawei.anyoffice.sdk.network.NetChangeCallback;
import com.huawei.anyoffice.sdk.network.NetStatusManager;
import com.huawei.anyoffice.sdk.sandbox.SDKClipboard;
import com.huawei.anyoffice.sdk.sandbox.SDKScreenShot;
import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.utils.BaseUtil;
import com.huawei.esdk.demo.widget.MyEditText;
import com.huawei.svn.sdk.server.SvnApiService;

/**
 * @author cWX223941
 *
 */
public class LoginActivity extends BaseActivity
{
    private static final String TAG = "LoginActivity";
    private MyEditText etGateway, etUserName, etPassword, etTunnelIP;
    private LinearLayout blockLogining, blockLogined;
    private TextView btnLogin, btnLogout, btnEnter, btnCheckUpdate, btnFaq;
    private LinearLayout btnEncryptDecrypt;
    private LinearLayout pbLogining;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        SDKContext.getInstance().init(this, "/data/data/" + getPackageName());
        setContentView(R.layout.activity_login);
        SDKScreenShot.disableScreenShot(this);

        init();
        //btnLogin.performClick();



    }

    // @Override
    // protected void onResume() {
    // // TODO Auto-generated method stub
    // super.onResume();
    // init();
    // }

    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
//        LinearLayout test = (LinearLayout) findViewById(R.id.layout_test);
//        test.setOnClickListener(new OnClickListener()
//        {
//            
//            @Override
//            public void onClick(View v)
//            {
//                // TODO Auto-generated method stub
//                System.err.println("test click");
//            }
//        });
//       TextView tv = (TextView) findViewById(R.id.edit_test1);
//       tv.setText(Html.fromHtml("<b>text3:</b>  Text with a "
//               + "<a href=\"http://www.google.com\">link</a> "
//               + "created in the Java source code using HTML."));
//       //tv.setMovementMethod(LinkMovementMethod.getInstance()); 
        
        etGateway = (MyEditText) findViewById(R.id.met_login_gateway);
        etUserName = (MyEditText) findViewById(R.id.met_login_username);
        etPassword = (MyEditText) findViewById(R.id.met_login_password);
        etTunnelIP = (MyEditText) findViewById(R.id.met_login_tunnelip);
        blockLogining = (LinearLayout) findViewById(R.id.layout_login_logining_block);
        blockLogined = (LinearLayout) findViewById(R.id.layout_login_logined_block);
        btnLogin = (TextView) findViewById(R.id.btn_login_login);
        btnLogout = (TextView) findViewById(R.id.btn_logout);
        btnEnter = (TextView) findViewById(R.id.btn_login_enter);
        btnCheckUpdate = (TextView) findViewById(R.id.btn_check_update);
        btnEncryptDecrypt = (LinearLayout) findViewById(R.id.btn_login_encrypt_decrypt);
        btnFaq = (TextView) findViewById(R.id.btn_login_faq);
        pbLogining = (LinearLayout) findViewById(R.id.layout_login_logining_progress);
        btnLogin.setOnClickListener(onClickListener);
        btnLogout.setOnClickListener(onClickListener);
        btnEnter.setOnClickListener(onClickListener);
        btnCheckUpdate.setOnClickListener(onClickListener);
        btnEncryptDecrypt.setOnClickListener(onClickListener);
        btnFaq.setOnClickListener(onClickListener);
        //etGateway.setTextChangedListener(textWatcher);
        //etUserName.setTextChangedListener(textWatcher);
        //etPassword.setTextChangedListener(textWatcher);
    }

    private void initData()
    {
        etGateway.setHintText(R.string.et_gateway);
        etUserName.setHintText(R.string.et_username);
        etPassword.setHintText(R.string.et_password);
        etTunnelIP.setHintText(R.string.et_tunnelip);
         etGateway.setText(Constants.LOGIN_IP);
//         etUserName.setText(Constants.LOGIN_USERNAME);
//         etPassword.setText(Constants.LOGIN_PASSWORD);
    }

    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnLogin.getId())
            {
                // do login
                doLogin();
            }
            else if (v.getId() == btnLogout.getId())
            {
                // do logout
                doLogout();
            }
            else if (v.getId() == btnEnter.getId())
            {
                Intent intent = new Intent(LoginActivity.this,
                        MenuActivity.class);
                LoginActivity.this.startActivity(intent);
            }
            else if (v.getId() == btnCheckUpdate.getId())
            {
                // check update
                doCheckUpdate();
            }
            else if (v.getId() == btnEncryptDecrypt.getId())
            {
                // do encrypt direct
                doEncryptWithoutLogin();
            }
            else if (v.getId() == btnFaq.getId())
            {
                // jump to faq
                Intent intent = new Intent(LoginActivity.this,
                        FaqsActivity.class);
                intent.putExtra(Constants.ACTIVITY_SEND_FAQ, "faqs_tunnel");
                LoginActivity.this.startActivity(intent);
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
                // set login button to be enable
                if (!btnLogin.isEnabled())
                {
                    Log.e(TAG, "start set login button to be enable.");
                    btnLogin.setEnabled(true);
                    btnLogin.setTextColor(getResources().getColor(
                            R.color.btn_content));
                }
            }
            else if (btnLogin.isEnabled())
            {
                // set login button to be not enable
                if (btnLogin.isEnabled())
                {
                    btnLogin.setEnabled(false);
                    btnLogin.setTextColor(getResources().getColor(
                            R.color.btn_content_unable));
                }
            }
        }
    };

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        if (KeyEvent.KEYCODE_BACK == keyCode)
        {
            SDKContext.getInstance().uninit();
            Process.killProcess(Process.myPid());
        }
        return super.onKeyDown(keyCode, event);
    }

    /**
     * if all edit text have content,then return true,otherwise return false
     * 
     * @return true:all have content;false:otherwise
     * */
    private boolean checkEditContent()
    {
        if (etGateway.hasContent() && etUserName.hasContent()
                && etPassword.hasContent()
        // && loginActivity.etTunnelIP.hasContent()
        )
        {
            return true;
        }
        return false;
    }

    private Handler loginHandler = new Handler()
    {
        public void handleMessage(Message msg)
        {
            handleLoginResult(msg.arg1, msg.arg2);
        }
    };

    /**
     * handler all login status
     * 
     * @param status
     * @param loginActivity
     * */
    private void handleLoginResult(int status, int ret)
    {
        switch (status)
        {
        case NetStatusManager.NET_STATUS_ONLINE:
            Log.e(TAG,
                    "Tunnel is online, tunnel ip:"
                            + SvnApiService.getIpAddress());
            showOnlineView();
            BaseUtil.showToast(R.string.tunnel_online, this);

            break;
        case NetStatusManager.NET_STATUS_OFFLINE:
            showOfflineView();
            BaseUtil.showToast(
                    String.format(getString(R.string.tunnel_offline), ret),
                    this);
            break;
        default:

            break;

        }
    }

    /**
     * handler all loginWithout status
     * 
     * @param status
     * @param loginActivity
     * */
    private void handleWithOutLoginResult(int status)
    {
        switch (status)
        {
        case 0:
            // BaseUtil.showToast(R.string.login_success, this);
            break;
        default:
            BaseUtil.showToast(R.string.login_false, this);
            break;
        }
    }

    private void showOfflineView()
    {
        Log.i(TAG, "showOfflineView");
        blockLogining.setVisibility(View.VISIBLE);
        blockLogined.setVisibility(View.GONE);
        btnLogin.setVisibility(View.VISIBLE);
        pbLogining.setVisibility(View.GONE);
        btnEncryptDecrypt.setVisibility(View.VISIBLE);
    }

    private void showOnlineView()
    {
        Log.i(TAG, "showOnlineView");
        blockLogining.setVisibility(View.GONE);
        blockLogined.setVisibility(View.VISIBLE);
        String tunnelIp = SvnApiService.getIpAddress();
        etTunnelIP.setText(tunnelIp);
        btnEncryptDecrypt.setVisibility(View.GONE);
    }

    private void showLogingView()
    {
        Log.i(TAG, "showLogingView");
        blockLogining.setVisibility(View.VISIBLE);
        blockLogined.setVisibility(View.GONE);
        btnLogin.setVisibility(View.GONE);
        pbLogining.setVisibility(View.VISIBLE);
        btnEncryptDecrypt.setVisibility(View.GONE);
    }

    /**
     * check the VPN connect status
     * 
     * @date 2014.6.5
     * */
    private static boolean isConnectedVPN()
    {
        int status = NetStatusManager.getInstance().getNetStatus();
        return (NetStatusManager.NET_STATUS_ONLINE == status);
    }

    private void doLogin()
    {
        showLogingView();
        // login task
        AsyncTask<Object, Integer, Integer> loginTask = new AsyncTask<Object, Integer, Integer>()
        {
            @Override
            protected Integer doInBackground(Object... paramVarArgs)
            {
                Log.i(TAG, "login");
                // 1.get data for login
                String gateway = etGateway.getText().trim();
                String username = etUserName.getText().trim();
                String password = etPassword.getText().trim();
                // 初始化SDK工作环境
                String workPath = "/data/data/" + getPackageName();
                File f = new File(workPath);
                if (!f.exists())
                {
                    f.mkdir();
                }
                Log.i(TAG, "Begin to init sdk envirenment");
                boolean inited = SDKContext.getInstance().init(
                        LoginActivity.this, workPath);
                Log.i(TAG, "SDKContext.getInstance().init:" + inited);
                NetStatusManager.getInstance().setNetChangeCallback(callback);
                int status = NetStatusManager.getInstance().getNetStatus();
                Log.i(TAG, "NetStatusManager.getInstance().getNetStatus():"
                        + status);
                String res = "Login SVN Success";
                // System.out.println("Begin to create login param");
                // 初始化登录参数
                LoginParam loginParam = new LoginParam();
                // 设置应用的业务类型，
                loginParam.setServiceType(getPackageName());
                loginParam.setLoginTitle("SvnSdkDemo");
                loginParam.setAutoLoginType(AutoLoginType.auto_login_enable);
                // loginParam.setLoginBackground(true);
                loginParam.setLoginBackground(false);
                
                //不设置登录参数，从AnyOffce获取，获取不到将界面输入
                
                // 设置AnyOffice网关地址
//                 loginParam.setInternetAddress(new
//                 InetSocketAddress(gateway,443));
//                 loginParam.setIntranetAddress(new InetSocketAddress(
//                         gateway, 443));
                // 设置用户信息
//                UserInfo userInfo = loginParam.new UserInfo();
//                userInfo.userName = username;
//                userInfo.password = password;
//                Log.i(TAG, "LoginParam username:" + username + ",password:"
//                        + password);
//                 loginParam.setUserInfo(userInfo);
                loginParam.setUseSecureTransfer(true);
                System.out.println("Begin to login gateway");
                int ret = LoginAgent.getInstance().loginSync(
                        LoginActivity.this, loginParam);

                Log.i(TAG, "login result " + ret);
                if (ret == 0)
                {
//                    LoginParam retParam = LoginAgent.getInstance()
//                            .getLoginParam();
//                    Log.i(TAG,
//                            "get login param, autologinflag is "
//                                    + retParam.getAutoLoginType());
//                    Log.i(TAG,
//                            "get login param, service type is "
//                                    + retParam.getServiceType());

                    status = NetStatusManager.getInstance().getNetStatus();
                    Log.i(TAG, "NetStatusManager.getInstance().getNetStatus():"
                            + status);
                    if (status != NetStatusManager.NET_STATUS_CONNECTING)
                    {
                        Message msg = loginHandler.obtainMessage();
                        msg.what = 0;
                        msg.arg1 = status;
                        msg.arg2 = ret;
                        loginHandler.sendMessage(msg);
                    }
                }
                else
                {
                    res = "login error, ErrorCode:" + ret;

                }

                return ret;
            }

            protected void onPostExecute(Integer result)
            {
                BaseUtil.showToast("loginSync return:" + result,
                        LoginActivity.this);
                if (result != 0)
                {
                    showOfflineView();
                }
                else
                {
                    LoginParam loginParam = LoginAgent.getInstance().getLoginParam();
                    if(loginParam != null)
                    {
                        InetSocketAddress gatewayAddress = loginParam.getInternetAddress();
                        if(gatewayAddress != null)
                        {
                            Log.i(TAG, "gatewayAddress:" + gatewayAddress.toString());
                        }
                        
                        
                        UserInfo userInfo = LoginAgent.getInstance().getUserInfo();

                        if (userInfo != null)
                        {
                            Log.i(TAG, "logined userInfo username:"
                                    + userInfo.userName + ",password:"
                                    + userInfo.password);
                        }
                        else
                        {
                            Log.i(TAG, "no logined userInfo");
                        }
                    }
                    else 
                    {
                        Log.i(TAG, "no loginParam");
                    }
                    
                    
                  

                }
            }

        };
        // start login task
        loginTask.execute(new Object());

    }


    private NetChangeCallback callback = new NetChangeCallback()
    {
        @Override
        public void onNetChanged(int oldState, int newState, int errorCode)
        {
            Log.e(TAG, String.format(
                    "oldState:%d, newState:%d, errorCode:%d, ", oldState,
                    newState, errorCode));
            Message msg = loginHandler.obtainMessage();
            msg.what = 0;
            msg.arg1 = newState;
            msg.arg2 = errorCode;
            loginHandler.sendMessage(msg);
        }
    };

    private void doLogout()
    {
        //清空账户信息
        SDKContext.getInstance().reset();

    }

    // 检查应用更新
    private void doCheckUpdate()
    {
        Log.i(TAG, "doCheckUpdate");
        AppManager manager = new AppManager();
        PackageInfo pInfo = null;
        try
        {
            pInfo = getPackageManager().getPackageInfo(getPackageName(), 0);
        }
        catch (NameNotFoundException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        if (pInfo != null)
        {
            // 当前版本
            String version = pInfo.versionName;
            // 更新信息
            AppInfo appinfo = manager.checkUpdateWithAppid(getPackageName(),
                    version, 0);
            if (appinfo != null)
            {
                // 有更新，应用商店安装
                System.out.println(appinfo.getAppVersion());
                manager.installPackage(this, getPackageName());
            }
            else
            {
                // 无更新
                BaseUtil.showToast("no update for:" + getPackageName(), this);
            }
        }
    }

    private void doEncryptWithoutLogin()
    {
        Log.i(TAG, "doEncryptWithoutLogin");
        // 初始化参数
        String gateway = etGateway.getText().trim();
        String username = etUserName.getText().trim();

        if (!SDKContext.getInstance().sdkInitComplete())
        {

            String workPath = "/data/data/" + getPackageName();
            File f = new File(workPath);
            if (!f.exists())
            {
                f.mkdir();
            }
            Log.i(TAG, "Begin to init sdk envirenment");

            // 初始化SDK工作环境
            boolean inited = SDKContext.getInstance().init(LoginActivity.this,
                    username, workPath);
            Log.i(TAG, "SDKContext.getInstance().init:" + inited);
        }

        int res = LoginAgent.getInstance().doAppAuthentication(
                new InetSocketAddress(gateway, 443), null);
        Log.i(TAG, "doAppAuthentication ret:" + res);
//        if (res == 0)
//        {
            // jump to encrypt activity
            Intent intent = new Intent(this, EnDecryptMenuActivity.class);
            startActivity(intent);
//        }
//        else {
//        	
//		}

    }
}
