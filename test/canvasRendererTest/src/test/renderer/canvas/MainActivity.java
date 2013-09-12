package test.renderer.canvas;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.view.View;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;

public class MainActivity extends Activity {
	
	private NativeLayer mNativeLayer;
	
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
      //  setContentView(R.layout.main);
        mNativeLayer = new NativeLayer();
       // MainView view = new MainView(this);
        MainSurfaceView view = new MainSurfaceView(this);
        view.SetNativeLayer(mNativeLayer);
        setContentView(view);
    }
    
   
}

class MainView extends View {
    private Bitmap mBitmap;
    private NativeLayer mNativeLayer;
    private long mPrevRenderTime;
    private final int mFps = 25;
    private final int mVidWidth = 320;
    private final int mVidHeight = 240;
    
    public MainView(Context context) {
        super(context);
        mBitmap = Bitmap.createBitmap(mVidWidth, mVidHeight, Bitmap.Config.RGB_565);
    }

    public void SetNativeLayer(NativeLayer nativeLayer) {
    	mNativeLayer = nativeLayer;
    	mNativeLayer.DoSetup(mBitmap);
    	mPrevRenderTime = System.currentTimeMillis();
    }
    
    @Override protected void onDraw(Canvas canvas) {
    	final int timePerFrame = 1000 / mFps;
    	long curTime = System.currentTimeMillis();
    	Log.d("View", "prev time " + mPrevRenderTime + ", cur time " + curTime);
    	if (curTime - mPrevRenderTime >= timePerFrame)
    	{
    		Rect rcView = new Rect();
    		rcView.set(0, 0, getWidth(), getHeight());
    	//	Log.d("View", "wnd size " + getWidth() + "x" + getHeight());
    		mNativeLayer.DoRender();
    		canvas.drawBitmap(mBitmap, null, rcView, null);
    		
    		mPrevRenderTime = curTime;
    	}

    	// force a redraw, with a different time-based pattern.
		invalidate();
    }
}

class MainSurfaceView extends SurfaceView implements SurfaceHolder.Callback{
    private Bitmap mBitmap;
    private NativeLayer mNativeLayer;
    private final int mVidWidth = 320;
    private final int mVidHeight = 240;
    private RenderThread mRenderThread;
    private SurfaceHolder mHolder;
    private boolean mExitThread;
    
    public MainSurfaceView(Context context) {
        super(context);
        mBitmap = Bitmap.createBitmap(mVidWidth, mVidHeight, Bitmap.Config.RGB_565);
    }
    
    public void SetNativeLayer(NativeLayer nativeLayer) {
    	mNativeLayer = nativeLayer;
    	mNativeLayer.DoSetup(mBitmap);
    	mHolder = getHolder();
    	mHolder.addCallback(this);
    }
    
    public Rect CalcTargetRect(Rect dispRect, Rect vidRect) {
    	float widthScale = (float)dispRect.width() / vidRect.width();
    	float heightScale = (float)dispRect.height() / vidRect.height();
    	float targetScale = min(widthScale, heightScale);
    	int targetWidth = (int)(targetScale * (float)vidRect.width());
    	int targetHeight = (int)(targetScale * (float)vidRect.height());

    	int targetLeft = 0;
    	if (targetWidth < dispRect.width())
    	{
    		targetLeft = (dispRect.width() - targetWidth) / 2;
    	}

    	int targetTop = 0;
    	if (targetHeight < dispRect.height())
    	{
    		targetTop = (dispRect.height() - targetHeight) / 2;
    	}

    	return new Rect(targetLeft, targetTop, targetLeft + targetWidth, targetTop + targetHeight);
    }
       
    private float min(float x, float y) {
		if (x <= y)	{
			return x;
		}
		else {
			return y;
		}
	}

	class RenderThread extends Thread {
    	public void run() {
    		final int fps = 25;
    		final int timePerFrame = 1000 / fps;
    		long nextRenderTime = System.currentTimeMillis();
    		while (!mExitThread)
    		{
    			long curTime = System.currentTimeMillis();
    			if (curTime >= nextRenderTime)
    			{
    	    	/*	Rect rcView = new Rect();
    	    		rcView.set(0, 0, getWidth(), getHeight());*/
    			/*	Rect rcView = CalcTargetRect(new Rect(0, 0, getWidth(), getHeight()), 
    						new Rect(0, 0, mVidWidth, mVidHeight));*/
    				Rect rcView = CalcTargetRect(new Rect(0, 0, mVidWidth, mVidHeight), 
    						new Rect(0, 0, mVidWidth, mVidHeight));
    	    		Log.d("View", "wnd size " + rcView.width() + "x" + rcView.height());
    	    		Canvas canvas = mHolder.lockCanvas();
    	    		if (canvas == null)
    	    		{
    	    			Log.e("View", "Surface not created yet!");
    	    			nextRenderTime += timePerFrame;
    	    			continue;
    	    		}
    	    		mNativeLayer.DoRender();
    	    		canvas.drawBitmap(mBitmap, null, rcView, null);
    	    		mHolder.unlockCanvasAndPost(canvas);
    	    		
    	    		nextRenderTime += timePerFrame;
    	    	}
    			else
    			{
    				try {
						sleep(nextRenderTime - curTime);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
    			}
    		}
    	}
    }
    
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    	Log.d("View", "surfaceChanged fmt " + format + ", size " + width + "x" + height);
    }
    
    public void surfaceCreated(SurfaceHolder holder) {
    	Log.d("View", "surfaceCreated in");
    	mExitThread = false;
    	mRenderThread = new RenderThread();
    	mRenderThread.start();
    	Log.d("View", "surfaceCreated out");
    }
    
    public void surfaceDestroyed(SurfaceHolder holder) {
    	Log.d("View", "surfaceDestroyed in");
    	mExitThread = true;
    	if (mRenderThread != null)
    	{
    		try {
				mRenderThread.join();
			} catch (InterruptedException e) {
			// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	Log.d("View", "surfaceDestroyed out");
    }
}



