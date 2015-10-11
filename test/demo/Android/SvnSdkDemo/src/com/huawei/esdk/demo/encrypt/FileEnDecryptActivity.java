/**
 * 
 */
package com.huawei.esdk.demo.encrypt;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.huawei.anyoffice.sdk.doc.SecReader;
import com.huawei.anyoffice.sdk.exception.NoRMSAppFoundException;
import com.huawei.anyoffice.sdk.exception.NoRecommendedAppException;
import com.huawei.esdk.demo.GuideActivity;
import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;
import com.huawei.esdk.demo.utils.FileUtil;
import com.huawei.esdk.demo.utils.StringUtil;
import com.huawei.esdk.demo.utils.ThemeUtil;
import com.huawei.svn.sdk.fsm.SvnFile;
import com.huawei.svn.sdk.fsm.SvnFileInputStream;
import com.huawei.svn.sdk.fsm.SvnFileOutputStream;
import com.huawei.svn.sdk.fsm.SvnFileTool;
import com.huawei.svn.sdk.media.SvnMediaPlayer;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.TextView;

/**
 * @author cWX223941
 *
 */
public class FileEnDecryptActivity extends BaseActivity
{
    private static final String TAG = "EnDecryptActivity";
    private EditText etFilePath, etOriginalPath, etEncryptPath;
    private TextView btnBrowse, btnBack, btnEncrypt, btnDecrypt, btnGuide, btnOpen;
    private TextView resultText;
    private final int FILE_SYSTEM_REQUEST_CODE = 1;
    private FileSystemEntity fileInfo;

    //    private String                                  enDecryptType;
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_endecrypt);
        init();
    }
    


    private void init()
    {
        initView();
        initData();
    }

    private void initView()
    {
        etFilePath = (EditText) findViewById(R.id.et_encrypt_decrypt_filepath);
        etOriginalPath = (EditText) findViewById(R.id.et_encrypt_decrypt_originalpath);
        etEncryptPath = (EditText) findViewById(R.id.et_encrypt_decrypt_encryptpath);
        btnBrowse = (TextView) findViewById(R.id.btn_encrypt_decrypt_browse);
        btnBack = (TextView) findViewById(R.id.btn_endecrypt_back);
        btnEncrypt = (TextView) findViewById(R.id.btn_encrypt_decrypt_encrypt);
        btnDecrypt = (TextView) findViewById(R.id.btn_encrypt_decrypt_decrypt);
        btnGuide = (TextView) findViewById(R.id.btn_endecrypt_guide);
        resultText = (TextView) findViewById(R.id.tv_encrypt_decrypt_result);
        btnOpen = (TextView) findViewById(R.id.btn_encrypt_decrypt_open);
        btnEncrypt.setEnabled(false);
        btnDecrypt.setEnabled(false);
        btnOpen.setEnabled(false);
        btnBrowse.setOnClickListener(onClickListener);
        btnBack.setOnClickListener(onClickListener);
        btnEncrypt.setOnClickListener(onClickListener);
        btnDecrypt.setOnClickListener(onClickListener);
        
        btnOpen.setOnClickListener(onClickListener);
        
        btnGuide.setOnClickListener(onClickListener);
        etFilePath.addTextChangedListener(textWatcher);
        
        
    }

    private void initData()
    {
        etOriginalPath.setText(Constants.FILE_PATH_ORIGINAL);
        etEncryptPath.setText(Constants.FILE_PATH_ENCRYPT);
        File originPath = new File(Constants.FILE_PATH_ORIGINAL);
        originPath.mkdirs();
        File encryptPath = new File(Constants.FILE_PATH_ENCRYPT);
        encryptPath.mkdirs();
    }
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnBrowse.getId())
            {
                Intent intent = new Intent(FileEnDecryptActivity.this,
                        FileBrowserActivity.class);
                FileEnDecryptActivity.this.startActivityForResult(intent,
                        FILE_SYSTEM_REQUEST_CODE);
            }
            else if (v.getId() == btnBack.getId())
            {
                FileEnDecryptActivity.this.finish();
            }
            else if (v.getId() == btnGuide.getId())
            {
                Intent intent = new Intent(FileEnDecryptActivity.this,
                        GuideActivity.class);
                intent.putExtra(Constants.GUIDE_CATEGORY,
                        Constants.GUIDE_CATEGORY_FILE_ENDECRYPT);
                FileEnDecryptActivity.this.startActivity(intent);
            }
            else if (v.getId() == btnEncrypt.getId())
            {
            
                    viewWhenClickEncrypt();
                    encrypt();
                
            }
            else if (v.getId() == btnDecrypt.getId())
            {
                
                    //1:pager prepare
                    viewWhenClickDecrypt();
                    decrypt();
                
            
            }
            else if (v.getId() == btnOpen.getId())
            {
                
                    Log.i(TAG, "open file." + fileInfo.getFullPath());
                    //open the file
                    //openOriginFile(fileInfo.getFullPath());
                    openFile(fileInfo.getFullPath());
               
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
                //set login button to be enable
                ThemeUtil
                        .setBtnToEnable(btnEncrypt, FileEnDecryptActivity.this);
                ThemeUtil
                        .setBtnToEnable(btnDecrypt, FileEnDecryptActivity.this);
                
                ThemeUtil
                .setBtnToEnable(btnOpen, FileEnDecryptActivity.this);
                
            }
            else
            {
                //set login button to be not enable
                ThemeUtil
                        .setBtnToUnable(btnEncrypt, FileEnDecryptActivity.this);
                ThemeUtil
                        .setBtnToUnable(btnDecrypt, FileEnDecryptActivity.this);
                ThemeUtil
                .setBtnToUnable(btnOpen, FileEnDecryptActivity.this);
            }
        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
        if (FILE_SYSTEM_REQUEST_CODE == requestCode)
        {
            if (RESULT_OK == resultCode)
            {
                fileInfo = (FileSystemEntity) data
                        .getSerializableExtra(Constants.FILE_BROWSE_RESULT);
                initPageByFileInfo();
            }
        }
    }

    /**
     * if all edit text have content,then return true,otherwise return false
     * @return true:all have content;false:otherwise
     * */
    private boolean checkEditContent()
    {
        if (!StringUtil.isEmpty(etFilePath.getText().toString()))
        {
            return true;
        }
        return false;
    }

    private void viewWhenClickEncrypt()
    {
        btnEncrypt.setText(R.string.ing);
        btnEncrypt.setEnabled(false);
    }

    private void viewWhenClickDecrypt()
    {
        btnDecrypt.setText(R.string.ing);
        btnDecrypt.setEnabled(false);
    }

    private void handleEncryptResult(int status)
    {
        switch (status)
        {
            case Constants.STATUS_REQUEST_SUCCESS:
                Log.e(TAG, "encrpty success.");
                btnEncrypt.setText(R.string.encrypt);
                btnEncrypt.setEnabled(true);
                resultText.setText(R.string.encrypt_success);
                break;
            case Constants.STATUS_REQUEST_FALSE:
                Log.e(TAG, "encrpty failed.");
                btnEncrypt.setText(R.string.encrypt);
                btnEncrypt.setEnabled(true);
                break;
            default:
                break;
        }
    }

    private void handleDecryptResult(int status)
    {
        switch (status)
        {
            case Constants.STATUS_REQUEST_SUCCESS:
                Log.e(TAG, "encrpty success.");
                btnDecrypt.setText(R.string.decrypt);
                btnDecrypt.setEnabled(true);
                resultText.setText(R.string.decrypt_success);
                break;
            case Constants.STATUS_REQUEST_FALSE:
                Log.e(TAG, "encrpty failed.");
                btnDecrypt.setText(R.string.decrypt);
                btnDecrypt.setEnabled(true);
                break;
            default:
                break;
        }
    }

    private void initPageByFileInfo()
    {
        if (null == fileInfo)
        {
            btnEncrypt.setEnabled(false);
            btnDecrypt.setEnabled(false);
            return;
        }
        if (fileInfo.isEncrptedFile())
        {
            //the file is encrypt
            etFilePath.setText(fileInfo.getFullPath());
            btnEncrypt.setVisibility(View.GONE);
            btnDecrypt.setText(R.string.decrypt);
            btnDecrypt.setVisibility(View.VISIBLE);
        }
        else
        {
            //the file is original
            etFilePath.setText(fileInfo.getFullPath());
            btnEncrypt.setText(R.string.encrypt);
            btnEncrypt.setVisibility(View.VISIBLE);
            btnDecrypt.setVisibility(View.GONE);
        }
        resultText.setText("");
    }

    
    //文档浏览
    private void openFile(String filePath)
    {
    	if(filePath == null)
    	{
    		return;
    	}
    	
    	int pos = filePath.indexOf('.');
    	
    	if(pos <= 0)
    	{
    		return;
    	}
    	
    	
    	
    	
    	String pathExt = filePath.substring(pos + 1);
    	
    	
    	String avFileType = "mp3|wav|wma|avi|mp4|mpg|wmv|3gp|m4a|aac";
    	
    	if(pathExt.toLowerCase().matches(avFileType))
    	{
    		SvnMediaPlayer.getInstance().play(this, filePath);
    		return;
    	}
    	
    	
        SecReader reader = new SecReader();
        
        reader.setRecommendedApp("com.kingsoft.moffice_pro_hw", SecReader.SDK_MIMETYPE_DOCUMENT);
        
        boolean ret = false;
        try
        {
            //打开文件
            ret = reader.openDocWithSDK(this, filePath, getPackageName(), null);
        }
        catch (NoRMSAppFoundException e)
        {
            e.printStackTrace();
        }
        catch (NoRecommendedAppException e)
        {
            e.printStackTrace();
        }
        Log.e(TAG, "open file path:" + filePath + " res:" + ret);
   
    }
    
    
    
    private void openOriginFile(String url) throws IOException {
        // Create URI
        File file= new File(url);
        Uri uri = Uri.fromFile(file);
        
        Intent intent = new Intent(Intent.ACTION_VIEW);
        // Check what kind of file you are trying to open, by comparing the url with extensions.
        // When the if condition is matched, plugin sets the correct intent (mime) type, 
        // so Android knew what application to use to open the file
        if (url.toString().contains(".doc") || url.toString().contains(".docx")) {
            // Word document
            intent.setDataAndType(uri, "application/msword");
        } else if(url.toString().contains(".pdf")) {
            // PDF file
            intent.setDataAndType(uri, "application/pdf");
        } else if(url.toString().contains(".ppt") || url.toString().contains(".pptx")) {
            // Powerpoint file
            intent.setDataAndType(uri, "application/vnd.ms-powerpoint");
        } else if(url.toString().contains(".xls") || url.toString().contains(".xlsx")) {
            // Excel file
            intent.setDataAndType(uri, "application/vnd.ms-excel");
        } else if(url.toString().contains(".zip") || url.toString().contains(".rar")) {
            // WAV audio file
            intent.setDataAndType(uri, "application/x-wav");
        } else if(url.toString().contains(".rtf")) {
            // RTF file
            intent.setDataAndType(uri, "application/rtf");
        } else if(url.toString().contains(".wav") || url.toString().contains(".mp3")) {
            // WAV audio file
            intent.setDataAndType(uri, "audio/x-wav");
        } else if(url.toString().contains(".gif")) {
            // GIF file
            intent.setDataAndType(uri, "image/gif");
        } else if(url.toString().contains(".jpg") || url.toString().contains(".jpeg") || url.toString().contains(".png")) {
            // JPG file
            intent.setDataAndType(uri, "image/jpeg");
        } else if(url.toString().contains(".txt")) {
            // Text file
            intent.setDataAndType(uri, "text/plain");
        } else if(url.toString().contains(".3gp") || url.toString().contains(".mpg") || url.toString().contains(".mpeg") || url.toString().contains(".mpe") || url.toString().contains(".mp4") || url.toString().contains(".avi")) {
            // Video files
            intent.setDataAndType(uri, "video/*");
        } else {
            //if you want you can also define the intent type for any other file
            
            //additionally use else clause below, to manage other unknown extensions
            //in this case, Android will show all applications installed on the device
            //so you can choose which application to use
            intent.setDataAndType(uri, "*/*");
        }
        
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
        this.startActivity(intent);
    }


    //文件加密
    private void encrypt()
    {
        int status = Constants.STATUS_REQUEST_FALSE;
        // ////////////////////////////加密////////////////////////////////////////////////
        SvnFileOutputStream fileOutStream = null;
        FileInputStream fileInStream = null;
        try
        {
            File originFile = new File(etFilePath.getText().toString().trim());
            if (originFile.exists() && originFile.isFile())
            {
                //原始文件
                fileInStream = new FileInputStream(originFile);
                
                //加密文件
                fileOutStream = new SvnFileOutputStream(
                        Constants.FILE_PATH_ENCRYPT + "/"
                                + originFile.getName());
                //加密写入，使用和OutputStream一致
                FileUtil.streamCopy(fileInStream, fileOutStream);
                Log.i(TAG, "encrypt success!");
                status = Constants.STATUS_REQUEST_SUCCESS;
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            Log.e(TAG, "encrypt error:" + e.getMessage());
        }
        handleEncryptResult(status);
    }

    //文件解密
    private void decrypt()
    {
        int status = Constants.STATUS_REQUEST_FALSE;
        // ///////////////////////解密///////////////////////////////////////////
        SvnFileInputStream fileInStream = null;
        FileOutputStream fileOutStream = null;
        try
        {
            SvnFile originFile = new SvnFile(etFilePath.getText().toString()
                    .trim());
            if (originFile.exists() && originFile.isFile()
                    && SvnFileTool.isEncFile(originFile.getPath()))
            {
                //加密文件
                fileInStream = new SvnFileInputStream(originFile);
                
                //解密文件
                fileOutStream = new FileOutputStream(new File(
                        Constants.FILE_PATH_ORIGINAL + "/"
                                + originFile.getName()));
                
                //解密读，使用和InputStream一致
                FileUtil.write(fileInStream, fileOutStream);
                Log.i(TAG, "decipher success!");
                status = Constants.STATUS_REQUEST_SUCCESS;
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
            Log.e(TAG, "decipher error:" + e.getMessage());
        }
        handleDecryptResult(status);
    }
}
