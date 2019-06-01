#include <sstream>
#include "skiatext.h"

static sk_sp<SkTypeface> _typeface() {
#ifdef SK_BUILD_FOR_ANDROID
    return SkTypeface::MakeFromName("sans-serif", SkFontStyle());
#elif defined(SK_BUILD_FOR_IOS)
    return SkTypeface::MakeFromName("Hiragino Sans GB W3", SkFontStyle());
#else
    return nullptr;
#endif
}

namespace skiatext {
    Renderer::Renderer() {
    }

    Renderer::~Renderer() {
    }

    void Renderer::setSurface(Surface* surface) {
        this->surface = surface;
        updateMatrix();
    }

    void Renderer::updateMatrix() {
        auto width = static_cast<float>(surface->width());
        auto height = static_cast<float>(surface->height());
        auto owidth = static_cast<float>(200);
        auto oheight = static_cast<float>(200);
        auto scaleX = width / owidth;
        auto scaleY = height / oheight;
        auto scale = std::min(scaleX, scaleY);
        _matrix.setScale(scale, scale);
        if (scaleX < scaleY) {
            _matrix.postTranslate(0, (height - oheight * scale) * 0.5f);
        } else {
            _matrix.postTranslate((width - owidth * scale) * 0.5f, 0);
        }
    }

    void Renderer::draw() {
        if (!surface) {
            return;
        }
        if (canvas == nullptr) {
            auto s = surface->getSurface();
            if (s != nullptr) {
                canvas = s->getCanvas();
                updateMatrix();
            } else {
                return;
            }
        }
        surface->enter();
        if (_matrix.getScaleX() != 0 && _matrix.getScaleY() != 0) {
            canvas->setMatrix(_matrix);
        }
        canvas->drawColor(SK_ColorCYAN);
        SkPaint p;
        p.setTextSize(30);
        p.setStrokeWidth(2.0);
        p.setAntiAlias(false);
        p.setTypeface(_typeface());
        std::stringstream ss;
        ss << "draw" << count;
        std::string res = ss.str();
        canvas->drawText(res.c_str(), res.length(), 0, 30, p);
        SkRect r = {50, 50, 80, 80};
        p.setColor(SK_ColorLTGRAY);
        canvas->drawRect(r, p);
        surface->exit();
        count += 1;
    }

    bool Renderer::flush() {
        if (!surface) {
            return false;
        }
        draw();
        return surface->present();
    }
}
