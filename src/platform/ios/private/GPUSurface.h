#import <skiatext.h>

class GPUSurface : public skiatext::Surface {
public:
    GPUSurface(CAEAGLLayer* layer);

    ~GPUSurface() override;

protected:

    void onEnter() override;

    void onExit() override;

    void onPresent() override;

    SkSurface* onCreateSurface() override;

    void onUpdateSize() override;

private:
    // for GPU rendering
    GrContext *grContext = nullptr;
    EAGLContext *eaglContext;
    GLuint framebuffer = 0;
    GLuint depthStencilRenderbuffer = 0;
    GLuint colorBuffer = 0;
    // for EAGLLayer
    CAEAGLLayer* layer = nil;

    void tearDownSurface();

    void setupColorAttachmentFromLayer(EAGLContext* context);

    void setupColorAttachmentFromPixel(EAGLContext* context);
};
