#ifndef GL_OVERLAY_HXX_0CE4416214724128B8A7FE62D60DFF33
#define GL_OVERLAY_HXX_0CE4416214724128B8A7FE62D60DFF33

#include "gl_content.hxx"
#include "gl_data_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

    class TGlOverlayWindow : public IGlContentRenderEx,
        public IEventSourceContainer
    {
    public:
        TGlOverlayWindow() : _is_begin(false), INIT_EXPORT_INTERFACE(IGlText){}
    public:
        virtual TStatus STDCALL PaintGL() = 0;
        virtual TStatus STDCALL MouseGL(TMouseEvent event) = 0;
        virtual TStatus STDCALL InitializeGL() = 0;
        virtual TStatus STDCALL KeyGL(TKeyEvent event) = 0;
    protected:
        void dglUnProject(int x, int y, float &fX, float &fY, float &fZ) {
            GLdouble modelMatrix[16], projMatrix[16];
            GLint viewport[4];
            GLfloat winZ = 0.0f;
            GLdouble objX, objY, objZ;

            // ��ȡOpenGL����״̬
            glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
            glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
            glGetIntegerv(GL_VIEWPORT, viewport);

            // ִ������ת��
            gluUnProject(
                x, viewport[3] - y, winZ, // ע��Y���귭ת
                modelMatrix, projMatrix, viewport,
                &objX, &objY, &objZ
            );

            fX = static_cast<float>(objX);
            fY = static_cast<float>(objY);
            fZ = static_cast<float>(objZ);
        }
    protected:
        void dglTextOutput(const string &str, int x, int y, int nFontSize, const char *szFont = NULL)
        {
            if (m_pIGlText)
            {
                IGlTextEx *p_gl_text_ex = NULL;
                m_pIGlText->QueryInterface(IID_IGlTextEx, (void**)&p_gl_text_ex);

                y += nFontSize;

                float fX = 0.0f, fY = 0.0f, fZ = 0.0f;
                dglUnProject(x, y, fX, fY, fZ);

                TGlTextInfoEx ti_ex[] = { { nFontSize, fX, fY, fZ, NULL } };
                strcpy(ti_ex[0].szText, str.c_str());

                p_gl_text_ex->TextOutEx(ti_ex, 1, szFont);

                p_gl_text_ex->Release();
            }
        }
        int dglTextOutputWidth(const string &str, int nFontSize, const char *szFont = NULL)
        {
            int nRetWidth = 0;

            if (m_pIGlText)
            {
                IGlTextEx *p_gl_text_ex = NULL;
                m_pIGlText->QueryInterface(IID_IGlTextEx, (void**)&p_gl_text_ex);

                p_gl_text_ex->GetTextWidth(str.c_str(), nFontSize, szFont, nRetWidth);

                p_gl_text_ex->Release();

            }

            return nRetWidth;
        }

        void dglProject(double x, double y, double z, int &wx, int&wy)
        {
            if (m_pIGlText)
            {
                IGlUtils *p_gl_utils = NULL;
                m_pIGlText->QueryInterface(IID_IGlUtils, (void**)&p_gl_utils);

                p_gl_utils->ProjectModelPoint(x,y, z, wx, wy);

                p_gl_utils->Release();
            }
        }

        void dglGetWindowSize(int &w, int &h)
        {
            GLint viewport[4];
            // ��ȡ�ӿڲ���
            glGetIntegerv(GL_VIEWPORT, viewport);

            w = viewport[2];
            h = viewport[3];
        }
    protected:
        void dglBegin(GLenum mode) {
            _mode = mode;
            _is_begin = true;
        }
        void dglEnd()
        {
            if (_is_begin)
            {
                glBegin(_mode);
                for (size_t i = 0; i < _vertexes.size() / 3; i++)
                {
                    glVertex3fv(&_vertexes[i * 3]);
                }
                glEnd();
                _is_begin = false;
                _vertexes.clear();
            }
        }
        void dglVertex(int x, int y)
        {
            float fX = 0.0f, fY = 0.0f, fZ = 0.0f;
            dglUnProject(x, y, fX, fY, fZ);

            //glVertex3f(fX, fY, fZ);
            _vertexes.push_back(fX);
            _vertexes.push_back(fY);
            _vertexes.push_back(fZ);
        }
    private:
        GLenum _mode;
        vector<GLfloat> _vertexes;
        bool _is_begin;
        bool _is_visible;
    public:
        BEGIN_IMPLEMENT_NO_COUNT_ROOT()
            IMPLEMENT_INTERFACE(IGlContentRender)
            IMPLEMENT_INTERFACE_ROOT(IGlContentRender)
            IMPLEMENT_INTERFACE(IGlContentRenderEx)
            IMPLEMENT_INTERFACE(IEventSourceContainer)
        END_IMPLEMENT_ROOT()
    public:
        BEGIN_IMPLEMENT_EVENT_SOURCE_CONTAINER()
            IMPLEMENT_EVENT_SOURCE_CONTAINER(IGlText)
        END_IMPLEMENT_EVENT_SOURCE_CONTAINER()
    public:
        IMPLEMENT_EVENT_SOURCE(TGlOverlayWindow, IGlText)
    };

    class TGlDraggableOverlay : public KMAS::Die_maker::comx::TGlOverlayWindow {
    public:
        // �������϶�ģʽö��
        enum DragMode {
            DRAG_NONE = 0,
            DRAG_HORIZONTAL = 1,
            DRAG_VERTICAL = 2,
            DRAG_BOTH = 3
        };

    protected:
        int x_;
        int y_;
        int width_;
        int height_;
        int heightMax_;
        float alpha_;

        int original_x_;
        int original_y_;

        bool is_dragging_ = false;
        int drag_start_x_ = 0;
        int drag_start_y_ = 0;

        int screen_width_ = 0;
        int screen_height_ = 0;

        bool position_initialized_ = false;

        // ��������������
        bool horizontal_center_ = false;
        bool vertical_center_ = false;

        DragMode drag_mode_ = DRAG_BOTH;
    public:
        // �޸ĺ�Ĺ��캯������Ӷ�����϶�ģʽ����
        TGlDraggableOverlay(int x = 100, int y = 100, int width = 300,
            int height = 200, float alpha = 0.2f,
            bool horizontal_center = false,
            bool vertical_center = false,
            DragMode drag_mode = DRAG_BOTH)
            : original_x_(x), original_y_(y), x_(x), y_(y), width_(width),
            height_(height), alpha_(alpha),
            horizontal_center_(horizontal_center),
            vertical_center_(vertical_center), drag_mode_(drag_mode){
            if (alpha_ < 0.0f)
                alpha_ = 0.0f;
            else if (alpha_ > 1.0f)
                alpha_ = 1.0f;
        }

        // ���������ö��뷽ʽ�ķ���
        void SetAlignment(bool horizontal_center, bool vertical_center) {
            horizontal_center_ = horizontal_center;
            vertical_center_ = vertical_center;
            position_initialized_ = false; // ��Ҫ���¼���λ��
        }

        // �����������϶�ģʽ�ķ���
        void SetDragMode(DragMode mode) { drag_mode_ = mode; }
    protected:
        void setGeometry(int x, int y, int width = -1, int height = -1)
        {
            x_ = x;
            y_ = y;

            if(width != -1)width_ = width;
            if(height != -1)height_ = height;
        }
        void getGeometry(int &x, int &y, int &width, int &height)
        {
            x = x_;
            y = y_;
            width = width_;
            height = height_;
        }
    private:
        void ProcessNegativePosition() {
            if (position_initialized_)
                return;

            UpdateWindowSize();

            // ����ˮƽλ��
            if (horizontal_center_) {
                x_ = (screen_width_ - width_) / 2;
            }
            else if (original_x_ < 0) {
                x_ = screen_width_ + original_x_ - width_;
            }
            else {
                x_ = original_x_;
            }

            // �����ֱλ��
            if (vertical_center_) {
                y_ = (screen_height_ - height_) / 2;
            }
            else if (original_y_ < 0) {
                y_ = screen_height_ + original_y_ - height_;
            }
            else {
                y_ = original_y_;
            }

            ConstrainWindowPosition();
            position_initialized_ = true;
        }
    protected:
        void dglVertex(int x, int y) {
            if (x < 0) {
                x += width_;
            }

            if (y < 0) {
                y += height_;
            }
            KMAS::Die_maker::comx::TGlOverlayWindow::dglVertex(x_ + x,
                y_ + y);
        }

        void dglTextOutput(const char *szText, int x, int y, int fontSize,
            const char *szFont = NULL) {
            if (x < 0) {
                int nTextWidth =
                    dglTextOutputWidth(szText, fontSize, szFont);
                x += width_;
                x -= nTextWidth;
            }

            if (y < 0) {
                y += height_;
                y -= fontSize;
            }
            KMAS::Die_maker::comx::TGlOverlayWindow::dglTextOutput(
                szText, x_ + x, y_ + y, fontSize, szFont);
        }

    public:
        virtual void initializeGL() = 0;
        virtual void paintGL() = 0;
        virtual bool mouseGL(TMouseEvent event) = 0;
        virtual bool keyGL(TKeyEvent event) = 0;
    public:
        virtual bool isVisible() {
            return true;
        }
        virtual bool isIgnoreMouse() { return false; }
    public:
        TStatus STDCALL InitializeGL() override {
            initializeGL();
            return M_OK;
        }

        TStatus STDCALL PaintGL() override {
            if (!isVisible()) { return M_OK; }

            if (!position_initialized_) {
                ProcessNegativePosition();
            }

            UpdateWindowSize();
            ConstrainWindowPosition();

            GLboolean blendEnabled = glIsEnabled(GL_BLEND);
            GLint blendSrc, blendDst;
            glGetIntegerv(GL_BLEND_SRC, &blendSrc);
            glGetIntegerv(GL_BLEND_DST, &blendDst);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glColor4f(0.2f, 0.2f, 0.2f, alpha_);
            dglBegin(GL_QUADS);
            {
                dglVertex(0, 0);
                dglVertex(width_, 0);
                dglVertex(width_, height_);
                dglVertex(0, height_);
            }
            dglEnd();

            float borderAlpha = alpha_ * 3.0f;
            if (borderAlpha > 1.0f)
                borderAlpha = 1.0f;
            glColor4f(0.9f, 0.9f, 0.9f, borderAlpha);
            dglBegin(GL_LINE_LOOP);
            {
                dglVertex(0, 0);
                dglVertex(width_, 0);
                dglVertex(width_, height_);
                dglVertex(0, height_);
            }
            dglEnd();

            paintGL();

            if (!blendEnabled) {
                glDisable(GL_BLEND);
            }
            else {
                glBlendFunc(blendSrc, blendDst);
            }

            return M_OK;
        }

        TStatus STDCALL MouseGL(TMouseEvent event) override {
            
            const int mouse_x = event.X();
            const int mouse_y = event.Y();
            const bool in_window = IsPointInWindow(mouse_x, mouse_y);

            if (!in_window && !is_dragging_ || isIgnoreMouse())
            {
                return M_FAIL;
            }

            // �����϶�ģʽֱ�ӷ���
            if (drag_mode_ == DRAG_NONE) {
                return M_OK;
            }

            if (mouseGL(event))
                return M_OK;

            switch (event.Action()) {
            case TMouseEvent::LPRESS:
                if (in_window && (event.Flags() & 0x1)) {
                    is_dragging_ = true;
                    drag_start_x_ = mouse_x - x_;
                    drag_start_y_ = mouse_y - y_;
                }
                break;

            case TMouseEvent::LRELEASE:
                if (is_dragging_) {
                    is_dragging_ = false;
                }
                break;

            case TMouseEvent::MOTION:
                if (is_dragging_ && (event.Flags() & 0x1)) {
                    // �����϶�ģʽ����λ��
                    if (drag_mode_ == DRAG_BOTH ||
                        drag_mode_ == DRAG_HORIZONTAL) {
                        x_ = mouse_x - drag_start_x_;
                    }
                    if (drag_mode_ == DRAG_BOTH ||
                        drag_mode_ == DRAG_VERTICAL) {
                        y_ = mouse_y - drag_start_y_;
                    }
                    ConstrainWindowPosition();
                }
                break;
            }

            return M_OK;
        }

        virtual TStatus STDCALL KeyGL(TKeyEvent event)
        {
            const int mouse_x = event.X();
            const int mouse_y = event.Y();
            const bool in_window = IsPointInWindow(mouse_x, mouse_y);

            if (!in_window)
            {
                return M_FAIL;
            }

            return keyGL(event) ? M_OK : M_FAIL;
        }

    private:
        bool IsPointInWindow(int px, int py) const {
            return (px >= x_) && (px <= x_ + width_) && (py >= y_) &&
                (py <= y_ + height_);
        }

        void ConstrainWindowPosition() {
            if (x_ < 0)
                x_ = 5;

            if (y_ < 0)
                y_ = 5;

            if (x_ + width_ > screen_width_)
                x_ = screen_width_ - width_ - 5;

            if (y_ + height_ > screen_height_)
                y_ = screen_height_ - height_ - 5;
        }

        bool UpdateWindowSize() {
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);

            bool bResize = !(screen_width_ == viewport[2] &&
                screen_height_ == viewport[3]);
            screen_width_ = viewport[2];
            screen_height_ = viewport[3];

            if (bResize) {
                // ���¼���λ����ά�ֳ�ʼ��ϵ
                if (horizontal_center_) {
                    x_ = (screen_width_ - width_) / 2;
                }
                else if (original_x_ < 0) {
                    x_ = screen_width_ + original_x_ - width_;
                }
                else {
                    x_ = original_x_;
                }

                if (vertical_center_) {
                    y_ = (screen_height_ - height_) / 2;
                }
                else if (original_y_ < 0) {
                    y_ = screen_height_ + original_y_ - height_;
                }
                else {
                    y_ = original_y_;
                }
            }

            return bResize;
        }
    };
}
}
}

#endif // !GL_OVERLAY_HXX_0CE4416214724128B8A7FE62D60DFF33
