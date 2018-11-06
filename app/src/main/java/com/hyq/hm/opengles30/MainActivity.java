package com.hyq.hm.opengles30;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.SurfaceTexture;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.HandlerThread;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.PermissionChecker;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Toast;

import com.google.android.exoplayer2.ExoPlayerFactory;
import com.google.android.exoplayer2.SimpleExoPlayer;
import com.google.android.exoplayer2.source.ExtractorMediaSource;
import com.google.android.exoplayer2.source.MediaSource;
import com.google.android.exoplayer2.trackselection.AdaptiveTrackSelection;
import com.google.android.exoplayer2.trackselection.DefaultTrackSelector;
import com.google.android.exoplayer2.trackselection.TrackSelection;
import com.google.android.exoplayer2.trackselection.TrackSelector;
import com.google.android.exoplayer2.upstream.DataSource;
import com.google.android.exoplayer2.upstream.DefaultBandwidthMeter;
import com.google.android.exoplayer2.upstream.DefaultDataSourceFactory;
import com.google.android.exoplayer2.util.Util;
import com.hyq.hm.gles.EGLUtils;
import com.hyq.hm.gles.GLES;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    private String[] denied;
    private String[] permissions = {Manifest.permission.WRITE_EXTERNAL_STORAGE};

    private Handler mainHandler;

    private SeekBar seekBar;


    private Surface mSurface;

    private Handler playHandler;
    private HandlerThread playThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mainHandler = new Handler();

        playThread = new HandlerThread("PlayThread");
        playThread.start();
        playHandler = new Handler(playThread.getLooper());

        eglUtils = new EGLUtils();
        gles = new GLES();
        surfaceView = findViewById(R.id.surface_view);

        seekBar = findViewById(R.id.seek_bar);
        playView = findViewById(R.id.play_view);



        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(final SurfaceHolder holder, int format, final int width, final int height) {
                playHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        boolean init = false;
                        while (isResume){
                            if(player != null && player.getVideoFormat() != null){
                                if(player.getVideoFormat().width != 0 && player.getVideoFormat().height != 0){
                                    init = true;
                                    break;
                                }
                            }
                            try {
                                Thread.sleep(100);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                        if(init){
                            eglUtils.initEGL(holder.getSurface());
                            gles.initFrame(player.getVideoFormat().width,player.getVideoFormat().height,width,height);
                            final SurfaceTexture surfaceTexture = gles.getSurfaceTexture();
                            surfaceTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
                                @Override
                                public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                                    gles.drawFrame();
                                    eglUtils.swap();
                                }
                            });
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    mSurface = new Surface(surfaceTexture);
                                    player.setVideoSurface(mSurface);
                                    player.setPlayWhenReady(isPlayer);
                                    if(player.getPlayWhenReady()){
                                        videoTime();
                                    }
                                }
                            });
                        }
                    }
                });
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                playHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        if(gles != null){
                            gles.releaseFrame();
                        }
                        if(eglUtils != null){
                            eglUtils.release();
                        }
                    }
                });
                if(mSurface != null){
                    mSurface.release();
                    mSurface = null;
                }
            }
        });


        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                isTracking = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                player.seekTo(seekBar.getProgress()*player.getDuration()/100);
                isTracking = false;
            }
        });

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            ArrayList<String> list = new ArrayList<>();
            for (int i = 0; i < permissions.length; i++) {
                if (PermissionChecker.checkSelfPermission(this, permissions[i]) == PackageManager.PERMISSION_DENIED) {
                    list.add(permissions[i]);
                }
            }
            if (list.size() != 0) {
                denied = new String[list.size()];
                for (int i = 0; i < list.size(); i++) {
                    denied[i] = list.get(i);
                }
                ActivityCompat.requestPermissions(this, denied, 5);
            } else {
                init();
            }
        } else {
            init();
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == 5) {
            boolean isDenied = false;
            for (int i = 0; i < denied.length; i++) {
                String permission = denied[i];
                for (int j = 0; j < permissions.length; j++) {
                    if (permissions[j].equals(permission)) {
                        if (grantResults[j] != PackageManager.PERMISSION_GRANTED) {
                            isDenied = true;
                            break;
                        }
                    }
                }
            }
            if (isDenied) {
                Toast.makeText(this, "请开启权限", Toast.LENGTH_SHORT).show();
            } else {
                init();

            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }
    private SimpleExoPlayer player;
    private SurfaceView surfaceView;

    private EGLUtils eglUtils;
    private GLES gles;

    private ImageView playView;


    private void init(){
        Uri url = Uri.parse(Environment.getExternalStorageDirectory().getAbsolutePath() +"/HMSDK/video/1533617323450.mp4");
        DefaultBandwidthMeter bandwidthMeter = new DefaultBandwidthMeter();


        TrackSelection.Factory videoTrackSelectionFactory = new AdaptiveTrackSelection.Factory(bandwidthMeter);
        TrackSelector trackSelector = new DefaultTrackSelector(videoTrackSelectionFactory);

        player = ExoPlayerFactory.newSimpleInstance(this, trackSelector);


        DataSource.Factory dataSourceFactory = new DefaultDataSourceFactory(this,
                Util.getUserAgent(this, "ExoPlayerTime"), bandwidthMeter);


        MediaSource videoSource = new ExtractorMediaSource.Factory(dataSourceFactory).createMediaSource(url, mainHandler,null);
//        LoopingMediaSource loopingSource = new LoopingMediaSource(videoSource);
        player.prepare(videoSource);
    }

    private boolean isTracking = false;
    private void videoTime(){
        seekBar.postDelayed(new Runnable() {
            @Override
            public void run() {
                if(!isTracking){
                    int progress = (int) (player.getContentPosition()*100/player.getDuration());
                    seekBar.setProgress(progress);
                }
                if(player.getContentPosition() >= player.getDuration()){
                    player.setPlayWhenReady(false);
                    playView.setImageResource(R.drawable.ic_play);
                }
                if(isResume && player.getPlayWhenReady()){
                    videoTime();
                }
            }
        },100);
    }







    private boolean isResume = false;
    private boolean isPlayer = false;
    @Override
    protected void onResume() {
        super.onResume();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M){
            View decorView = getWindow().getDecorView();
            int mHideFlags =
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            decorView.setSystemUiVisibility(mHideFlags);
        }else{
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        isResume = true;

    }
    @Override
    protected void onPause() {
        super.onPause();
        isResume = false;
        if(player != null){
            isPlayer = player.getPlayWhenReady();
            if(isPlayer){
                player.setPlayWhenReady(false);
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(player != null){
            player.stop();
            player.release();
        }
        gles.release();
        playThread.quit();
    }

    public void onPlayer(View view){
        if(player.getPlayWhenReady()){
            player.setPlayWhenReady(false);
            playView.setImageResource(R.drawable.ic_play);
        }else{
            if(player.getContentPosition() >= player.getDuration()){
                player.seekTo(0);
            }
            player.setPlayWhenReady(true);
            playView.setImageResource(R.drawable.ic_stop);
            videoTime();
        }
    }
}
