/**
 * 
 */
package com.huawei.esdk.demo;

import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;

import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.common.MenuItemEntity;
import com.huawei.esdk.demo.encrypt.DataEnDecryptActivity;
import com.huawei.esdk.demo.encrypt.FileEnDecryptActivity;
import com.huawei.esdk.demo.encrypt.SqliteActivity;
import com.huawei.esdk.demo.widget.MenuItemView;
import com.huawei.esdk.demo.widget.OnMenuItemClickListener;

/**
 * @author cWX223941
 *
 */
public class EnDecryptMenuActivity extends BaseActivity implements OnMenuItemClickListener
{
    //    private static final String TAG     = "EnDecryptMenuActivity";
    private TextView btnBack, btnFaq;
    //    private LinearLayout                listContainer;
    private MenuItemView itemEncryptDecrypt;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_endecrypt_menu);
        init();
    }

    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
        btnBack = (TextView) findViewById(R.id.btn_endecrypt_menu_back);
        btnFaq = (TextView) findViewById(R.id.btn_endecrypt_menu_faq);
        //        listContainer                   = (LinearLayout)findViewById(R.id.layout_endecrypt_menu_list_container);
        itemEncryptDecrypt = (MenuItemView) findViewById(R.id.item_endecrypt_menu_encryptdecrypt);
        
        btnBack.setOnClickListener(onClickListener);
        btnFaq.setOnClickListener(onClickListener);
        
        itemEncryptDecrypt.setOnClickListener(onMenuClickListener);
        itemEncryptDecrypt.setItemClickListener(this);
    }

    private void initData()
    {
        itemEncryptDecrypt.setItemName(getString(R.string.menu_encrypt_decrypt_test));
        itemEncryptDecrypt.setChildList(getEncryptDecryptMenuList());
    }

    
    
    private OnClickListener onMenuClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            MenuItemView menu = (MenuItemView) v;
            menu.setExpanded(!menu.isExpanded());
   
        }
    };
    
    
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnBack.getId())
            {
                EnDecryptMenuActivity.this.finish();
            }
            else if (v.getId() == btnFaq.getId())
            {
                Intent intent = new Intent(EnDecryptMenuActivity.this,
                        FaqsActivity.class);
                
                String faqsType = "faqs_encrypt";
                         
                intent.putExtra(Constants.ACTIVITY_SEND_FAQ, faqsType);
                
                startActivity(intent);
            }
        }
    };



    private ArrayList<MenuItemEntity> getEncryptDecryptMenuList()
    {
        ArrayList<MenuItemEntity> encryptDecryptList = new ArrayList<MenuItemEntity>();
        MenuItemEntity fileEncryptDecrypt = new MenuItemEntity(getString(
                R.string.file_encrypt_decrypt), Constants.MENU_ENCRYPT_FILE);
        MenuItemEntity dataEncryptDecrypt = new MenuItemEntity(
                getString(R.string.data_encrypt_decrypt), Constants.MENU_ENCRYPT_DATA);
        MenuItemEntity sqlite = new MenuItemEntity(getString(R.string.use_sqlite), Constants.MENU_ENCRYPT_SQLITE);
        encryptDecryptList.add(fileEncryptDecrypt);
        encryptDecryptList.add(dataEncryptDecrypt);
        encryptDecryptList.add(sqlite);
        return encryptDecryptList;
    }

    @Override
    public void onMenuItemClicked(MenuItemEntity menu)
    {
        Intent intent = null;
        switch (menu.getAction())
        {
            case Constants.MENU_ENCRYPT_FILE:
                intent = new Intent(EnDecryptMenuActivity.this,
                        FileEnDecryptActivity.class);
                EnDecryptMenuActivity.this.startActivity(intent);
                break;
            case Constants.MENU_ENCRYPT_DATA:
                intent = new Intent(EnDecryptMenuActivity.this,
                        DataEnDecryptActivity.class);
                EnDecryptMenuActivity.this.startActivity(intent);
                break;
            case Constants.MENU_ENCRYPT_SQLITE:
                intent = new Intent(EnDecryptMenuActivity.this,
                        SqliteActivity.class);
                EnDecryptMenuActivity.this.startActivity(intent);
                break;
            default:
                break;
        }
        
    }
}
