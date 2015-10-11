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
import com.huawei.esdk.demo.encrypt.ClipboardActivity;
import com.huawei.esdk.demo.encrypt.DataEnDecryptActivity;
import com.huawei.esdk.demo.encrypt.FileEnDecryptActivity;
import com.huawei.esdk.demo.encrypt.SqliteActivity;
import com.huawei.esdk.demo.http.HttpActivity;
import com.huawei.esdk.demo.http.SpeedCompareActivity;
import com.huawei.esdk.demo.http.WebViewActivity;
import com.huawei.esdk.demo.mdm.MDMActivity;
import com.huawei.esdk.demo.widget.MenuItemView;
import com.huawei.esdk.demo.widget.OnMenuItemClickListener;

/**
 * @author cWX223941
 * 
 */
public class MenuActivity extends BaseActivity implements
        OnMenuItemClickListener {
    // private static final String TAG = "MenuActivity";
    private TextView btnBack, btnFaq;
    // private LinearLayout listContainer;
    private MenuItemView itemHttp;
    private MenuItemView itemEncryptDecrypt;
    private MenuItemView itemCheckMDM;
    //private MenuItemView itemSpeedCompare;

    private MenuItemView currentMenuItemView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
        init();
    }

    private void init() {
        initView();
        initData();
    }

    private void initView() {
        btnBack = (TextView) findViewById(R.id.btn_menu_back);
        btnFaq = (TextView) findViewById(R.id.btn_menu_faq);
        // listContainer =
        // (LinearLayout)findViewById(R.id.layout_menu_list_container);
        itemHttp = (MenuItemView) findViewById(R.id.item_menu_http);
        itemEncryptDecrypt = (MenuItemView) findViewById(R.id.item_menu_encryptdecrypt);
        itemCheckMDM = (MenuItemView) findViewById(R.id.item_menu_mdm);
        //itemSpeedCompare = (MenuItemView) findViewById(R.id.item_menu_speed);
        btnBack.setOnClickListener(onClickListener);
        btnFaq.setOnClickListener(onClickListener);
        itemHttp.setOnClickListener(onMenuClickListener);
        itemEncryptDecrypt.setOnClickListener(onMenuClickListener);
        itemCheckMDM.setOnClickListener(onMenuClickListener);
        //itemSpeedCompare.setOnClickListener(onMenuClickListener);
        itemHttp.setItemClickListener(this);
        itemEncryptDecrypt.setItemClickListener(this);
        itemCheckMDM.setItemClickListener(this);
        //itemSpeedCompare.setItemClickListener(this);

        currentMenuItemView = itemHttp;
    }

    private void initData() {
        itemHttp.setItemName(this.getString(R.string.menu_http_test));
        itemHttp.setChildList(getHttpMenuList());
        itemEncryptDecrypt.setItemName(this
                .getString(R.string.menu_encrypt_decrypt_test));
        itemEncryptDecrypt.setChildList(getEncryptDecryptMenuList());
        itemCheckMDM.setItemName(this.getString(R.string.menu_mdm_test));
        itemCheckMDM.setChildList(getMDMMenuList());
    }

    private OnClickListener onMenuClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            MenuItemView menu = (MenuItemView) v;
            menu.setExpanded(!menu.isExpanded());

            currentMenuItemView = menu;

            if (menu == itemHttp) {
                itemEncryptDecrypt.setExpanded(false);
                itemCheckMDM.setExpanded(false);
            } else if (menu == itemEncryptDecrypt) {
                itemHttp.setExpanded(false);
                itemCheckMDM.setExpanded(false);
            } else{
                itemHttp.setExpanded(false);
                itemEncryptDecrypt.setExpanded(false);
            }

        }
    };
    private OnClickListener onClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            if (v.getId() == btnBack.getId()) {
                MenuActivity.this.finish();
            } else if (v.getId() == btnFaq.getId()) {
                Intent intent = new Intent(MenuActivity.this,
                        FaqsActivity.class);

                String faqsType = "faqs_http";

                if (currentMenuItemView == itemEncryptDecrypt) {
                    faqsType = "faqs_encrypt";
                } else if (currentMenuItemView == itemCheckMDM) {
                    faqsType = "faqs_mdm";
                }

                intent.putExtra(Constants.ACTIVITY_SEND_FAQ, faqsType);

                MenuActivity.this.startActivity(intent);
            } else if (v.getId() == itemCheckMDM.getId()) {
                Intent intent = new Intent(MenuActivity.this, MDMActivity.class);
                MenuActivity.this.startActivity(intent);
            }
        }
    };

    private ArrayList<MenuItemEntity> getHttpMenuList() {
        ArrayList<MenuItemEntity> httpList = new ArrayList<MenuItemEntity>();
        MenuItemEntity httpClient = new MenuItemEntity(
                getString(R.string.httpclient), Constants.MENU_HTTP_HTTPCLIENT);
        MenuItemEntity urlConnection = new MenuItemEntity(
                getString(R.string.urlconnection),
                Constants.MENU_HTTP_URLCONNECTION);
        MenuItemEntity webview = new MenuItemEntity(
                getString(R.string.webview), Constants.MENU_HTTP_WEBVIEW);
        
        MenuItemEntity speedEntity = new MenuItemEntity(
                getString(R.string.speed_compare), Constants.MENU_SPEED_COMPARE);
        
        httpList.add(httpClient);
        httpList.add(urlConnection);
        httpList.add(webview);
        httpList.add(speedEntity);
        return httpList;
    }

    private ArrayList<MenuItemEntity> getEncryptDecryptMenuList() {
        ArrayList<MenuItemEntity> encryptDecryptList = new ArrayList<MenuItemEntity>();
        MenuItemEntity fileEncryptDecrypt = new MenuItemEntity(
                getString(R.string.file_encrypt_decrypt),
                Constants.MENU_ENCRYPT_FILE);
        MenuItemEntity dataEncryptDecrypt = new MenuItemEntity(
                getString(R.string.data_encrypt_decrypt),
                Constants.MENU_ENCRYPT_DATA);
        MenuItemEntity sqlite = new MenuItemEntity(
                getString(R.string.use_sqlite), Constants.MENU_ENCRYPT_SQLITE);
        MenuItemEntity clipboard = new MenuItemEntity(
                getString(R.string.use_clipboard), Constants.MENU_ENCRYPT_CLIPBOARD);
        encryptDecryptList.add(fileEncryptDecrypt);
        encryptDecryptList.add(dataEncryptDecrypt);
        encryptDecryptList.add(sqlite);
        
        encryptDecryptList.add(clipboard);
        return encryptDecryptList;
    }

    private ArrayList<MenuItemEntity> getMDMMenuList() {
        ArrayList<MenuItemEntity> mdmList = new ArrayList<MenuItemEntity>();
        MenuItemEntity mdmCheck = new MenuItemEntity(
                getString(R.string.mdm_check), Constants.MENU_MDM_CHECK);
        mdmList.add(mdmCheck);
        return mdmList;
    }

    private ArrayList<MenuItemEntity> getSpeedList() {
        ArrayList<MenuItemEntity> speedList = new ArrayList<MenuItemEntity>();
        MenuItemEntity speedEntity = new MenuItemEntity(
                getString(R.string.speed_compare), Constants.MENU_SPEED_COMPARE);
        speedList.add(speedEntity);
        return speedList;
    }

    @Override
    public void onMenuItemClicked(MenuItemEntity menu) {
        Intent intent = null;
        switch (menu.getAction()) {
        case Constants.MENU_HTTP_HTTPCLIENT:
            intent = new Intent(MenuActivity.this, HttpActivity.class);
            intent.putExtra(Constants.ACTIVITY_SEND_HTTPTYPE,
                    Constants.ACTIVITY_SEND_HTTPTYPE_HTTPCLIENT);
            MenuActivity.this.startActivity(intent);
            break;
        case Constants.MENU_HTTP_URLCONNECTION:
            intent = new Intent(MenuActivity.this, HttpActivity.class);
            intent.putExtra(Constants.ACTIVITY_SEND_HTTPTYPE,
                    Constants.ACTIVITY_SEND_HTTPTYPE_URLCONNECTION);
            MenuActivity.this.startActivity(intent);
            break;
        case Constants.MENU_HTTP_WEBVIEW:
            intent = new Intent(MenuActivity.this, WebViewActivity.class);
            MenuActivity.this.startActivity(intent);
            break;
            
        case Constants.MENU_SPEED_COMPARE:
            intent=new Intent(MenuActivity.this, SpeedCompareActivity.class);
            MenuActivity.this.startActivity(intent);            
            break;
        case Constants.MENU_ENCRYPT_FILE:
            intent = new Intent(MenuActivity.this, FileEnDecryptActivity.class);
            MenuActivity.this.startActivity(intent);
            break;
        case Constants.MENU_ENCRYPT_DATA:
            intent = new Intent(MenuActivity.this, DataEnDecryptActivity.class);
            MenuActivity.this.startActivity(intent);
            break;
        case Constants.MENU_ENCRYPT_SQLITE:
            intent = new Intent(MenuActivity.this, SqliteActivity.class);
            MenuActivity.this.startActivity(intent);
            break;
        case Constants.MENU_ENCRYPT_CLIPBOARD:
            intent = new Intent(MenuActivity.this, ClipboardActivity.class);
            MenuActivity.this.startActivity(intent);
            break;
            
        case Constants.MENU_MDM_CHECK:
            intent = new Intent(MenuActivity.this, MDMActivity.class);
            MenuActivity.this.startActivity(intent);
            break;

            
      
        default:
            break;
        }
    }
}
