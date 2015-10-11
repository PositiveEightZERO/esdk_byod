/**
 * 
 */
package com.huawei.esdk.demo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;

import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;

/**
 * @author cWX223941
 *
 */
public class GuideActivity extends BaseActivity
{
    private static final String TAG = "GuideActivity";
    private TextView btnBack;
    //    private TextView                            subTitle;
    //    protected LinearLayout                      conContainer;
    private String guideType;
    //    private TextView                            tvContent;
    private LinearLayout conContainer;
    private LayoutParams titleParams;
    private LayoutParams conParams;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_http_guide);
        init();
    }

    private void init()
    {
        initView();
        initData();
        initListener();
    }

    @SuppressLint("CutPasteId")
    private void initView()
    {
        btnBack = (TextView) findViewById(R.id.btn_http_guide_back);
        //        subTitle                            = (TextView)findViewById(R.id.tv_http_guide_subtitle);
        //        tvContent                           = (TextView)findViewById(R.id.tv_http_guide_subtitle);
        conContainer = (LinearLayout) findViewById(R.id.layout_http_guide_container);
    }

    private void initData()
    {
        guideType = (String) this.getIntent().getStringExtra(
                Constants.GUIDE_CATEGORY);
        //        subTitle.setText("5 Steps to step up http" + guideType + ":");
        //        initDataByGuideType(guideType);
        titleParams = new LayoutParams(LayoutParams.MATCH_PARENT,
                LayoutParams.WRAP_CONTENT);
        titleParams.gravity = Gravity.CENTER_VERTICAL | Gravity.LEFT;
        titleParams.topMargin = 10;
        titleParams.bottomMargin = 10;
        conParams = new LayoutParams(LayoutParams.MATCH_PARENT,
                LayoutParams.WRAP_CONTENT);
        conParams.gravity = Gravity.CENTER_VERTICAL | Gravity.LEFT;
        conParams.topMargin = 5;
        conParams.bottomMargin = 5;
        loadContent();
    }

    private void initListener()
    {
        btnBack.setOnClickListener(onClickListener);
    }
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnBack.getId())
            {
                GuideActivity.this.finish();
            }
        }
    };

    //    public void setContent(String res){
    //        tvContent.setText(res);
    //    }
    //    private void initDataByGuideType(String type){
    //        if(StringUtil.isEmpty(type)){
    //            return;
    //        }
    //        String content = getGuideConByType(type);
    //        setContent(content);
    //    }
    private void loadContent()
    {
        InputStream inputStream = null;
        InputStreamReader inputStreamReader = null;
        try
        {
            inputStream = getAssets().open(getFilePath(guideType));
            inputStreamReader = new InputStreamReader(inputStream);
        }
        catch (UnsupportedEncodingException e1)
        {
            e1.printStackTrace();
        }
        catch (IOException e2)
        {
            e2.printStackTrace();
        }
        BufferedReader reader = new BufferedReader(inputStreamReader);
        String line;
        try
        {
            boolean isFirst = true;
            while ((line = reader.readLine()) != null)
            {
                if (isFirst)
                {
                    isFirst = false;
                    conContainer.addView(getTitleTextView(line.trim()));
                    continue;
                }
                Log.d(TAG, "add content:*" + line + "*");
                conContainer.addView(getConTextView(line));
            }
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    private TextView getTitleTextView(String con)
    {
        TextView titleView = new TextView(this);
        titleView.setText(con);
        titleView.setTextColor(getResources().getColor(R.color.black));
        titleView.setLayoutParams(titleParams);
        return titleView;
    }

    private TextView getConTextView(String con)
    {
        TextView conView = new TextView(this);
        conView.setText(con);
        conView.setTextColor(getResources().getColor(R.color.con_grey));
        conView.setLayoutParams(conParams);
        return conView;
    }

    //    private String getGuideConByType(String type){
    //        String content = "";
    //        if(StringUtil.isEmpty(type)){
    //            return null;
    //        }
    //        if(Constants.GUIDE_CATEGORY_URLCONNECTION.equals(type)){
    //            content = BaseUtil.getContentFromAssetFile(Constants.ASSETS_GUIDE_URLCONNECTION_FILE, this);
    //        }else if(Constants.GUIDE_CATEGORY_HTTPCLIENT.equals(type)){
    //            content = BaseUtil.getContentFromAssetFile(Constants.ASSETS_GUIDE_HTTPCLIENT_FILE, this);
    //        }
    //        return content;
    //    }
    private String getFilePath(String type)
    {
        String filePath = "";
        if (Constants.GUIDE_CATEGORY_URLCONNECTION.equals(type))
        {
            filePath = Constants.ASSETS_GUIDE_URLCONNECTION_FILE;
        }
        else if (Constants.GUIDE_CATEGORY_HTTPCLIENT.equals(type))
        {
            filePath = Constants.ASSETS_GUIDE_HTTPCLIENT_FILE;
        }
        else if (Constants.GUIDE_CATEGORY_FILE_ENDECRYPT.equals(type))
        {
            filePath = Constants.ASSETS_GUIDE_FILE_ENDECRYPT_FILE;
        }
        else if (Constants.GUIDE_CATEGORY_DATA_ENDECRYPT.equals(type))
        {
            filePath = Constants.ASSETS_GUIDE_DATA_ENDECRYPT_FILE;
        }
        return filePath;
    }
}
