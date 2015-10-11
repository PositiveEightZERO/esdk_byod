package com.huawei.esdk.anyoffice.cordova;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaArgs;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaResourceApi;
import org.apache.cordova.PluginResult;
import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.content.pm.ActivityInfo;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.FrameLayout;
import android.widget.VideoView;

import com.huawei.byod.R;

public class VideoPlayerCordova extends CordovaPlugin implements
        OnCompletionListener, OnPreparedListener, OnErrorListener,
        OnDismissListener, VideoControllerView.MediaPlayerControl
{

    protected static final String TAG = "VideoPlayer";

    protected static final String ASSETS = "/android_asset/";

    private CallbackContext callbackContext = null;

    private Dialog dialog;

    private VideoView videoView;

    private MediaPlayer player;

    private VideoControllerView controller;

    private StreamProxy proxy = new StreamProxy();

    /**
     * Called after plugin construction and fields have been initialized.
     */
    protected void pluginInitialize()
    {

        proxy.init();
        proxy.start();
    }

    /**
     * Executes the request and returns PluginResult.
     *
     * @param action
     *            The action to execute.
     * @param args
     *            JSONArray of arguments for the plugin.
     * @param callbackId
     *            The callback id used when calling back into JavaScript.
     * @return A PluginResult object with a status and message.
     */
    public boolean execute(String action, CordovaArgs args,
            CallbackContext callbackContext) throws JSONException
    {
        if (action.equals("play"))
        {
            this.callbackContext = callbackContext;

            CordovaResourceApi resourceApi = webView.getResourceApi();
            String target = args.getString(0);
            final JSONObject options = args.getJSONObject(1);

            String fileUriStr;
            try
            {
                Uri targetUri = resourceApi.remapUri(Uri.parse(target));
                fileUriStr = targetUri.toString();
            }
            catch (IllegalArgumentException e)
            {
                fileUriStr = target;
            }

            Log.v(TAG, fileUriStr);

            final String path = stripFileProtocol(fileUriStr);

            // Create dialog in new thread
            cordova.getActivity().runOnUiThread(new Runnable()
            {
                public void run()
                {
                    openVideoDialog(path, options);
                }
            });

            // Don't return any result now
            PluginResult pluginResult = new PluginResult(
                    PluginResult.Status.NO_RESULT);
            pluginResult.setKeepCallback(true);
            callbackContext.sendPluginResult(pluginResult);
            callbackContext = null;

            return true;
        }
        else if (action.equals("close"))
        {
            if (dialog != null)
            {
                if (player.isPlaying())
                {
                    player.stop();
                }
                player.release();
                dialog.dismiss();
            }

            if (callbackContext != null)
            {
                PluginResult result = new PluginResult(PluginResult.Status.OK);
                result.setKeepCallback(false); // release status callback in JS
                                               // side
                callbackContext.sendPluginResult(result);
                callbackContext = null;
            }

            return true;
        }
        return false;
    }

    /**
     * Removes the "file://" prefix from the given URI string, if applicable. If
     * the given URI string doesn't have a "file://" prefix, it is returned
     * unchanged.
     *
     * @param uriString
     *            the URI string to operate on
     * @return a path without the "file://" prefix
     */
    public static String stripFileProtocol(String uriString)
    {
        if (uriString.startsWith("file://"))
        {
            return Uri.parse(uriString).getPath();
        }
        return uriString;
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    protected void openVideoDialog(String path, JSONObject options)
    {

        Log.i(TAG, "openVideoDialog-----1");
        // Let's create the main dialog
        dialog = new Dialog(cordova.getActivity(), R.style.Dialog_Fullscreen);// android.R.style.Theme_NoTitleBar
        dialog.getWindow().getAttributes().windowAnimations = android.R.style.Animation_Dialog;
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCancelable(true);
        dialog.setOnDismissListener(this);

        // Main container layout
        FrameLayout main = new FrameLayout(cordova.getActivity());
        main.setLayoutParams(new FrameLayout.LayoutParams(
                LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        // main.setOrientation(LinearLayout.VERTICAL);
        // main.setHorizontalGravity(Gravity.CENTER_HORIZONTAL);
        // main.setVerticalGravity(Gravity.CENTER_VERTICAL);

        videoView = new VideoView(cordova.getActivity());
        videoView.setLayoutParams(new FrameLayout.LayoutParams(
                LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        videoView.setOnTouchListener(new View.OnTouchListener()
        {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                // TODO Auto-generated method stub
                controller.show();
                return false;
            }
        });
        // videoView.setVideoURI(uri);
        // videoView.setVideoPath(path);
        main.addView(videoView);

        Log.i(TAG, "openVideoDialog-----2");

        if(player == null)
        {
            player = new MediaPlayer();
            player.setOnPreparedListener(this);
            player.setOnCompletionListener(this);
            player.setOnErrorListener(this);
        }
        else {
            player.reset();
        }

        controller = new VideoControllerView(cordova.getActivity());

        if (path.startsWith(ASSETS))
        {
            String f = path.substring(15);
            AssetFileDescriptor fd = null;
            try
            {
                fd = cordova.getActivity().getAssets().openFd(f);
                player.setDataSource(fd.getFileDescriptor(),
                        fd.getStartOffset(), fd.getLength());
            }
            catch (Exception e)
            {
                PluginResult result = new PluginResult(
                        PluginResult.Status.ERROR, e.getLocalizedMessage());
                result.setKeepCallback(false); // release status callback in JS
                                               // side
                callbackContext.sendPluginResult(result);
                callbackContext = null;
                return;
            }
        }
        else
        {
            try
            {

                String playUrl = String.format("http://127.0.0.1:%d/%s",
                        proxy.getPort(), path);

                player.setDataSource(playUrl);
            }
            catch (Exception e)
            {
                PluginResult result = new PluginResult(
                        PluginResult.Status.ERROR, e.getLocalizedMessage());
                result.setKeepCallback(false); // release status callback in JS
                                               // side
                callbackContext.sendPluginResult(result);
                callbackContext = null;
                return;
            }
        }
        Log.i(TAG, "openVideoDialog-----3");
        player.setVolume(20, 20);
        // player.setVideoScalingMode(MediaPlayer.VIDEO_SCALING_MODE_SCALE_TO_FIT);

        final SurfaceHolder mHolder = videoView.getHolder();
        mHolder.setKeepScreenOn(true);
        mHolder.addCallback(new SurfaceHolder.Callback()
        {
            @Override
            public void surfaceCreated(SurfaceHolder holder)
            {
                Log.v(TAG, "surfaceCreated----");
                player.setDisplay(holder);
                try
                {
                    player.prepareAsync();
                    Log.v(TAG, "prepareAsync----");
                }
                catch (Exception e)
                {
                    PluginResult result = new PluginResult(
                            PluginResult.Status.ERROR, e.getLocalizedMessage());
                    result.setKeepCallback(false); // release status callback in
                                                   // JS side
                    callbackContext.sendPluginResult(result);
                    callbackContext = null;
                }
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder)
            {
                Log.i(TAG, "surfaceDestroyed");
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format,
                    int width, int height)
            {
            }
        });

        WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
        lp.copyFrom(dialog.getWindow().getAttributes());
        lp.width = WindowManager.LayoutParams.MATCH_PARENT;
        lp.height = WindowManager.LayoutParams.MATCH_PARENT;

        dialog.setContentView(main);
        dialog.show();
        dialog.getWindow().setAttributes(lp);

        Log.i(TAG, "openVideoDialog-----5");
    }

    @Override
    public boolean onError(MediaPlayer mp, int what, int extra)
    {
        Log.e(TAG, "MediaPlayer.onError(" + what + ", " + extra + ")");
        if (mp.isPlaying())
        {
            mp.stop();
        }
        mp.release();
        dialog.dismiss();
        player = null;
        return false;
    }

    @Override
    public void onPrepared(MediaPlayer mp)
    {
        Log.v(TAG, "onPrepared----");
        controller.setMediaPlayer(this);
        controller.setAnchorView((FrameLayout) videoView.getParent());

        // Get the dimensions of the video
        int videoWidth = mp.getVideoWidth();
        int videoHeight = mp.getVideoHeight();

        // Get the width of the screen
        int screenWidth = cordova.getActivity().getWindowManager()
                .getDefaultDisplay().getWidth();

        // Get the SurfaceView layout parameters
        android.view.ViewGroup.LayoutParams lp = videoView.getLayoutParams();

        // Set the width of the SurfaceView to the width of the screen
        lp.width = screenWidth;

        // Set the height of the SurfaceView to match the aspect ratio of the
        // video
        // be sure to cast these as floats otherwise the calculation will likely
        // be 0
        lp.height = (int) (((float) videoHeight / (float) videoWidth) * (float) screenWidth);

        // Commit the layout parameters
        videoView.setLayoutParams(lp);

        mp.start();
    }

    @Override
    public void onCompletion(MediaPlayer mp)
    {
        Log.d(TAG, "MediaPlayer completed");
        mp.release();
        dialog.dismiss();
        player = null;
    }

    @Override
    public void onDismiss(DialogInterface dialog)
    {
        Log.d(TAG, "Dialog dismissed");
        if (callbackContext != null)
        {
            PluginResult result = new PluginResult(PluginResult.Status.OK);
            result.setKeepCallback(false); // release status callback in JS side
            callbackContext.sendPluginResult(result);
            callbackContext = null;
        }
    }

    // Implement VideoMediaController.MediaPlayerControl
    @Override
    public boolean canPause()
    {
        return true;
    }

    @Override
    public boolean canSeekBackward()
    {
        return true;
    }

    @Override
    public boolean canSeekForward()
    {
        return true;
    }

    @Override
    public int getBufferPercentage()
    {
        return 0;
    }

    @Override
    public int getCurrentPosition()
    {
        if (player == null)
        {
            return 0;
        }
        return player.getCurrentPosition();
    }

    @Override
    public int getDuration()
    {
        if (player == null)
        {
            return 0;
        }
        return player.getDuration();
    }

    @Override
    public boolean isPlaying()
    {
        if (player == null)
        {
            return false;
        }
        return player.isPlaying();
    }

    @Override
    public void pause()
    {
        if (player == null)
        {
            return;
        }
        player.pause();
    }

    @Override
    public void seekTo(int i)
    {
        if (player == null)
        {
            return;
        }
        player.seekTo(i);
    }

    @Override
    public void start()
    {
        if (player == null)
        {
            return;
        }
        player.start();
    }

    // @Override
    // public boolean isFullScreen() {
    // return false;
    // }
    //
    // @Override
    // public void toggleFullScreen() {
    //
    // }
    //

    private boolean mFullScreen = true;

    @Override
    public boolean isFullScreen()
    {
        if (mFullScreen)
        {
            Log.v("FullScreen", "--set icon full screen--");
            return false;
        }
        else
        {
            Log.v("FullScreen", "--set icon small full screen--");
            return true;
        }
    }

    @Override
    public void toggleFullScreen()
    {
        Log.v("FullScreen",
                "-----------------click toggleFullScreen-----------");
        setFullScreen(isFullScreen());

    }

    // End VideoMediaController.MediaPlayerControl

    @SuppressLint("NewApi")
    public void setFullScreen(boolean fullScreen)
    {
        fullScreen = false;

        Activity activity = cordova.getActivity();
        if (mFullScreen)

        {
            activity.getWindow()
                    .getDecorView()
                    .setSystemUiVisibility(
                            View.SYSTEM_UI_FLAG_FULLSCREEN
                                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
            activity.getWindow().clearFlags(
                    WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);

            Log.v("FullScreen",
                    "-----------Set full screen SCREEN_ORIENTATION_LANDSCAPE------------");
            activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
            DisplayMetrics displaymetrics = new DisplayMetrics();
            activity.getWindowManager().getDefaultDisplay()
                    .getMetrics(displaymetrics);
            int height = displaymetrics.heightPixels;
            int width = displaymetrics.widthPixels;
            android.widget.FrameLayout.LayoutParams params = (android.widget.FrameLayout.LayoutParams) videoView
                    .getLayoutParams();
            params.width = width;
            params.height = height;
            params.setMargins(0, 0, 0, 0);
            // set icon is full screen
            mFullScreen = fullScreen;
        }
        else
        {
            Log.v("FullScreen",
                    "-----------Set small screen SCREEN_ORIENTATION_PORTRAIT------------");
            activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            activity.getWindow().addFlags(
                    WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
            DisplayMetrics displaymetrics = new DisplayMetrics();
            activity.getWindowManager().getDefaultDisplay()
                    .getMetrics(displaymetrics);
            final FrameLayout mFrame = (FrameLayout) videoView.getParent();
            // int height = displaymetrics.heightPixels;
            int height = mFrame.getHeight();// get height Frame Container video
            int width = displaymetrics.widthPixels;
            android.widget.FrameLayout.LayoutParams params = (android.widget.FrameLayout.LayoutParams) videoView
                    .getLayoutParams();
            params.width = width;
            params.height = height;
            params.setMargins(0, 0, 0, 0);
            // set icon is small screen
            mFullScreen = !fullScreen;

        }
    }

    // End VideoMediaController.MediaPlayerControl
}
