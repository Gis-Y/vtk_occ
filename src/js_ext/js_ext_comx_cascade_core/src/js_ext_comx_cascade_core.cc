#include <comx_napi.hxx>
#include <type_cast.hxx>
using namespace KMAS::type;

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

#include <ModelIO/ModelIO.h>

TopTools_HSequenceOfShape
    g_aHSequenceOfShape; // new TopTools_HSequenceOfShape();
TopTools_IndexedMapOfShape g_mapFace;
TopTools_IndexedMapOfShape g_mapEdge;

NCollection_IndexedDataMap<TopoDS_Shape, Quantity_Color> aShapeGroup;

ModelIO g_modelIO;

////////////////////////////////////////////////////////////////////////////
// system pre-define segment, please don't modify the following codes.

JS_EXT_DATA_DECLARE()

///////////////////////////////////////////////////////////////////////////
// implement GetExEntry function

#define GETENTRYEX_FUNC_USAGE                                                  \
        "GetEntryEx Usage: var exEntry =  comx.occore.GetEntryEx();"
JS_EXT_FUNC_BEGIN(GetEntryEx, 0, GETENTRYEX_FUNC_USAGE) {
        unsigned long long ulRet = (unsigned long long)((void *)&g_modelIO);

        string ret_val = type_cast<string>(ulRet);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement GetEntry function

#define GETENTRY_FUNC_USAGE                                                    \
        "GetEntry Usage: var entry =  comx.occore.GetEntry();"
JS_EXT_FUNC_BEGIN(GetEntry, 0, GETENTRY_FUNC_USAGE) {
        // para_type p0 = JS_EXT_PARA(para_type, 0);

        unsigned long long ulRet =
            (unsigned long long)((void *)&g_aHSequenceOfShape);

        string ret_val = type_cast<string>(ulRet);
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement IsEmpty function

#define ISEMPTY_FUNC_USAGE                                                     \
        "IsEmpty Usage: var is_empty =  comx.occore.IsEmpty();"
JS_EXT_FUNC_BEGIN(IsEmpty, 0, ISEMPTY_FUNC_USAGE) {
        // para_type p0 = JS_EXT_PARA(para_type, 0);

        string ret_val = type_cast<string>(g_aHSequenceOfShape.IsEmpty());
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement Clear function

#define CLEAR_FUNC_USAGE "Clear Usage: comx.occore.Clear();"
JS_EXT_FUNC_BEGIN(Clear, 0, CLEAR_FUNC_USAGE) {
        // para_type p0 = JS_EXT_PARA(para_type, 0);

        g_aHSequenceOfShape.Clear();

        string ret_val = type_cast<string>("undefined");
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
///////////////////////////////////////////////////////////////////////////
// implement GetBrepInfo function

#define GetBrepInfo_FUNC_USAGE                                                 \
        "GetBrepInfo Usage: var res =  comx.occore.GetBrepInfo(face_id);"
JS_EXT_FUNC_BEGIN(GetBrepInfo, 1, GetBrepInfo_FUNC_USAGE) {
        Standard_Integer id = JS_EXT_PARA(Standard_Integer, 0);
        stringstream sstr;

        if (id > g_mapFace.Extent()) {
                const TopoDS_Shape &objEdge =
                    g_mapEdge.FindKey(id - g_mapFace.Extent());
                BRepTools::Write(objEdge, sstr);

        } else {
                const TopoDS_Shape &objFace = g_mapFace.FindKey(id);

                /*const TopoDS_Face &F = TopoDS::Face(objFace);
                Handle(Geom_Surface) S = BRep_Tool::Surface(F);

                Handle(Geom_BSplineSurface) BSurface =
                GeomConvert::SurfaceToBSplineSurface(S); BRep_Builder builder;
                TopoDS_Face Face;
                builder.MakeFace(Face, BSurface, 0.1);*/

                TopoDS_Face F = TopoDS::Face(objFace);
                BRepBuilderAPI_NurbsConvert nurbs(F);
                Handle(Geom_Surface) geom_extrusion =
                    BRepLib_FindSurface(nurbs).Surface();
                Handle(Geom_BSplineSurface) geombspline_extrusion =
                    GeomConvert::SurfaceToBSplineSurface(geom_extrusion);
                BRep_Builder builder;
                TopoDS_Face Face;
                builder.MakeFace(Face, geombspline_extrusion,
                                 Precision::Confusion());

                BRepTools::Write(/*objFace*/ Face, sstr);

                for (TopExp_Explorer edgeExp(objFace, TopAbs_EDGE);
                     edgeExp.More(); edgeExp.Next()) {
                        const TopoDS_Shape &objEdge = edgeExp.Current();
                        /*const TopoDS_Edge E = TopoDS::Edge(objEdge);
                        BRepBuilderAPI_NurbsConvert nurbs(E);
                        Handle(Geom_Curve) geom_extrusion = nurbs->*/

                        BRepTools::Write(objEdge, sstr);
                }
        }

        string ret_val = type_cast<string>(sstr.str());
        JS_EXT_FUNC_ASSIGN_RET(ret_val);
}
JS_EXT_FUNC_END()
// 0x4cc0c89c-0x0090-0x47e3-0xb7-0x63-0xc1-0xaf-0xf1-0xe7-0x99-0x4e
// please don't modify or delete the previous line codes.

////////////////////////////////////////////////////////////////////////////
// please set your js.ext namespace in the following codes.

#define JS_EXT_NS "comx.occore"

////////////////////////////////////////////////////////////////////////////
// entry segment, please replace your function name in the following codes.

JS_EXT_ENTRY_BEGIN()
JS_EXT_ENTRY(GetEntryEx)
JS_EXT_ENTRY(GetEntry)
JS_EXT_ENTRY(IsEmpty)
JS_EXT_ENTRY(Clear)
JS_EXT_ENTRY(GetBrepInfo)
JS_EXT_ENTRY_END()

JS_EXT_MAIN_BEGIN(JS_EXT_NS, 5)
JS_EXT_FUNC_REG(GetEntryEx)
JS_EXT_FUNC_REG(GetEntry)
JS_EXT_FUNC_REG(IsEmpty)
JS_EXT_FUNC_REG(Clear)
JS_EXT_FUNC_REG(GetBrepInfo)
JS_EXT_MAIN_END()