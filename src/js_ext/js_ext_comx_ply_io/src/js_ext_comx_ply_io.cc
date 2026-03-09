#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;
using namespace comx::napi;

#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib> // 用于 std::getenv

#include <gl/vtk_context.hxx>

using namespace std;

#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkWarpVector.h>
#include <vtkWindowToImageFilter.h>

#include < vtkBMPWriter.h>
#include <vtkCellData.h>
#include <vtkCellPicker.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkPNGWriter.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTriangle.h>
#include <vtkWindowToImageFilter.h>

#include "js_ext_comx_ply_io.h"
#include <cmath>
#include <dcip_ply_io.hxx>
#include <filesystem>
#include <fstream> //ifstream
#include <io.h>
#include <iostream>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <string> //包含getline()
#include <vector>
#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkArrowSource.h>
#include <vtkAssembly.h>
#include <vtkAxesActor.h>
#include <vtkCellDataToPointData.h>
#include <vtkClipClosedSurface.h>
#include <vtkClipPolyData.h>
#include <vtkColorTransferFunction.h>
#include <vtkConeSource.h>
#include <vtkContourFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkGlyph3DMapper.h>
#include <vtkLine.h>
#include <vtkLineSource.h>
#include <vtkLookupTable.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkOutputWindow.h>
#include <vtkPlaneCollection.h>
#include <vtkPlaneWidget.h>
#include <vtkResampleWithDataSet.h>
#include <vtkSTLReader.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarRepresentation.h>
#include <vtkScalarBarWidget.h>
#include <vtkStructuredGrid.h>
#include <vtkTextActor.h>
#include <vtkTextActor3D.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkTypeFloat64Array.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkCleanPolyData.h>
#include <vtkFillHolesFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkFillHolesFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkConnectivityFilter.h>
#include <vtkDataSetMapper.h>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVectorText.h>
#include <vtkFollower.h>
#include <sstream>
#include <iomanip>


/// <summary>
/// //////////////////////
/// #include <fstream>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
/// </summary>

#include "TriangleMeshIsoLine.hxx"
#include"ply/motai.hxx"

namespace fs = std::filesystem;

dcip_ply::triangle_part_t g_mesh;

#define RED 169
#define GREEN 166
#define BLUE 171

#define COLOR_NUM 512
double m_clrArray[COLOR_NUM * 3];

bool m_init = false;

double _max_value = 100;
double _min_value = 0;
int _grade_num = 512;


double signedLog(double x, double eps = 1e-12) {
        if (x > 0.0)
                return std::log(x);
        else if (x < 0.0)
                return -std::log(std::max(std::abs(x), eps));
        else
                return 0.0;
}

std::vector<string> splitAndConvertToInt(const std::string &input,
                                         char delimiter) {
        std::vector<string> result;
        std::stringstream ss(input);
        std::string token;

        // ʹ�� std::getline �ָ��ַ���
        while (std::getline(ss, token, delimiter)) {
                // ת��Ϊ���������ӵ� vector

                result.push_back(token);
        }

        return result;
}

void setExtremum(const double &max_value, const double &min_value) {
        _max_value = max_value;
        _min_value = min_value;
}
// 云图颜色
void _generateColorbar() {
        if (m_init)
                return;
        m_init = true;
        for (int i = 0; i < COLOR_NUM / 4; ++i) {
                double p = i / (COLOR_NUM / 4 - 1.0);
                /*double T */ p = (1 - cos(p * 3.1415926)) / 2.0;
                double Er = 1.0, Eg = 0.0, Eb = 0.0;
                double Fr = 1.0, Fg = 1.0, Fb = 0.0;
                m_clrArray[i * 3 + 0] = p * Fr + (1 - p) * Er;
                m_clrArray[i * 3 + 1] = p * Fg + (1 - p) * Eg;
                m_clrArray[i * 3 + 2] = p * Fb + (1 - p) * Eb;
        }

        for (int i = COLOR_NUM / 4; i < 2 * COLOR_NUM / 4; ++i) {
                double p = (i - COLOR_NUM / 4) / (COLOR_NUM / 4 - 1.0);
                /*double T */ p = (1 - cos(p * 3.1415926)) / 2.0;
                double Er = 1.0, Eg = 1.0, Eb = 0.0;
                double Fr = 0.0, Fg = 1.0, Fb = 0.0;
                m_clrArray[i * 3 + 0] = p * Fr + (1 - p) * Er;
                m_clrArray[i * 3 + 1] = p * Fg + (1 - p) * Eg;
                m_clrArray[i * 3 + 2] = p * Fb + (1 - p) * Eb;
        }

        for (int i = 2 * COLOR_NUM / 4; i < 3 * COLOR_NUM / 4; ++i) {
                double p = (i - 2 * COLOR_NUM / 4) / (COLOR_NUM / 4 - 1.0);
                /*double T */ p = (1 - cos(p * 3.1415926)) / 2.0;
                double Er = 0.0, Eg = 1.0, Eb = 0.0;
                double Fr = 0.0, Fg = 1.0, Fb = 1.0;
                m_clrArray[i * 3 + 0] = p * Fr + (1 - p) * Er;
                m_clrArray[i * 3 + 1] = p * Fg + (1 - p) * Eg;
                m_clrArray[i * 3 + 2] = p * Fb + (1 - p) * Eb;
        }

        for (int i = 3 * COLOR_NUM / 4; i < COLOR_NUM; ++i) {
                double p = (i - 3 * COLOR_NUM / 4) / (COLOR_NUM / 4 - 1.0);
                /*double T */ p = (1 - cos(p * 3.1415926)) / 2.0;
                double Er = 0.0, Eg = 1.0, Eb = 1.0;
                double Fr = 0.0, Fg = 0.0, Fb = 1.0;
                m_clrArray[i * 3 + 0] = p * Fr + (1 - p) * Er;
                m_clrArray[i * 3 + 1] = p * Fg + (1 - p) * Eg;
                m_clrArray[i * 3 + 2] = p * Fb + (1 - p) * Eb;
        }
}
// 得到云图颜色
void getColor(const double &value, double &red, double &green, double &blue) {
        double valStep = (_max_value - _min_value) / _grade_num;

        int cur_grade = static_cast<int>((value - _min_value) / valStep);

        if (cur_grade < 0) {
                cur_grade = 0;
        }

        if (cur_grade > (_grade_num - 1)) {
                cur_grade = (_grade_num - 1);
        }

        int clrStep = COLOR_NUM / (_grade_num - 1);
        int cur_color_grade = clrStep * cur_grade;

        if (cur_grade == (_grade_num - 1)) {
                cur_color_grade = COLOR_NUM - 1;
        }

        /*if (_is_inverse || _is_temperature)
        {
                cur_color_grade = COLOR_NUM - 1 - cur_color_grade;
        }*/

        if (fabs(_max_value - _min_value) < 1e-10) {
                cur_color_grade = _grade_num / 2;
        }

        red = m_clrArray[cur_color_grade * 3 + 0];
        green = m_clrArray[cur_color_grade * 3 + 1];
        blue = m_clrArray[cur_color_grade * 3 + 2];
}
// 云图实时剖切（未实现）
class BuildVTKWidgetCall : public vtkCommand {
      public:
        static BuildVTKWidgetCall *New() { return new BuildVTKWidgetCall; }

      public:
        virtual void Execute(vtkObject *caller, unsigned long eventId,
                             void *callData) {
                vtkPlaneWidget *pWidget =
                    reinterpret_cast<vtkPlaneWidget *>(caller);
                double center[3];
                pWidget->GetCenter(center);

                double normal[3];
                pWidget->GetNormal(normal);

                if (pWidget) {
                        if (isImage) {
                                pWidget->GetPlane(planeNew);

                                if (isVir) {
                                        vtkSmartPointer<vtkPolyDataMapper>
                                            coneMapper02 =
                                                vtkPolyDataMapper::New();
                                        coneMapper02->SetInputConnection(
                                            resampler->GetOutputPort());
                                        coneMapper02->ScalarVisibilityOff();
                                        vtkSmartPointer<vtkActor> actor02 =
                                            vtkSmartPointer<vtkActor>::New();
                                        actor02->SetMapper(coneMapper02);
                                        actor02->GetProperty()->SetColor(
                                            0.5, 0.5,
                                            0.5); // 设置polyData2的颜色
                                        actor02->GetProperty()->SetOpacity(
                                            0.03); // 设置polyData2的透明度为0.5
                                        renderer->AddActor(actor02);
                                } else {
                                        vtkSmartPointer<vtkPolyDataMapper>
                                            coneMapper02 =
                                                vtkPolyDataMapper::New();
                                        coneMapper02->SetInputConnection(NULL);
                                        coneMapper02->ScalarVisibilityOff();
                                        vtkSmartPointer<vtkActor> actor02 =
                                            vtkSmartPointer<vtkActor>::New();
                                        actor02->SetMapper(coneMapper02);
                                        actor02->GetProperty()->SetColor(
                                            0.5, 0.5,
                                            0.5); // 设置polyData2的颜色
                                        actor02->GetProperty()->SetOpacity(
                                            0.03); // 设置polyData2的透明度为0.5
                                        renderer->AddActor(actor02);
                                }

                                double normal[3];
                                planeNew->GetNormal(normal);
                                planeNew->SetNormal(-normal[0], -normal[1],
                                                    -normal[2]);

                                vtkPlanes->RemoveAllItems();
                                vtkPlanes->AddItem(planeNew);
                                clipper->SetInputConnection(
                                    resampler->GetOutputPort());
                                clipper->SetClippingPlanes(vtkPlanes);
                                clipper->SetActivePlaneId(1);
                                clipper->SetScalarModeToColors();
                                clipper->Update();

                                mapper->SetInputConnection(
                                    clipper->GetOutputPort());
                                // isPCall = true;
                        } else {
                                vtkPlanes->RemoveAllItems();
                                pWidget->GetPlane(planeNew);

                                vtkPlanes->AddItem(planeNew);
                                clipper->SetInputConnection(
                                    resampler->GetOutputPort());
                                clipper->SetClippingPlanes(vtkPlanes);
                                clipper->SetActivePlaneId(1);
                                clipper->SetScalarModeToColors();

                                if (isVir) {
                                        vtkSmartPointer<vtkPolyDataMapper>
                                            coneMapper02 =
                                                vtkPolyDataMapper::New();
                                        coneMapper02->SetInputConnection(
                                            resampler->GetOutputPort());
                                        coneMapper02->ScalarVisibilityOff();
                                        vtkSmartPointer<vtkActor> actor02 =
                                            vtkSmartPointer<vtkActor>::New();
                                        actor02->SetMapper(coneMapper02);
                                        actor02->GetProperty()->SetColor(
                                            0.5, 0.5,
                                            0.5); // 设置polyData2的颜色
                                        actor02->GetProperty()->SetOpacity(
                                            0.03); // 设置polyData2的透明度为0.5
                                        renderer->AddActor(actor02);
                                } else {
                                        vtkSmartPointer<vtkPolyDataMapper>
                                            coneMapper02 =
                                                vtkPolyDataMapper::New();
                                        coneMapper02->SetInputConnection(NULL);
                                        coneMapper02->ScalarVisibilityOff();
                                        vtkSmartPointer<vtkActor> actor02 =
                                            vtkSmartPointer<vtkActor>::New();
                                        actor02->SetMapper(coneMapper02);
                                        actor02->GetProperty()->SetColor(
                                            0.5, 0.5,
                                            0.5); // 设置polyData2的颜色
                                        actor02->GetProperty()->SetOpacity(
                                            0.03); // 设置polyData2的透明度为0.5
                                        renderer->AddActor(actor02);
                                }

                                // renderer->AddActor(actor02);
                                mapper->SetInputConnection(
                                    clipper->GetOutputPort());
                        }
                }
        }
        void setPlane(vtkSmartPointer<vtkPlane> other) { pPlane = other; }
        void setRenderer(vtkSmartPointer<vtkRenderer> other) {
                renderer = other;
        }
        void setMapper(vtkSmartPointer<vtkPolyDataMapper> other) {
                mapper = other;
        }

      private:
        vtkSmartPointer<vtkPolyData> clippedOutput =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkTextProperty> textCutterProperty =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor> textCutterActor =
            vtkSmartPointer<vtkTextActor>::New();
        vtkSmartPointer<vtkPlane> pPlane = vtkSmartPointer<vtkPlane>::New();
        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkPolyData> clipedData = vtkPolyData::New();
        vtkSmartPointer<vtkPlaneCollection> vtkPlanes =
            vtkSmartPointer<vtkPlaneCollection>::New();

      public:
        double minValue = -1000000;
        double maxValue = -1000000;
        bool isImage = false;
        bool isPCall = false;
        bool isVir = false;
        vtkSmartPointer<vtkResampleWithDataSet> resampler =
            vtkSmartPointer<vtkResampleWithDataSet>::New();
        vtkSmartPointer<vtkPolyDataMapper> coneMapper02 =
            vtkPolyDataMapper::New();
        vtkSmartPointer<vtkClipPolyData> cliper =
            vtkSmartPointer<vtkClipPolyData>::New();
        vtkSmartPointer<vtkClipClosedSurface> clipper =
            vtkSmartPointer<vtkClipClosedSurface>::New();
        vtkSmartPointer<vtkPlane> planeNew = vtkSmartPointer<vtkPlane>::New();
};

//******************************************************************************
// 任意点的类
struct Point3D {
        double x;
        double y;
        double z;
};
// 箭头1
struct ArrowFrame {
        string name;
        string selectPoint;
        double x, y, z;
        double xn, yn, zn;
        double size;
        vector<double> circle;
};
// 箭头2
struct arrow {
        string name;
        string mark;
        string selectPoint;
        string size;
        double x, y, z;
        double xn, yn, zn;
        double xr, yr, zr;
};
// 固定支撑
struct constrain {
        string name;
        string selectPoint;
        double x, y, z;
        int xr, yr, zr;
        int xn, yn, zn;
};
// 梁
struct beam {
        string name;
        vector<Point3D> pos;
        double radius;
        double len;
        double num;
        double M;
        double poi;
};
// 弹簧
struct spring {
        string name;
        string type1, type2;
        double stiffness_x, stiffness_y, stiffness_z;
        double startX, startY, startZ;
        double endX, endY, endZ;
};
// 连接
struct connects {
        string name;
        string mark1, mark2;
        double x1, y1, z1;
        double x2, y2, z2;
        int xn, yn, zn;
        int xr, yr, zr;
};
// rb3
struct RB3 {
        // int id;
        string type;
        vector<int> id;
        string name;
        vector<Point3D> cp;
        double x, y, z;
};
// 面的力（载荷-分布载荷）
struct forceFace {
        int selectFaceId;
        string size;
        int xn, yn, zn;
        vector<double> disface;
};
// 面上的固定支撑（约束-固定支撑-添加面）
struct fixFace {
        int selectFaceId;
        int xn, yn, zn;
        int xr, yr, zr;
};
// 高斯点
struct guass {
        int id;      // 编号
        Point3D pts; // 上面points结构体定义的点（x,y,z）
};
// 统一设置的颜色值
struct RGBS {
        double r, g, b;
};
// 目前未用
struct FACE {
        string sign;
        double centerPoints[3] = {0, 0, 0};
        int IdSelect;
};
// 前处理类的管理（目前未用）
class dataManage {
      public:
        vector<guass>
            pts; // 创建一个容器，pts的值只存储在<guass>容器里，取值也是根据对应的类取
        vector<ArrowFrame> af; // af的值只存储在<ArrowFrame>容器里
        vector<arrow> arr;
        vector<constrain> con;
        vector<beam> bm;
        vector<spring> sp;
        vector<connects> ct;
        vector<RB3> rb;
        vector<forceFace> fF;
        vector<fixFace> fA;
};
// 目前未用
struct DataSet {
        double x;
        double y;
        double z;
        double value;
};

//************************************************************************************
// 显示动力学后处理模块（collision）
class postCollision {
      public:
        int frame = 1;
        int fileCount = 0;
        string currentName;
        double range[2] = {-228.129, 63.7251};
        vector<double> sigma_xx;
        std::vector<vtkSmartPointer<vtkDataSetReader>> readerArray;
        string information;
        string attValue = "";
        string path = "";
        vtkSmartPointer<vtkDataSetMapper> mapper =
            vtkSmartPointer<vtkDataSetMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        vtkSmartPointer<vtkDataSetReader> reader =
            vtkSmartPointer<vtkDataSetReader>::New();
        vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
            vtkSmartPointer<vtkColorTransferFunction>::New();
        vtkSmartPointer<vtkColorTransferFunction> colorMap =
            vtkSmartPointer<vtkColorTransferFunction>::New();
        vtkSmartPointer<vtkAxesActor> Axes =
            vtkSmartPointer<vtkAxesActor>::New();
        vtkSmartPointer<vtkOrientationMarkerWidget> widget =
            vtkSmartPointer<vtkOrientationMarkerWidget>::New();
        vtkSmartPointer<vtkScalarBarActor> scalarBarActor =
            vtkSmartPointer<vtkScalarBarActor>::New();
        vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget =
            vtkSmartPointer<vtkScalarBarWidget>::New();
};
// 结构后处理模块
class postManage {
      public:
        vtkSmartPointer<vtkSTLReader> stlReader =
            vtkSmartPointer<vtkSTLReader>::New();
        vtkSmartPointer<vtkXMLStructuredGridReader> readers =
            vtkSmartPointer<vtkXMLStructuredGridReader>::New();
        vtkSmartPointer<vtkPolyData> targetData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkPointData> PointData =
            vtkSmartPointer<vtkPointData>::New();
        vtkSmartPointer<vtkStructuredGrid> structuredGrid =
            vtkSmartPointer<vtkStructuredGrid>::New();
        vtkSmartPointer<vtkPoints> pointsValue =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkDoubleArray> xData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> yData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> zData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> MagnitudeData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> strainXData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> strainYData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> strainZData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> strainData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> stressXData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> stressYData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> stressZData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> stressData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
            vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
        vtkSmartPointer<vtkResampleWithDataSet> resampler =
            vtkSmartPointer<vtkResampleWithDataSet>::New();
        vtkSmartPointer<vtkColorTransferFunction> colorMap =
            vtkSmartPointer<vtkColorTransferFunction>::New();
        vtkSmartPointer<vtkLookupTable> lut =
            vtkSmartPointer<vtkLookupTable>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkScalarBarActor> scalarBarActor =
            vtkSmartPointer<vtkScalarBarActor>::New();
        vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget =
            vtkSmartPointer<vtkScalarBarWidget>::New();
        vtkSmartPointer<vtkPlaneCollection> vtkPlanes =
            vtkSmartPointer<vtkPlaneCollection>::New();
        vtkSmartPointer<vtkClipClosedSurface> clipper =
            vtkSmartPointer<vtkClipClosedSurface>::New();
        vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
        vtkSmartPointer<vtkClipPolyData> vtkClipper =
            vtkSmartPointer<vtkClipPolyData>::New();
        vtkSmartPointer<vtkPlaneWidget> pWidget =
            vtkSmartPointer<vtkPlaneWidget>::New();
        vtkSmartPointer<vtkPolyData> polys =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<BuildVTKWidgetCall> pCall =
            vtkSmartPointer<BuildVTKWidgetCall>::New();
        vtkSmartPointer<vtkResampleWithDataSet> resamplers =
            vtkSmartPointer<vtkResampleWithDataSet>::New();
        vtkSmartPointer<vtkPolyDataMapper> fillMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> fillActor = vtkSmartPointer<vtkActor>::New();

        double range[2] = {0, 0};
        vector<vtkSmartPointer<vtkActor>> motiActor;
        // vector<vtkSmartPointer<vtkPolyData>> motiData;
        vector<string> path;
        vector<vector<string>> newPath;

        // STL缓存相关成员变量
        vtkSmartPointer<vtkPolyData> cachedStlData =
            vtkSmartPointer<vtkPolyData>::New(); // 缓存的STL数据

        vtkSmartPointer<vtkPolyData> plyData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkPoints> plyPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> plyShells =
            vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkFloatArray> plyVonmises =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plyDisX =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plyDisY =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plyDisZ =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plyDisTotal =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySR11 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySR22 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySR33 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySR12 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySR13 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySR23 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySS11 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySS22 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySS33 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySS12 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySS13 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plySS23 =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plyTemperature =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkFloatArray> plyAllDis =
            vtkSmartPointer<vtkFloatArray>::New();
        vtkSmartPointer<vtkPolyDataMapper> plyMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkColorTransferFunction> plyColorMap =
            vtkSmartPointer<vtkColorTransferFunction>::New();
        vtkSmartPointer<vtkActor> plyActor = vtkSmartPointer<vtkActor>::New();

        vtkSmartPointer<vtkSphereSource> minPoint =
            vtkSmartPointer<vtkSphereSource>::New();
        vtkSmartPointer<vtkLineSource> minline =
            vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkTextProperty> mintextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> mintitle = 
            vtkSmartPointer<vtkTextActor3D>::New();
        vtkSmartPointer<vtkPolyDataMapper> minMapperLine =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkPolyDataMapper> minMapperPoint =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> minActorLine =
            vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkActor> minActorPoint =
            vtkSmartPointer<vtkActor>::New();

        vtkSmartPointer<vtkSphereSource> maxPoint =
            vtkSmartPointer<vtkSphereSource>::New();
        vtkSmartPointer<vtkLineSource> maxline =
            vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkTextProperty> maxtextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> maxtitle =
            vtkSmartPointer<vtkTextActor3D>::New();
        vtkSmartPointer<vtkPolyDataMapper> maxMapperLine =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkPolyDataMapper> maxMapperPoint =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> maxActorLine =
            vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkActor> maxActorPoint =
            vtkSmartPointer<vtkActor>::New();


        vtkSmartPointer<vtkFollower> minFollower = vtkSmartPointer<vtkFollower>::New();
        vtkSmartPointer<vtkFollower> maxFollower = vtkSmartPointer<vtkFollower>::New();



};
// 梁的数据
struct beamsData {
        vector<vtkSmartPointer<vtkActor>> line;
        vector<vtkSmartPointer<vtkActor>> point;
};
// 结构前处理数据（使用中）
class workData {
      public:
   
        map<string, vtkSmartPointer<vtkActor>> Points; // 创建的任何点都存储在points容器中


        map<string, vtkSmartPointer<vtkActor>> Arrows; // 存储箭头
        std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> Arrows1;
        map<string, vtkSmartPointer<vtkAssembly>> ArrowAssemblies;
        map<string, vtkSmartPointer<vtkTextActor3D>> TitleArrows; // 箭头关联的3D文本标签（如标注箭头含义）


        map<string, vtkSmartPointer<vtkActor>> Constrains; // 固定支撑（约束）
        std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ConstrainsTranslationMap;
        std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ConstrainsRotationMap;
        map<string, vtkSmartPointer<vtkAssembly>> ConstrainsAssemblies;


        // 热约束
        std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ThermalConstrainsMap;
        map<string, vtkSmartPointer<vtkAssembly>> ThermalConstrainsAssemblies;


         // 对流换热
        std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ConvectionsMap;
        map<string, vtkSmartPointer<vtkAssembly>> ConvectionsAssemblies;


        // 表面热流
        std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> HeatFluxesMap;
        map<string, vtkSmartPointer<vtkAssembly>> HeatFluxesAssemblies;


        map<string, vtkSmartPointer<vtkActor>> Spring;            // 弹簧
        map<string, vtkSmartPointer<vtkActor>> Connects;          // 连接
        map<string, vtkSmartPointer<vtkActor>> Rb3;               // rb3
        map<string, beamsData> Beams;                             // 梁
        map<string, vtkSmartPointer<vtkTextActor3D>> mark;        // 标记
        map<string, vtkSmartPointer<vtkActor>> distributedArrows; // 新增字段
        map<string, vtkSmartPointer<vtkActor>> pointActor; // rb3的点

        vtkSmartPointer<vtkActor> actor_Mesh = vtkSmartPointer<vtkActor>::New();
        // vector也作为容器，创建vector<guass>容器，pts的值只存储在<guass>容器中，取值也是根据对应的类取
        // 以下之后会被转换为vtkActor存入对应的map中
        vector<guass> pts; // 高斯点
        map<string, vector<guass>> newPts;
        vector<ArrowFrame> af; // 箭头
        vector<arrow> arr;     // 箭头
        vector<constrain> con; // 固定支撑（约束）
        vector<beam> bm;       // 梁
        vector<spring> sp;     // 弹簧
        vector<connects> ct;   // 连接
        vector<RB3> rb;        // rb3
        vector<forceFace> fF;  // 面力-载荷-分布载荷
        vector<fixFace> fA; // 面上的固定支撑-约束-固定支撑-添加面
        vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
};
// ply结构，读取ply时存储在以下几个数据成员变量里
struct IPlyEntry {
        virtual bool Points(const float *&pts, int &nlen) = 0;       // 点
        virtual bool Triangles(const int *&elements, int &elen) = 0; // 三角形
        virtual bool Normals(const float *&normals, int &nnlen) = 0; // 向量
};

struct TPlyEntry : public IPlyEntry, public dcip_ply::triangle_part_t {
        virtual bool Points(const float *&pts, int &nlen) {
                pts = &points[0];
                nlen = static_cast<int>(points.size() / 3);

                return true;
        }

        virtual bool Triangles(const int *&ptr_triangles, int &tlen) {
                ptr_triangles = &triangles[0];
                tlen = static_cast<int>(triangles.size() / 3);

                return true;
        }

        virtual bool Normals(const float *&normals, int &nnlen) {
                normals = &vertexNormals[0];
                nnlen = static_cast<int>(vertexNormals.size() / 3);

                return true;
        }
        virtual ~TPlyEntry() {}
};

vector<IPlyEntry *> g_entries;
vtkSmartPointer<vtkActor> currentActor = vtkSmartPointer<vtkActor>::New();
workData *pRet;
string types = "";

void setCurrentActor(vtkSmartPointer<vtkActor> input) { currentActor = input; }

vtkSmartPointer<vtkActor> getCurrentActor() { return currentActor; }

void setWorkData(workData *input) { pRet = input; }

workData *getWorkData() { return pRet; }

void setTypes(string input) { types = input; }

string getTypes() { return types; }

string setArrow1(double x, double y, double z, double nx, double ny, double nz, string name, vtkRenderer* renderer, workData* work, int flag, string id);
void getFilesClassified(string path, vector<vector<string>>& classifiedFiles);
string OnModelVTKLoad(string pathSTL, string pathVts, double size, vtkRenderer* renderer, postManage* work);
string OnModelVTKLoadWithDeformation(string path, string path1, double deformationScale, vtkRenderer* renderer, postManage* work);
string setSphereEx(double x, double y, double z, double radius, string name, vtkRenderer* renderer, workData* work, vector<string> ids);
string modifySphereEx(double x, double y, double z, double radius, string name, vtkRenderer* renderer, workData* work, vector<string> ids);
string generateBrep(string id, string path, workData* work, string path2);
void callbackMouse(TMouseEvent* event, void* pRawInf);
string setConstraintEx(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale);
string setArrow1Ex(double x, double y, double z, double nx, double ny, double nz, string name, vtkRenderer* renderer, workData* work, int flag, string id, double scale);
string modifyConstraintEx(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale);
string modifyArrowEx(double x, double y, double z, double nx, double ny, double nz, string name, vtkRenderer* renderer, workData* work, string id, double scale);
bool CreateConstraints(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale, string id, int xd, int yd, int zd, int xr, int yr, int zr, int flag);
bool CreateConstraint(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale, int xd, int yd, int zd, int xr, int yr, int zr);
vtkSmartPointer<vtkPolyDataAlgorithm> CreateTransLationCone(vector<bool>& hasConstraint);
bool modifyConstraintsNew(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale, string id, int xd, int yd, int zd, int xr, int yr, int zr, int flag);
vtkSmartPointer<vtkPolyDataAlgorithm> CreateTransLationCone(const vector<bool>& hasConstraint, double radius);
static void removeHeatFluxesByName(vtkRenderer* renderer, workData* work, const std::string& name);
bool GetGaussPoints(workData* work, string id, double scale, vector<guass>& pts);
static void removeConvectionsByName(vtkRenderer* renderer, workData* work, const std::string& name);
bool CreateThermalConstraint(double x, double y, double z, double xn, double yn, double zn, string name, vtkRenderer* renderer, workData* work, double scale);
bool createHeatFluxes(double x, double y, double z, double nx, double ny, double nz,string name, vtkRenderer* renderer,workData* work, string id, double scale);
bool createConvections(double x, double y, double z, double nx, double ny,double nz, string name, vtkRenderer* renderer,workData* work, string id, double scale);
bool CreateThermalConstraints(double x, double y, double z, double xn, double yn, double zn, string name, vtkRenderer* renderer, workData* work, string id, double scale);
void GetScreenCoordinate(vtkRenderer* renderer, double x, double y, double z, double& screenX, double& screenY);
bool _init_callback = false; // 未用
//////////////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following codes.








JS_EXT_DATA_DECLARE()

// 0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
// please don't modify or delete the previous line codes.


#define WORLDTOSCREEN_FUNC_USAGE "WorldToScreen Usage: comx_sdk.WorldToScreen(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(WorldToScreen, 4, WORLDTOSCREEN_FUNC_USAGE)
{
        //Put your codes here
    unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
    IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);


    double x = JS_EXT_PARA(double, 1);
    double y = JS_EXT_PARA(double, 2);
    double z = JS_EXT_PARA(double, 3);

    if (!_init_callback) {
        _init_callback = true;
        pVtkContext->RegisterMouseListenCallback(callbackMouse);
    }

    void* pRawRenderer = NULL;
    pVtkContext->GetRenderer(pRawRenderer);

    vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;

    double x_screen, y_screen;
    GetScreenCoordinate(renderer, x, y, z, x_screen, y_screen);

    string res = to_string(x_screen) + "," + to_string(y_screen);

    JS_EXT_FUNC_ASSIGN_RET(res);


}
JS_EXT_FUNC_END()


#define SETHEATFLUXES_FUNC_USAGE "setHeatFluxes Usage: comx_sdk.setHeatFluxes(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(setHeatFluxes, 11, SETHEATFLUXES_FUNC_USAGE)
{
        //Put your codes here
    unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
    IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);

    unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
    workData* pEntry = (workData*)((void*)ullWorkData);
    setWorkData(pEntry);

    double x = JS_EXT_PARA(double, 2);
    double y = JS_EXT_PARA(double, 3);
    double z = JS_EXT_PARA(double, 4);

    double nx = JS_EXT_PARA(double, 5);
    double ny = JS_EXT_PARA(double, 6);
    double nz = JS_EXT_PARA(double, 7);

    string name = JS_EXT_PARA(string, 8);
    string id = JS_EXT_PARA(string, 9);
    double scale = JS_EXT_PARA(double, 10);

    if (!_init_callback) {
        _init_callback = true;
        pVtkContext->RegisterMouseListenCallback(callbackMouse);
    }

    void* pRawRenderer = NULL;
    pVtkContext->GetRenderer(pRawRenderer);

    vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;

    string ret_val = type_cast<string>(createHeatFluxes(
        x, y, z, nx, ny, nz, name, renderer, pEntry, id, scale));

    JS_EXT_FUNC_ASSIGN_RET(ret_val);

}
JS_EXT_FUNC_END()


#define SETCONVECTIONS_FUNC_USAGE "setConvections Usage: comx_sdk.setConvections(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(setConvections, 11, SETCONVECTIONS_FUNC_USAGE)
{
        //Put your codes here
    unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
    IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);

    unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
    workData* pEntry = (workData*)((void*)ullWorkData);
    setWorkData(pEntry);

    double x = JS_EXT_PARA(double, 2);
    double y = JS_EXT_PARA(double, 3);
    double z = JS_EXT_PARA(double, 4);

    double nx = JS_EXT_PARA(double, 5);
    double ny = JS_EXT_PARA(double, 6);
    double nz = JS_EXT_PARA(double, 7);

    string name = JS_EXT_PARA(string, 8);
    string id = JS_EXT_PARA(string, 9);
    double scale = JS_EXT_PARA(double, 10);

    if (!_init_callback) {
        _init_callback = true;
        pVtkContext->RegisterMouseListenCallback(callbackMouse);
    }

    void* pRawRenderer = NULL;
    pVtkContext->GetRenderer(pRawRenderer);

    vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;

    string ret_val = type_cast<string>(createConvections(
        x, y, z, nx, ny, nz, name, renderer, pEntry, id, scale));

    JS_EXT_FUNC_ASSIGN_RET(ret_val);

}
JS_EXT_FUNC_END()


#define SETTHERMALCONSTRAINS_FUNC_USAGE "setThermalConstrains Usage: comx_sdk.setThermalConstrains(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(setThermalConstrains, 11, SETTHERMALCONSTRAINS_FUNC_USAGE)
{
        //Put your codes here
    unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
    IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);

    unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
    workData* pEntry = (workData*)((void*)ullWorkData);
    setWorkData(pEntry);

    double x = JS_EXT_PARA(double, 2);
    double y = JS_EXT_PARA(double, 3);
    double z = JS_EXT_PARA(double, 4);

    double nx = JS_EXT_PARA(double, 5);
    double ny = JS_EXT_PARA(double, 6);
    double nz = JS_EXT_PARA(double, 7);

    string name = JS_EXT_PARA(string, 8);
    string id = JS_EXT_PARA(string, 9);
    double scale = JS_EXT_PARA(double, 10);

    if (!_init_callback) {
        _init_callback = true;
        pVtkContext->RegisterMouseListenCallback(callbackMouse);
    }

    void* pRawRenderer = NULL;
    pVtkContext->GetRenderer(pRawRenderer);

    vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;

    string ret_val = type_cast<string>(CreateThermalConstraints(
        x, y, z, nx, ny, nz, name, renderer, pEntry, id, scale));

    JS_EXT_FUNC_ASSIGN_RET(ret_val);

}
JS_EXT_FUNC_END()


#define MODIFYCONSTRAINTSEXEX_FUNC_USAGE "modifyConstraintsExEx Usage: comx_sdk.modifyConstraintsExEx(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(modifyConstraintsExEx, 15, MODIFYCONSTRAINTSEXEX_FUNC_USAGE)
{
        //Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData* pEntry = (workData*)((void*)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        string name = JS_EXT_PARA(string, 5);
        double scale = JS_EXT_PARA(double, 6);

        string id = JS_EXT_PARA(string, 7);
        int xd = JS_EXT_PARA(int, 8);
        int yd = JS_EXT_PARA(int, 9);
        int zd = JS_EXT_PARA(int, 10);

        int xr = JS_EXT_PARA(int, 11);
        int yr = JS_EXT_PARA(int, 12);
        int zr = JS_EXT_PARA(int, 13);

        int flag = JS_EXT_PARA(int, 14);

        if (!_init_callback) {
            _init_callback = true;
            pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void* pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;

        // return Napi::Boolean::From(info.Env(), false);

        string ret_val = type_cast<string>(
            modifyConstraintsNew(x, y, z, name, renderer, pEntry, scale, id, xd, yd, zd, xr, yr, zr, flag));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);

}
JS_EXT_FUNC_END()

#define SETCONSTRAINTSEXEX_FUNC_USAGE                                          \
        "setConstraintsExEx Usage: comx_sdk.setConstraintsExEx(/*put your "    \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(setConstraintsExEx, 15, SETCONSTRAINTSEXEX_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        string name = JS_EXT_PARA(string, 5);
        double scale = JS_EXT_PARA(double, 6);

        string id = JS_EXT_PARA(string, 7);
        int xd = JS_EXT_PARA(int, 8);
        int yd = JS_EXT_PARA(int, 9);
        int zd = JS_EXT_PARA(int, 10);

        int xr = JS_EXT_PARA(int, 11);
        int yr = JS_EXT_PARA(int, 12);
        int zr = JS_EXT_PARA(int, 13);

        int flag = JS_EXT_PARA(int, 14);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // return Napi::Boolean::From(info.Env(), false);

        string ret_val = type_cast<string>(
            CreateConstraints(x, y, z, name, renderer, pEntry, scale,id, xd, yd, zd, xr, yr, zr, flag));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYCONSTRAINTSEX_FUNC_USAGE                                         \
        "modifyConstraintsEx Usage: comx_sdk.modifyConstraintsEx(/*put your "  \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(modifyConstraintsEx, 7, MODIFYCONSTRAINTSEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        string name = JS_EXT_PARA(string, 5);
        double scale = JS_EXT_PARA(double, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // return Napi::Boolean::From(info.Env(), false);

        string ret_val = type_cast<string>(
            modifyConstraintEx(x, y, z, name, renderer, pEntry, scale));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYARROWSEX_FUNC_USAGE                                              \
        "modifyArrowsEx Usage: comx_sdk.modifyArrowsEx(/*put your parameters " \
        "list here*/);"
JS_EXT_FUNC_BEGIN(modifyArrowsEx, 11, MODIFYARROWSEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double nx = JS_EXT_PARA(double, 5);
        double ny = JS_EXT_PARA(double, 6);
        double nz = JS_EXT_PARA(double, 7);

        string name = JS_EXT_PARA(string, 8);
        string id = JS_EXT_PARA(string, 9);
        double scale = JS_EXT_PARA(double, 10);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(modifyArrowEx(
            x, y, z, nx, ny, nz, name, renderer, pEntry, id, scale));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SETCONSTRAINTSEX_FUNC_USAGE                                            \
        "setConstraintsEx Usage: comx_sdk.setConstraintsEx(/*put your "        \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(setConstraintsEx, 7, SETCONSTRAINTSEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        string name = JS_EXT_PARA(string, 5);
        double scale = JS_EXT_PARA(double, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // return Napi::Boolean::From(info.Env(), false);

        string ret_val = type_cast<string>(
            setConstraintEx(x, y, z, name, renderer, pEntry, scale));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SETARROWSEX_FUNC_USAGE                                                 \
        "setArrowsEx Usage: comx_sdk.setArrowsEx(/*put your parameters list "  \
        "here*/);"
JS_EXT_FUNC_BEGIN(setArrowsEx, 12, SETARROWSEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double nx = JS_EXT_PARA(double, 5);
        double ny = JS_EXT_PARA(double, 6);
        double nz = JS_EXT_PARA(double, 7);

        string name = JS_EXT_PARA(string, 8);
        int flag = JS_EXT_PARA(int, 9);
        string id = JS_EXT_PARA(string, 10);
        double scale = JS_EXT_PARA(double, 11);
        cout << "setArrows_id:" << id << endl;

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // cout << 1 << endl;
        string ret_val = type_cast<string>(setArrow1Ex(
            x, y, z, nx, ny, nz, name, renderer, pEntry, flag, id, scale));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYSPHERESEX_FUNC_USAGE                                             \
        "modifySpheresEx Usage: comx_sdk.modifySpheresEx(/*put your "          \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(modifySpheresEx, 8, MODIFYSPHERESEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double radius = JS_EXT_PARA(double, 5);
        string name = JS_EXT_PARA(string, 6);
        string StringID = JS_EXT_PARA(string, 7);

        vector<string> Id = splitAndConvertToInt(StringID, ',');

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            modifySphereEx(x, y, z, radius, name, renderer, pEntry, Id));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SETSPHERESEX_FUNC_USAGE                                                \
        "setSpheresEx Usage: comx_sdk.setSpheresEx(/*put your parameters "     \
        "list here*/);"
JS_EXT_FUNC_BEGIN(setSpheresEx, 8, SETSPHERESEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double radius = JS_EXT_PARA(double, 5);
        string name = JS_EXT_PARA(string, 6);
        string StringID = JS_EXT_PARA(string, 7);

        vector<string> Id = splitAndConvertToInt(StringID, ',');

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            setSphereEx(x, y, z, radius, name, renderer, pEntry, Id));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define RENDERERMODELEXEX_FUNC_USAGE                                           \
        "rendererModelExEx Usage: comx_sdk.rendererModelExEx(/*put your "      \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(rendererModelExEx, 6, RENDERERMODELEXEX_FUNC_USAGE) {
        // Put your codes here
        //  这是一个预留的函数，目前返回空字符串
        string result = "rendererModelExEx not implemented yet";
        JS_EXT_FUNC_ASSIGN_RET(result);
}
JS_EXT_FUNC_END()

// 渲染ply（目前未用）

#define RENDERERMODELEX_FUNC_USAGE                                             \
        "rendererModelEx Usage: comx_sdk.rendererModelEx(/*put your "          \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(rendererModelEx, 6, RENDERERMODELEX_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);
        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string stlPath = JS_EXT_PARA(string, 2);
        int VectorIdex = JS_EXT_PARA(int, 3);
        VectorIdex -= 1;
        double fileIndex = JS_EXT_PARA(double, 4);
        double scale = JS_EXT_PARA(double, 5);

        if (pEntry == nullptr) {
                cout << "[ERROR] pEntry为空指针" << endl;
                string error_msg = "Error: pEntry is null";
                JS_EXT_FUNC_ASSIGN_RET(error_msg);
        } else if (VectorIdex < 0 || VectorIdex >= pEntry->newPath.size()) {
                cout << "[ERROR] VectorIdex越界: " << VectorIdex << endl;
                string error_msg = "Error: VectorIdex out of bounds";
                JS_EXT_FUNC_ASSIGN_RET(error_msg);
        } else if (fileIndex < 0 ||
                   fileIndex >= pEntry->newPath[VectorIdex].size()) {
                cout << "[ERROR] fileIndex越界: " << fileIndex << endl;
                string error_msg = "Error: fileIndex out of bounds";
                JS_EXT_FUNC_ASSIGN_RET(error_msg);
        } else {
                if (!_init_callback) {
                        _init_callback = true;
                        pVtkContext->RegisterMouseListenCallback(callbackMouse);
                }

                void *pRawRenderer = NULL;
                pVtkContext->GetRenderer(pRawRenderer);
                vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

                string res = OnModelVTKLoadWithDeformation(
                    stlPath, pEntry->newPath[VectorIdex][fileIndex], scale,
                    renderer, pEntry);

                JS_EXT_FUNC_ASSIGN_RET(res);
        }
}
JS_EXT_FUNC_END()

#define RENDERERMODEL_FUNC_USAGE                                               \
        "rendererModel Usage: comx_sdk.rendererModel(/*put your parameters "   \
        "list here*/);"
JS_EXT_FUNC_BEGIN(rendererModel, 5, RENDERERMODEL_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string stlPath = JS_EXT_PARA(string, 2);
        int VectorIdex = JS_EXT_PARA(int, 3);
        // id = id - 1; // 索引从1开始，这里转换为0开始 newadd
        double fileIndex = JS_EXT_PARA(double, 4);
        // double scale = JS_EXT_PARA(double, 5);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }
        cout << "[DEBUG] rendererModel: \n";
        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string res =
            OnModelVTKLoad(stlPath, pEntry->newPath[VectorIdex][fileIndex],
                           1000, renderer, pEntry);

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define LOADFILESMODEL_FUNC_USAGE                                              \
        "loadFilesModel Usage: comx_sdk.loadFilesModel(/*put your parameters " \
        "list here*/);"
JS_EXT_FUNC_BEGIN(loadFilesModel, 4, LOADFILESMODEL_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string stlPath = JS_EXT_PARA(string, 2);
        string vtsPath = JS_EXT_PARA(string, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;
        pEntry->newPath.clear();

        // 加载并缓存STL文件
        {
                ifstream file(stlPath.c_str());
                if (!file.good()) {
                        cout << "[ERROR] STL文件不存在: " << stlPath << endl;
                } else {
                        file.close();
                        vtkSmartPointer<vtkSTLReader> stlReader =
                            vtkSmartPointer<vtkSTLReader>::New();
                        stlReader->SetFileName(stlPath.c_str());
                        vtkOutputWindow::SetGlobalWarningDisplay(1);

                        try {
                                stlReader->Update();
                                vtkPolyData *stlOutput = stlReader->GetOutput();
                                if (stlOutput &&
                                    stlOutput->GetNumberOfPoints() > 0) {
                                        pEntry->cachedStlData->DeepCopy(
                                            stlOutput);
                                } else {
                                        cout << "[ERROR] STL文件读取失败: "
                                             << stlPath << endl;
                                }
                        } catch (const std::exception &e) {
                                cout << "[ERROR] STL文件读取异常: " << e.what()
                                     << endl;
                        }
                }
        }

        getFilesClassified(vtsPath, pEntry->newPath);

        string result = "[";
        for (int i = 0; i < pEntry->newPath.size(); i++) {
                if (i > 0) {
                        result += ",";
                }
                int index = i + 1;
                int fileCount = pEntry->newPath[i].size();
                result +=
                    "[" + to_string(index) + "," + to_string(fileCount) + "]";
        }
        result += "]";

        JS_EXT_FUNC_ASSIGN_RET(result);
}
JS_EXT_FUNC_END()

#define GENERATEBREPSEX_FUNC_USAGE                                             \
        "generateBrepsEx Usage: comx.ply.generateBrepsEx(/*put your "          \
        "parameters list here*/);"
JS_EXT_FUNC_BEGIN(generateBrepsEx, 5, GENERATEBREPSEX_FUNC_USAGE) {
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        string id = JS_EXT_PARA(string, 2);
        string path = JS_EXT_PARA(string, 3);
        string path_Brep = JS_EXT_PARA(string, 4);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string res =
            type_cast<string>(generateBrep(id, path, pEntry, path_Brep));

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

void createAndDisplayPolyData(const std::vector<double> &points,
                              const std::vector<double> &normals,
                              const std::vector<int> &triangles,
                              vtkRenderer *renderer) {

        _generateColorbar();

        renderer->RemoveAllViewProps();

        vtkPolyData *polyData = vtkPolyData::New();

        vtkPoints *vtkPointsObj = vtkPoints::New();
        for (size_t i = 0; i < points.size() / 3; ++i) {
                vtkPointsObj->InsertNextPoint(points[i * 3], points[i * 3 + 1],
                                              points[i * 3 + 2]);
        }
        polyData->SetPoints(vtkPointsObj);
        vtkPointsObj->Delete();

        vtkDoubleArray *normalsArray = vtkDoubleArray::New();
        normalsArray->SetNumberOfComponents(3);
        for (size_t i = 0; i < normals.size() / 3; ++i) {
                normalsArray->InsertNextTuple3(
                    normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
        }
        polyData->GetPointData()->SetNormals(normalsArray);
        normalsArray->Delete();

        vtkCellArray *trianglesArray = vtkCellArray::New();
        for (size_t i = 0; i < triangles.size() / 3; ++i) {
                vtkTriangle *triangle = vtkTriangle::New();
                triangle->GetPointIds()->SetId(0, triangles[i * 3]);
                triangle->GetPointIds()->SetId(1, triangles[i * 3 + 1]);
                triangle->GetPointIds()->SetId(2, triangles[i * 3 + 2]);
                trianglesArray->InsertNextCell(triangle);
                triangle->Delete();
        }
        polyData->SetPolys(trianglesArray);
        trianglesArray->Delete();

        vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
        mapper->SetInputData(polyData);

        vtkActor *actor = vtkActor::New();
        actor->SetMapper(mapper);
        actor->SetPickable(true);

        actor->GetProperty()->SetColor(RED / 255., GREEN / 255., BLUE / 255.);

        renderer->AddActor(actor);

        mapper->Delete();
        actor->Delete();
        polyData->Delete();
}
// 判断一个点是否在包围盒内
bool isInBounds(double x, double y, double z, double *bound) {
        if ((x > bound[0] && x < bound[1]) && (y > bound[2] && y < bound[3]) &&
            (z > bound[4] && z < bound[5])) {
                return true;
        } else {
                return false;
        }
}

// 鼠标响应事件
void callbackMouse(TMouseEvent *event, void *pRawInf) {

        workData *work = getWorkData();
        string type = getTypes();
        // 左键触发以下功能
        if (event->Action() == TMouseEvent::LPRESS) {
                IVtkContext *pInf = (IVtkContext *)pRawInf;

                void *pRawMouseOp = NULL;
                pInf->GetProperty("HitTest", pRawMouseOp);
                IVtkMouseOperator *pMouseOp = (IVtkMouseOperator *)pRawMouseOp;

                double x, y, z;
                // 将gl画布中的二维坐标点转换为相对gl画布的三维坐标点
                pMouseOp->HitTest(event->X(), event->Y(), x, y, z);

                // cout << "3D x = " << x << ", y = " << y << " , z = " << z
                //<< endl;

                double nx, ny, nz;
                // 将gl画布中的二维向量转换为相对gl画布的三维向量
                pMouseOp->HitTestNormal(event->X(), event->Y(), nx, ny, nz);

                /* cout << "3D nx = " << nx << ", ny = " << ny << " , nz = " <<
                   nz
                      << endl;*/
                // 选点
                if (type == "points") {
                        // 遍历点（points）容器中所有的点
                        for (auto it = work->Points.begin();
                             it != work->Points.end(); it++) {
                                it->second->GetProperty()->SetColor(0, 1, 0);
                                double bounds[6];
                                it->second->GetBounds(bounds);

                                if (isInBounds(x, y, z, bounds)) {
                                        it->second->GetProperty()->SetColor(
                                            1, 0, 0);
                                        setCurrentActor(it->second);
                                }
                        }
                        /*
                            for (int i = 0; i < work->Points.size(); i++) {
                                    work->Points[i]->GetProperty()->SetColor(0,
                           1, 0); double bounds[6];
                                    work->Points[i]->GetBounds(bounds);

                                    if (isInBounds(x, y, z, bounds)) {
                                            work->Points[i]
                                                ->GetProperty()
                                                ->SetColor(1, 0, 0);
                                            setCurrentActor(work->Points[i]);
                                    }
                            }*/
                }
                // 选中箭头的（未用）
                if (type == "arrows") {

                        for (auto it = work->Arrows.begin();
                             it != work->Arrows.end(); it++) {
                                it->second->GetProperty()->SetColor(0, 1, 0);
                                double bounds[6];
                                it->second->GetBounds(bounds);

                                if (isInBounds(x, y, z, bounds)) {
                                        it->second->GetProperty()->SetColor(
                                            1, 0, 0);
                                        setCurrentActor(it->second);
                                }
                        }
                        /*
                            for (int i = 0; i < work->Arrows.size(); i++) {
                                    work->Arrows[i]->GetProperty()->SetColor(0,
                           1, 0); double bounds[6];
                                    work->Arrows[i]->GetBounds(bounds);

                                    if (isInBounds(x, y, z, bounds)) {
                                            work->Arrows[i]
                                                ->GetProperty()
                                                ->SetColor(1, 0, 0);
                                            setCurrentActor(work->Arrows[i]);
                                    }
                            }*/
                }
                // 固定支撑
                //if (type == "constraints") {

                //       /* for (auto it = work->Constrains.begin();
                //             it != work->Constrains.end(); it++) {
                //                it->second->GetProperty()->SetColor(0, 1, 0);
                //                double bounds[6];
                //                it->second->GetBounds(bounds);

                //                if (isInBounds(x, y, z, bounds)) {
                //                        it->second->GetProperty()->SetColor(
                //                            1, 0, 0);
                //                        setCurrentActor(it->second);
                //                }
                //        }*/

                //        for (auto it = work->ConstrainsRotationMap.begin(); it != work->ConstrainsRotationMap.end(); it++)
                //        {
                //                it->second->GetProperty()->SetColor(0, 1, 0);
                //                double bounds[6];
                //                it->second->GetBounds(bounds);

                //                if (isInBounds(x, y, z, bounds)) {
                //                        it->second->GetProperty()->SetColor(
                //                            1, 0, 0);
                //                        setCurrentActor(it->second);
                //                }
                //        }

                //        for (auto it = work->ConstrainsTranslationMap.begin(); it != work->ConstrainsTranslationMap.end(); it++)
                //        {
                //                it->second->GetProperty()->SetColor(0, 1, 0);
                //                double bounds[6];
                //                it->second->GetBounds(bounds);

                //                if (isInBounds(x, y, z, bounds)) {
                //                    it->second->GetProperty()->SetColor(
                //                        1, 0, 0);
                //                    setCurrentActor(it->second);
                //                }
                //        }
                //        /*
                //            for (int i = 0; i < work->Constrains.size(); i++) {
                //                    work->Constrains[i]->GetProperty()->SetColor(
                //                        0, 1, 0);
                //                    double bounds[6];
                //                    work->Constrains[i]->GetBounds(bounds);

                //                    if (isInBounds(x, y, z, bounds)) {
                //                            work->Constrains[i]
                //                                ->GetProperty()
                //                                ->SetColor(1, 0, 0);
                //                            setCurrentActor(work->Constrains[i]);
                //                    }
                //            }*/
                //}
                // 梁的线条
                if (type == "beams_line") {

                        for (auto it = work->Beams.begin();
                             it != work->Beams.end(); it++) {
                                it->second.line[0]->GetProperty()->SetColor(
                                    0, 1, 0);
                                double bounds[6];
                                it->second.line[0]->GetBounds(bounds);

                                if (isInBounds(x, y, z, bounds)) {
                                        it->second.line[0]
                                            ->GetProperty()
                                            ->SetColor(1, 0, 0);
                                        setCurrentActor(it->second.line[0]);
                                }
                        }
                        /*
                        for (int i = 0; i < work->Beams.size(); i++) {

                                work->Beams[i].line[0]->GetProperty()->SetColor(
                                    0, 1, 0);
                                double bounds[6];
                                work->Beams[i].line[0]->GetBounds(bounds);

                                if (isInBounds(x, y, z, bounds)) {
                                        work->Beams[i]
                                            .line[0]
                                            ->GetProperty()
                                            ->SetColor(1, 0, 0);
                                        setCurrentActor(work->Beams[i].line[0]);
                                }
                        }*/
                }
                // 梁上的点
                if (type == "beams_point") {

                        for (auto it = work->Beams.begin();
                             it != work->Beams.end(); it++) {

                                for (int j = 0; j < it->second.point.size();
                                     j++) {
                                        it->second.point[j]
                                            ->GetProperty()
                                            ->SetColor(0, 1, 0);
                                        double bounds[6];
                                        it->second.point[j]->GetBounds(bounds);

                                        if (isInBounds(x, y, z, bounds)) {
                                                it->second.point[j]
                                                    ->GetProperty()
                                                    ->SetColor(1, 0, 0);
                                                setCurrentActor(
                                                    it->second.point[j]);
                                        }
                                }
                        }
                        /*
                            for (int i = 0; i < work->Beams.size(); i++) {

                                    for (int j = 0; j <
                           work->Beams[i].point.size(); j++) { work->Beams[i]
                                                .point[j]
                                                ->GetProperty()
                                                ->SetColor(0, 1, 0);
                                            double bounds[6];
                                            work->Beams[i].point[j]->GetBounds(
                                                bounds);

                                            if (isInBounds(x, y, z, bounds)) {
                                                    work->Beams[i]
                                                        .point[j]
                                                        ->GetProperty()
                                                        ->SetColor(1, 0, 0);
                                                    setCurrentActor(
                                                        work->Beams[i].point[j]);
                                            }
                                    }
                            }
                            */
                }
        }
}
// 创建一个点？？？？？
string setPoint(double x, double y, double z, double radius, string name,
                vtkRenderer *renderer,
                workData *work) 
{ 

        // === 【新增步骤】检查是否已存在同名点 ===
        auto it = work->Points.find(name);
        if (it != work->Points.end()) {
            // 1. 如果存在，先从渲染器中移除旧的 Actor
            // 否则旧的会一直留在屏幕上
            renderer->RemoveActor(it->second);

            // map 中的旧指针稍后会被 operator[] 覆盖，引用计数归零自动销毁
        }

        vtkSmartPointer<vtkSphereSource> source =
            vtkSmartPointer<vtkSphereSource>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        // 创建球体的
        source->SetRadius(radius);
        source->SetCenter(x, y, z);
        source->SetPhiResolution(11);
        source->SetThetaResolution(21);

        mapper->SetInputConnection(source->GetOutputPort());
        actor->SetMapper(mapper);
        actor->GetProperty()->SetDiffuseColor(0, 1, 0);
        actor->GetProperty()->SetDiffuse(.8);
        actor->GetProperty()->SetSpecular(.5);
        actor->GetProperty()->SetSpecularColor(1.0, 1.0, 1.0);
        actor->GetProperty()->SetSpecularPower(30.0);
        // int i = work->Points.size();
        // actor->SetObjectName("P 0 " + to_string(i));
        actor->SetObjectName(name); // 对于球体这个类创建一个命名
        renderer->AddActor(actor);  // 渲染
        // string res = "Point" + to_string(i);
        work->Points[name] = actor;
        // work->Points.push_back(actor);
        return name;
}
// 修改球体的内容
string modifyPoint(double x, double y, double z, double radius, string name,
                   vtkRenderer *renderer, workData *work) {
        vtkSmartPointer<vtkSphereSource> source =
            vtkSmartPointer<vtkSphereSource>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

        source->SetRadius(radius);
        source->SetCenter(x, y, z);
        source->SetPhiResolution(11);
        source->SetThetaResolution(21);
        // 以上均为创建球体的操作
        // 覆盖球体1
        mapper->SetInputConnection(source->GetOutputPort());
        work->Points[name]->SetMapper(mapper);
        work->Points[name]->GetProperty()->SetDiffuseColor(0, 1, 0);
        work->Points[name]->GetProperty()->SetDiffuse(.8);
        work->Points[name]->GetProperty()->SetSpecular(.5);
        work->Points[name]->GetProperty()->SetSpecularColor(1.0, 1.0, 1.0);
        work->Points[name]->GetProperty()->SetSpecularPower(30.0);

        renderer->AddActor(work->Points[name]);
        string res = "ok";
        return res;
}

// 你需要的 includes（如果已有可省略）
#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkProp.h>
#include <vtkPropCollection.h>
#include <vtkRenderer.h>

// 安全移除 name 对应的所有箭头资源（actors / assembly / title）
// - renderer: 渲染器
// - work: workData*
// - name: 要移除的键
static void removeArrowsByName(vtkRenderer *renderer, workData *work,
                               const std::string &name) {
        if (!renderer || !work)
                return;

        // 1. 从 renderer 中移除 Arrows1[name] 中的每个 actor
        auto itVec = work->Arrows1.find(name);
        if (itVec != work->Arrows1.end()) {
                auto &vec = itVec->second;
                for (auto &actorSP : vec) {
                        if (actorSP) {
                                renderer->RemoveActor(
                                    actorSP); // smartpointer 隐式转换为裸指针
                        }
                }
                vec.clear();
                // 可选：erase vector entry，如果你希望后续检查不存在则 erase
                work->Arrows1.erase(itVec);
        }

        // 2. 如果存在 assembly，先移除 renderer 中的 assembly，并 erase map
        // 以降低引用计数
        auto itAsm = work->ArrowAssemblies.find(name);
        if (itAsm != work->ArrowAssemblies.end()) {
                if (itAsm->second) {
                        renderer->RemoveActor(itAsm->second);
                }
                work->ArrowAssemblies.erase(itAsm);
        }

        // 3. 如果存在 title actor，移除并 erase
        auto itTitle = work->TitleArrows.find(name);
        if (itTitle != work->TitleArrows.end()) {
                if (itTitle->second) {
                        renderer->RemoveActor(itTitle->second);
                }
                work->TitleArrows.erase(itTitle);
        }
}

// 修改箭头（力）边界-载荷-统一用的
string modifyArrow(double x, double y, double z, double nx, double ny,
                   double nz, string name, vtkRenderer *renderer,
                   workData *work, string id) {
        if (work->ArrowAssemblies.find(name) == work->ArrowAssemblies.end()) {
                return "KO";
        }
        renderer->RemoveActor(work->ArrowAssemblies[name]);

        work->Arrows1[name].clear();
        setArrow1(x, y, z, nx, ny, nz, name, renderer, work, 1, id);

        /*
            string tit = std::to_string(
                std::sqrt(std::pow(nx, 2) + std::pow(ny, 2) + std::pow(nz, 2)));
            vtkSmartPointer<vtkArrowSource> arrowSource =
                vtkSmartPointer<vtkArrowSource>::New();
            vtkSmartPointer<vtkTextProperty> polyTextProp =
                vtkSmartPointer<vtkTextProperty>::New();
            vtkSmartPointer<vtkTextActor3D> polyTitle =
                vtkSmartPointer<vtkTextActor3D>::New();
            vtkOutputWindow::SetGlobalWarningDisplay(0);
            polyTextProp->SetFontSize(20);
            polyTextProp->SetColor(0.0, 1.0, 0.0);
            polyTextProp->SetFontFamilyToTimes();
            work->TitleArrows[name]->SetTextProperty(polyTextProp);
            work->TitleArrows[name]->SetInput(tit.c_str());
            work->TitleArrows[name]->SetPosition(x, y, z);
            arrowSource->SetTipLength(0.3);
            arrowSource->SetTipRadius(0.1);
            arrowSource->SetShaftRadius(0.05);
            // 创建箭头的映射器
            vtkSmartPointer<vtkPolyDataMapper> arrowMapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
            arrowMapper->SetInputConnection(arrowSource->GetOutputPort());

            // 创建箭头的演员
            vtkSmartPointer<vtkActor> arrowActor =
           vtkSmartPointer<vtkActor>::New(); arrowActor->SetMapper(arrowMapper);
            arrowActor->GetProperty()->SetColor(0.0, 1.0,
                                                0.0); // 设置箭头的颜色为红色

            double normal[3] = {1.0, 0.0, 0.0};
            double newDirection[3] = {nx, ny, nz};
            double angle =
                acos(vtkMath::Dot(normal, newDirection) /
                     (vtkMath::Norm(normal) * vtkMath::Norm(newDirection)));
            double axis[3];
            vtkMath::Cross(normal, newDirection, axis);
            // 设置箭头的位置、方向和大小
            vtkSmartPointer<vtkTransform> transform =
                vtkSmartPointer<vtkTransform>::New();
            //transform->Translate(x, y,z); // 平移箭头到指定位置
            transform->RotateWXYZ(vtkMath::DegreesFromRadians(angle),
                                  axis);  // 绕Z轴旋转45度
            transform->Scale(40, 40, 40); // 放大箭头1.5倍
            //int k = work->Arrows.size();
            // 应用变换到箭头演员
            vtkSmartPointer<vtkAssembly> arrowAssembly =
                vtkSmartPointer<vtkAssembly>::New();
            for (auto& actor : work->Arrows1[name]) {
                actor->SetUserTransform(transform);
                actor->SetObjectName(name);
            }

            for (const auto& actor : work->Arrows1[name]) {
                arrowAssembly->AddPart(actor);
            }
            //work->Arrows[name]->SetUserTransform(transform);
            renderer->AddActor(arrowAssembly);
            renderer->AddActor(work->TitleArrows[name]);
            work->ArrowAssemblies[name] = arrowAssembly;
            //renderer->AddActor(work->Arrows[name]);
            */
        string res = "ok";

        return res;
}

// 修改箭头（力）边界-载荷-统一用的
string modifyArrowEx(double x, double y, double z, double nx, double ny,
                     double nz, string name, vtkRenderer *renderer,
                     workData *work, string id, double scale) {
        if (work->ArrowAssemblies.find(name) == work->ArrowAssemblies.end()) {
                return "KO";
        }
        if (work->TitleArrows.find(name) == work->TitleArrows.end()) {
                return "KO";
        }
        removeArrowsByName(renderer, work, name);
        setArrow1Ex(x, y, z, nx, ny, nz, name, renderer, work, 1, id, scale);

        string res = "ok";

        return res;
}

//载荷箭头
string setArrow(double x, double y, double z, double nx, double ny, double nz,
                string name, workData *work) {

        // 仅保留与创建箭头相关的代码
        vtkSmartPointer<vtkArrowSource> arrowSource =
            vtkSmartPointer<vtkArrowSource>::New();
        arrowSource->SetTipLength(0.3);
        arrowSource->SetTipRadius(0.1);
        arrowSource->SetShaftRadius(0.05);

        // 创建箭头的映射器
        vtkSmartPointer<vtkPolyDataMapper> arrowMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        arrowMapper->SetInputConnection(arrowSource->GetOutputPort());

        // 创建箭头的演员
        vtkSmartPointer<vtkActor> arrowActor = vtkSmartPointer<vtkActor>::New();
        arrowActor->SetMapper(arrowMapper);
        arrowActor->GetProperty()->SetColor(0.0, 1.0,
                                            0.0); // 设置箭头的颜色为绿色

        // 计算箭头的旋转角度和轴
        double normal[3] = {1.0, 0.0, 0.0};
        double newDirection[3] = {nx, ny, nz};
        double angle =
            acos(vtkMath::Dot(normal, newDirection) /
                 (vtkMath::Norm(normal) * vtkMath::Norm(newDirection)));
        double axis[3];
        vtkMath::Cross(normal, newDirection, axis);

        // 设置箭头的位置、方向和大小
        vtkSmartPointer<vtkTransform> transform =
            vtkSmartPointer<vtkTransform>::New();
        transform->Translate(x, y, z); // 平移箭头到指定位置
        transform->RotateWXYZ(vtkMath::DegreesFromRadians(angle),
                              axis);  // 旋转箭头
        transform->Scale(40, 40, 40); // 放大箭头

        // 应用变换到箭头演员
        arrowActor->SetUserTransform(transform);
        arrowActor->SetObjectName(name);

        // 将箭头演员存储到workData
        work->Arrows1[name].push_back(arrowActor);
        // work->Arrows[name] = arrowActor;
        // renderer->AddActor(arrowActor);

        // 返回名称（与函数声明一致）
        return name;
}


//对流换热
string setConvection(double x, double y, double z, double nx, double ny, double nz,
    string name, workData* work, double scale) {

    // 1. 创建箭头源 (保持一致的几何比例)
    vtkSmartPointer<vtkArrowSource> arrowSource =
        vtkSmartPointer<vtkArrowSource>::New();
    arrowSource->SetTipLength(0.3);
    arrowSource->SetTipRadius(0.1);
    arrowSource->SetShaftRadius(0.05);

    // 2. 创建映射器
    vtkSmartPointer<vtkPolyDataMapper> arrowMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    arrowMapper->SetInputConnection(arrowSource->GetOutputPort());

    // 3. 创建演员
    vtkSmartPointer<vtkActor> arrowActor = vtkSmartPointer<vtkActor>::New();
    arrowActor->SetMapper(arrowMapper);

    // ⭐ 修改颜色：设置为天蓝色 (Sky Blue) 表示对流/流体
    arrowActor->GetProperty()->SetColor(0.0, 0.6, 1.0);

    // 4. 计算旋转角度和轴
    double normal[3] = { 1.0, 0.0, 0.0 }; // vtkArrowSource 默认指向 X+
    double newDirection[3] = { nx, ny, nz };

    // 防止法线长度为0导致除零错误
    double normDir = vtkMath::Norm(newDirection);
    if (normDir < 1e-6) normDir = 1.0;

    double angle =
        acos(vtkMath::Dot(normal, newDirection) /
            (vtkMath::Norm(normal) * normDir));

    double axis[3];
    vtkMath::Cross(normal, newDirection, axis);

    // 5. 设置变换
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();

    // 平移：箭头底部位于 (x,y,z)，指向外部
    transform->Translate(x, y, z);

    // 旋转：对齐法线方向
    transform->RotateWXYZ(vtkMath::DegreesFromRadians(angle), axis);

    // 缩放
    transform->Scale(scale, scale, scale);

    // 应用变换
    arrowActor->SetUserTransform(transform);
    arrowActor->SetObjectName(name);

    // 6. 存储到 ConvectionsMap
    if (work) {
        work->ConvectionsMap[name].push_back(arrowActor);
    }

    return name;
}

//载荷箭头
string setArrowEx(double x, double y, double z, double nx, double ny, double nz,
                  string name, workData *work, double scale) {

        // 仅保留与创建箭头相关的代码
        vtkSmartPointer<vtkArrowSource> arrowSource =
            vtkSmartPointer<vtkArrowSource>::New();
        arrowSource->SetTipLength(0.3);
        arrowSource->SetTipRadius(0.1);
        arrowSource->SetShaftRadius(0.05);

        // 创建箭头的映射器
        vtkSmartPointer<vtkPolyDataMapper> arrowMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        arrowMapper->SetInputConnection(arrowSource->GetOutputPort());

        // 创建箭头的演员
        vtkSmartPointer<vtkActor> arrowActor = vtkSmartPointer<vtkActor>::New();
        arrowActor->SetMapper(arrowMapper);
        arrowActor->GetProperty()->SetColor(0.0, 1.0,
                                            0.3); // 设置箭头的颜色为绿色

        // 计算箭头的旋转角度和轴
        double normal[3] = {1.0, 0.0, 0.0};
        double newDirection[3] = {nx, ny, nz};
        double angle =
            acos(vtkMath::Dot(normal, newDirection) /
                 (vtkMath::Norm(normal) * vtkMath::Norm(newDirection)));
        double axis[3];
        vtkMath::Cross(normal, newDirection, axis);

        // 设置箭头的位置、方向和大小
        vtkSmartPointer<vtkTransform> transform =
            vtkSmartPointer<vtkTransform>::New();
        transform->Translate(x, y, z); // 平移箭头到指定位置
        transform->RotateWXYZ(vtkMath::DegreesFromRadians(angle),
                              axis);           // 旋转箭头
        transform->Scale(scale, scale, scale); // 放大箭头

        // 应用变换到箭头演员
        arrowActor->SetUserTransform(transform);
        arrowActor->SetObjectName(name);

        // 将箭头演员存储到workData
        work->Arrows1[name].push_back(arrowActor);
        // work->Arrows[name] = arrowActor;
        // renderer->AddActor(arrowActor);

        // 返回名称（与函数声明一致）
        return name;
}

string setArrow1(double x, double y, double z, double nx, double ny, double nz,
                 string name, vtkRenderer *renderer, workData *work, int flag,
                 string id) {
        vector<int> ids; // 高斯点
        vtkSmartPointer<vtkAssembly> arrowAssembly =
            vtkSmartPointer<vtkAssembly>::New();
        vector<guass> pts;
        if (work->newPts.find(id) != work->newPts.end()) {
                pts = work->newPts[id];
        }
        cout << flag << endl;
        cout << "高斯点个数size:" << pts.size() << endl;
        cout << "id:" << id << endl;
        cout << "x:" << x << "   y:" << y << "   z:" << z << endl;
        cout << "nx:" << nx << "   ny:" << ny << "   nz:" << nz << endl;
        if (pts.size() <= 0) {
                string tit = std::to_string(std::sqrt(
                    std::pow(nx, 2) + std::pow(ny, 2) + std::pow(nz, 2)));

                vtkSmartPointer<vtkTextProperty> polyTextProp =
                    vtkSmartPointer<vtkTextProperty>::New();
                vtkSmartPointer<vtkTextActor3D> polyTitle =
                    vtkSmartPointer<vtkTextActor3D>::New();
                vtkOutputWindow::SetGlobalWarningDisplay(0);
                polyTextProp->SetFontSize(20);
                polyTextProp->SetColor(0.0, 1.0, 0.0);
                polyTextProp->SetFontFamilyToTimes();
                polyTitle->SetTextProperty(polyTextProp);
                polyTitle->SetInput(tit.c_str());
                polyTitle->SetPosition(x, y, z);
                work->TitleArrows[name] = polyTitle;
                setArrow(x, y, z, nx, ny, nz, name, work);
                // 从workData获取箭头演员
                // vtkActor* arrowActor = work->Arrows[name];
                // arrowAssembly->AddPart(arrowActor);
        } else {
                cout << "pts.size > 0" << endl;
                for (int i = 0; i < pts.size(); i++) {
                        /* if (work->pts[i].id % 2 != 0) {
                                 continue;
                         }*/
                        if (i % 200 != 0) {
                                continue;
                        }
                        if (i == 0) {
                                cout << "x:" << pts[i].pts.x
                                     << "     y:" << pts[i].pts.y
                                     << "     z:" << pts[i].pts.z << endl;
                                string tit = std::to_string(std::sqrt(
                                    std::pow(nx, 2) + std::pow(ny, 2) +
                                    std::pow(nz, 2)));

                                vtkSmartPointer<vtkTextProperty> polyTextProp =
                                    vtkSmartPointer<vtkTextProperty>::New();
                                vtkSmartPointer<vtkTextActor3D> polyTitle =
                                    vtkSmartPointer<vtkTextActor3D>::New();
                                vtkOutputWindow::SetGlobalWarningDisplay(0);
                                polyTextProp->SetFontSize(20);
                                polyTextProp->SetColor(0.0, 1.0, 0.0);
                                polyTextProp->SetFontFamilyToTimes();
                                polyTitle->SetTextProperty(polyTextProp);
                                polyTitle->SetInput(tit.c_str());
                                polyTitle->SetPosition(x, y, z);
                                work->TitleArrows[name] = polyTitle;
                                setArrow(pts[i].pts.x, pts[i].pts.y,
                                         pts[i].pts.z, nx, ny, nz, name, work);

                                // 从workData获取箭头演员
                                // vtkActor* arrowActor = work->Arrows[name];

                                // arrowAssembly->AddPart(arrowActor);
                                continue;
                        }

                        /*if (work->pts[i].id % 2 != 0) {
                            continue;  // 跳过ID为奇数的点
                        }*/

                        /*string uniqueName =
                            name + "_" + std::to_string(work->pts[i].id);

                        setArrow(pts[i].pts.x, pts[i].pts.y,
                            pts[i].pts.z, nx, ny, nz, uniqueName,
                            work);*/
                        setArrow(pts[i].pts.x, pts[i].pts.y, pts[i].pts.z, nx,
                                 ny, nz, name, work);

                        // 从workData获取箭头演员
                        // vtkActor* arrowActor = work->Arrows[uniqueName];
                        // arrowAssembly->AddPart(arrowActor);
                }
        }

        for (const auto &actor : work->Arrows1[name]) {
                arrowAssembly->AddPart(actor);
        }

        renderer->AddActor(arrowAssembly);
        renderer->AddActor(work->TitleArrows[name]);
        work->ArrowAssemblies[name] = arrowAssembly;
        return name;
}

// 2. 单个热流箭头创建函数 (修正版)
string setHeatFlux(double x, double y, double z, double nx, double ny, double nz,
    string name, workData* work, double scale) {

    // 1. 创建箭头源
    vtkSmartPointer<vtkArrowSource> arrowSource =
        vtkSmartPointer<vtkArrowSource>::New();
    // 保持原来的比例
    arrowSource->SetTipLength(0.3);
    arrowSource->SetTipRadius(0.1);
    arrowSource->SetShaftRadius(0.05);

    // 2. 映射器
    vtkSmartPointer<vtkPolyDataMapper> arrowMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    arrowMapper->SetInputConnection(arrowSource->GetOutputPort());

    // 3. 演员
    vtkSmartPointer<vtkActor> arrowActor = vtkSmartPointer<vtkActor>::New();
    arrowActor->SetMapper(arrowMapper);
    arrowActor->GetProperty()->SetColor(1.0, 0.0, 0.0); // 红色

    // 4. 计算旋转
    // -----------------------------------------------------------
    // 逻辑：将箭头从默认的 (1,0,0) 旋转到目标方向 (法线反方向)
    // -----------------------------------------------------------
    double sourceDir[3] = { 1.0, 0.0, 0.0 }; // VTK 箭头默认朝向 X+

    // 目标方向：指向物体内部，即法线的反方向
    double targetDir[3] = { -nx, -ny, -nz };
    vtkMath::Normalize(targetDir); // 归一化非常重要

    // 计算旋转轴和角度
    double axis[3];
    vtkMath::Cross(sourceDir, targetDir, axis);
    double dot = vtkMath::Dot(sourceDir, targetDir);

    // 处理平行或反向的特殊情况 (防止叉乘结果为0导致轴错误)
    if (dot > 0.9999) { // 方向相同
        axis[0] = 0; axis[1] = 1; axis[2] = 0;
    }
    else if (dot < -0.9999) { // 方向相反
        axis[0] = 0; axis[1] = 0; axis[2] = 1; // 任意垂直轴
    }

    double angleRadians = acos(dot);
    double angleDegrees = vtkMath::DegreesFromRadians(angleRadians);

    // 5. 设置变换矩阵
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();

    // -----------------------------------------------------------
    // ⭐ 关键修正逻辑 ⭐
    // -----------------------------------------------------------

    // 步骤 A: 先移动到目标点 (x,y,z)
    transform->Translate(x, y, z);

    // 步骤 B: 沿着法线方向向外移动一段距离 ( offset )
    // 既然箭头是“射入”表面的，我们需要把箭头往“回”拉一个长度。
    // 因为 arrowSource 长度默认是 1，被 Scale 放大后长度是 scale。
    // 我们需要把起点沿着法线方向 (nx, ny, nz) 移动 'scale' 的距离。
    // 这样箭头的尾巴在空中，尖端刚好落在 (x,y,z)。

    // 如果觉得“没有紧挨着”，可能是视觉误差。
    // 尝试稍微减小一点这个距离，让尖端稍微插入表面一点点，比如 scale * 0.9
    // 或者不做修改，严格使用 scale。

    double offset = scale;
    transform->Translate(nx * offset, ny * offset, nz * offset);

    // 步骤 C: 旋转方向
    transform->RotateWXYZ(angleDegrees, axis);

    // 步骤 D: 缩放
    transform->Scale(scale, scale, scale);

    // 6. 应用变换
    arrowActor->SetUserTransform(transform);
    arrowActor->SetObjectName(name);

    // 7. 存储
    if (work) {
        work->HeatFluxesMap[name].push_back(arrowActor);
    }

    return name;
}

// 3. 批量/Assembly 创建函数
bool createHeatFluxes(double x, double y, double z, double nx, double ny, double nz,
    string name, vtkRenderer* renderer,
    workData* work, string id, double scale) {

    // 先清理旧的同名对象
    removeHeatFluxesByName(renderer, work, name);

    vtkSmartPointer<vtkAssembly> ConstraintsAssembly = vtkSmartPointer<vtkAssembly>::New();
    vector<guass> pts;

    // 获取高斯点 (假设 GetGaussPoints 已定义)
    // scale * 2 可能是为了稀疏化显示或者调整采样密度
    if (GetGaussPoints(work, id, scale * 2, pts)) {
        for (auto& p : pts) {
            // 这里假设高斯点结构体中包含具体的坐标
            // 关于法线：如果面是平的，使用传入的 nx,ny,nz；
            // 如果是曲面，理想情况下 pts 结构体应包含 p.nx, p.ny, p.nz
            // 这里暂且使用传入的统一法线 nx, ny, nz
            setHeatFlux(p.pts.x, p.pts.y, p.pts.z, nx, ny, nz, name, work, scale);
        }

        // 将生成的 Actor 加入 Assembly
        for (const auto& actor : work->HeatFluxesMap[name]) {
            ConstraintsAssembly->AddPart(actor);
        }

        renderer->AddActor(ConstraintsAssembly);
        work->HeatFluxesAssemblies[name] = ConstraintsAssembly;
        return true;
    }

    return false;
}
bool createConvections(double x, double y, double z, double nx, double ny,
    double nz, string name, vtkRenderer* renderer,
    workData* work, string id, double scale) {
    removeConvectionsByName(renderer, work, name);
    vtkSmartPointer<vtkAssembly> ConstraintsAssembly = vtkSmartPointer<vtkAssembly>::New();
    vector<guass> pts;

    if (GetGaussPoints(work, id, scale * 2, pts)) {
        for (auto& p : pts) {
            setConvection(p.pts.x, p.pts.y, p.pts.z, nx, ny, nz, name, work, scale);
        }
        for (const auto& actor : work->ConvectionsMap[name]) {
            ConstraintsAssembly->AddPart(actor);
        }
        renderer->AddActor(ConstraintsAssembly);
        work->ConvectionsAssemblies[name] = ConstraintsAssembly;
        return true;
    }


    return false;
}


string setArrow1Ex(double x, double y, double z, double nx, double ny,
                   double nz, string name, vtkRenderer *renderer,
                   workData *work, int flag, string id, double scale) {
        vector<int> ids; // 高斯点
        removeArrowsByName(renderer, work, name);
        vtkSmartPointer<vtkAssembly> arrowAssembly =
            vtkSmartPointer<vtkAssembly>::New();
        vector<guass> pts;
        if (work->newPts.find(id) != work->newPts.end()) {
                pts = work->newPts[id];
        }
        cout << flag << endl;
        cout << "高斯点个数size:" << pts.size() << endl;
        cout << "id:" << id << endl;
        cout << "x:" << x << "   y:" << y << "   z:" << z << endl;
        cout << "nx:" << nx << "   ny:" << ny << "   nz:" << nz << endl;
        if (pts.size() <= 0) {
                string tit = std::to_string(std::sqrt(
                    std::pow(nx, 2) + std::pow(ny, 2) + std::pow(nz, 2)));

                vtkSmartPointer<vtkTextProperty> polyTextProp =
                    vtkSmartPointer<vtkTextProperty>::New();
                vtkSmartPointer<vtkTextActor3D> polyTitle =
                    vtkSmartPointer<vtkTextActor3D>::New();
                vtkOutputWindow::SetGlobalWarningDisplay(0);
                polyTextProp->SetFontSize(scale / 10);
                polyTextProp->SetColor(0.0, 1.0, 0.0);
                polyTextProp->SetFontFamilyToTimes();
                polyTitle->SetTextProperty(polyTextProp);
                polyTitle->SetInput(tit.c_str());
                polyTitle->SetPosition(x, y, z);
                work->TitleArrows[name] = polyTitle;
                setArrowEx(x, y, z, nx, ny, nz, name, work, scale);
                // 从workData获取箭头演员
                // vtkActor* arrowActor = work->Arrows[name];
                // arrowAssembly->AddPart(arrowActor);
        } else {
                cout << "pts.size > 0" << endl;
                if (work->Arrows1.find(name) != work->Arrows1.end()) {
                        work->Arrows1[name].clear();
                }
                vector<guass> filtered;
                for (auto &p : pts) {
                        bool ok = true;
                        for (auto &f : filtered) {
                                double dx = p.pts.x - f.pts.x;
                                double dy = p.pts.y - f.pts.y;
                                double dz = p.pts.z - f.pts.z;
                                if (dx * dx + dy * dy + dz * dz <
                                    scale * scale) {
                                        ok = false;
                                        break;
                                }
                        }
                        if (ok)
                                filtered.push_back(p);
                }
                for (auto &p : filtered) {
                        setArrowEx(p.pts.x, p.pts.y, p.pts.z, nx, ny, nz, name,
                                   work, scale);
                }

                string tit = std::to_string(std::sqrt(
                    std::pow(nx, 2) + std::pow(ny, 2) + std::pow(nz, 2)));

                vtkSmartPointer<vtkTextProperty> polyTextProp =
                    vtkSmartPointer<vtkTextProperty>::New();
                vtkSmartPointer<vtkTextActor3D> polyTitle =
                    vtkSmartPointer<vtkTextActor3D>::New();
                vtkOutputWindow::SetGlobalWarningDisplay(0);
                polyTextProp->SetFontSize(scale / 10);
                polyTextProp->SetColor(0.0, 1.0, 0.0);
                polyTextProp->SetFontFamilyToTimes();
                polyTitle->SetTextProperty(polyTextProp);
                polyTitle->SetInput(tit.c_str());
                polyTitle->SetPosition(x, y, z);
                work->TitleArrows[name] = polyTitle;
        }

        for (const auto &actor : work->Arrows1[name]) {
                arrowAssembly->AddPart(actor);
        }

        renderer->AddActor(arrowAssembly);
        renderer->AddActor(work->TitleArrows[name]);
        work->ArrowAssemblies[name] = arrowAssembly;
        return name;
}


bool GetGaussPoints(workData* work, string id, double scale, vector<guass>& pts)
{
    if (work->newPts.find(id) != work->newPts.end()) {
        vector<guass> tmpPts = work->newPts[id];
        for (auto& p : tmpPts) {
            bool ok = true;
            for (auto& f : pts) {
                double dx = p.pts.x - f.pts.x;
                double dy = p.pts.y - f.pts.y;
                double dz = p.pts.z - f.pts.z;
                if (dx * dx + dy * dy + dz * dz <
                    scale * scale) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                pts.push_back(p);
        }
        return true;
    }
    return false;
}


static void removeHeatFluxesByName(vtkRenderer* renderer, workData* work,
    const std::string& name) {
    if (!renderer || !work)
        return;

    // 清理 HeatFluxesMap (vector)
    auto itVec = work->HeatFluxesMap.find(name);
    if (itVec != work->HeatFluxesMap.end()) {
        auto& vec = itVec->second;
        for (auto& actorSP : vec) {
            if (actorSP) {
                renderer->RemoveActor(actorSP);
            }
        }
        vec.clear();
        work->HeatFluxesMap.erase(itVec);
    }

    // 清理 HeatFluxesAssemblies (Assembly)
    auto itAsm = work->HeatFluxesAssemblies.find(name);
    if (itAsm != work->HeatFluxesAssemblies.end()) {
        if (itAsm->second) {
            renderer->RemoveActor(itAsm->second);
        }
        work->HeatFluxesAssemblies.erase(itAsm);
    }
}

static void removeConvectionsByName(vtkRenderer* renderer, workData* work,
    const std::string& name) {
    if (!renderer || !work)
        return;

    // 1. 清理 ConvectionsMap (std::vector<vtkSmartPointer<vtkActor>>)
    auto itVec = work->ConvectionsMap.find(name);
    if (itVec != work->ConvectionsMap.end()) {
        auto& vec = itVec->second;
        for (auto& actorSP : vec) {
            if (actorSP) {
                // 从渲染器中移除 Actor
                renderer->RemoveActor(actorSP);
            }
        }
        vec.clear(); // 清空 vector
        work->ConvectionsMap.erase(itVec); // 从 map 中移除该条目
    }

    // 2. 清理 ConvectionsAssemblies (vtkAssembly)
    auto itAsm = work->ConvectionsAssemblies.find(name);
    if (itAsm != work->ConvectionsAssemblies.end()) {
        if (itAsm->second) {
            // 从渲染器中移除 Assembly
            renderer->RemoveActor(itAsm->second);
        }
        work->ConvectionsAssemblies.erase(itAsm); // 从 map 中移除
    }
}


static void removeThermalConstraintsByName(vtkRenderer* renderer, workData* work,
    const std::string& name) {
    if (!renderer || !work)
        return;

    // 1. 清理 ThermalConstrainsMap (std::vector<vtkSmartPointer<vtkActor>>)
    // 对应您要求的 std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>>
    auto itVec = work->ThermalConstrainsMap.find(name);
    if (itVec != work->ThermalConstrainsMap.end()) {
        auto& vec = itVec->second;
        for (auto& actorSP : vec) {
            if (actorSP) {
                // 从渲染器中移除 Actor
                renderer->RemoveActor(actorSP);
            }
        }
        vec.clear(); // 清空 vector
        work->ThermalConstrainsMap.erase(itVec); // 从 map 中移除该条目
    }

    // 2. 清理 ThermalConstrainsAssemblies (vtkAssembly)
    // 对应您要求的 map<string, vtkSmartPointer<vtkAssembly>>
    auto itAsm = work->ThermalConstrainsAssemblies.find(name);
    if (itAsm != work->ThermalConstrainsAssemblies.end()) {
        if (itAsm->second) {
            // 从渲染器中移除 Assembly
            renderer->RemoveActor(itAsm->second);
        }
        work->ThermalConstrainsAssemblies.erase(itAsm); // 从 map 中移除
    }
}


static void removeConstraintsByName(vtkRenderer* renderer, workData* work,
    const std::string& name) {
    if (!renderer || !work)
        return;

    // 1. 从 renderer 中移除 Arrows1[name] 中的每个 actor
    auto itVec = work->ConstrainsTranslationMap.find(name);
    if (itVec != work->ConstrainsTranslationMap.end()) {
        auto& vec = itVec->second;
        for (auto& actorSP : vec) {
            if (actorSP) {
                renderer->RemoveActor(
                    actorSP); // smartpointer 隐式转换为裸指针
            }
        }
        vec.clear();
        // 可选：erase vector entry，如果你希望后续检查不存在则 erase
        work->ConstrainsTranslationMap.erase(itVec);
    }

    auto itVec1 = work->ConstrainsRotationMap.find(name);
    if (itVec1 != work->ConstrainsRotationMap.end()) {
        auto& vec = itVec1->second;
        for (auto& actorSP : vec) {
            if (actorSP) {
                renderer->RemoveActor(
                    actorSP); // smartpointer 隐式转换为裸指针
            }
        }
        vec.clear();
        // 可选：erase vector entry，如果你希望后续检查不存在则 erase
        work->ConstrainsRotationMap.erase(itVec1);
    }


    // 2. 如果存在 assembly，先移除 renderer 中的 assembly，并 erase map
    // 以降低引用计数
    auto itAsm = work->ConstrainsAssemblies.find(name);
    if (itAsm != work->ConstrainsAssemblies.end()) {
        if (itAsm->second) {
            renderer->RemoveActor(itAsm->second);
        }
        work->ConstrainsAssemblies.erase(itAsm);
    }

}


//修改约束
bool modifyConstraintsNew(double x, double y, double z, string name,
    vtkRenderer* renderer, workData* work, double scale, string id,
    int xd, int yd, int zd, int xr, int yr, int zr, int flag)
{
    removeConstraintsByName(renderer, work, name);
    vtkSmartPointer<vtkAssembly> ConstraintsAssembly = vtkSmartPointer<vtkAssembly>::New();
    vector<guass> pts;
    if (flag == 1)
    {
        if (GetGaussPoints(work, id, scale*2, pts)) {
            for (auto& p : pts) {
                CreateConstraint(p.pts.x, p.pts.y, p.pts.z, name, renderer, work, scale, xd, yd, zd, xr, yr, zr);
            }
            for (const auto& actor : work->ConstrainsTranslationMap[name]) {
                ConstraintsAssembly->AddPart(actor);
            }
            for (const auto& actor : work->ConstrainsRotationMap[name])
            {
                ConstraintsAssembly->AddPart(actor);
            }
            renderer->AddActor(ConstraintsAssembly);
            work->ConstrainsAssemblies[name] = ConstraintsAssembly;
            return true;
        }
    }else if (flag == 2)
    {
        CreateConstraint(x, y, z, name, renderer, work, scale, xd, yd, zd, xr, yr, zr);
        for (const auto& actor : work->ConstrainsTranslationMap[name]) {
            ConstraintsAssembly->AddPart(actor);
        }
        for (const auto& actor : work->ConstrainsRotationMap[name])
        {
            ConstraintsAssembly->AddPart(actor);
        }
        renderer->AddActor(ConstraintsAssembly);
        work->ConstrainsAssemblies[name] = ConstraintsAssembly;
        return true;
    }
   
    return false;
}


// 约束
bool CreateConstraints(double x, double y, double z, string name,
    vtkRenderer* renderer, workData* work, double scale, string id,
    int xd, int yd, int zd, int xr, int yr, int zr,int flag)
{
    removeConstraintsByName(renderer, work, name);
    vtkSmartPointer<vtkAssembly> ConstraintsAssembly = vtkSmartPointer<vtkAssembly>::New();
    vector<guass> pts;
    if (flag == 1)
    {
        if (GetGaussPoints(work, id, scale*2, pts)) {
            for (auto& p : pts) {
                CreateConstraint(p.pts.x, p.pts.y, p.pts.z, name, renderer, work, scale, xd, yd, zd, xr, yr, zr);
            }
            for (const auto& actor : work->ConstrainsTranslationMap[name]) {
                ConstraintsAssembly->AddPart(actor);
            }
            for (const auto& actor : work->ConstrainsRotationMap[name])
            {
                ConstraintsAssembly->AddPart(actor);
            }
            renderer->AddActor(ConstraintsAssembly);
            work->ConstrainsAssemblies[name] = ConstraintsAssembly;
            return true;
        }
        
    }
    else if (flag == 2)
    {
        CreateConstraint(x, y, z, name, renderer, work, scale, xd, yd, zd, xr, yr, zr);
        for (const auto& actor : work->ConstrainsTranslationMap[name]) {
            ConstraintsAssembly->AddPart(actor);
        }
        for (const auto& actor : work->ConstrainsRotationMap[name])
        {
            ConstraintsAssembly->AddPart(actor);
        }
        renderer->AddActor(ConstraintsAssembly);
        work->ConstrainsAssemblies[name] = ConstraintsAssembly;
        return true;
    }
    return false;
}

// 热约束
bool CreateThermalConstraints(double x, double y, double z, double xn, double yn, double zn, string name,vtkRenderer* renderer, workData* work, string id, double scale)
{
    removeThermalConstraintsByName(renderer, work, name);
    vtkSmartPointer<vtkAssembly> ConstraintsAssembly = vtkSmartPointer<vtkAssembly>::New();
    vector<guass> pts;
   
    if (GetGaussPoints(work, id, scale * 2, pts)) {
        for (auto& p : pts) {
            CreateThermalConstraint(p.pts.x, p.pts.y, p.pts.z, xn, yn, zn, name, renderer, work, scale);
        }
        for (const auto& actor : work->ThermalConstrainsMap[name]) {
            ConstraintsAssembly->AddPart(actor);
        }
        renderer->AddActor(ConstraintsAssembly);
        work->ThermalConstrainsAssemblies[name] = ConstraintsAssembly;
        return true;
    }

    
    return false;
}

//热约束
bool CreateThermalConstraint(
    double x, double y, double z,
    double xn, double yn, double zn, // 法线向量
    string name,
    vtkRenderer* renderer, workData* work, double scale)
{
    // 热约束颜色：橙红色
    double thermalColor[3] = { 1.0, 0.4, 0.0 };

    // 1. 创建位置点和法线
    vtkSmartPointer<vtkPoints> polyPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

    // 插入坐标点
    polyPoints->InsertNextPoint(x, y, z);
    polyData->SetPoints(polyPoints);

    // 插入法线向量
    vtkSmartPointer<vtkDoubleArray> normals = vtkSmartPointer<vtkDoubleArray>::New();
    normals->SetNumberOfComponents(3);
    normals->InsertNextTuple3(xn, yn, zn);
    polyData->GetPointData()->SetNormals(normals);

    // 2. 创建几何形状：两个底部对接的棱锥

    // ⭐ 定义几何参数：减小半径使棱锥变细
    double coneHeight = 0.5;
    double coneRadius = 0.125; // ⭐ 修改：原为0.25，减小此值即可变瘦

    // === 底部棱锥 (接触面的那个) ===
    vtkSmartPointer<vtkConeSource> coneBottom = vtkSmartPointer<vtkConeSource>::New();
    coneBottom->SetResolution(6);        // 六棱锥
    coneBottom->SetHeight(coneHeight);
    coneBottom->SetRadius(coneRadius);   // ⭐ 设置较小的半径
    coneBottom->SetDirection(-1, 0, 0);  // 尖端指向 -X (指向原点)
    coneBottom->SetCenter(0.25, 0, 0);   // 中心位置不变

    // === 顶部棱锥 (远离面的那个) ===
    vtkSmartPointer<vtkConeSource> coneTop = vtkSmartPointer<vtkConeSource>::New();
    coneTop->SetResolution(6);           // 六棱锥
    coneTop->SetHeight(coneHeight);
    coneTop->SetRadius(coneRadius);      // ⭐ 设置较小的半径
    coneTop->SetDirection(1, 0, 0);      // 尖端指向 +X
    coneTop->SetCenter(0.75, 0, 0);      // 中心位置不变

    // 合并两个棱锥
    vtkSmartPointer<vtkAppendPolyData> appendFilter =
        vtkSmartPointer<vtkAppendPolyData>::New();
    appendFilter->AddInputConnection(coneBottom->GetOutputPort());
    appendFilter->AddInputConnection(coneTop->GetOutputPort());

    // 3. 使用 Glyph3D 放置图标
    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    glyph->SetInputData(polyData);
    glyph->SetSourceConnection(appendFilter->GetOutputPort());
    glyph->SetScaleFactor(scale);

    // ⭐ 关键：启用根据法线旋转
    glyph->SetVectorModeToUseNormal();
    glyph->SetScaleModeToDataScalingOff();

    // 4. 创建 Mapper 和 Actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyph->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // 设置外观
    actor->GetProperty()->SetRepresentationToWireframe();
    actor->GetProperty()->SetColor(thermalColor);
    actor->GetProperty()->SetLineWidth(2.0);

    // 5. 存入 work 数据结构
    if (work) {
        work->ThermalConstrainsMap[name].push_back(actor);
    }

    return true;
}

bool CreateConstraint(
    double x, double y, double z, string name,
    vtkRenderer* renderer, workData* work, double scale,
    int xd, int yd, int zd, int xr, int yr, int zr)
{
    double mainColor[3] = { 0.0, 1.0, 0.3 };
    double assistColor[3] = { 1.0, 0.5, 0.0 };

    vtkSmartPointer<vtkPoints> polyPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyPoints->InsertNextPoint(x, y, z);
    polyData->SetPoints(polyPoints);

    vector<bool> mainDir = { xd == 1, yd == 1, zd == 1 };
    vector<bool> assistDir = { xr == 1, yr == 1, zr == 1 };

    // 判断是否全 false
    bool hasMain = mainDir[0] || mainDir[1] || mainDir[2];
    bool hasAssist = assistDir[0] || assistDir[1] || assistDir[2];

    // ========= 主锥体 =========
    if (hasMain)
    {
        vtkSmartPointer<vtkPolyDataAlgorithm> mainCone =
            CreateTransLationCone(mainDir, 0.5);

        vtkSmartPointer<vtkGlyph3D> glyphMain =
            vtkSmartPointer<vtkGlyph3D>::New();
        glyphMain->SetInputData(polyData);
        glyphMain->SetSourceConnection(mainCone->GetOutputPort());
        glyphMain->SetScaleFactor(scale);

        vtkSmartPointer<vtkPolyDataMapper> mapperMain =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapperMain->SetInputConnection(glyphMain->GetOutputPort());

        vtkSmartPointer<vtkActor> actorMain =
            vtkSmartPointer<vtkActor>::New();
        actorMain->SetMapper(mapperMain);
        actorMain->GetProperty()->SetRepresentationToWireframe();
        actorMain->GetProperty()->SetColor(mainColor);
        actorMain->GetProperty()->SetLineWidth(2.0);

        work->ConstrainsTranslationMap[name].push_back(actorMain);
    }

    // ========= 次锥体（旋转）=========
    if (hasAssist)
    {
        vtkSmartPointer<vtkPolyDataAlgorithm> assistCone =
            CreateTransLationCone(assistDir, 0.25);

        vtkSmartPointer<vtkGlyph3D> glyphAssist =
            vtkSmartPointer<vtkGlyph3D>::New();
        glyphAssist->SetInputData(polyData);
        glyphAssist->SetSourceConnection(assistCone->GetOutputPort());
        glyphAssist->SetScaleFactor(scale * 1.2);
        glyphAssist->SetScaleModeToScaleByVector();

        vtkSmartPointer<vtkPolyDataMapper> mapperAssist =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapperAssist->SetInputConnection(glyphAssist->GetOutputPort());

        vtkSmartPointer<vtkActor> actorAssist =
            vtkSmartPointer<vtkActor>::New();
        actorAssist->SetMapper(mapperAssist);
        actorAssist->GetProperty()->SetRepresentationToWireframe();
        actorAssist->GetProperty()->SetColor(assistColor);
        actorAssist->GetProperty()->SetLineWidth(2.0);

        work->ConstrainsRotationMap[name].push_back(actorAssist);
    }

    return true;
}






// 2025-3-5

// 组合三个锥体形成固定支撑
vtkSmartPointer<vtkPolyDataAlgorithm>
CreateTransLationCone(vector<bool> &hasConstraint) {
        int freeLength = 3;
        vtkSmartPointer<vtkConeSource> *coneSource =
            new vtkSmartPointer<vtkConeSource>[freeLength];
        vtkSmartPointer<vtkTransform> *transform =
            new vtkSmartPointer<vtkTransform>[freeLength];
        vtkSmartPointer<vtkTransformFilter> *transformFilter =
            new vtkSmartPointer<vtkTransformFilter>[freeLength];

        for (int i = 0; i < freeLength; i++) {
                coneSource[i] = vtkSmartPointer<vtkConeSource>::New();
                coneSource[i]->SetHeight(1);
                coneSource[i]->SetResolution(8);
        }

        for (int i = 0; i < freeLength; i++) {
                transform[i] = vtkSmartPointer<vtkTransform>::New();
        }
        double H = coneSource[0]->GetHeight();
        transform[0]->Translate(-H / 2, 0, 0);
        transform[1]->RotateZ(90);
        transform[1]->Translate(-H / 2, 0, 0);
        transform[2]->RotateY(-90);
        transform[2]->Translate(-H / 2, 0, 0);
        for (int i = 0; i < freeLength; i++) {
                transformFilter[i] = vtkSmartPointer<vtkTransformFilter>::New();
                transformFilter[i]->SetTransform(transform[i]);
                transformFilter[i]->SetInputConnection(
                    coneSource[i]->GetOutputPort());
        }
        vtkSmartPointer<vtkAppendPolyData> appendPolyData =
            vtkSmartPointer<vtkAppendPolyData>::New();
        for (int i = 0; i < freeLength; i++) {
                if (hasConstraint[i]) {
                        appendPolyData->AddInputConnection(
                            transformFilter[i]->GetOutputPort());
                }
        }
        appendPolyData->Update();
        return appendPolyData;
}


// 组合三个锥体形成固定支撑（可控制粗细 radius）
vtkSmartPointer<vtkPolyDataAlgorithm>
CreateTransLationCone(const vector<bool>& hasConstraint, double radius)
{
    int freeLength = 3;

    vtkSmartPointer<vtkConeSource> coneSource[3];
    vtkSmartPointer<vtkTransform> transform[3];
    vtkSmartPointer<vtkTransformFilter> transformFilter[3];

    // ==================== 创建三个方向的圆锥 ====================
    for (int i = 0; i < freeLength; i++)
    {
        coneSource[i] = vtkSmartPointer<vtkConeSource>::New();
        coneSource[i]->SetHeight(1);         // 基础高度
        coneSource[i]->SetRadius(radius);    // ⭐ 设置粗细
        coneSource[i]->SetResolution(8);     // 线框圆锥
    }

    // ==================== 创建变换 ====================
    for (int i = 0; i < freeLength; i++)
    {
        transform[i] = vtkSmartPointer<vtkTransform>::New();
    }

    double H = coneSource[0]->GetHeight();

    // X-, Y-, Z- 三方向放置
    transform[0]->Translate(-H / 2, 0, 0);
    transform[1]->RotateZ(90);
    transform[1]->Translate(-H / 2, 0, 0);
    transform[2]->RotateY(-90);
    transform[2]->Translate(-H / 2, 0, 0);

    // ==================== 三个方向都加入 TransformFilter ====================
    for (int i = 0; i < freeLength; i++)
    {
        transformFilter[i] = vtkSmartPointer<vtkTransformFilter>::New();
        transformFilter[i]->SetTransform(transform[i]);
        transformFilter[i]->SetInputConnection(coneSource[i]->GetOutputPort());
    }

    // ==================== 组合成一个 PolyData ====================
    vtkSmartPointer<vtkAppendPolyData> appendPolyData =
        vtkSmartPointer<vtkAppendPolyData>::New();

    for (int i = 0; i < freeLength; i++)
    {
        if (hasConstraint[i])
        {
            appendPolyData->AddInputConnection(transformFilter[i]->GetOutputPort());
        }
    }

    appendPolyData->Update();
    return appendPolyData;
}





// 修改固定支撑
string modifyConstraint(double x, double y, double z, string name,
                        vtkRenderer *renderer, workData *work) {
        vtkSmartPointer<vtkPolyDataMapper> polyDataMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> polyDataActor =
            vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkGlyph3D> plygh3D =
            vtkSmartPointer<vtkGlyph3D>::New();
        vtkSmartPointer<vtkPoints> polyPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkTextProperty> polyTextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> polyTitle =
            vtkSmartPointer<vtkTextActor3D>::New();
        vtkSmartPointer<vtkPolyDataAlgorithm> composedCone_fix =
            vtkSmartPointer<vtkPolyDataAlgorithm>::New();
        vtkSmartPointer<vtkProperty> polyWireframeProperty =
            vtkSmartPointer<vtkProperty>::New();
        vector<bool> hasConstraint;
        polyTextProp->SetFontSize(20);
        polyTextProp->SetColor(1.0, 0.0, 0.0);
        polyTextProp->SetFontFamilyToTimes();
        polyTitle->SetTextProperty(polyTextProp);
        polyTitle->SetInput("");
        polyTitle->SetPosition(x, y, z);
        polyPoints->Initialize();
        polyPoints->InsertNextPoint(x, y, z);
        polyData->SetPoints(polyPoints);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        composedCone_fix = CreateTransLationCone(hasConstraint);
        plygh3D->SetInputData(polyData);
        plygh3D->SetSourceData(composedCone_fix->GetOutput());
        plygh3D->SetScaleFactor(40);
        polyDataMapper->SetInputConnection(plygh3D->GetOutputPort());
        polyWireframeProperty->SetRepresentationToWireframe();
        work->Constrains[name]->SetMapper(polyDataMapper);
        work->Constrains[name]->SetProperty(polyWireframeProperty);
        work->Constrains[name]->GetProperty()->SetColor(0, 1, 0);

        renderer->AddActor(work->Constrains[name]);
        renderer->AddActor(polyTitle);
        string res = "ok";

        return res;
}

// 修改固定支撑
string modifyConstraintEx(double x, double y, double z, string name,
                          vtkRenderer *renderer, workData *work, double scale) {
        vtkSmartPointer<vtkPolyDataMapper> polyDataMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> polyDataActor =
            vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkGlyph3D> plygh3D =
            vtkSmartPointer<vtkGlyph3D>::New();
        vtkSmartPointer<vtkPoints> polyPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkTextProperty> polyTextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> polyTitle =
            vtkSmartPointer<vtkTextActor3D>::New();
        vtkSmartPointer<vtkPolyDataAlgorithm> composedCone_fix =
            vtkSmartPointer<vtkPolyDataAlgorithm>::New();
        vtkSmartPointer<vtkProperty> polyWireframeProperty =
            vtkSmartPointer<vtkProperty>::New();
        vector<bool> hasConstraint;
        polyTextProp->SetFontSize(20);
        polyTextProp->SetColor(1.0, 0.0, 0.0);
        polyTextProp->SetFontFamilyToTimes();
        polyTitle->SetTextProperty(polyTextProp);
        polyTitle->SetInput("");
        polyTitle->SetPosition(x, y, z);
        polyPoints->Initialize();
        polyPoints->InsertNextPoint(x, y, z);
        polyData->SetPoints(polyPoints);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        composedCone_fix = CreateTransLationCone(hasConstraint);
        plygh3D->SetInputData(polyData);
        plygh3D->SetSourceData(composedCone_fix->GetOutput());
        plygh3D->SetScaleFactor(scale);
        polyDataMapper->SetInputConnection(plygh3D->GetOutputPort());
        polyWireframeProperty->SetRepresentationToWireframe();
        work->Constrains[name]->SetMapper(polyDataMapper);
        work->Constrains[name]->SetProperty(polyWireframeProperty);
        work->Constrains[name]->GetProperty()->SetColor(0.0, 1.0, 0.3);

        renderer->AddActor(work->Constrains[name]);
        renderer->AddActor(polyTitle);
        string res = "ok";

        return res;
}

// 创建固定支撑
string setConstraint(double x, double y, double z, string name,
                     vtkRenderer *renderer, workData *work) {
        vtkSmartPointer<vtkPolyDataMapper> polyDataMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> polyDataActor =
            vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkGlyph3D> plygh3D =
            vtkSmartPointer<vtkGlyph3D>::New();
        vtkSmartPointer<vtkPoints> polyPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkTextProperty> polyTextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> polyTitle =
            vtkSmartPointer<vtkTextActor3D>::New();
        vtkSmartPointer<vtkPolyDataAlgorithm> composedCone_fix =
            vtkSmartPointer<vtkPolyDataAlgorithm>::New();
        vtkSmartPointer<vtkProperty> polyWireframeProperty =
            vtkSmartPointer<vtkProperty>::New();
        vector<bool> hasConstraint;
        polyTextProp->SetFontSize(20);
        polyTextProp->SetColor(1.0, 0.0, 0.0);
        polyTextProp->SetFontFamilyToTimes();
        polyTitle->SetTextProperty(polyTextProp);
        polyTitle->SetInput("");
        polyTitle->SetPosition(x, y, z);
        polyPoints->Initialize();
        polyPoints->InsertNextPoint(x, y, z);
        polyData->SetPoints(polyPoints);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        composedCone_fix = CreateTransLationCone(hasConstraint);
        plygh3D->SetInputData(polyData);
        plygh3D->SetSourceData(composedCone_fix->GetOutput());
        plygh3D->SetScaleFactor(40);
        polyDataMapper->SetInputConnection(plygh3D->GetOutputPort());
        polyWireframeProperty->SetRepresentationToWireframe();
        polyDataActor->SetMapper(polyDataMapper);
        polyDataActor->SetProperty(polyWireframeProperty);
        polyDataActor->GetProperty()->SetColor(0, 1, 0);

        // int k = work->Constrains.size();
        polyDataActor->SetObjectName(name);
        work->Constrains[name] = polyDataActor;

        renderer->AddActor(work->Constrains[name]);
        renderer->AddActor(polyTitle);
        // string res = "fix " + to_string(k);
        // work->Constrains.push_back(polyDataActor);

        return name;
}

// 创建固定支撑
string setConstraintEx(double x, double y, double z, string name,
                       vtkRenderer *renderer, workData *work, double scale) {
        vtkSmartPointer<vtkPolyDataMapper> polyDataMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> polyDataActor =
            vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkGlyph3D> plygh3D =
            vtkSmartPointer<vtkGlyph3D>::New();
        vtkSmartPointer<vtkPoints> polyPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkTextProperty> polyTextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> polyTitle =
            vtkSmartPointer<vtkTextActor3D>::New();
        vtkSmartPointer<vtkPolyDataAlgorithm> composedCone_fix =
            vtkSmartPointer<vtkPolyDataAlgorithm>::New();
        vtkSmartPointer<vtkProperty> polyWireframeProperty =
            vtkSmartPointer<vtkProperty>::New();
        vector<bool> hasConstraint;
        polyTextProp->SetFontSize(20);
        polyTextProp->SetColor(1.0, 0.0, 0.0);
        polyTextProp->SetFontFamilyToTimes();
        polyTitle->SetTextProperty(polyTextProp);
        polyTitle->SetInput("");
        polyTitle->SetPosition(x, y, z);
        polyPoints->Initialize();
        polyPoints->InsertNextPoint(x, y, z);
        polyData->SetPoints(polyPoints);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        hasConstraint.push_back(true);
        composedCone_fix = CreateTransLationCone(hasConstraint);
        plygh3D->SetInputData(polyData);
        plygh3D->SetSourceData(composedCone_fix->GetOutput());
        plygh3D->SetScaleFactor(scale);
        polyDataMapper->SetInputConnection(plygh3D->GetOutputPort());
        polyWireframeProperty->SetRepresentationToWireframe();
        polyDataActor->SetMapper(polyDataMapper);
        polyDataActor->SetProperty(polyWireframeProperty);
        polyDataActor->GetProperty()->SetColor(0.0, 1.0, 0.3);

        // int k = work->Constrains.size();
        polyDataActor->SetObjectName(name);
        work->Constrains[name] = polyDataActor;

        renderer->AddActor(work->Constrains[name]);
        renderer->AddActor(polyTitle);
        // string res = "fix " + to_string(k);
        // work->Constrains.push_back(polyDataActor);

        return name;
}
// 判断两点之间距离
double distanceBetweenPoints(double x1, double y1, double z1, double x2,
                             double y2, double z2) {
        return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) +
                         std::pow(z2 - z1, 2));
}
// 修改rb3
string modifySphere(double x, double y, double z, double radius, string name,
                    vtkRenderer *renderer, workData *work) {

        vector<double> circle;

        double centerPoints[3] = {x, y, z};
        work->sphereSource->SetRadius(radius);       // 设置球体半径
        work->sphereSource->SetCenter(centerPoints); // 设置球体中心位置

        // 创建球体的映射器
        // vtkSmartPointer<vtkPolyDataMapper> mapper =
        //    vtkSmartPointer<vtkPolyDataMapper>::New();
        work->mapper->SetInputConnection(work->sphereSource->GetOutputPort());

        // 创建球体的演员（Actor）
        // vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        work->actor->SetMapper(work->mapper);
        work->actor->GetProperty()->SetColor(0.5, 0.0, 0.5);
        work->actor->GetProperty()->SetOpacity(0.2);

        vector<int> ids;
        for (int i = 0; i < work->pts.size(); i++) {
                double x = centerPoints[0] - work->pts[i].pts.x;
                double y = centerPoints[1] - work->pts[i].pts.y;
                double z = centerPoints[2] - work->pts[i].pts.z;
                double distance = distanceBetweenPoints(
                    work->pts[i].pts.x, work->pts[i].pts.y, work->pts[i].pts.z,
                    centerPoints[0], centerPoints[1], centerPoints[2]);
                if (distance <= radius) {
                        circle.push_back(work->pts[i].pts.x);
                        circle.push_back(work->pts[i].pts.y);
                        circle.push_back(work->pts[i].pts.z);

                        bool isId = true;
                        for (int j = 0; j < ids.size(); j++) {
                                if (work->pts[i].id == ids[j]) {
                                        isId = false;
                                }
                        }
                        if (isId) {
                                ids.push_back(work->pts[i].id);
                        }
                }
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(centerPoints); // 添加第一个点
        for (int i = 0; i < circle.size() / 3; i++) {
                points->InsertNextPoint(circle[i * 3], circle[i * 3 + 1],
                                        circle[i * 3 + 2]);
        }
        // 创建线条单元
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();

        for (int i = 0; i < circle.size() / 3; i++) {
                vtkIdType line[2] = {0, i + 1}; // 线条连接的点的索引
                lines->InsertNextCell(2, line); // 添加线条
        }

        // 创建 PolyData 数据对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points); // 设置点集
        polyData->SetLines(lines);   // 设置线条单元

        vtkSmartPointer<vtkPolyDataMapper> mapper_line =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_line->SetInputData(polyData);

        // int k = work->Rb3.size();
        work->Rb3[name]->SetMapper(mapper_line);
        work->Rb3[name]->GetProperty()->SetColor(0, 1, 0);

        renderer->AddActor(work->Rb3[name]);

        work->actor->SetVisibility(1);
        renderer->AddActor(work->actor);
        // work->Rb3.push_back(actor_line)

        string res = name + " Breps:";
        for (int i = 0; i < ids.size(); i++) {
                res += to_string(ids[i]) + " ";
        }

        return res;
}

string modifySphereEx(double x, double y, double z, double radius, string name,
                      vtkRenderer *renderer, workData *work,
                      vector<string> ids) {

        vector<double> circle;

        double centerPoints[3] = {x, y, z};
        work->sphereSource->SetRadius(radius);       // 设置球体半径
        work->sphereSource->SetCenter(centerPoints); // 设置球体中心位置

        // 创建球体的映射器
        // vtkSmartPointer<vtkPolyDataMapper> mapper =
        //    vtkSmartPointer<vtkPolyDataMapper>::New();
        work->mapper->SetInputConnection(work->sphereSource->GetOutputPort());

        // 创建球体的演员（Actor）
        // vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        work->actor->SetMapper(work->mapper);
        work->actor->GetProperty()->SetColor(0.5, 0.0, 0.5);
        work->actor->GetProperty()->SetOpacity(0.2);

        for (auto id : ids) {

                auto it = work->newPts.find(id);
                if (it == work->newPts.end()) {
                        continue; // 没找到这个id，跳过
                }
                vector<guass> &points = it->second;
                for (int i = 0; i < points.size();
                     i++) { // work->pts.size()高斯点类
                        double x = centerPoints[0] -
                                   points[i].pts.x; // 中心点X与高斯点x的距离
                        double y = centerPoints[1] - points[i].pts.y;
                        double z = centerPoints[2] - points[i].pts.z;
                        // distanceBetweenPoints判断两点之间距离
                        double distance = distanceBetweenPoints(
                            points[i].pts.x, points[i].pts.y, points[i].pts.z,
                            centerPoints[0], centerPoints[1], centerPoints[2]);
                        if (distance <= radius) {
                                circle.push_back(points[i].pts.x);
                                circle.push_back(points[i].pts.y);
                                circle.push_back(points[i].pts.z);
                                // 判断高斯点
                                /*bool isId = true;
                                for (int j = 0; j < GuassIds.size(); j++) {
                                    if (points[i].id == GuassIds[j]) {
                                        isId = false;
                                    }
                                }
                                if (isId) {
                                    GuassIds.push_back(points[i].id);
                                }*/
                        }
                }
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(centerPoints); // 添加第一个点
        for (int i = 0; i < circle.size() / 3; i++) {
                points->InsertNextPoint(circle[i * 3], circle[i * 3 + 1],
                                        circle[i * 3 + 2]);
        }
        // 创建线条单元
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();

        for (int i = 0; i < circle.size() / 3; i++) {
                vtkIdType line[2] = {0, i + 1}; // 线条连接的点的索引
                lines->InsertNextCell(2, line); // 添加线条
        }

        // 创建 PolyData 数据对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points); // 设置点集
        polyData->SetLines(lines);   // 设置线条单元

        vtkSmartPointer<vtkPolyDataMapper> mapper_line =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_line->SetInputData(polyData);

        // int k = work->Rb3.size();
        work->Rb3[name]->SetMapper(mapper_line);
        work->Rb3[name]->GetProperty()->SetColor(0, 1, 0);

        renderer->AddActor(work->Rb3[name]);

        work->actor->SetVisibility(1);
        renderer->AddActor(work->actor);
        // work->Rb3.push_back(actor_line)

        string res = name + " Breps:";
        for (int i = 0; i < ids.size(); i++) {
                res += ids[i] + " ";
        }

        return res;
}

string setSphereEx(double x, double y, double z, double radius, string name,
                   vtkRenderer *renderer, workData *work, vector<string> ids) {
        vector<double> circle;

        double centerPoints[3] = {x, y, z};
        work->sphereSource->SetRadius(radius);       // 设置球体半径
        work->sphereSource->SetCenter(centerPoints); // 设置球体中心位置

        // 创建球体的映射器
        // vtkSmartPointer<vtkPolyDataMapper> mapper =
        //    vtkSmartPointer<vtkPolyDataMapper>::New();
        work->mapper->SetInputConnection(work->sphereSource->GetOutputPort());

        // 创建球体的演员（Actor）
        // vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        work->actor->SetMapper(work->mapper);
        work->actor->GetProperty()->SetColor(0.5, 0.0, 0.5);
        work->actor->GetProperty()->SetOpacity(0.2);
        // 以上是球体功能
        // vector<int> GuassIds;                             // 高斯点
        for (auto id : ids) {

                auto it = work->newPts.find(id);
                if (it == work->newPts.end()) {
                        continue; // 没找到这个id，跳过
                }
                vector<guass> &points = it->second;
                for (int i = 0; i < points.size();
                     i++) { // work->pts.size()高斯点类
                        double x = centerPoints[0] -
                                   points[i].pts.x; // 中心点X与高斯点x的距离
                        double y = centerPoints[1] - points[i].pts.y;
                        double z = centerPoints[2] - points[i].pts.z;
                        // distanceBetweenPoints判断两点之间距离
                        double distance = distanceBetweenPoints(
                            points[i].pts.x, points[i].pts.y, points[i].pts.z,
                            centerPoints[0], centerPoints[1], centerPoints[2]);
                        if (distance <= radius) {
                                circle.push_back(points[i].pts.x);
                                circle.push_back(points[i].pts.y);
                                circle.push_back(points[i].pts.z);
                                // 判断高斯点
                                /*bool isId = true;
                                for (int j = 0; j < GuassIds.size(); j++) {
                                    if (points[i].id == GuassIds[j]) {
                                        isId = false;
                                    }
                                }
                                if (isId) {
                                    GuassIds.push_back(points[i].id);
                                }*/
                        }
                }
        }
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(centerPoints); // 添加第一个点
        for (int i = 0; i < circle.size() / 3; i++) {
                points->InsertNextPoint(circle[i * 3], circle[i * 3 + 1],
                                        circle[i * 3 + 2]);
        }
        // 创建线条单元
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();

        for (int i = 0; i < circle.size() / 3; i++) {
                vtkIdType line[2] = {0, i + 1}; // 线条连接的点的索引
                lines->InsertNextCell(2, line); // 添加线条
        }

        // 创建 PolyData 数据对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points); // 设置点集
        polyData->SetLines(lines);   // 设置线条单元

        vtkSmartPointer<vtkPolyDataMapper> mapper_line =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_line->SetInputData(polyData);

        vtkSmartPointer<vtkActor> actor_line = vtkSmartPointer<vtkActor>::New();
        // int k = work->Rb3.size();
        actor_line->SetMapper(mapper_line);
        actor_line->GetProperty()->SetColor(0, 1, 0);
        actor_line->SetObjectName(name);
        work->Rb3[name] = actor_line;
        renderer->AddActor(work->Rb3[name]);

        work->actor->SetVisibility(1);
        renderer->AddActor(work->actor);
        // work->Rb3.push_back(actor_line)
        // 2025-3-5
        // ====================== 新增：渲染所有点 ======================
        vtkSmartPointer<vtkPoints> allPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> vertices =
            vtkSmartPointer<vtkCellArray>::New();

        for (auto id : ids) {
                auto it = work->newPts.find(id);
                if (it == work->newPts.end()) {
                        continue;
                }

                vector<guass> &points = it->second;
                for (int i = 0; i < points.size(); i++) {
                        allPoints->InsertNextPoint(
                            points[i].pts.x, points[i].pts.y, points[i].pts.z);
                        vtkIdType pid = static_cast<vtkIdType>(
                            allPoints->GetNumberOfPoints() - 1);
                        vertices->InsertNextCell(1, &pid);
                }
        }

        vtkSmartPointer<vtkPolyData> pointCloud =
            vtkSmartPointer<vtkPolyData>::New();
        pointCloud->SetPoints(allPoints);
        pointCloud->SetVerts(vertices);

        vtkSmartPointer<vtkPolyDataMapper> pointMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        pointMapper->SetInputData(pointCloud);

        vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
        pointActor->SetMapper(pointMapper);
        pointActor->GetProperty()->SetColor(1, 0, 0); // 红色点
        pointActor->GetProperty()->SetPointSize(3);   // 点大小

        work->pointActor[name] = pointActor;
        renderer->AddActor(work->pointActor[name]);
        string res = name + " Breps:";
        for (int i = 0; i < ids.size(); i++) {
                res += ids[i] + " ";
        }

        return res;
}

string setSphere(double x, double y, double z, double radius, string name,
                 vtkRenderer *renderer, workData *work) {
        vector<double> circle;

        double centerPoints[3] = {x, y, z};
        work->sphereSource->SetRadius(radius);       // 设置球体半径
        work->sphereSource->SetCenter(centerPoints); // 设置球体中心位置

        // 创建球体的映射器
        // vtkSmartPointer<vtkPolyDataMapper> mapper =
        //    vtkSmartPointer<vtkPolyDataMapper>::New();
        work->mapper->SetInputConnection(work->sphereSource->GetOutputPort());

        // 创建球体的演员（Actor）
        // vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        work->actor->SetMapper(work->mapper);
        work->actor->GetProperty()->SetColor(0.5, 0.0, 0.5);
        work->actor->GetProperty()->SetOpacity(0.2);
        // 以上是球体功能
        vector<int> ids;                             // 高斯点
        for (int i = 0; i < work->pts.size(); i++) { // work->pts.size()高斯点类
                double x = centerPoints[0] -
                           work->pts[i].pts.x; // 中心点X与高斯点x的距离
                double y = centerPoints[1] - work->pts[i].pts.y;
                double z = centerPoints[2] - work->pts[i].pts.z;
                // distanceBetweenPoints判断两点之间距离
                double distance = distanceBetweenPoints(
                    work->pts[i].pts.x, work->pts[i].pts.y, work->pts[i].pts.z,
                    centerPoints[0], centerPoints[1], centerPoints[2]);
                if (distance <= radius) {
                        circle.push_back(work->pts[i].pts.x);
                        circle.push_back(work->pts[i].pts.y);
                        circle.push_back(work->pts[i].pts.z);
                        // 判断高斯点
                        bool isId = true;
                        for (int j = 0; j < ids.size(); j++) {
                                if (work->pts[i].id == ids[j]) {
                                        isId = false;
                                }
                        }
                        if (isId) {
                                ids.push_back(work->pts[i].id);
                        }
                }
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        points->InsertNextPoint(centerPoints); // 添加第一个点
        for (int i = 0; i < circle.size() / 3; i++) {
                points->InsertNextPoint(circle[i * 3], circle[i * 3 + 1],
                                        circle[i * 3 + 2]);
        }
        // 创建线条单元
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();

        for (int i = 0; i < circle.size() / 3; i++) {
                vtkIdType line[2] = {0, i + 1}; // 线条连接的点的索引
                lines->InsertNextCell(2, line); // 添加线条
        }

        // 创建 PolyData 数据对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points); // 设置点集
        polyData->SetLines(lines);   // 设置线条单元

        vtkSmartPointer<vtkPolyDataMapper> mapper_line =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_line->SetInputData(polyData);

        vtkSmartPointer<vtkActor> actor_line = vtkSmartPointer<vtkActor>::New();
        // int k = work->Rb3.size();
        actor_line->SetMapper(mapper_line);
        actor_line->GetProperty()->SetColor(0, 1, 0);
        actor_line->SetObjectName(name);
        work->Rb3[name] = actor_line;
        renderer->AddActor(work->Rb3[name]);

        work->actor->SetVisibility(1);
        renderer->AddActor(work->actor);
        // work->Rb3.push_back(actor_line)
        // 2025-3-5
        // ====================== 新增：渲染所有点 ======================
        vtkSmartPointer<vtkPoints> allPoints =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> vertices =
            vtkSmartPointer<vtkCellArray>::New();

        for (int i = 0; i < work->pts.size(); i++) {
                allPoints->InsertNextPoint(
                    work->pts[i].pts.x, work->pts[i].pts.y, work->pts[i].pts.z);
                vtkIdType pid = static_cast<vtkIdType>(i);
                vertices->InsertNextCell(1, &pid);
        }

        vtkSmartPointer<vtkPolyData> pointCloud =
            vtkSmartPointer<vtkPolyData>::New();
        pointCloud->SetPoints(allPoints);
        pointCloud->SetVerts(vertices);

        vtkSmartPointer<vtkPolyDataMapper> pointMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        pointMapper->SetInputData(pointCloud);

        vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
        pointActor->SetMapper(pointMapper);
        pointActor->GetProperty()->SetColor(1, 0, 0); // 红色点
        pointActor->GetProperty()->SetPointSize(3);   // 点大小

        work->pointActor[name] = pointActor;
        renderer->AddActor(work->pointActor[name]);
        string res = name + " Breps:";
        for (int i = 0; i < ids.size(); i++) {
                res += to_string(ids[i]) + " ";
        }

        return res;
}
// 梁的分段
vector<Point3D> divideLine3D(Point3D p1, Point3D p2, int N) {
        std::vector<Point3D> points;
        double deltaX = (p2.x - p1.x) / N;
        double deltaY = (p2.y - p1.y) / N;
        double deltaZ = (p2.z - p1.z) / N;

        for (int i = 0; i <= N; ++i) {
                Point3D p;
                p.x = p1.x + i * deltaX;
                p.y = p1.y + i * deltaY;
                p.z = p1.z + i * deltaZ;
                points.push_back(p);
        }

        return points;
}
// 修改梁
string modifyBeam(double seg, double x1, double y1, double z1, double x2,
                  double y2, double z2, double r, double g, double b,
                  double radius, string name, vtkRenderer *renderer,
                  workData *work) {
        string centers = "";

        Point3D p1;
        p1.x = x1;
        p1.y = y1;
        p1.z = z1;

        Point3D p2;
        p2.x = x2;
        p2.y = y2;
        p2.z = z2;

        vector<Point3D> pos = divideLine3D(p1, p2, seg);
        vector<vector<double>> nodeCoord;
        for (const auto &point : pos) {
                vector<double> Point;
                Point.push_back(point.x);
                Point.push_back(point.y);
                Point.push_back(point.z);
                nodeCoord.push_back(Point);
        }

        for (size_t i = 0; i < work->Beams[name].point.size(); i++) {
                // 获取球体中心点坐标
                double center[3] = {nodeCoord[i][0], nodeCoord[i][1],
                                    nodeCoord[i][2]};

                // 创建球体对象
                vtkSmartPointer<vtkSphereSource> sphereSource =
                    vtkSmartPointer<vtkSphereSource>::New();
                sphereSource->SetCenter(center);
                sphereSource->SetRadius(radius);
                centers += std::to_string(center[0]) + " " +
                           std::to_string(center[1]) + " " +
                           std::to_string(center[2]) + " ";
                // 映射器
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                    vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(sphereSource->GetOutputPort());

                work->Beams[name].point[i]->SetMapper(mapper);
                work->Beams[name].point[i]->GetProperty()->SetColor(r, g, b);

                renderer->AddActor(work->Beams[name].point[i]);
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

        for (size_t i = 0; i < nodeCoord.size(); ++i) {
                points->InsertNextPoint(
                    nodeCoord[i].data()); // 将 vector 转换为 double*
        }

        // 创建线段集合
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();

        for (size_t i = 0; i < nodeCoord.size() - 1; ++i) {
                vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
                line->GetPointIds()->SetId(0, i);     // 第一个点的索引
                line->GetPointIds()->SetId(1, i + 1); // 第二个点的索引
                lines->InsertNextCell(line);
        }

        // 创建 PolyData 对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetLines(lines);

        // 创建 Mapper
        vtkSmartPointer<vtkPolyDataMapper> mapper_line =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_line->SetInputData(polyData);

        // 创建 Actor

        work->Beams[name].line[0]->SetMapper(mapper_line);
        work->Beams[name].line[0]->SetObjectName(name);
        work->Beams[name].line[0]->GetProperty()->SetColor(r, g, b);
        work->Beams[name].line[0]->GetProperty()->SetLineWidth(2.0);

        renderer->AddActor(work->Beams[name].line[0]);

        string res = "types:" + name;
        res = res + " centers:" + centers;

        return res;
}
// 创建梁，seg为段数
string setBeam(double seg, double x1, double y1, double z1, double x2,
               double y2, double z2, double r, double g, double b,
               double radius, string name, vtkRenderer *renderer,
               workData *work) {
        string centers = "";
        beamsData bs;
        Point3D p1;
        p1.x = x1;
        p1.y = y1;
        p1.z = z1;

        Point3D p2;
        p2.x = x2;
        p2.y = y2;
        p2.z = z2;

        vector<Point3D> pos = divideLine3D(p1, p2, seg);
        vector<vector<double>> nodeCoord;
        for (const auto &point : pos) {
                vector<double> Point;
                Point.push_back(point.x);
                Point.push_back(point.y);
                Point.push_back(point.z);
                nodeCoord.push_back(Point);
        }

        for (size_t i = 0; i < nodeCoord.size(); i++) {
                // 获取球体中心点坐标
                double center[3] = {nodeCoord[i][0], nodeCoord[i][1],
                                    nodeCoord[i][2]};

                // 创建球体对象
                vtkSmartPointer<vtkSphereSource> sphereSource =
                    vtkSmartPointer<vtkSphereSource>::New();
                sphereSource->SetCenter(center);
                sphereSource->SetRadius(radius);
                centers += std::to_string(center[0]) + " " +
                           std::to_string(center[1]) + " " +
                           std::to_string(center[2]) + " ";
                // 映射器
                vtkSmartPointer<vtkPolyDataMapper> mapper =
                    vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(sphereSource->GetOutputPort());

                // 创建演员并添加到渲染器
                vtkSmartPointer<vtkActor> actor =
                    vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);
                actor->SetObjectName(name + " " + to_string(i));
                actor->GetProperty()->SetColor(r, g, b);
                bs.point.push_back(actor);
                renderer->AddActor(actor);
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

        for (size_t i = 0; i < nodeCoord.size(); ++i) {
                points->InsertNextPoint(
                    nodeCoord[i].data()); // 将 vector 转换为 double*
        }

        // 创建线段集合
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();

        for (size_t i = 0; i < nodeCoord.size() - 1; ++i) {
                vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
                line->GetPointIds()->SetId(0, i);     // 第一个点的索引
                line->GetPointIds()->SetId(1, i + 1); // 第二个点的索引
                lines->InsertNextCell(line);
        }

        // 创建 PolyData 对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetLines(lines);

        // 创建 Mapper
        vtkSmartPointer<vtkPolyDataMapper> mapper_line =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_line->SetInputData(polyData);

        // 创建 Actor
        vtkSmartPointer<vtkActor> actor_line = vtkSmartPointer<vtkActor>::New();
        actor_line->SetMapper(mapper_line);
        actor_line->SetObjectName(name);
        actor_line->GetProperty()->SetColor(r, g, b);
        actor_line->GetProperty()->SetLineWidth(2.0);
        bs.line.push_back(actor_line);

        work->Beams[name] = bs;

        renderer->AddActor(actor_line);

        string res = "types:" + name;
        res = res + " centers:" + centers;

        return res;
}
// 修改弹簧
string modifySpring(double x, double y, double z, double X, double Y, double Z,
                    double r, double g, double b, string name,
                    vtkRenderer *renderer, workData *work) {

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        // points->InsertNextPoint(pointCallBacks->pickPositions); //
        // 添加第一个点

        points->InsertNextPoint(x, y, z);
        points->InsertNextPoint(X, Y, Z);
        // 创建线条单元
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();
        vtkIdType line[2] = {0, 1};     // 线条连接的点的索引
        lines->InsertNextCell(2, line); // 添加线条

        // 创建 PolyData 数据对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points); // 设置点集
        polyData->SetLines(lines);   // 设置线条单元

        vtkSmartPointer<vtkPolyDataMapper> mapper_s =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_s->SetInputData(polyData);

        work->Spring[name]->SetMapper(mapper_s);
        work->Spring[name]->GetProperty()->SetColor(r, g, b);

        work->Spring[name]->SetObjectName(name);

        renderer->AddActor(work->Spring[name]);

        return name;
}
// 创建弹簧
string setspring(double x, double y, double z, double X, double Y, double Z,
                 double r, double g, double b, string name,
                 vtkRenderer *renderer, workData *work) {

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        // points->InsertNextPoint(pointCallBacks->pickPositions); //
        // 添加第一个点

        points->InsertNextPoint(x, y, z);
        points->InsertNextPoint(X, Y, Z);
        // 创建线条单元
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();
        vtkIdType line[2] = {0, 1};     // 线条连接的点的索引
        lines->InsertNextCell(2, line); // 添加线条

        // 创建 PolyData 数据对象
        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points); // 设置点集
        polyData->SetLines(lines);   // 设置线条单元

        vtkSmartPointer<vtkPolyDataMapper> mapper_s =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_s->SetInputData(polyData);

        vtkSmartPointer<vtkActor> actor_s = vtkSmartPointer<vtkActor>::New();
        actor_s->SetMapper(mapper_s);
        actor_s->GetProperty()->SetColor(r, g, b);

        actor_s->SetObjectName(name);
        work->Spring[name] = actor_s;

        renderer->AddActor(work->Spring[name]);

        return name;
}
// 修改连接
string modifyConnect(double x, double y, double z, double X, double Y, double Z,
                     double r, double g, double b, string name,
                     vtkRenderer *renderer, workData *work) {

        // int connectId = work->Connects.size();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

        points->InsertNextPoint(x, y, z);
        points->InsertNextPoint(X, Y, Z);

        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();
        vtkIdType line[2] = {0, 1};
        lines->InsertNextCell(2, line);

        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetLines(lines);

        vtkSmartPointer<vtkPolyDataMapper> mapper_s =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_s->SetInputData(polyData);

        work->Connects[name]->SetMapper(mapper_s);
        work->Connects[name]->GetProperty()->SetColor(r, g, b);
        work->Connects[name]->SetObjectName(name);

        renderer->AddActor(work->Connects[name]);

        return name;
}
// 创建连接
string setConnect(double x, double y, double z, double X, double Y, double Z,
                  double r, double g, double b, string name,
                  vtkRenderer *renderer, workData *work) {

        // int connectId = work->Connects.size();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

        points->InsertNextPoint(x, y, z);
        points->InsertNextPoint(X, Y, Z);

        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();
        vtkIdType line[2] = {0, 1};
        lines->InsertNextCell(2, line);

        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetLines(lines);

        vtkSmartPointer<vtkPolyDataMapper> mapper_s =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper_s->SetInputData(polyData);

        vtkSmartPointer<vtkActor> actor_s = vtkSmartPointer<vtkActor>::New();
        actor_s->SetMapper(mapper_s);
        actor_s->GetProperty()->SetColor(r, g, b);
        actor_s->SetObjectName(name);

        work->Connects[name] = actor_s;

        renderer->AddActor(work->Connects[name]);

        return name;
}
// 关闭背景网格-边界-背景网格
void OnCloseMesh(vtkRenderer *renderer, workData *work) {
        work->actor_Mesh->SetVisibility(0);
        renderer->AddActor(work->actor_Mesh);
}
// 创建背景网格
void OnOpenMesh(Point3D startPoint, Point3D endPoint, double numXSegments,
                double numYSegments, double numZSegments, vtkRenderer *renderer,
                workData *work) {

        work->actor_Mesh->New();
        work->actor_Mesh->SetVisibility(1);
        double xMin = startPoint.x;
        double yMin = startPoint.y;
        double zMin = startPoint.z;
        double xMax = endPoint.x;
        double yMax = endPoint.y;
        double zMax = endPoint.z;
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        for (int k = 0; k <= numZSegments; ++k) {
                double z = zMin + (zMax - zMin) * k / numZSegments;
                for (int j = 0; j <= numYSegments; ++j) {
                        double y = yMin + (yMax - yMin) * j / numYSegments;
                        for (int i = 0; i <= numXSegments; ++i) {
                                double x =
                                    xMin + (xMax - xMin) * i / numXSegments;
                                points->InsertNextPoint(x, y, z);
                        }
                }
        }
        vtkSmartPointer<vtkCellArray> lines =
            vtkSmartPointer<vtkCellArray>::New();
        // 沿X方向添加线
        for (int k = 0; k <= numZSegments; ++k) {
                for (int j = 0; j <= numYSegments; ++j) {
                        for (int i = 0; i < numXSegments; ++i) {
                                int pointId1 =
                                    j * (numXSegments + 1) + i +
                                    k * (numXSegments + 1) * (numYSegments + 1);
                                int pointId2 = pointId1 + 1;
                                vtkSmartPointer<vtkLine> line =
                                    vtkSmartPointer<vtkLine>::New();
                                line->GetPointIds()->SetId(0, pointId1);
                                line->GetPointIds()->SetId(1, pointId2);
                                lines->InsertNextCell(line);
                        }
                }
        }

        // 沿Y方向添加线
        for (int k = 0; k <= numZSegments; ++k) {
                for (int i = 0; i <= numXSegments; ++i) {
                        for (int j = 0; j < numYSegments; ++j) {
                                int pointId1 =
                                    j * (numXSegments + 1) + i +
                                    k * (numXSegments + 1) * (numYSegments + 1);
                                int pointId2 = pointId1 + (numXSegments + 1);
                                vtkSmartPointer<vtkLine> line =
                                    vtkSmartPointer<vtkLine>::New();
                                line->GetPointIds()->SetId(0, pointId1);
                                line->GetPointIds()->SetId(1, pointId2);
                                lines->InsertNextCell(line);
                        }
                }
        }

        // 沿Z方向添加线
        for (int j = 0; j <= numYSegments; ++j) {
                for (int i = 0; i <= numXSegments; ++i) {
                        for (int k = 0; k < numZSegments; ++k) {
                                int pointId1 =
                                    j * (numXSegments + 1) + i +
                                    k * (numXSegments + 1) * (numYSegments + 1);
                                int pointId2 =
                                    pointId1 +
                                    (numXSegments + 1) * (numYSegments + 1);
                                vtkSmartPointer<vtkLine> line =
                                    vtkSmartPointer<vtkLine>::New();
                                line->GetPointIds()->SetId(0, pointId1);
                                line->GetPointIds()->SetId(1, pointId2);
                                lines->InsertNextCell(line);
                        }
                }
        }

        vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetLines(lines);
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(polyData);
        // vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        work->actor_Mesh->SetMapper(mapper);
        work->actor_Mesh->GetProperty()->SetColor(
            1.0, 1.0, 1.0); // 设置线的颜色，这里为黑色，可按��调整
        renderer->AddActor(work->actor_Mesh);
}

// 按前缀和后缀排序存储文件到二维vector中
void getFilesClassified(string path, vector<vector<string>> &classifiedFiles) {
        cout << "[DEBUG] getFilesClassified 开始处理路径: " << path << endl;

        // 检查路径是否存在
        if (path.empty()) {
                cout << "[ERROR] 路径为空!" << endl;
                return;
        }

        // 临时存储：前缀 -> (后缀数字 -> 文件路径)
        map<string, map<int, string>> prefixSuffixMap;

        // 文件句柄
        intptr_t hFile = 0;
        // 文件信息
        struct _finddata_t fileinfo;
        string p;

        try {
                string searchPattern = p.assign(path).append("\\*");
                cout << "[DEBUG] 搜索模式: " << searchPattern << endl;

                if ((hFile = _findfirst(searchPattern.c_str(), &fileinfo)) !=
                    -1) {
                        int fileCount = 0;
                        int vtsFileCount = 0;

                        do {
                                fileCount++;
                                cout << "[DEBUG] 处理文件/目录 #" << fileCount
                                     << ": " << fileinfo.name << endl;

                                // 如果是目录,递归之
                                if ((fileinfo.attrib & _A_SUBDIR)) {
                                        if (strcmp(fileinfo.name, ".") != 0 &&
                                            strcmp(fileinfo.name, "..") != 0) {
                                                cout << "[DEBUG] "
                                                        "递归处理子目录: "
                                                     << fileinfo.name << endl;
                                                getFilesClassified(
                                                    p.assign(path)
                                                        .append("\\")
                                                        .append(fileinfo.name),
                                                    classifiedFiles);
                                        }
                                } else {
                                        // 处理文件
                                        string filename = fileinfo.name;
                                        string fullPath =
                                            p.assign(path).append("\\").append(
                                                fileinfo.name);
                                        cout << "[DEBUG] 处理文件: " << filename
                                             << " -> " << fullPath << endl;

                                        // 使用正则表达式解析文件名：前缀postprocess后缀.vts
                                        std::regex pattern(
                                            "^(\\d+)postprocess(\\d+)\\.vts$");
                                        std::smatch matches;

                                        if (std::regex_search(filename, matches,
                                                              pattern)) {
                                                vtsFileCount++;
                                                string prefixStr =
                                                    matches[1]
                                                        .str(); // 例如："2",
                                                                // "3", "6"
                                                string suffixStr =
                                                    matches[2]
                                                        .str(); // 例如："00",
                                                                // "10", "20"

                                                cout
                                                    << "[DEBUG] 匹配VTS文件: "
                                                    << filename
                                                    << " -> 前缀: " << prefixStr
                                                    << ", 后缀: " << suffixStr
                                                    << endl;

                                                try {
                                                        int prefixNum = std::stoi(
                                                            prefixStr); // 转换为数字用于排序
                                                        int suffixNum = std::stoi(
                                                            suffixStr); // 转换为数字用于排序

                                                        // 存储到嵌套map中
                                                        prefixSuffixMap
                                                            [prefixStr]
                                                            [suffixNum] =
                                                                fullPath;
                                                        cout
                                                            << "[DEBUG] "
                                                               "成功存储: 前缀="
                                                            << prefixNum
                                                            << ", 后缀="
                                                            << suffixNum
                                                            << endl;
                                                } catch (
                                                    const std::exception &e) {
                                                        cout << "[ERROR] "
                                                                "数字转换失败: "
                                                             << e.what()
                                                             << " (前缀: "
                                                             << prefixStr
                                                             << ", 后缀: "
                                                             << suffixStr << ")"
                                                             << endl;
                                                }
                                        } else {
                                                cout << "[DEBUG] "
                                                        "文件不匹配VTS模式: "
                                                     << filename << endl;
                                        }
                                }
                        } while (_findnext(hFile, &fileinfo) == 0);

                        _findclose(hFile);
                        cout << "[DEBUG] 文件扫描完成. 总文件数: " << fileCount
                             << ", VTS文件数: " << vtsFileCount << endl;
                } else {
                        cout << "[ERROR] 无法打开目录: " << path
                             << " (错误码: " << errno << ")" << endl;
                        return;
                }
        } catch (const std::exception &e) {
                cout << "[ERROR] getFilesClassified 异常: " << e.what() << endl;
                if (hFile != -1) {
                        _findclose(hFile);
                }
                return;
        }

        // 清空输出容器
        classifiedFiles.clear();

        cout << "[DEBUG] 开始组织分类结果, 前缀组数: " << prefixSuffixMap.size()
             << endl;

        // 按前缀排序（字符串自然排序：1, 2, 3, 6...）
        for (const auto &prefixGroup : prefixSuffixMap) {
                vector<string> suffixSortedFiles;
                cout << "[DEBUG] 处理前缀组: " << prefixGroup.first
                     << ", 文件数: " << prefixGroup.second.size() << endl;

                // 按后缀数字排序（0, 10, 20, 30...）
                for (const auto &suffixFile : prefixGroup.second) {
                        suffixSortedFiles.push_back(suffixFile.second);
                        cout << "[DEBUG]   添加文件: 后缀=" << suffixFile.first
                             << " -> " << suffixFile.second << endl;
                }

                // 添加到结果中
                classifiedFiles.push_back(suffixSortedFiles);
                cout << "[DEBUG] 前缀组 " << prefixGroup.first << " 完成, 包含 "
                     << suffixSortedFiles.size() << " 个文件" << endl;
        }

        cout << "[DEBUG] getFilesClassified 完成. 总前缀组数: "
             << classifiedFiles.size() << endl;
}

// 筛选模态需要的文件（很少用）
void getFiles(string path, vector<string> &files) {
        // 文件句柄
        intptr_t hFile = 0;
        // 文件信息
        struct _finddata_t fileinfo;
        string p;
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(),
                                &fileinfo)) != -1) {
                do {
                        // 如果是目录,迭代之
                        // 如果不是,加入列表
                        if ((fileinfo.attrib & _A_SUBDIR)) {
                                if (strcmp(fileinfo.name, ".") != 0 &&
                                    strcmp(fileinfo.name, "..") != 0)
                                        getFiles(
                                            p.assign(path).append("\\").append(
                                                fileinfo.name),
                                            files);
                        } else {
                                files.push_back(
                                    p.assign(path).append("\\").append(
                                        fileinfo.name));
                        }
                } while (_findnext(hFile, &fileinfo) == 0);
                _findclose(hFile);
        }
}
// 模态筛选
std::string extractFileName(const std::string &path) {
        // 查找最后一个斜杠的位置
        size_t pos = path.find_last_of("/\\");
        // 提取文件名部分
        return (pos == std::string::npos) ? path : path.substr(pos + 1);
}
// 删除文件（模态）
void deleteFilesByExtension(const char *directory, const char *extension) {
        try {
                for (const auto &entry : fs::directory_iterator(directory)) {
                        if (entry.is_regular_file() &&
                            entry.path().extension() == extension) {
                                fs::remove(entry.path());
                                std::cout
                                    << "已删除文件: " << entry.path().string()
                                    << std::endl;
                        }
                }
        } catch (fs::filesystem_error &e) {
                std::cout << "出现错误: " << e.what() << std::endl;
        }
}
// 删除所有文件（模态）
void deleteAllFiles(string folderPath) {
        try {
                for (const auto &entry : fs::directory_iterator(folderPath)) {
                        if (fs::is_regular_file(entry)) {
                                fs::remove(entry.path());
                        }
                }
                // std::cout << "文件夹下所有文件已成功删除。" << std::endl;
        } catch (const fs::filesystem_error &e) {
                // std::cerr << "出现错误: " << e.what() << std::endl;
        }
}




// 辅助函数：统计网格中的孔洞数量
int CountHoles(vtkPolyData* poly) {
    if (!poly) return 0;

    // 1. 提取边界边 (Boundary Edges)
    // 边界边就是那些只属于一个三角形的边，也就是洞的边缘
    vtkSmartPointer<vtkFeatureEdges> edges = vtkSmartPointer<vtkFeatureEdges>::New();
    edges->SetInputData(poly);
    edges->BoundaryEdgesOn();      // 开启边界检测
    edges->FeatureEdgesOff();      // 关闭特征棱
    edges->ManifoldEdgesOff();
    edges->NonManifoldEdgesOff();
    edges->Update();

    // 2. 统计连通区域
    // 每一个连通的边界线圈，代表一个洞
    vtkSmartPointer<vtkConnectivityFilter> connect = vtkSmartPointer<vtkConnectivityFilter>::New();
    connect->SetInputConnection(edges->GetOutputPort());
    connect->SetExtractionModeToAllRegions();
    connect->ColorRegionsOn();
    connect->Update();

    return connect->GetNumberOfExtractedRegions();
}

// 辅助函数：处理等值线几何切割 (带补洞和统计)
vtkSmartPointer<vtkPolyData> processIsolineGeometry(vtkPolyData* inputPoly, int isoLevels) {
    if (!inputPoly || inputPoly->GetNumberOfPoints() == 0) return nullptr;

    cout << "\n[DEBUG] [Repair] === 开始网格修复流程 ===" << endl;

    // --- 步骤 0: 初始诊断 ---
    int holesBefore = CountHoles(inputPoly);
    cout << "[DEBUG] [Repair] 修复前检测到缝隙/孔洞数量: " << holesBefore << endl;

    // --- 步骤 1: 基础清洗 (合并微小裂缝) ---
    vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
    cleaner->SetInputData(inputPoly);
    cleaner->PointMergingOn();
    cleaner->ConvertLinesToPointsOn();
    cleaner->ConvertPolysToLinesOn();
    cleaner->ConvertStripsToPolysOn();
    cleaner->Update();

    // --- 步骤 2: 填补大洞 (Fill Holes) ---
    // 这里设置 HoleSize 为一个较大的值，以确保能填补大缝隙
    vtkSmartPointer<vtkFillHolesFilter> filler = vtkSmartPointer<vtkFillHolesFilter>::New();
    filler->SetInputConnection(cleaner->GetOutputPort());
    filler->SetHoleSize(10000.0); // 设置孔洞大小上限，数值越大补的洞越大
    filler->Update();

    // --- 步骤 3: 确保三角化 ---
    // 补洞后可能会产生非三角形的多边形，需要转回三角形
    vtkSmartPointer<vtkTriangleFilter> triangulator = vtkSmartPointer<vtkTriangleFilter>::New();
    triangulator->SetInputConnection(filler->GetOutputPort());
    triangulator->PassVertsOff();
    triangulator->PassLinesOff();
    triangulator->Update();

    vtkPolyData* cleanPoly = triangulator->GetOutput();

    // --- 步骤 4: 修复后诊断 ---
    int holesAfter = CountHoles(cleanPoly);
    cout << "[DEBUG] [Repair] 修复后剩余缝隙/孔洞数量: " << holesAfter << endl;

    if (holesBefore > 0 && holesAfter == 0) {
        cout << "[DEBUG] [Repair] 完美！所有缝隙已闭合。" << endl;
    }
    else if (holesAfter > 0) {
        cout << "[WARNING] [Repair] 仍有 " << holesAfter << " 个缝隙无法闭合，可能是非流形结构。" << endl;
    }
    cout << "[DEBUG] [Repair] === 修复流程结束 ===\n" << endl;

    // ===========================================
    // 以下为原有的等值线切割逻辑 (保持不变)
    // ===========================================

    TriangleMeshIsoLine::TriangleMesh inputMesh;

    vtkDataArray* scalars = cleanPoly->GetPointData()->GetScalars();
    if (!scalars) return nullptr;

    vtkDataArray* inputNormals = cleanPoly->GetPointData()->GetNormals();
    bool hasNormals = (inputNormals != nullptr);

    vtkIdType numPoints = cleanPoly->GetNumberOfPoints();
    for (vtkIdType i = 0; i < numPoints; i++) {
        double p[3];
        cleanPoly->GetPoint(i, p);
        double val = scalars->GetTuple1(i);

        double nx = 0.0, ny = 0.0, nz = 1.0;
        if (hasNormals) {
            double n[3];
            inputNormals->GetTuple(i, n);
            nx = n[0]; ny = n[1]; nz = n[2];
        }

        if (!std::isnan(val) && !std::isinf(val)) {
            inputMesh.addNode(p[0], p[1], p[2], val, nx, ny, nz);
        }
        else {
            inputMesh.addNode(p[0], p[1], p[2], 0.0, nx, ny, nz);
        }
    }

    cleanPoly->GetPolys()->InitTraversal();
    vtkNew<vtkIdList> idList;
    while (cleanPoly->GetPolys()->GetNextCell(idList)) {
        if (idList->GetNumberOfIds() == 3) {
            inputMesh.addTriangle(idList->GetId(0), idList->GetId(1), idList->GetId(2));
        }
    }

    double minVal, maxVal;
    inputMesh.getValueRange(minVal, maxVal);
    if (std::abs(maxVal - minVal) < 1e-6) maxVal = minVal + 1.0;

    std::vector<double> isoValues;
    double step = (maxVal - minVal) / isoLevels;
    for (int i = 1; i < isoLevels; i++) {
        isoValues.push_back(minVal + i * step);
    }

    TriangleMeshIsoLine::TriangleMesh splitMesh = inputMesh.splitByIsolines(isoValues);

    vtkSmartPointer<vtkPolyData> resultPoly = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> newPolys = vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkDoubleArray> cellScalars = vtkSmartPointer<vtkDoubleArray>::New();
    cellScalars->SetName("DeformationMagnitude");

    vtkSmartPointer<vtkFloatArray> newNormals = vtkSmartPointer<vtkFloatArray>::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->SetName("Normals");

    const auto& coords = splitMesh.getNodeCoords();
    const auto& norms = splitMesh.getNodeNormals();
    size_t newNumNodes = splitMesh.getNodeCount();

    newPoints->Allocate(newNumNodes);
    newNormals->SetNumberOfTuples(newNumNodes);

    for (size_t i = 0; i < newNumNodes; i++) {
        newPoints->InsertNextPoint(coords[i * 3], coords[i * 3 + 1], coords[i * 3 + 2]);
        newNormals->SetTuple3(i, norms[i * 3], norms[i * 3 + 1], norms[i * 3 + 2]);
    }

    const auto& elems = splitMesh.getElements();
    const auto& nodeVals = splitMesh.getNodeValues();
    size_t newNumTris = splitMesh.getTriangleCount();

    newPolys->Allocate(newNumTris);
    cellScalars->SetNumberOfComponents(1);
    cellScalars->SetNumberOfTuples(newNumTris);

    for (size_t i = 0; i < newNumTris; i++) {
        vtkIdType ids[3] = { (vtkIdType)elems[i * 3], (vtkIdType)elems[i * 3 + 1], (vtkIdType)elems[i * 3 + 2] };
        newPolys->InsertNextCell(3, ids);
        double avgVal = (nodeVals[ids[0]] + nodeVals[ids[1]] + nodeVals[ids[2]]) / 3.0;
        cellScalars->SetTuple1(i, avgVal);
    }

    resultPoly->SetPoints(newPoints);
    resultPoly->SetPolys(newPolys);
    resultPoly->GetCellData()->SetScalars(cellScalars);
    if (hasNormals) {
        resultPoly->GetPointData()->SetNormals(newNormals);
    }

    return resultPoly;
}




string OnModelVTKLoadWithDeformation(string path, string path1,
    double deformationScale,
    vtkRenderer* renderer, postManage* work) {

    cout << "\n[DEBUG] >>> OnModelVTKLoadWithDeformation 开始 <<<" << endl;

    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkXMLStructuredGridReader> readers = vtkSmartPointer<vtkXMLStructuredGridReader>::New();
    vtkSmartPointer<vtkPolyData> targetData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkWarpVector> warpVector = vtkSmartPointer<vtkWarpVector>::New();
    vtkSmartPointer<vtkResampleWithDataSet> resampler = vtkSmartPointer<vtkResampleWithDataSet>::New();

    double range[2] = { 0, 0 };
    renderer->RemoveAllViewProps();

    // --- 1. 加载 STL ---
    if (work->cachedStlData != nullptr && work->cachedStlData->GetNumberOfPoints() > 0) {
        cout << "[DEBUG] 使用缓存 STL" << endl;
        targetData->DeepCopy(work->cachedStlData);
    }
    else {
        string pathSTL = path + "/data/locomotive_bogieassembly_sldasm2.stl";
        ifstream file(pathSTL.c_str());
        if (!file.good()) return "Error: STL文件不存在";
        file.close();

        stlReader->SetFileName(pathSTL.c_str());
        stlReader->Update();
        targetData = stlReader->GetOutput();

        if (!targetData || targetData->GetNumberOfPoints() == 0) return "Error: STL文件加载失败";
    }

    // --- 2. 加载 VTS ---
    readers->SetFileName(path1.c_str());
    readers->Update();
    structuredGrid = readers->GetOutput();

    vtkDataArray* dataArray = structuredGrid->GetPointData()->GetArray(0);
    if (!dataArray) return "Error: 无法获取数据数组";

    vtkIdType numValues = dataArray->GetNumberOfTuples();

    vtkSmartPointer<vtkDoubleArray> MagnitudeData = vtkSmartPointer<vtkDoubleArray>::New();
    MagnitudeData->SetName("DeformationMagnitude");
    MagnitudeData->SetNumberOfComponents(1);
    MagnitudeData->SetNumberOfTuples(numValues);

    vtkNew<vtkDoubleArray> warpData;
    warpData->SetName("Deformation");
    warpData->SetNumberOfComponents(3);
    warpData->SetNumberOfTuples(numValues);

    vector<double> values;
    values.reserve(numValues);

    for (vtkIdType i = 0; i < numValues; i++) {
        double vx = dataArray->GetComponent(i, 0);
        double vy = dataArray->GetComponent(i, 1);
        double vz = dataArray->GetComponent(i, 2);
        double mag = std::sqrt(vx * vx + vy * vy + vz * vz);

        MagnitudeData->InsertTuple1(i, mag);
        double scaledWarp[] = { vx * deformationScale, vy * deformationScale, vz * deformationScale };
        warpData->SetTuple(i, scaledWarp);
        values.push_back(mag);
    }

    std::sort(values.begin(), values.end());
    range[0] = values[values.size() / 100 * 5];
    range[1] = values[values.size() - values.size() / 100 * 5];

    structuredGrid->GetPointData()->SetScalars(MagnitudeData);
    structuredGrid->GetPointData()->SetVectors(warpData);

    // --- 3. 重采样与形变 ---
    resampler->SetInputData(targetData);
    resampler->SetSourceData(structuredGrid);
    resampler->Update();


    warpVector->SetInputConnection(resampler->GetOutputPort());
    warpVector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Deformation");
    warpVector->SetScaleFactor(1.0);
    warpVector->Update();

    // --- 4. 【关键步骤】变形后重算法线 ---
    // 先计算法线，供切割算法使用
    vtkSmartPointer<vtkPolyDataNormals> normalsFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsFilter->SetInputConnection(warpVector->GetOutputPort());
    normalsFilter->ComputePointNormalsOn();
    normalsFilter->SplittingOff();
    normalsFilter->Update();

    // --- 5. 【关键步骤】等值线切割 (含自动补洞) ---
    int lutNum = 10;
    // 注意：这里我们传入 normalsFilter 的输出，
    // processIsolineGeometry 内部会先执行 补洞(FillHoles) 再执行 切割
    vtkSmartPointer<vtkPolyData> isoPoly = processIsolineGeometry(normalsFilter->GetOutput(), lutNum);

    // --- 6. 渲染设置 ---
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(lutNum);
    lut->SetRange(range[0], range[1]);
    lut->Build();

    vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
    ctf->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
    ctf->AddRGBPoint(0.125, 0.12, 0.32, 0.85);
    ctf->AddRGBPoint(0.25, 0.37, 0.71, 1.0);
    ctf->AddRGBPoint(0.375, 0.38, 1.0, 0.43);
    ctf->AddRGBPoint(0.5, 0.0, 1.0, 0.0);
    ctf->AddRGBPoint(0.625, 0.79, 1.0, 0.09);
    ctf->AddRGBPoint(0.75, 1.0, 1.0, 0.0);
    ctf->AddRGBPoint(0.875, 1.0, 0.70, 0.09);
    ctf->AddRGBPoint(0.95, 1.0, 0.38, 0.0);
    ctf->AddRGBPoint(1.0, 1.0, 0.0, 0.0);

    for (int i = 0; i < lutNum; i++) {
        double val = range[0] + ((double)i / (lutNum - 1)) * (range[1] - range[0]);
        double rgb[3];
        ctf->GetColor(val, rgb);
        lut->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
    }

    if (isoPoly && isoPoly->GetNumberOfCells() > 0) {
        mapper->SetInputData(isoPoly);
        mapper->SetScalarModeToUseCellData();
        mapper->SetColorModeToMapScalars();
        mapper->InterpolateScalarsBeforeMappingOff();
        cout << "[DEBUG] 使用等值线切割网格 (已修复缝隙)" << endl;
    }
    else {
        cout << "[WARNING] 切割失败，回退到普通渲染" << endl;
        mapper->SetInputConnection(warpVector->GetOutputPort());
        mapper->SetScalarModeToUsePointData();
    }

    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(range[0], range[1]);

    actor->SetMapper(mapper);
    actor->GetProperty()->SetInterpolationToFlat();
    actor->GetProperty()->SetSpecular(0.2);
    actor->GetProperty()->SetSpecularPower(10);

    // ScalarBar 设置
    vtkSmartPointer<vtkScalarBarActor> scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBarActor->SetLookupTable(lut);
    scalarBarActor->SetTitle("Deformation");
    scalarBarActor->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);
    scalarBarActor->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);
    scalarBarActor->SetLabelFormat("%.3e");
    scalarBarActor->GetTitleTextProperty()->SetFontSize(40);
    scalarBarActor->GetLabelTextProperty()->SetFontSize(40);
    scalarBarActor->SetMaximumWidthInPixels(160);
    scalarBarActor->SetVerticalTitleSeparation(10);
    scalarBarActor->GetTitleTextProperty()->SetShadow(false);
    scalarBarActor->GetTitleTextProperty()->SetFontFamilyToArial();
    scalarBarActor->GetLabelTextProperty()->SetShadow(false);
    scalarBarActor->GetLabelTextProperty()->SetFontFamilyToArial();
    scalarBarActor->SetMaximumNumberOfColors(lutNum);
    scalarBarActor->SetNumberOfLabels(lutNum + 1);

    renderer->AddActor(actor);
    renderer->AddActor(scalarBarActor);
    renderer->GradientBackgroundOn();
    renderer->SetBackground(0.1, 0.2, 0.4);
    renderer->SetBackground2(0.2, 0.4, 0.8);

    cout << "[DEBUG] 渲染完成" << endl;
    return "ok";
}

string OnModelVTKLoadWithDeformationold(string path, string path1,
                                     double deformationScale,
                                     vtkRenderer *renderer, postManage *work) {

        cout << "[DEBUG] OnModelVTKLoadWithDeformation 开始，形变缩放: "
             << deformationScale << endl;

        vtkSmartPointer<vtkSTLReader> stlReader =
            vtkSmartPointer<vtkSTLReader>::New();
        vtkSmartPointer<vtkXMLStructuredGridReader> readers =
            vtkSmartPointer<vtkXMLStructuredGridReader>::New();
        vtkSmartPointer<vtkPolyData> targetData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkPointData> PointData =
            vtkSmartPointer<vtkPointData>::New();
        vtkSmartPointer<vtkStructuredGrid> structuredGrid =
            vtkSmartPointer<vtkStructuredGrid>::New();
        vtkSmartPointer<vtkPoints> pointsValue =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkDoubleArray> xData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> yData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> zData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> MagnitudeData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
            vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
        vtkSmartPointer<vtkResampleWithDataSet> resampler =
            vtkSmartPointer<vtkResampleWithDataSet>::New();
        vtkSmartPointer<vtkColorTransferFunction> colorMap =
            vtkSmartPointer<vtkColorTransferFunction>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

        // 新增：形变相关组件
        vtkSmartPointer<vtkWarpVector> warpVector =
            vtkSmartPointer<vtkWarpVector>::New();

        double range[2] = {0, 0};
        renderer->RemoveAllViewProps();

        // 使用缓存的STL数据
        if (work->cachedStlData != nullptr &&
            work->cachedStlData->GetNumberOfPoints() > 0) {
                cout << "[DEBUG] 使用缓存的STL数据" << endl;
                cout << "[DEBUG] 缓存点数: "
                     << work->cachedStlData->GetNumberOfPoints() << endl;
                cout << "[DEBUG] 缓存单元数: "
                     << work->cachedStlData->GetNumberOfCells() << endl;
                // 使用DeepCopy避免引用问题
                targetData->DeepCopy(work->cachedStlData);
        } else {
                // 如果缓存不可用，回退到原来的加载方式
                string pathSTL =
                    path + "/data/locomotive_bogieassembly_sldasm2.stl";
                cout << "[DEBUG] 缓存不可用，从文件加载STL: " << pathSTL
                     << endl;

                // 检查文件是否存在
                ifstream file(pathSTL.c_str());
                if (!file.good()) {
                        cout << "[ERROR] STL文件不存在: " << pathSTL << endl;
                        return "Error: STL文件不存在";
                }
                file.close();

                stlReader->SetFileName(pathSTL.c_str());
                stlReader->Update();
                targetData = stlReader->GetOutput();

                if (!targetData || targetData->GetNumberOfPoints() == 0) {
                        cout << "[ERROR] STL文件加载失败或为空" << endl;
                        return "Error: STL文件加载失败";
                }

                cout << "[DEBUG] STL加载成功, 点数: "
                     << targetData->GetNumberOfPoints() << endl;
        }

        // 加载VTS文件
        string pathVts = path1;
        cout << "[DEBUG] 加载VTS: " << pathVts << endl;
        readers->SetFileName(pathVts.c_str());
        readers->Update();
        structuredGrid = readers->GetOutput();
        PointData = structuredGrid->GetPointData();

        vtkDataArray *dataArray = PointData->GetArray(0);
        if (!dataArray) {
                cout << "[ERROR] 无法获取数据数组" << endl;
                return "Error: 无法获取数据数组";
        }

        std::string arrayName = dataArray->GetName();
        vtkIdType numValues = dataArray->GetNumberOfTuples();
        vtkIdType numTypes = dataArray->GetNumberOfComponents();

        cout << "[DEBUG] 数据数组: " << arrayName << ", 点数: " << numValues
             << ", 分量: " << numTypes << endl;

        // 设置数据数组
        xData->SetName("DeformationX");
        xData->SetNumberOfComponents(1);
        yData->SetName("DeformationY");
        yData->SetNumberOfComponents(1);
        zData->SetName("DeformationZ");
        zData->SetNumberOfComponents(1);
        MagnitudeData->SetName("DeformationMagnitude");
        MagnitudeData->SetNumberOfComponents(1);

        // 创建形变向量数组
        vtkNew<vtkDoubleArray> warpData;
        warpData->SetName("Deformation");
        warpData->SetNumberOfComponents(3);
        warpData->SetNumberOfTuples(numValues);

        vector<double> values;
        values.reserve(numValues);

        // 处理数据并应用缩放
        for (vtkIdType i = 0; i < numValues; i++) {
                double value_x = dataArray->GetComponent(i, 0);
                double value_y = dataArray->GetComponent(i, 1);
                double value_z = dataArray->GetComponent(i, 2);

                // 计算原始幅度
                double magnitude = std::sqrt(
                    value_x * value_x + value_y * value_y + value_z * value_z);

                // 存储分量数据
                xData->InsertTuple1(i, value_x);
                yData->InsertTuple1(i, value_y);
                zData->InsertTuple1(i, value_z);
                MagnitudeData->InsertTuple1(i, magnitude);

                // 应用缩放因子到形变向量
                double scaledWarp[] = {value_x * deformationScale,
                                       value_y * deformationScale,
                                       value_z * deformationScale};
                warpData->SetTuple(i, scaledWarp);

                values.emplace_back(magnitude);
        }

        // 排序获取合理的颜色范围
        std::sort(values.begin(), values.end());

        // 设置数据到结构化网格
        PointData->SetScalars(MagnitudeData);
        PointData->SetVectors(warpData); // 关键：设置形变向量

        // 计算颜色范围
        MagnitudeData->GetRange(range);
        cout << "[DEBUG] 原始范围: " << range[0] << " - " << range[1] << endl;

        // 使用5%和95%分位数作为颜色范围
        range[0] = values[values.size() / 100 * 5];
        range[1] = values[values.size() - values.size() / 100 * 5];
        cout << "[DEBUG] 调整后范围: " << range[0] << " - " << range[1] << endl;

        // 重采样到STL网格
        resampler->SetInputData(targetData);
        resampler->SetSourceData(structuredGrid);
        resampler->Update();

        // 应用形变（关键步骤）
        warpVector->SetInputConnection(resampler->GetOutputPort());
        warpVector->SetInputArrayToProcess(
            0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Deformation");
        warpVector->SetScaleFactor(1.0); // 缩放已经在数据中应用了
        warpVector->Update();

        cout << "[DEBUG] 应用形变，缩放因子: " << deformationScale << endl;

        // 设置颜色映射（彩虹色谱）
        colorMap->AddRGBPoint(range[0], 0.0, 0.0, 1.0); // 蓝色
        colorMap->AddRGBPoint(range[0] + 0.25 * (range[1] - range[0]), 0.0, 1.0,
                              1.0); // 青色
        colorMap->AddRGBPoint(range[0] + 0.5 * (range[1] - range[0]), 0.0, 1.0,
                              0.0); // 绿色
        colorMap->AddRGBPoint(range[0] + 0.75 * (range[1] - range[0]), 1.0, 1.0,
                              0.0);                     // 黄色
        colorMap->AddRGBPoint(range[1], 1.0, 0.0, 0.0); // 红色
        colorMap->SetRange(range[0], range[1]);

        // 配置mapper（连接到形变后的数据）
        mapper->SetInputConnection(warpVector->GetOutputPort());
        mapper->ScalarVisibilityOn();
        mapper->SetLookupTable(colorMap);
        mapper->SetScalarModeToUsePointData();
        mapper->SetColorModeToMapScalars();
        mapper->SetScalarRange(range[0], range[1]);
        mapper->Update();

        // 创建标量条
        vtkSmartPointer<vtkScalarBarActor> scalarBarActor =
            vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBarActor->SetLookupTable(colorMap);
        scalarBarActor->SetTitle("Deformation");
        scalarBarActor->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);
        scalarBarActor->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);
        scalarBarActor->SetLabelFormat("%.2e");
        scalarBarActor->SetNumberOfLabels(6);
        scalarBarActor->SetMaximumWidthInPixels(80);
        scalarBarActor->SetVerticalTitleSeparation(10);
        scalarBarActor->GetTitleTextProperty()->SetShadow(false);
        scalarBarActor->GetTitleTextProperty()->SetFontFamilyToArial();
        scalarBarActor->GetLabelTextProperty()->SetShadow(false);
        scalarBarActor->GetLabelTextProperty()->SetFontFamilyToArial();

        // 设置actor和渲染
        actor->SetMapper(mapper);
        renderer->AddActor(actor);
        renderer->AddActor(scalarBarActor);

        // 设置背景
        renderer->GradientBackgroundOn();
        renderer->SetBackground(0.1, 0.2, 0.4);
        renderer->SetBackground2(0.2, 0.4, 0.8);

        cout << "[DEBUG] 渲染完成，形变缩放: " << deformationScale << endl;
        return "ok";
}

// 模态的渲染
string OnModelVTKLoad(string pathSTL, string pathVts, double size,
                      vtkRenderer *renderer, postManage *work) {

        vtkSmartPointer<vtkSTLReader> stlReader =
            vtkSmartPointer<vtkSTLReader>::New();
        vtkSmartPointer<vtkXMLStructuredGridReader> readers =
            vtkSmartPointer<vtkXMLStructuredGridReader>::New();
        vtkSmartPointer<vtkPolyData> targetData =
            vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkPointData> PointData =
            vtkSmartPointer<vtkPointData>::New();
        vtkSmartPointer<vtkStructuredGrid> structuredGrid =
            vtkSmartPointer<vtkStructuredGrid>::New();
        vtkSmartPointer<vtkPoints> pointsValue =
            vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkDoubleArray> xData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> yData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> zData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDoubleArray> MagnitudeData =
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
            vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
        vtkSmartPointer<vtkResampleWithDataSet> resampler =
            vtkSmartPointer<vtkResampleWithDataSet>::New();
        vtkSmartPointer<vtkColorTransferFunction> colorMap =
            vtkSmartPointer<vtkColorTransferFunction>::New();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        double range[2] = {0, 0};
        renderer->RemoveAllViewProps();

        // 使用缓存的STL数据
        if (work->cachedStlData != nullptr &&
            work->cachedStlData->GetNumberOfPoints() > 0) {
                cout << "[DEBUG] 使用缓存的STL数据" << endl;
                cout << "[DEBUG] 缓存点数: "
                     << work->cachedStlData->GetNumberOfPoints() << endl;
                cout << "[DEBUG] 缓存单元数: "
                     << work->cachedStlData->GetNumberOfCells() << endl;
                // 使用DeepCopy避免引用问题
                targetData->DeepCopy(work->cachedStlData);
        } else {
                // 如果缓存不可用，回退到原来的加载方式
                cout << "[DEBUG] 缓存不可用，从文件加载STL: " << pathSTL
                     << endl;

                // 检查文件是否存在
                ifstream file(pathSTL.c_str());
                if (!file.good()) {
                        cout << "[ERROR] 回退STL文件不存在: " << pathSTL
                             << endl;
                        return "Error: STL文件不存在";
                }
                file.close();

                stlReader->SetFileName(pathSTL.c_str());
                stlReader->Update();
                targetData = stlReader->GetOutput();

                if (!targetData || targetData->GetNumberOfPoints() == 0) {
                        cout << "[ERROR] STL文件加载失败" << endl;
                        return "Error: STL文件加载失败";
                }
        }
        readers->SetFileName(pathVts.c_str());
        readers->Update();
        structuredGrid = readers->GetOutput();
        PointData = structuredGrid->GetPointData();
        structuredGrid->GetNumberOfCells();
        // vtkTypeFloat64Array* dataArray =
        // vtkTypeFloat64Array::SafeDownCast(PointData->GetArray(0));
        vtkDataArray *dataArray = PointData->GetArray(0);
        std::string arrayName = dataArray->GetName();
        vtkIdType numValues = dataArray->GetNumberOfTuples();
        vtkIdType numTypes = dataArray->GetNumberOfComponents();

        xData->SetName("DeformationX");
        xData->SetNumberOfComponents(1);
        yData->SetName("DeformationY");
        yData->SetNumberOfComponents(1);
        zData->SetName("DeformationZ");
        zData->SetNumberOfComponents(1);
        MagnitudeData->SetName("Deformation");
        MagnitudeData->SetNumberOfComponents(1);
        vtkNew<vtkDoubleArray> warpData;
        warpData->SetNumberOfComponents(3);
        vector<double> values;
        values.reserve(1000);
        // vtkTypeFloat64Array* dataArray =
        // vtkTypeFloat64Array::SafeDownCast(PointData->GetArray(0));
        for (vtkIdType i = 0; i < numValues; i++) {
                double value_x = dataArray->GetComponent(i, 0);
                double value_y = dataArray->GetComponent(i, 1);
                double value_z = dataArray->GetComponent(i, 2);
                double magnitude = std::sqrt(
                    value_x * value_x + value_y * value_y + value_z * value_z);
                xData->InsertTuple1(i, dataArray->GetComponent(i, 0));
                yData->InsertTuple1(i, dataArray->GetComponent(i, 1));
                zData->InsertTuple1(i, dataArray->GetComponent(i, 2));
                // magnitude = signedLog(magnitude);
                MagnitudeData->InsertTuple1(i, magnitude);
                double warp[] = {value_x, value_y, value_z};
                warpData->InsertNextTuple(warp);
                values.emplace_back(magnitude);
        }
        std::sort(values.begin(), values.end());

        cout << 55 << endl;
        // PointData->SetVectors(warpData);
        PointData->SetScalars(MagnitudeData);
        MagnitudeData->GetRange(range);
        cout << "premin:" << range[0] << " "
             << "premax:" << range[1] << endl;
        cout << "size:" << values.size() << endl;
        range[0] = values[values.size() / 100 * 5];
        range[1] = values[values.size() - values.size() / 100 * 5];
        resampler->SetInputData(targetData);
        resampler->SetSourceData(structuredGrid);
        resampler->Update();

        cout << 65 << endl;

        // range[0] = signedLog(range[0]);
        // range[1] = signedLog(range[1]);

        cout << range[0] << " " << range[1] << endl;
        colorMap->AddRGBPoint(range[0], 0.0, 0.0,
                              1.0); // 红色   //彩虹图
        colorMap->AddRGBPoint(range[0] + 0.25 * (range[1] - range[0]), 0.0, 1.0,
                              1.0); // 黄色
        colorMap->AddRGBPoint(range[0] + 0.5 * (range[1] - range[0]), 0.0, 1.0,
                              0.0); // 绿色
        colorMap->AddRGBPoint(range[0] + 0.75 * (range[1] - range[0]), 1.0, 1.0,
                              0.0); // 青色
        colorMap->AddRGBPoint(range[1], 1.0, 0.0, 0.0);

        colorMap->SetRange(range[0], range[1]);

        /*colorMap->AddRGBPoint(1.76839, 0.02, 0.3813, 0.9981);
        colorMap->AddRGBPoint(6.0363, 0.0200001, 0.424268, 0.96907);
        colorMap->AddRGBPoint(10.3042, 0.02, 0.467234, 0.940033);
        colorMap->AddRGBPoint(23.1079, 0.02, 0.5826, 0.834333);
        colorMap->AddRGBPoint(27.3758, 0.02, 0.6188, 0.796);
        colorMap->AddRGBPoint(31.6437, 0.0200001, 0.652535, 0.749802);
        colorMap->AddRGBPoint(35.9117, 0.02, 0.686267, 0.7036);
        colorMap->AddRGBPoint(40.1796, 0.02, 0.72, 0.6574);
        colorMap->AddRGBPoint(44.4475, 0.0200001, 0.757035, 0.603735);
        colorMap->AddRGBPoint(44.4475, 0.0200001, 0.757035, 0.603735);
        colorMap->AddRGBPoint(48.7154, 0.02, 0.794067, 0.550066);
        colorMap->AddRGBPoint(52.9833, 0.02, 0.8311, 0.4964);
        colorMap->AddRGBPoint(57.2512, 0.0213543, 0.864537, 0.428558);
        colorMap->AddRGBPoint(65.787, 0.0159761, 0.931048, 0.292563);
        colorMap->AddRGBPoint(70.0549, 0.274211, 0.952563, 0.153568);
        colorMap->AddRGBPoint(74.3228, 0.493355, 0.961904, 0.111195);
        colorMap->AddRGBPoint(78.5907, 0.6439, 0.9773, 0.0469);
        colorMap->AddRGBPoint(82.8586, 0.762402, 0.98467, 0.0346002);
        colorMap->AddRGBPoint(95.6624, 0.999403, 0.955036, 0.0790666);
        colorMap->AddRGBPoint(99.9303, 0.9994, 0.910666, 0.148134);
        colorMap->AddRGBPoint(104.198, 0.9994, 0.8663, 0.2172);
        colorMap->AddRGBPoint(108.466, 0.99927, 0.818036, 0.217201);
        colorMap->AddRGBPoint(112.734, 0.999133, 0.769766, 0.2172);
        colorMap->AddRGBPoint(117.002, 0.999, 0.7215, 0.2172);
        colorMap->AddRGBPoint(121.27, 0.999136, 0.673436, 0.217201);
        colorMap->AddRGBPoint(125.538, 0.999267, 0.625366, 0.2172);
        colorMap->AddRGBPoint(129.806, 0.9994, 0.5773, 0.2172);
        colorMap->AddRGBPoint(134.074, 0.999403, 0.521068, 0.217201);
        colorMap->AddRGBPoint(142.609, 0.9994, 0.4086, 0.2172);
        colorMap->AddRGBPoint(146.877, 0.99476, 0.331773, 0.211231);
        colorMap->AddRGBPoint(151.145, 0.986713, 0.259518, 0.190122);
        colorMap->AddRGBPoint(155.413, 0.991246, 0.147994, 0.210789);
        colorMap->AddRGBPoint(159.681, 0.949903, 0.116867, 0.252901);
        colorMap->AddRGBPoint(163.949, 0.9032, 0.0784329, 0.2918);
        colorMap->AddRGBPoint(168.217, 0.8565, 0.04, 0.3307);
        colorMap->AddRGBPoint(172.485, 0.798903, 0.0433335, 0.358434);
        colorMap->AddRGBPoint(176.753, 0.741299, 0.0466667, 0.386167);
        colorMap->AddRGBPoint(181.021, 0.6837, 0.05, 0.4139);

        colorMap->SetRange(1.8, 180);*/

        // mapper->SetInputData(polyss);
        mapper->SetInputConnection(resampler->GetOutputPort());
        cout << 85 << endl;
        mapper->ScalarVisibilityOn();
        mapper->SetLookupTable(colorMap);
        mapper->SetScalarModeToUsePointData();
        mapper->SetColorModeToMapScalars();
        mapper->SetScalarRange(range[0], range[1]);
        // mapper->SetColorModeToDefault();
        mapper->ScalarVisibilityOn();
        mapper->Update();

        vtkSmartPointer<vtkScalarBarActor> scalarBarActor =
            vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBarActor->SetLookupTable(colorMap);
        scalarBarActor->SetTitle("Dis");

        scalarBarActor->GetTitleTextProperty()->SetColor(1.0, 0.0, 0.0);
        scalarBarActor->GetLabelTextProperty()->SetColor(0.0, 1.0, 0.0);
        scalarBarActor->SetLabelFormat("%.2e");
        // work->scalarBarActor->SetLabelFormat("%.2f");
        scalarBarActor->SetNumberOfLabels(6);
        scalarBarActor->SetMaximumWidthInPixels(80);
        scalarBarActor->SetVerticalTitleSeparation(10);
        scalarBarActor->GetTitleTextProperty()->SetShadow(false);
        scalarBarActor->GetTitleTextProperty()->SetFontFamilyToArial();
        scalarBarActor->GetLabelTextProperty()->SetShadow(false);
        scalarBarActor->GetLabelTextProperty()->SetFontFamilyToArial();

        actor->SetMapper(mapper);
        renderer->AddActor(actor);
        renderer->AddActor(scalarBarActor);

        return "ok";
}

// （未用）模态
void MoTaiSSh(string stpPath, string order, string fileName, string mat,
              string poi, string wei, string nums, string vtsPath) {

        string shMes = "scp -r " + stpPath +
                       " qwerty@10.5.30.158:/home/qwerty/project/motai";
        system(shMes.c_str());

        string shMes2 =
            "ssh qwerty@10.5.30.158 \"cd ..;cd "
            "qwerty/project/free;/usr/local/openmpi/bin/mpirun -n " +
            order + " ./demo /home/qwerty/project/motai/" + fileName + " " +
            mat + " " + poi + " " + wei + " " + nums + "\"";
        system(shMes2.c_str());

        string shMes3 = "scp -r "
                        "qwerty@10.5.30.158:/home/qwerty/project/free/"
                        "unitfcm5-0-1e-10-451 " +
                        vtsPath;
        system(shMes3.c_str());
        cout << 66 << endl;
}
// （未用）模态
void GuDingSSh(string stpPath, string order, string fileName, string mat,
               string poi, string wei, string nums, string vtsPath) {

        string shMes = "scp -r " + stpPath +
                       " qwerty@10.5.30.158:/home/qwerty/project/motai";
        system(shMes.c_str());

        string shMes2 = "ssh qwerty@10.5.30.158 \"cd ..;cd "
                        "qwerty/project/ys;/usr/local/openmpi/bin/mpirun -n " +
                        order + " ./demo /home/qwerty/project/motai/" +
                        fileName + " " + mat + " " + poi + " " + wei + " " +
                        nums + "\"";
        system(shMes2.c_str());

        string shMes3 =
            "scp -r "
            "qwerty@10.5.30.158:/home/qwerty/project/ys/unitfcm5-0-1e-10-451 " +
            vtsPath;
        system(shMes3.c_str());
}



void OnMinMax(double x, double y, double z, double x1, double y1, double z1,
    double minValue, double maxValue, double length,
    vtkRenderer* renderer, postManage* work)
{
    if (!renderer || !work) return;

    // ========= 防止重复添加 =========
    renderer->RemoveActor(work->minActorPoint);
    renderer->RemoveActor(work->minActorLine);
    if (work->minFollower) renderer->RemoveActor(work->minFollower);

    renderer->RemoveActor(work->maxActorPoint);
    renderer->RemoveActor(work->maxActorLine);
    if (work->maxFollower) renderer->RemoveActor(work->maxFollower);

    // ========= 科学计数法转换 =========
    std::ostringstream ossMin, ossMax;
    ossMin << std::scientific << std::setprecision(4) << minValue;
    ossMax << std::scientific << std::setprecision(4) << maxValue;

    // ======================================
    //               最小值部分（3D）
    // ======================================
    work->minPoint->SetCenter(x, y, z);
    work->minPoint->SetRadius(10);

    work->minline->SetPoint1(x, y, z);
    work->minline->SetPoint2(x + length, y, z);
    work->minline->Update();

    work->minMapperLine->SetInputConnection(work->minline->GetOutputPort());
    work->minMapperPoint->SetInputConnection(work->minPoint->GetOutputPort());

    work->minActorLine->SetMapper(work->minMapperLine);
    work->minActorLine->GetProperty()->SetColor(1, 0, 0);

    work->minActorPoint->SetMapper(work->minMapperPoint);
    work->minActorPoint->GetProperty()->SetColor(1, 0, 0);

    renderer->AddActor(work->minActorLine);
    renderer->AddActor(work->minActorPoint);

    // ======================================
    //               最大值部分（3D）
    // ======================================
    work->maxPoint->SetCenter(x1, y1, z1);
    work->maxPoint->SetRadius(10);

    work->maxline->SetPoint1(x1, y1, z1);
    work->maxline->SetPoint2(x1 + length, y1, z1);
    work->maxline->Update();

    work->maxMapperLine->SetInputConnection(work->maxline->GetOutputPort());
    work->maxMapperPoint->SetInputConnection(work->maxPoint->GetOutputPort());

    work->maxActorLine->SetMapper(work->maxMapperLine);
    work->maxActorLine->GetProperty()->SetColor(1, 0, 0);

    work->maxActorPoint->SetMapper(work->maxMapperPoint);
    work->maxActorPoint->GetProperty()->SetColor(1, 0, 0);

    renderer->AddActor(work->maxActorLine);
    renderer->AddActor(work->maxActorPoint);

    // ======================================
    //           3D follower 文本标签
    // ======================================

    vtkCamera* cam = renderer->GetActiveCamera();
    if (!cam) return;
    cout<<"cam: "<<cam<<endl;

    // ---- 最小值文本 ----
    std::string strMin = "MinValue: " + ossMin.str();
    vtkSmartPointer<vtkVectorText> minTextSource = vtkSmartPointer<vtkVectorText>::New();
    minTextSource->SetText(strMin.c_str());

    vtkSmartPointer<vtkPolyDataMapper> minTextMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    minTextMapper->SetInputConnection(minTextSource->GetOutputPort());

    vtkSmartPointer<vtkFollower> minFollower = vtkSmartPointer<vtkFollower>::New();
    minFollower->SetMapper(minTextMapper);
    minFollower->SetScale(5.0, 5.0, 5.0); // 调整大小
    minFollower->SetPosition(x + length, y, z);
    minFollower->SetCamera(cam);
    minFollower->GetProperty()->SetColor(1.0, 0.0, 0.0);
    minFollower->GetProperty()->SetLighting(false);  // 不受光照影响
    minFollower->GetProperty()->SetAmbient(1.0);     // 全亮
    minFollower->GetProperty()->SetDiffuse(0.0);
    minFollower->GetProperty()->SetSpecular(0.0);
    work->minFollower = minFollower; // 保存到 work

    renderer->AddActor(minFollower);

    // ---- 最大值文本 ----
    std::string strMax = "MaxValue: " + ossMax.str();
    vtkSmartPointer<vtkVectorText> maxTextSource = vtkSmartPointer<vtkVectorText>::New();
    maxTextSource->SetText(strMax.c_str());

    vtkSmartPointer<vtkPolyDataMapper> maxTextMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    maxTextMapper->SetInputConnection(maxTextSource->GetOutputPort());

    vtkSmartPointer<vtkFollower> maxFollower = vtkSmartPointer<vtkFollower>::New();
    maxFollower->SetMapper(maxTextMapper);
    maxFollower->SetScale(5.0, 5.0, 5.0);
    maxFollower->SetPosition(x1 + length, y1, z1);
    maxFollower->SetCamera(cam);
    maxFollower->GetProperty()->SetColor(1.0, 0.0, 0.0);
    maxFollower->GetProperty()->SetLighting(false);  // 不受光照影响
    maxFollower->GetProperty()->SetAmbient(1.0);     // 全亮
    maxFollower->GetProperty()->SetDiffuse(0.0);
    maxFollower->GetProperty()->SetSpecular(0.0);
    work->maxFollower = maxFollower; // 保存到 work

    renderer->AddActor(maxFollower);

}


void GetScreenCoordinate(vtkRenderer* renderer, double x, double y, double z, double& screenX, double& screenY)
{
    // 1. 设置要转换的世界坐标点 (x, y, z, w=1.0)
    renderer->SetWorldPoint(x, y, z, 1.0);

    // 2. 执行转换：World -> Display
    renderer->WorldToDisplay();

    // 3. 获取转换后的显示坐标 (x, y, z)
    // 注意：这里的 z 是深度值 (0-1)，x 和 y 是像素坐标
    double* displayPoint = renderer->GetDisplayPoint();

    screenX = static_cast<int>(displayPoint[0]);
    screenY = static_cast<int>(displayPoint[1]);
}

// 后处理加载云图读取最大最小值并进行渲染
void OnMinMax1(double x, double y, double z, double x1, double y1, double z1,
    double minValue, double maxValue, double length,
    vtkRenderer* renderer, postManage* work)
{
    cout << "minValue: " << minValue << " maxValue: " << maxValue << endl;

    // ========= 防止重复添加上次的 Actor =========
    renderer->RemoveActor(work->minActorPoint);
    renderer->RemoveActor(work->minActorLine);
    renderer->RemoveActor(work->mintitle);
    renderer->RemoveActor(work->maxActorPoint);
    renderer->RemoveActor(work->maxActorLine);
    renderer->RemoveActor(work->maxtitle);

    // ========= 科学计数法 =========
    std::ostringstream ossMin, ossMax;
    ossMin << std::scientific << std::setprecision(4) << minValue;
    ossMax << std::scientific << std::setprecision(4) << maxValue;

    // ======================================
    //               最小值部分
    // ======================================

    // Sphere
    work->minPoint->SetCenter(x, y, z);
    work->minPoint->SetRadius(10);

    // Line（‼ 不要先 Update() → 会导致未初始化数据崩溃）
    work->minline->SetPoint1(x, y, z);
    work->minline->SetPoint2(x + length, y, z);   // ← 不要用 x+y+z，避免乱跑
    work->minline->Update();

    // Text
    work->mintextProp->SetFontSize(60);
    work->mintextProp->SetColor(1.0, 0.0, 0.0);
    work->mintextProp->SetFontFamilyToTimes();

    std::string strMin = "MinValue: " + ossMin.str();
    work->mintitle->SetTextProperty(work->mintextProp);
    work->mintitle->SetInput(strMin.c_str());
    work->mintitle->SetPosition(x + length, y, z);

    // Mapper
    work->minMapperLine->SetInputConnection(work->minline->GetOutputPort());
    work->minMapperPoint->SetInputConnection(work->minPoint->GetOutputPort());

    // Actor
    work->minActorLine->SetMapper(work->minMapperLine);
    work->minActorLine->GetProperty()->SetColor(1, 0, 0);

    work->minActorPoint->SetMapper(work->minMapperPoint);
    work->minActorPoint->GetProperty()->SetColor(1, 0, 0);

    // Renderer Add
    renderer->AddActor(work->minActorLine);
    renderer->AddActor(work->minActorPoint);
    renderer->AddActor(work->mintitle);


    // ======================================
    //               最大值部分
    // ======================================

    work->maxPoint->SetCenter(x1, y1, z1);
    work->maxPoint->SetRadius(10);

    work->maxline->SetPoint1(x1, y1, z1);
    work->maxline->SetPoint2(x1 + length, y1, z1);
    work->maxline->Update();

    work->maxtextProp->SetFontSize(60);
    work->maxtextProp->SetColor(1.0, 0.0, 0.0);
    work->maxtextProp->SetFontFamilyToTimes();

    std::string strMax = "MaxValue: " + ossMax.str();
    work->maxtitle->SetTextProperty(work->maxtextProp);
    work->maxtitle->SetInput(strMax.c_str());
    work->maxtitle->SetPosition(x1 + length, y1, z1);

    work->maxMapperLine->SetInputConnection(work->maxline->GetOutputPort());
    work->maxMapperPoint->SetInputConnection(work->maxPoint->GetOutputPort());

    work->maxActorLine->SetMapper(work->maxMapperLine);
    work->maxActorLine->GetProperty()->SetColor(1, 0, 0);

    work->maxActorPoint->SetMapper(work->maxMapperPoint);
    work->maxActorPoint->GetProperty()->SetColor(1, 0, 0);

    renderer->AddActor(work->maxActorLine);
    renderer->AddActor(work->maxActorPoint);
    renderer->AddActor(work->maxtitle);
}

// 结构stl渲染模式（老的未用）
void OnRenderers(string path, double rate, vtkRenderer *renderer,
                 postManage *work) {

        renderer->RemoveAllViewProps();

        string pathSTL = path + "/locomotive_bogieassembly_sldasm2.stl";
        std::cout << pathSTL << std::endl;
        work->stlReader->SetFileName(pathSTL.c_str());
        work->stlReader->Update();
        work->targetData = work->stlReader->GetOutput();
        string pathVTS = path + "/postprocess0.vts";
        std::cout << pathVTS << std::endl;
        work->readers->SetFileName(pathVTS.c_str());
        work->readers->Update();
        work->structuredGrid = work->readers->GetOutput();
        work->PointData = work->structuredGrid->GetPointData();
        vtkDataArray *dataArray = work->PointData->GetArray(0);
        std::string arrayName = dataArray->GetName();
        // cout << arrayName << endl;
        vtkIdType numValues = dataArray->GetNumberOfTuples();
        vtkIdType numTypes = dataArray->GetNumberOfComponents();
        // nodeValue.resize(numValues);
        work->pointsValue = work->structuredGrid->GetPoints();
        if (work->pointsValue) {
                for (vtkIdType i = 0;
                     i < work->pointsValue->GetNumberOfPoints(); i++) {
                        double point[3];
                        work->pointsValue->GetPoint(i, point);
                }
        }
        work->xData->SetName("DeformationX");
        work->xData->SetNumberOfComponents(1);
        work->yData->SetName("DeformationY");
        work->yData->SetNumberOfComponents(1);
        work->zData->SetName("DeformationZ");
        work->zData->SetNumberOfComponents(1);
        work->MagnitudeData->SetName("Deformation");
        work->MagnitudeData->SetNumberOfComponents(0);
        for (vtkIdType i = 0; i < numValues; i++) {
                double value_x = dataArray->GetComponent(i, 0);
                // double value_y = dataArray->GetComponent(i, 1);
                // double value_z = dataArray->GetComponent(i, 2);
                // double magnitude = std::sqrt(
                //    value_x * value_x + value_y * value_y + value_z *
                //    value_z);

                // work->xData->InsertTuple1(i,
                //                           dataArray->GetComponent(i, 0) *
                //                           rate);
                // work->yData->InsertTuple1(i,
                //                          dataArray->GetComponent(i, 1) *
                //                          rate);
                // work->zData->InsertTuple1(i,
                //                         dataArray->GetComponent(i, 2) *
                //                         rate);
                work->MagnitudeData->InsertTuple1(i, value_x * rate);
        }
        /*
        vtkDataArray *stressDataArray = work->PointData->GetArray(1);
        std::string stressArrayName = stressDataArray->GetName();
        // cout << stressDataArray->GetName() << endl;
        vtkIdType stressNumValues = stressDataArray->GetNumberOfTuples();
        vtkIdType stressNumTypes = stressDataArray->GetNumberOfComponents();
        work->stressXData->SetName("StressX");
        work->stressXData->SetNumberOfComponents(1);
        work->stressYData->SetName("StressY");
        work->stressYData->SetNumberOfComponents(1);
        work->stressZData->SetName("StressY");
        work->stressZData->SetNumberOfComponents(1);
        work->stressData->SetName("Stress");
        work->stressData->SetNumberOfComponents(0);
        for (vtkIdType i = 0; i < stressNumValues; i++) {

                double stress = stressDataArray->GetComponent(i, 0);

                work->stressData->InsertTuple1(i, stress * rate);
        }*/

        work->PointData->SetScalars(work->MagnitudeData);
        work->MagnitudeData->GetRange(work->range);

        const double eps = 1e-12;
        if (work->range[0] > 0.0) {
                // 正数，直接取对数
                work->range[0] = std::log(work->range[0]);
        } else if (work->range[0] < 0.0) {
                // 负数，先取绝对值再取对数
                double abs0 = std::abs(work->range[0]);
                work->range[0] = 0 - std::log(std::max(abs0, eps));
        } else {
                work->range[0] = 0.0;
        }

        if (work->range[1] > 0.0) {
                // 正数，直接取对数
                work->range[1] = std::log(work->range[1]);
        } else if (work->range[1] < 0.0) {
                // 负数，先取绝对值再取对数
                double abs0 = std::abs(work->range[1]);
                work->range[1] = 0 - std::log(std::max(abs0, eps));
        } else {
                work->range[1] = 0.0;
        }

        work->resampler->SetInputData(work->targetData);
        work->resampler->SetSourceData(work->structuredGrid);
        work->resampler->Update();

        work->surfaceFilter->SetInputConnection(
            work->resampler->GetOutputPort());
        work->surfaceFilter->Update();
        work->polys->DeepCopy(work->surfaceFilter->GetOutput());

        work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                    1.0); // 红色   //彩虹图
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.25 * (work->range[1] - work->range[0]), 0.0, 1.0,
            1.0); // 黄色
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.5 * (work->range[1] - work->range[0]), 0.0, 1.0,
            0.0); // 绿色
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.75 * (work->range[1] - work->range[0]), 1.0, 1.0,
            0.0); // 青色
        work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);

        work->colorMap->SetRange(work->range[0], work->range[1]);

        work->mapper->SetInputConnection(work->resampler->GetOutputPort());
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetLookupTable(work->colorMap);

        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0], work->range[1]);
        work->mapper->SetColorModeToDefault();
        work->mapper->Update();

        work->actor->SetMapper(work->mapper);
        renderer->GradientBackgroundOn();
        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->AddActor(work->actor);
}
// （未用）

void OnRenderers1(string path, string path1, double rate, vtkRenderer *renderer,
                  postManage *work) {

        renderer->RemoveAllViewProps();

        string pathSTL = path + "/data/locomotive_bogieassembly_sldasm2.stl";
        work->stlReader->SetFileName(pathSTL.c_str());
        work->stlReader->Update();
        work->targetData = work->stlReader->GetOutput();
        string pathVTS = path1;
        work->readers->SetFileName(pathVTS.c_str());
        work->readers->Update();
        work->structuredGrid = work->readers->GetOutput();
        work->PointData = work->structuredGrid->GetPointData();

        vtkDataArray *dataArray = work->PointData->GetArray(0);
        if (!dataArray) {
                std::cerr << "Error: data array[0] is null\n";
                return;
        }

        vtkIdType numValues = dataArray->GetNumberOfTuples();

        work->pointsValue = work->structuredGrid->GetPoints();
        if (work->pointsValue) {
                for (vtkIdType i = 0;
                     i < work->pointsValue->GetNumberOfPoints(); i++) {
                        double point[3];
                        work->pointsValue->GetPoint(i, point);
                }
        }
        work->xData->SetName("DeformationX");
        work->xData->SetNumberOfComponents(1);
        work->yData->SetName("DeformationY");
        work->yData->SetNumberOfComponents(1);
        work->zData->SetName("DeformationZ");
        work->zData->SetNumberOfComponents(1);
        work->MagnitudeData->SetName("Deformation");
        work->MagnitudeData->SetNumberOfComponents(1);

        for (vtkIdType i = 0; i < numValues; i++) {
                double value_x = dataArray->GetComponent(i, 0);
                double value_y = dataArray->GetComponent(i, 1);
                double value_z = dataArray->GetComponent(i, 2);
                double magnitude = std::sqrt(
                    value_x * value_x + value_y * value_y + value_z * value_z);
                // cout << magnitude << endl;
                //  work->xData->InsertTuple1(i,
                //                            dataArray->GetComponent(i, 0) *
                //                            rate);
                //  work->yData->InsertTuple1(i,
                //                           dataArray->GetComponent(i, 1) *
                //                           rate);
                //  work->zData->InsertTuple1(i,
                //                          dataArray->GetComponent(i, 2) *
                //                          rate);
                work->MagnitudeData->InsertTuple1(i, magnitude);

                // cout << value_x << endl;
        }

        /*
        vtkDataArray *stressDataArray = work->PointData->GetArray(1);
        std::string stressArrayName = stressDataArray->GetName();
        // cout << stressDataArray->GetName() << endl;
        vtkIdType stressNumValues = stressDataArray->GetNumberOfTuples();
        vtkIdType stressNumTypes = stressDataArray->GetNumberOfComponents();
        work->stressXData->SetName("StressX");
        work->stressXData->SetNumberOfComponents(1);
        work->stressYData->SetName("StressY");
        work->stressYData->SetNumberOfComponents(1);
        work->stressZData->SetName("StressY");
        work->stressZData->SetNumberOfComponents(1);
        work->stressData->SetName("Stress");
        work->stressData->SetNumberOfComponents(0);
        for (vtkIdType i = 0; i < stressNumValues; i++) {

                double stress = stressDataArray->GetComponent(i, 0);

                work->stressData->InsertTuple1(i, stress * rate);
        }*/

        work->PointData->SetScalars(work->MagnitudeData);
        work->MagnitudeData->GetRange(work->range);

        work->range[0] = signedLog(work->range[0]);
        work->range[1] = signedLog(work->range[1]);
        // cout << work->range[0] << " " << work->range[1] << endl;

        /* for (auto& elem : data)
         {
             cout << elem << endl;
         }*/
        work->resampler->SetInputData(work->targetData);
        work->resampler->SetSourceData(work->structuredGrid);
        work->resampler->Update();

        work->surfaceFilter->SetInputConnection(
            work->resampler->GetOutputPort());
        work->surfaceFilter->Update();
        work->polys->DeepCopy(work->surfaceFilter->GetOutput());
        cout << work->range[0] << " " << work->range[1] << endl;
        work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                    1.0); // 红色   //彩虹图
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.25 * (work->range[1] - work->range[0]), 0.0, 1.0,
            1.0); // 黄色
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.5 * (work->range[1] - work->range[0]), 0.0, 1.0,
            0.0); // 绿色
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.75 * (work->range[1] - work->range[0]), 1.0, 1.0,
            0.0); // 青色
        work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);

        work->colorMap->SetRange(work->range[0], work->range[1]);

        work->mapper->SetInputConnection(work->resampler->GetOutputPort());
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetLookupTable(work->colorMap);

        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0], work->range[1]);
        work->mapper->SetColorModeToDefault();
        work->mapper->Update();

        work->actor->SetMapper(work->mapper);
        renderer->GradientBackgroundOn();
        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->AddActor(work->actor);
}

void OnRenderers_test(string path, string path1, double rate,
                      vtkRenderer *renderer, postManage *work, double max,
                      double min) {

        renderer->RemoveAllViewProps();

        string pathSTL = path + "/data/locomotive_bogieassembly_sldasm2.stl";
        work->stlReader->SetFileName(pathSTL.c_str());
        work->stlReader->Update();
        work->targetData = work->stlReader->GetOutput();
        string pathVTS = path1;
        work->readers->SetFileName(pathVTS.c_str());
        work->readers->Update();
        work->structuredGrid = work->readers->GetOutput();
        work->PointData = work->structuredGrid->GetPointData();

        vtkDataArray *dataArray = work->PointData->GetArray(0);
        std::cout << "filepathxxxxxxxxxxxxxxxxxx" << pathVTS.c_str()
                  << std::endl;
        cout << "filepathyyyyyyyyyyyyyyyyyyy" << pathVTS << endl;
        std::string arrayName = dataArray->GetName();
        // cout << arrayName << endl;
        vtkIdType numValues = dataArray->GetNumberOfTuples();
        vtkIdType numTypes = dataArray->GetNumberOfComponents();
        cout << numTypes << endl;
        cout << numValues << endl;
        // nodeValue.resize(numValues);
        work->pointsValue = work->structuredGrid->GetPoints();
        if (work->pointsValue) {
                for (vtkIdType i = 0;
                     i < work->pointsValue->GetNumberOfPoints(); i++) {
                        double point[3];
                        work->pointsValue->GetPoint(i, point);
                }
        }
        work->xData->SetName("DeformationX");
        work->xData->SetNumberOfComponents(1);
        work->yData->SetName("DeformationY");
        work->yData->SetNumberOfComponents(1);
        work->zData->SetName("DeformationZ");
        work->zData->SetNumberOfComponents(1);
        work->MagnitudeData->SetName("Deformation");
        work->MagnitudeData->SetNumberOfComponents(0);
        // work->MagnitudeData->SetNumberOfComponents(1);
        vector<double> data;
        for (vtkIdType i = 0; i < numValues; i++) {
                double value_x = dataArray->GetComponent(i, 0);
                double value_y = dataArray->GetComponent(i, 1);
                double value_z = dataArray->GetComponent(i, 2);
                double magnitude = std::sqrt(
                    value_x * value_x + value_y * value_y + value_z * value_z);
                // cout << magnitude << endl;
                //  work->xData->InsertTuple1(i,
                //                            dataArray->GetComponent(i, 0) *
                //                            rate);
                //  work->yData->InsertTuple1(i,
                //                           dataArray->GetComponent(i, 1) *
                //                           rate);
                //  work->zData->InsertTuple1(i,
                //                          dataArray->GetComponent(i, 2) *
                //                          rate);
                work->MagnitudeData->InsertTuple1(i, magnitude);
                data.push_back(magnitude);
                // cout << value_x << endl;
        }

        /*
        vtkDataArray *stressDataArray = work->PointData->GetArray(1);
        std::string stressArrayName = stressDataArray->GetName();
        // cout << stressDataArray->GetName() << endl;
        vtkIdType stressNumValues = stressDataArray->GetNumberOfTuples();
        vtkIdType stressNumTypes = stressDataArray->GetNumberOfComponents();
        work->stressXData->SetName("StressX");
        work->stressXData->SetNumberOfComponents(1);
        work->stressYData->SetName("StressY");
        work->stressYData->SetNumberOfComponents(1);
        work->stressZData->SetName("StressY");
        work->stressZData->SetNumberOfComponents(1);
        work->stressData->SetName("Stress");
        work->stressData->SetNumberOfComponents(0);
        for (vtkIdType i = 0; i < stressNumValues; i++) {

                double stress = stressDataArray->GetComponent(i, 0);

                work->stressData->InsertTuple1(i, stress * rate);
        }*/

        work->PointData->SetScalars(work->MagnitudeData);
        // work->MagnitudeData->GetRange(work->range);
        auto max_it = std::max_element(data.begin(), data.end());
        auto min_it = std::min_element(data.begin(), data.end());
        cout << *max_it << " " << *min_it << endl;
        work->range[0] = max;
        work->range[1] = min;
        cout << "data数组长度:" << data.size() << endl;
        /* for (auto& elem : data)
         {
             cout << elem << endl;
         }*/
        work->resampler->SetInputData(work->targetData);
        work->resampler->SetSourceData(work->structuredGrid);
        work->resampler->Update();

        work->surfaceFilter->SetInputConnection(
            work->resampler->GetOutputPort());
        work->surfaceFilter->Update();
        work->polys->DeepCopy(work->surfaceFilter->GetOutput());
        cout << work->range[0] << " " << work->range[1] << endl;
        work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                    1.0); // 红色   //彩虹图
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.25 * (work->range[1] - work->range[0]), 0.0, 1.0,
            1.0); // 黄色
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.5 * (work->range[1] - work->range[0]), 0.0, 1.0,
            0.0); // 绿色
        work->colorMap->AddRGBPoint(
            work->range[0] + 0.75 * (work->range[1] - work->range[0]), 1.0, 1.0,
            0.0); // 青色
        work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);

        work->colorMap->SetRange(work->range[0], work->range[1]);

        work->mapper->SetInputConnection(work->resampler->GetOutputPort());
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetLookupTable(work->colorMap);

        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0], work->range[1]);
        work->mapper->SetColorModeToDefault();
        work->mapper->Update();

        work->actor->SetMapper(work->mapper);
        renderer->GradientBackgroundOn();
        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->AddActor(work->actor);
}

void setPostBar(int attribute, vtkRenderer *renderer, postManage *work) {
        work->scalarBarActor->SetLookupTable(work->colorMap);
        switch (attribute) {
        case 1:
                work->scalarBarActor->SetTitle("DistanceX()");
                break;
        case 2:
                work->scalarBarActor->SetTitle("Dis X");
                break;
        case 3:
                work->scalarBarActor->SetTitle("Dis Y");
                break;
        case 4:
                work->scalarBarActor->SetTitle("Dis Z");
                break;
        case 5:
                work->scalarBarActor->SetTitle("DisTotal");
                break;
        case 6:
                work->scalarBarActor->SetTitle("Stress");
                break;
        case 7:
                work->scalarBarActor->SetTitle("Strain X");
                break;
        case 8:
                work->scalarBarActor->SetTitle("Strain Y");
                break;
        case 9:
                work->scalarBarActor->SetTitle("Strain Z");
                break;
        case 10:
                work->scalarBarActor->SetTitle("Stress");
                break;
        case 11:
                work->scalarBarActor->SetTitle("Stress X");
                break;
        case 12:
                work->scalarBarActor->SetTitle("Stress Y");
                break;
        case 13:
                work->scalarBarActor->SetTitle("Stress Z");
                break;

        default:
                break;
        }
        work->scalarBarActor->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);
        work->scalarBarActor->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);
        work->scalarBarActor->SetLabelFormat("%.3e");
        work->scalarBarActor->GetTitleTextProperty()->SetFontSize(
            80); // 2025-2-7
        work->scalarBarActor->GetLabelTextProperty()->SetFontSize(40);
        // work->scalarBarActor->SetLabelFormat("%.2f");
        work->scalarBarActor->SetNumberOfLabels(10);
        work->scalarBarActor->SetMaximumWidthInPixels(80);
        work->scalarBarActor->SetVerticalTitleSeparation(10);
        work->scalarBarActor->GetTitleTextProperty()->SetShadow(false);
        work->scalarBarActor->GetTitleTextProperty()->SetFontFamilyToArial();
        work->scalarBarActor->GetLabelTextProperty()->SetShadow(false);
        work->scalarBarActor->GetLabelTextProperty()->SetFontFamilyToArial();
        /*
        work->scalarBarWidget->SetInteractor(
            renderer->GetRenderWindow()->GetInteractor());
        work->scalarBarWidget->SetScalarBarActor(work->scalarBarActor);

        work->scalarBarWidget->GetScalarBarRepresentation()->SetPosition(0.9,
                                                                         0.6);
        work->scalarBarWidget->GetScalarBarRepresentation()->SetPosition2(0.1,
                                                                          0.3);*/

        // work->scalarBarWidget->On();
        renderer->AddActor(work->scalarBarActor);

        // renderer->Render();
}
// 关闭云图颜色条
void closePostBar(vtkRenderer *renderer, postManage *work) {
        renderer->RemoveActor(work->scalarBarActor);

        renderer->Render();
}
// 切换云图物理量（下拉菜单处函数）-未用
void setPostChange(int attribute, vtkRenderer *renderer, postManage *work) {

        if (attribute == 1) {

                renderer->RemoveAllViewProps();
                work->scalarBarActor->SetTitle("Dis");
                work->PointData->SetScalars(work->MagnitudeData);
                work->MagnitudeData->GetRange(work->range);
                work->resampler->SetInputData(work->targetData);
                work->resampler->SetSourceData(work->structuredGrid);
                work->resampler->Update();
                work->colorMap->RemoveAllPoints();

                work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0, 1.0);
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.25 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 1.0); // 黄色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.5 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 0.0); // 绿色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.75 * (work->range[1] - work->range[0]),
                    1.0, 1.0, 0.0); // 青色
                work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);
                work->colorMap->SetRange(work->range[0], work->range[1]);

                work->mapper->SetInputConnection(
                    work->resampler->GetOutputPort());
                work->mapper->ScalarVisibilityOn();
                work->mapper->SetLookupTable(work->colorMap);
                work->mapper->SetScalarModeToUsePointData();
                work->mapper->SetScalarRange(work->range[0], work->range[1]);
                work->mapper->SetColorModeToDefault();
                work->mapper->Update();
                work->actor->SetMapper(work->mapper);
                renderer->AddActor(work->actor);

        } else if (attribute == 2) {

                renderer->RemoveAllViewProps();

                work->scalarBarActor->SetTitle("Dis X");
                work->PointData->SetScalars(work->xData);
                work->xData->GetRange(work->range);
                work->resampler->SetInputData(work->targetData);
                work->resampler->SetSourceData(work->structuredGrid);
                work->resampler->Update();
                work->colorMap->RemoveAllPoints();

                work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                            1.0); // 红色 //彩虹图
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.25 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 1.0); // 黄色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.5 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 0.0); // 绿色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.75 * (work->range[1] - work->range[0]),
                    1.0, 1.0, 0.0); // 青色
                work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);
                work->colorMap->SetRange(work->range[0], work->range[1]);

                work->mapper->SetInputConnection(
                    work->resampler->GetOutputPort());
                work->mapper->ScalarVisibilityOn();
                work->mapper->SetLookupTable(work->colorMap);
                work->mapper->SetScalarModeToUsePointData();
                work->mapper->SetScalarRange(work->range[0], work->range[1]);
                work->mapper->SetColorModeToDefault();
                work->mapper->Update();
                work->actor->SetMapper(work->mapper);
                renderer->AddActor(work->actor);

        } else if (attribute == 3) {
                renderer->RemoveAllViewProps();

                work->scalarBarActor->SetTitle("Dis Y");

                work->PointData->SetScalars(work->yData);
                work->yData->GetRange(work->range);
                work->resampler->SetInputData(work->targetData);
                work->resampler->SetSourceData(work->structuredGrid);
                work->resampler->Update();
                work->colorMap->RemoveAllPoints();

                work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                            1.0); // 红色 //彩虹图
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.25 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 1.0); // 黄色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.5 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 0.0); // 绿色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.75 * (work->range[1] - work->range[0]),
                    1.0, 1.0, 0.0); // 青色
                work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);
                work->colorMap->SetRange(work->range[0], work->range[1]);

                work->mapper->SetInputConnection(
                    work->resampler->GetOutputPort());
                work->mapper->ScalarVisibilityOn();
                work->mapper->SetLookupTable(work->colorMap);
                work->mapper->SetScalarModeToUsePointData();
                work->mapper->SetScalarRange(work->range[0], work->range[1]);
                work->mapper->SetColorModeToDefault();
                work->mapper->Update();
                work->actor->SetMapper(work->mapper);
                renderer->AddActor(work->actor);

        } else if (attribute == 4) {

                renderer->RemoveAllViewProps();

                work->scalarBarActor->SetTitle("Dis Z");
                work->PointData->SetScalars(work->zData);
                work->zData->GetRange(work->range);
                work->resampler->SetInputData(work->targetData);
                work->resampler->SetSourceData(work->structuredGrid);
                work->resampler->Update();
                work->colorMap->RemoveAllPoints();

                work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                            1.0); // 红色 //彩虹图
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.25 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 1.0); // 黄色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.5 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 0.0); // 绿色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.75 * (work->range[1] - work->range[0]),
                    1.0, 1.0, 0.0); // 青色
                work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);
                work->colorMap->SetRange(work->range[0], work->range[1]);

                work->mapper->SetInputConnection(
                    work->resampler->GetOutputPort());
                work->mapper->ScalarVisibilityOn();
                work->mapper->SetLookupTable(work->colorMap);
                work->mapper->SetScalarModeToUsePointData();
                work->mapper->SetScalarRange(work->range[0], work->range[1]);
                work->mapper->SetColorModeToDefault();
                work->mapper->Update();
                work->actor->SetMapper(work->mapper);
                renderer->AddActor(work->actor);

        } else if (attribute == 5) {

                renderer->RemoveAllViewProps();

                work->scalarBarActor->SetTitle("Stress");
                work->PointData->SetScalars(work->stressData);
                work->stressData->GetRange(work->range);

                work->resampler->SetInputData(work->targetData);
                work->resampler->SetSourceData(work->structuredGrid);
                work->resampler->Update();
                work->colorMap->RemoveAllPoints();

                work->colorMap->AddRGBPoint(work->range[0], 0.0, 0.0,
                                            1.0); // 红色 //彩虹图
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.25 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 1.0); // 黄色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.5 * (work->range[1] - work->range[0]),
                    0.0, 1.0, 0.0); // 绿色
                work->colorMap->AddRGBPoint(
                    work->range[0] + 0.75 * (work->range[1] - work->range[0]),
                    1.0, 1.0, 0.0); // 青色
                work->colorMap->AddRGBPoint(work->range[1], 1.0, 0.0, 0.0);
                work->colorMap->SetRange(work->range[0], work->range[1]);

                work->mapper->SetInputConnection(
                    work->resampler->GetOutputPort());
                work->mapper->ScalarVisibilityOn();
                work->mapper->SetLookupTable(work->colorMap);
                work->mapper->SetScalarModeToUsePointData();
                work->mapper->SetScalarRange(work->range[0], work->range[1]);
                work->mapper->SetColorModeToDefault();
                work->mapper->Update();
                work->actor->SetMapper(work->mapper);
                renderer->AddActor(work->actor);
        }
}
// 高斯点，放到pts容器中
void setPts(string path, string id, workData *work) {
        ifstream fs;
        guass p;
        fs.open(path);
        work->pts.clear();
        double x, y, z;
        vector<guass> temppts;
        while (fs >> x >> y >> z) {
                p.id = stoi(id);
                p.pts.x = x;
                p.pts.y = y;
                p.pts.z = z;
                work->pts.push_back(p);
                temppts.push_back(p);
        }
        cout << "guass num: " << temppts.size() << endl;
        work->newPts[id] = temppts;

        fs.close();
}
// 未用
void printfPts(workData *work) {
        for (int i = 0; i < work->pts.size(); i++) {
                // cout << work->pts[i].id << " " << work->pts[i].pts.x << " "
                //    << work->pts[i].pts.y << " " << work->pts[i].pts.z <<
                //    endl;
        }
}
void ensure_directory(const fs::path &dir) {
        std::error_code ec;
        // 如果目标目录不存在，就递归创建所有父目录
        if (!fs::exists(dir, ec)) {
                if (!fs::create_directories(dir, ec) || ec) {
                        throw std::runtime_error(
                            "Failed to create directory '" + dir.string() +
                            "': " + ec.message());
                }
        }
}
// 生成brep文件（前处理）
string generateBrep(string id, string path, workData *work, string path2) {

        string name = path2 + "face" + id + ".brep";
        string guassExePath = path + "src/guass/demo1.exe ";
        string guassTxtPath = path2 + "guass/guass.txt ";
        fs::path dir = fs::path(path2 + "guass");
        ensure_directory(dir);
        std::replace(guassExePath.begin(), guassExePath.end(), '\\', '/');
        string guass = path + "src/guass/demo1.exe " + name + " " + path2 +
                       "guass/guass.txt";
        cout << "namePath: " << name << endl;
        cout << "guassExePath: " << guassExePath << endl;
        cout << "guassTxtPath: " << guassTxtPath << endl;
        system(guass.c_str()); // 生成

        setPts(guassTxtPath, id, work);

        return "ok";
        // printfPts(work);
}

// 设置标记-几何-标记
void setMark(double x, double y, double z, double r, double g, double b,
             double size, string mark, string name, vtkRenderer *renderer,
             workData *work) {
        vtkSmartPointer<vtkTextProperty> polyTextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> polyTitle =
            vtkSmartPointer<vtkTextActor3D>::New();

        double centerPoints[3] = {x, y, z};
        polyTextProp->SetFontSize(size);
        polyTextProp->SetColor(r, g, b);
        polyTextProp->SetFontFamilyToTimes();
        polyTitle->SetTextProperty(polyTextProp);
        polyTitle->SetInput(mark.c_str());
        polyTitle->SetPosition(centerPoints);
        work->mark[name] = polyTitle;

        renderer->AddActor(polyTitle);
}
// 修改标记
void motifyMark(double x, double y, double z, double r, double g, double b,
                double size, string mark, string name, vtkRenderer *renderer,
                workData *work) {
        vtkSmartPointer<vtkTextProperty> polyTextProp =
            vtkSmartPointer<vtkTextProperty>::New();
        vtkSmartPointer<vtkTextActor3D> polyTitle =
            vtkSmartPointer<vtkTextActor3D>::New();

        double centerPoints[3] = {x, y, z};
        polyTextProp->SetFontSize(size);
        polyTextProp->SetColor(r, g, b);
        polyTextProp->SetFontFamilyToTimes();
        work->mark[name]->SetTextProperty(polyTextProp);
        work->mark[name]->SetInput(mark.c_str());
        work->mark[name]->SetPosition(centerPoints);

        // work->mark[name] = polyTitle;

        renderer->AddActor(work->mark[name]);
}
// 读取vtp文件
void LoadVtpPly(string name1, string name2, vtkRenderer *renderer) {

        /*
        cout << 2 << endl;
        fs::path current_path = name1;

        std::cout << current_path << endl;
        for (const auto& entry : fs::directory_iterator(current_path)) {
            if (entry.path().extension() != ".vtp") continue;
            std::cout << entry.path().string() << std::endl;

            string fileName = entry.path().string();
            // 读取第一个VTP文件
            vtkSmartPointer<vtkXMLPolyDataReader> reader =
                vtkSmartPointer<vtkXMLPolyDataReader>::New();
            reader->SetFileName(fileName.c_str());
            reader->Update();
            cout << 2 << endl;
            // 创建第一个Mapper和Actor
            vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(reader->GetOutputPort());

            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // 设置颜色为红色
            renderer->AddActor(actor);
        }
        renderer->ResetCamera();
        */

        // 读取第一个VTP文件
        vtkSmartPointer<vtkXMLPolyDataReader> reader1 =
            vtkSmartPointer<vtkXMLPolyDataReader>::New();
        reader1->SetFileName(name1.c_str());
        reader1->Update();
        cout << 2 << endl;
        // 创建第一个Mapper和Actor
        vtkSmartPointer<vtkPolyDataMapper> mapper1 =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper1->SetInputConnection(reader1->GetOutputPort());

        vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
        actor1->SetMapper(mapper1);
        actor1->GetProperty()->SetColor(1.0, 0.0, 0.0); // 设置颜色为红色

        // 读取第二个VTP文件
        vtkSmartPointer<vtkXMLPolyDataReader> reader2 =
            vtkSmartPointer<vtkXMLPolyDataReader>::New();
        reader2->SetFileName(name2.c_str());
        reader2->Update();

        // 创建第二个Mapper和Actor
        vtkSmartPointer<vtkPolyDataMapper> mapper2 =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper2->SetInputConnection(reader2->GetOutputPort());

        vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
        actor2->SetMapper(mapper2);
        actor2->GetProperty()->SetColor(0.0, 0.0, 1.0); // 设置颜色为蓝色
        renderer->AddActor(actor1);
        renderer->AddActor(actor2);
        renderer->ResetCamera();
}
// 改变云图颜色（颜色映射表）
void SetColorMap1(RGBS color1, RGBS color2, vtkRenderer *renderer,
                  postManage *work) {
        work->plyColorMap->RemoveAllPoints();

        work->plyColorMap->AddRGBPoint(work->range[0], color1.r, color1.g,
                                       color1.b);
        work->plyColorMap->AddRGBPoint(work->range[1], color2.r, color2.g,
                                       color2.b);

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->ScalarVisibilityOn();
        work->plyMapper->SetLookupTable(work->plyColorMap);
        work->plyMapper->SetScalarModeToUsePointData();
        work->plyMapper->SetScalarRange(work->range[0], work->range[1]);
        work->plyMapper->SetColorModeToDefault();
        work->plyMapper->Update();
        work->plyActor->SetMapper(work->plyMapper);
        renderer->AddActor(work->plyActor);
}
// 改变云图颜色（颜色映射表）
void SetColorMap2(RGBS color1, RGBS color2, RGBS color3, vtkRenderer *renderer,
                  postManage *work) {
        work->plyColorMap->RemoveAllPoints();

        work->plyColorMap->AddRGBPoint(work->range[0], color1.r, color1.g,
                                       color1.b);
        work->plyColorMap->AddRGBPoint((work->range[1] + work->range[0]) / 2,
                                       color2.r, color2.g, color2.b);
        work->plyColorMap->AddRGBPoint(work->range[1], color3.r, color3.g,
                                       color3.b);

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->ScalarVisibilityOn();
        work->plyMapper->SetLookupTable(work->plyColorMap);
        work->plyMapper->SetScalarModeToUsePointData();
        work->plyMapper->SetScalarRange(work->range[0], work->range[1]);
        work->plyMapper->SetColorModeToDefault();
        work->plyMapper->Update();
        work->plyActor->SetMapper(work->plyMapper);
        renderer->AddActor(work->plyActor);
}
// 改变云图颜色（颜色映射表）
void SetColorMap3(RGBS color1, RGBS color2, RGBS color3, RGBS color4,
                  vtkRenderer *renderer, postManage *work) {
        work->plyColorMap->RemoveAllPoints();

        work->plyColorMap->AddRGBPoint(work->range[0], color1.r, color1.g,
                                       color1.b);
        work->plyColorMap->AddRGBPoint((work->range[1] - work->range[0]) / 3 +
                                           work->range[0],
                                       color2.r, color2.g, color2.b);
        work->plyColorMap->AddRGBPoint(
            (work->range[1] - work->range[0]) / 3 * 2 + work->range[0],
            color3.r, color3.g, color3.b);
        work->plyColorMap->AddRGBPoint(work->range[1], color4.r, color4.g,
                                       color4.b);

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->ScalarVisibilityOn();
        work->plyMapper->SetLookupTable(work->plyColorMap);
        work->plyMapper->SetScalarModeToUsePointData();
        work->plyMapper->SetScalarRange(work->range[0], work->range[1]);
        work->plyMapper->SetColorModeToDefault();
        work->plyMapper->Update();
        work->plyActor->SetMapper(work->plyMapper);
        renderer->AddActor(work->plyActor);
}
// 改变云图颜色（颜色映射表）
void SetColorMap4(RGBS color1, RGBS color2, RGBS color3, RGBS color4,
                  RGBS color5, vtkRenderer *renderer, postManage *work) {
        work->plyColorMap->RemoveAllPoints();

        work->plyColorMap->AddRGBPoint(work->range[0], color1.r, color1.g,
                                       color1.b);
        work->plyColorMap->AddRGBPoint((work->range[1] - work->range[0]) / 4 +
                                           work->range[0],
                                       color2.r, color2.g, color2.b);
        work->plyColorMap->AddRGBPoint(
            (work->range[1] - work->range[0]) / 4 * 2 + work->range[0],
            color3.r, color3.g, color3.b);
        work->plyColorMap->AddRGBPoint(
            (work->range[1] - work->range[0]) / 4 * 3 + work->range[0],
            color4.r, color4.g, color4.b);
        work->plyColorMap->AddRGBPoint(work->range[1], color5.r, color5.g,
                                       color5.b);

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->ScalarVisibilityOn();
        work->plyMapper->SetLookupTable(work->plyColorMap);
        work->plyMapper->SetScalarModeToUsePointData();
        work->plyMapper->SetScalarRange(work->range[0], work->range[1]);
        work->plyMapper->SetColorModeToDefault();
        work->plyMapper->Update();
        work->plyActor->SetMapper(work->plyMapper);
        renderer->AddActor(work->plyActor);
}
// 云图切割
void cutter(double x, double y, double z, double xn, double yn, double zn,
            vtkRenderer *renderer, postManage *work) {
        // 创建 vtkClipPolyData 对象
        renderer->RemoveAllViewProps();
        work->vtkPlanes->RemoveAllItems();

        work->clipper->SetInputData(work->polys);

        // 创建裁剪平面等相关配置，这里简单示例，比如创建一个平面
        vtkSmartPointer<vtkPlane> clipPlane = vtkSmartPointer<vtkPlane>::New();
        clipPlane->SetOrigin(x, y, z);
        clipPlane->SetNormal(xn, yn, zn); // 设置平面的法向量等参数
        work->vtkPlanes->AddItem(clipPlane);
        work->clipper->SetClippingPlanes(work->vtkPlanes);
        work->clipper->Update();

        work->resamplers->SetInputData(work->clipper->GetOutput());
        work->resamplers->SetSourceData(work->structuredGrid);
        work->resamplers->Update();
        work->fillMapper->SetInputData(NULL);
        work->fillMapper->SetInputConnection(work->resamplers->GetOutputPort());
        work->fillMapper->ScalarVisibilityOn();
        work->fillMapper->SetLookupTable(work->colorMap);
        work->fillMapper->SetScalarModeToUsePointData();
        work->fillMapper->SetScalarRange(work->range[0], work->range[1]);
        work->fillMapper->SetColorModeToDefault();
        work->fillMapper->Update();
        work->fillActor->SetMapper(work->fillMapper);
        renderer->AddActor(work->fillActor);
}
// 未用
double polynomial(double x) {
        if (x >= 0.00 && x <= 648.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0461 * x +
                       0.0000;
        } else if (x > 648.00 && x <= 3000.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0234 * x +
                       22.2000;
        } else if (x > 3000.00 && x <= 17496.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + (-0.0098) * x +
                       22.9000;
        } else if (x > 17496.00 && x <= 30132.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0024 * x +
                       24.3000;
        } else if (x > 30132.00 && x <= 35520.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + (-0.0003) * x +
                       23.6000;
        } else if (x > 35520.00 && x <= 39780.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0002 * x +
                       24.5000;
        } else if (x > 39780.00 && x <= 47355.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0000 * x +
                       24.8000;
        } else if (x > 47355.00 && x <= 55176.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       25.3000;
        } else if (x > 55176.00 && x <= 67680.00) {
                return 0.0000 * x * x * x + 0.0000 * x * x + 0.0000 * x +
                       25.7000;
        } else if (x > 67680.00 && x <= 77400.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0001 * x +
                       26.3000;
        } else if (x > 77400.00 && x <= 95082.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0000 * x +
                       26.8000;
        } else if (x > 95082.00 && x <= 119364.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0001 * x +
                       27.1000;
        } else if (x > 119364.00 && x <= 150255.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       29.9000;
        } else if (x > 150255.00 && x <= 195408.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0000 * x +
                       31.3000;
        } else if (x > 195408.00 && x <= 266760.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       34.8000;
        } else if (x > 266760.00 && x <= 372300.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0000 * x +
                       38.0000;
        } else if (x > 372300.00 && x <= 531366.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       45.7000;
        } else if (x > 531366.00 && x <= 813600.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0001 * x +
                       57.4000;
        } else if (x > 813600.00 && x <= 1397250.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       80.3000;
        } else if (x > 1397250.00 && x <= 2666664.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0001 * x +
                       117.3000;
        } else if (x > 2666664.00 && x <= 6069168.00) {
                return 0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       206.3000;
        } else if (x > 6069168.00 && x <= 9244290.00) {
                return 0.0000 * x * x * x + 0.0000 * x * x + 0.0001 * x +
                       439.3000;
        } else if (x > 9244290.00 && x <= 15184845.00) {
                return -0.0000 * x * x * x + 0.0000 * x * x + 0.0001 * x +
                       676.3000;
        } else if (x > 15184845.00 && x <= 20072907.00) {
                return -0.0000 * x * x * x + (-0.0000) * x * x + 0.0001 * x +
                       1228.8000;
        } else {
                // If x is out of all defined ranges
                return 0.0;
        }
}

bool parseLine(const string &line, DataSet &data) {
        istringstream iss(line);
        string token;
        vector<string> tokens;
        while (iss >> token) {
                tokens.push_back(token);
        }
        if (tokens.size() == 4) {
                try {
                        data.value = stod(tokens[0]);
                        data.x = stod(tokens[1]);
                        data.y = stod(tokens[2]);
                        data.z = stod(tokens[3]);
                        return true;
                } catch (...) {
                        return false;
                }
        }
        return false;
}


// 辅助函数：将 TPlyEntry 转换为 TriangleMesh
// fieldIndex: 0=vonmises, 1=Temperature, etc. (根据需要扩展)
TriangleMeshIsoLine::TriangleMesh convertToIsoMesh(TPlyEntry* pEntry, int fieldIndex) {
    TriangleMeshIsoLine::TriangleMesh mesh;

    // 1. 确定要使用哪个物理量进行切割
    std::vector<float>* pValueVec = nullptr;
    if (fieldIndex == 0) pValueVec = &pEntry->vonmises;
    else if (fieldIndex == 1) pValueVec = &pEntry->Temperature;
    // ... 其他字段

    if (!pValueVec || pValueVec->size() != pEntry->points.size() / 3) {
        // 如果节点数据不存在，填充默认值
        for (size_t i = 0; i < pEntry->points.size() / 3; i++) {
            mesh.addNode(pEntry->points[i * 3], pEntry->points[i * 3 + 1], pEntry->points[i * 3 + 2], 0.0);
        }
    }
    else {
        // 2. 填充节点和物理量
        for (size_t i = 0; i < pEntry->points.size() / 3; i++) {
            mesh.addNode(
                pEntry->points[i * 3],     // x
                pEntry->points[i * 3 + 1], // y
                pEntry->points[i * 3 + 2], // z
                (*pValueVec)[i]            // value (用于生成等值线)
            );
        }
    }

    // 3. 填充三角形拓扑
    for (size_t i = 0; i < pEntry->triangles.size() / 3; i++) {
        mesh.addTriangle(
            pEntry->triangles[i * 3],
            pEntry->triangles[i * 3 + 1],
            pEntry->triangles[i * 3 + 2]
        );
    }

    // 4. 计算法线 (可选，用于渲染光照)
    mesh.calculateNodeNormals();

    return mesh;
}







void rendererPly(vtkRenderer* renderer, postManage* work, TPlyEntry* pEntry, int isPointMode) {
    if (!pEntry || pEntry->points.empty()) return;

    // =================================================================================
    // 第一阶段：数据适配 (VTK -> TriangleMesh)
    // =================================================================================

    TriangleMeshIsoLine::TriangleMesh inputMesh;

    size_t numNodes = pEntry->points.size() / 3;
    bool hasVonMises = (pEntry->vonmises.size() == numNodes);

    for (size_t i = 0; i < numNodes; i++) {
        double val = hasVonMises ? pEntry->vonmises[i] : 0.0;
        // 传入占位法线 (0,0,1)
        inputMesh.addNode(
            pEntry->points[i * 3],
            pEntry->points[i * 3 + 1],
            pEntry->points[i * 3 + 2],
            val,
            0.0, 0.0, 1.0
        );
    }

    size_t numTris = pEntry->triangles.size() / 3;
    for (size_t i = 0; i < numTris; i++) {
        inputMesh.addTriangle(
            pEntry->triangles[i * 3],
            pEntry->triangles[i * 3 + 1],
            pEntry->triangles[i * 3 + 2]
        );
    }

    // ---------------------------------------------------------
    // 执行切割算法
    // ---------------------------------------------------------
    double minVal, maxVal;
    inputMesh.getValueRange(minVal, maxVal);
    // 防止范围为0导致除零错误
    if (std::abs(maxVal - minVal) < 1e-6) maxVal = minVal + 1.0;
    work->range[0] = minVal;
    work->range[1] = maxVal;

    const int ISO_LEVELS = 10;
    std::vector<double> isoValues;
    double step = (maxVal - minVal) / ISO_LEVELS;
    for (int i = 1; i < ISO_LEVELS; i++) {
        isoValues.push_back(minVal + i * step);
    }

    // 获取切割后的网格数据
    TriangleMeshIsoLine::TriangleMesh splitMesh = inputMesh.splitByIsolines(isoValues);

    // =================================================================================
    // 第二阶段：VTK 数据重建 (根据环境变量分流)
    // =================================================================================

    // 清理旧数据
    renderer->RemoveAllViewProps();
    work->plyPoints->Initialize();
    work->plyShells->Initialize();
    work->plyVonmises->Initialize();
    work->plyData->Initialize();


   


    if (isPointMode == 1) {
        // =========================================================
    // 1. 填充顶点 (保持不变)
    // =========================================================
        const auto& newCoords = splitMesh.getNodeCoords();
        // 【新增】获取节点上的精确数值
        const auto& newValues = splitMesh.getNodeValues();
        size_t newNumNodes = splitMesh.getNodeCount();

        work->plyPoints->Allocate(newNumNodes);
        // 【修改】为 PointData 分配空间
        work->plyVonmises->SetName("vonmises");
        work->plyVonmises->SetNumberOfTuples(newNumNodes);

        for (size_t i = 0; i < newNumNodes; i++) {
            work->plyPoints->InsertNextPoint(
                newCoords[i * 3],
                newCoords[i * 3 + 1],
                newCoords[i * 3 + 2]
            );

            // 【关键修改 1】直接填充点上的数值，而不是计算三角形平均值
            // 这样每个点都有准确的物理量，VTK 会在点之间进行颜色插值
            work->plyVonmises->SetTuple1(i, newValues[i]);
        }

        // =========================================================
        // 2. 填充单元 (只负责拓扑结构)
        // =========================================================
        const auto& newElems = splitMesh.getElements();
        size_t newNumTris = splitMesh.getTriangleCount();
        work->plyShells->Allocate(newNumTris);

        for (size_t i = 0; i < newNumTris; i++) {
            vtkIdType ids[3] = {
                (vtkIdType)newElems[i * 3],
                (vtkIdType)newElems[i * 3 + 1],
                (vtkIdType)newElems[i * 3 + 2]
            };
            work->plyShells->InsertNextCell(3, ids);

            // 【删除】原来在这里计算 avgVal 的代码全部删掉
            // 我们不再需要单元数据了
        }

        // =========================================================
        // 3. 组装 PolyData
        // =========================================================
        work->plyData->SetPoints(work->plyPoints);
        work->plyData->SetPolys(work->plyShells);

        // 【关键修改 2】绑定到 PointData (点数据)
        work->plyData->GetPointData()->AddArray(work->plyVonmises);
        work->plyData->GetPointData()->SetActiveScalars("vonmises");

        // 清理旧的 CellData (防止残留)
        work->plyData->GetCellData()->Initialize();

        // =========================================================
        // 4. 渲染设置 (实现平滑渐变)
        // =========================================================
        work->range[0] = minVal;
        work->range[1] = maxVal;

        // 【关键修改 3】增加 LUT 的分辨率
        // 如果只有 10 个值，颜色过渡会显得只有 10 阶。
        // 改为 256，颜色就会像彩虹一样连续平滑过渡。
        const int LUT_RES = 256;
        work->lut->SetNumberOfTableValues(LUT_RES);
        work->lut->SetRange(minVal, maxVal);

        // 使用 ColorTransferFunction 构建连续色谱
        // 这里保持你的蓝红配色逻辑，但在中间进行连续插值
        vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
        ctf->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
        ctf->AddRGBPoint(0.125, 0.12, 0.32, 0.85);
        ctf->AddRGBPoint(0.25, 0.37, 0.71, 1.0);
        ctf->AddRGBPoint(0.375, 0.38, 1.0, 0.43);
        ctf->AddRGBPoint(0.5, 0.0, 1.0, 0.0);
        ctf->AddRGBPoint(0.625, 0.79, 1.0, 0.09);
        ctf->AddRGBPoint(0.75, 1.0, 1.0, 0.0);
        ctf->AddRGBPoint(0.875, 1.0, 0.70, 0.09);
        ctf->AddRGBPoint(0.95, 1.0, 0.38, 0.0);
        ctf->AddRGBPoint(1.0, 1.0, 0.0, 0.0);

        work->lut->Build(); // 先重置
        for (int i = 0; i < LUT_RES; i++) {
            double ratio = (double)i / (LUT_RES - 1);
            double rgb[3];
            ctf->GetColor(ratio, rgb); // 获取插值后的颜色
            work->lut->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
        }

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->SetScalarRange(minVal, maxVal);
        work->plyMapper->SetLookupTable(work->lut);

        // 【关键修改 4】Mapper 设置
        work->plyMapper->SetScalarModeToUsePointData(); // 使用点数据
        work->plyMapper->SetColorModeToMapScalars();
        work->plyMapper->InterpolateScalarsBeforeMappingOn(); // 开启插值，让颜色平滑融合

        work->plyActor->SetMapper(work->plyMapper);

        // 【可选优化】开启 Gouraud Shading 让光照也平滑
        work->plyActor->GetProperty()->SetInterpolationToGouraud();

        // 环境设置保持不变
        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->GradientBackgroundOn();
        renderer->AddActor(work->plyActor);
    }
    else {
        // *************************************************************
        // 分支 B: 单元渲染模式 (Triangle Rendering - 原有逻辑)
        // *************************************************************
        std::cout << "[DEBUG] 使用单元渲染。" << std::endl;

        // 1. 填充顶点
        const auto& newCoords = splitMesh.getNodeCoords();
        size_t newNumNodes = splitMesh.getNodeCount();
        work->plyPoints->Allocate(newNumNodes);

        for (size_t i = 0; i < newNumNodes; i++) {
            work->plyPoints->InsertNextPoint(
                newCoords[i * 3],
                newCoords[i * 3 + 1],
                newCoords[i * 3 + 2]
            );
        }

        // 2. 填充单元 (三角形)
        const auto& newElems = splitMesh.getElements();
        const auto& nodeVals = splitMesh.getNodeValues();
        size_t newNumTris = splitMesh.getTriangleCount();
        work->plyShells->Allocate(newNumTris);

        work->plyVonmises->SetName("vonmises");
        work->plyVonmises->SetNumberOfTuples(newNumTris);

        for (size_t i = 0; i < newNumTris; i++) {
            vtkIdType ids[3] = {
                (vtkIdType)newElems[i * 3],
                (vtkIdType)newElems[i * 3 + 1],
                (vtkIdType)newElems[i * 3 + 2]
            };
            work->plyShells->InsertNextCell(3, ids);

            // 3. 计算单元颜色值 (平均值)
            double avgVal = (nodeVals[ids[0]] + nodeVals[ids[1]] + nodeVals[ids[2]]) / 3.0;
            work->plyVonmises->SetTuple1(i, avgVal);
        }

        // 组装 PolyData
        work->plyData->SetPoints(work->plyPoints);
        work->plyData->SetPolys(work->plyShells);
        work->plyData->GetCellData()->AddArray(work->plyVonmises);
        work->plyData->GetCellData()->SetActiveScalars("vonmises");

        // --- 4. 渲染设置 ---
        work->range[0] = minVal;
        work->range[1] = maxVal;

        // 使用你现有的 10 色 LUT 逻辑
        work->lut->SetNumberOfTableValues(10);
        work->lut->SetRange(minVal, maxVal);
        work->lut->Build();

        // 硬编码的 蓝 -> 红 颜色表 (与你的原代码一致)
        work->lut->SetTableValue(0, 0.0, 0.0, 1.0);
        work->lut->SetTableValue(1, 0.12, 0.32, 0.85);
        work->lut->SetTableValue(2, 0.37, 0.71, 1.0);
        work->lut->SetTableValue(3, 0.38, 1.0, 0.43);
        work->lut->SetTableValue(4, 0.0, 1.0, 0.0);
        work->lut->SetTableValue(5, 0.79, 1.0, 0.09);
        work->lut->SetTableValue(6, 1.0, 1.0, 0.0);
        work->lut->SetTableValue(7, 1.0, 0.70, 0.09);
        work->lut->SetTableValue(8, 1.0, 0.38, 0.0);
        work->lut->SetTableValue(9, 1.0, 0.0, 0.0);

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->SetScalarRange(minVal, maxVal);
        work->plyMapper->SetLookupTable(work->lut);

        // 【关键】为了获得锐利的色带边缘
        work->plyMapper->SetScalarModeToUseCellData();
        work->plyMapper->SetColorModeToMapScalars();
        work->plyMapper->InterpolateScalarsBeforeMappingOff();

        work->plyActor->SetMapper(work->plyMapper);

        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->GradientBackgroundOn();
        renderer->AddActor(work->plyActor);
    }

}

//后处理云图渲染
void rendererPly1(vtkRenderer* renderer, postManage* work, TPlyEntry* pEntry) {
    if (!pEntry || pEntry->points.empty()) return;

    // =================================================================================
    // 第一阶段：数据适配 (移除法线输入，只传几何和物理值)
    // =================================================================================

    TriangleMeshIsoLine::TriangleMesh inputMesh;

    size_t numNodes = pEntry->points.size() / 3;
    bool hasVonMises = (pEntry->vonmises.size() == numNodes);

    // 【修改 1】不再提取 source normals，让算法只关注几何切割
    for (size_t i = 0; i < numNodes; i++) {
        double val = hasVonMises ? pEntry->vonmises[i] : 0.0;

        // 使用默认法线 (0,0,1) 占位即可，因为我们后续不会用到它
        // 算法库内部插值时会插值这个占位符，但我们在输出阶段会忽略它
        inputMesh.addNode(
            pEntry->points[i * 3],
            pEntry->points[i * 3 + 1],
            pEntry->points[i * 3 + 2],
            val,
            0.0, 0.0, 1.0 // 传入占位法线
        );
    }

    // 填充三角形拓扑 (保持不变)
    size_t numTris = pEntry->triangles.size() / 3;
    for (size_t i = 0; i < numTris; i++) {
        inputMesh.addTriangle(
            pEntry->triangles[i * 3],
            pEntry->triangles[i * 3 + 1],
            pEntry->triangles[i * 3 + 2]
        );
    }

    // ---------------------------------------------------------
    // 执行切割算法 (保持不变)
    // ---------------------------------------------------------
    double minVal, maxVal;
    inputMesh.getValueRange(minVal, maxVal);
    if (std::abs(maxVal - minVal) < 1e-6) maxVal = minVal + 1.0;

    const int ISO_LEVELS = 10;
    std::vector<double> isoValues;
    double step = (maxVal - minVal) / ISO_LEVELS;
    for (int i = 1; i < ISO_LEVELS; i++) {
        isoValues.push_back(minVal + i * step);
    }

    TriangleMeshIsoLine::TriangleMesh splitMesh = inputMesh.splitByIsolines(isoValues);

    // =================================================================================
    // 第二阶段：VTK 数据重建 (【修改 2】移除法线传递)
    // =================================================================================

    renderer->RemoveAllViewProps();
    work->plyPoints->Initialize();
    work->plyShells->Initialize();
    work->plyVonmises->Initialize();
    work->plyData->Initialize();

    // 1. 填充顶点
    const std::vector<double>& newCoords = splitMesh.getNodeCoords();
    size_t newNumNodes = splitMesh.getNodeCount();
    work->plyPoints->Allocate(newNumNodes);

    for (size_t i = 0; i < newNumNodes; i++) {
        work->plyPoints->InsertNextPoint(
            newCoords[i * 3],
            newCoords[i * 3 + 1],
            newCoords[i * 3 + 2]
        );
    }

    // 【修改 3】完全删除法线数组的创建和填充代码
    /* vtkSmartPointer<vtkFloatArray> newNormals = ...
    ...
    work->plyData->GetPointData()->SetNormals(newNormals);
    */
    // 删除上面这些代码！让 VTK 自己去算！

    // 2. 填充单元 (三角形)
    const std::vector<int>& newElems = splitMesh.getElements();
    size_t newNumTris = splitMesh.getTriangleCount();
    work->plyShells->Allocate(newNumTris);

    for (size_t i = 0; i < newNumTris; i++) {
        vtkIdType triangleIndices[3];
        triangleIndices[0] = static_cast<vtkIdType>(newElems[i * 3]);
        triangleIndices[1] = static_cast<vtkIdType>(newElems[i * 3 + 1]);
        triangleIndices[2] = static_cast<vtkIdType>(newElems[i * 3 + 2]);
        work->plyShells->InsertNextCell(3, triangleIndices);
    }

    // 3. 计算单元颜色值
    work->plyVonmises->SetName("vonmises");
    work->plyVonmises->SetNumberOfTuples(newNumTris);
    const std::vector<double>& newNodeValues = splitMesh.getNodeValues();

    for (size_t i = 0; i < newNumTris; i++) {
        int n1 = newElems[i * 3];
        int n2 = newElems[i * 3 + 1];
        int n3 = newElems[i * 3 + 2];
        double val = (newNodeValues[n1] + newNodeValues[n2] + newNodeValues[n3]) / 3.0;
        work->plyVonmises->SetTuple1(i, val);
    }

    // 4. 组装 PolyData
    work->plyData->SetPoints(work->plyPoints);
    work->plyData->SetPolys(work->plyShells);

    // 【修改 4】只绑定颜色数据，不要绑定法线
    work->plyData->GetCellData()->AddArray(work->plyVonmises);
    work->plyData->GetCellData()->SetActiveScalars("vonmises");

    // =================================================================================
    // 第三阶段：渲染配置 (保持不变)
    // =================================================================================

    work->range[0] = minVal;
    work->range[1] = maxVal;

    work->lut->SetNumberOfTableValues(ISO_LEVELS);
    work->lut->SetRange(work->range[0], work->range[1]);

    vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
    ctf->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
    ctf->AddRGBPoint(0.125, 0.12, 0.32, 0.85);
    ctf->AddRGBPoint(0.25, 0.37, 0.71, 1.0);
    ctf->AddRGBPoint(0.375, 0.38, 1.0, 0.43);
    ctf->AddRGBPoint(0.5, 0.0, 1.0, 0.0);
    ctf->AddRGBPoint(0.625, 0.79, 1.0, 0.09);
    ctf->AddRGBPoint(0.75, 1.0, 1.0, 0.0);
    ctf->AddRGBPoint(0.875, 1.0, 0.70, 0.09);
    ctf->AddRGBPoint(0.95, 1.0, 0.38, 0.0);
    ctf->AddRGBPoint(1.0, 1.0, 0.0, 0.0);

    for (int i = 0; i < ISO_LEVELS; i++) {
        double ratio = (double)i / (ISO_LEVELS > 1 ? ISO_LEVELS - 1 : 1);
        double rgb[3];
        ctf->GetColor(ratio, rgb);
        work->lut->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
    }
    work->lut->Build();

    work->plyMapper->SetInputData(work->plyData);
    work->plyMapper->SetScalarRange(work->range);
    work->plyMapper->SetLookupTable(work->lut);
    work->plyMapper->SetScalarModeToUseCellData();
    work->plyMapper->SetColorModeToMapScalars();
    work->plyMapper->InterpolateScalarsBeforeMappingOff();

    work->plyActor->SetMapper(work->plyMapper);

    renderer->SetBackground(0.655, 0.655, 0.737);
    renderer->SetBackground2(0.204, 0.204, 0.404);
    renderer->GradientBackgroundOn();

    renderer->AddActor(work->plyActor);
}



// 通用函数：处理任意属性的等值线切割 + VTK 渲染 (支持点模式/面模式切换)
// pData: 指向你想要可视化的数据向量的指针 (例如 &pEntry->DisX)
// arrayName: VTK 内部使用的数组名称 (例如 "DisX")
void rendererPlyGeneric(vtkRenderer* renderer, postManage* work, TPlyEntry* pEntry,
    std::vector<float>* pData, const char* arrayName ,int isPointMode)
{
    if (!pEntry || pEntry->points.empty() || !pData) return;

    // --- 1. 数据适配 (使用 TriangleMeshIsoLine) ---
    TriangleMeshIsoLine::TriangleMesh inputMesh;
    size_t numNodes = pEntry->points.size() / 3;

    // 安全检查：确保传入的数据大小与节点数量匹配
    if (pData->size() != numNodes) return;

    // 使用传入的特定属性数据 (pData) 填充节点
    for (size_t i = 0; i < numNodes; i++) {
        // 传入 0,0,1 作为占位法线，因为我们使用 Flat Shading (面法线)
        inputMesh.addNode(
            pEntry->points[i * 3],
            pEntry->points[i * 3 + 1],
            pEntry->points[i * 3 + 2],
            (*pData)[i],  // <--- 【关键修改】：使用通用指针获取数据
            0.0, 0.0, 1.0
        );
    }

    // 填充三角形 (直接复制，因为我们修改了 addNode 为 O(1) 复杂度)
    size_t numTris = pEntry->triangles.size() / 3;
    for (size_t i = 0; i < numTris; i++) {
        inputMesh.addTriangle(
            pEntry->triangles[i * 3],
            pEntry->triangles[i * 3 + 1],
            pEntry->triangles[i * 3 + 2]
        );
    }

    // --- 2. 等值线切割 ---
    double minVal, maxVal;
    inputMesh.getValueRange(minVal, maxVal);
    // 防止最大最小值相等导致的除零错误
    if (std::abs(maxVal - minVal) < 1e-6) maxVal = minVal + 1.0;

    const int ISO_LEVELS = 10; // 与你的 10 色 LUT 保持一致
    std::vector<double> isoValues;
    double step = (maxVal - minVal) / ISO_LEVELS;
    for (int i = 1; i < ISO_LEVELS; i++) {
        isoValues.push_back(minVal + i * step);
    }

    TriangleMeshIsoLine::TriangleMesh splitMesh = inputMesh.splitByIsolines(isoValues);


    // --- 0. 环境准备与清理 ---
    renderer->RemoveAllViewProps();
    work->plyPoints->Initialize();
    work->plyShells->Initialize();
    work->plyVonmises->Initialize(); // 复用作标量容器
    work->plyData->Initialize();

    // --- 1. 检查环境变量 ---


    if (isPointMode == 1) {
        // =========================================================
    // 1. 填充顶点 (保持不变)
    // =========================================================
        const auto& newCoords = splitMesh.getNodeCoords();
        // 【新增】获取节点上的精确数值
        const auto& newValues = splitMesh.getNodeValues();
        size_t newNumNodes = splitMesh.getNodeCount();

        work->plyPoints->Allocate(newNumNodes);
        // 【修改】为 PointData 分配空间
        work->plyVonmises->SetName(arrayName);
        work->plyVonmises->SetNumberOfTuples(newNumNodes);

        for (size_t i = 0; i < newNumNodes; i++) {
            work->plyPoints->InsertNextPoint(
                newCoords[i * 3],
                newCoords[i * 3 + 1],
                newCoords[i * 3 + 2]
            );

            // 【关键修改 1】直接填充点上的数值，而不是计算三角形平均值
            // 这样每个点都有准确的物理量，VTK 会在点之间进行颜色插值
            work->plyVonmises->SetTuple1(i, newValues[i]);
        }

        // =========================================================
        // 2. 填充单元 (只负责拓扑结构)
        // =========================================================
        const auto& newElems = splitMesh.getElements();
        size_t newNumTris = splitMesh.getTriangleCount();
        work->plyShells->Allocate(newNumTris);

        for (size_t i = 0; i < newNumTris; i++) {
            vtkIdType ids[3] = {
                (vtkIdType)newElems[i * 3],
                (vtkIdType)newElems[i * 3 + 1],
                (vtkIdType)newElems[i * 3 + 2]
            };
            work->plyShells->InsertNextCell(3, ids);

            // 【删除】原来在这里计算 avgVal 的代码全部删掉
            // 我们不再需要单元数据了
        }

        // =========================================================
        // 3. 组装 PolyData
        // =========================================================
        work->plyData->SetPoints(work->plyPoints);
        work->plyData->SetPolys(work->plyShells);

        // 【关键修改 2】绑定到 PointData (点数据)
        work->plyData->GetPointData()->AddArray(work->plyVonmises);
        work->plyData->GetPointData()->SetActiveScalars(arrayName);

        // 清理旧的 CellData (防止残留)
        work->plyData->GetCellData()->Initialize();

        // =========================================================
        // 4. 渲染设置 (实现平滑渐变)
        // =========================================================
        work->range[0] = minVal;
        work->range[1] = maxVal;

        // 【关键修改 3】增加 LUT 的分辨率
        // 如果只有 10 个值，颜色过渡会显得只有 10 阶。
        // 改为 256，颜色就会像彩虹一样连续平滑过渡。
        const int LUT_RES = 256;
        work->lut->SetNumberOfTableValues(LUT_RES);
        work->lut->SetRange(minVal, maxVal);

        // 使用 ColorTransferFunction 构建连续色谱
        // 这里保持你的蓝红配色逻辑，但在中间进行连续插值
        vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
        ctf->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
        ctf->AddRGBPoint(0.125, 0.12, 0.32, 0.85);
        ctf->AddRGBPoint(0.25, 0.37, 0.71, 1.0);
        ctf->AddRGBPoint(0.375, 0.38, 1.0, 0.43);
        ctf->AddRGBPoint(0.5, 0.0, 1.0, 0.0);
        ctf->AddRGBPoint(0.625, 0.79, 1.0, 0.09);
        ctf->AddRGBPoint(0.75, 1.0, 1.0, 0.0);
        ctf->AddRGBPoint(0.875, 1.0, 0.70, 0.09);
        ctf->AddRGBPoint(0.95, 1.0, 0.38, 0.0);
        ctf->AddRGBPoint(1.0, 1.0, 0.0, 0.0);

        work->lut->Build(); // 先重置
        for (int i = 0; i < LUT_RES; i++) {
            double ratio = (double)i / (LUT_RES - 1);
            double rgb[3];
            ctf->GetColor(ratio, rgb); // 获取插值后的颜色
            work->lut->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
        }

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->SetScalarRange(minVal, maxVal);
        work->plyMapper->SetLookupTable(work->lut);

        // 【关键修改 4】Mapper 设置
        work->plyMapper->SetScalarModeToUsePointData(); // 使用点数据
        work->plyMapper->SetColorModeToMapScalars();
        work->plyMapper->InterpolateScalarsBeforeMappingOn(); // 开启插值，让颜色平滑融合

        work->plyActor->SetMapper(work->plyMapper);

        // 【可选优化】开启 Gouraud Shading 让光照也平滑
        work->plyActor->GetProperty()->SetInterpolationToGouraud();

        // 环境设置保持不变
        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->GradientBackgroundOn();
        renderer->AddActor(work->plyActor);
    }
    else {
        // *************************************************************
        // 分支 B: 单元渲染模式 (Triangle Rendering - 原有逻辑)
        // *************************************************************
        std::cout << "[DEBUG] 使用单元渲染。" << std::endl;

        // 1. 填充顶点
        const auto& newCoords = splitMesh.getNodeCoords();
        size_t newNumNodes = splitMesh.getNodeCount();
        work->plyPoints->Allocate(newNumNodes);

        for (size_t i = 0; i < newNumNodes; i++) {
            work->plyPoints->InsertNextPoint(
                newCoords[i * 3],
                newCoords[i * 3 + 1],
                newCoords[i * 3 + 2]
            );
        }

        // 2. 填充单元 (三角形)
        const auto& newElems = splitMesh.getElements();
        const auto& nodeVals = splitMesh.getNodeValues();
        size_t newNumTris = splitMesh.getTriangleCount();
        work->plyShells->Allocate(newNumTris);

        work->plyVonmises->SetName(arrayName);
        work->plyVonmises->SetNumberOfTuples(newNumTris);

        for (size_t i = 0; i < newNumTris; i++) {
            vtkIdType ids[3] = {
                (vtkIdType)newElems[i * 3],
                (vtkIdType)newElems[i * 3 + 1],
                (vtkIdType)newElems[i * 3 + 2]
            };
            work->plyShells->InsertNextCell(3, ids);

            // 3. 计算单元颜色值 (平均值)
            double avgVal = (nodeVals[ids[0]] + nodeVals[ids[1]] + nodeVals[ids[2]]) / 3.0;
            work->plyVonmises->SetTuple1(i, avgVal);
        }

        // 组装 PolyData
        work->plyData->SetPoints(work->plyPoints);
        work->plyData->SetPolys(work->plyShells);
        work->plyData->GetCellData()->AddArray(work->plyVonmises);
        work->plyData->GetCellData()->SetActiveScalars(arrayName);

        // --- 4. 渲染设置 ---
        work->range[0] = minVal;
        work->range[1] = maxVal;

        // 使用你现有的 10 色 LUT 逻辑
        work->lut->SetNumberOfTableValues(10);
        work->lut->SetRange(minVal, maxVal);
        work->lut->Build();

        // 硬编码的 蓝 -> 红 颜色表 (与你的原代码一致)
        work->lut->SetTableValue(0, 0.0, 0.0, 1.0);
        work->lut->SetTableValue(1, 0.12, 0.32, 0.85);
        work->lut->SetTableValue(2, 0.37, 0.71, 1.0);
        work->lut->SetTableValue(3, 0.38, 1.0, 0.43);
        work->lut->SetTableValue(4, 0.0, 1.0, 0.0);
        work->lut->SetTableValue(5, 0.79, 1.0, 0.09);
        work->lut->SetTableValue(6, 1.0, 1.0, 0.0);
        work->lut->SetTableValue(7, 1.0, 0.70, 0.09);
        work->lut->SetTableValue(8, 1.0, 0.38, 0.0);
        work->lut->SetTableValue(9, 1.0, 0.0, 0.0);

        work->plyMapper->SetInputData(work->plyData);
        work->plyMapper->SetScalarRange(minVal, maxVal);
        work->plyMapper->SetLookupTable(work->lut);

        // 【关键】为了获得锐利的色带边缘
        work->plyMapper->SetScalarModeToUseCellData();
        work->plyMapper->SetColorModeToMapScalars();
        work->plyMapper->InterpolateScalarsBeforeMappingOff();

        work->plyActor->SetMapper(work->plyMapper);

        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);
        renderer->GradientBackgroundOn();
        renderer->AddActor(work->plyActor);
    }
}


// 通用函数：处理任意属性的等值线切割 + VTK 渲染
// pData: 指向你想要可视化的数据向量的指针 (例如 &pEntry->DisX)
// arrayName: VTK 内部使用的数组名称 (例如 "DisX")
void rendererPlyGeneric1(vtkRenderer* renderer, postManage* work, TPlyEntry* pEntry,
    std::vector<float>* pData, const char* arrayName)
{
    if (!pEntry || pEntry->points.empty() || !pData) return;

    // --- 1. 数据适配 (使用 TriangleMeshIsoLine) ---
    TriangleMeshIsoLine::TriangleMesh inputMesh;
    size_t numNodes = pEntry->points.size() / 3;

    // 安全检查：确保传入的数据大小与节点数量匹配
    if (pData->size() != numNodes) return;

    // 使用传入的特定属性数据 (pData) 填充节点
    for (size_t i = 0; i < numNodes; i++) {
        // 传入 0,0,1 作为占位法线，因为我们使用 Flat Shading (面法线)
        inputMesh.addNode(
            pEntry->points[i * 3],
            pEntry->points[i * 3 + 1],
            pEntry->points[i * 3 + 2],
            (*pData)[i],  // <--- 【关键修改】：使用通用指针获取数据
            0.0, 0.0, 1.0
        );
    }

    // 填充三角形 (直接复制，因为我们修改了 addNode 为 O(1) 复杂度)
    size_t numTris = pEntry->triangles.size() / 3;
    for (size_t i = 0; i < numTris; i++) {
        inputMesh.addTriangle(
            pEntry->triangles[i * 3],
            pEntry->triangles[i * 3 + 1],
            pEntry->triangles[i * 3 + 2]
        );
    }

    // --- 2. 等值线切割 ---
    double minVal, maxVal;
    inputMesh.getValueRange(minVal, maxVal);
    // 防止最大最小值相等导致的除零错误
    if (std::abs(maxVal - minVal) < 1e-6) maxVal = minVal + 1.0;

    const int ISO_LEVELS = 10; // 与你的 10 色 LUT 保持一致
    std::vector<double> isoValues;
    double step = (maxVal - minVal) / ISO_LEVELS;
    for (int i = 1; i < ISO_LEVELS; i++) {
        isoValues.push_back(minVal + i * step);
    }

    TriangleMeshIsoLine::TriangleMesh splitMesh = inputMesh.splitByIsolines(isoValues);

    // --- 3. VTK 重建 ---
    renderer->RemoveAllViewProps();
    work->plyPoints->Initialize();
    work->plyShells->Initialize();

    // 我们复用 plyVonmises 作为一个通用的标量容器
    // (你也可以在结构体里把它重命名为 plyScalarData 以便更清晰，但直接复用完全没问题)
    work->plyVonmises->Initialize();
    work->plyData->Initialize();

    // 填充点 (Points)
    size_t newNumNodes = splitMesh.getNodeCount();
    const std::vector<double>& newCoords = splitMesh.getNodeCoords();
    work->plyPoints->Allocate(newNumNodes);
    for (size_t i = 0; i < newNumNodes; i++) {
        work->plyPoints->InsertNextPoint(newCoords[i * 3], newCoords[i * 3 + 1], newCoords[i * 3 + 2]);
    }

    // 填充单元 (Cells)
    size_t newNumTris = splitMesh.getTriangleCount();
    const std::vector<int>& newElems = splitMesh.getElements();
    work->plyShells->Allocate(newNumTris);
    for (size_t i = 0; i < newNumTris; i++) {
        vtkIdType ids[3] = { (vtkIdType)newElems[i * 3], (vtkIdType)newElems[i * 3 + 1], (vtkIdType)newElems[i * 3 + 2] };
        work->plyShells->InsertNextCell(3, ids);
    }

    // 填充单元数据 (Cell Data) - 计算平均值以实现 Flat Shading
    work->plyVonmises->SetName(arrayName); // 动态设置名称
    work->plyVonmises->SetNumberOfTuples(newNumTris);
    const std::vector<double>& nodeVals = splitMesh.getNodeValues();

    for (size_t i = 0; i < newNumTris; i++) {
        double val = (nodeVals[newElems[i * 3]] + nodeVals[newElems[i * 3 + 1]] + nodeVals[newElems[i * 3 + 2]]) / 3.0;
        work->plyVonmises->SetTuple1(i, val);
    }

    // 组装 PolyData
    work->plyData->SetPoints(work->plyPoints);
    work->plyData->SetPolys(work->plyShells);
    work->plyData->GetCellData()->AddArray(work->plyVonmises);
    work->plyData->GetCellData()->SetActiveScalars(arrayName);

    // --- 4. 渲染设置 ---
    work->range[0] = minVal;
    work->range[1] = maxVal;

    // 使用你现有的 10 色 LUT 逻辑
    work->lut->SetNumberOfTableValues(10);
    work->lut->SetRange(minVal, maxVal);
    work->lut->Build();

    // 硬编码的 蓝 -> 红 颜色表 (与你的原代码一致)
    work->lut->SetTableValue(0, 0.0, 0.0, 1.0);
    work->lut->SetTableValue(1, 0.12, 0.32, 0.85);
    work->lut->SetTableValue(2, 0.37, 0.71, 1.0);
    work->lut->SetTableValue(3, 0.38, 1.0, 0.43);
    work->lut->SetTableValue(4, 0.0, 1.0, 0.0);
    work->lut->SetTableValue(5, 0.79, 1.0, 0.09);
    work->lut->SetTableValue(6, 1.0, 1.0, 0.0);
    work->lut->SetTableValue(7, 1.0, 0.70, 0.09);
    work->lut->SetTableValue(8, 1.0, 0.38, 0.0);
    work->lut->SetTableValue(9, 1.0, 0.0, 0.0);

    work->plyMapper->SetInputData(work->plyData);
    work->plyMapper->SetScalarRange(minVal, maxVal);
    work->plyMapper->SetLookupTable(work->lut);

    // 【关键】为了获得锐利的色带边缘
    work->plyMapper->SetScalarModeToUseCellData();
    work->plyMapper->SetColorModeToMapScalars();
    work->plyMapper->InterpolateScalarsBeforeMappingOff();

    work->plyActor->SetMapper(work->plyMapper);

    renderer->SetBackground(0.655, 0.655, 0.737);
    renderer->SetBackground2(0.204, 0.204, 0.404);
    renderer->GradientBackgroundOn();
    renderer->AddActor(work->plyActor);
}

void setPlyPostChange(int attribute, vtkRenderer* renderer, postManage* work, TPlyEntry* pEntry, int isPointMode) {
    if (!pEntry) return;

    // 根据 attribute 的值，调用通用函数处理不同的数据数组

    if (attribute == 1) {
        // VonMises
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->vonmises, "vonmises", isPointMode);
    }
    else if (attribute == 2) {
        // DisX
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->DisX, "DisX", isPointMode);
    }
    else if (attribute == 3) {
        // DisY
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->DisY, "DisY", isPointMode);
    }
    else if (attribute == 4) {
        // DisZ
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->DisZ, "DisZ", isPointMode);
    }
    else if (attribute == 5) {
        // DisTotal
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->DisTotal, "DisTotal", isPointMode);
    }
    else if (attribute == 6) {
        // Temperature
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->Temperature, "Temperature", isPointMode);
    }
    else if (attribute == 7) {
        // SR11
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SR11, "SR11", isPointMode);
    }
    else if (attribute == 8) {
        // SR22
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SR22, "SR22", isPointMode);
    }
    else if (attribute == 9) {
        // SR33
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SR33, "SR33", isPointMode);
    }
    else if (attribute == 10) {
        // SR12
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SR12, "SR12", isPointMode);
    }
    else if (attribute == 11) {
        // SR13
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SR13, "SR13", isPointMode);
    }
    else if (attribute == 12) {
        // SR23
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SR23, "SR23", isPointMode);
    }
    else if (attribute == 13) {
        // SS11
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SS11, "SS11", isPointMode);
    }
    else if (attribute == 14) {
        // SS22
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SS22, "SS22", isPointMode);
    }
    else if (attribute == 15) {
        // SS33
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SS33, "SS33", isPointMode);
    }
    else if (attribute == 16) {
        // SS12
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SS12, "SS12", isPointMode);
    }
    else if (attribute == 17) {
        // SS13
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SS13, "SS13", isPointMode);
    }
    else if (attribute == 18) {
        // SS23
        rendererPlyGeneric(renderer, work, pEntry, &pEntry->SS23, "SS23", isPointMode);
    }
}



vector<float> extractNumbers(const string &str) {
        vector<float> numbers;
        stringstream ss(str);
        string temp;
        while (ss >> temp) {
                double num;
                stringstream convert(temp);
                if (convert >> num) {
                        numbers.push_back(num);
                }
        }
        return numbers;
}





// 结构后处理颜色条-针对Ply的
void setPlyPostBar(int attribute, vtkRenderer* renderer, postManage* work) {
    work->scalarBarActor->SetLookupTable(NULL);
    // work->scalarBarActor->SetLookupTable(work->plyColorMap);
    work->scalarBarActor->SetLookupTable(work->lut);
    string minMax = "MinValue: " + std::to_string(work->range[0]) + "\n" +
        "MaxValue: " + std::to_string(work->range[1]);
    string title = "";
    switch (attribute) {
    case 1:
        title = "vonmises(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 2:
        title = "DistanceX(mm)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 3:
        title = "DistanceY(mm)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 4:
        title = "DistanceZ(mm)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 5:
        title = "DistanceTotal(mm)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
        // --- 新增项 ---
    case 6:
        title = "Temperature(mm)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
        // --- 以下顺延 ---
    case 7: // 原 case 6
        title = "StrainX(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 8: // 原 case 7
        title = "StrainY(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 9: // 原 case 8
        title = "StrainZ(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 10: // 原 case 9
        title = "StrainXX(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 11: // 原 case 10
        title = "StrainYY(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 12: // 原 case 11
        title = "StrainZZ(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 13: // 原 case 12
        title = "StressX(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 14: // 原 case 13
        title = "StressY(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 15: // 原 case 14
        title = "StressZ(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 16: // 原 case 15
        title = "StressXX(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 17: // 原 case 16
        title = "StressYY(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    case 18: // 原 case 17
        title = "StressZZ(MPa)\n" + minMax;
        work->scalarBarActor->SetTitle(title.c_str());
        break;
    default:
        break;
    }
    // 字体颜色设置
    work->scalarBarActor->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);
    work->scalarBarActor->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);
    work->scalarBarActor->SetLabelFormat("%.3e");
    // work->scalarBarActor->SetLabelFormat("%.2f");
    work->scalarBarActor->GetTitleTextProperty()->SetFontSize(40);
    work->scalarBarActor->GetLabelTextProperty()->SetFontSize(40);
    work->scalarBarActor->SetNumberOfLabels(10);
    work->scalarBarActor->SetMaximumWidthInPixels(160);
    work->scalarBarActor->SetVerticalTitleSeparation(10);
    work->scalarBarActor->GetTitleTextProperty()->SetShadow(false);
    work->scalarBarActor->GetTitleTextProperty()->SetFontFamilyToArial();
    work->scalarBarActor->GetLabelTextProperty()->SetShadow(false);
    work->scalarBarActor->GetLabelTextProperty()->SetFontFamilyToArial();


    renderer->AddActor(work->scalarBarActor);

    // renderer->Render();
}
// 云图切割（使用中）
void plyCutter(double x, double y, double z, double xn, double yn, double zn,
               vtkRenderer *renderer, postManage *work) {
        // 创建 vtkClipPolyData 对象
        renderer->RemoveAllViewProps();
        work->vtkPlanes->RemoveAllItems();

        work->clipper->SetInputData(work->plyData);

        // 创建裁剪平面等相关配置，这里简单示例，比如创建一个平面
        vtkSmartPointer<vtkPlane> clipPlane = vtkSmartPointer<vtkPlane>::New();
        clipPlane->SetOrigin(x, y, z);
        clipPlane->SetNormal(xn, yn, zn); // 设置平面的法向量等参数
        work->vtkPlanes->AddItem(clipPlane);
        work->clipper->SetClippingPlanes(work->vtkPlanes);
        work->clipper->Update();

        // work->resamplers->SetInputData(work->clipper->GetOutput());
        // work->resamplers->SetSourceData(work->structuredGrid);
        // work->resamplers->Update();
        // work->fillMapper->SetInputData(clipedData);
        work->fillMapper->SetInputConnection(work->clipper->GetOutputPort());
        work->fillMapper->ScalarVisibilityOn();
        work->fillMapper->SetLookupTable(work->plyColorMap);
        // work->fillMapper->SetScalarModeToUsePointData();  //
        // 注释掉，使用rendererPly中设置的单元数据模式
        work->fillMapper->SetScalarRange(work->range[0], work->range[1]);
        work->fillMapper->SetColorModeToDefault();
        work->fillMapper->Update();
        work->fillActor->SetMapper(work->fillMapper);
        renderer->AddActor(work->fillActor);
}

// 显示动力学中的渲染
string OnRenderCollision(string path, vtkRenderer *renderer,
                         postCollision *work) {
        renderer->RemoveAllViewProps();
        const std::string folderPath = path + "/mpm";
        work->path = path;
        work->fileCount = 0;
        for (const auto &entry : fs::directory_iterator(folderPath)) {
                if (fs::is_regular_file(entry.path())) {
                        work->fileCount++;
                }
        }
        // range[0] = sigma_xx[0];
        // range[1] = sigma_xx[1];
        string pathVtk = path + "/mpm/box3D_FEMmpData1.vtk";
        work->reader->SetFileName(pathVtk.c_str());
        work->reader->ReadAllVectorsOn();
        work->reader->ReadAllScalarsOn();
        work->reader->ReadAllFieldsOn();
        work->reader->Update();

        int num1 = work->reader->GetNumberOfScalarsInFile(); // 读取标量数量
        int num2 = work->reader->GetNumberOfVectorsInFile(); // 读取矢量数量
        vector<string> scalarname_;
        for (int i = 0; i < num1; i++) {
                string scalarname =
                    work->reader->GetScalarsNameInFile(i); // 取到每个标量名
                scalarname_.push_back(scalarname); // 存储每个标量名
        }

        vector<string> vectorname_;
        for (int i = 0; i < num2; i++) {
                string vectorname =
                    work->reader->GetVectorsNameInFile(i); // 取到每个矢量
                vectorname_.push_back(vectorname);         // 存储每个矢量
        }
        string res = "scalarname: ";
        for (auto a : scalarname_) {
                res += a + " ";
        }
        res += "vectorname: ";
        cout << "------------vectorname----------" << endl;
        for (auto b : vectorname_) {
                res += b + " ";
        }

        // work->reader->SetScalarsName("sigma_xx");//设置单个的标量，vector存储了标量名，想显示哪个就传入哪个
        // work->reader->Update();

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint =
            vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(work->reader->GetOutputPort());
        cellToPoint->Update();

        // 获取转换后的数据集
        vtkDataSet *outputDataSet = cellToPoint->GetOutput();

        // 获取点数据
        vtkPointData *pointData = outputDataSet->GetPointData();

        vtkDataArray *sigmaXXArray = pointData->GetArray("sigma_xx");
        if (!sigmaXXArray) {
                cout << "error" << endl;
        }

        // 获取标量的最大最小值
        sigmaXXArray->GetRange(work->range);

        work->colorMap->RemoveAllPoints();
        // 彩虹图，从红色到蓝色，共 10 个颜色点
        work->colorMap->AddRGBPoint(work->range[0], 1.0, 0.0, 0.0); // 红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.1 * (work->range[1] - work->range[0]),
                                    1.0, 0.38, 0.0); // 橙红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.2 * (work->range[1] - work->range[0]),
                                    1.0, 0.76, 0.0); // 黄色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.3 * (work->range[1] - work->range[0]),
                                    0.85, 1.0, 0.0); // 黄绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.4 * (work->range[1] - work->range[0]),
                                    0.47, 1.0, 0.0); // 绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.5 * (work->range[1] - work->range[0]),
                                    0.09, 1.0, 0); // 青绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.6 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.29); // 青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.7 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.67); // 蓝青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.8 * (work->range[1] - work->range[0]),
                                    0.0, 0.95, 1.0); // 浅蓝色
        work->colorMap->AddRGBPoint(work->range[1], 0.0, 0.56, 1.0); // 蓝色

        work->colorMap->SetRange(work->range[0], work->range[1]);
        // work->mapper->SetInputData(work->reader->GetOutput());
        work->mapper->SetInputData(outputDataSet);
        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0],
                                     work->range[1]); // 设置到mapper
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetColorModeToDefault();
        work->mapper->SetLookupTable(work->colorMap);

        renderer->GradientBackgroundOn();
        renderer->SetBackground(0.655, 0.655, 0.737);
        renderer->SetBackground2(0.204, 0.204, 0.404);

        // reader->GetOutput()->GetScalarRange(range);

        work->actor->SetMapper(work->mapper);
        renderer->AddActor(work->actor);

        return res;
}
// 显示动力学云图切换
void OnChangePostBarCollision(string name, vtkRenderer *renderer,
                              postCollision *work) {
        work->reader->SetScalarsName(
            name.c_str()); // 设置单个的标量，vector存储了标量名，想显示哪个就传入哪个
        work->reader->Update();

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint =
            vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(work->reader->GetOutputPort());
        cellToPoint->Update();

        // 获取转换后的数据集
        vtkDataSet *outputDataSet = cellToPoint->GetOutput();

        // 获取点数据
        vtkPointData *pointData = outputDataSet->GetPointData();

        vtkDataArray *sigmaXXArray = pointData->GetArray(name.c_str());
        work->currentName = name;
        if (!sigmaXXArray) {
                cout << "error" << endl;
        }

        // 获取标量的最大最小值
        sigmaXXArray->GetRange(work->range);
        // cout << "min " << work->range[0] << "max " << work->range[1] << endl;
        work->colorMap->RemoveAllPoints();
        // 彩虹图，从红色到蓝色，共 10 个颜色点
        work->colorMap->AddRGBPoint(work->range[0], 1.0, 0.0, 0.0); // 红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.1 * (work->range[1] - work->range[0]),
                                    1.0, 0.38, 0.0); // 橙红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.2 * (work->range[1] - work->range[0]),
                                    1.0, 0.76, 0.0); // 黄色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.3 * (work->range[1] - work->range[0]),
                                    0.85, 1.0, 0.0); // 黄绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.4 * (work->range[1] - work->range[0]),
                                    0.47, 1.0, 0.0); // 绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.5 * (work->range[1] - work->range[0]),
                                    0.09, 1.0, 0); // 青绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.6 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.29); // 青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.7 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.67); // 蓝青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.8 * (work->range[1] - work->range[0]),
                                    0.0, 0.95, 1.0); // 浅蓝色
        work->colorMap->AddRGBPoint(work->range[1], 0.0, 0.56, 1.0); // 蓝色
        work->colorMap->SetRange(work->range[0], work->range[1]);
        // work->reader->GetOutput()->GetPointData()->GetScalars()->GetRange(work->range);
        // this->_pVTKWidget->renderWindow()->Render();
        work->scalarBarActor->SetLookupTable(work->colorMap);
        work->scalarBarActor->SetTitle(name.c_str());
        work->scalarBarActor->GetTitleTextProperty()->SetColor(1.0, 1.0, 1.0);
        work->scalarBarActor->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);
        work->scalarBarActor->SetLabelFormat("%.2e");
        work->scalarBarActor->SetNumberOfLabels(6);
        work->scalarBarActor->SetMaximumWidthInPixels(160);
        work->scalarBarActor->SetVerticalTitleSeparation(10);

        renderer->AddActor2D(work->scalarBarActor);
}
// 显示左快播功能
int OnleftFastPlayCollision(vtkRenderer *renderer, postCollision *work) {
        work->frame = 1;
        string paths = work->path + "/mpm/box3D_FEMmpData" +
                       to_string(work->frame) + ".vtk";
        work->reader->SetFileName(paths.c_str());
        work->reader->ReadAllVectorsOn();
        work->reader->ReadAllScalarsOn();
        work->reader->ReadAllFieldsOn();
        work->reader->Update();

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint =
            vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(work->reader->GetOutputPort());
        cellToPoint->Update();
        vtkDataSet *outputDataSet = cellToPoint->GetOutput();
        vtkPointData *pointData = outputDataSet->GetPointData();
        vtkDataArray *sigmaXXArray =
            pointData->GetArray(work->currentName.c_str());
        if (!sigmaXXArray) {
                cout << "error" << endl;
        }
        sigmaXXArray->GetRange(work->range);
        work->colorMap->RemoveAllPoints();
        // 彩虹图，从红色到蓝色，共 10 个颜色点
        work->colorMap->AddRGBPoint(work->range[0], 1.0, 0.0, 0.0); // 红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.1 * (work->range[1] - work->range[0]),
                                    1.0, 0.38, 0.0); // 橙红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.2 * (work->range[1] - work->range[0]),
                                    1.0, 0.76, 0.0); // 黄色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.3 * (work->range[1] - work->range[0]),
                                    0.85, 1.0, 0.0); // 黄绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.4 * (work->range[1] - work->range[0]),
                                    0.47, 1.0, 0.0); // 绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.5 * (work->range[1] - work->range[0]),
                                    0.09, 1.0, 0); // 青绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.6 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.29); // 青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.7 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.67); // 蓝青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.8 * (work->range[1] - work->range[0]),
                                    0.0, 0.95, 1.0); // 浅蓝色
        work->colorMap->AddRGBPoint(work->range[1], 0.0, 0.56, 1.0); // 蓝色
        work->colorMap->SetRange(work->range[0], work->range[1]);
        // cout << "min " << work->range[0] << "max " << work->range[1] << endl;
        work->mapper->SetInputData(outputDataSet);
        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0],
                                     work->range[1]); // 设置到mapper
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetColorModeToDefault();
        work->mapper->SetLookupTable(work->colorMap);

        return work->frame;
}
// 显示右快播功能
int OnrightFastPlayCollision(vtkRenderer *renderer, postCollision *work) {
        work->frame = work->fileCount;
        string paths = work->path + "/mpm/box3D_FEMmpData" +
                       to_string(work->frame) + ".vtk";
        work->reader->SetFileName(paths.c_str());
        work->reader->ReadAllVectorsOn();
        work->reader->ReadAllScalarsOn();
        work->reader->ReadAllFieldsOn();
        work->reader->Update();

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint =
            vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(work->reader->GetOutputPort());
        cellToPoint->Update();
        vtkDataSet *outputDataSet = cellToPoint->GetOutput();
        vtkPointData *pointData = outputDataSet->GetPointData();
        vtkDataArray *sigmaXXArray =
            pointData->GetArray(work->currentName.c_str());
        if (!sigmaXXArray) {
                cout << "error" << endl;
        }
        sigmaXXArray->GetRange(work->range);
        work->colorMap->RemoveAllPoints();
        // 彩虹图，从红色到蓝色，共 10 个颜色点
        work->colorMap->AddRGBPoint(work->range[0], 1.0, 0.0, 0.0); // 红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.1 * (work->range[1] - work->range[0]),
                                    1.0, 0.38, 0.0); // 橙红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.2 * (work->range[1] - work->range[0]),
                                    1.0, 0.76, 0.0); // 黄色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.3 * (work->range[1] - work->range[0]),
                                    0.85, 1.0, 0.0); // 黄绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.4 * (work->range[1] - work->range[0]),
                                    0.47, 1.0, 0.0); // 绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.5 * (work->range[1] - work->range[0]),
                                    0.09, 1.0, 0); // 青绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.6 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.29); // 青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.7 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.67); // 蓝青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.8 * (work->range[1] - work->range[0]),
                                    0.0, 0.95, 1.0); // 浅蓝色
        work->colorMap->AddRGBPoint(work->range[1], 0.0, 0.56, 1.0); // 蓝色
        work->colorMap->SetRange(work->range[0], work->range[1]);
        // cout << "min " << work->range[0] << "max " << work->range[1] << endl;
        work->mapper->SetInputData(outputDataSet);
        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0],
                                     work->range[1]); // 设置到mapper
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetColorModeToDefault();
        work->mapper->SetLookupTable(work->colorMap);
        return work->frame;
}
// 显示左播放功能
int OnleftPlayCollision(vtkRenderer *renderer, postCollision *work) {
        if (work->frame == 1) {
                work->frame = work->fileCount;
        } else {
                work->frame = (work->frame - 1) % work->fileCount;
        }

        string paths = work->path + "/mpm/box3D_FEMmpData" +
                       to_string(work->frame) + ".vtk";
        work->reader->SetFileName(paths.c_str());
        work->reader->ReadAllVectorsOn();
        work->reader->ReadAllScalarsOn();
        work->reader->ReadAllFieldsOn();
        work->reader->Update();

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint =
            vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(work->reader->GetOutputPort());
        cellToPoint->Update();
        vtkDataSet *outputDataSet = cellToPoint->GetOutput();
        vtkPointData *pointData = outputDataSet->GetPointData();
        vtkDataArray *sigmaXXArray =
            pointData->GetArray(work->currentName.c_str());
        if (!sigmaXXArray) {
                cout << "error" << endl;
        }
        sigmaXXArray->GetRange(work->range);
        work->colorMap->RemoveAllPoints();
        // 彩虹图，从红色到蓝色，共 10 个颜色点
        work->colorMap->AddRGBPoint(work->range[0], 1.0, 0.0, 0.0); // 红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.1 * (work->range[1] - work->range[0]),
                                    1.0, 0.38, 0.0); // 橙红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.2 * (work->range[1] - work->range[0]),
                                    1.0, 0.76, 0.0); // 黄色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.3 * (work->range[1] - work->range[0]),
                                    0.85, 1.0, 0.0); // 黄绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.4 * (work->range[1] - work->range[0]),
                                    0.47, 1.0, 0.0); // 绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.5 * (work->range[1] - work->range[0]),
                                    0.09, 1.0, 0); // 青绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.6 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.29); // 青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.7 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.67); // 蓝青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.8 * (work->range[1] - work->range[0]),
                                    0.0, 0.95, 1.0); // 浅蓝色
        work->colorMap->AddRGBPoint(work->range[1], 0.0, 0.56, 1.0); // 蓝色
        work->colorMap->SetRange(work->range[0], work->range[1]);
        // cout << "min " << work->range[0] << "max " << work->range[1] << endl;
        work->mapper->SetInputData(outputDataSet);
        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0],
                                     work->range[1]); // 设置到mapper
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetColorModeToDefault();
        work->mapper->SetLookupTable(work->colorMap);
        return work->frame;
}
// 显示右播放功能
int OnrightPlayCollision(vtkRenderer *renderer, postCollision *work) {
        if (work->frame == work->fileCount) {
                work->frame = 1;
        } else if (work->frame == (work->fileCount - 1)) {
                work->frame = work->frame + 1;
        } else {
                work->frame = (work->frame + 1) % work->fileCount;
        }
        string paths = work->path + "/mpm/box3D_FEMmpData" +
                       to_string(work->frame) + ".vtk";
        work->reader->SetFileName(paths.c_str());
        work->reader->ReadAllVectorsOn();
        work->reader->ReadAllScalarsOn();
        work->reader->ReadAllFieldsOn();
        work->reader->Update();

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint =
            vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(work->reader->GetOutputPort());
        cellToPoint->Update();
        vtkDataSet *outputDataSet = cellToPoint->GetOutput();
        vtkPointData *pointData = outputDataSet->GetPointData();
        vtkDataArray *sigmaXXArray =
            pointData->GetArray(work->currentName.c_str());
        if (!sigmaXXArray) {
                cout << "error" << endl;
        }
        sigmaXXArray->GetRange(work->range);
        work->colorMap->RemoveAllPoints();
        // 彩虹图，从红色到蓝色，共 10 个颜色点
        work->colorMap->AddRGBPoint(work->range[0], 1.0, 0.0, 0.0); // 红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.1 * (work->range[1] - work->range[0]),
                                    1.0, 0.38, 0.0); // 橙红色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.2 * (work->range[1] - work->range[0]),
                                    1.0, 0.76, 0.0); // 黄色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.3 * (work->range[1] - work->range[0]),
                                    0.85, 1.0, 0.0); // 黄绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.4 * (work->range[1] - work->range[0]),
                                    0.47, 1.0, 0.0); // 绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.5 * (work->range[1] - work->range[0]),
                                    0.09, 1.0, 0); // 青绿色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.6 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.29); // 青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.7 * (work->range[1] - work->range[0]),
                                    0.0, 1.0, 0.67); // 蓝青色
        work->colorMap->AddRGBPoint(work->range[0] +
                                        0.8 * (work->range[1] - work->range[0]),
                                    0.0, 0.95, 1.0); // 浅蓝色
        work->colorMap->AddRGBPoint(work->range[1], 0.0, 0.56, 1.0); // 蓝色
        work->colorMap->SetRange(work->range[0], work->range[1]);
        // cout << "min " << work->range[0] << "max " << work->range[1] << endl;
        work->mapper->SetInputData(outputDataSet);
        work->mapper->SetScalarModeToUsePointData();
        work->mapper->SetScalarRange(work->range[0],
                                     work->range[1]); // 设置到mapper
        work->mapper->ScalarVisibilityOn();
        work->mapper->SetColorModeToDefault();
        work->mapper->SetLookupTable(work->colorMap);
        return work->frame;
}
// 固定接口
// Collision对应显示动力学
#define ONGETFRAMES_FUNC_USAGE                                                 \
        "OnGetFrames Usage: "                                                  \
        "comx.ply.OnGetFrames(vtkRenderer *renderer, postCollision *work);"
JS_EXT_FUNC_BEGIN(OnGetFrames, 2, ONGETFRAMES_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // OnrightPlayCollision(renderer, post);

        int res = type_cast<int>(post->fileCount);
        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define ONRIGHTPLAYCOLLISIONS_FUNC_USAGE                                       \
        "OnrightPlayCollisions Usage: "                                        \
        "comx.ply.OnrightPlayCollisions(vtkRenderer *renderer, postCollision " \
        "*work, string "                                                       \
        "path);"
JS_EXT_FUNC_BEGIN(OnrightPlayCollisions, 2, ONRIGHTPLAYCOLLISIONS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // OnrightPlayCollision(renderer, post);

        int res = type_cast<int>(OnrightPlayCollision(renderer, post));
        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define ONLEFTPLAYCOLLISIONS_FUNC_USAGE                                        \
        "OnleftPlayCollisions Usage: "                                         \
        "comx.ply.OnleftPlayCollisions(vtkRenderer *renderer, postCollision "  \
        "*work, string "                                                       \
        "path);"
JS_EXT_FUNC_BEGIN(OnleftPlayCollisions, 2, ONLEFTPLAYCOLLISIONS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        int res = type_cast<int>(OnleftPlayCollision(renderer, post));
        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define ONRIGHTFASTPLAYCOLLISIONS_FUNC_USAGE                                   \
        "OnrightFastPlayCollisions Usage: "                                    \
        "comx.ply.OnrightFastPlayCollisions(vtkRenderer *renderer, "           \
        "postCollision *work, string "                                         \
        "path);"
JS_EXT_FUNC_BEGIN(OnrightFastPlayCollisions, 2,
                  ONRIGHTFASTPLAYCOLLISIONS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        int res = type_cast<int>(OnrightFastPlayCollision(renderer, post));
        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define ONLEGTFASTPLAYCOLLISIONS_FUNC_USAGE                                    \
        "OnleftFastPlayCollisions Usage: "                                     \
        "comx.ply.OnleftFastPlayCollisions(vtkRenderer *renderer, "            \
        "postCollision *work, string "                                         \
        "path);"
JS_EXT_FUNC_BEGIN(OnleftFastPlayCollisions, 2,
                  ONLEGTFASTPLAYCOLLISIONS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        int res = type_cast<int>(OnleftFastPlayCollision(renderer, post));
        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define ONCHANGEPOSTBARCOLLISIONS_FUNC_USAGE                                   \
        "OnChangePostBarCollisions Usage: "                                    \
        "comx.ply.OnChangePostBarCollisions(vtkRenderer *renderer, "           \
        "postCollision *work, string "                                         \
        "path);"
JS_EXT_FUNC_BEGIN(OnChangePostBarCollisions, 3,
                  ONCHANGEPOSTBARCOLLISIONS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        string name = JS_EXT_PARA(string, 2);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        OnChangePostBarCollision(name, renderer, post);

        // int res = type_cast<int>(pEntry->path.size());
}
JS_EXT_FUNC_END()

#define ONRENDERCOLLISIONS_FUNC_USAGE                                          \
        "OnRenderCollisions Usage: "                                           \
        "comx.ply.OnRenderCollisions(vtkRenderer *renderer, postCollision "    \
        "*work, string "                                                       \
        "path);"
JS_EXT_FUNC_BEGIN(OnRenderCollisions, 3, ONRENDERCOLLISIONS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postCollision *post = (postCollision *)((void *)ullWorkData);

        string path = JS_EXT_PARA(string, 2);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string res = OnRenderCollision(path, renderer, post);

        // int res = type_cast<int>(pEntry->path.size());

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define SETPLYCHANGEDATA_FUNC_USAGE                                            \
        "setPlyPostChanges Usage: "                                            \
        "comx.ply.setPlyPostChanges(vtkRenderer *renderer, postManage *work, " \
        "int "                                                                 \
        "attribute);"
JS_EXT_FUNC_BEGIN(setPlyPostChanges, 5, SETPLYCHANGEDATA_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *post = (postManage *)((void *)ullWorkData);

        unsigned long long ullpEntry = JS_EXT_PARA(unsigned long long, 2);
        TPlyEntry *pEntry = (TPlyEntry *)((void *)ullpEntry);

        int attribute = JS_EXT_PARA(int, 3);

        int isPointMode = JS_EXT_PARA(int, 4);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        setPlyPostChange(attribute, renderer, post, pEntry, isPointMode);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()
#define LOADVTPPLYS_FUNC_USAGE                                                 \
        "LoadVtpPlys Usage: "                                                  \
        "comx.ply.LoadVtpPlys(vtkRenderer* renderer, postManage* work, "       \
        "string postFile, string postFile1);"
JS_EXT_FUNC_BEGIN(LoadVtpPlys, 4, LOADVTPPLYS_FUNC_USAGE) {
        // Put your codes here
        cout << 1 << endl;
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *post = (postManage *)((void *)ullWorkData);
        // setWorkData(pEntry);

        string name1 = JS_EXT_PARA(string, 2);
        string name2 = JS_EXT_PARA(string, 3);
        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        LoadVtpPly(name1, name2, renderer);
}
JS_EXT_FUNC_END()

#define RENDERERPLYS_FUNC_USAGE                                                \
        "rendererPlys Usage: "                                                 \
        "comx.ply.rendererPlys(vtkRenderer* renderer, postManage* work, "      \
        "TPlyEntry* pEntry, string postFile);"
JS_EXT_FUNC_BEGIN(rendererPlys, 5, RENDERERPLYS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *post = (postManage *)((void *)ullWorkData);
        // setWorkData(pEntry);

        unsigned long long ullpEntry = JS_EXT_PARA(unsigned long long, 2);
        TPlyEntry *pEntry = (TPlyEntry *)((void *)ullpEntry);

        string postFile = JS_EXT_PARA(string, 3);

        int isPointMode = JS_EXT_PARA(int, 4);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        ifstream file(postFile);
        if (!file) {
                cerr << "无法打开文件" << endl;
        }

        string line;
        int index = 0;
        while (getline(file, line)) {
                if (index == 0) {
                        pEntry->vonmises = extractNumbers(line);
                } else if (index == 1) {
                        pEntry->DisX = extractNumbers(line);
                } else if (index == 2) {
                        pEntry->DisY = extractNumbers(line);
                } else if (index == 3) {
                        pEntry->DisZ = extractNumbers(line);
                } else if (index == 4) {
                        pEntry->DisTotal = extractNumbers(line);
                } // [修改] Index 5 改为 Temperature
                else if (index == 5) {
                    pEntry->Temperature = extractNumbers(line);
                }
                else if (index == 6) {
                    pEntry->SR11 = extractNumbers(line);
                }
                else if (index == 7) {
                    pEntry->SR12 = extractNumbers(line);
                }
                else if (index == 8) {
                    pEntry->SR13 = extractNumbers(line);
                }
                else if (index == 9) {
                    pEntry->SR22 = extractNumbers(line);
                }
                else if (index == 10) {
                    pEntry->SR23 = extractNumbers(line);
                }
                else if (index == 11) {
                    pEntry->SR33 = extractNumbers(line);
                }
                else if (index == 12) {
                    pEntry->SS11 = extractNumbers(line);
                }
                else if (index == 13) {
                    pEntry->SS12 = extractNumbers(line);
                }
                else if (index == 14) {
                    pEntry->SS13 = extractNumbers(line);
                }
                else if (index == 15) {
                    pEntry->SS22 = extractNumbers(line);
                }
                else if (index == 16) {
                    pEntry->SS23 = extractNumbers(line);
                }
                else if (index == 17) {
                    pEntry->SS33 = extractNumbers(line);
                }
                index++;
        }
        rendererPly(renderer, post, pEntry, isPointMode);
}
JS_EXT_FUNC_END()

#define MINMAXVALUES_FUNC_USAGE                                                \
        "maxMinValues Usage: "                                                 \
        "comx.ply.maxMinValues(IVtkContext *pVtkContext, workData *work, "     \
        "string path, string type);"
JS_EXT_FUNC_BEGIN(maxMinValues, 4, MINMAXVALUES_FUNC_USAGE) {
    unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
    IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);

    unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
    postManage* pEntry = (postManage*)((void*)ullWorkData);

    string path = JS_EXT_PARA(string, 2);
    string type = JS_EXT_PARA(string, 3);

    if (!_init_callback) {
        _init_callback = true;
        pVtkContext->RegisterMouseListenCallback(callbackMouse);
    }

    void* pRawRenderer = NULL;
    pVtkContext->GetRenderer(pRawRenderer);
    vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;

    // --------------------------
    // 所有 DataSet
    // --------------------------
    DataSet VomisesMax, VomisesMin;
    DataSet DisXMax, DisXMin;
    DataSet DisYMax, DisYMin;
    DataSet DisZMax, DisZMin;
    DataSet DisTotalMax, DisTotalMin;

    DataSet SR11Max, SR11Min;
    DataSet SR22Max, SR22Min;
    DataSet SR33Max, SR33Min;
    DataSet SR12Max, SR12Min;
    DataSet SR13Max, SR13Min;
    DataSet SR23Max, SR23Min;

    DataSet SS11Max, SS11Min;
    DataSet SS22Max, SS22Min;
    DataSet SS33Max, SS33Min;
    DataSet SS12Max, SS12Min;
    DataSet SS13Max, SS13Min;
    DataSet SS23Max, SS23Min;

    // [新增] 温度变量声明
    DataSet TemperatureMax, TemperatureMin;

    // --------------------------
    // 读取文件
    // --------------------------
    ifstream file(path);
    if (!file) {
        cerr << "无法打开文件" << endl;
    }

    string line;
    while (getline(file, line)) {

        if (line == "&VomisesMax X Y Z") {
            getline(file, line); parseLine(line, VomisesMax);
        }
        else if (line == "&VomisesMin X Y Z") {
            getline(file, line); parseLine(line, VomisesMin);
        }

        else if (line == "&DisXMax X Y Z") {
            getline(file, line); parseLine(line, DisXMax);
        }
        else if (line == "&DisXMin X Y Z") {
            getline(file, line); parseLine(line, DisXMin);
        }

        else if (line == "&DisYMax X Y Z") {
            getline(file, line); parseLine(line, DisYMax);
        }
        else if (line == "&DisYMin X Y Z") {
            getline(file, line); parseLine(line, DisYMin);
        }

        else if (line == "&DisZMax X Y Z") {
            getline(file, line); parseLine(line, DisZMax);
        }
        else if (line == "&DisZMin X Y Z") {
            getline(file, line); parseLine(line, DisZMin);
        }

        else if (line == "&DisTotalMax X Y Z") {
            getline(file, line); parseLine(line, DisTotalMax);
        }
        else if (line == "&DisTotalMin X Y Z") {
            getline(file, line); parseLine(line, DisTotalMin);
        }

        else if (line == "&SR11Max X Y Z") {
            getline(file, line); parseLine(line, SR11Max);
        }
        else if (line == "&SR11Min X Y Z") {
            getline(file, line); parseLine(line, SR11Min);
        }

        else if (line == "&SR22Max X Y Z") {
            getline(file, line); parseLine(line, SR22Max);
        }
        else if (line == "&SR22Min X Y Z") {
            getline(file, line); parseLine(line, SR22Min);
        }

        else if (line == "&SR33Max X Y Z") {
            getline(file, line); parseLine(line, SR33Max);
        }
        else if (line == "&SR33Min X Y Z") {
            getline(file, line); parseLine(line, SR33Min);
        }

        else if (line == "&SR12Max X Y Z") {
            getline(file, line); parseLine(line, SR12Max);
        }
        else if (line == "&SR12Min X Y Z") {
            getline(file, line); parseLine(line, SR12Min);
        }

        else if (line == "&SR13Max X Y Z") {
            getline(file, line); parseLine(line, SR13Max);
        }
        else if (line == "&SR13Min X Y Z") {
            getline(file, line); parseLine(line, SR13Min);
        }

        else if (line == "&SR23Max X Y Z") {
            getline(file, line); parseLine(line, SR23Max);
        }
        else if (line == "&SR23Min X Y Z") {
            getline(file, line); parseLine(line, SR23Min);
        }

        else if (line == "&SS11Max X Y Z") {
            getline(file, line); parseLine(line, SS11Max);
        }
        else if (line == "&SS11Min X Y Z") {
            getline(file, line); parseLine(line, SS11Min);
        }

        else if (line == "&SS22Max X Y Z") {
            getline(file, line); parseLine(line, SS22Max);
        }
        else if (line == "&SS22Min X Y Z") {
            getline(file, line); parseLine(line, SS22Min);
        }

        else if (line == "&SS33Max X Y Z") {
            getline(file, line); parseLine(line, SS33Max);
        }
        else if (line == "&SS33Min X Y Z") {
            getline(file, line); parseLine(line, SS33Min);
        }

        else if (line == "&SS12Max X Y Z") {
            getline(file, line); parseLine(line, SS12Max);
        }
        else if (line == "&SS12Min X Y Z") {
            getline(file, line); parseLine(line, SS12Min);
        }

        else if (line == "&SS13Max X Y Z") {
            getline(file, line); parseLine(line, SS13Max);
        }
        else if (line == "&SS13Min X Y Z") {
            getline(file, line); parseLine(line, SS13Min);
        }

        else if (line == "&SS23Max X Y Z") {
            getline(file, line); parseLine(line, SS23Max);
        }
        else if (line == "&SS23Min X Y Z") {
            getline(file, line); parseLine(line, SS23Min);
        }
        // [新增] 温度解析逻辑
        else if (line == "&TemperatureMax X Y Z") {
            getline(file, line); parseLine(line, TemperatureMax);
        }
        else if (line == "&TemperatureMin X Y Z") {
            getline(file, line); parseLine(line, TemperatureMin);
        }
    }

    // --------------------------
    // 渲染 max/min
    // --------------------------
    auto R = [&](DataSet& mn, DataSet& mx) {
        OnMinMax(mn.x, mn.y, mn.z, mx.x, mx.y, mx.z,
            mn.value, mx.value, 100, renderer, pEntry);
        };

    if (type == "Vomises") R(VomisesMin, VomisesMax);
    else if (type == "DisX") R(DisXMin, DisXMax);
    else if (type == "DisY") R(DisYMin, DisYMax);
    else if (type == "DisZ") R(DisZMin, DisZMax);
    else if (type == "DisTotal") R(DisTotalMin, DisTotalMax);

    else if (type == "SR11") R(SR11Min, SR11Max);
    else if (type == "SR22") R(SR22Min, SR22Max);
    else if (type == "SR33") R(SR33Min, SR33Max);
    else if (type == "SR12") R(SR12Min, SR12Max);
    else if (type == "SR13") R(SR13Min, SR13Max);
    else if (type == "SR23") R(SR23Min, SR23Max);

    else if (type == "SS11") R(SS11Min, SS11Max);
    else if (type == "SS22") R(SS22Min, SS22Max);
    else if (type == "SS33") R(SS33Min, SS33Max);
    else if (type == "SS12") R(SS12Min, SS12Max);
    else if (type == "SS13") R(SS13Min, SS13Max);
    else if (type == "SS23") R(SS23Min, SS23Max);

    // [新增] 温度类型判断
    else if (type == "Temperature") R(TemperatureMin, TemperatureMax);
}
JS_EXT_FUNC_END()

#define LOADFILES_FUNC_USAGE                                                   \
        "loadFiles1 Usage: "                                                   \
        "comx.ply.loadFiles1(IVtkContext *pVtkContext, workData *work, "       \
        "string path1, string path2);"
JS_EXT_FUNC_BEGIN(loadFiles1, 4, LOADFILES_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string path1 = JS_EXT_PARA(string, 2);
        string path2 = JS_EXT_PARA(string, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // pEntry->newPath.clear();
        // cout << "path2: " << path2 << endl;
        // getFilesClassified(path2, pEntry->newPath);

        pEntry->path.clear();
        // pEntry->motiActor.clear();

        vector<string> files;

        getFiles(path2, files);

        int size = files.size();
        for (int i = 0; i < size; i++) {
                for (size_t j = 0; j < files[i].length(); ++j) {
                        if (files[i][j] == '\\') {
                                files[i].replace(j, 1, "/");
                        }
                }

                size_t filename_start = files[i].rfind('/') + 1;
                std::string filename = files[i].substr(filename_start);

                std::regex prefix_pattern("^postprocess");
                std::smatch prefix_matches;
                if (!std::regex_search(filename, prefix_matches,
                                       prefix_pattern)) {
                        continue;
                        // std::cout << "前缀部分: " << prefix_matches[0] <<
                        // std::endl;
                }
                // 识别 ".vts"
                std::regex suffix_pattern("\.vts$");
                std::smatch suffix_matches;
                if (!std::regex_search(filename, suffix_matches,
                                       suffix_pattern)) {
                        continue;
                        // std::cout << "文件后缀是:.vts" << std::endl;
                }
                cout << files[i].c_str() << endl;
                pEntry->path.push_back(files[i].c_str());
                // OnModelVTKLoad(path1, files[i].c_str(), renderer, pEntry);
        }

        int res = type_cast<int>(pEntry->path.size());
        // int res = type_cast<int>(pEntry->newPath.size());
        for (int i = 0; i < pEntry->path.size(); i++) {
                cout << pEntry->path[i] << endl;
        }
        cout << "motai file size: " << res << "\n";
        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define LOADFILES_FUNC_USAGE                                                   \
        "loadFiles Usage: "                                                    \
        "comx.ply.loadFiles(IVtkContext *pVtkContext, workData *work, "        \
        "string path1, string path2);"
JS_EXT_FUNC_BEGIN(loadFiles, 4, LOADFILES_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string path1 = JS_EXT_PARA(string, 2);
        string path2 = JS_EXT_PARA(string, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        pEntry->path.clear();
        // pEntry->motiActor.clear();

        vector<string> files;
        getFiles(path2, files);

        int size = files.size();
        for (int i = 0; i < size; i++) {
                for (size_t j = 0; j < files[i].length(); ++j) {
                        if (files[i][j] == '\\') {
                                files[i].replace(j, 1, "/");
                        }
                }

                size_t filename_start = files[i].rfind('/') + 1;
                std::string filename = files[i].substr(filename_start);

                std::regex prefix_pattern("\\d+postprocess");
                std::smatch prefix_matches;
                if (!std::regex_search(filename, prefix_matches,
                                       prefix_pattern)) {
                        continue;
                        // std::cout << "前缀部分: " << prefix_matches[0] <<
                        // std::endl;
                }
                // 识别 ".vts"
                std::regex suffix_pattern("\.vts$");
                std::smatch suffix_matches;
                if (!std::regex_search(filename, suffix_matches,
                                       suffix_pattern)) {
                        continue;
                        // std::cout << "文件后缀是:.vts" << std::endl;
                }
                cout << files[i].c_str() << endl;
                pEntry->path.push_back(files[i].c_str());
                // OnModelVTKLoad(path1, files[i].c_str(), renderer, pEntry);
        }

        int res = type_cast<int>(pEntry->path.size());

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define DELETEALLFILE_FUNC_USAGE                                               \
        "deleteAllFile Usage: "                                                \
        "comx.ply.deleteAllFile(string path);"
JS_EXT_FUNC_BEGIN(deleteAllFile, 1, DELETEALLFILE_FUNC_USAGE) {
        // Put your codes here
        string path = JS_EXT_PARA(string, 0);
        deleteAllFiles(path);
}
JS_EXT_FUNC_END()

#define SSHMOTAI_FUNC_USAGE                                                    \
        "sshMoTais Usage: "                                                    \
        "comx.ply.sshMoTais(string path);"
JS_EXT_FUNC_BEGIN(sshMoTais, 7, SSHMOTAI_FUNC_USAGE) {
        // Put your codes here
        string stpPath = JS_EXT_PARA(string, 0);    // step文件路径
        string order = JS_EXT_PARA(string, 1);      // 阶数
        string fileName = extractFileName(stpPath); // step文件名
        string mat = JS_EXT_PARA(string, 2);        // 杨氏模量
        string poi = JS_EXT_PARA(string, 3);        // 泊松比
        string wei = JS_EXT_PARA(string, 4);        // 未知
        string nums = JS_EXT_PARA(string, 5);       // 数量
        string vtsPath = JS_EXT_PARA(string, 6);    // vts文件路径
        cout << stpPath << endl;
        cout << order << endl;
        cout << fileName << endl;
        MoTaiSSh(stpPath, order, fileName, mat, poi, wei, nums, vtsPath);
}
JS_EXT_FUNC_END()

#define GUDINGSSH_FUNC_USAGE                                                   \
        "GuDingSShs Usage: "                                                   \
        "comx.ply.GuDingSShs(string path);"
JS_EXT_FUNC_BEGIN(GuDingSShs, 7, GUDINGSSH_FUNC_USAGE) {
        // Put your codes here
        string stpPath = JS_EXT_PARA(string, 0);    // step文件路径
        string order = JS_EXT_PARA(string, 1);      // 阶数
        string fileName = extractFileName(stpPath); // step文件名
        string mat = JS_EXT_PARA(string, 2);        // 杨氏模量
        string poi = JS_EXT_PARA(string, 3);        // 泊松比
        string wei = JS_EXT_PARA(string, 4);        // 未知
        string nums = JS_EXT_PARA(string, 5);       // 数量
        string vtsPath = JS_EXT_PARA(string, 6);    // vts文件路径

        GuDingSSh(stpPath, order, fileName, mat, poi, wei, nums, vtsPath);
}
JS_EXT_FUNC_END()

#define POLYNOMIALS_FUNC_USAGE                                                 \
        "polynomials Usage: "                                                  \
        "comx.ply.polynomials(double size);"
JS_EXT_FUNC_BEGIN(polynomials, 1, POLYNOMIALS_FUNC_USAGE) {
        // Put your codes here
        double size = JS_EXT_PARA(double, 0);

        double res = polynomial(size);
        // renderer->AddActor(pEntry->motiActor[id]);

        // int res = type_cast<string>(res);

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define RENDERERMOTI_FUNC_USAGE                                                \
        "rendererMoTi Usage: "                                                 \
        "comx.ply.rendererMoTi(IVtkContext *pVtkContext, workData *work, "     \
        "string path, int id);"
JS_EXT_FUNC_BEGIN(rendererMoTi, 5, RENDERERMOTI_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string path = JS_EXT_PARA(string, 2);
        int id = JS_EXT_PARA(int, 3);
        // id = id - 1; // 索引从1开始，这里转换为0开始 newadd
        double size = JS_EXT_PARA(double, 4);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // 验证索引有效性
        if (id < 0 || id >= pEntry->path.size()) {
                cout << "[ERROR] 无效的newPath索引: " << id << " (范围: 0-"
                     << (pEntry->newPath.size() - 1) << ")" << endl;
                string errorMsg = "Error: 无效的文件组索引 " + to_string(id);
                JS_EXT_FUNC_ASSIGN_RET(errorMsg);
                return Napi::String::From(info.Env(), errorMsg);
        }

        // 构造STL文件路径（使用默认的STL文件）
        string pathSTL = path + "/data/locomotive_bogieassembly_sldasm2.stl";
        string res =
            OnModelVTKLoad(pathSTL, pEntry->path[id], size, renderer, pEntry);
        // string res = OnModelVTKLoadWithAnimation(path, pEntry->newPath[id],
        // size, 0, 500, true, renderer, pEntry); OnRenderers1(path,
        // pEntry->path[id], size, renderer, pEntry);

        cout << "[DEBUG] OnModelVTKLoadWithAnimation 返回: " << res << endl;

        // renderer->AddActor(pEntry->motiActor[id]);
        // int res = type_cast<string>(res);

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define GENERATEBREPS_FUNC_USAGE                                               \
        "generateBreps Usage: "                                                \
        "comx.ply.generateBreps(IVtkContext *pVtkContext, workData *work, "    \
        "int id, string path);"
JS_EXT_FUNC_BEGIN(generateBreps, 4, GENERATEBREPS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        string id = JS_EXT_PARA(string, 2);
        string path = JS_EXT_PARA(string, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // string res = type_cast<string>(generateBrep(id, path, pEntry));

        // JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define ONOPENMESH_FUNC_USAGE                                                  \
        "OnOpenMeshs Usage: "                                                  \
        "comx.ply.OnOpenMeshs();"
JS_EXT_FUNC_BEGIN(OnOpenMeshs, 12, ONOPENMESH_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x1 = JS_EXT_PARA(double, 2);
        double y1 = JS_EXT_PARA(double, 3);
        double z1 = JS_EXT_PARA(double, 4);

        double x2 = JS_EXT_PARA(double, 5);
        double y2 = JS_EXT_PARA(double, 6);
        double z2 = JS_EXT_PARA(double, 7);

        int numXSegments = JS_EXT_PARA(int, 8);
        int numYSegments = JS_EXT_PARA(int, 9);
        int numZSegments = JS_EXT_PARA(int, 10);

        bool isMesh = JS_EXT_PARA(bool, 11);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        Point3D startPoint;
        startPoint.x = x1;
        startPoint.y = y1;
        startPoint.z = z1;

        Point3D endPoint;
        endPoint.x = x2;
        endPoint.y = y2;
        endPoint.z = z2;

        if (isMesh) {
                OnOpenMesh(startPoint, endPoint, numXSegments, numYSegments,
                           numZSegments, renderer, pEntry);
        } else {
                OnCloseMesh(renderer, pEntry);
        }
}
JS_EXT_FUNC_END()

#define GETCURRENTACTORNAME_FUNC_USAGE                                         \
        "getCurrentActorName Usage: "                                          \
        "comx.ply.getCurrentActorName();"
JS_EXT_FUNC_BEGIN(getCurrentActorName, 2, GETCURRENTACTORNAME_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = "type:" + getCurrentActor()->GetObjectName();
        ret_val +=
            " center:" + std::to_string(getCurrentActor()->GetCenter()[0]) +
            " " + std::to_string(getCurrentActor()->GetCenter()[1]) + " " +
            std::to_string(getCurrentActor()->GetCenter()[2]);

        string res = type_cast<string>(ret_val);

        JS_EXT_FUNC_ASSIGN_RET(res);
}
JS_EXT_FUNC_END()

#define MOUSEPICK_FUNC_USAGE                                                   \
        "mousePick Usage: "                                                    \
        "comx.ply.mousePick();"
JS_EXT_FUNC_BEGIN(mousePick, 3, MOUSEPICK_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        string type = JS_EXT_PARA(string, 2);
        setTypes(type);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;
}
JS_EXT_FUNC_END()

#define CUTTERS_FUNC_USAGE                                                     \
        "cutters Usage: "                                                      \
        "comx.ply.cutters(vtkRenderer *renderer, postManage *work);"
JS_EXT_FUNC_BEGIN(cutters, 8, CUTTERS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double xn = JS_EXT_PARA(double, 5);
        double yn = JS_EXT_PARA(double, 6);
        double zn = JS_EXT_PARA(double, 7);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        cutter(x, y, z, xn, yn, zn, renderer, pEntry);
        // plyCutter(x, y, z, xn, yn, zn, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define COLORRGBS1_FUNC_USAGE                                                  \
        "colorRGBS Usage: "                                                    \
        "comx.ply.colorRGBS(vtkRenderer *renderer, postManage *work, double "  \
        "r1, double g1, double b1, double r2, double g2, double b2);"
JS_EXT_FUNC_BEGIN(colorRGBS1, 8, COLORRGBS1_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        double r1 = JS_EXT_PARA(int, 2);
        double g1 = JS_EXT_PARA(int, 3);
        double b1 = JS_EXT_PARA(int, 4);

        double r2 = JS_EXT_PARA(int, 5);
        double g2 = JS_EXT_PARA(int, 6);
        double b2 = JS_EXT_PARA(int, 7);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        RGBS c1 = {r1, g1, b1};
        RGBS c2 = {r2, g2, b2};
        SetColorMap1(c1, c2, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define COLORRGBS2_FUNC_USAGE                                                  \
        "colorRGBS2 Usage: "                                                   \
        "comx.ply.colorRGBS2(vtkRenderer *renderer, postManage *work, double " \
        "r1, double g1, double b1, double r2, double g2, double b2, double "   \
        "r3, double g3, double b3);"
JS_EXT_FUNC_BEGIN(colorRGBS2, 11, COLORRGBS2_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        double r1 = JS_EXT_PARA(int, 2);
        double g1 = JS_EXT_PARA(int, 3);
        double b1 = JS_EXT_PARA(int, 4);

        double r2 = JS_EXT_PARA(int, 5);
        double g2 = JS_EXT_PARA(int, 6);
        double b2 = JS_EXT_PARA(int, 7);

        double r3 = JS_EXT_PARA(int, 8);
        double g3 = JS_EXT_PARA(int, 9);
        double b3 = JS_EXT_PARA(int, 10);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        RGBS c1 = {r1, g1, b1};
        RGBS c2 = {r2, g2, b2};
        RGBS c3 = {r3, g3, b3};
        SetColorMap2(c1, c2, c3, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define COLORRGBS3_FUNC_USAGE                                                  \
        "colorRGBS3 Usage: "                                                   \
        "comx.ply.colorRGBS3(vtkRenderer *renderer, postManage *work, double " \
        "r1, double g1, double b1, double r2, double g2, double b2, double "   \
        "r3, double g3, double b3, double r4, double g4, double b4);"
JS_EXT_FUNC_BEGIN(colorRGBS3, 14, COLORRGBS3_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        double r1 = JS_EXT_PARA(int, 2);
        double g1 = JS_EXT_PARA(int, 3);
        double b1 = JS_EXT_PARA(int, 4);

        double r2 = JS_EXT_PARA(int, 5);
        double g2 = JS_EXT_PARA(int, 6);
        double b2 = JS_EXT_PARA(int, 7);

        double r3 = JS_EXT_PARA(int, 8);
        double g3 = JS_EXT_PARA(int, 9);
        double b3 = JS_EXT_PARA(int, 10);

        double r4 = JS_EXT_PARA(int, 11);
        double g4 = JS_EXT_PARA(int, 12);
        double b4 = JS_EXT_PARA(int, 13);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        RGBS c1 = {r1, g1, b1};
        RGBS c2 = {r2, g2, b2};
        RGBS c3 = {r3, g3, b3};
        RGBS c4 = {r4, g4, b4};
        SetColorMap3(c1, c2, c3, c4, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define COLORRGBS4_FUNC_USAGE                                                  \
        "colorRGBS4 Usage: "                                                   \
        "comx.ply.colorRGBS4(vtkRenderer *renderer, postManage *work, double " \
        "r1, double g1, double b1, double r2, double g2, double b2, double "   \
        "r3, double g3, double b3, double r4, double g4, double b4, double "   \
        "r5, double g5, double b5);"
JS_EXT_FUNC_BEGIN(colorRGBS4, 17, COLORRGBS4_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        double r1 = JS_EXT_PARA(int, 2);
        double g1 = JS_EXT_PARA(int, 3);
        double b1 = JS_EXT_PARA(int, 4);

        double r2 = JS_EXT_PARA(int, 5);
        double g2 = JS_EXT_PARA(int, 6);
        double b2 = JS_EXT_PARA(int, 7);

        double r3 = JS_EXT_PARA(int, 8);
        double g3 = JS_EXT_PARA(int, 9);
        double b3 = JS_EXT_PARA(int, 10);

        double r4 = JS_EXT_PARA(int, 11);
        double g4 = JS_EXT_PARA(int, 12);
        double b4 = JS_EXT_PARA(int, 13);

        double r5 = JS_EXT_PARA(int, 14);
        double g5 = JS_EXT_PARA(int, 15);
        double b5 = JS_EXT_PARA(int, 16);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        RGBS c1 = {r1, g1, b1};
        RGBS c2 = {r2, g2, b2};
        RGBS c3 = {r3, g3, b3};
        RGBS c4 = {r4, g4, b4};
        RGBS c5 = {r5, g5, b5};
        SetColorMap4(c1, c2, c3, c4, c5, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define SETCHANGEDATA_FUNC_USAGE                                               \
        "setChangeData Usage: "                                                \
        "comx.ply.setPostChange(vtkRenderer *renderer, postManage *work, int " \
        "attribute);"
JS_EXT_FUNC_BEGIN(setChangeData, 3, SETCHANGEDATA_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        int attribute = JS_EXT_PARA(int, 2);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        setPostChange(attribute, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define SETPOSTBARS_FUNC_USAGE                                                 \
        "setPostBar Usage: "                                                   \
        "comx.ply.setPostBar(vtkRenderer *renderer, workData *work, int "      \
        "attribute, int isBar);"
JS_EXT_FUNC_BEGIN(setPostBars, 4, SETPOSTBARS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        int attribute = JS_EXT_PARA(int, 2);
        int isBar = JS_EXT_PARA(int, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);
        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        if (isBar == 1) {
                setPlyPostBar(attribute, renderer, pEntry);
                // setPostBar(attribute, renderer, pEntry);
        } else {
                closePostBar(renderer, pEntry);
        }

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define RENDERER_FUNC_USAGE                                                    \
        "renderer Usage: "                                                     \
        "comx.ply.renderer(vtkRenderer *renderer, workData *work, string "     \
        "path, double rate);"
JS_EXT_FUNC_BEGIN(renderer, 4, RENDERER_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        string path = JS_EXT_PARA(string, 2);
        double rate = JS_EXT_PARA(double, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        OnRenderers(path, rate, renderer, pEntry);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define OPENCLOSEOPACITY_FUNC_USAGE                                            \
        "openCloseOpacity Usage: "                                             \
        "comx.ply.openCloseOpacity(vtkRenderer *renderer, workData *work, "    \
        "bool isOpacity);"
JS_EXT_FUNC_BEGIN(openCloseOpacity, 3, OPENCLOSEOPACITY_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        postManage *pEntry = (postManage *)((void *)ullWorkData);

        bool isOpacity = JS_EXT_PARA(bool, 2);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        if (isOpacity) {
                pEntry->actor->GetProperty()->SetOpacity(0.2);
        } else {
                pEntry->actor->GetProperty()->SetOpacity(1);
        }

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define MODIFYMARKS_FUNC_USAGE                                                 \
        "modifyMarks Usage: "                                                  \
        "comx.ply.modifyMarks(vtkRenderer *renderer, workData *work, double "  \
        "x, double y, double z, double r, double g, double b, double size, "   \
        "string mark, string name);"
JS_EXT_FUNC_BEGIN(modifyMarks, 11, MODIFYMARKS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double r = JS_EXT_PARA(double, 5);
        double g = JS_EXT_PARA(double, 6);
        double b = JS_EXT_PARA(double, 7);

        double size = JS_EXT_PARA(double, 8);
        string name = JS_EXT_PARA(string, 9);
        string mark = JS_EXT_PARA(string, 10);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        motifyMark(x, y, z, r, g, b, size, name, mark, renderer, pEntry);

        // JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SERMARKS_FUNC_USAGE                                                    \
        "setMarks Usage: "                                                     \
        "comx.ply.setMarks(vtkRenderer *renderer, workData *work, double "     \
        "x, double y, double z, double r, double g, double b, double size, "   \
        "string mark, string name);"
JS_EXT_FUNC_BEGIN(setMarks, 11, SERMARKS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double r = JS_EXT_PARA(double, 5);
        double g = JS_EXT_PARA(double, 6);
        double b = JS_EXT_PARA(double, 7);

        double size = JS_EXT_PARA(double, 8);
        string mark = JS_EXT_PARA(string, 9);
        string name = JS_EXT_PARA(string, 10);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;
        setMark(x, y, z, r, g, b, size, mark, name, renderer, pEntry);
        // JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYSPHERES_FUNC_USAGE                                               \
        "modifySpheres Usage: "                                                \
        "comx.ply.modifySpheres(vtkRenderer *renderer, workData *work, "       \
        "double "                                                              \
        "x, double y, double z, double radius, string name);"
JS_EXT_FUNC_BEGIN(modifySpheres, 7, MODIFYSPHERES_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double radius = JS_EXT_PARA(double, 5);
        string name = JS_EXT_PARA(string, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            modifySphere(x, y, z, radius, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SERSPHERES_FUNC_USAGE                                                  \
        "setSpheres Usage: "                                                   \
        "comx.ply.setSpheres(vtkRenderer *renderer, workData *work, double "   \
        "x, double y, double z, double radius, string name);"
JS_EXT_FUNC_BEGIN(setSpheres, 7, SERSPHERES_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double radius = JS_EXT_PARA(double, 5);
        string name = JS_EXT_PARA(string, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            setSphere(x, y, z, radius, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYCONNECTS_FUNC_USAGE                                              \
        "modifyConnects Usage: "                                               \
        "comx.ply.modifyConnects(vtkRenderer *renderer, workData *work, "      \
        "double "                                                              \
        "x, double y, double z, double X, double "                             \
        "Y, double Z, double r, double g, "                                    \
        "double b);"
JS_EXT_FUNC_BEGIN(modifyConnects, 12, MODIFYCONNECTS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double X = JS_EXT_PARA(double, 5);
        double Y = JS_EXT_PARA(double, 6);
        double Z = JS_EXT_PARA(double, 7);

        double r = JS_EXT_PARA(double, 8);
        double g = JS_EXT_PARA(double, 9);
        double b = JS_EXT_PARA(double, 10);

        string name = JS_EXT_PARA(string, 11);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            modifyConnect(x, y, z, X, Y, Z, r, g, b, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SERCONNECTS_FUNC_USAGE                                                 \
        "setSpring Usage: "                                                    \
        "comx.ply.setConnects(vtkRenderer *renderer, workData *work, double "  \
        "x, double y, double z, double X, double "                             \
        "Y, double Z, double r, double g, "                                    \
        "double b);"
JS_EXT_FUNC_BEGIN(setConnects, 12, SERCONNECTS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double X = JS_EXT_PARA(double, 5);
        double Y = JS_EXT_PARA(double, 6);
        double Z = JS_EXT_PARA(double, 7);

        double r = JS_EXT_PARA(double, 8);
        double g = JS_EXT_PARA(double, 9);
        double b = JS_EXT_PARA(double, 10);

        string name = JS_EXT_PARA(string, 11);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            setConnect(x, y, z, X, Y, Z, r, g, b, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYSPRINGS_FUNC_USAGE                                               \
        "modifySprings Usage: "                                                \
        "comx.ply.modifySprings(vtkRenderer *renderer, workData *work, "       \
        "double "                                                              \
        "x, double y, double z, double X, double "                             \
        "Y, double Z, double r, double g, double b, string name);"
JS_EXT_FUNC_BEGIN(modifySprings, 12, MODIFYSPRINGS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double X = JS_EXT_PARA(double, 5);
        double Y = JS_EXT_PARA(double, 6);
        double Z = JS_EXT_PARA(double, 7);

        double r = JS_EXT_PARA(double, 8);
        double g = JS_EXT_PARA(double, 9);
        double b = JS_EXT_PARA(double, 10);

        string name = JS_EXT_PARA(string, 11);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            modifySpring(x, y, z, X, Y, Z, r, g, b, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define SETSPRINGS_FUNC_USAGE                                                  \
        "setSpring Usage: "                                                    \
        "comx.ply.setSprings(vtkRenderer *renderer, workData *work, double "   \
        "x, double y, double z, double X, double "                             \
        "Y, double Z, double r, double g, double b, string name);"
JS_EXT_FUNC_BEGIN(setSprings, 12, SETSPRINGS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double X = JS_EXT_PARA(double, 5);
        double Y = JS_EXT_PARA(double, 6);
        double Z = JS_EXT_PARA(double, 7);

        double r = JS_EXT_PARA(double, 8);
        double g = JS_EXT_PARA(double, 9);
        double b = JS_EXT_PARA(double, 10);

        string name = JS_EXT_PARA(string, 11);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            setspring(x, y, z, X, Y, Z, r, g, b, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()


void SetAllMapActorsColor(
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>>& ConstrainsMap,
    double r, double g, double b)
{
    for (auto& kv : ConstrainsMap) {
        auto& actors = kv.second;  // vector<vtkSmartPointer<vtkActor>>

        for (auto& actor : actors) {
            if (actor) {
                actor->GetProperty()->SetColor(r, g, b);
            }
        }
    }
}

void SetActorsColor(std::vector<vtkSmartPointer<vtkActor>>& actors,
    double r, double g, double b)
{
    for (auto& actor : actors) {
        if (actor) {
            actor->GetProperty()->SetColor(r, g, b);
        }
    }
}


void AddActorsToRenderer(
    vtkRenderer* renderer,
    const std::vector<vtkSmartPointer<vtkActor>>& actors)
{
    if (!renderer) return;

    for (auto& actor : actors) {
        if (actor) {
            renderer->AddActor(actor);
        }
    }
}


#define PICKUP_FUNC_USAGE                                                      \
        "setArrows Usage: "                                                    \
        "comx.ply.pickup(vtkRenderer *renderer, workData *work, string "       \
        "types, int index);"
JS_EXT_FUNC_BEGIN(pickup, 4, PICKUP_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        string typess = JS_EXT_PARA(string, 2);
        string name = JS_EXT_PARA(string, 3);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        if (typess == "points") {

                for (auto it = pEntry->Points.begin();
                     it != pEntry->Points.end(); it++) {
                        it->second->GetProperty()->SetColor(0, 1, 0);
                }

                pEntry->Points[name]->GetProperty()->SetColor(1, 0, 0);
        }

        if (typess == "arrows") {

                /*for (auto it = pEntry->Arrows.begin();
                     it != pEntry->Arrows.end(); it++) {
                        it->second->GetProperty()->SetColor(0, 1, 0);
                }*/

                // pEntry->Arrows[name]->GetProperty()->SetColor(1, 0, 0);

                // 遍历每个键，并遍历其对应的所有箭头
                for (auto it = pEntry->Arrows1.begin();
                     it != pEntry->Arrows1.end(); ++it) {
                        // 遍历每个箭头
                        for (auto &actor : it->second) {
                                // 设置箭头的颜色为绿色
                                actor->GetProperty()->SetColor(0, 1, 0.3);
                        }
                }
                if (pEntry->Arrows1.find(name) != pEntry->Arrows1.end()) {
                        for (auto &actor : pEntry->Arrows1[name]) {
                                actor->GetProperty()->SetColor(
                                    1, 0, 0); // 设置颜色为红色
                        }
                }
        }

        if (typess == "constraints") {


                SetAllMapActorsColor(pEntry->ConstrainsRotationMap, 1, 0.5, 0);
                SetAllMapActorsColor(pEntry->ConstrainsTranslationMap, 0, 1, 0.3);

                if (pEntry->ConstrainsRotationMap.find(name) != pEntry->ConstrainsRotationMap.end())
                {
                        SetActorsColor(pEntry->ConstrainsRotationMap[name], 1, 0, 0);
                }

                if (pEntry->ConstrainsTranslationMap.find(name) != pEntry->ConstrainsTranslationMap.end())
                {
                        SetActorsColor(pEntry->ConstrainsTranslationMap[name], 1, 0, 0);
                }
                
        }

        // renderer->AddActor(pEntry->Points[0]);
        renderer->Render();

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define CHANGECOLOR_FUNC_USAGE                                                 \
        "changeColor Usage: "                                                  \
        "comx.ply.changeColor(vtkRenderer *renderer, workData *work, string "  \
        "types, string name, double r, double g, double b);"
JS_EXT_FUNC_BEGIN(changeColor, 7, CHANGECOLOR_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        string typess = JS_EXT_PARA(string, 2);
        string name = JS_EXT_PARA(string, 3);

        double r = JS_EXT_PARA(double, 4);
        double g = JS_EXT_PARA(double, 5);
        double b = JS_EXT_PARA(double, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        if (typess == "points") {
                cout << "changecolor:point" << endl;
                pEntry->Points[name]->GetProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->Points[name]);
        }

        if (typess == "arrows") {

                // pEntry->Arrows[name]->GetProperty()->SetColor(r, g, b);

                pEntry->TitleArrows[name]->GetTextProperty()->SetColor(r, g, b);

                // renderer->AddActor(pEntry->Arrows[name]);

                renderer->AddActor(pEntry->TitleArrows[name]);

                if (pEntry->Arrows1.find(name) != pEntry->Arrows1.end()) {
                        for (auto &actor : pEntry->Arrows1[name]) {
                                actor->GetProperty()->SetColor(r, g, b);
                                renderer->AddActor(actor);
                        }
                }
        }

        if (typess == "constraints") {
                cout << "changecolor:constraints" << endl;
                /*pEntry->Constrains[name]->GetProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->Constrains[name]);*/


                if (pEntry->ConstrainsRotationMap.find(name) != pEntry->ConstrainsRotationMap.end())
                {
                    SetActorsColor(pEntry->ConstrainsRotationMap[name], r, g, b);
                    AddActorsToRenderer(renderer, pEntry->ConstrainsRotationMap[name]);

                }

                if (pEntry->ConstrainsTranslationMap.find(name) != pEntry->ConstrainsTranslationMap.end())
                {
                    SetActorsColor(pEntry->ConstrainsTranslationMap[name], r, g, b);
                    AddActorsToRenderer(renderer, pEntry->ConstrainsTranslationMap[name]);
                }
        }

        if (typess == "beams") {

                pEntry->Beams[name].line[0]->GetProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->Beams[name].line[0]);
        }

        if (typess == "spring") {
                pEntry->Spring[name]->GetProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->Spring[name]);
        }

        if (typess == "connect") {
                pEntry->Connects[name]->GetProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->Connects[name]);
        }

        if (typess == "rb3") {
                pEntry->Rb3[name]->GetProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->Rb3[name]);
        }

        if (typess == "mark") {
                pEntry->mark[name]->GetTextProperty()->SetColor(r, g, b);
                renderer->AddActor(pEntry->mark[name]);
        }

        // renderer->AddActor(pEntry->Points[0]);
        // renderer->Render();

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define HIDESHOW_FUNC_USAGE \
        "hideShow Usage: " \
        "comx.ply.hideShow(vtkRenderer *renderer, workData *work, string " \
        "types, string name, bool isVisible);"
JS_EXT_FUNC_BEGIN(hideShow, 5, HIDESHOW_FUNC_USAGE) {

    unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
    IVtkContext* pVtkContext = (IVtkContext*)((void*)ullVtk);
    if (!pVtkContext) {
        cout << "Error: pVtkContext is NULL\n";
        return Napi::Boolean::From(info.Env(), false);
    }

    unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
    workData* pEntry = (workData*)((void*)ullWorkData);
    if (!pEntry) {
        cout << "Error: workData pointer is NULL\n";
        return Napi::Boolean::From(info.Env(), false);
    }

    setWorkData(pEntry);

    string typess = JS_EXT_PARA(string, 2);
    string name = JS_EXT_PARA(string, 3);
    bool isVisible = JS_EXT_PARA(bool, 4);

    if (!_init_callback) {
        _init_callback = true;
        pVtkContext->RegisterMouseListenCallback(callbackMouse);
    }

    void* pRawRenderer = NULL;
    pVtkContext->GetRenderer(pRawRenderer);

    vtkRenderer* renderer = (vtkRenderer*)pRawRenderer;
    if (!renderer) {
        cout << "Error: renderer is NULL\n";
        return Napi::Boolean::From(info.Env(), false);
    }

    int isVis = isVisible ? 0 : 1; // 注意：VTK SetVisibility(1)是显示，0是隐藏

    // 辅助 lambda：用于处理简单的单 Actor Map (如 Points, Spring 等)
    auto safeSet = [&](auto& mapObj, const string& key) -> bool {
        if (!mapObj.count(key)) {
            cout << "Error: key not found -> " << key << "\n";
            return false;
        }
        auto actor = mapObj[key];
        if (!actor) {
            cout << "Error: actor is NULL for key -> " << key
                << "\n";
            return false;
        }
        actor->SetVisibility(isVis);
        // 确保 actor 在渲染器中（如果之前被移除过，这里加回来比较保险，或者依赖外部逻辑）
        // renderer->AddActor(actor); 
        return true;
        };

    // 辅助 lambda：用于处理 vector<Actor> 类型的 Map (批量隐藏/显示)
    auto safeSetVectorMap = [&](auto& mapObj, const string& key) -> bool {
        if (!mapObj.count(key)) {
            cout << "Error: key not found in VectorMap -> " << key << "\n";
            return false;
        }
        for (auto actor : mapObj[key]) {
            if (actor) {
                actor->SetVisibility(isVis);
                // renderer->AddActor(actor); // 可选：视具体逻辑决定是否需要重新Add
            }
        }
        return true;
        };

    // 辅助 lambda：用于处理 Assembly 类型的 Map
    auto safeSetAssemblyMap = [&](auto& mapObj, const string& key) -> bool {
        if (!mapObj.count(key)) {
            // Assembly 没找到未必是错误，可能只存在散件
            return false;
        }
        auto assembly = mapObj[key];
        if (assembly) {
            assembly->SetVisibility(isVis);
            // renderer->AddActor(assembly);
            return true;
        }
        return false;
        };


    bool ok = true;

    if (typess == "points")
        ok = safeSet(pEntry->Points, name);
    if (typess == "spring")
        ok = safeSet(pEntry->Spring, name);
    if (typess == "connect")
        ok = safeSet(pEntry->Connects, name);
    if (typess == "rb3")
        ok = safeSet(pEntry->Rb3, name);
    if (typess == "mark")
        ok = safeSet(pEntry->mark, name);
    if (typess == "rb3Point")
        ok = safeSet(pEntry->pointActor, name);

    // ---- 特殊逻辑 ----
    if (typess == "pick") {
        if (!pEntry->actor->GetMapper()) {
            cout << "Error: pick actor is NULL\n";
            return Napi::Boolean::From(info.Env(), false);
        }
        pEntry->actor->SetVisibility(isVis);
        renderer->AddActor(pEntry->actor);
    }

    if (typess == "arrows") {
        // 处理 Title
        if (pEntry->TitleArrows.count(name)) {
            pEntry->TitleArrows[name]->SetVisibility(isVis);
        }

        // 处理 Assembly (优先)
        if (pEntry->ArrowAssemblies.count(name)) {
            pEntry->ArrowAssemblies[name]->SetVisibility(isVis);
        }

        // 处理散件 (如果没有 Assembly 或者为了双重保险)
        safeSetVectorMap(pEntry->Arrows1, name);
    }

    if (typess == "constraints") {
        // 处理 Assembly
        if (pEntry->ConstrainsAssemblies.count(name)) {
            pEntry->ConstrainsAssemblies[name]->SetVisibility(isVis);
        }

        // 处理散件
        safeSetVectorMap(pEntry->ConstrainsTranslationMap, name);
        safeSetVectorMap(pEntry->ConstrainsRotationMap, name);
    }

    // [新增] 热约束 (Thermal Constraints)
    if (typess == "thermalConstraints") {
        // 尝试隐藏 Assembly
        if (pEntry->ThermalConstrainsAssemblies.count(name)) {
            pEntry->ThermalConstrainsAssemblies[name]->SetVisibility(isVis);
        }

        safeSetVectorMap(pEntry->ThermalConstrainsMap, name);
    }

    // [新增] 对流换热 (Convections)
    if (typess == "convections") {
        if (pEntry->ConvectionsAssemblies.count(name)) {
            pEntry->ConvectionsAssemblies[name]->SetVisibility(isVis);
        }

        safeSetVectorMap(pEntry->ConvectionsMap, name);
    }

    // [新增] 表面热流 (Heat Fluxes)
    if (typess == "heatFluxes") {
        if (pEntry->HeatFluxesAssemblies.count(name)) {
            pEntry->HeatFluxesAssemblies[name]->SetVisibility(isVis);
        }

        safeSetVectorMap(pEntry->HeatFluxesMap, name);
    }

    if (typess == "beams") {
        if (!pEntry->Beams.count(name)) {
            cout << "Error: Beams not found -> " << name << "\n";
            ok = false;
        }
        else {
            auto& beam = pEntry->Beams[name];

            if (beam.line[0]) {
                beam.line[0]->SetVisibility(isVis);
            }

            for (auto pt : beam.point) {
                if (pt) {
                    pt->SetVisibility(isVis);
                }
            }
        }
    }

    if (typess == "all") {
        renderer->RemoveAllViewProps();
    }

    // 强制刷新渲染器以应用可见性更改
    // renderer->Render(); 

    // -----------------------
    //       返回结果
    // -----------------------
    if (!ok) {
        return Napi::Boolean::From(info.Env(), false);
    }

    return Napi::String::From(info.Env(), "ok");
}
JS_EXT_FUNC_END()

#define GETCURRENTNAME_FUNC_USAGE                                              \
        "getCurrentName Usage: "                                               \
        "comx.ply.getCurrentName(vtkRenderer *renderer, workData *work, "      \
        "string "                                                              \
        "types);"
JS_EXT_FUNC_BEGIN(getCurrentName, 3, GETCURRENTNAME_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        string typess = JS_EXT_PARA(string, 2);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;
        string ret_val = "";
        /*
        if (typess == "points") {

                if (pEntry->Points.size() == 0) {
                        return Napi::Boolean::From(info.Env(), false);
                }
                int index = pEntry->Points.size() - 1;
        }

        if (typess == "arrows") {
                if (pEntry->Arrows.size() == 0) {
                        return Napi::Boolean::From(info.Env(), false);
                }
                int index = pEntry->Arrows.size() - 1;
                // cout << pEntry->Arrows[index]->GetObjectName() << endl;

                ret_val =
                    type_cast<string>(pEntry->Arrows[index]->GetObjectName());
        }

        if (typess == "constraints") {
                if (pEntry->Constrains.size() == 0) {
                        return Napi::Boolean::From(info.Env(), false);
                }
                int index = pEntry->Constrains.size() - 1;
                ret_val = type_cast<string>(
                    pEntry->Constrains[index]->GetObjectName());
        }
        */
        renderer->Render();

        // return Napi::Boolean::From(info.Env(), false);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYBEAMS_FUNC_USAGE                                                 \
        "modifyBeams Usage: "                                                  \
        "comx.ply.modifyBeams(vtkRenderer *renderer,workData *work,double "    \
        "seg, double x1, double y1, "                                          \
        "double z1, double x2,double y2, double z2, double r,double g, "       \
        "double b, double radius, string name);"
JS_EXT_FUNC_BEGIN(modifyBeams, 14, MODIFYBEAMS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double seg = JS_EXT_PARA(double, 2);

        double x1 = JS_EXT_PARA(double, 3);
        double y1 = JS_EXT_PARA(double, 4);
        double z1 = JS_EXT_PARA(double, 5);

        double x2 = JS_EXT_PARA(double, 6);
        double y2 = JS_EXT_PARA(double, 7);
        double z2 = JS_EXT_PARA(double, 8);

        double r = JS_EXT_PARA(double, 9);
        double g = JS_EXT_PARA(double, 10);
        double b = JS_EXT_PARA(double, 11);

        double radius = JS_EXT_PARA(double, 12);
        string name = JS_EXT_PARA(string, 13);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val =
            type_cast<string>(modifyBeam(seg, x1, y1, z1, x2, y2, z2, r, g, b,
                                         radius, name, renderer, pEntry));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define SETBEAMS_FUNC_USAGE                                                    \
        "setBeams Usage: "                                                     \
        "comx.ply.setBeams(vtkRenderer *renderer,workData *work,double "       \
        "seg, double x1, double y1, "                                          \
        "double z1, double x2,double y2, double z2, double r,double g, "       \
        "double b, double radius, string name);"
JS_EXT_FUNC_BEGIN(setBeams, 14, SETBEAMS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double seg = JS_EXT_PARA(double, 2);

        double x1 = JS_EXT_PARA(double, 3);
        double y1 = JS_EXT_PARA(double, 4);
        double z1 = JS_EXT_PARA(double, 5);

        double x2 = JS_EXT_PARA(double, 6);
        double y2 = JS_EXT_PARA(double, 7);
        double z2 = JS_EXT_PARA(double, 8);

        double r = JS_EXT_PARA(double, 9);
        double g = JS_EXT_PARA(double, 10);
        double b = JS_EXT_PARA(double, 11);

        double radius = JS_EXT_PARA(double, 12);
        string name = JS_EXT_PARA(string, 13);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val =
            type_cast<string>(setBeam(seg, x1, y1, z1, x2, y2, z2, r, g, b,
                                      radius, name, renderer, pEntry));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define MODIFYCONSTRAINTS_FUNC_USAGE                                           \
        "modifyConstraints Usage: "                                            \
        "comx.ply.modifyConstraints(vtkRenderer *renderer, workData *work, "   \
        "double x, "                                                           \
        "double y, double z, string name);"
JS_EXT_FUNC_BEGIN(modifyConstraints, 6, MODIFYCONSTRAINTS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        string name = JS_EXT_PARA(string, 5);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // return Napi::Boolean::From(info.Env(), false);

        string ret_val = type_cast<string>(
            modifyConstraint(x, y, z, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define SETCONSTRAINTS_FUNC_USAGE                                              \
        "setConstraints Usage: "                                               \
        "comx.ply.setConstraints(vtkRenderer *renderer, workData *work, "      \
        "double x, "                                                           \
        "double y, double z, string name);"
JS_EXT_FUNC_BEGIN(setConstraints, 6, SETCONSTRAINTS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        string name = JS_EXT_PARA(string, 5);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // return Napi::Boolean::From(info.Env(), false);

        string ret_val =
            type_cast<string>(setConstraint(x, y, z, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define MODIFYARROWS_FUNC_USAGE                                                \
        "modifyArrows Usage: "                                                 \
        "comx.ply.modifyArrows(vtkRenderer *renderer, workData *work, double " \
        "x, "                                                                  \
        "double y, double z, double nx, double ny, double nz, string name);"
JS_EXT_FUNC_BEGIN(modifyArrows, 10, MODIFYARROWS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double nx = JS_EXT_PARA(double, 5);
        double ny = JS_EXT_PARA(double, 6);
        double nz = JS_EXT_PARA(double, 7);

        string name = JS_EXT_PARA(string, 8);
        string id = JS_EXT_PARA(string, 9);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            modifyArrow(x, y, z, nx, ny, nz, name, renderer, pEntry, id));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define SETARROWS_FUNC_USAGE                                                   \
        "setArrows Usage: "                                                    \
        "comx.ply.setArrows(vtkRenderer *renderer, workData *work, double x, " \
        "double y, double z, double nx, double ny, double nz, string name);"
JS_EXT_FUNC_BEGIN(setArrows, 11, SETARROWS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double nx = JS_EXT_PARA(double, 5);
        double ny = JS_EXT_PARA(double, 6);
        double nz = JS_EXT_PARA(double, 7);

        string name = JS_EXT_PARA(string, 8);
        int flag = JS_EXT_PARA(int, 9);
        string id = JS_EXT_PARA(string, 10);
        cout << "setArrows_id:" << id << endl;

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        // cout << 1 << endl;
        string ret_val = type_cast<string>(
            setArrow1(x, y, z, nx, ny, nz, name, renderer, pEntry, flag, id));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define MODIFYARROWS_FUNC_USAGE                                                \
        "modifyPoints Usage: "                                                 \
        "comx.ply.modifyPoints(vtkRenderer *renderer, workData *work, double " \
        "x, double y, double z, double radius, string name);"
JS_EXT_FUNC_BEGIN(modifyPoints, 7, MODIFYARROWS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double radius = JS_EXT_PARA(double, 5);
        string name = JS_EXT_PARA(string, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            modifyPoint(x, y, z, radius, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);

        // return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define SETPOINTS_FUNC_USAGE                                                   \
        "setPoints Usage: "                                                    \
        "comx.ply.setPoints(vtkRenderer *renderer, workData *work, double x, " \
        "double y, double z, double radius, string name);"
JS_EXT_FUNC_BEGIN(setPoints, 7, SETPOINTS_FUNC_USAGE) {
        // Put your codes here

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 0);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        unsigned long long ullWorkData = JS_EXT_PARA(unsigned long long, 1);
        workData *pEntry = (workData *)((void *)ullWorkData);
        setWorkData(pEntry);

        double x = JS_EXT_PARA(double, 2);
        double y = JS_EXT_PARA(double, 3);
        double z = JS_EXT_PARA(double, 4);

        double radius = JS_EXT_PARA(double, 5);
        string name = JS_EXT_PARA(string, 6);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        string ret_val = type_cast<string>(
            setPoint(x, y, z, radius, name, renderer, pEntry));

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define RENDERENTRYTOVTLBUFFER_FUNC_USAGE                                      \
        "RenderEntryToVtlBuffer Usage: "                                       \
        "comx.ply.RenderEntryToVtlBuffer(entry, infVtkContext);"
JS_EXT_FUNC_BEGIN(RenderEntryToVtlBuffer, 2,
                  RENDERENTRYTOVTLBUFFER_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullEntry = JS_EXT_PARA(unsigned long long, 0);
        IPlyEntry *pEntry = (IPlyEntry *)((void *)ullEntry);

        unsigned long long ullVtk = JS_EXT_PARA(unsigned long long, 1);
        IVtkContext *pVtkContext = (IVtkContext *)((void *)ullVtk);

        if (!_init_callback) {
                _init_callback = true;
                pVtkContext->RegisterMouseListenCallback(callbackMouse);
        }

        void *pRawRenderer = NULL;
        pVtkContext->GetRenderer(pRawRenderer);

        vtkRenderer *renderer = (vtkRenderer *)pRawRenderer;

        auto it = std::find(g_entries.begin(), g_entries.end(), pEntry);
        auto inf = *it;

        int index = -1;
        if (it != g_entries.end())
                index = std::distance(g_entries.begin(), it);

        // render codes

        if (index == -1)
                return Napi::Boolean::From(info.Env(), false);

        const float *points = NULL, *normals = NULL;
        const int *triangles = NULL;
        int nlen = 0, tlen = 0, nnlen = 0;

        inf->Points(points, nlen);
        inf->Normals(normals, nnlen);
        inf->Triangles(triangles, tlen);

        vector<double> pts, nors;
        copy(points, points + nlen * 3, inserter(pts, pts.end()));
        copy(normals, normals + nlen * 3, inserter(nors, nors.end()));

        // cout << "PLY vertex number is " << nlen << endl;

        vector<int> tris;
        copy(triangles, triangles + tlen * 3, inserter(tris, tris.end()));

        createAndDisplayPolyData(pts, nors, tris, renderer);

        return Napi::Boolean::From(info.Env(), false);
}
JS_EXT_FUNC_END()

#define RENDERALLTOGLBUFFER_FUNC_USAGE                                         \
        "RenderAllToGlBuffer Usage: comx.ply.RenderAllToGlBuffer(mid);"
JS_EXT_FUNC_BEGIN(RenderAllToGlBuffer, 1, RENDERALLTOGLBUFFER_FUNC_USAGE) {
        // Put your codes here
        int mid = JS_EXT_PARA(int, 0);

        Napi::Array ret = Napi::Array::New(info.Env());
        Napi::Object retObj = ret.As<Napi::Object>();

        uint32_t arr_idx = 0;
        for (auto inf : g_entries) {
                int sid = static_cast<int>(arr_idx);

                const float *points = NULL, *normals = NULL;
                const int *triangles = NULL;
                int nlen = 0, tlen = 0;

                inf->Points(points, nlen);
                inf->Normals(normals, nlen);
                inf->Triangles(triangles, tlen);

                vector<double> pts, nors;
                copy(points, points + nlen * 3, inserter(pts, pts.end()));
                copy(normals, normals + nlen * 3, inserter(nors, nors.end()));

                vector<int> tris;
                copy(triangles, triangles + tlen * 3,
                     inserter(tris, tris.end()));

                Napi::Value tri_buf =
                    GL::createBufferEx(info.Env(), mid, sid, pts, nors, tris,
                                       {}, {}, {}, {}, {}, RED, GREEN, BLUE);

                retObj.Set((uint32_t)arr_idx++, tri_buf);
        }

        // return ret;
}
JS_EXT_FUNC_END()

#define CLEAR_FUNC_USAGE "Clear Usage: comx.ply.Clear();"
JS_EXT_FUNC_BEGIN(Clear, 0, CLEAR_FUNC_USAGE) {
        // Put your codes here
        for (auto inf : g_entries) {
                delete inf;
        }
        g_entries.clear();
}
JS_EXT_FUNC_END()

#define RELEASEBYENTRY_FUNC_USAGE                                              \
        "ReleaseByEntry Usage: comx.ply.ReleaseByEntry(entry);"
JS_EXT_FUNC_BEGIN(ReleaseByEntry, 1, RELEASEBYENTRY_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullEntry = JS_EXT_PARA(unsigned long long, 0);
        IPlyEntry *pEntry = (IPlyEntry *)((void *)ullEntry);

        auto it = std::find(g_entries.begin(), g_entries.end(), pEntry);

        if (it == g_entries.end())
                return Napi::Boolean::From(info.Env(), false);

        auto inf = *it;

        g_entries.erase(it);
        delete inf;

        return Napi::Boolean::From(info.Env(), true);
}
JS_EXT_FUNC_END()

#define RELEASEBYINDEX_FUNC_USAGE                                              \
        "ReleaseByIndex Usage: comx.ply.ReleaseByIndex(index);"
JS_EXT_FUNC_BEGIN(ReleaseByIndex, 1, RELEASEBYINDEX_FUNC_USAGE) {
        // Put your codes here
        int index = JS_EXT_PARA(int, 0);
        if (index < 0 || index >= static_cast<int>(g_entries.size()))
                return Napi::Boolean::From(info.Env(), false);

        auto it = g_entries.begin() + index;
        auto inf = *it;

        g_entries.erase(it);
        delete inf;
}
JS_EXT_FUNC_END()

#define GETENTRYBYINDEX_FUNC_USAGE                                             \
        "GetEntryByIndex Usage: comx.ply.GetEntryByIndex(index);"
JS_EXT_FUNC_BEGIN(GetEntryByIndex, 1, GETENTRYBYINDEX_FUNC_USAGE) {
        // Put your codes here
        int index = JS_EXT_PARA(int, 0);
        if (index < 0 || index >= static_cast<int>(g_entries.size()))
                return Napi::Boolean::From(info.Env(), false);

        auto it = g_entries.begin() + index;
        auto inf = *it;

        unsigned long long ulRet = (unsigned long long)((void *)&inf);

        string ret_val = type_cast<string>(ulRet);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define RENDERTOGLBUFFER_FUNC_USAGE                                            \
        "RenderEntryToGlBuffer Usage: comx.ply.RenderEntryToGlBuffer(entry, "  \
        "mid);"
JS_EXT_FUNC_BEGIN(RenderEntryToGlBuffer, 2, RENDERTOGLBUFFER_FUNC_USAGE) {
        // Put your codes here
        unsigned long long ullEntry = JS_EXT_PARA(unsigned long long, 0);

        IPlyEntry *pEntry = (IPlyEntry *)((void *)ullEntry);

        int mid = JS_EXT_PARA(int, 1);
        auto it = std::find(g_entries.begin(), g_entries.end(), pEntry);
        auto inf = *it;

        int index = -1;
        if (it != g_entries.end())
                index = std::distance(g_entries.begin(), it);

        // render codes

        if (index == -1)
                return Napi::Boolean::From(info.Env(), false);

        int sid = index;

        Napi::Array ret = Napi::Array::New(info.Env());
        Napi::Object retObj = ret.As<Napi::Object>();

        const float *points = NULL, *normals = NULL;
        const int *triangles = NULL;
        int nlen = 0, tlen = 0, nnlen = 0;

        inf->Points(points, nlen);
        inf->Normals(normals, nnlen);
        inf->Triangles(triangles, tlen);

        vector<double> pts, nors;
        copy(points, points + nlen * 3, inserter(pts, pts.end()));
        copy(normals, normals + nlen * 3, inserter(nors, nors.end()));

        // cout << "PLY vertex number is " << nlen << endl;

        vector<int> tris;
        copy(triangles, triangles + tlen * 3, inserter(tris, tris.end()));

        Napi::Value tri_buf =
            GL::createBufferEx(info.Env(), mid, sid, pts, nors, tris, {}, {},
                               {}, {}, {}, RED, GREEN, BLUE);

        retObj.Set((uint32_t)0, tri_buf);

        return ret;
}
JS_EXT_FUNC_END()

#define IMPORT_FUNC_USAGE "Import Usage: comx.ply.Import(filename);"
JS_EXT_FUNC_BEGIN(Import, 1, IMPORT_FUNC_USAGE) {
        // Put your codes here
        string filename = JS_EXT_PARA(string, 0);
        TPlyEntry *pEntry = new TPlyEntry;

        dcip_ply::triangle_part_t &mesh = *pEntry;
        dcip_ply::readPLYBinary(filename, mesh);

        g_entries.push_back(pEntry);

        IPlyEntry *pRet = pEntry;

        unsigned long long ulRet = (unsigned long long)((void *)pRet);

        string ret_val = type_cast<string>(ulRet);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define INITWORKAREA_FUNC_USAGE "Import Usage: comx.ply.InitWorkArea();"
JS_EXT_FUNC_BEGIN(InitWorkArea, 0, INITWORKAREA_FUNC_USAGE) {
        // Put your codes here
        workData *work = new workData;

        workData *pRet = work;

        unsigned long long ulRet = (unsigned long long)((void *)pRet);

        string ret_val = type_cast<string>(ulRet);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define INITPOSTMANAGE_FUNC_USAGE "Import Usage: comx.ply.InitPostManage();"
JS_EXT_FUNC_BEGIN(InitPostManage, 0, INITPOSTMANAGE_FUNC_USAGE) {
        // Put your codes here
        postManage *work = new postManage;

        postManage *pRet = work;

        unsigned long long ulRet = (unsigned long long)((void *)pRet);

        string ret_val = type_cast<string>(ulRet);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define INITPOSTCOLLISION_FUNC_USAGE                                           \
        "Import Usage: comx.ply.InitPostCollision();"
JS_EXT_FUNC_BEGIN(InitPostCollision, 0, INITPOSTCOLLISION_FUNC_USAGE) {
        // Put your codes here
        postCollision *work = new postCollision;

        postCollision *pRet = work;

        unsigned long long ulRet = (unsigned long long)((void *)pRet);

        string ret_val = type_cast<string>(ulRet);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////////////////
// please set your javascript extension namespace in the
// following codes.

#define JS_EXT_NS "comx.ply"

//////////////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the
// following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(WorldToScreen)
JS_EXT_ENTRY(setHeatFluxes)
JS_EXT_ENTRY(setConvections)
JS_EXT_ENTRY(setThermalConstrains)
JS_EXT_ENTRY(modifyConstraintsExEx)
JS_EXT_ENTRY(setConstraintsExEx)
JS_EXT_ENTRY(modifyConstraintsEx)
JS_EXT_ENTRY(modifyArrowsEx)
JS_EXT_ENTRY(setArrowsEx)
JS_EXT_ENTRY(setConstraintsEx)

JS_EXT_ENTRY(rendererModelExEx)
JS_EXT_ENTRY(setSpheresEx)
JS_EXT_ENTRY(modifySpheresEx)

JS_EXT_ENTRY(rendererModelEx)
JS_EXT_ENTRY(rendererModel)
JS_EXT_ENTRY(loadFilesModel)
JS_EXT_ENTRY(generateBrepsEx)
JS_EXT_ENTRY(RenderEntryToVtlBuffer)
JS_EXT_ENTRY(RenderAllToGlBuffer)
JS_EXT_ENTRY(Clear)
JS_EXT_ENTRY(ReleaseByEntry)
JS_EXT_ENTRY(ReleaseByIndex)
JS_EXT_ENTRY(GetEntryByIndex)
JS_EXT_ENTRY(RenderEntryToGlBuffer)
JS_EXT_ENTRY(Import)
JS_EXT_ENTRY(setPoints)
JS_EXT_ENTRY(InitWorkArea)
JS_EXT_ENTRY(InitPostManage)
JS_EXT_ENTRY(setArrows)
JS_EXT_ENTRY(setConstraints)
JS_EXT_ENTRY(setBeams)
JS_EXT_ENTRY(pickup)
JS_EXT_ENTRY(setSprings)
JS_EXT_ENTRY(setConnects)
JS_EXT_ENTRY(getCurrentName)
JS_EXT_ENTRY(renderer)
JS_EXT_ENTRY(setPostBars)
JS_EXT_ENTRY(mousePick)
JS_EXT_ENTRY(setSpheres)
JS_EXT_ENTRY(getCurrentActorName)
JS_EXT_ENTRY(setChangeData)
JS_EXT_ENTRY(generateBreps)
JS_EXT_ENTRY(modifyPoints)
JS_EXT_ENTRY(hideShow)
JS_EXT_ENTRY(colorRGBS1)
JS_EXT_ENTRY(colorRGBS2)
JS_EXT_ENTRY(colorRGBS3)
JS_EXT_ENTRY(colorRGBS4)
JS_EXT_ENTRY(cutters)
JS_EXT_ENTRY(modifyArrows)
JS_EXT_ENTRY(modifyConstraints)
JS_EXT_ENTRY(modifyBeams)
JS_EXT_ENTRY(modifySprings)
JS_EXT_ENTRY(modifyConnects)
JS_EXT_ENTRY(modifySpheres)
JS_EXT_ENTRY(modifyMarks)
JS_EXT_ENTRY(setMarks)
JS_EXT_ENTRY(openCloseOpacity)
JS_EXT_ENTRY(loadFiles)
JS_EXT_ENTRY(loadFiles1)
JS_EXT_ENTRY(rendererMoTi)
JS_EXT_ENTRY(changeColor)
JS_EXT_ENTRY(deleteAllFile)
JS_EXT_ENTRY(polynomials)
JS_EXT_ENTRY(GuDingSShs)
JS_EXT_ENTRY(sshMoTais)
JS_EXT_ENTRY(maxMinValues)
JS_EXT_ENTRY(rendererPlys)
JS_EXT_ENTRY(setPlyPostChanges)
JS_EXT_ENTRY(OnRenderCollisions)
JS_EXT_ENTRY(InitPostCollision)
JS_EXT_ENTRY(OnChangePostBarCollisions)
JS_EXT_ENTRY(OnleftFastPlayCollisions)
JS_EXT_ENTRY(OnrightFastPlayCollisions)
JS_EXT_ENTRY(OnleftPlayCollisions)
JS_EXT_ENTRY(OnrightPlayCollisions)
JS_EXT_ENTRY(OnGetFrames)
JS_EXT_ENTRY(OnOpenMeshs)
JS_EXT_ENTRY(LoadVtpPlys)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 79)
JS_EXT_FUNC_REG(WorldToScreen)
JS_EXT_FUNC_REG(setHeatFluxes)
JS_EXT_FUNC_REG(setConvections)
JS_EXT_FUNC_REG(setThermalConstrains)
JS_EXT_FUNC_REG(modifyConstraintsExEx)
JS_EXT_FUNC_REG(setConstraintsExEx)
JS_EXT_FUNC_REG(modifyConstraintsEx)
JS_EXT_FUNC_REG(modifyArrowsEx)
JS_EXT_FUNC_REG(setConstraintsEx)
JS_EXT_FUNC_REG(setArrowsEx)
JS_EXT_FUNC_REG(setSpheresEx)
JS_EXT_FUNC_REG(rendererModelExEx)
JS_EXT_FUNC_REG(rendererModelEx)
JS_EXT_FUNC_REG(rendererModel)
JS_EXT_FUNC_REG(loadFilesModel)
JS_EXT_FUNC_REG(generateBrepsEx)
JS_EXT_FUNC_REG(RenderEntryToVtlBuffer)
JS_EXT_FUNC_REG(RenderAllToGlBuffer)
JS_EXT_FUNC_REG(Clear)
JS_EXT_FUNC_REG(ReleaseByEntry)
JS_EXT_FUNC_REG(ReleaseByIndex)
JS_EXT_FUNC_REG(GetEntryByIndex)
JS_EXT_FUNC_REG(RenderEntryToGlBuffer)
JS_EXT_FUNC_REG(Import)
JS_EXT_FUNC_REG(setPoints)
JS_EXT_FUNC_REG(InitWorkArea)
JS_EXT_FUNC_REG(InitPostManage)
JS_EXT_FUNC_REG(setArrows)
JS_EXT_FUNC_REG(setConstraints)
JS_EXT_FUNC_REG(setBeams)
JS_EXT_FUNC_REG(pickup)
JS_EXT_FUNC_REG(setSprings)
JS_EXT_FUNC_REG(setConnects)
JS_EXT_FUNC_REG(getCurrentName)
JS_EXT_FUNC_REG(renderer)
JS_EXT_FUNC_REG(setPostBars)
JS_EXT_FUNC_REG(mousePick)
JS_EXT_FUNC_REG(setSpheres)
JS_EXT_FUNC_REG(getCurrentActorName)
JS_EXT_FUNC_REG(setChangeData)
JS_EXT_FUNC_REG(generateBreps)
JS_EXT_FUNC_REG(modifyPoints)
JS_EXT_FUNC_REG(hideShow)
JS_EXT_FUNC_REG(modifyArrows)
JS_EXT_FUNC_REG(modifyConstraints)
JS_EXT_FUNC_REG(modifyBeams)
JS_EXT_FUNC_REG(modifySprings)
JS_EXT_FUNC_REG(colorRGBS1)
JS_EXT_FUNC_REG(colorRGBS2)
JS_EXT_FUNC_REG(colorRGBS3)
JS_EXT_FUNC_REG(colorRGBS4)
JS_EXT_FUNC_REG(cutters)
JS_EXT_FUNC_REG(modifyConnects)
JS_EXT_FUNC_REG(modifySpheres)
JS_EXT_FUNC_REG(modifyMarks)
JS_EXT_FUNC_REG(setMarks)
JS_EXT_FUNC_REG(openCloseOpacity)
JS_EXT_FUNC_REG(loadFiles)
JS_EXT_FUNC_REG(loadFiles1)
JS_EXT_FUNC_REG(rendererMoTi)
JS_EXT_FUNC_REG(changeColor)
JS_EXT_FUNC_REG(deleteAllFile)
JS_EXT_FUNC_REG(polynomials)
JS_EXT_FUNC_REG(GuDingSShs)
JS_EXT_FUNC_REG(sshMoTais)
JS_EXT_FUNC_REG(maxMinValues)
JS_EXT_FUNC_REG(rendererPlys)
JS_EXT_FUNC_REG(setPlyPostChanges)
JS_EXT_FUNC_REG(OnRenderCollisions)
JS_EXT_FUNC_REG(InitPostCollision)
JS_EXT_FUNC_REG(OnChangePostBarCollisions)
JS_EXT_FUNC_REG(OnleftFastPlayCollisions)
JS_EXT_FUNC_REG(OnrightFastPlayCollisions)
JS_EXT_FUNC_REG(OnleftPlayCollisions)
JS_EXT_FUNC_REG(OnrightPlayCollisions)
JS_EXT_FUNC_REG(OnGetFrames)
JS_EXT_FUNC_REG(OnOpenMeshs)
JS_EXT_FUNC_REG(LoadVtpPlys)
JS_EXT_MAIN_END()