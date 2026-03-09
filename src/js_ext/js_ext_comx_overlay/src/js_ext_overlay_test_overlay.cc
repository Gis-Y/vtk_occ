#define _NO_BASETYPES

#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstdio> // 引入 sprintf

#include <gl/gl_overlay.hxx>

using namespace std;

enum OverlayShape {
    SHAPE_RECTANGLE = 0,
    SHAPE_ROUNDED_RECT = 1,
    SHAPE_ELLIPSE = 2,
    SHAPE_ARROW = 3,

    SHAPE_COLOR_BAR_V = 4, // 垂直颜色条
    SHAPE_COLOR_BAR_H = 5  // 水平颜色条
};

//////////////////////////////////////////////////////////////////////////////////////
// system pre-define segment

JS_EXT_DATA_DECLARE()

// 1. MyOverlay 类定义

class MyOverlay : public KMAS::Die_maker::comx::TGlDraggableOverlay {
private:
    int m_shapeType;
    float m_bgColor[4];
    float m_textColor[4];
    string m_text;
    int m_fontSize;
    int m_offsetX, m_offsetY;
    int m_worldX, m_worldY, m_worldZ;

    // 数值范围，用于颜色条显示
    double m_minValue = 0.0;
    double m_maxValue = 1.0;
    string m_unit = "";
    bool m_hasUserMoved;
    int m_lastUserX;
    int m_lastUserY;

    // [新增] 辅助判断：是否为颜色条
    bool IsColorBar() const {
        return m_shapeType == SHAPE_COLOR_BAR_V || m_shapeType == SHAPE_COLOR_BAR_H;
    }

public:
    MyOverlay(int x = 10, int y = 10, int width = 300, int height = 200)
        : KMAS::Die_maker::comx::TGlDraggableOverlay(x, y, width, height) {

        this->alpha_ = 0.0f; // 隐藏基类背景

        m_shapeType = SHAPE_RECTANGLE;

        // [默认] 普通形状背景：深灰色
        m_bgColor[0] = 0.2f; m_bgColor[1] = 0.2f; m_bgColor[2] = 0.2f; m_bgColor[3] = 0.8f;

        // [默认] 普通形状文本：亮黄色
        m_textColor[0] = 1.0f; m_textColor[1] = 1.0f; m_textColor[2] = 0.0f; m_textColor[3] = 1.0f;

        m_text = "Overlay Info";
        m_fontSize = 14;

        m_offsetX = x;
        m_offsetY = y;

        m_lastUserX = x;
        m_lastUserY = y;


        m_hasUserMoved = false;

        SetDragMode(DRAG_BOTH);
    }

    void SetRange(double min, double max, string unit) {
        m_minValue = min;
        m_maxValue = max;
        m_unit = unit;
    }

    // [关键修改] 设置形状时，如果是颜色条，自动应用全透明背景和白色字体
    void SetShape(int shape) {
        m_shapeType = shape;

        if (IsColorBar()) {
            // 颜色条配置：全透明背景，白色文字
            m_bgColor[0] = 0.0f; m_bgColor[1] = 0.0f; m_bgColor[2] = 0.0f; m_bgColor[3] = 0.0f; // Alpha = 0
            m_textColor[0] = 1.0f; m_textColor[1] = 1.0f; m_textColor[2] = 1.0f; m_textColor[3] = 1.0f; // White
        }
        else {
            // 切换回普通形状时，恢复默认深灰背景和黄色文字 (可选)
            // 如果不希望重置，可以注释掉下面这几行
            //m_bgColor[0] = 0.2f; m_bgColor[1] = 0.2f; m_bgColor[2] = 0.2f; m_bgColor[3] = 0.8f;
            //m_textColor[0] = 1.0f; m_textColor[1] = 1.0f; m_textColor[2] = 0.0f; m_textColor[3] = 1.0f;
        }
    }

    void SetSize(int w, int h) { this->width_ = w; this->height_ = h; this->heightMax_ = h; }
    void SetBackgroundColor(float r, float g, float b, float a) {
        m_bgColor[0] = r; m_bgColor[1] = g; m_bgColor[2] = b; m_bgColor[3] = a;
    }
    void SetTextContent(string text) { m_text = text; }
    void SetTextSize(int size) { if (size > 0) m_fontSize = size; }
    void SetTextColor(float r, float g, float b, float a) {
        m_textColor[0] = r; m_textColor[1] = g; m_textColor[2] = b; m_textColor[3] = a;
    }

    // [关键修改] SetPosition 逻辑分流
    void SetPosition(int x, int y, int z) {
        if (IsColorBar()) {
            // ===========================
            // 模式 A: 颜色条 (固定 UI)
            // ===========================
            // x, y 直接就是屏幕坐标，忽略 z
            this->x_ = x;
            this->y_ = y;
            this->original_x_ = x;
            this->original_y_ = y;

            setGeometry(x, y);

            // [重要] 标记为已初始化，阻止 paintGL 中的投影逻辑覆盖它
            this->position_initialized_ = true;
        }
        else {
            // ===========================
            // 模式 B: 普通悬浮窗 (3D 跟随)
            // ===========================
            // x, y, z 是世界坐标，需要投影
            m_worldX = x; m_worldY = y; m_worldZ = z;
            int wx = 0, wy = 0;
            dglProject(x, y, z, wx, wy);

            if (m_shapeType == SHAPE_ARROW) {
                m_offsetX = wx - width_;
                m_offsetY = wy - (height_ / 2);
            }
            else {
                m_offsetX = wx;
                m_offsetY = wy;
            }
            setGeometry(m_offsetX, m_offsetY);

            // 3D 模式下，允许后续 paintGL 自动更新位置
            this->position_initialized_ = false;
            this->original_x_ = m_offsetX;
            this->original_y_ = m_offsetY;
        }
    }

    void initializeGL() {}
    bool isUtf8Start(char c) { return (c & 0xC0) != 0x80; }
    bool isNewline(char c) { return c == '\n'; }

private:
    void GetHeatMapColor(float value, float& r, float& g, float& b) {
        const int NUM_COLORS = 5;
        static float color[NUM_COLORS][3] = {
            {0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0} };

        float idx1, idx2, fractBetween = 0;
        if (value <= 0) { idx1 = idx2 = 0; }
        else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }
        else {
            value = value * (NUM_COLORS - 1);
            idx1 = floor(value);
            idx2 = idx1 + 1;
            fractBetween = value - float(idx1);
        }

        int i1 = (int)idx1;
        int i2 = (int)idx2;
        r = (color[i2][0] - color[i1][0]) * fractBetween + color[i1][0];
        g = (color[i2][1] - color[i1][1]) * fractBetween + color[i1][1];
        b = (color[i2][2] - color[i1][2]) * fractBetween + color[i1][2];
    }

public:
    void paintGL() {
        this->alpha_ = 0.0f;

        // [关键修改] 仅对普通形状（非颜色条）执行位置同步逻辑
        if (!IsColorBar()) {
            int wx = 0, wy = 0;
            dglProject(m_worldX, m_worldY, m_worldZ, wx, wy);

            // [修改建议] 只要投影成功，就更新位置。
            // 如果你的模型离摄像机很近或很远，坐标可能会很大，
            // 建议保留这个范围检查，或者针对箭头适当放宽范围。
            if (wx > -5000000 && wx < 5000000 && wy > -50000 && wy < 50000) {

                if (m_shapeType == SHAPE_ARROW) {
                    // 箭头的热点位置修正（保持原样即可）
                    m_offsetX = wx - width_;
                    m_offsetY = wy - (height_ / 2);
                }
                else {
                    m_offsetX = wx;
                    m_offsetY = wy;
                }

                // 应用计算出的屏幕坐标
                setGeometry(m_offsetX, m_offsetY);

                this->position_initialized_ = false;
                this->original_x_ = m_offsetX;
                this->original_y_ = m_offsetY;
            
            }
        }
        // 如果是颜色条，跳过上面的同步，位置一直保持 SetPosition 设定的屏幕坐标

        // 2. 设置 OpenGL 状态
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (IsColorBar()) {
            DrawColorBar(); // 绘制颜色条
        }
        else {
            // ==========================================
            // 绘制普通形状 (保留原逻辑，不要动)
            // ==========================================
            glColor4f(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);

            float headWidth = 0.0f;
            if (m_shapeType == SHAPE_ARROW) {
                headWidth = (float)height_ * 0.6f;
                if (headWidth > width_ * 0.5f) headWidth = width_ * 0.5f;
            }

            if (m_shapeType == SHAPE_RECTANGLE) {
                dglBegin(GL_QUADS);
                dglVertex(0, 0); dglVertex(width_, 0); dglVertex(width_, height_); dglVertex(0, height_);
                dglEnd();
            }
            else if (m_shapeType == SHAPE_ELLIPSE) {
                dglBegin(GL_POLYGON);
                float cx = width_ / 2.0f; float cy = height_ / 2.0f;
                float rx = width_ / 2.0f; float ry = height_ / 2.0f;
                for (int i = 0; i < 360; i += 10) {
                    float theta = i * 3.14159f / 180.0f;
                    dglVertex((int)(cx + rx * cos(theta)), (int)(cy + ry * sin(theta)));
                }
                dglEnd();
            }
            else if (m_shapeType == SHAPE_ARROW) {
                float bodyWidth = width_ - headWidth;
                dglBegin(GL_POLYGON);
                dglVertex(0, 0); dglVertex(0, height_); dglVertex((int)bodyWidth, height_);
                dglVertex(width_, height_ / 2); dglVertex((int)bodyWidth, 0);
                dglEnd();
            }

            // 绘制边框
            glLineWidth(1.0f);
            glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
            if (m_shapeType == SHAPE_ARROW) {
                float bodyWidth = width_ - headWidth;
                dglBegin(GL_LINE_LOOP);
                dglVertex(0, 0); dglVertex(0, height_); dglVertex((int)bodyWidth, height_);
                dglVertex(width_, height_ / 2); dglVertex((int)bodyWidth, 0);
                dglEnd();
            }
            else if (m_shapeType == SHAPE_RECTANGLE) {
                dglBegin(GL_LINE_LOOP);
                dglVertex(0, 0); dglVertex(width_, 0); dglVertex(width_, height_); dglVertex(0, height_);
                dglEnd();
            }
            glPopAttrib();

            // 4. 绘制普通文本
            glColor4f(m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
            int paddingLeft = 10;
            int paddingRight = 10;
            if (m_shapeType == SHAPE_ARROW) paddingRight = (int)headWidth + 5;

            int maxTextWidth = width_ - paddingLeft - paddingRight;
            if (maxTextWidth < 10) maxTextWidth = 10;

            vector<string> lines;
            string currentLine = "";
            for (size_t i = 0; i < m_text.length(); ++i) {
                char c = m_text[i];
                if (c == '\n') { lines.push_back(currentLine); currentLine = ""; continue; }
                string nextChar = ""; nextChar += c;
                int j = i + 1;
                while (j < m_text.length() && !isUtf8Start(m_text[j])) nextChar += m_text[j++];
                i = j - 1;
                string testLine = currentLine + nextChar;
                int w = dglTextOutputWidth(testLine.c_str(), m_fontSize);
                if (w > maxTextWidth) {
                    if (!currentLine.empty()) { lines.push_back(currentLine); currentLine = nextChar; }
                    else { lines.push_back(testLine); currentLine = ""; }
                }
                else { currentLine = testLine; }
            }
            if (!currentLine.empty()) lines.push_back(currentLine);

            int lineCount = (int)lines.size();
            int lineSpacing = 2;
            int lineHeight = m_fontSize + lineSpacing;
            int totalTextHeight = lineCount * m_fontSize + (lineCount - 1) * lineSpacing;
            int startY = (height_ - totalTextHeight) / 2;
            if (lineCount == 1) startY = (height_ - m_fontSize) / 2;

            for (int k = 0; k < lineCount; ++k) {
                int drawY = startY + k * lineHeight;
                dglTextOutput(lines[k].c_str(), paddingLeft, drawY, m_fontSize);
            }
        }
    }

    // [关键修改] isVisible 分流逻辑
   // [修改] isVisible 分流逻辑
    bool isVisible() override {
        // 1. 如果是颜色条 或者 是箭头(SHAPE_ARROW)，强制一直显示
        //    这样它就不会因为视口检测逻辑而被系统隐藏
        if (IsColorBar() || m_shapeType == SHAPE_ARROW) {
            return true;
        }
        else {
            // 2. 普通形状的视口检测
            // [修复Bug] 原代码写的是 dglProject(0,0,0...)，这是不对的
            // 应该检测当前对象的实际坐标 m_worldX, m_worldY, m_worldZ
            int wx = 0, wy = 0;
            dglProject(m_worldX, m_worldY, m_worldZ, wx, wy);

            int sx, sy;
            dglGetWindowSize(sx, sy);

            // 只有当坐标在屏幕范围内才显示
            return !(wx < 0 || wx > sx || wy < 0 || wy > sy);
        }
    }

    bool isIgnoreMouse() override { return false; }
    bool mouseGL(TMouseEvent event) override { return false; }
    bool keyGL(TKeyEvent event) override { return false; }

    void DrawColorBar() {
        // =========================================================
        // 【关键修改 1】检测用户拖拽行为
        // =========================================================
        // is_dragging_ 是基类成员，当用户按住鼠标左键拖动时为 true
        // 一旦检测到拖动，就将 m_hasUserMoved 设为 true，永久停止自动吸附
        if (this->is_dragging_) {
            m_hasUserMoved = true;
            m_lastUserX = this->x_;
            m_lastUserY = this->y_;
        }

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // --- 1. 环境净化 ---
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // --- 2. 建立 2D 坐标系 ---
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        int sw = viewport[2];
        int sh = viewport[3];

        // --- 高度自适应逻辑 (始终执行，保证高度随画布变化) ---
        // 注意：如果你的类没有 heightMax_，请用 500 代替
        int maxSetHeight = this->heightMax_;
        int relativeHeight = (int)(sh * 0.7f);
        this->height_ = (relativeHeight < maxSetHeight) ? relativeHeight : maxSetHeight;

        // =========================================================
        // 【关键修改 2】位置控制逻辑
        // =========================================================

        // 只有在“用户从未移动过”的情况下，才强制吸附到右侧中间
        // 一旦用户动过，这里就不执行，x_ 和 y_ 就会保持在用户最后拖到的地方
        if (!m_hasUserMoved) {
            // [情况 A] 用户从未拖动过 -> 强制吸附在右侧中间
            this->x_ = sw - this->width_ - 20;
            this->y_ = (sh - this->height_) / 2;
        }
        else {
            if (!this->is_dragging_) {
                this->x_ = m_lastUserX;
                this->y_ = m_lastUserY;
            }

            // --- 边界检查 (防止窗口变小后跑出屏幕) ---

            // 1. 防止跑出左边
            if (this->x_ < 0) this->x_ = 0;

            // 2. 防止跑出右边 (用新的 sw 计算)
            if (this->x_ + this->width_ > sw) this->x_ = sw - this->width_;

            // 3. 防止跑出上边
            if (this->y_ < 0) this->y_ = 0;

            // 4. 防止跑出下边 (用新的 sh 计算)
            if (this->y_ + this->height_ > sh) this->y_ = sh - this->height_;

            // --- 更新备份 ---
            // 如果边界检查修正了坐标，也要更新备份，否则下次又会跑出去
            m_lastUserX = this->x_;
            m_lastUserY = this->y_;
        }

        // 注意：如果用户改了窗口大小(sh变了)，height_会变。
        // 如果用户已经拖动过位置 (m_hasUserMoved为true)，y_ 不会变，
        // 颜色条会以当前的顶部 y_ 为基准向下伸缩，符合操作直觉。

        // =========================================================

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, viewport[2], viewport[3], 0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // --- 3. 绘制几何图形 ---

        // 背景框 (保持半透明)
        glColor4f(0.2f, 0.2f, 0.2f, alpha_);
        glBegin(GL_QUADS);
        glVertex2i(x_, y_);
        glVertex2i(x_ + width_, y_);
        glVertex2i(x_ + width_, y_ + height_);
        glVertex2i(x_, y_ + height_);
        glEnd();

        int padding = 10;
        int headerHeight = m_fontSize * 3 + 20;

        // 颜色条位置
        int barX = x_ + padding;
        int barWidth = 30;
        int barTopY = y_ + padding + headerHeight;
        int barBottomY = y_ + height_ - padding;
        int barHeight = barBottomY - barTopY;

        if (barHeight > 0) {
            int numBlocks = 10;
            float blockHeight = (float)barHeight / numBlocks;
            for (int i = 0; i < numBlocks; i++) {
                float colorRatio = 1.0f - (float)i / (numBlocks - 1);
                float r, g, b;
                GetHeatMapColor(colorRatio, r, g, b);

                // 色块半透明 (0.6f)
                glColor4f(r, g, b, 0.6f);
                int yTop = barTopY + (int)(i * blockHeight);
                int yBottom = barTopY + (int)((i + 1) * blockHeight);

                glBegin(GL_QUADS);
                glVertex2i(barX, yTop);
                glVertex2i(barX + barWidth, yTop);
                glVertex2i(barX + barWidth, yBottom);
                glVertex2i(barX, yBottom);
                glEnd();
            }
        }

        // --- 4. 恢复矩阵 ---
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // --- 5. 绘制文字 ---
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        char buf[128];

        dglTextOutput(m_text.c_str(), padding, padding, m_fontSize);

        sprintf(buf, "Max: %.3e", m_maxValue);
        dglTextOutput(buf, padding, padding + m_fontSize + 5, m_fontSize);
        sprintf(buf, "Min: %.3e", m_minValue);
        dglTextOutput(buf, padding, padding + (m_fontSize + 5) * 2, m_fontSize);

        /*if (barHeight > 0) {
            int numLabels = 11;
            for (int i = 0; i < numLabels; i++) {
                float ratio = (float)i / (numLabels - 1);
                double val = m_maxValue - (m_maxValue - m_minValue) * ratio;
                int labelRelY = (padding + headerHeight) + (int)(ratio * barHeight);
                sprintf(buf, "%.3e", val);
                dglTextOutput(buf, padding + barWidth + 8, labelRelY - m_fontSize / 2, m_fontSize);
            }
        }*/


        if (barHeight > 0) {
            int numLabels = 11;

            // =========================================================
            // 【新增防重叠逻辑】
            // =========================================================
            // 1. 定义文字所需的最小安全垂直间距（字体高度 + 4像素留白）
            float minSpacing = m_fontSize + 4.0f;

            // 2. 计算当前每个刻度分到的实际平均像素高度
            float currentSpacing = (float)barHeight / (numLabels - 1);

            // 3. 决定显示的步长
            int step = 1; // 默认每个都显示

            if (currentSpacing < minSpacing) {
                step = 2; // 空间不够，隔一个显示
            }
            if (currentSpacing * 2.0f < minSpacing) {
                step = 5; // 空间极度不够，只显示顶、中、底 (0, 5, 10)
            }
            // =========================================================

            for (int i = 0; i < numLabels; i++) {
                // 如果当前的索引不是 step 的倍数，直接跳过，不画文字
                if (i % step != 0) continue;

                float ratio = (float)i / (numLabels - 1);
                double val = m_maxValue - (m_maxValue - m_minValue) * ratio;

                // 计算文字相对于窗口顶部的偏移
                // headerHeight 已经在内了
                int labelRelY = (padding + headerHeight) + (int)(ratio * barHeight);

                sprintf(buf, "%.3e", val);
                // x 坐标偏移：色块宽(30) + 间隔(8)
                dglTextOutput(buf, padding + barWidth + 8, labelRelY - m_fontSize / 2, m_fontSize);
            }
        }

        // --- 6. 恢复现场 ---
        glPopAttrib();
    }
};

//////////////////////////////////////////////////////////////////////////////////////
// 2. 对象管理器 (保持不变)
class OverlayManager {
public:
    static std::map<uint64_t, MyOverlay*>& GetInstanceMap() {
        static std::map<uint64_t, MyOverlay*> instances;
        return instances;
    }
    static MyOverlay* GetOverlay(uint64_t id) {
        auto& map = GetInstanceMap();
        if (map.find(id) != map.end()) { return map[id]; }
        return nullptr;
    }
};

//////////////////////////////////////////////////////////////////////////////////////
// 3. 接口注册区域 (保持不变)

// --- SetColorPostBar ---
#define SETCOLORPOSTBAR_FUNC_USAGE \
    "setColorPostBar Usage: setColorPostBar(id, min, max, unit);"
JS_EXT_FUNC_BEGIN(setColorPostBar, 4, SETCOLORPOSTBAR_FUNC_USAGE)
{
    string strId = JS_EXT_PARA(string, 0);
    double min = JS_EXT_PARA(double, 1);
    double max = JS_EXT_PARA(double, 2);
    string unit = JS_EXT_PARA(string, 3);

    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetRange(min, max, unit);

    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- CreateOverlay ---
#define CREATE_FUNC_USAGE "Usage: var id = overlay_test.overlay.CreateOverlay();"
JS_EXT_FUNC_BEGIN(CreateOverlay, 0, CREATE_FUNC_USAGE) {
    MyOverlay* p_overlay = new MyOverlay();
    uint64_t id = (uint64_t)((void*)p_overlay);
    OverlayManager::GetInstanceMap()[id] = p_overlay;
    string strId = to_string(id);
    JS_EXT_FUNC_ASSIGN_RET(strId);
}
JS_EXT_FUNC_END()

// --- GetEntry ---
#define GET_ENTRY_USAGE "Usage: var entry = overlay_test.overlay.GetEntry(id);"
JS_EXT_FUNC_BEGIN(GetEntry, 1, GET_ENTRY_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    uint64_t id = std::stoull(strId);
    MyOverlay* p = OverlayManager::GetOverlay(id);
    if (!p) { JS_EXT_FUNC_ASSIGN_RET(string("0")); }
    KMAS::Die_maker::comx::IGlContentRender* p_entry = p;
    uint64_t ullEntry = (uint64_t)((void*)p_entry);
    string strEntry = to_string(ullEntry);
    JS_EXT_FUNC_ASSIGN_RET(strEntry);
}
JS_EXT_FUNC_END()

// --- DestroyOverlay ---
#define DESTROY_USAGE "Usage: DestroyOverlay(id);"
JS_EXT_FUNC_BEGIN(DestroyOverlay, 1, DESTROY_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    uint64_t id = std::stoull(strId);
    auto& map = OverlayManager::GetInstanceMap();
    if (map.find(id) != map.end()) {
        MyOverlay* p = map[id];
        delete p;
        map.erase(id);
    }
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetShape ---
#define SET_SHAPE_USAGE "Usage: SetShape(id, shapeType);"
JS_EXT_FUNC_BEGIN(SetShape, 2, SET_SHAPE_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    int type = JS_EXT_PARA(int, 1);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetShape(type);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetSize ---
#define SET_SIZE_USAGE "Usage: SetSize(id, w, h);"
JS_EXT_FUNC_BEGIN(SetSize, 3, SET_SIZE_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    int w = JS_EXT_PARA(int, 1);
    int h = JS_EXT_PARA(int, 2);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetSize(w, h);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetBackgroundColor ---
#define SET_BG_COLOR_USAGE "Usage: SetBackgroundColor(id, r, g, b, a);"
JS_EXT_FUNC_BEGIN(SetBackgroundColor, 5, SET_BG_COLOR_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    float r = JS_EXT_PARA(float, 1);
    float g = JS_EXT_PARA(float, 2);
    float b = JS_EXT_PARA(float, 3);
    float a = JS_EXT_PARA(float, 4);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetBackgroundColor(r, g, b, a);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetText ---
#define SET_TEXT_USAGE "Usage: SetText(id, text);"
JS_EXT_FUNC_BEGIN(SetText, 2, SET_TEXT_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    string text = JS_EXT_PARA(string, 1);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetTextContent(text);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetTextSize ---
#define SET_TEXT_SIZE_USAGE "Usage: SetTextSize(id, size);"
JS_EXT_FUNC_BEGIN(SetTextSize, 2, SET_TEXT_SIZE_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    int size = JS_EXT_PARA(int, 1);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetTextSize(size);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetTextColor ---
#define SET_TEXT_COLOR_USAGE "Usage: SetTextColor(id, r, g, b, a);"
JS_EXT_FUNC_BEGIN(SetTextColor, 5, SET_TEXT_COLOR_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    float r = JS_EXT_PARA(float, 1);
    float g = JS_EXT_PARA(float, 2);
    float b = JS_EXT_PARA(float, 3);
    float a = JS_EXT_PARA(float, 4);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetTextColor(r, g, b, a);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

// --- SetPosition ---
#define SET_POS_USAGE "Usage: SetPosition(id, x, y);"
JS_EXT_FUNC_BEGIN(SetPosition, 4, SET_POS_USAGE) {
    string strId = JS_EXT_PARA(string, 0);
    int x = JS_EXT_PARA(int, 1);
    int y = JS_EXT_PARA(int, 2);
    int z = JS_EXT_PARA(int, 3);
    MyOverlay* p = OverlayManager::GetOverlay(std::stoull(strId));
    if (p) p->SetPosition(x, y, z);
    string res = "ok";
    JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////////////////

#define JS_EXT_NS "overlay_test.overlay"

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(setColorPostBar)
JS_EXT_ENTRY(CreateOverlay)
JS_EXT_ENTRY(GetEntry)
JS_EXT_ENTRY(DestroyOverlay)
JS_EXT_ENTRY(SetShape)
JS_EXT_ENTRY(SetSize)
JS_EXT_ENTRY(SetBackgroundColor)
JS_EXT_ENTRY(SetText)
JS_EXT_ENTRY(SetTextSize)
JS_EXT_ENTRY(SetTextColor)
JS_EXT_ENTRY(SetPosition)
JS_EXT_ENTRY_END()

// [关键修复]：将 2 改回 1
JS_EXT_MAIN_BEGIN(JS_EXT_NS, 1)
JS_EXT_FUNC_REG(setColorPostBar)
JS_EXT_FUNC_REG(CreateOverlay)
JS_EXT_FUNC_REG(GetEntry)
JS_EXT_FUNC_REG(DestroyOverlay)
JS_EXT_FUNC_REG(SetShape)
JS_EXT_FUNC_REG(SetSize)
JS_EXT_FUNC_REG(SetBackgroundColor)
JS_EXT_FUNC_REG(SetText)
JS_EXT_FUNC_REG(SetTextSize)
JS_EXT_FUNC_REG(SetTextColor)
JS_EXT_FUNC_REG(SetPosition)
JS_EXT_MAIN_END()