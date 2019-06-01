package com.lvpengwei.skiatext;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.util.AttributeSet;
import android.view.TextureView;

public class SkiaView extends TextureView implements TextureView.SurfaceTextureListener {
    private SkiaRender render;
    private SkiaSurface skiaSurface;
    public SkiaView(Context context) {
        super(context);
        setupSurfaceTexture();
    }

    public SkiaView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setupSurfaceTexture();
    }

    public SkiaView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setupSurfaceTexture();
    }

    private void setupSurfaceTexture() {
        render = new SkiaRender();
        setOpaque(false);
        setSurfaceTextureListener(this);
    }
    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        if (skiaSurface != null) {
            skiaSurface.release();
            skiaSurface = null;
        }
        skiaSurface = SkiaSurface.fromSurfaceTexture(surface);
        skiaSurface.updateSize();
        render.setSurface(skiaSurface);
        render.flush();
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        skiaSurface.updateSize();
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        if (skiaSurface != null) {
            skiaSurface.release();
            skiaSurface = null;
        }
        render.setSurface(null);
        return true;
    }

    public boolean flush() {
        return render.flush();
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {

    }
}
