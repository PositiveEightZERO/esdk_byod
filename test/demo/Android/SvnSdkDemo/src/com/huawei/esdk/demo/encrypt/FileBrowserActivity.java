/**
 * 
 */
package com.huawei.esdk.demo.encrypt;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.TextView;

import com.huawei.esdk.demo.R;
import com.huawei.esdk.demo.common.BaseActivity;
import com.huawei.esdk.demo.common.Constants;

/**
 * @author cWX223941
 *
 */
public class FileBrowserActivity extends BaseActivity
{
    private ListView fileSystem;
    private TextView btnBack;
    BrowserAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_filebrowser);
        init();
    }

    private void init()
    {
        initView();
    }

    private void initView()
    {
        fileSystem = (ListView) findViewById(R.id.lv_file_browser);
        btnBack = (TextView) findViewById(R.id.btn_filebrowser_back);
        btnBack.setOnClickListener(onClickListener);
        adapter = new BrowserAdapter();
        fileSystem.setAdapter(adapter);
        //fileSystem.setOnClickListener(onClickListener);
        //fileSystem.setFileItemClickListener(fileItemClickListener);
        fileSystem.setOnItemClickListener(new OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                    int position, long id)
            {
                FileSystemEntity file = (FileSystemEntity) adapter
                        .getItem(position);
                if (file.isDirectory())
                {
                    if (file.isExpanded())
                    {
                        file.setExpanded(false);
                        adapter.collapseBranchAtIndex(position);
                        adapter.notifyDataSetChanged();
                    }
                    else
                    {
                        file.setExpanded(true);
                        adapter.expandBranchAtIndex(position);
                        adapter.notifyDataSetChanged();
                    }
                    //[self.tableView reloadData]; //do not update datasource or tableview here     
                    //rows will be inserted/deleted using datasourceManager delegate methods
                }
                else
                {
                    Intent intent = new Intent();
                    intent.putExtra(Constants.FILE_BROWSE_RESULT, file);
                    FileBrowserActivity.this.setResult(RESULT_OK, intent);
                    FileBrowserActivity.this.finish();
                }
            }
        });
    }
    private OnFileItemClickListener fileItemClickListener = new OnFileItemClickListener()
    {
        @Override
        public void onFileItemClicked(View itemView, FileSystemEntity entity)
        {
            Intent intent = new Intent();
            intent.putExtra(Constants.FILE_BROWSE_RESULT, entity);
            FileBrowserActivity.this.setResult(RESULT_OK, intent);
            FileBrowserActivity.this.finish();
        }
    };
    private OnClickListener onClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            if (v.getId() == btnBack.getId())
            {
                FileBrowserActivity.this.finish();
            }
            //            else if(v.getId() == fileSystem.getId())
            //            {
            //                Intent intent = new Intent();
            ////                intent.putExtra(Constants.ACTIVITY_RESULT_KEY_OBJECT,
            ////                        (RequestInfo) object);
            //                FileBrowserActivity.this.setResult(RESULT_OK, intent);
            //                FileBrowserActivity.this.finish();
            //            }
        }
    };

    @Override
    protected void onResume()
    {
        super.onResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }
    //    @Override
    //    public void doClickAction(Object object, View view, int type)
    //    {
    //        if (Constants.CLICK_IFACE_TYPE_FILE_SYSTEM_FILE == type)
    //        {
    //            
    //        }
    //    }
}
