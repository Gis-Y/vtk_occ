#pragma once
#include "AppTypes.h"
#include <vtkSTLReader.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkStructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkResampleWithDataSet.h>
#include <vtkClipClosedSurface.h>
#include <vtkPlaneWidget.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <dcip_ply_io.hxx> // 假设这是你的外部库

// 显示动力学后处理模块
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

// 结构前处理数据（核心类）
class workData {
public:
    map<string, vtkSmartPointer<vtkActor>> Points;
    map<string, vtkSmartPointer<vtkActor>> Arrows;
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> Arrows1;
    map<string, vtkSmartPointer<vtkAssembly>> ArrowAssemblies;
    map<string, vtkSmartPointer<vtkTextActor3D>> TitleArrows;

    map<string, vtkSmartPointer<vtkActor>> Constrains;
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ConstrainsTranslationMap;
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ConstrainsRotationMap;
    map<string, vtkSmartPointer<vtkAssembly>> ConstrainsAssemblies;

    map<string, vtkSmartPointer<vtkActor>> ThermalConstrains;
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ThermalConstrainsMap;
    map<string, vtkSmartPointer<vtkAssembly>> ThermalConstrainsAssemblies;

    map<string, vtkSmartPointer<vtkActor>> Convections;
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> ConvectionsMap;
    map<string, vtkSmartPointer<vtkAssembly>> ConvectionsAssemblies;

    map<string, vtkSmartPointer<vtkActor>> HeatFluxes;
    std::map<std::string, std::vector<vtkSmartPointer<vtkActor>>> HeatFluxesMap;
    map<string, vtkSmartPointer<vtkAssembly>> HeatFluxesAssemblies;

    map<string, vtkSmartPointer<vtkActor>> Spring;
    map<string, vtkSmartPointer<vtkActor>> Connects;
    map<string, vtkSmartPointer<vtkActor>> Rb3;
    map<string, beamsData> Beams;
    map<string, vtkSmartPointer<vtkTextActor3D>> mark;
    map<string, vtkSmartPointer<vtkActor>> distributedArrows;
    map<string, vtkSmartPointer<vtkActor>> pointActor;

    vtkSmartPointer<vtkActor> actor_Mesh = vtkSmartPointer<vtkActor>::New();
    vector<guass> pts;
    map<string, vector<guass>> newPts;
    
    // 用于通用绘图的临时变量
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
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

// PLY 接口定义
struct IPlyEntry {
    virtual bool Points(const float *&pts, int &nlen) = 0;
    virtual bool Triangles(const int *&elements, int &elen) = 0;
    virtual bool Normals(const float *&normals, int &nnlen) = 0;
    virtual ~IPlyEntry() {}
};

// 全局变量声明 (如果需要跨文件访问)
extern vector<IPlyEntry *> g_entries;