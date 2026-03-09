#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>

// OCC Headers
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <NCollection_IndexedDataMap.hxx>

// JsonCpp Header
#include "jsoncpp/json/json.h"
#include "FaceProperty.h"
using namespace std;




// --- 特征数据结构 ---
struct FaceSignature
{
    int originalId;   // 原始 ID (Map的Key)
    double cX, cY, cZ; // 质心
    double nX, nY, nZ; // 法向量
    int surfaceType;  // 曲面类型枚举值
    double area;      // 面积

    // 边界框
    double bboxMinX, bboxMinY, bboxMinZ;
    double bboxMaxX, bboxMaxY, bboxMaxZ;

    FaceSignature() : originalId(-1), cX(0), cY(0), cZ(0),
        nX(0), nY(0), nZ(1), surfaceType(0), area(0),
        bboxMinX(0), bboxMinY(0), bboxMinZ(0),
        bboxMaxX(0), bboxMaxY(0), bboxMaxZ(0) {}
};



class FaceFeatureManager
{
public:
    // ==========================================
    // 功能 3: 从 Map 中提取特征信息
    // ==========================================
    static std::vector<FaceSignature> ExtractFromMap(
        const NCollection_IndexedDataMap<int, FaceProperty>& inputMap)
    {
        std::vector<FaceSignature> signatures;
        signatures.reserve(inputMap.Extent());

        for (int i = 1; i <= inputMap.Extent(); ++i)
        {
            const FaceProperty& prop = inputMap.FindFromIndex(i);
            int keyId = inputMap.FindKey(i);

            if (prop.faceObj.IsNull()) continue;

            FaceSignature sig;
            sig.originalId = keyId;

            // 1. 计算几何属性 (质心 & 面积)
            GProp_GProps systemProps;
            BRepGProp::SurfaceProperties(prop.faceObj, systemProps);
            gp_Pnt centroid = systemProps.CentreOfMass();
            sig.area = systemProps.Mass();
            sig.cX = centroid.X(); sig.cY = centroid.Y(); sig.cZ = centroid.Z();

            // 2. 获取曲面类型
            BRepAdaptor_Surface adaptor(prop.faceObj);
            sig.surfaceType = (int)adaptor.GetType();

            // 3. 计算法向量 (封装的私有函数)
            gp_Vec normal = CalculateNormal(prop.faceObj, centroid);
            sig.nX = normal.X(); sig.nY = normal.Y(); sig.nZ = normal.Z();

            // 4. 计算边界框
            Bnd_Box box;
            BRepBndLib::Add(prop.faceObj, box);
            box.Get(sig.bboxMinX, sig.bboxMinY, sig.bboxMinZ,
                sig.bboxMaxX, sig.bboxMaxY, sig.bboxMaxZ);

            signatures.push_back(sig);
        }
        return signatures;
    }

    // ==========================================
    // 功能 1: 把特征信息生成 JSON 文件
    // ==========================================
    static bool SaveToJson(const std::vector<FaceSignature>& signatures, const std::string& filePath)
    {
        Json::Value root(Json::arrayValue);

        for (const auto& sig : signatures) {
            Json::Value node;
            node["id"] = sig.originalId;
            node["type"] = sig.surfaceType;
            node["area"] = sig.area;

            Json::Value center(Json::arrayValue);
            center.append(sig.cX); center.append(sig.cY); center.append(sig.cZ);
            node["center"] = center;

            Json::Value normal(Json::arrayValue);
            normal.append(sig.nX); normal.append(sig.nY); normal.append(sig.nZ);
            node["normal"] = normal;

            Json::Value bbox(Json::objectValue);
            Json::Value bmin(Json::arrayValue);
            bmin.append(sig.bboxMinX); bmin.append(sig.bboxMinY); bmin.append(sig.bboxMinZ);
            Json::Value bmax(Json::arrayValue);
            bmax.append(sig.bboxMaxX); bmax.append(sig.bboxMaxY); bmax.append(sig.bboxMaxZ);
            bbox["min"] = bmin;
            bbox["max"] = bmax;
            node["bbox"] = bbox;

            root.append(node);
        }

        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "    "; // 4空格缩进

        std::ofstream outputFile(filePath);
        if (!outputFile.is_open()) return false;

        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &outputFile);
        outputFile.close();

        return true;
    }

    // ==========================================
    // 功能 2: 把包含特征信息的 JSON 读取出来
    // ==========================================
    static std::vector<FaceSignature> LoadFromJson(const std::string& filePath)
    {
        std::vector<FaceSignature> signatures;
        std::ifstream inputFile(filePath);
        if (!inputFile.is_open()) return signatures;

        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errs;

        if (!Json::parseFromStream(builder, inputFile, &root, &errs)) {
            std::cerr << "JSON Parse Error: " << errs << std::endl;
            return signatures;
        }

        if (!root.isArray()) return signatures;

        for (const auto& node : root) {
            FaceSignature sig;
            sig.originalId = node["id"].asInt();
            sig.surfaceType = node["type"].asInt();
            sig.area = node["area"].asDouble();

            if (node["center"].isArray() && node["center"].size() == 3) {
                sig.cX = node["center"][0].asDouble();
                sig.cY = node["center"][1].asDouble();
                sig.cZ = node["center"][2].asDouble();
            }

            if (node["normal"].isArray() && node["normal"].size() == 3) {
                sig.nX = node["normal"][0].asDouble();
                sig.nY = node["normal"][1].asDouble();
                sig.nZ = node["normal"][2].asDouble();
            }

            // 读取 bbox (简化省略具体代码，逻辑同上)
            // ...

            signatures.push_back(sig);
        }
        return signatures;
    }

    // ==========================================
    // 功能 4: 对比新旧特征，返回 ID 映射表
    // 返回 map<OldID, NewID>
    // ==========================================
    static std::map<int, int> MapOldToNewIDs(
        const std::vector<FaceSignature>& oldSigs,
        const std::vector<FaceSignature>& newSigs)
    {
        std::map<int, int> mapping; // OldID -> NewID
        std::vector<bool> newIdUsed(newSigs.size(), false); // 标记新面是否已被匹配

        // 遍历每一个旧面，尝试在新的列表中找到最相似的
        for (const auto& oldSig : oldSigs) {
            int bestMatchIndex = -1;
            double minDiffScore = std::numeric_limits<double>::max();

            // 阈值设定
            const double AREA_TOLERANCE = 0.05; // 面积差异允许 5%
            const double DIST_TOLERANCE = 1.0;  // 质心距离允许 1.0mm (假设没有大幅移动)
            const double DOT_TOLERANCE = 0.9;   // 法向量点积 (cos 25度 approx 0.9)

            for (size_t i = 0; i < newSigs.size(); ++i) {
                if (newIdUsed[i]) continue; // 已经被占用的新面跳过
                const auto& newSig = newSigs[i];

                // 1. 快速过滤：曲面类型必须一致
                if (oldSig.surfaceType != newSig.surfaceType) continue;

                // 2. 快速过滤：面积差异
                double areaDiffRatio = std::abs(oldSig.area - newSig.area) / (oldSig.area + 1e-9);
                if (areaDiffRatio > AREA_TOLERANCE) continue;

                // 3. 计算法向量匹配度 (点积)
                gp_Vec vOld(oldSig.nX, oldSig.nY, oldSig.nZ);
                gp_Vec vNew(newSig.nX, newSig.nY, newSig.nZ);
                double dotProd = vOld.Dot(vNew);

                // 注意：如果面是平面对称或无方向(如球)，可能需要特殊处理，这里处理一般情况
                if (dotProd < DOT_TOLERANCE) continue;

                // 4. 计算位置距离
                gp_Pnt pOld(oldSig.cX, oldSig.cY, oldSig.cZ);
                gp_Pnt pNew(newSig.cX, newSig.cY, newSig.cZ);
                double dist = pOld.Distance(pNew);

                if (dist > DIST_TOLERANCE) continue;

                // 5. 综合评分 (越小越好)
                // 权重：距离影响最大，面积次之
                double score = dist * 10.0 + areaDiffRatio * 100.0 + (1.0 - dotProd) * 5.0;

                if (score < minDiffScore) {
                    minDiffScore = score;
                    bestMatchIndex = i;
                }
            }

            // 如果找到了匹配项
            if (bestMatchIndex != -1) {
                mapping[oldSig.originalId] = newSigs[bestMatchIndex].originalId;
                newIdUsed[bestMatchIndex] = true; // 锁定该新面
            }
        }

        return mapping;
    }

private:
    // 私有辅助：计算法向量
    static gp_Vec CalculateNormal(const TopoDS_Face& face, const gp_Pnt& p) {
        BRepAdaptor_Surface adaptor(face);
        double u, v;

        if (adaptor.GetType() == GeomAbs_Plane) {
            gp_Dir d = adaptor.Plane().Axis().Direction();
            return (face.Orientation() == TopAbs_REVERSED) ? -gp_Vec(d) : gp_Vec(d);
        }

        GeomAPI_ProjectPointOnSurf proj(p, adaptor.Surface().Surface());
        if (proj.NbPoints() > 0) proj.LowerDistanceParameters(u, v);
        else {
            u = (adaptor.FirstUParameter() + adaptor.LastUParameter()) * 0.5;
            v = (adaptor.FirstVParameter() + adaptor.LastVParameter()) * 0.5;
        }

        GeomLProp_SLProps props(adaptor.Surface().Surface(), u, v, 1, 1e-7);
        if (props.IsNormalDefined()) {
            gp_Vec norm(props.Normal());
            if (face.Orientation() == TopAbs_REVERSED) norm.Reverse();
            return norm;
        }
        return gp_Vec(0, 0, 1);
    }
};