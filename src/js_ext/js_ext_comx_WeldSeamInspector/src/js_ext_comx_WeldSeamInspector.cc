#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;

#include <ModelIO/ModelIO.h>
#include<WeldSeamInspector/ShapeAnalyzer.cpp>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////
/* DCiP plugin Javascript parameter and return value parsing/wrap guide

   1)Parse callback paramter by index from javascript by using the following
   line: var cb = JS_EXT_PARA(TComxCallback, [index]);

     Use the following codes to invokd callback function:
     cb([parameters list]);

     cb also can be saved as a global variable.

   2)Parse common paramter by index from javascript by using the following line:
     var val = JS_EXT_PARA([value_type], [index]);

   3)Parse buffer paramter by index from javascript by using the following line:
     TComxBuffer buf = JS_EXT_PARA(TComxBuffer, [index]);

     Use the following codes to operate buffer:

        void * arr_buf = buf.Data();
        size_t arr_byte_size = buf.ByteLength();

        int16_t *arr_data = (int16_t*)arr_buf;
        size_t arr_len = arr_byte_size / sizeof(int16_t);

        for (size_t i = 0; i < arr_len; i++)
        {
             arr_data[i] += 2;
        }

   4)Return a buffer by using the following line:
     JS_EXT_FUNC_ASSIGN_RET_EX(buf, 1);

   5)Return a common value by using the follwoing line
     JS_EXT_FUNC_ASSIGN_RET([value]);
*/

// 哈希函数
struct FaceHasher {
    std::size_t operator()(const TopoDS_Face& face) const {
        return TopTools_ShapeMapHasher::HashCode(face, 2147483647); // 大质数
    }
};

// 比较器：底层对象是否相同
struct FaceEqual {
    bool operator()(const TopoDS_Face& f1, const TopoDS_Face& f2) const {
        return f1.IsSame(f2);
    }
};

// 映射函数：构建 face -> id 快速查找表
std::unordered_map<TopoDS_Face, int, FaceHasher, FaceEqual>
buildFaceMap(const NCollection_IndexedDataMap<int, FaceProperty>& faceMap) {
    std::unordered_map<TopoDS_Face, int, FaceHasher, FaceEqual> result;
    for (int i = 1; i <= faceMap.Extent(); ++i) {
        int id = faceMap.FindKey(i);
        const TopoDS_Face& face = faceMap.FindFromIndex(i).faceObj;
        result[face] = id;
    }
    return result;
}


// 辅助函数：将vector<int>转换为逗号分隔的字符串
std::string vectorToCommaSeparatedString(const std::vector<int>& vec) {
    if (vec.empty()) {
        return "";
    }
    
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }
        oss << vec[i]; // 格式化为3位数字，如"001"
    }
    return oss.str();
}


// 主函数：转换为 JSON
Json::Value convertToJson(
    const std::vector<DetectionResult>& detectionResults,
    const std::unordered_map<TopoDS_Face, int, FaceHasher, FaceEqual>& faceToIDMap,
    const NCollection_IndexedDataMap<TopoDS_Shape, std::string>& shapeName,
    const NCollection_IndexedDataMap<TopoDS_Face, std::vector<int>>& faceEdge)
{
    Json::Value root;
    Json::Value relations(Json::arrayValue);
    cout<<"detectionResults size: "<<detectionResults.size()<<endl;

    for (const auto& result : detectionResults) {
        size_t count = std::min(result.attachedFaces1.size(), result.attachedFaces2.size());
        for (size_t i = 0; i < count; ++i) {
            const auto& f1 = result.attachedFaces1[i];
            const auto& f2 = result.attachedFaces2[i];

            auto it1 = faceToIDMap.find(f1);
            auto it2 = faceToIDMap.find(f2);

            if (it1 == faceToIDMap.end() || it2 == faceToIDMap.end()) {
                std::cerr << "[Warning] Face not found in faceMap.\n";
                continue;
            }

            // 获取形状名称 - 使用 FindIndex 直接查找
            std::string contactShapeName = "unknown";
            std::string targetShapeName = "unknown";
            
            // 查找shape1名称
            
            int shapeIndex1 = shapeName.FindIndex(result.shape1);
            if (shapeIndex1 > 0) {
                contactShapeName = shapeName.FindFromIndex(shapeIndex1);
            }
            

            
            // 查找shape2名称
            int shapeIndex2 = shapeName.FindIndex(result.shape2);
            if (shapeIndex2 > 0) {
                targetShapeName = shapeName.FindFromIndex(shapeIndex2);
            }

            
            // 获取边ID - 使用 FindIndex 直接查找
            std::string contactEdgeIDs = "";
            std::string targetEdgeIDs = "";
            
            // 查找face1的边ID
            int faceIndex1 = faceEdge.FindIndex(f1);
            if (faceIndex1 > 0) {
                contactEdgeIDs = vectorToCommaSeparatedString(faceEdge.FindFromIndex(faceIndex1));
            }
            
            // 查找face2的边ID
            int faceIndex2 = faceEdge.FindIndex(f2);
            if (faceIndex2 > 0) {
                targetEdgeIDs = vectorToCommaSeparatedString(faceEdge.FindFromIndex(faceIndex2));
            }

            Json::Value relation;
            relation["contactFaceID"] = it1->second;  // 保持原来的数字格式
            relation["targetFaceID"] = it2->second;   // 保持原来的数字格式
            relation["contactEdgeID"] = contactEdgeIDs;
            relation["targetEdgeID"] = targetEdgeIDs;
            relation["contact"] = contactShapeName;
            relation["target"] = targetShapeName;
            relation["relationType"] = "contact";
            relations.append(relation);
        }
    }

    root["relations"] = relations;
    return root;
}


int CountMatchingShapes(
    const NCollection_Map<TopoDS_Shape, TopTools_ShapeMapHasher>& shapes,
    const std::vector<DetectionResult>& detectionResults)
{
    int count = 0;

    for (NCollection_Map<TopoDS_Shape, TopTools_ShapeMapHasher>::Iterator it(shapes); it.More(); it.Next()) {
        const TopoDS_Shape& s = it.Key();

        for (const auto& det : detectionResults) {
            if (s.IsSame(det.shape1)) {  // 判断是否是同一个 Shape
                count++;
            }
        }
    }

    return count;
}
//////////////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following codes.

JS_EXT_DATA_DECLARE()

// 0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
// please don't modify or delete the previous line codes.


#define GETSHAPERELATION_FUNC_USAGE "getShapeRelation Usage: comx_sdk.getShapeRelation(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(getShapeRelation, 2, GETSHAPERELATION_FUNC_USAGE)
{
    unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
    double maxDistance = JS_EXT_PARA(double, 1);
    ModelIO& rDB = (*(ModelIO*)((void*)ullDB));
    Handle(TDocStd_Document) document;
    NCollection_IndexedDataMap<int, FaceProperty> faceMap;
    rDB.GetDocument(document);
    rDB.GetFaceMap(faceMap);
    NCollection_Map<TopoDS_Shape, TopTools_ShapeMapHasher> shapes = rDB.getShapes();
    cout<<"shapes size: "<<shapes.Extent()<<endl;
    
    ShapeAnalyzer analyzer;
    std::vector<DetectionResult> detectionResults;
    analyzer.getShapeRelation(document, detectionResults, maxDistance);


    cout<<"count:" << CountMatchingShapes(shapes, detectionResults)<<endl;
    
    std::unordered_map<TopoDS_Face, int, FaceHasher, FaceEqual> fastFaceMap;
    fastFaceMap = buildFaceMap(faceMap);
    
    // 创建 shapeName 容器 - 从 detectionResults 中提取形状名称
    NCollection_IndexedDataMap<TopoDS_Shape, std::string> shapeName = rDB.getShapeNameMap();
    for (int i = 1; i <= shapeName.Extent(); i++) {
        const TopoDS_Shape& shape = shapeName.FindKey(i);      // 获取 key
        const std::string& name = shapeName.FindFromIndex(i);  // 获取 value
        cout << "shapeName: " << name << endl;
    }

    NCollection_IndexedDataMap<TopoDS_Shape, std::string> shapeName1 = rDB.getShapeNameMap1();
    for (int i = 1; i <= shapeName1.Extent(); i++) {
        const TopoDS_Shape& shape = shapeName1.FindKey(i);      // 获取 key
        const std::string& name = shapeName1.FindFromIndex(i);  // 获取 value
        cout << "shapeName: " << name << endl;
    }
    



    NCollection_IndexedDataMap<TopoDS_Face, std::vector<int>> faceEdge = rDB.getFaceEdgeMap();
    cout<<"shapeName size: "<<shapeName.Extent()<<endl;
    cout<<"faceEdge size: "<<faceEdge.Extent()<<endl;

    
    Json::Value jsonRoot = convertToJson(detectionResults, fastFaceMap, shapeName1, rDB.getFaceEdgeMap());
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";  // 2空格缩进
    std::string jsonStr = Json::writeString(writer, jsonRoot);
    JS_EXT_FUNC_ASSIGN_RET(jsonStr); 
}
JS_EXT_FUNC_END()

#define GETCHECKFACE_FUNC_USAGE                                                \
        "GetCheckFace Usage: comx_sdk.GetCheckFace(/*put your parameters "     \
        "list here*/);"
JS_EXT_FUNC_BEGIN(GetCheckFace, 1, GETCHECKFACE_FUNC_USAGE) {
        // Put your codes here
    cout<<"GetCheckFace"<<endl;
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));
        Handle(TDocStd_Document) document;
        vector<TopoDS_Face> faces;
        NCollection_IndexedDataMap<int, FaceProperty> faceMap;
        rDB.GetDocument(document);
        rDB.GetFaceMap(faceMap);
        ShapeAnalyzer analyzer;

        analyzer.GetCheckFace(document, faces, 5);
        cout <<"faces size: " << faces.size() << endl;
        cout << "faceMap size: " << faceMap.Extent() << endl;
        
        std::vector<int> faceIDs;
        faceIDs.reserve(faces.size());

        for (const TopoDS_Face& f : faces) {
            bool found = false;

    
            const int n = faceMap.Extent();
            for (int idx = 1; idx <= n; ++idx) {
               
                int id = faceMap.FindKey(idx);
                const FaceProperty& prop = faceMap.ChangeFromIndex(idx);
                if (prop.faceObj.IsEqual(f)) {
                    faceIDs.push_back(id);
                    found = true;
                    break;
                }
            }

            // �������ԡ��Ҳ���������ʾ�����������
            if (!found) {
                std::cerr << "Warning: �Ҳ��� face �� faceMap �еĶ�Ӧ��Ŀ\n";
            }
        }

        cout<<"faceIDs size: "<<faceIDs.size()<<endl;
        Json::Value root(Json::arrayValue);

        // 3. �� vector<int> �е�ÿ��ֵ push �� root ��
        for (int x : faceIDs) {
            root.append(x);  // ��� root �� arrayValue���ͻ�� x �ŵ�����ĩβ
        }

        // 4a. �� root ���л����ַ��� (Ư����ʽ������ 2 ���ո�)
        Json::StreamWriterBuilder writerBuilder;      
        writerBuilder["indentation"] = "";
        std::string jsonText = Json::writeString(writerBuilder, root);
        cout << jsonText << endl;
        JS_EXT_FUNC_ASSIGN_RET(jsonText);

}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////////////////
// please set your javascript extension namespace in the following codes.

#define JS_EXT_NS "comx.WeldSeamInspector"

//////////////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(getShapeRelation)
JS_EXT_ENTRY(GetCheckFace)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 2)
JS_EXT_FUNC_REG(getShapeRelation)
JS_EXT_FUNC_REG(GetCheckFace)
JS_EXT_MAIN_END()
