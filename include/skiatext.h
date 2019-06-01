#ifndef SKIA_TEXT_H
#define SKIA_TEXT_H

#include <skia.h>
namespace skiatext {
	class Surface {
    public:
        virtual ~Surface();

        void enter();

        void exit();

        bool present();

        int width() const {
            return _width;
        }

        int height() const {
            return _height;
        }

        void updateSize();

    protected:
        int _width = 0;
        int _height = 0;

        virtual void onEnter() = 0;

        virtual void onExit()  = 0;

        virtual void onPresent() = 0;

        virtual SkSurface* onCreateSurface() = 0;

        virtual void onUpdateSize() = 0;

    private:
        SkSurface* surface = nullptr;

        SkSurface* getSurface();

        friend class Renderer;
    };

    class Renderer {
    public:
        explicit Renderer();

        ~Renderer();

        void updateMatrix();
        
        void setSurface(Surface* surface);

        void draw();

        bool flush();

    private:
        Surface* surface = nullptr;
        SkMatrix _matrix = {};
        SkCanvas* canvas = nullptr;
        int count = 0;
    };
}

#endif //PAG_PAG_SKIA_H
