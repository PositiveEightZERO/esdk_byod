package com.huawei.svn.sdk.fsm;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.concurrent.atomic.AtomicInteger;

import android.util.Log;

import com.huawei.shield.ProxyConstruct;
import com.huawei.shield.WedgeClass;
import com.huawei.svn.sdk.SvnConstants;


/**
 * SvnFileOutputStream
 *  〈功能详细描述〉
 * 
 * @author [作者]（必须）
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
@WedgeClass(value="Ljava/io/FileOutputStream;")
@ProxyConstruct(value="Ljava/io/FileOutputStream;")
public class SvnFileOutputStream extends FileOutputStream
{
    // private static final Logger logger = Logger
    // .getLogger(SvnFileOutputStream.class.getSimpleName());

    // private static void setLog()
    // {
    // LogUtil.setLog(logger);
    // }
    /** 
     * 是否正在销毁自己 
     */
    private static final ThreadLocal<Boolean> runningFinalize = new ThreadLocal<Boolean>();
    
    /** 
     * close锁 
     */
    private final Object closeLock = new Object();

    /** 
     * 文件句柄
     */
    private int fileDesc = -1;

    // private final FileDescriptor fd;

    /** 
     * 是否追加
     */
    private boolean append = false;

    /** 
     * 文件通道
     */
    //private FileChannel channel = null;

    /** 
     * 使用数
     */
    private AtomicInteger useCount = new AtomicInteger();

    /** 
     * 是否关闭
     */
    private volatile boolean closed = false;


    /**
     * 获取是否正在销毁自己
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
     * 根据指定路径名称，获取输出流
     * 
     * @param name
     *            文件名称
     * @throws FileNotFoundException
     *             文件未找到异常
     */
    public SvnFileOutputStream(String name) throws FileNotFoundException
    {

        this(name != null ? new SvnFile(name) : null, false);

    }

    /**
     * 根据指定路径名称和写入模式，获取输出流
     * 
     * @param name
     *            文件名称
     * @param append
     *            写入模式
     * @throws FileNotFoundException
     *             文件未找到异常
     */
    public SvnFileOutputStream(String name, boolean append)
            throws FileNotFoundException,
            NullPointerException
    {
        this(name != null ? new SvnFile(name) : null, append);
    }
    
    
    public SvnFileOutputStream(File file) throws FileNotFoundException,
    NullPointerException
    {
        this(file != null ? new SvnFile(file.getPath()) : null);
       
    }
    
    
    /**
     * 根据指定文件和写入模式，获取输出流
     * 
     * @param file
     *            文件
     * @param append
     *            写入模式
     * @throws FileNotFoundException
     *             文件未找到异常
     */
    public SvnFileOutputStream(File file, boolean append)
            throws FileNotFoundException
    {
        this(file != null ? new SvnFile(file.getPath()) : null, append);
    }

    /**
     * 根据指定文件，获取输出流
     * 
     * @param file
     *            文件
     * @throws FileNotFoundException
     *             文件未找到异常
     * @throws NullPointerException
     *             空指针异常
     */
    public SvnFileOutputStream(SvnFile file) throws FileNotFoundException,
            NullPointerException
    {
        this(file, false);
    }

    /**
     * 根据指定文件和写入模式，获取输出流
     * 
     * @param file
     *            文件
     * @param append
     *            写入模式
     * @throws FileNotFoundException
     *             文件未找到异常
     */
    public SvnFileOutputStream(SvnFile file, boolean append)
            throws FileNotFoundException
    {
        super(
                file != null ? file.getEncpath()
                        : null, append);
        this.append = append;
        try
        {
            super.close();
        }
        catch (IOException e)
        {
            Log.i("SDK", "SvnFileOutputStream super close exception");
            //e.printStackTrace();
        }
        if (file != null)
        {
            fileDesc = open(file.getPath(), this.append);
            if (fileDesc != 0)
            {
                useCount.getAndIncrement();
            }
        }
    }

    /**
     * 根据指定文件名和写入模式，打开文件
     * 
     * @param name
     *            文件名称
     * @param append
     *            写入模式
     * @return 打开结果
     */
    private int open(String name, boolean append)
    {
        String mode = SvnConstants.OPER_WRITE_STR;
        if (append)
        {
            mode = SvnConstants.OPER_APPEND;

        }
        
        closed = false;
        
        return SvnFileTool.openFile(name, mode);
    }

    /**
     * 将缓冲区中的内容写入输出流
     * 
     * @param b
     *            写入缓冲区
     */
    @Override
    public void write(byte[] b)
    {
        SvnFileTool.writeFile(b, this.fileDesc);
    }

    /**
     * 在输出流中写一个int值
     * 
     * @param b
     *            写入值
     * @throws IOException
     *              I/O异常
     */
    @Override
    public void write(int b) throws IOException
    {
        byte[] tempBuf = new byte[1];

        tempBuf[0] = (byte) b;
        SvnFileTool.writeFile(tempBuf, fileDesc);
        // logger.info("the writeLen of SvnFileOutputStream.write(int): " +
        // writeLen);
        // logger.info("the content of SvnFileOutputStream.write(int): " + new
        // String(tempBuf));
    }

    /**
     * 将缓冲区中指定偏移量后的指定长度字节，写入输出流
     * 
     * @param b
     *            写入缓冲区
     * @param off
     *            偏移量
     * @param len
     *            写入长度
     * @throws IOException
     *              I/O异常
     */
    public void write(byte[] b, int off, int len) throws IOException
    {
        byte[] temp = new byte[len];
        System.arraycopy(b, off, temp, 0, len);
//        for (int i = 0; i < len; i++)
//        {
//            temp[i] = b[off + i];
//        }
        SvnFileTool.writeFile(temp, fileDesc);
        // logger.info("the writeLen of SvnFileOutputStream.write(byte[],int,int): "
        // + writeLen);
        // logger.info("the content of SvnFileOutputStream.writewrite(byte[],int,int): "
        // + new String(b));
    }

    /**
     * 关闭输出流
     * 
     * @throws IOException
     *              I/O异常
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
//            /*
//             * Decrement FD use count associated with the channel The use count
//             * is incremented whenever a new channel is obtained from this
//             * stream
//             */
//            // fd.decrementAndGetUseCount();
//            channel.close();
//        }

        /*
         * Decrement FD use count associated with this stream
         */
        // int useCount = fd.decrementAndGetUseCount();

        /*
         * If FileDescriptor is still in use by another stream, the finalizer
         * will not close it
         */
        // if ((useCount <= 0) || !isRunningFinalize()) {
        if (!isRunningFinalize() && fileDesc != 0
                && useCount.decrementAndGet() == 0)
        {
            // close0();
            SvnFileTool.closeFile(fileDesc);
        }
    }

    /**
     * 获取通道
     * 
     * @return 文件通道
     */
    public FileChannel getChannel()
    {
        try
        {
            throw new IOException("FileChannel not supported!");
        }
        catch (IOException e)
        {
            //e.printStackTrace();
            Log.i("SDK", "FileChannel not supported!");
            // logger.severe("FileChannel not supported!");
        }
        return null;
    }

    /* (non-Javadoc)
     * @see java.io.FileOutputStream#finalize()
     */
    @Override
    protected void finalize() throws IOException
    {
        super.finalize();
        if (fileDesc != 0)
        {
            flush();
            
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
