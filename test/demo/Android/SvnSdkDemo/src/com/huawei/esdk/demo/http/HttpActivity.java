/**
 * 
 */
package com.huawei.esdk.demo.http;

import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;

import com.huawei.esdk.demo.GuideActivity;
import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;

/**
 * @author cWX223941
 *
 */
public class HttpActivity extends BaseActivity
{
    private static final String TAG = "HttpActivity";
    private TextView btnLogin, btnUserInfo, btnDownload, btnUpload;
    private TextView btnBack, btnGuide;
    private ViewPager httpViewPager;
    private ArrayList<TextView> navBtnList;
    private HttpPagerAdapter pageAdapter;
//    private UserInfoEntity userInfo;
    private String httpType;
    private TextView httpTitle;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_http);
        init();
    }

    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
        btnLogin = (TextView) findViewById(R.id.btn_http_nav_login);
        btnUserInfo = (TextView) findViewById(R.id.btn_http_nav_userinfo);
        btnDownload = (TextView) findViewById(R.id.btn_http_nav_download);
        btnUpload = (TextView) findViewById(R.id.btn_http_nav_upload);
        btnBack = (TextView) findViewById(R.id.btn_http_back);
        btnGuide = (TextView) findViewById(R.id.btn_http_guide);
        httpViewPager = (ViewPager) findViewById(R.id.vp_http);
        httpTitle = (TextView) findViewById(R.id.tv_http_title);
        
        btnLogin.setOnClickListener(onClickListener);
        btnUserInfo.setOnClickListener(onClickListener);
        btnDownload.setOnClickListener(onClickListener);
        btnUpload.setOnClickListener(onClickListener);
        btnBack.setOnClickListener(onClickListener);
        btnGuide.setOnClickListener(onClickListener);
        httpViewPager.setOnPageChangeListener(onPageChangeListener);
    }

    private void initData()
    {
        //init navigation list
        navBtnList = new ArrayList<TextView>();
        navBtnList.add(btnLogin);
        navBtnList.add(btnUserInfo);
        navBtnList.add(btnDownload);
        navBtnList.add(btnUpload);

        pageAdapter = new HttpPagerAdapter(getSupportFragmentManager());
        httpViewPager.setAdapter(pageAdapter);
        //set cache for view pager,just 3 but set 4 for safe
        httpViewPager.setOffscreenPageLimit(4);
        //set first navigation button
        navBtnClick(btnLogin, false);
        //get http type
        httpType = getIntent().getStringExtra(Constants.ACTIVITY_SEND_HTTPTYPE);
        httpTitle.setText(httpType);
        Log.d(TAG, "httpType:" + httpType);
    }

 
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnBack.getId())
            {
                HttpActivity.this.finish();
            }
            else if (v.getId() == btnGuide.getId())
            {
                Intent intent = new Intent(HttpActivity.this,
                        GuideActivity.class);
                intent.putExtra(
                        Constants.GUIDE_CATEGORY,
                        Constants.ACTIVITY_SEND_HTTPTYPE_URLCONNECTION
                                .equals(httpType) ? Constants.GUIDE_CATEGORY_URLCONNECTION
                                : Constants.GUIDE_CATEGORY_HTTPCLIENT);
                HttpActivity.this.startActivity(intent);
            }
            else
            {
                navBtnClick(v, true);
            }
        }
    };
    private OnPageChangeListener onPageChangeListener = new OnPageChangeListener()
    {
        @Override
        public void onPageSelected(int position)
        {
            navBtnClick(navBtnList.get(position), false);
            //            pageAdapter.getFragmentByPosition(position).reLoadData();
        }

        @Override
        public void onPageScrolled(int arg0, float arg1, int arg2)
        {
        }

        @Override
        public void onPageScrollStateChanged(int arg0)
        {
        }
    };

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

    public String getHttpType()
    {
        return httpType;
    }

    /**
     * when navigation button had been click,will do view change and view pager change,
     * program can call this method which is not click action ,but should set the isClick to be false
     * @param btn
     * @param urlConnectionActivity
     * @param isClick true:is click action;false:otherwise
     * */
    public void navBtnClick(View btn, boolean isClick)
    {
        clearAllNavBtnBg();
        if (btn.getId() == btnLogin.getId())
        {
            setBtnSelectedStatus((TextView) btn);
            if (isClick)
            {
                httpViewPager.setCurrentItem(0, true);
            }
        }
        else if (btn.getId() == btnUserInfo.getId())
        {
            setBtnSelectedStatus((TextView) btn);
            if (isClick)
            {
                httpViewPager.setCurrentItem(1, true);
            }
        }
        else if (btn.getId() == btnDownload.getId())
        {
            setBtnSelectedStatus((TextView) btn);
            if (isClick)
            {
                httpViewPager.setCurrentItem(2, true);
            }
        }
        else if (btn.getId() == btnUpload.getId())
        {
            setBtnSelectedStatus((TextView) btn);
            if (isClick)
            {
                httpViewPager.setCurrentItem(3, true);
            }
        }
    }

    /**
     * clear all navigations's background and set text's color to button content
     * @param urlConnectionActivity
     * */
    public void clearAllNavBtnBg()
    {
        btnLogin.setBackgroundResource(0);
        btnUserInfo.setBackgroundResource(0);
        btnDownload.setBackgroundResource(0);
        btnUpload.setBackgroundResource(0);
        btnLogin.setTextColor(getResources().getColor(R.color.btn_content));
        btnUserInfo.setTextColor(getResources().getColor(R.color.btn_content));
        btnDownload.setTextColor(getResources().getColor(R.color.btn_content));
        btnUpload.setTextColor(getResources().getColor(R.color.btn_content));
    }

    /**
     * change one navigation button to be selected,change it's background and text's color
     * @param btn
     * @param urlConnectionActivity
     * */
    public void setBtnSelectedStatus(TextView btn)
    {
        if (btn.getId() == btnLogin.getId())
        {
            btn.setBackgroundResource(R.drawable.http_nav_leftbtn_bg_selected);
        }
        else if (btn.getId() == btnUpload.getId())
        {
            btn.setBackgroundResource(R.drawable.http_nav_rightbtn_bg_selected);
        }
        else
        {
            btn.setBackgroundResource(R.drawable.http_nav_btn_bg_selected);
        }
        btn.setTextColor(getResources().getColor(R.color.white));
    }
}
