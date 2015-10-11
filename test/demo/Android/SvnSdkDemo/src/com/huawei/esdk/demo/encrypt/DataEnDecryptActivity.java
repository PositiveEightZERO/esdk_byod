/**
 * 
 */
package com.huawei.esdk.demo.encrypt;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.huawei.anyoffice.sdk.sandbox.EncryptTool;
import com.huawei.esdk.demo.GuideActivity;
import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.utils.BaseUtil;
import com.huawei.esdk.demo.utils.StringUtil;
import com.huawei.esdk.demo.utils.ThemeUtil;
//import com.huawei.svn.sdk.server.SvnApiService;

/**
 * @author cWX223941
 *
 */
public class DataEnDecryptActivity extends BaseActivity
{
    private static final String TAG = "DataEnDecryptActivity";
    private TextView btnEncrypt, btnDecrypt, btnBack, btnGuide;
    private EditText etOriginData;
    private TextView tvEncryptedData;
    private ImageView iconLock;
    byte[] encryptedData = null;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_data_endecrypt);
        init();
    }

    private void init()
    {
        initView();
        initData();
        initListener();
    }

    private void initView()
    {
        btnEncrypt = (TextView) findViewById(R.id.btn_dataencrypt_encrypt);
        btnDecrypt = (TextView) findViewById(R.id.btn_dataencrypt_decrypt);
        btnBack = (TextView) findViewById(R.id.btn_dataencrypt_back);
        btnGuide = (TextView) findViewById(R.id.btn_dataencrypt_guide);
        etOriginData = (EditText) findViewById(R.id.et_dataencrypt_data);
        tvEncryptedData = (TextView) findViewById(R.id.tv_dataencrypt_result);
        iconLock = (ImageView) findViewById(R.id.iv_dataencrypt_lock);
    }

    private void initData()
    {
        btnEncrypt.setEnabled(false);
        btnDecrypt.setEnabled(false);
    }

    private void initListener()
    {
        btnEncrypt.setOnClickListener(onClickListener);
        btnDecrypt.setOnClickListener(onClickListener);
        btnBack.setOnClickListener(onClickListener);
        btnGuide.setOnClickListener(onClickListener);
        etOriginData.addTextChangedListener(encryptTextWatcher);
        tvEncryptedData.addTextChangedListener(decryptTextWatcher);
    }
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnEncrypt.getId())
            {
                doEncrypt();
            }
            else if (v.getId() == btnDecrypt.getId())
            {
                doDecrypt();
            }
            else if (v.getId() == btnBack.getId())
            {
                DataEnDecryptActivity.this.finish();
            }
            else if (v.getId() == btnGuide.getId())
            {
                Intent intent = new Intent(DataEnDecryptActivity.this,
                        GuideActivity.class);
                intent.putExtra(Constants.GUIDE_CATEGORY,
                        Constants.GUIDE_CATEGORY_DATA_ENDECRYPT);
                DataEnDecryptActivity.this.startActivity(intent);
            }
        }
    };
    private TextWatcher encryptTextWatcher = new TextWatcher()
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
            if (checkEncryptEditContent())
            {
                //set login button to be enable
                ThemeUtil
                        .setBtnToEnable(btnEncrypt, DataEnDecryptActivity.this);
            }
            else
            {
                //set login button to be not enable
                ThemeUtil
                        .setBtnToUnable(btnEncrypt, DataEnDecryptActivity.this);
            }
        }
    };
    private TextWatcher decryptTextWatcher = new TextWatcher()
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
            if (checkDecryptEditContent())
            {
                //set login button to be enable
                ThemeUtil
                        .setBtnToEnable(btnDecrypt, DataEnDecryptActivity.this);
            }
            else
            {
                //set login button to be not enable
                ThemeUtil
                        .setBtnToUnable(btnDecrypt, DataEnDecryptActivity.this);
            }
        }
    };

    /**
     * if Encrypt Data edit text have content,then return true,otherwise return false
     * @return true:all have content;false:otherwise
     * */
    private boolean checkEncryptEditContent()
    {
        if (!StringUtil.isEmpty(etOriginData.getText().toString()))
        {
            return true;
        }
        return false;
    }

    /**
     * if Decrypt Data edit text have content,then return true,otherwise return false
     * @return true:all have content;false:otherwise
     * */
    private boolean checkDecryptEditContent()
    {
        if (!StringUtil.isEmpty(tvEncryptedData.getText().toString()))
        {
            return true;
        }
        return false;
    }

    //数据加密
    private void doEncrypt()
    {
        String originText = etOriginData.getText().toString().trim();
        if (StringUtil.isEmpty(originText))
        {
            Log.e(TAG, "DecryptData is empty");
            return;
        }
        
        //加密接口
        encryptedData = EncryptTool.Encrypt(originText);

        tvEncryptedData.setText(BaseUtil.Bytes2HexString(encryptedData));
        
        iconLock.setImageResource(R.drawable.icon_lock_black);
        btnDecrypt.setVisibility(View.VISIBLE);
    }

    
    //数据解密
    private void doDecrypt()
    {
        if (null == encryptedData)
        {
            Log.e(TAG, "Encrypt Byte is null.");
            return;
        }
        
        //解密接口
        String result = EncryptTool.Decrypt(encryptedData);
        etOriginData.setText(result);
        
        iconLock.setImageResource(R.drawable.icon_unlock_black);
        btnEncrypt.setVisibility(View.VISIBLE);
    }


}
