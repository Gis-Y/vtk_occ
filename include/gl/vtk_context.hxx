#ifndef E38A5E0EB868410A8B5E2CE2626E0C19
#define E38A5E0EB868410A8B5E2CE2626E0C19

#include "gl_mouse.hxx"

typedef void(*mouse_listen_fn_t)(TMouseEvent *event, void *inf);

struct IVtkContext {
        virtual bool Initialize(void *wID) = 0;
        virtual bool Resize() = 0;
        virtual bool Paint() = 0;
        virtual bool GetZBuffer(float *&zbuf) = 0;
        virtual bool GetRange(double &xmin, double &ymin, double &zmin,
                double &xmax, double &ymax, double &zmax) = 0;
        virtual bool GetRenderer(void *&renderer) = 0;
        virtual bool ReceiveMouseEvent(TMouseEvent *event) = 0;
        virtual bool RegisterMouseListenCallback(mouse_listen_fn_t cb) = 0;
        virtual bool GetRenderWindow(void *&renderWindow) = 0;
        virtual bool SetProperty(const char *name, void *prop) = 0;
        virtual bool GetProperty(const char *name, void *&prop) = 0;
};

struct IVtkMouseOperator {
        virtual bool HitTest(const int winX, const int winY, double &x, double &y, double &z, bool bAutoNearest = false) = 0;
        virtual bool HitTestNormal(const int winX, const int winY, double &nx, double &ny, double &nz) = 0;
};

#endif // !E38A5E0EB868410A8B5E2CE2626E0C19


