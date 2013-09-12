package test.renderer.canvas;

import android.graphics.Bitmap;

public class NativeLayer {
	private static native final void NativeInit();
	private native int NativeSetup();
	private native int NativeRender();
    
	private Bitmap mBitmap;
	 
    static {
        System.loadLibrary("renderer_jni");
        NativeInit();
    }
    
    public NativeLayer() {
    }

    public int DoSetup(Bitmap bmp) {
    	mBitmap = bmp;
    	return NativeSetup();
    }
    public int DoRender() {
    	return NativeRender();
    }
}
