/**
 * 
 */
package com.huawei.esdk.demo.common;

import com.huawei.esdk.demo.utils.FileUtil;

/**
 * @author cWX223941
 *
 */
public class Constants
{
    
    /* ############################################# Login Information ##################################################################### */
    public static final String LOGIN_USERNAME = "lzy";
    public static final String LOGIN_PASSWORD = "Admin@123";
    public static final String LOGIN_IP = "10.170.26.241";
    
//    public static final String LOGIN_USERNAME = "showtest";
//    public static final String LOGIN_PASSWORD = "Admin@123";
//    public static final String LOGIN_IP = "58.60.106.145";

     
    /* ############################################# HTTP Test ##################################################### */

    public static final String HTTP_TEST_SERVER = "172.22.8.206";
//    public static final String HTTP_TEST_SERVER = "172.19.110.5";
    public static final String HTTP_TEST_PORT = "8080";
    
    public static final String HTTP_TEST_BASE_URL = "http://"
            + HTTP_TEST_SERVER + ":" + HTTP_TEST_PORT
            + "/HttpServerDemo/e";
    
    
    public static final String HTTP_TEST_LOGIN_URL = "/Login.do";
    public static final String HTTP_TEST_USERINFO_URL = "/Userinfo.do";
    public static final String HTTP_TEST_DOWNLOAD_URL = "/Download.do";
    public static final String HTTP_TEST_UPLOAD_URL = "/execute_upload.do";
    
    public static final String HTTP_TEST_WEBVIEW_URL = "http://" + HTTP_TEST_SERVER + ":"
            + HTTP_TEST_PORT;
   
    
    /* #############################################Menu Item ####################################################################### */
    
    public static final int MENU_HTTP_HTTPCLIENT = 1;
    public static final int MENU_HTTP_URLCONNECTION = 2;

    public static final int MENU_HTTP_WEBVIEW = 3;
    public static final int MENU_SPEED_COMPARE = 4;
    
    public static final int MENU_ENCRYPT_FILE = 5;
    public static final int MENU_ENCRYPT_DATA = 6;
    public static final int MENU_ENCRYPT_SQLITE = 7;
    public static final int MENU_ENCRYPT_CLIPBOARD = 8;
    public static final int MENU_MDM_CHECK = 9;

    
    /* #############################################Encrypt /Decrypt ##################################################### */
    
    public static final String FOLDER_ROOT = "SvnSdkDemo";
    public static final String FOLDER_PATH_DOWNLOAD = FileUtil.getSDPath()
            + "/" + FOLDER_ROOT + "/Download";
    public static final String FILE_PATH_ORIGINAL = FileUtil.getSDPath() + "/"
            + FOLDER_ROOT + "/Original";
    public static final String FILE_PATH_ENCRYPT = FileUtil.getSDPath() + "/"
            + FOLDER_ROOT + "/Encrypt";
    
    
    public static final String FILE_BROWSE_RESULT = "FileBrowseResult";
    
    
    
    
    /* #############################################Login or Logout or Request Status ##################################################### */
    public static final int LOGIN_STATUS_SUCCESS = 0;
    public static final int LOGIN_STATUS_FALSE = 1;
    
    public static final int LOGOUT_STATUS_SUCCESS = 0;
    public static final int LOGOUT_STATUS_FALSE = 1;
    
    public static final int STATUS_REQUEST_SUCCESS = 0;
    public static final int STATUS_REQUEST_FALSE = 1;

    /* #############################################Guide category ######################################################################## */
    public static final String GUIDE_CATEGORY = "GuideCategory";
    public static final String GUIDE_CATEGORY_URLCONNECTION = "UrlConnection";
    public static final String GUIDE_CATEGORY_HTTPCLIENT = "HttpClient";
    public static final String GUIDE_CATEGORY_FILE_ENDECRYPT = "FileEnDecrypt";
    public static final String GUIDE_CATEGORY_DATA_ENDECRYPT = "DataEnDecrypt";
    /* #############################################Activity Send ######################################################################### */
    public static final String ACTIVITY_SEND_HTTPTYPE = "HttpType";
    public static final String ACTIVITY_SEND_HTTPTYPE_URLCONNECTION = "UrlConnection";
    public static final String ACTIVITY_SEND_HTTPTYPE_HTTPCLIENT = "HttpClient";
    public static final String ACTIVITY_SEND_ENDECRYPT_TYPE = "EnDecryptType";
    public static final String ACTIVITY_SEND_ENDECRYPT_TYPE_LOGIN = "Login";
    public static final String ACTIVITY_SEND_ENDECRYPT_TYPE_NO_LOGIN = "NoLogin";
    public static final String ACTIVITY_SEND_FAQ = "Faq";
    /* #############################################Click Interface Type################################################################### */
    public static final int CLICK_IFACE_TYPE_FILE_SYSTEM_FILE = 1001;

    /* #############################################Assets Folder or File ################################################################# */
    public static final String ASSETS_FAQ = "faq";
    public static final String ASSETS_GUIDE = "guide";
    public static final String ASSETS_GUIDE_HTTPCLIENT_FILE = "guide/guide_httpclient.txt";
    public static final String ASSETS_GUIDE_URLCONNECTION_FILE = "guide/guide_urlconnection.txt";
    public static final String ASSETS_GUIDE_FILE_ENDECRYPT_FILE = "guide/guide_file_endecrypt.txt";
    public static final String ASSETS_GUIDE_DATA_ENDECRYPT_FILE = "guide/guide_data_endecrypt.txt";
    /* ################################################################################################################## */
    public static final int LOGIN_RESULT_SUCCESS = 0;

    /* login action中产生的问题分类*/
    public static final String LOGIN_ERROR_FILE = "FileError";
    public static final String LOGIN_SUCCESS = "Success";
    public static final boolean LOGIN_AUTO = true;



    /* ###########################################FAQ Format ########################################################################## */
    public static final String FAQ_QUESTION = "===Q===";
    public static final String FAQ_ANSWER = "===A===";
    /* ################################################################################################################## */
}
