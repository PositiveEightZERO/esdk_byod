package com.huawei.svn.sdk.fsm;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.concurrent.atomic.AtomicInteger;

import android.util.Log;

import com.huawei.shield.ProxyConstruct;
import com.huawei.shield.WedgeClass;
import com.huawei.svn.sdk.SvnConstants;


/**
 * SvnFileInputStream类 
 * 
 * 
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
@WedgeClass(value="Ljava/io/FileInputStream;")
@ProxyConstruct(value="Ljava/io/FileInputStream;")
public class SvnFileInputStream extends FileInputStream
{

    // private static final Logger log =
    // Logger.getLogger(SvnFileInputStream.class.getSimpleName());

    // private static void setLog()
    // {
    // LogUtil.setLog(log);
    // }
    /**
     * 是否正在销毁自己
     */
    private static final ThreadLocal<Boolean> runningFinalize = new ThreadLocal<Boolean>();
    
    /** 
     * close锁对象
     */
    private final Object closeLock = new Object();

    /**
     *  文件句柄
     */
    private int fileDesc = -1;

    /* File Descriptor - handle to the open file */
    // private FileDescriptor fd;

    /** 
     * 使用数
     */
    private AtomicInteger useCount = new AtomicInteger();

//    /** 
//     * FileChannel对象 
//     */
//    private FileChannel channel = null;

    /** 
     * 是否关闭 
     */
    private volatile boolean closed = false;


    /**
     * 获取是否正在销毁
     * 
     * @return 是否正在销毁
     */
    private static boolean isRunningFinalize()
    {
        Boolean val = runningFinalize.get();
        if (val != null)
        {
            return val.booleanValue();
        }
        return false;
    }

    /**
     * 根据文件路径名，创建输入流
     * 
     * @param name
     *            文件路径名
     * @throws FileNotFoundException
     *            文件不存在异常
     */
    public SvnFileInputStream(String name) throws FileNotFoundException
    {
        this(name != null ? new SvnFile(name) : null);

    }
    
    
    public SvnFileInputStream(File file) throws FileNotFoundException
    {
        this(file != null ? new SvnFile(file.getPath()) : null);
    }
    /**
     * 根据文件，创建输入流
     * 
     * @param file
     *            文件
     * @throws FileNotFoundException
     *             the file not found exception
     */
    public SvnFileInputStream(SvnFile file) throws FileNotFoundException
    {
        super(file != null ? file.getEncpath() : null);
        // this.fd = super.getFD();
        // if (file != null && !file.getOrigPath().isEmpty() && file.exists())
        // {
        // fileDesc = SvnFileTool.openFile(file.getOrigPath(),
        // SvnFileTool.OPER_READ_STR);
        // log.info("the result of openFile in SvnFileInputStream is fileDesc : "
        // + fileDesc);
        // }
        // super(SvnFileTool.encPathname(file.getPath()));
        try
        {
            super.close();
        }
        catch (IOException e)
        {
            //e.printStackTrace();
            Log.i("SDK", "SvnFileInputStream super close exception");
        }
        if (file != null)
        {
            fileDesc = SvnFileTool.openFile(file.getPath(),
                    SvnConstants.OPER_READ_STR);
            if (fileDesc != 0)
            {
                useCount.getAndIncrement();
            }
            
            closed = false;
            // log.info("the result of openFile in SvnFileInputStream is fileDesc : "
            // + fileDesc);
        }
    }

    /**
     * 关闭输入流
     * 
     * @throws IOException
     *             I/O异常
     */
    public void close() throws IOException
    {
        synchronized (closeLock)
        {
            if (closed)
            {
                return;
            }
            closed = true;
        }
//        if (channel != null)
//        {
//            channel.close();
//        }

        if (!isRunningFinalize() && useCount.decrementAndGet() == 0)
        {
            if (fileDesc != 0)
            {
                SvnFileTool.closeFile(fileDesc);
            }
        }
    }

    /**
     * 读取一个字节
     * 
     * @return 所读字节
     * @throws IOException
     *             I/O异常
     */
    public int read() throws IOException
    {
        int ret = SvnConstants.FILE_EOF;
        byte[] buf = new byte[1];
        ret = this.read(buf, 0, 1);
        if (ret != SvnConstants.FILE_EOF)
        {
            return buf[0];
        }
        return ret;
        // return SvnFileTool.readFile(buf,0, fileDesc);
    }

    /**
     * 读取一定长度字节到指定的缓冲区
     * 
     * @param buf
     *            缓冲区
     * @return 所读字节长度
     * @throws IOException
     *              I/O异常
     */
    public int read(byte[] buf) throws IOException
    {
        int readLen = SvnConstants.FILE_EOF;
        if (buf != null && buf.length > 0)
        {
            readLen = this.read(buf, 0, buf.length);
        }
        return readLen;
    }

    /**
     * 在指定偏移量后，读取一定长度字节到指定的缓冲区
     * 
     * @param b
     *            缓冲区
     * @param offset
     *            偏移量
     * @param len
     *            长度
     * @return 实际读取字节数
     * @throws IOException
     *             I/O异常
     */
    public int read(byte[] b, int offset, int len) throws IOException
    {
        int readLen = SvnFileTool.readFile(b, offset, len, fileDesc);
        return readLen;
    }

    /**
     * 跳过指定数量字节
     * 
     * @param n
     *            指定数量
     * @return the long
     * @throws IOException
     *              I/O异常
     */
    public long skip(long n) throws IOException
    {
        long readLen = 0;
        if (n > 0)
        {
            readLen = SvnFileTool.seek(fileDesc, n);
        }
        return readLen;
    }

    /**
     * 获取通道
     * 
     * @return the channel
     */
    public FileChannel getChannel()
    {
        try
        {
            throw new IOException("FileChannel not supported");
        }
        catch (IOException e)
        {
            //e.printStackTrace();
            Log.i("SDK", "FileChannel not supported!");    
            // log.severe("FileChannel not supported");
        }
        return null;
    }

    /**
     * 估计输入流中可用字节数
     * 
     * @return 可用字节数
     * @throws IOException
     *              I/O异常
     */
    @Override
    public int available() throws IOException
    {
        int remant = 0;
        remant = SvnFileTool.available(fileDesc);
        return remant;
    }

    /* (non-Javadoc)
     * @see java.io.FileInputStream#finalize()
     */
    @Override
    protected void finalize() throws IOException
    {
        super.finalize();
        if (fileDesc != 0)
        {
            runningFinalize.set(Boolean.TRUE);
            try
            {
                close();
            }
            finally
            {
                runningFinalize.set(Boolean.FALSE);
            }
        }
    }
}
