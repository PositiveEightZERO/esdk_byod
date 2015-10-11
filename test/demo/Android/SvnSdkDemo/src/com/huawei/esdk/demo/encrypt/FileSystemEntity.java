package com.huawei.esdk.demo.encrypt;

import java.io.File;
import java.io.Serializable;

import com.huawei.esdk.demo.R;
import com.huawei.svn.sdk.fsm.SvnFile;
import com.huawei.svn.sdk.fsm.SvnFileTool;

public class FileSystemEntity implements Serializable
{
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    private String name;
    private String parentPath;

    private boolean isExpanded;
    
    private int level;
    
    
    
    public String getName()
    {
        return name;
    }
    public void setName(String name)
    {
        this.name = name;
    }
    public String getParentPath()
    {
        return parentPath;
    }
    public void setParentPath(String fullPath)
    {
        this.parentPath = fullPath;
    }
    public boolean isDirectory()
    {
        String fullPath = getFullPath();
        SvnFile file = new SvnFile(fullPath);
        
        return file.isDirectory();
    }
  
    public String getFullPath()
    {
        if(parentPath == null)
        {
            return null;
        }
        
        if(parentPath.endsWith("/"))
        {
            return parentPath + name;
        }
        
        return parentPath + "/" + name;
    }
    public boolean isEncrptedFile()
    {
        String fullPath = getFullPath();
        return SvnFileTool.isEncFile(fullPath);
    }

    
    public boolean isExpanded()
    {
        return isExpanded;
    }
    public void setExpanded(boolean isExpanded)
    {
        this.isExpanded = isExpanded;
    }
    public int getLevel()
    {
        return level;
    }
    public void setLevel(int level)
    {
        this.level = level;
    }
    public int getImage()
    {
        if(this.isDirectory())
        {
            if(isExpanded())
            {
                return R.drawable.icon_folder_open;
            }
            else {
                return R.drawable.icon_folder;
            }
        }
        
      
        if (!this.isEncrptedFile())
        {
            return R.drawable.icon_file_unlock;
        }
        else
        {
            return R.drawable.icon_file_lock;
        }
    }
    
    
    public int getType()
    {
        if(isDirectory())
        {
            return 0;
        }
        else if(isEncrptedFile())
        {
            return 1;
        }
    
        else
        {
            return 2;
        }
    }
}
