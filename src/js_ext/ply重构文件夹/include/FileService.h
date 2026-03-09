#pragma once
#include "AppContext.h"
#include <string>
#include <vector>

namespace FileService {
    // 文件操作
    void getFilesClassified(string path, vector<vector<string>> &classifiedFiles);
    void getFiles(string path, vector<string> &files);
    void deleteAllFiles(string folderPath);
    
    // 模型加载
    string OnModelVTKLoad(string pathSTL, string pathVts, double size, vtkRenderer *renderer, postManage *work);
    string OnModelVTKLoadWithDeformation(string path, string path1, double deformationScale, vtkRenderer *renderer, postManage *work);
    void LoadVtpPly(string name1, string name2, vtkRenderer *renderer);
    
    // 业务生成
    string generateBrep(string id, string path, workData *work, string path2);
}