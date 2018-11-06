package com.hyq.hm.gles;

import android.graphics.SurfaceTexture;
import android.util.Log;

/**
 * Created by 海米 on 2018/11/5.
 */

public class GLES {
    static {
        System.loadLibrary("hm_gles");
    }
    public GLES(){
        gles = initGLES();
    }
    private long gles;


    private SurfaceTexture surfaceTexture;

    public void initFrame(int videoWidth,int videoHeight,int screenWidth,int screenHeight){
        initFrame(gles);
        int left,top,viewWidth,viewHeight;
        float sh = screenWidth*1.0f/screenHeight;
        float vh = videoWidth*1.0f/videoHeight;
        if(sh < vh){
            left = 0;
            viewWidth = screenWidth;
            viewHeight = (int)(videoHeight*1.0f/videoWidth*viewWidth);
            top = (screenHeight - viewHeight)/2;
        }else{
            top = 0;
            viewHeight = screenHeight;
            viewWidth = (int)(videoWidth*1.0f/videoHeight*viewHeight);
            left = (screenWidth - viewWidth)/2;
        }
        setRect(gles,left,top,viewWidth,viewHeight);
        if(surfaceTexture != null){
            surfaceTexture.release();
        }
        surfaceTexture = new SurfaceTexture(getTexture(gles));
        surfaceTexture.setDefaultBufferSize(videoWidth,videoHeight);
    }
    private final float[] mSTMatrix = new float[16];
    public void drawFrame(){
        surfaceTexture.updateTexImage();
        surfaceTexture.getTransformMatrix(mSTMatrix);
        drawFrame(gles,mSTMatrix);
    }

    public SurfaceTexture getSurfaceTexture() {
        return surfaceTexture;
    }

    public void releaseFrame(){
        releaseFrame(gles);
    }

    public void release(){
        if(surfaceTexture != null){
            surfaceTexture.release();
        }
        release(gles);
    }
    private native long initGLES();

    private native void initFrame(long gles);

    private native void setRect(long gles,int left,int top,int right,int bottom);

    private native int getTexture(long gles);

    private native void drawFrame(long gles,float[] mSTMatrix);

    private native void release(long gles);

    private native void releaseFrame(long gles);
}
