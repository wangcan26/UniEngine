package com.universe.unilauncher;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class UniRenderView extends SurfaceView implements SurfaceHolder.Callback {
    private SurfaceHolder  mSurfaceHolder;


    public UniRenderView(Context context)
    {
        super(context);
    }

    public UniRenderView(Context context, AttributeSet atts)
    {
        super(context, atts);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

    }
}
