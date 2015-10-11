/*
 * 
 */
package com.huawei.svn.sdk.fsm;

import java.io.File;
import java.io.FileFilter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;

import com.huawei.shield.ProxyConstruct;
import com.huawei.shield.WedgeClass;
import com.huawei.svn.sdk.SvnConstants;

/**
 * SvnFile 文件加解密
 * 
 * 继承自File类，提供文件加解密读写功能
 * 
 * @author l00174413
 * @version 1.0
 * @see File SvnFileApi
 * @since 1.0
 */
@WedgeClass(value="Ljava/io/File;")
@ProxyConstruct(value="Ljava/io/File;")
public class SvnFile extends File
{
    // private static final Logger log =
    // Logger.getLogger(SvnFile.class.getSimpleName());
    /**
     * 唯一标识
     */
    private static final long serialVersionUID = -8028995053368972869L;
    /**
     * 句柄
     */
    private int fileDesc = -1;


    // private File file;
    // private transient int prefixLength;
    // int getPrefixLength()
    // {
    // return prefixLength;
    //
    // }

    /**
     * 获取密文路径
     * 
     * @return 密文路径
     */
    public String getEncpath()
    {
        if(!exists())
        {
            return SvnFileTool.encPathname(this.getPath());
        }
        
        if (isDirectory())
        {
            return getPath();
        }
        else
        {
            if(SvnFileTool.isEncFile(this.getPath()))
            {
                return SvnFileTool.encPathname(this.getPath());
            }
            else
            {
                return getPath();
            }

        }
    }

    /**
     * 根据路径来创建文件对象
     * 
     * @param pathname
     *            明文路径
     */
    public SvnFile(String pathname)
    {
        super(pathname);
        // this.encpath = SvnFileTool.encPathname(this.getPath());
        // System.out.println("origPath:" + origPath + ",encpath:" + encpath);
    }

    /**
     * 在指定目录名称中创建指定文件名的文件
     * 
     * @param parent
     *            父目录名
     * @param child
     *            子文件名
     */
    public SvnFile(String parent, String child)
    {
        super(parent, child);
        // this.encpath = SvnFileTool.encPathname(this.getPath());
    }

    /**
     * 在指定目录中创建指定文件名的文件
     * 
     * @param parent
     *            父目录
     * @param child
     *            子文件名
     */
    public SvnFile(SvnFile parent, String child)
    {
        super(parent, child);
        // this.encpath = SvnFileTool.encPathname(this.getPath());
    }
    
    
    public SvnFile(File parent, String child)
    {
        super(parent, child);
        // this.encpath = SvnFileTool.encPathname(this.getPath());
    }

    /**
     * 根据URI来创建文件
     * 
     * @param uri
     *            文件URI
     */
    public SvnFile(URI uri)
    {
        super(uri);
        // this.encpath = SvnFileTool.encPathname(this.getPath());
    }

    /**
     * 通过指定目录名和前缀长度，创建文件
     * 
     * @param pathname
     *            路径名
     * @param prefixLength
     *            前缀长度
     */
    private SvnFile(String pathname, int prefixLength)
    {
        super(pathname);
        // this.encpath = SvnFileTool.encPathname(this.getPath());
        // this.path = pathname;
        // this.prefixLength = prefixLength;
    }

    /**
     * 获取父目录
     * 
     * @return 父目录
     */
    public SvnFile getParentFile()
    {
        String p = this.getParent();
        // log.info("the parent path is : " + p);
        if (p == null)
        {
            return null;
        }
        return new SvnFile(p);
        // return new SvnFile(p, this.prefixLength);
    }

    /**
     * 文件是否是绝对的
     * 
     * @return 路径是否是绝对
     */
    // public boolean isAbsolute()
    // {
    // File file = new File(this.getPath());
    // if (file.isDirectory())
    // {
    // return file.isAbsolute();
    // }
    // else
    // {
    // String encpath = SvnFileTool.encPathname(this.getPath());
    // file = new File(encpath);
    // return file.isAbsolute();
    // }
    // }

    /**
     * 文件是否可读
     * 
     * @return 是否可读
     */
    public boolean canRead()
    {
        return SvnFileTool.access(this.getPath(), SvnConstants.OPER_READ);
    }

    /**
     * 文件是否可写
     * 
     * @return 是否可写
     */
    public boolean canWrite()
    {
        return SvnFileTool.access(this.getPath(), SvnConstants.OPER_WRITE);
    }

    /**
     * 文件是否可执行
     * 
     * @return 是否可执行
     */
    public boolean canExecute()
    {
        return SvnFileTool.access(this.getPath(), SvnConstants.OPER_EXEC);
    }

    /**
     * 文件是否存在
     * 
     * @return 是否存在
     */
    @Override
    public boolean exists()
    {
        return SvnFileTool.access(this.getPath(), SvnConstants.F_OK);
    }

    /**
     * 文件是否是目录
     * 
     * @return 是否是目录
     */
    public boolean isDirectory()
    {
        // String encpath = SvnFileTool.encPathname(this.getPath());
        File file = new File(this.getPath());
        // System.out.println("isDirectory:" + this.encpath);
        return file.isDirectory();
    }

    /**
     * 是否是文件
     * 
     * @return 是否是文件
     */
    public boolean isFile()
    {
        if (!exists() || isDirectory())
        {
            return false;
        }

        return true;
    }

    /**
     * 是否隐藏
     * 
     * @return 是否隐藏
     */
    public boolean isHidden()
    {
        // this.encpath = SvnFileTool.encPathname(this.getPath());
        if (!exists())
        {
            return false;
        }

        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.isHidden();
        }

        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.isHidden();
            }
            else
            {
                return file.isHidden();
            }
        }
    }

    /**
     * 上次修改时间
     * 
     * @return 修改时间
     */
    @Override
    public long lastModified()
    {
        long modTime = 0;
        if (this.exists() && this.canRead())
        {
            modTime = SvnFileTool.getLastModiTime(this.getPath());
        }
        return modTime * 1000;
    }

    /**
     * 文件长度
     * 
     * @return 文件长度
     */
    @Override
    public long length()
    {
        long length = 0;
        if (this.exists() && this.canRead())
        {
            length = SvnFileTool.getFileLength(this.getPath());
        }
        return length;
    }

    /**
     * 创建新文件
     * 
     * @return 是否创建成功
     * @throws IOException
     *             创建过程中出现IO异常
     */
    @Override
    public boolean createNewFile() throws IOException
    {
        boolean ret = false;
        fileDesc = SvnFileTool.openFile(this.getPath(),
                SvnConstants.OPER_WRITE_STR);
        // log.info("the result of openFile: " + fileDesc);
        if (fileDesc != 0)
        {
            ret = SvnFileTool.closeFile(fileDesc);
        }
        return ret;
    }

    /**
     * 指定目录下创建临时文件
     * 
     * @param prefix
     *            前缀名
     * @param suffix
     *            后缀名
     * @param directory
     *            指定目录
     * @return 临时文件
     * @throws IOException
     *             创建过程中出现IO异常
     */
    public static SvnFile createTempFile(String prefix, String suffix,
            SvnFile directory) throws IOException
    {
        boolean ret = false;
        File origDir = null;
        boolean need2Del = false;
        if (directory != null)
        {
            origDir = new File(((SvnFile) directory).getPath());
            if (!origDir.exists())
            {
                ret = origDir.mkdir();
            }
            if (ret)
            {
                need2Del = true;
            }
        }
        File fs = File.createTempFile(prefix, suffix, directory);
        SvnFile sf = new SvnFile(fs.getPath());
        ret = fs.delete();
        if (need2Del)
        {
            ret = origDir.delete();
        }
        ret = sf.createNewFile();
        if (ret)
        {
            return sf;
        }
        return null;
    }
    
    
    /**
     * 指定目录下创建临时文件
     * 
     * @param prefix
     *            前缀名
     * @param suffix
     *            后缀名
     * @param directory
     *            指定目录
     * @return 临时文件
     * @throws IOException
     *             创建过程中出现IO异常
     */
    public static SvnFile createTempFile(String prefix, String suffix,
            File directory) throws IOException
    {
        boolean ret = false;
        File origDir = null;
        boolean need2Del = false;
        if (directory != null)
        {
            origDir = new File(directory.getPath());
            if (!origDir.exists())
            {
                ret = origDir.mkdir();
            }
            if (ret)
            {
                need2Del = true;
            }
        }
        File fs = File.createTempFile(prefix, suffix, directory);
        SvnFile sf = new SvnFile(fs.getPath());
        ret = fs.delete();
        if (need2Del)
        {
            ret = origDir.delete();
        }
        ret = sf.createNewFile();
        if (ret)
        {
            return sf;
        }
        return null;
    }

    /**
     * 指定默认临时目录下创建临时文件
     * 
     * @param prefix
     *            前缀名
     * @param suffix
     *            后缀名
     * @return 临时文件
     * @throws IOException
     *             创建过程中出现IO异常
     */
    public static File createTempFile(String prefix, String suffix)
            throws IOException
    {
        return createTempFile(prefix, suffix, null);
    }

    /**
     * 删除
     * 
     * @return 是否删除成功
     */
    @Override
    public boolean delete()
    {
        boolean ret = false;
        if (this.exists())
        {
            ret = SvnFileTool.remove(this.getPath());
        }
        return ret;
    }

    /**
     * 获取指定文件下的所有文件名
     * 
     * @return 目录下所有文件名
     */
    @Override
    public String[] list()
    {
        // check whether file exists
        // if (!this.exists())
        // {
        // return new String[] {};
        // // return null;
        // }
        ArrayList<String> dirNames = SvnFileTool.list(this.getPath());
        if (dirNames == null || dirNames.size() == 0)
        {
            return new String[] {};
        }
        return dirNames.toArray(new String[dirNames.size()]);
    }

    /**
     * 获取指定文件下所有文件
     * 
     * @return 目录下所有文件
     */
    public SvnFile[] listFiles()
    {
        String[] ss = list();
        if (ss == null)
        {
            return null;
        }
        int n = ss.length;
        SvnFile[] fs = new SvnFile[n];
        for (int i = 0; i < n; i++)
        {
            fs[i] = new SvnFile(this.getPath() + separator + ss[i]);
        }
        return fs;
    }

    /**
     * 根据文件名条件，获取指定文件下所有符合条件的文件
     * 
     * @param filter
     *            文件名条件
     * @return 符合条件的文件
     */
    public SvnFile[] listFiles(FilenameFilter filter)
    {
        String[] ss = list();
        if (ss == null)
        {
            return null;
        }
        ArrayList<SvnFile> files = new ArrayList<SvnFile>();
        for (String s : ss)
        {
            if ((filter == null) || filter.accept(this, s))
            {
                files.add(new SvnFile(this.getPath() + separator + s));
            }
        }
        return files.toArray(new SvnFile[files.size()]);
    }

    /**
     * 根据文件条件，获取指定文件下所有符合条件的文件
     * 
     * @param filter
     *            文件条件
     * @return 符合条件的文件
     */
    public SvnFile[] listFiles(FileFilter filter)
    {
        String[] ss = list();
        if (ss == null)
        {
            return null;
        }
        ArrayList<SvnFile> files = new ArrayList<SvnFile>();
        for (String s : ss)
        {
            SvnFile f = new SvnFile(this.getPath() + separator + s);
            if ((filter == null) || filter.accept(f))
            {
                files.add(f);
            }
        }
        return files.toArray(new SvnFile[files.size()]);
    }

    /**
     * 创建目录
     * 
     * @return 是否成功
     */
    @Override
    public boolean mkdir()
    {
        if (!this.exists())
        {
            return SvnFileTool.createDir(this.getPath());
        }
        return false;
    }

    /**
     * 获取范式文件
     * 
     * @return 文件
     * @throws IOException
     *             过程中出现IO异常
     */
    public SvnFile getCanonicalFile() throws IOException
    {
        String canonPath = getCanonicalPath();
        // return new SvnFile(canonPath, fs.prefixLength(canonPath));
        return new SvnFile(canonPath);
    }

    /**
     * 获取绝对文件
     * 
     * @return the absolute file
     */
    public SvnFile getAbsoluteFile()
    {
        String absPath = getAbsolutePath();
        // return new SvnFile(canonPath, fs.prefixLength(canonPath));
        return new SvnFile(absPath);
    }

    /**
     * 逐级创建目录
     * 
     * @return 是否成功
     */
    public boolean mkdirs()
    {
        if (exists())
        {
            return false;
        }
        if (mkdir())
        {
            return true;
        }
        SvnFile canonFile = null;
        try
        {
            canonFile = getCanonicalFile();
        }
        catch (IOException e)
        {
            return false;
        }
        SvnFile parent = canonFile.getParentFile();
        return (parent != null && (parent.mkdirs() || parent.exists()) && canonFile
                .mkdir());
    }

    /**
     * 重命名
     * 
     * @param dest
     *            新文件
     * @return 是否成功
     */
    public boolean renameTo(SvnFile dest)
    {
        boolean ret = false;
        if (dest == null)
        {
            return ret;
        }
        // check original file whether exists
        if (!this.exists())
        {
            return ret;
        }
        // SvnFile svnDest = (SvnFile) dest;
        // orignal is writable
        if (this.canWrite())
        {
            if (SvnConstants.F_OK == SvnFileTool.renameDir(this.getPath(),
                    dest.getPath()))
            {
                ret = true;
                // this.encpath = svnDest.getEncpath();
            }
        }
        return ret;
    }
    
    /**
     * 重命名
     * 
     * @param dest
     *            新文件
     * @return 是否成功
     */
    public boolean renameTo(File dest)
    {
        boolean ret = false;
        if (dest == null)
        {
            return ret;
        }
        // check original file whether exists
        if (!this.exists())
        {
            return ret;
        }
        // SvnFile svnDest = (SvnFile) dest;
        // orignal is writable
        if (this.canWrite())
        {
            if (SvnConstants.F_OK == SvnFileTool.renameDir(this.getPath(),
                    dest.getPath()))
            {
                ret = true;
                // this.encpath = svnDest.getEncpath();
            }
        }
        return ret;
    }

    /**
     * 设置上次修改时间
     * 
     * @param time
     *            修改时间
     * @return 是否成功
     */
    public boolean setLastModified(long time)
    {
        if (!this.exists())
        {
            return false;
        }

        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.setLastModified(time);
        }
        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.setLastModified(time);
            }
            else
            {
                return file.setLastModified(time);
            }
        }
    }

    /**
     * 设置只读.
     * 
     * @return 是否成功
     */
    public boolean setReadOnly()
    {
        if (!this.exists())
        {
            return false;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.setReadOnly();
        }
        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.setReadOnly();
            }
            else
            {
                return file.setReadOnly();
            }
        }
    }

    /**
     * 设置是否可写，且是否只应用于文件所有者
     * 
     * @param writable
     *            是否可写
     * @param ownerOnly
     *            是否只读
     * @return 是否成功
     */
    public boolean setWritable(boolean writable, boolean ownerOnly)
    {
        if (!this.exists())
        {
            return false;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.setWritable(writable, ownerOnly);
        }
        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.setWritable(writable, ownerOnly);
            }
            else
            {
                return file.setWritable(writable, ownerOnly);
            }
        }
    }

    /**
     * 设置是否可写
     * 
     * @param writable
     *            是否可写
     * @return 是否成功
     */
    public boolean setWritable(boolean writable)
    {
        return setWritable(writable, true);
    }

    /**
     * 设置是否可读，且是否只应用于文件所有者
     * 
     * @param readable
     *            是否可读
     * @param ownerOnly
     *            是否仅所有者可读
     * @return 是否成功
     */
    public boolean setReadable(boolean readable, boolean ownerOnly)
    {
        if (!this.exists())
        {
            return false;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.setReadable(readable, ownerOnly);
        }
        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.setReadable(readable, ownerOnly);
            }
            else
            {
                return file.setReadable(readable, ownerOnly);
            }
        }
    }

    /**
     * 设置是否可读
     * 
     * @param readable
     *            是否可读
     * @return 是否成功
     */
    public boolean setReadable(boolean readable)
    {
        return setReadable(readable, true);
    }

    /**
     * 设置是否可执行，且是否只应用于文件所有者
     * 
     * @param executable
     *            是否可执行
     * @param ownerOnly
     *            是否仅所有者可执行
     * @return 是否成功
     */
    public boolean setExecutable(boolean executable, boolean ownerOnly)
    {
        if (!this.exists())
        {
            return false;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.setExecutable(executable, ownerOnly);
        }
        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.setExecutable(executable, ownerOnly);
            }
            else
            {
                return file.setExecutable(executable, ownerOnly);
            }
        }
    }

    /**
     * 设置是否可执行
     * 
     * @param executable
     *            是否可执行
     * @return 是否成功
     */
    public boolean setExecutable(boolean executable)
    {
        return setExecutable(executable, true);
    }

    /**
     * 获取总空间
     * 
     * @return 总空间
     */
    public long getTotalSpace()
    {
        if (!this.exists())
        {
            return 0;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.getTotalSpace();
        }
        else
        {
            
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.getTotalSpace();
            }
            else
            {
                return file.getTotalSpace();
            }
        }
    }

    /**
     * 获取空闲空间
     * 
     * @return 空闲空间
     */
    public long getFreeSpace()
    {
        if (!this.exists())
        {
            return 0;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.getFreeSpace();
        }
        else
        {
            
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.getFreeSpace();
            }
            else
            {
                return file.getFreeSpace();
            }
        }
    }

    /**
     * 获取可用空间
     * 
     * @return 可用空间
     */
    public long getUsableSpace()
    {
        if (!this.exists())
        {
            return 0;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            return file.getUsableSpace();
        }
        else
        {
            
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                return file.getUsableSpace();
            }
            else
            {
                return file.getUsableSpace();
            }
        }

    }

    /**
     * 文件比对
     * 
     * @param pathname
     *            比较对象
     * @return 比较结果
     */
    public int compareTo(File pathname)
    {
        if (this == pathname)
        {
            return 0;
        }
        if (pathname == null)
        {
            return 1;
        }
        if (pathname instanceof SvnFile)
        {
            return this.getEncpath().compareTo(
                    ((SvnFile) pathname).getEncpath());
        }
        else
        {
            return ((File) this).compareTo(pathname);
        }
        // if (pathname == null || !(pathname instanceof SvnFile))
        // {
        // return SvnConstants.SVN_FILE_BIG;
        // }
        // SvnFile tmpComFile = (SvnFile) pathname;
        // String encpath = SvnFileTool.encPathname(this.getPath());
        // String encpath = this.encpath;
        // String encCompath =
        // SvnFileTool.encPathname(tmpComFile.getOrigPath());
        // return encpath.compareToIgnoreCase(encCompath);
        // File orgFile = new File(encpath);
        // File comFile = new File(encCompath);
        // return orgFile.compareTo(comFile);
    }

    /**
     * 文件相等比较
     * 
     * @param obj
     *            相当比较对象
     * @return 是否相等
     */
    @Override
    public boolean equals(Object obj)
    {
        // if(this == obj)
        // {
        // return true;
        // }
        //
        // if(obj == null)
        // {
        // return false;
        // }
        return super.equals(obj);
        // if (obj instanceof SvnFile)
        // {
        // return compareTo((SvnFile) obj) == 0;
        // }
        // else if(obj instanceof File)
        // {
        // return getPath().equals(((File) obj).getPath());
        // }
        // return false;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.io.File#hashCode()
     */
    public int hashCode()
    {
        // String encpath = SvnFileTool.encPathname(this.getPath());
        // File file = new File(encpath);
        // File file = new File(this.encpath);
        return super.hashCode();
        // return this.getEncpath().toLowerCase(Locale.ENGLISH).hashCode() ^
        // 1234321;
    }

    // private synchronized void writeObject(java.io.ObjectOutputStream s)
    // throws IOException
    // {
    // s.defaultWriteObject();
    // s.writeChar(separatorChar); // Add the separator character
    // }
    /**
     * readObject is called to restore this filename. The original separator
     * character is read. If it is different than the separator character on
     * this system, then the old separator is replaced by the local separator.
     */
    // private synchronized void readObject(java.io.ObjectInputStream s) throws
    // IOException,
    // ClassNotFoundException
    // {
    // ObjectInputStream.GetField fields = s.readFields();
    // String pathField = (String) fields.get("path", null);
    // char sep = s.readChar(); // read the previous separator char
    // if (sep != separatorChar)
    // pathField = pathField.replace(sep, separatorChar);
    // this.path = fs.normalize(pathField);
    // this.prefixLength = fs.prefixLength(this.path);
    // }
    /**
     * 退出时删除
     */
    @Override
    public void deleteOnExit()
    {
        if(!exists())
        {
            return;
        }
        File file = new File(this.getPath());
        if (file.isDirectory())
        {
            file.deleteOnExit();
        }
        else
        {
            if (SvnFileTool.isEncFile(getPath()))
            {
                file = new File(this.getEncpath());
                file.deleteOnExit();
            }
            else
            {
                file.deleteOnExit();
            }
        }

    }
}
