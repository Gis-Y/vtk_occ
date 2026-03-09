#pragma once
#include "AppContext.h"

namespace VTKUtils {
    // ================== 工具函数 ==================
    void callbackMouse(void* event, void* pRawInf); // 需根据实际类型调整
    bool GetGaussPoints(workData* work, string id, double scale, vector<guass>& pts);
    double distanceBetweenPoints(double x1, double y1, double z1, double x2, double y2, double z2);

    // ================== 几何创建 ==================
    string setSphereEx(double x, double y, double z, double radius, string name, vtkRenderer* renderer, workData* work, vector<string> ids);
    string setArrow1Ex(double x, double y, double z, double nx, double ny, double nz, string name, vtkRenderer* renderer, workData* work, int flag, string id, double scale);
    string setArrowEx(double x, double y, double z, double nx, double ny, double nz, string name, workData *work, double scale);

    // ================== 约束/热流 ==================
    bool CreateConstraints(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale, string id, int xd, int yd, int zd, int xr, int yr, int zr, int flag);
    bool modifyConstraintsNew(double x, double y, double z, string name, vtkRenderer* renderer, workData* work, double scale, string id, int xd, int yd, int zd, int xr, int yr, int zr, int flag);
    
    bool CreateThermalConstraints(double x, double y, double z, double xn, double yn, double zn, string name, vtkRenderer* renderer, workData* work, string id, double scale);
    
    bool createHeatFluxes(double x, double y, double z, double nx, double ny, double nz, string name, vtkRenderer* renderer, workData* work, string id, double scale);
    bool createConvections(double x, double y, double z, double nx, double ny, double nz, string name, vtkRenderer* renderer, workData* work, string id, double scale);

    // ================== 基础绘图 ==================
    string modifyPoint(double x, double y, double z, double radius, string name, vtkRenderer *renderer, workData *work);
    string setPoint(double x, double y, double z, double radius, string name, vtkRenderer *renderer, workData *work);
    
    // ================== 清理函数 ==================
    void removeArrowsByName(vtkRenderer *renderer, workData *work, const std::string &name);
    void removeConstraintsByName(vtkRenderer* renderer, workData* work, const std::string& name);
    void removeHeatFluxesByName(vtkRenderer* renderer, workData* work, const std::string& name);
    void removeConvectionsByName(vtkRenderer* renderer, workData* work, const std::string& name);
    void removeThermalConstraintsByName(vtkRenderer* renderer, workData* work, const std::string& name);
}