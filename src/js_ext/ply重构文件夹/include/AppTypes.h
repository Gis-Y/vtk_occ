#pragma once
#include <string>
#include <vector>
#include <map>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkTextActor3D.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>

// VTK 渲染所需的头文件
#include <vtkRenderer.h>

using namespace std;

// 定义颜色常量
#define RED 169
#define GREEN 166
#define BLUE 171

// 基础几何结构
struct Point3D {
    double x, y, z;
};

struct guass {
    int id;
    Point3D pts;
};

struct RGBS {
    double r, g, b;
};

// 业务结构定义
struct ArrowFrame {
    string name;
    string selectPoint;
    double x, y, z;
    double xn, yn, zn;
    double size;
    vector<double> circle;
};

struct arrow {
    string name;
    string mark;
    string selectPoint;
    string size;
    double x, y, z;
    double xn, yn, zn;
    double xr, yr, zr;
};

struct constrain {
    string name;
    string selectPoint;
    double x, y, z;
    int xr, yr, zr;
    int xn, yn, zn;
};

struct beam {
    string name;
    vector<Point3D> pos;
    double radius;
    double len;
    double num;
    double M;
    double poi;
};

struct spring {
    string name;
    string type1, type2;
    double stiffness_x, stiffness_y, stiffness_z;
    double startX, startY, startZ;
    double endX, endY, endZ;
};

struct connects {
    string name;
    string mark1, mark2;
    double x1, y1, z1;
    double x2, y2, z2;
    int xn, yn, zn;
    int xr, yr, zr;
};

struct RB3 {
    string type;
    vector<int> id;
    string name;
    vector<Point3D> cp;
    double x, y, z;
};

struct forceFace {
    int selectFaceId;
    string size;
    int xn, yn, zn;
    vector<double> disface;
};

struct fixFace {
    int selectFaceId;
    int xn, yn, zn;
    int xr, yr, zr;
};

struct DataSet {
    double x, y, z;
    double value;
};

// 梁的渲染数据
struct beamsData {
    vector<vtkSmartPointer<vtkActor>> line;
    vector<vtkSmartPointer<vtkActor>> point;
};