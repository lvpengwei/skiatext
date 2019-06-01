package com.lvpengwei.skiatext;

import android.graphics.SurfaceTexture;
import android.view.Surface;

public class SkiaSurface {
    public static SkiaSurface fromSurface(Surface surface) {
        if (surface == null) {
            return null;
        }
        SkiaSurface skiaSurface = new SkiaSurface();
        skiaSurface.surface = surface;
        skiaSurface.nativeSurface = skiaSurface.setupFromSurface(surface);
        return skiaSurface;
    }

    public static SkiaSurface fromSurfaceTexture(SurfaceTexture surfaceTexture) {
        if (surfaceTexture == null) {
            return null;
        }
        return fromSurface(new Surface(surfaceTexture));
    }

    private SkiaSurface() {
    }

    private Surface surface = null;

    public native void updateSize();

    private native long setupFromSurface(Surface surface);

    public native void present();

    public native void release();

    private static native void nativeInit();

    protected void finalize() {
        release();
    }

    static {
        System.loadLibrary("skiatext");
        nativeInit();
    }

    long nativeSurface = 0;
}
