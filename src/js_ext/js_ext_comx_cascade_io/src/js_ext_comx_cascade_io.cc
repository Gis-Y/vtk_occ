#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
// Open Cascade library.
#include <Standard_TypeDef.hxx>
#include <gp_XYZ.hxx>
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimApI_MakeSphere.hxx>
#include <BRep_Tool.hxx>
#include <Standard_TypeDef.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pln.hxx>

#include <BRepTools.hxx>

#include <IGESControl_Controller.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>

#include <Interface_Static.hxx>
#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

#include <STEPConstruct.hxx>
#include <STEPConstruct_Styles.hxx>
#include <StepVisual_StyledItem.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <XSControl_TransferReader.hxx>
#include <XSControl_WorkSession.hxx>

// #include <BRepMesh.hxx>
#include <Poly_Triangulation.hxx>
#include <TShort_Array1OfShortReal.hxx>
#include <TopExp_Explorer.hxx>

#include <IGESCAFControl_Reader.hxx>
#include <Quantity_Color.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>

// Model IO Begin
#include <ModelIO/ModelIO.h>
#include <ModelIO/FaceDataExtractor.h>
#include <NCollection_IndexedDataMap.hxx>
// Model IO End
gp_Pnt ComputeFacesCentroid(const std::vector<TopoDS_Face>& faces)
{
    GProp_GProps props;
    double sumX = 0.0;
    double sumY = 0.0; 
    double sumZ = 0.0;
    double totalArea = 0.0;

    for (size_t i = 0; i < faces.size(); i++)
    {
        BRepGProp::SurfaceProperties(faces[i], props);
        double area = props.Mass();
        gp_Pnt center = props.CentreOfMass();

        sumX += center.X() * area;
        sumY += center.Y() * area;
        sumZ += center.Z() * area;
        totalArea += area;
    }

    if (totalArea > 1e-9)
    {
        sumX /= totalArea;
        sumY /= totalArea;
        sumZ /= totalArea;
    }

    return gp_Pnt(sumX, sumY, sumZ);
}
string ReadSTEPEx(const Standard_CString &aFileName, ModelIO &readStep) {
        readStep.Read(aFileName, GeomSTP);
        readStep.Perform();
        string ret_val = readStep.GetJson();
        return ret_val;
}

string ReadIGESEx(const Standard_CString &aFileName, ModelIO &readIgs) {
        readIgs.Read(aFileName, GeomIGS);
        readIgs.Perform();
        string ret_val = readIgs.GetJson();
        return ret_val;
}

string ReadBREPEx(const Standard_CString &aFileName, ModelIO &readBRep) {
        readBRep.Read(aFileName, GeomBREP);
        readBRep.Perform();
        string ret_val = readBRep.GetJson();
        return ret_val;
}

string ReadSTLEx(const Standard_CString &aFileName, ModelIO &readStl) {
        readStl.Read(aFileName, GeomSTL);
        readStl.Perform();
        string ret_val = readStl.GetJson();
        return ret_val;
}
string ReadGLTFEx(const Standard_CString &aFileName, ModelIO &readGltf) {
        readGltf.Read(aFileName, GeomGLTF);
        readGltf.Perform();
        string ret_val = readGltf.GetJson();
        return ret_val;
}

bool WriteSTEPEx(const Standard_CString &aFileName, ModelIO &model) {
        return model.Write(aFileName, GeomSTP);
}

bool WriteIGESEx(const Standard_CString &aFileName, ModelIO &model) {
        return model.Write(aFileName, GeomSTP);
}
bool WriteBREPEx(const Standard_CString &aFileName, ModelIO &model) {
        return model.Write(aFileName, GeomBREP);
}

bool WriteSTLEx(const Standard_CString &aFileName, ModelIO &model) {
        return model.Write(aFileName, GeomSTL);
}
bool WriteGLTFEx(const Standard_CString &aFileName, ModelIO &model) {
        return model.Write(aFileName, GeomGLTF);
}

bool ReadIGES(const Standard_CString &aFileName,
              TopTools_HSequenceOfShape &aHSequenceOfShape) {
        // aHSequenceOfShape.Clear();

        IGESControl_Reader Reader;
        Standard_Integer status = Reader.ReadFile(aFileName);

        if (status != IFSelect_RetDone) {
                return false;
        }

        Reader.TransferRoots();

        TopoDS_Shape aShape = Reader.OneShape();
        aHSequenceOfShape.Append(aShape);

        return true;
}

bool SaveIges(const std::string &igesFile,
              TopTools_HSequenceOfShape &aHSequenceOfShape) {
        IGESControl_Controller::Init();
        IGESControl_Writer igesWriter;

        for (int index = 1; index <= aHSequenceOfShape.Length(); ++index) {
                TopoDS_Shape shape = aHSequenceOfShape.ChangeValue(index);
                igesWriter.AddShape(shape);
        }

        igesWriter.ComputeModel();

        return igesWriter.Write(igesFile.c_str());
}

bool ReadSTEP(const Standard_CString &aFileName,
              TopTools_HSequenceOfShape &aHSequenceOfShape) {
        // aHSequenceOfShape.Clear();

        // create additional log file
        STEPControl_Reader aReader;

        IFSelect_ReturnStatus status = aReader.ReadFile(aFileName);
        if (status != IFSelect_RetDone)
                return false;

        aReader.WS()->TransferReader()->TransientProcess()->SetTraceLevel(
            1); // increase default trace level

        Standard_Boolean failsonly = Standard_False;
        aReader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);

        // Root transfers
        Standard_Integer nbr = aReader.NbRootsForTransfer();

        aReader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);

        for (Standard_Integer n = 1; n <= nbr; n++) {
                /*Standard_Boolean ok =*/aReader.TransferRoot(n);
        }

        // Collecting resulting entities
        Standard_Integer nbs = aReader.NbShapes();
        if (nbs == 0) {
                return false;
        }
        for (Standard_Integer i = 1; i <= nbs; i++) {
                aHSequenceOfShape.Append(aReader.Shape(i));
        }

        return true;
}

// STEPControl_AsIs
bool SaveSTEP(const std::string &aFileName,
              TopTools_HSequenceOfShape &aHSequenceOfShape) {
        // CREATE THE WRITER

        STEPControl_Writer aWriter;

        IFSelect_ReturnStatus status;
        for (Standard_Integer i = 1; i <= aHSequenceOfShape.Length(); i++) {
                status = aWriter.Transfer(aHSequenceOfShape.Value(i),
                                          STEPControl_AsIs);
                if (status != IFSelect_RetDone)
                        return false;
        }
        status = aWriter.Write(aFileName.c_str());
        return true;
}

bool SaveBREP(const std::string &aFileName,
              TopTools_HSequenceOfShape &aHSequenceOfShape) {
        // CREATE THE WRITER

        std::filebuf aFileBuf;
        std::ostream aStream(&aFileBuf);
        if (!aFileBuf.open(aFileName, ios::out)) {
                return false;
        }

        for (Standard_Integer i = 1; i <= aHSequenceOfShape.Length(); i++) {
                BRepTools::Write(aHSequenceOfShape.Value(i), aStream);
        }
        return true;
}

bool OpenBREP(const std::string &aFileName,
              TopTools_HSequenceOfShape &aHSequenceOfShape) {
        TopoDS_Face aShape;

        std::filebuf aFileBuf;
        std::istream aStream(&aFileBuf);
        if (!aFileBuf.open(aFileName, ios::in)) {
                return false;
        }

        BRep_Builder aBuilder;
        BRepTools::Read(aShape, aStream, aBuilder);

        aHSequenceOfShape.Append(aShape);

        return true;
}

////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following codes.

JS_EXT_DATA_DECLARE()

///////////////////////////////////////////////////////////////////////////
// implement ImportIgesEx function

//#define GETFACEHASHCODES_FUNC_USAGE                                            \
//        "GetFaceHashCodes Usage: var res =  comx.occio.GetFaceHashCodes(db);"
// JS_EXT_FUNC_BEGIN(GetFaceHashCodes, 1, GETFACEHASHCODES_FUNC_USAGE) {
//         unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
//
//         ModelIO &rDB = (*(ModelIO *)((void *)ullDB));
//         string ret_val = rDB.GetFaceHashCodes();
//
//         JS_EXT_FUNC_ASSIGN_RET(ret_val);
// }
// JS_EXT_FUNC_END()
std::vector<int> splitAndConvertToInt(const std::string &input,
                                      char delimiter) {
        std::vector<int> result;
        std::stringstream ss(input);
        std::string token;

        // ʹ�� std::getline �ָ��ַ���
        while (std::getline(ss, token, delimiter)) {
                // ת��Ϊ���������ӵ� vector
                try {
                        int number = std::stoi(token); // ת��Ϊ����
                        result.push_back(number);
                } catch (const std::invalid_argument &e) {
                        std::cerr << "Invalid number: " << token << std::endl;
                } catch (const std::out_of_range &e) {
                        std::cerr << "Number out of range: " << token
                                  << std::endl;
                }
        }

        return result;
}
#define GETCOMPONMENTSTEP_FUNC_USAGE                                           \
        "GetComponentStep Usage: var res =  comx.occio.GetComponentStep(id, "  \
        "path);"
JS_EXT_FUNC_BEGIN(GetComponentStep, 3, GETCOMPONMENTSTEP_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        int id = JS_EXT_PARA(int, 1);
        string path = JS_EXT_PARA(string, 2);

        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        // rDB.CreateComponentStpById(id, path.c_str());

        // JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define GETFACEBREP_FUNC_USAGE                                                 \
        "GetFaceBrep Usage: var res =  comx.occio.GetFaceBrep(id, path);"
JS_EXT_FUNC_BEGIN(GetFaceBrep, 3, GETFACEBREP_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string StringID = JS_EXT_PARA(string, 1);
        string path = JS_EXT_PARA(string, 2);

        vector<int> Id = splitAndConvertToInt(StringID, ',');
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        rDB.CreateBrepById(Id, path.c_str());

        // JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

#define GETFACEPOINT_FUNC_USAGE                                                \
        "GetFacePoint Usage: var res =  comx.occio.GetFacePoint(id);"
JS_EXT_FUNC_BEGIN(GetFacePoint, 2, GETFACEPOINT_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        int id = JS_EXT_PARA(int, 1);

        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));
        double x = 0;
        double y = 0;
        double z = 0;
        rDB.CreateCenterById(id, x, y, z);
        string ret_val =
            "point:" + to_string(x) + " " + to_string(y) + " " + to_string(z);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

///////////////////////////////////////////////////////////////////////////
// implement ImportIgesEx function

#define IMPORTIGESEX_FUNC_USAGE                                                \
        "ImportIgesEx Usage: var res =  comx.occio.ImportIgesEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ImportIgesEx, 2, IMPORTIGESEX_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        string ret_val = ReadIGESEx(strFilename.c_str(), rDB);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

///////////////////////////////////////////////////////////////////////////
// implement ImportStepEx function

#define IMPORTSTEPEX_FUNC_USAGE                                                \
        "ImportStepEx Usage: var res =  comx.occio.ImportStepEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ImportStepEx, 2, IMPORTSTEPEX_FUNC_USAGE) {

        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        string ret_val = ReadSTEPEx(strFilename.c_str(), rDB);
        // cout << 11 << endl;
        // rDB.modelTrees = ret_val;
        // cout << rDB.modelTrees << endl;
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////
// implement ImportBRepEx function

#define IMPORTBREPEX_FUNC_USAGE                                                \
        "ImportBRepEx Usage: var res =  comx.occio.ImportBRepEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ImportBRepEx, 2, IMPORTBREPEX_FUNC_USAGE) {

        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        string ret_val = ReadBREPEx(strFilename.c_str(), rDB);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()

//////////////////////////////////////////////////////////////////////////
// implement ImportStlEx function

#define IMPORTSTLEX_FUNC_USAGE                                                 \
        "ImportStlEx Usage: var res =  comx.occio.ImportStlEx(db, "            \
        "filename);"
JS_EXT_FUNC_BEGIN(ImportStlEx, 2, IMPORTSTLEX_FUNC_USAGE) {

        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        string ret_val = ReadSTLEx(strFilename.c_str(), rDB);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
//////////////////////////////////////////////////////////////////////////
// implement ImportGltfEx function

#define IMPORTGLTFEX_FUNC_USAGE                                                \
        "ImportGltfEx Usage: var res =  comx.occio.ImportGltfEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ImportGltfEx, 2, IMPORTGLTFEX_FUNC_USAGE) {

        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        string ret_val = ReadGLTFEx(strFilename.c_str(), rDB);

        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportIges function

#define EXPORTIGESEX_FUNC_USAGE                                                \
        "ExportIgesEx Usage: var res =  comx.occio.ExportIgesEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ExportIgesEx, 2, EXPORTIGESEX_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        bool ret_val = WriteIGESEx(strFilename.c_str(), rDB);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportStep function

#define EXPORTSTEPEX_FUNC_USAGE                                                \
        "ExportStepEx Usage: var res =  comx.occio.ExportStepEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ExportStepEx, 2, EXPORTSTEPEX_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        bool ret_val = WriteSTEPEx(strFilename.c_str(), rDB);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportBRep function

#define EXPORTBREPEX_FUNC_USAGE                                                \
        "ExportBRepEx Usage: var res =  comx.occio.ExportBRepEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ExportBRepEx, 2, EXPORTBREPEX_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        cout << "ExportBRepEx" << endl;

        bool ret_val = WriteBREPEx(strFilename.c_str(), rDB);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportStl function

#define EXPORTSTLEX_FUNC_USAGE                                                 \
        "ExportStlEx Usage: var res =  comx.occio.ExportStlEx(db, "            \
        "filename);"
JS_EXT_FUNC_BEGIN(ExportStlEx, 2, EXPORTSTLEX_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        bool ret_val = WriteSTLEx(strFilename.c_str(), rDB);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportStl function

#define EXPORTGLTFEX_FUNC_USAGE                                                \
        "ExportGltfEx Usage: var res =  comx.occio.ExportGltfEx(db, "          \
        "filename);"
JS_EXT_FUNC_BEGIN(ExportGltfEx, 2, EXPORTGLTFEX_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);

        string strFilename = JS_EXT_PARA(string, 1);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));

        bool ret_val = WriteGLTFEx(strFilename.c_str(), rDB);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ImportIges function

#define IMPORTIGES_FUNC_USAGE                                                  \
        "ImportIges Usage: var res =  comx.occio.ImportIges(db, filename);"
JS_EXT_FUNC_BEGIN(ImportIges, 2, IMPORTIGES_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string strFilename = JS_EXT_PARA(string, 1);

        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        string ret_val = type_cast<string>(ReadIGES(strFilename.c_str(), rDB));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ImportStep function

#define IMPORTSTEP_FUNC_USAGE                                                  \
        "ImportStep Usage: var res =  comx.occio.ImportStep(db, filename);"
JS_EXT_FUNC_BEGIN(ImportStep, 2, IMPORTSTEP_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string strFilename = JS_EXT_PARA(string, 1);

        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        string ret_val = type_cast<string>(ReadSTEP(strFilename.c_str(), rDB));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportIges function

#define EXPORTIGES_FUNC_USAGE                                                  \
        "ExportIges Usage: var res =  comx.occio.ExportIges(db, filename);"
JS_EXT_FUNC_BEGIN(ExportIges, 2, EXPORTIGES_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string strFilename = JS_EXT_PARA(string, 1);

        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        string ret_val = type_cast<string>(SaveIges(strFilename, rDB));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement ExportStep function

#define EXPORTSTEP_FUNC_USAGE                                                  \
        "ExportStep Usage: var res =  comx.occio.ExportStep(db, filename);"
JS_EXT_FUNC_BEGIN(ExportStep, 2, EXPORTSTEP_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string strFilename = JS_EXT_PARA(string, 1);

        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        string ret_val = type_cast<string>(SaveSTEP(strFilename, rDB));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement SaveBrep function

#define SAVEBREP_FUNC_USAGE                                                    \
        "SaveBrep Usage: var res =  comx.occio.SaveBrep(db, filename);"
JS_EXT_FUNC_BEGIN(SaveBrep, 2, SAVEBREP_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string strFilename = JS_EXT_PARA(string, 1);

        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        string ret_val = type_cast<string>(SaveBREP(strFilename, rDB));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement OpenBrep function

#define OPENBREP_FUNC_USAGE                                                    \
        "OpenBrep Usage: var res =  comx.occio.OpenBrep(db, filename);"
JS_EXT_FUNC_BEGIN(OpenBrep, 2, OPENBREP_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        string strFilename = JS_EXT_PARA(string, 1);

        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        string ret_val = type_cast<string>(OpenBREP(strFilename, rDB));
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement Plus function

#define PLUS_FUNC_USAGE "Plus Usage: var res =  comx.occio.Plus(left, right);"
JS_EXT_FUNC_BEGIN(Plus, 2, PLUS_FUNC_USAGE) {
        // para_type p0 = JS_EXT_PARA(para_type, 0);
        double left = JS_EXT_PARA(double, 0);
        double right = JS_EXT_PARA(double, 1);

        string ret_val = type_cast<string>(left + right);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
// 0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
// please don't modify or delete the previous line codes.
#define GETFACEAREA_FUNC_USAGE "GetFaceArea Usage: comx_sdk.GetFaceArea(/*put your parameters list here*/);"

#define GETMAPOLDTONEWIDS_FUNC_USAGE "GetMapOldToNewIDs Usage: comx_sdk.GetMapOldToNewIDs(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(GetMapOldToNewIDs, 2, GETMAPOLDTONEWIDS_FUNC_USAGE)
{
    unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
    string filePath = JS_EXT_PARA(string, 1);

    ModelIO& rDB = (*(ModelIO*)((void*)ullDB));

    NCollection_IndexedDataMap<int, FaceProperty> newMap;
    rDB.GetFaceMap(newMap);

    std::vector<FaceSignature> loadedOldSigs = FaceFeatureManager::LoadFromJson(filePath);
    std::vector<FaceSignature> newSigs = FaceFeatureManager::ExtractFromMap(newMap);

    std::map<int, int> idMapping = FaceFeatureManager::MapOldToNewIDs(loadedOldSigs, newSigs);

    Json::Value jsonResult;
    for (auto const& [oldId, newId] : idMapping) {
        jsonResult[std::to_string(oldId)] = newId;
    }

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::string jsonString = Json::writeString(builder, jsonResult);

  
    JS_EXT_FUNC_ASSIGN_RET(jsonString);

}
JS_EXT_FUNC_END()


#define SAVEFACEFEATURE_FUNC_USAGE "SaveFaceFeature Usage: comx_sdk.SaveFaceFeature(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(SaveFaceFeature, 2, SAVEFACEFEATURE_FUNC_USAGE)
{
    unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
    string filePath = JS_EXT_PARA(string, 1);

    ModelIO& rDB = (*(ModelIO*)((void*)ullDB));
    
    NCollection_IndexedDataMap<int, FaceProperty> oldMap;
    rDB.GetFaceMap(oldMap);
    std::vector<FaceSignature> oldSigs = FaceFeatureManager::ExtractFromMap(oldMap);

    // 满足要求 1
    FaceFeatureManager::SaveToJson(oldSigs, filePath);
    cout << "已保存旧模型特征。" << endl;

}
JS_EXT_FUNC_END()


#define GETFACENORMAL_FUNC_USAGE "GetFaceNormal Usage: comx_sdk.GetFaceNormal(/*put your parameters list here*/);"
JS_EXT_FUNC_BEGIN(GetFaceNormal, 2, GETFACENORMAL_FUNC_USAGE)
{
        //Put your codes here
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        int id = JS_EXT_PARA(int, 1);

        ModelIO& rDB = (*(ModelIO*)((void*)ullDB));
        double x = 0;
        double y = 0;
        double z = 0;
        rDB.GetNormalById(id, x, y, z);
        string ret_val =
            "normal:" + to_string(x) + " " + to_string(y) + " " + to_string(z);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);

}
JS_EXT_FUNC_END()

JS_EXT_FUNC_BEGIN(GetFaceArea, 3, GETFACEAREA_FUNC_USAGE)
{
        //Put your codes here
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        int id = JS_EXT_PARA(int, 1);
        double area = JS_EXT_PARA(double, 2);

        ModelIO& rDB = (*(ModelIO*)((void*)ullDB));
        //if (rDB.GetFaceArea(id, area))
        if (rDB.CalculateBoundingCircleArea(id, area))
        {
            string ret_val = type_cast<string>(area);
            JS_EXT_FUNC_ASSIGN_RET(ret_val);
        }
        else
        {
            JS_EXT_FUNC_ASSIGN_RET(string("0"));
        }
        JS_EXT_FUNC_ASSIGN_RET(string("0"));
}

JS_EXT_FUNC_END()

JS_EXT_FUNC_BEGIN(GetFaceCircumcircleArea, 3, GETFACEAREA_FUNC_USAGE)
{
    std::cout << "调用了GetFaceCircumcircleArea" << std::endl;
    //Put your codes here
    unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
    int id = JS_EXT_PARA(int, 1);
    double area = JS_EXT_PARA(double, 2);

    ModelIO& rDB = (*(ModelIO*)((void*)ullDB));
    if (rDB.CalculateBoundingCircleArea(id, area))
    {
        string ret_val = type_cast<string>(area);
        std::cout << "计算的面积为" << ret_val << std::endl;
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
    }
    else
    {
        JS_EXT_FUNC_ASSIGN_RET(string("0"));
    }
    JS_EXT_FUNC_ASSIGN_RET(string("0"));
}

JS_EXT_FUNC_END()



#define GETFACESPOINT_FUNC_USAGE                                               \
        "GetFacesPoint Usage: comx.occio.GetFacesPoint(/*put your parameters " \
        "list here*/);"
JS_EXT_FUNC_BEGIN(GetFacesPoint, 2, GETFACESPOINT_FUNC_USAGE) {
    unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
    string StringID = JS_EXT_PARA(string, 1);

    ModelIO& rDB = (*(ModelIO*)((void*)ullDB));
    vector<int> Id = splitAndConvertToInt(StringID, ',');
    NCollection_IndexedDataMap<int, FaceProperty> faceMap;

    rDB.GetFaceMap(faceMap);
    vector<TopoDS_Face> faces;

    for (auto& elem : Id)
    {
        faces.push_back(faceMap.FindFromKey(elem).faceObj);
    }

    gp_Pnt point = ComputeFacesCentroid(faces);
    string ret_val =
        "point:" + to_string(point.X()) + " " + to_string(point.Y()) + " " + to_string(point.Z());
    JS_EXT_FUNC_ASSIGN_RET(ret_val);

}
JS_EXT_FUNC_END()

////////////////////////////////////////////////////////////////////////////
// please set your js.ext namespace in the following codes.

#define JS_EXT_NS "comx.occio"

////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the following codes.

JS_EXT_ENTRY_BEGIN()


JS_EXT_ENTRY(GetMapOldToNewIDs)
JS_EXT_ENTRY(SaveFaceFeature)
JS_EXT_ENTRY(GetFaceCircumcircleArea)
JS_EXT_ENTRY(GetFaceNormal)
JS_EXT_ENTRY(GetFaceArea)
JS_EXT_ENTRY(GetFacesPoint)
JS_EXT_ENTRY(ImportIgesEx) 
JS_EXT_ENTRY(ImportStepEx) 
JS_EXT_ENTRY(ImportBRepEx) 
JS_EXT_ENTRY(ImportStlEx) 
JS_EXT_ENTRY(ImportGltfEx)
JS_EXT_ENTRY(ExportIgesEx) 
JS_EXT_ENTRY(ExportStepEx) 
JS_EXT_ENTRY(ExportBRepEx) 
JS_EXT_ENTRY(ExportStlEx) 
JS_EXT_ENTRY(ExportGltfEx)
JS_EXT_ENTRY(ImportIges) 
JS_EXT_ENTRY(ImportStep) 
JS_EXT_ENTRY(ExportIges) 
JS_EXT_ENTRY(ExportStep) 
JS_EXT_ENTRY(SaveBrep)
                
JS_EXT_ENTRY(OpenBrep) 
JS_EXT_ENTRY(Plus) 
JS_EXT_ENTRY(GetFaceBrep) 
JS_EXT_ENTRY(GetFacePoint)
JS_EXT_ENTRY(GetComponentStep) 
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 26)
JS_EXT_FUNC_REG(GetMapOldToNewIDs)
JS_EXT_FUNC_REG(SaveFaceFeature)
JS_EXT_FUNC_REG(GetFaceCircumcircleArea)
JS_EXT_FUNC_REG(GetFaceNormal)
JS_EXT_FUNC_REG(GetFaceArea)
JS_EXT_FUNC_REG(GetFacesPoint) 
JS_EXT_FUNC_REG(ImportIgesEx)
                            
JS_EXT_FUNC_REG(ImportStepEx) 
JS_EXT_FUNC_REG(ImportBRepEx) 
JS_EXT_FUNC_REG(ImportStlEx)
                                
JS_EXT_FUNC_REG(ImportGltfEx) 
JS_EXT_FUNC_REG(ExportIgesEx) 
JS_EXT_FUNC_REG(ExportStepEx)
JS_EXT_FUNC_REG(ExportBRepEx) 
JS_EXT_FUNC_REG(ExportStlEx) 
JS_EXT_FUNC_REG(ExportGltfEx)
JS_EXT_FUNC_REG(ImportIges) 
JS_EXT_FUNC_REG(ImportStep) 
JS_EXT_FUNC_REG(ExportIges)
JS_EXT_FUNC_REG(ExportStep) 
JS_EXT_FUNC_REG(SaveBrep)
JS_EXT_FUNC_REG(OpenBrep) 
JS_EXT_FUNC_REG(Plus)
JS_EXT_FUNC_REG(GetFaceBrep)
JS_EXT_FUNC_REG(GetFacePoint)
JS_EXT_FUNC_REG(GetComponentStep)
JS_EXT_MAIN_END()