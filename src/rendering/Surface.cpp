#include "skiatext.h"

namespace skiatext {
    Surface::~Surface() {
        SkSafeUnref(surface);
    }

    void Surface::enter() {
        onEnter();
    }

    void Surface::exit() {
        onExit();
    }

    bool Surface::present() {
        onPresent();
        return true;
    }

    void Surface::updateSize() {
        SkSafeUnref(surface);
        surface = nullptr;
        onUpdateSize();
    }

    SkSurface* Surface::getSurface() {
        if (surface == nullptr) {
            surface = onCreateSurface();
        }
        return surface;
    }
}
