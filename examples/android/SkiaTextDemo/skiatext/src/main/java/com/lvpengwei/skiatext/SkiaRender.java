package com.lvpengwei.skiatext;

public class SkiaRender {
    private SkiaSurface skiaSurface = null;

    public SkiaRender() {
        nativeSetup();
    }

    public SkiaSurface getSurface() {
        return skiaSurface;
    }

    public void setSurface(SkiaSurface surface) {
        this.skiaSurface = surface;
        if (surface == null) {
            nativeSetSurface(0);
        } else {
            nativeSetSurface(surface.nativeSurface);
        }
    }

    private native void nativeSetSurface(long surface);

    public native void draw();

    public native boolean flush();

    protected void finalize() {
        release();
    }

    public final void release() {
        nativeRelease();
    }

    public native final void nativeRelease();

    private native final void nativeSetup();

    private static native final void nativeInit();

    static {
        System.loadLibrary("skiatext");
        nativeInit();
    }

    private long nativeContext = 0;
}
