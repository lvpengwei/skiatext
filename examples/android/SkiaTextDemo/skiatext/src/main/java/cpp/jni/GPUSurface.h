//
// Created by dom on 26/01/2018.
//

#ifndef ANDROID_GPUSURFACE_H
#define ANDROID_GPUSURFACE_H

#include "skiatext.h"
#include <android/native_window.h>
#include "GPUContext.h"

namespace skiatext {
    class GPUSurface : public Surface {
    public:
        GPUSurface(ANativeWindow* nativeWindow, bool useMSAA = false);

        ~GPUSurface();

    protected:

        void onEnter() override;

        void onExit() override;

        void onPresent() override;

        void onUpdateSize() override;

        SkSurface* onCreateSurface() override;

    private:
        ANativeWindow* nativeWindow = nullptr;
        bool useMSAA = false;
        EGLSurface eglSurface = nullptr;
        GPUContext* context = nullptr;

        void tearDownSurface();
    };
}

#endif //ANDROID_GPUSURFACE_H
