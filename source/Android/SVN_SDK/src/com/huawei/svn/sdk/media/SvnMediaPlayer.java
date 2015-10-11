package com.huawei.svn.sdk.media;

import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.WindowManager.LayoutParams;
import android.widget.FrameLayout;
import android.widget.MediaController;
import android.widget.VideoView;

public class SvnMediaPlayer {

	protected static final String TAG = "VideoPlayer";

	protected static final String ASSETS = "/android_asset/";



	private VideoView videoView;

	
	
	//private int lastPos = 0;

	private SvnMediaProxy proxy =  SvnMediaProxy.getInstance();

	private static SvnMediaPlayer instance = null;

	public static SvnMediaPlayer getInstance() {
		if (instance == null) {
			instance = new SvnMediaPlayer();
		}

		return instance;
	}

	private SvnMediaPlayer() {

	}

	public void play(final Activity context, String url) {
		if(context == null || url == null)
		{
			return;
		}
		
		final String path = stripFileProtocol(url);

		Handler handler = new Handler(context.getMainLooper());
		handler.post(new Runnable() {
			public void run() {
				openVideoDialog(context, path);
			}
		});

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
	public static String stripFileProtocol(String uriString) {
		if (uriString.startsWith("file://")) {
			return Uri.parse(uriString).getPath();
		}
		return uriString;
	}

	@TargetApi(Build.VERSION_CODES.JELLY_BEAN)
	protected void openVideoDialog(Activity context, String path) {
	
		Log.i(TAG, "openVideoDialog-----1:" + path);
		final ViewGroup rootView = (ViewGroup) ((Activity) context).findViewById(android.R.id.content);

		// Main container layout
		final FrameLayout main = new FrameLayout(context);
		main.setLayoutParams(new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

		//main.setBackgroundColor(Color.DKGRAY);
		//main.setAlpha(0.8f);
		
		 main.setOnTouchListener(new View.OnTouchListener() {
				
			 @Override
			 public boolean onTouch(View paramView, MotionEvent paramMotionEvent)
			 {
			 // TODO Auto-generated method stub
				 Log.e(TAG, "main onTouch");
//				
//				 Log.e(TAG, "main layout:" + main);
//				 Log.e(TAG, "video layout:" + videoView);
//				
//				 Log.e(TAG, "mediaController layout:" + mediaController);
//				
//				 Log.e(TAG, "mediaController layout's parent:" +
//				 mediaController.getParent());
				 return true;
			
			
			 }
		 });
		
		main.setOnKeyListener(new View.OnKeyListener() {

			@Override
			public boolean onKey(View paramView, int keyCode, KeyEvent event) {
				// TODO Auto-generated method stub
				 Log.e(TAG, "main onKey------");
				if (keyCode == KeyEvent.KEYCODE_BACK) {
					// 弹出确定退出对话框

					// 这里不需要执行父类的点击事件，所以直接return
					rootView.removeView(main);
					return true;
				}
				// 继续执行父类的其他点击事件
				return false;
			}
		});
		
		//main.setLayerType(View.LAYER_TYPE_NONE, null);
		
		main.setLayerType(View.LAYER_TYPE_HARDWARE, null);
		rootView.addView(main);

		videoView = new VideoView(context);

		// videoView.setBackgroundColor(Color.DKGRAY);

		FrameLayout.LayoutParams videoParams = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT,
				LayoutParams.MATCH_PARENT);
		videoParams.gravity = Gravity.CENTER_VERTICAL;
		videoView.setLayoutParams(videoParams);
		// videoView.setZOrderMediaOverlay(true);
		//videoView.setBackgroundColor(Color.RED);
		videoView.setZOrderOnTop(true);
		//videoView.setZOrderMediaOverlay(true);
		
		videoView.setOnKeyListener(new View.OnKeyListener() {

			@Override
			public boolean onKey(View paramView, int keyCode, KeyEvent event) {
				// TODO Auto-generated method stub
				 Log.e(TAG, "videoView onKey------");
				if (keyCode == KeyEvent.KEYCODE_BACK) {
					// 弹出确定退出对话框

					// 这里不需要执行父类的点击事件，所以直接return
					rootView.removeView(main);
					
					videoView = null;
					return true;
				}
				// 继续执行父类的其他点击事件
				return false;
			}
		});
		
		main.addView(videoView);
		
		main.bringChildToFront(videoView);

//		final MediaController mediaController = new MediaController(context);

		
		final MediaController mediaController = new MediaController(context){
		    @Override
		    public void show(int timeout) {
		        super.show(0);
		        //Log.e(TAG, "VideoView isShow:"+ videoView.isShown());
		        
		       
		        //Log.e(TAG, "VideoView top:"+ videoView.getTop() + ", bottom:" + videoView.getBottom());
		        
		        
		        int [] location = new int[2];
		        
		        videoView.getLocationOnScreen(location);
		        
		        //Log.e(TAG, "VideoView screen top:"+ location[0] + ", left:" + location[1] );
		    }	    
		    
		};
		
		

		videoView.setMediaController(mediaController);
		

		try {

			//String encodedPath = URLEncoder.encode(path, "UTF-8");
			
			String playUrl = String.format("http://127.0.0.1:%d/%s", proxy.getPort(), path);
			
			//String name = path.substring(path.lastIndexOf('/') + 1);
			
			//String encodedName = URLEncoder.encode(name, "UTF-8");
			
			//String playUrl = String.format("http://10.170.26.237:8180/HttpServerDemo/e/Download.do?fileName=%s", encodedName);

			videoView.setVideoPath(playUrl);
		} catch (Exception e) {

			Log.i(TAG, "openVideoDialog-----" + e.getMessage());
			return;
		}

		videoView.requestFocus();
		videoView.start();

		Log.i(TAG, "openVideoDialog-----5");

		
		
		
	}
//	
//	public void resumePlay()
//	{
//		if(videoView != null)
//		{
//			videoView.seekTo(lastPos);
//			
//			Log.e(TAG, "resumePlay seekTo pos:" + lastPos);
//			videoView.start();
//			
//		}
//		
//	}
//	
//	
//	public void suspendPlay()
//	{
//		if(videoView != null)
//		{
//			
//			lastPos = videoView.getCurrentPosition();
//			
//			Log.e(TAG, "pause pos:" + lastPos);
//			
//			videoView.pause();
//			//videoView.suspend();
//			
//			
//			
//		}
//	}

	// End VideoMediaController.MediaPlayerControl
}
