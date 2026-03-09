#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;
using namespace comx::napi;

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

#include <base/function.hxx>
#include <base/type_cast.hxx>
using namespace KMAS::type;

// Open Cascade library.
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimApI_MakeSphere.hxx>
#include <BRepTools.hxx>
#include <BRep_ListIteratorOfListOfCurveRepresentation.hxx>
#include <BRep_Tool.hxx>
#include <Brep_TEdge.hxx>
#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>
#include <Standard_TypeDef.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
// #include <BRepMesh.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Poly_Triangulation.hxx>
#include <TShort_Array1OfShortReal.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <Poly.hxx>
#include <Poly_Connect.hxx>
#include <Poly_Triangulation.hxx>

#include <Geom2dAdaptor.hxx>
#include <Geom2dAdaptor_curve.hxx>

#include <GeomAdaptor.hxx>
#include <GeomAdaptor_curve.hxx>

#include <BRepBuilderAPI_NurbsConvert.hxx>
#include <Geom2dConvert.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom_Plane.hxx>

#include <BRepLib_FindSurface.hxx>
#include <GeomConvert.hxx>
#include <Geom_BSplineSurface.hxx>

#include <BRepLib.hxx>
#include <GeomLib.hxx>

#include <AIS_Shape.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <CPnts_AbscissaPoint.hxx>

#include <IMeshTools_Parameters.hxx>
#include <Poly_Polygon3D.hxx>
#include <Quantity_Color.hxx>

#include <ModelIO/CurveAdaptiveDiscrete.h>
#include <ModelIO/ModelIO.h>

// TopTools_HSequenceOfShape g_aHSequenceOfShape;//new
// TopTools_HSequenceOfShape();

////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following codes.

JS_EXT_DATA_DECLARE()

///////////////////////////////////////////////////////////////////////////
// implement RenderToGLCache function

struct point_t {
        Standard_Real x, y, z;

      public:
        point_t(Standard_Real vx = 0., Standard_Real vy = 0.,
                Standard_Real vz = 0.)
            : x(vx), y(vy), z(vz) {}
};

template <typename T> void ExportArray2File(string fileName, T arr) {
        ofstream exportFile;
        exportFile.open(fileName.c_str(), ios::out);

        for (int i = 0; i < arr.size(); i++) {
                exportFile << arr[i] << endl;
        }
        exportFile.close();
}

void ComputeNormals(const TopoDS_Face &theFace,
                    const Handle(Poly_Triangulation) & theTris,
                    Poly_Connect &thePolyConnect,
                    vector<KMAS::vector3d_t> &ret) {
        if (theTris.IsNull() || theTris->HasNormals()) {
                return;
        }

        // take in face the surface location
        const TopoDS_Face aZeroFace =
            TopoDS::Face(theFace.Located(TopLoc_Location()));
        Handle(Geom_Surface) aSurf = BRep_Tool::Surface(aZeroFace);
        const Poly_Array1OfTriangle &aTriangles = theTris->Triangles();
        if (!theTris->HasUVNodes() || aSurf.IsNull()) {
                // compute normals by averaging triangulation normals sharing
                // the same vertex
                Poly::ComputeNormals(theTris);
                return;
        }

        const Standard_Real aTol = Precision::Confusion();
        // Handle(TShort_HArray1OfShortReal) aNormals = new
        // TShort_HArray1OfShortReal(1, theTris->NbNodes() * 3);
        const TColgp_Array1OfPnt2d &aNodesUV = theTris->UVNodes();
        Standard_Integer aTri[3];
        const TColgp_Array1OfPnt &aNodes = theTris->Nodes();
        gp_Dir aNorm;
        for (Standard_Integer aNodeIter = aNodes.Lower();
             aNodeIter <= aNodes.Upper(); ++aNodeIter) {
                // try to retrieve normal from real surface first, when UV
                // coordinates are available
                if (GeomLib::NormEstim(aSurf, aNodesUV.Value(aNodeIter), aTol,
                                       aNorm) > 1) {
                        if (thePolyConnect.Triangulation() != theTris) {
                                thePolyConnect.Load(theTris);
                        }

                        // compute flat normals
                        gp_XYZ eqPlan(0.0, 0.0, 0.0);
                        for (thePolyConnect.Initialize(aNodeIter);
                             thePolyConnect.More(); thePolyConnect.Next()) {
                                aTriangles(thePolyConnect.Value())
                                    .Get(aTri[0], aTri[1], aTri[2]);
                                const gp_XYZ v1(aNodes(aTri[1]).Coord() -
                                                aNodes(aTri[0]).Coord());
                                const gp_XYZ v2(aNodes(aTri[2]).Coord() -
                                                aNodes(aTri[1]).Coord());
                                const gp_XYZ vv = v1 ^ v2;
                                const Standard_Real aMod = vv.Modulus();
                                if (aMod >= aTol) {
                                        eqPlan += vv / aMod;
                                }
                        }
                        const Standard_Real aModMax = eqPlan.Modulus();
                        aNorm = (aModMax > aTol) ? gp_Dir(eqPlan) : gp::DZ();
                }

                KMAS::vector3d_t nor;
                nor.x = aNorm.X();
                nor.y = aNorm.Y();
                nor.z = aNorm.Z();

                ret.push_back(nor);
        }
}

bool IsPlanar(const TopoDS_Face &objFace) {
        Handle(Geom_Surface) surface = BRep_Tool::Surface(objFace);
        if (surface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
                return true;
        }

        return false;
}

bool CreateGlBuffer4Face(const TopoDS_Face &objFace, vector<double> &triangles,
                         vector<double> &triangles_normal) {
        TopLoc_Location location;

        opencascade::handle<Poly_Triangulation> triFace =
            BRep_Tool::Triangulation(objFace, location);

        if (triFace.IsNull()) {
                BRepMesh_IncrementalMesh(objFace, 0.01);
                triFace = BRep_Tool::Triangulation(objFace, location);

                if (triFace.IsNull()) {
                        return false;
                }
        }

        vector<KMAS::vector3d_t> normals;
        ComputeNormals(objFace, triFace, Poly_Connect(), normals);

        Standard_Integer nTriangles = triFace->NbTriangles();

        gp_Pnt vertex1;
        gp_Pnt vertex2;
        gp_Pnt vertex3;

        Standard_Integer nVertexIndex1 = 0;
        Standard_Integer nVertexIndex2 = 0;
        Standard_Integer nVertexIndex3 = 0;

        TColgp_Array1OfPnt nodes(1, triFace->NbNodes());
        Poly_Array1OfTriangle objTriangles(1, triFace->NbTriangles());

        nodes = triFace->Nodes();
        objTriangles = triFace->Triangles();

        for (Standard_Integer i = 1; i <= triFace->NbTriangles(); i++) {
                Poly_Triangle aTriangle = objTriangles.Value(i);

                aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);

                vertex1 = nodes.Value(nVertexIndex1)
                              .Transformed(location.Transformation());
                vertex2 = nodes.Value(nVertexIndex2)
                              .Transformed(location.Transformation());
                vertex3 = nodes.Value(nVertexIndex3)
                              .Transformed(location.Transformation());

                point_t pt1(vertex1.X(), vertex1.Y(), vertex1.Z());
                point_t pt2(vertex2.X(), vertex2.Y(), vertex2.Z());
                point_t pt3(vertex3.X(), vertex3.Y(), vertex3.Z());

                triangles.push_back(pt1.x);
                triangles.push_back(pt1.y);
                triangles.push_back(pt1.z);

                KMAS::vector3d_t nor1 = normals[nVertexIndex1 - 1];
                triangles_normal.push_back(nor1.x);
                triangles_normal.push_back(nor1.y);
                triangles_normal.push_back(nor1.z);

                triangles.push_back(pt2.x);
                triangles.push_back(pt2.y);
                triangles.push_back(pt2.z);

                KMAS::vector3d_t nor2 = normals[nVertexIndex2 - 1];
                triangles_normal.push_back(nor2.x);
                triangles_normal.push_back(nor2.y);
                triangles_normal.push_back(nor2.z);

                triangles.push_back(pt3.x);
                triangles.push_back(pt3.y);
                triangles.push_back(pt3.z);

                KMAS::vector3d_t nor3 = normals[nVertexIndex3 - 1];
                triangles_normal.push_back(nor3.x);
                triangles_normal.push_back(nor3.y);
                triangles_normal.push_back(nor3.z);
        }

        return true;
}

bool CreateGlBuffer4FaceEx(const TopoDS_Face &objFace, vector<double> &points,
                           vector<double> &vnormals, vector<int> &triangles) {

        TopLoc_Location location;

        opencascade::handle<Poly_Triangulation> triFace =
            BRep_Tool::Triangulation(objFace, location);

        if (triFace.IsNull()) {

                cout << "triFace is Null" << endl;
                BRepMesh_IncrementalMesh(objFace, 0.01);
                triFace = BRep_Tool::Triangulation(objFace, location);

                if (triFace.IsNull()) {
                        return false;
                }
        }

        vector<KMAS::vector3d_t> normals;
        ComputeNormals(objFace, triFace, Poly_Connect(), normals);
        vnormals.resize(normals.size() * 3);
        copy(&normals[0].x, &normals[0].x + normals.size() * 3, &vnormals[0]);

        Standard_Integer nTriangles = triFace->NbTriangles();

        Standard_Integer nVertexIndex1 = 0;
        Standard_Integer nVertexIndex2 = 0;
        Standard_Integer nVertexIndex3 = 0;

        TColgp_Array1OfPnt nodes(1, triFace->NbNodes());
        Poly_Array1OfTriangle objTriangles(1, triFace->NbTriangles());

        nodes = triFace->Nodes();

        points.resize(nodes.Size() * 3);
        for (int nid = 0; nid < nodes.Size(); ++nid) {
                gp_Pnt vertex =
                    nodes.Value(nid + 1).Transformed(location.Transformation());
                points[nid * 3 + 0] = vertex.X();
                points[nid * 3 + 1] = vertex.Y();
                points[nid * 3 + 2] = vertex.Z();
        }

        objTriangles = triFace->Triangles();

        triangles.clear();
        for (Standard_Integer i = 1; i <= triFace->NbTriangles(); i++) {
                Poly_Triangle aTriangle = objTriangles.Value(i);

                aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);

                triangles.push_back(nVertexIndex1 - 1);
                triangles.push_back(nVertexIndex2 - 1);
                triangles.push_back(nVertexIndex3 - 1);
        }

        return true;
}

bool CreateGlBuffer4FaceEdge(const TopoDS_Face &objFace, vector<double> &points,
                             vector<double> &vnormals, vector<int> &lines) {
        TopLoc_Location location;

        opencascade::handle<Poly_Triangulation> triFace =
            BRep_Tool::Triangulation(objFace, location);

        if (triFace.IsNull()) {
                BRepMesh_IncrementalMesh(objFace, 0.01);
                triFace = BRep_Tool::Triangulation(objFace, location);

                if (triFace.IsNull()) {
                        return false;
                }
        }

        vector<KMAS::vector3d_t> normals;
        ComputeNormals(objFace, triFace, Poly_Connect(), normals);
        vnormals.resize(normals.size() * 3);
        copy(&normals[0].x, &normals[0].x + normals.size() * 3, &vnormals[0]);

        Standard_Integer nTriangles = triFace->NbTriangles();

        Standard_Integer nVertexIndex1 = 0;
        Standard_Integer nVertexIndex2 = 0;
        Standard_Integer nVertexIndex3 = 0;

        TColgp_Array1OfPnt nodes(1, triFace->NbNodes());
        Poly_Array1OfTriangle objTriangles(1, triFace->NbTriangles());

        nodes = triFace->Nodes();

        points.resize(nodes.Size() * 3);
        for (int nid = 0; nid < nodes.Size(); ++nid) {
                gp_Pnt vertex =
                    nodes.Value(nid + 1).Transformed(location.Transformation());
                points[nid * 3 + 0] = vertex.X();
                points[nid * 3 + 1] = vertex.Y();
                points[nid * 3 + 2] = vertex.Z();
        }

        objTriangles = triFace->Triangles();

        lines.clear();

        for (Standard_Integer i = 1; i <= triFace->NbTriangles(); i++) {
                Poly_Triangle aTriangle = objTriangles.Value(i);

                aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);

                // triangles.push_back(nVertexIndex1 - 1);
                // triangles.push_back(nVertexIndex2 - 1);
                // triangles.push_back(nVertexIndex3 - 1);

                lines.push_back(nVertexIndex1 - 1);
                lines.push_back(nVertexIndex2 - 1);

                lines.push_back(nVertexIndex2 - 1);
                lines.push_back(nVertexIndex3 - 1);

                lines.push_back(nVertexIndex3 - 1);
                lines.push_back(nVertexIndex1 - 1);
        }

        return true;
}

void CreateGlBuffer4Edge(const TopoDS_Edge &objEdge, vector<double> &lines) {
        Standard_Real firstParam, lastParam;
        TopLoc_Location location;
        Handle(Geom_Curve) curve3D =
            BRep_Tool::Curve(objEdge, location, firstParam, lastParam);

        TopAbs_ShapeEnum t = objEdge.ShapeType();

        lines.clear();
        if (curve3D.IsNull()) {
                return;
        }

        GeomAdaptor_Curve adaptedCurve(curve3D);

        int LINE_SEG_NUM = 100;
        Standard_Real rParaStep = (lastParam - firstParam) / LINE_SEG_NUM;

        for (Standard_Integer i = 0; i < LINE_SEG_NUM; ++i) {
                gp_Pnt firstPoint, lastPoint;
                Standard_Real p1 = firstParam + i * rParaStep;
                Standard_Real p2 = firstParam + (i + 1) * rParaStep;

                adaptedCurve.D0(p1, firstPoint);
                adaptedCurve.D0(p2, lastPoint);

                firstPoint = firstPoint.Transformed(location.Transformation());
                lastPoint = lastPoint.Transformed(location.Transformation());

                lines.push_back(firstPoint.X());
                lines.push_back(firstPoint.Y());
                lines.push_back(firstPoint.Z());

                lines.push_back(lastPoint.X());
                lines.push_back(lastPoint.Y());
                lines.push_back(lastPoint.Z());
        }
}

void GetGlBuffer4Edge(const TopoDS_Edge &E, vector<double> &points,
                      vector<int> &lines) {
        TopLoc_Location L;
        opencascade::handle<Poly_Polygon3D> edge = BRep_Tool::Polygon3D(E, L);

        if (edge.IsNull()) {
                return;
        }
}

void CreateGlBuffer4EdgeEx(const TopoDS_Edge &objEdge, vector<double> &points,
                           vector<int> &lines) {
        Standard_Real firstParam, lastParam;
        TopLoc_Location location;
        Handle(Geom_Curve) curve3D =
            BRep_Tool::Curve(objEdge, location, firstParam, lastParam);

        TopAbs_ShapeEnum t = objEdge.ShapeType();

        points.clear();
        lines.clear();
        if (curve3D.IsNull()) {
                return;
        }

        GeomAdaptor_Curve adaptedCurve(curve3D);

        int LINE_SEG_NUM = 100;
        Standard_Real rParaStep = (lastParam - firstParam) / LINE_SEG_NUM;

        for (Standard_Integer i = 0; i <= LINE_SEG_NUM; ++i) {
                gp_Pnt pt;
                Standard_Real para = firstParam + i * rParaStep;

                adaptedCurve.D0(para, pt);

                pt = pt.Transformed(location.Transformation());

                points.push_back(pt.X());
                points.push_back(pt.Y());
                points.push_back(pt.Z());

                if (i < LINE_SEG_NUM) {
                        lines.push_back(i);
                        lines.push_back(i + 1);
                }
        }
}

TopTools_IndexedMapOfShape g_mapFace;
TopTools_IndexedMapOfShape g_mapEdge;

///////////////////////////////////////////////////////////////////////////
// implement RenderToBuffer function

#define RenderToBufferEx_FUNC_USAGE                                            \
        "RenderToBufferEx Usage: var res =  "                                  \
        "comx.occrender.RenderToBufferEx(db);"
JS_EXT_FUNC_BEGIN(RenderToBufferEx, 3, RenderToBufferEx_FUNC_USAGE) {
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        ModelIO &rDB = (*(ModelIO *)((void *)ullDB));
        g_mapEdge.Clear();
        g_mapFace.Clear();

        bool faceFlag = JS_EXT_PARA(bool, 1);
        bool edgeFlag = JS_EXT_PARA(bool, 2);

        uint32_t arr_idx = 0;

        Napi::Array ret = Napi::Array::New(info.Env());
        Napi::Object retObj = ret.As<Napi::Object>();

        NCollection_IndexedDataMap<int, FaceProperty> faceMap;
        rDB.GetFaceMap(faceMap);

        for (NCollection_IndexedDataMap<int, FaceProperty>::Iterator
                 aKeyFaceIter(faceMap);
             aKeyFaceIter.More(); aKeyFaceIter.Next()) {
                int faceId = aKeyFaceIter.Key();
                FaceProperty faceProp = aKeyFaceIter.Value();

                if (faceFlag) {
                        Napi::Value tri_buf = GL::createBufferEx(
                            info.Env(), 2025, faceId, faceProp.points,
                            faceProp.normals, faceProp.elements, {}, {}, {}, {},
                            {}, faceProp.red, faceProp.green, faceProp.blue);

                        retObj.Set(arr_idx++, tri_buf);
                }

                if (edgeFlag) {

                        for (auto it = faceProp.edgeProperties.begin();
                             it != faceProp.edgeProperties.end(); it++) {

                                vector<int> freeLines;
                                vector<int> innerLines;
                                vector<int> shareLines;

                                int edge_id = (*it).id;

                                if ((*it).edgeType == FreeEdge) {
                                        for (int i = 0;
                                             i < (*it).edges.size() - 1; i++) {
                                                freeLines.push_back(
                                                    (*it).edges[i]);
                                                freeLines.push_back(
                                                    (*it).edges[i + 1]);
                                        }
                                        Napi::Value freeLine_buf =
                                            GL::createBufferEx(
                                                info.Env(), 2026, edge_id,
                                                faceProp.points, {}, {}, {},
                                                freeLines, {}, {}, {}, 255, 0,
                                                0);

                                        retObj.Set(arr_idx++, freeLine_buf);

                                } else if ((*it).edgeType == InnerEdge) {
                                        for (int i = 0;
                                             i < (*it).edges.size() - 1; i++) {
                                                innerLines.push_back(
                                                    (*it).edges[i]);
                                                innerLines.push_back(
                                                    (*it).edges[i + 1]);
                                        }

                                        Napi::Value innerLine_buf =
                                            GL::createBufferEx(
                                                info.Env(), 2027, edge_id,
                                                faceProp.points, {}, {}, {},
                                                innerLines, {}, {}, {}, 255,
                                                255, 0);

                                        retObj.Set(arr_idx++, innerLine_buf);
                                } else {
                                        for (int i = 0;
                                             i < (*it).edges.size() - 1; i++) {
                                                shareLines.push_back(
                                                    (*it).edges[i]);
                                                shareLines.push_back(
                                                    (*it).edges[i + 1]);
                                        }

                                        Napi::Value shareLine_buf =
                                            GL::createBufferEx(
                                                info.Env(), 2028, edge_id,
                                                faceProp.points, {}, {}, {},
                                                shareLines, {}, {}, {}, 0, 255,
                                                255);

                                        retObj.Set(arr_idx++, shareLine_buf);
                                }
                        }
                }
        }

        return ret;
}
JS_EXT_FUNC_END()

///////////////////////////////////////////////////////////////////////////
// implement RenderToBuffer function

#define RenderToBuffer_FUNC_USAGE                                              \
        "RenderToBuffer Usage: var res =  comx.occrender.RenderToBuffer(db);"
JS_EXT_FUNC_BEGIN(RenderToBuffer, 1, RenderToBuffer_FUNC_USAGE) {

        cout << "Get into RenderToBuffer" << endl;
        unsigned long long ullDB = JS_EXT_PARA(unsigned long long, 0);
        TopTools_HSequenceOfShape &rDB =
            (*(TopTools_HSequenceOfShape *)((void *)ullDB));

        // cout << "info.Env(): " << info.Env() << endl;

        Napi::Array ret = Napi::Array::New(info.Env());
        Napi::Object retObj = ret.As<Napi::Object>();

        g_mapEdge.Clear();
        g_mapFace.Clear();

        uint32_t arr_idx = 0;

        int totalPointCounts = 0;
        int totalLineCounts = 0;

        Standard_Integer iNumOfShape = rDB.Length();

        for (int idx = 1; idx <= iNumOfShape; ++idx) {
                // TopoDS_Shape &objShape = rDB.ChangeValue(idx);
                // BRepTools::Clean(objShape);

                TopoDS_Shape &_objShape = rDB.ChangeValue(idx);
                BRepTools::Clean(_objShape);

                BRepBuilderAPI_Sewing aSewingTool;
                aSewingTool.Init();

                aSewingTool.Load(_objShape);
                aSewingTool.Perform();
                TopoDS_Shape objShape = aSewingTool.SewedShape();
                if (objShape.IsNull())
                        objShape = _objShape;
                BRepTools::Clean(objShape);

                BRepMesh_IncrementalMesh(objShape, 0.01, true, 0.2, false);

                TopExp::MapShapes(objShape, TopAbs_EDGE, g_mapEdge);
                TopExp::MapShapes(objShape, TopAbs_FACE, g_mapFace);

                TColStd_Array1OfInteger arrFaceCountOfEdge(1,
                                                           g_mapEdge.Extent());
                arrFaceCountOfEdge.Init(0);

                // Create Face

                for (TopExp_Explorer faceExp(objShape, TopAbs_FACE);
                     faceExp.More(); faceExp.Next()) {
                        const TopoDS_Face &objFace =
                            TopoDS::Face(faceExp.Current());

                        Standard_Integer face_id = g_mapFace.FindIndex(objFace);
                        string strFaceID = type_cast<string>(face_id);
                        string face_comments =
                            "render-cascade.face" + strFaceID;

                        int color[] = {255, 255, 0};

                        vector<double> points;
                        vector<double> normals;
                        vector<int> triangles;

                        if (!CreateGlBuffer4FaceEx(objFace, points, normals,
                                                   triangles)) {
                                continue;
                        }

                        Napi::Value tri_buf = GL::createBuffer(
                            info.Env(), 2025, face_id, points, normals,
                            triangles, {}, {}, {}, {}, {});

                        retObj.Set(arr_idx++, tri_buf);

                        for (TopExp_Explorer edgeExp(faceExp.Current(),
                                                     TopAbs_EDGE);
                             edgeExp.More(); edgeExp.Next()) {
                                arrFaceCountOfEdge(
                                    g_mapEdge.FindIndex(edgeExp.Current()))++;
                        }
                }

                // Create Trangles Edge
                int flag = 1;
                for (TopExp_Explorer faceExp(objShape, TopAbs_FACE);
                     faceExp.More(); faceExp.Next()) {
                        const TopoDS_Face &objFace =
                            TopoDS::Face(faceExp.Current());

                        Standard_Integer edge_id =
                            flag + g_mapFace.FindIndex(objFace);
                        flag++;
                        string strEdgeID = type_cast<string>(edge_id);
                        string face_comments =
                            "render-cascade.edge" + strEdgeID;

                        int color[] = {255, 255, 0};

                        vector<double> points;
                        vector<double> normals;
                        vector<int> lines;

                        if (!CreateGlBuffer4FaceEdge(objFace, points, normals,
                                                     lines)) {
                                continue;
                        }

                        Napi::Value line_buf =
                            GL::createBuffer(info.Env(), 2026, edge_id, points,
                                             {}, {}, {}, lines, {}, {}, {});

                        // retObj.Set(arr_idx++, line_buf);
                }

                for (TopExp_Explorer edgeExp(objShape, TopAbs_EDGE);
                     edgeExp.More(); edgeExp.Next()) {
                        const TopoDS_Edge &objEdge =
                            TopoDS::Edge(edgeExp.Current());

                        vector<double> points;
                        vector<int> lines;

                        GetGlBuffer4Edge(objEdge, points, lines);
                }

                // Create Shape Edge

                for (Standard_Integer idx_edge = 1;
                     idx_edge <= g_mapEdge.Extent(); ++idx_edge) {
                        vector<double> points;
                        vector<int> lines;

                        BRepAdaptor_Curve adaptorCurve(
                            TopoDS::Edge(g_mapEdge(idx_edge)));

                        CurveAdaptiveDiscrete(adaptorCurve, points, lines,
                                              QuasiUniformDeflection);
                        Standard_Integer edge_id =
                            idx_edge + g_mapFace.Extent();

                        string strEdgeID = type_cast<string>(edge_id);
                        string edge_comments =
                            "render-cascade.face" + strEdgeID;

                        totalPointCounts += points.size() / 3;
                        totalLineCounts += lines.size() / 3;

                        if (lines.empty()) {
                                continue;
                        }

                        switch (arrFaceCountOfEdge(idx_edge)) {
                        case 0:
                                // Free Edge
                                {
                                        int color[] = {255, 0, 0};
                                        // JS_EXT_GL_CACHE_ENTRY_EX(&points[0],
                                        // NULL, points.size() / 3,
                                        //     NULL, NULL&triangles_prop[0], 0,
                                        //     &lines[0], NULL, lines.size() /
                                        //     2, NULL, NULL, 0,
                                        //     edge_comments.c_str(), 1, color,
                                        //     2026, edge_id, true);

                                        Napi::Value line_buf = GL::createBuffer(
                                            info.Env(), 2026, edge_id, points,
                                            {}, {}, {}, lines, {}, {}, {});

                                        retObj.Set(arr_idx++, line_buf);
                                }
                                break;
                        case 1: {
                                int color[] = {255, 255, 255};
                                // JS_EXT_GL_CACHE_ENTRY_EX(&points[0], NULL,
                                // points.size() / 3,
                                //     NULL, NULL&triangles_prop[0], 0,
                                //    &lines[0], NULL, lines.size() / 2,
                                //     NULL, NULL, 0,
                                //     edge_comments.c_str(), 1, color, 2027,
                                //     edge_id, true);

                                Napi::Value line_buf = GL::createBuffer(
                                    info.Env(), 2027, edge_id, points, {}, {},
                                    {}, lines, {}, {}, {});

                                retObj.Set(arr_idx++, line_buf);
                        }
                        // Border Edge
                        break;
                        default: // Shared Edge
                        {
                                int color[] = {5, 5, 5};
                                // JS_EXT_GL_CACHE_ENTRY_EX(&points[0], NULL,
                                // points.size() / 3,
                                //     NULL, NULL&triangles_prop[0], 0,
                                //     &lines[0], NULL, lines.size() / 2,
                                //     NULL, NULL, 0,
                                //     edge_comments.c_str(), 1, color, 2028,
                                //     edge_id, true);

                                Napi::Value line_buf = GL::createBuffer(
                                    info.Env(), 2028, edge_id, points, {}, {},
                                    {}, lines, {}, {}, {});

                                retObj.Set(arr_idx++, line_buf);
                        } break;
                        }
                }
        }

        return ret;
}
JS_EXT_FUNC_END()

///////////////////////////////////////////////////////////////////////////
// implement RenderToBufferGLTF function

#include <gltf/threeAnimation.hxx>
#include <gltf/threeMath.hxx>

using namespace three;

vector<GLTFMesh> gltfMeshes;

ofstream logFile;

vector<string> split(const string &str, string delim) {
        vector<string> res;
        if ("" == str)
                return res;

        char *strs = new char[str.length() + 1];
        strcpy_s(strs, strlen(str.c_str()) + 1, str.c_str());

        char *d = new char[delim.length() + 1];
        strcpy_s(d, strlen(delim.c_str()) + 1, delim.c_str());
        char *buf;
        char *p = strtok_s(strs, d, &buf);
        while (p) {
                string s = p;
                res.push_back(s);

                p = strtok_s(NULL, d, &buf);
        }
        return res;
}

bool readPropertyFile(string propFile) {
        ifstream inPropFile;
        inPropFile.open(propFile.c_str(), ios::in);

        if (!inPropFile.is_open()) {
                cout << "Open File: " << propFile << " Failed" << endl;
                return false;
        }

        while (!inPropFile.eof()) {
                string temp;
                inPropFile >> temp;

                vector<string> props;
                props = split(temp, ",");

                if (props.size() != 10) {
                        continue;
                }

                GLTFMesh _gltfMesh;
                _gltfMesh.id = props[0];
                _gltfMesh.name = props[1];
                _gltfMesh.uid = props[2];
                //_gltfMesh.duration = props[3];
                _gltfMesh.frameCounts = atoi(props[4].c_str());
                _gltfMesh.animationCounts = atoi(props[5].c_str());
                _gltfMesh.morphTargetInfluencesCounts = atoi(props[6].c_str());

                if (strcmp(props[7].c_str(), "true") == 0) {
                        _gltfMesh.morphTargetsRelative = true;
                } else {
                        _gltfMesh.morphTargetsRelative = false;
                }

                _gltfMesh.bindMode = props[8];
                _gltfMesh.bonesCounts = atoi(props[9].c_str());

                gltfMeshes.push_back(_gltfMesh);
        }

        return true;
}

#define RenderToBufferGLTF_FUNC_USAGE                                          \
        "RenderToBufferGLTF Usage: var res =  "                                \
        "comx.occrender.RenderToBufferGLTF(db);"
JS_EXT_FUNC_BEGIN(RenderToBufferGLTF, 1, RenderToBufferGLTF_FUNC_USAGE) {
        logFile.open("F:\\gltfLog.txt", ios::out);
        logFile << "Get into RenderToBuffer gltf" << endl;
        string cacheFileBaseName = JS_EXT_PARA(string, 0);
        Napi::Array ret = Napi::Array::New(info.Env());
        Napi::Object retObj = ret.As<Napi::Object>();
        uint32_t arr_idx = 0;
        Standard_Integer face_id = 1;

        gltfMeshes.clear();

        if (!readPropertyFile(cacheFileBaseName + ".prop")) {
                return ret;
        }

        for (unsigned int i = 0; i < gltfMeshes.size(); i++) {
                gltfMeshes[i].ReadCache(cacheFileBaseName);
                gltfMeshes[i].parseAnimation();
        }

        for (int idx = 0; idx < gltfMeshes.size(); ++idx) {
                // if (idx != 0) continue;
                Napi::Value tri_buf = GL::createBuffer(
                    info.Env(), 2025, face_id++,
                    gltfMeshes[idx].animations[0].frames[20].position,
                    gltfMeshes[idx].animations[0].frames[20].normals,
                    gltfMeshes[idx].index, {}, {}, {}, {}, {});

                retObj.Set(arr_idx++, tri_buf);
        }

        logFile.close();

        return ret;
}
JS_EXT_FUNC_END()

// 0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
// please don't modify or delete the previous line codes.

////////////////////////////////////////////////////////////////////////////
// please set your js.ext namespace in the following codes.

#define JS_EXT_NS "comx.occrender"

////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(RenderToBufferGLTF)
JS_EXT_ENTRY(RenderToBufferEx)
JS_EXT_ENTRY(RenderToBuffer)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 3)
JS_EXT_FUNC_REG(RenderToBufferGLTF)
JS_EXT_FUNC_REG(RenderToBufferEx)
JS_EXT_FUNC_REG(RenderToBuffer)
JS_EXT_MAIN_END()