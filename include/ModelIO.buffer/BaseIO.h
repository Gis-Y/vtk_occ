#ifndef BASEIO_H
#define BASEIO_H

// Presentation

#ifdef new
#undef new
#endif // new

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <random>

#include <NCollection_IndexedDataMap.hxx>
#include <NCollection_Map.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDocStd_Document.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <Quantity_Color.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_NameOfColor.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_Integer.hxx>
#include <TopoDS_TShape.hxx>
#include <IMeshTools_Parameters.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

#include <TColStd_HSequenceOfExtendedString.hxx>
#include <XCAFDoc_LayerTool.hxx>
#include <XCAFDoc_VisMaterialTool.hxx>
#include <XCAFPrs_IndexedDataMapOfShapeStyle.hxx>

#include <TopLoc_IndexedMapOfLocation.hxx>

#include <TNaming_NamedShape.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_GraphNode.hxx>
#include <XCAFPrs_Style.hxx>
#include <NCollection_Sequence.hxx>


#include <TDocStd_Document.hxx>
#include <STEPCAFControl_Reader.hxx>

//#include <XCAFApp_Application.hxx>
//#include <PrsMgr_PresentationManager.hxx>
//#include <Standard_DefineAlloc.hxx>
//#include <AIS_InteractiveContext.hxx>
//#include <AIS_ColoredShape.hxx>
//#include <StdPrs_ToolTriangulatedShape.hxx>
//#include <AIS_ColoredShape.hxx>
//#include <XCAFPrs_AISObject.hxx>
//#include <AIS_ColoredShape.hxx>
//#include <XCAFPrs.hxx>

//#include "GTree.h"
#include "tcl_5.0.6/tree.h"

#include "jsoncpp/json/json.h"

#include "MeshIO_md5.h"
#include "OccBufferFileIO.h"

using namespace std;
using namespace tcl;
using namespace Json;

// Upper Bound Max 2147483647
#define UPPERBOUND		99999
#define FACEOFFSET	   100000
#define SHAPEOFFSET    900000

#define HUESTEP			360
#define SATURATIONSTEP	17
#define LIGHTNESSSTEP	19

//#define OUTFILEFLAG		1

#include <iostream>
#include <fstream>

bool fileExists(const std::string& fileName) {
        std::ifstream infile(fileName);
        return infile.good();
}

enum TreeType
{
	TreeShape,
	TreeFace
};

class FaceProperty
{
public:
	FaceProperty()
	{
		red = 0;
		green = 0;
		blue = 255;
		label = "Null";
		name = "NoName";
	}

	int red;
	int green;
	int blue;
	//int alpha;

	int hashCode;

	TopoDS_Face faceObj;

	//unsigned long long faceAddress;

	string label;
	string name;

	vector<double> points;
	vector<double> normals;
	vector<int> elements;
	set< vector<int> > edges;
public:
	void BuildPoints(const TColgp_Array1OfPnt &aNodes, const TopLoc_Location &location)
	{
		//for (TColgp_Array1OfPnt::Iterator anIter(aNodes); anIter.More(); anIter.Next())
		for (int i = 1; i <= aNodes.Length(); i++)
		{
			gp_Pnt vertex = aNodes.Value(i).Transformed(location.Transformation());

			points.push_back(vertex.X());
			points.push_back(vertex.Y());
			points.push_back(vertex.Z());
		}
	}

	void BuildElements(const Poly_Array1OfTriangle &aTri)
	{
		for (Standard_Integer i = 1; i <= aTri.Length(); i++)
		{
			Standard_Integer nVertexIndex1 = 0;
			Standard_Integer nVertexIndex2 = 0;
			Standard_Integer nVertexIndex3 = 0;

			Poly_Triangle aTriangle = aTri.Value(i);
			aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);

			// -1是为了从1开始变为0开始
			elements.push_back(nVertexIndex1 - 1);
			elements.push_back(nVertexIndex2 - 1);
			elements.push_back(nVertexIndex3 - 1);
		}
	}
	void AddEdge(const TColStd_Array1OfInteger &aEdge)
	{
		vector<int> _edge;
		for (TColStd_Array1OfInteger::Iterator anIter(aEdge); anIter.More(); anIter.Next())
		{
			_edge.push_back(anIter.Value() - 1);
		}
		edges.insert(_edge);
	}

	void SetColor(Quantity_Color& aColor)
	{
		red = int(aColor.Red() * 255);
		green = int(aColor.Green() * 255);
		blue = int(aColor.Blue() * 255);
	}


	void SetFaceLabel(const TDF_Label& _label)
	{
		TCollection_AsciiString entry;
		TDF_Tool::Entry(_label, entry);
		label = entry.ToCString();
	}

	void ComputeNormals()
	{
		normals.resize(points.size());

		for (unsigned int i = 0, il = elements.size(); i < il; i += 3) {

			// elements从1开始，而不是0，所以-1
			int vA = elements[i + 0];
			int vB = elements[i + 1];
			int vC = elements[i + 2];

			if (vA == 0 || vB == 0 || vC == 0)
			{
				int a = 1;
			}

			gp_Vec pA(points[vA * 3], points[vA * 3 + 1], points[vA * 3 + 2]);
			gp_Vec pB(points[vB * 3], points[vB * 3 + 1], points[vB * 3 + 2]);
			gp_Vec pC(points[vC * 3], points[vC * 3 + 1], points[vC * 3 + 2]);

			gp_Vec cb = pC - pB;
			gp_Vec ab = pA - pB;
			cb = cb ^ ab;

			gp_Vec nA(normals[vA * 3], normals[vA * 3 + 1], normals[vA * 3 + 2]);
			gp_Vec nB(normals[vB * 3], normals[vB * 3 + 1], normals[vB * 3 + 2]);
			gp_Vec nC(normals[vC * 3], normals[vC * 3 + 1], normals[vC * 3 + 2]);

			nA = nA + cb;
			nB = nB + cb;
			nC = nC + cb;

			normals[vA * 3] = nA.X();
			normals[vA * 3 + 1] = nA.Y();
			normals[vA * 3 + 2] = nA.Z();

			normals[vB * 3] = nB.X();
			normals[vB * 3 + 1] = nB.Y();
			normals[vB * 3 + 2] = nB.Z();

			normals[vC * 3] = nC.X();
			normals[vC * 3 + 1] = nC.Y();
			normals[vC * 3 + 2] = nC.Z();
		}

		for (unsigned int i = 0, il = normals.size(); i < il; i += 3) {
			gp_XYZ _nor(normals[i], normals[i + 1], normals[i + 2]);

			const Standard_Real aModMax = _nor.Modulus();
			gp_Dir aNorm = (aModMax > 1e-6) ? gp_Dir(_nor) : gp::DZ();

			normals[i] = aNorm.X();
			normals[i + 1] = aNorm.Y();
			normals[i + 2] = aNorm.Z();
		}
	}

	void Dump()
	{
		cout << "Face Property: " << endl;
		cout << "Color Red  : " << red << endl;
		cout << "Color Green: " << green << endl;
		cout << "Color Blue : " << blue << endl;
		cout << "HashCode   : " << hashCode << endl;
		cout << "points size: " << points.size() << endl;
		cout << "elems  size: " << elements.size() << endl;
		cout << "edges  size: " << edges.size() << endl;
		cout << endl;
	}
};

class TreeLabel
{
public:
	TreeLabel()
	{
		HashCode = 0;
		Name = "Model";
	}
	TreeLabel(const TopoDS_Shape &_Shape, TreeType _NodeType = TreeShape, int _Level = 0) :
		Shape(_Shape), NodeType(_NodeType), Level(_Level)
	{
		if (_NodeType == TreeShape)
		{
			HashCode = Shape.HashCode(UPPERBOUND) + SHAPEOFFSET;
		}
		else
		{
			HashCode = Shape.HashCode(UPPERBOUND) + FACEOFFSET;
		}

		Name = GetName();
	}
	TreeLabel(const TDF_Label &_Label, const TopoDS_Shape &_Shape, TreeType _NodeType = TreeShape, int _Level = 0) :
		Label(_Label), Shape(_Shape), NodeType(_NodeType), Level(_Level)
	{
		if (_NodeType == TreeShape)
		{
			HashCode = Shape.HashCode(UPPERBOUND) + SHAPEOFFSET;
		}
		else
		{
			HashCode = Shape.HashCode(UPPERBOUND) + FACEOFFSET;
		}
		Name = GetName();
	}

	TDF_Label Label;
	TopoDS_Shape Shape;
	TreeType NodeType;
	int Level;
	int HashCode;
	string Name;
	vector<int> subFaceHashCodes;

	friend bool operator < (const TreeLabel& lhs, const TreeLabel& rhs)
	{
		return lhs.HashCode < rhs.HashCode;
	}
	friend bool operator == (const TreeLabel& lhs, const TreeLabel& rhs)
	{
		return lhs.HashCode == rhs.HashCode;
	}

	string GetName()
	{
		TCollection_ExtendedString str = "";

		if (Label.IsNull() && Shape.IsNull())
		{
			str = "Model";
		}

		if (!Label.IsNull())
		{
			Handle(TDataStd_Name) anAttribute;

			if (Label.FindAttribute(TDataStd_Name::GetID(), anAttribute))
			{
				str = anAttribute->Get();
			}
		}

		if (str == "")
		{
			switch (Shape.ShapeType())
			{
			case TopAbs_COMPOUND:
				str = "Compound";
				break;
			case TopAbs_COMPSOLID:
				str = "CSolid";
				break;
			case TopAbs_SOLID:
				str = "Solid";
				break;
			case  TopAbs_SHELL:
				str = "Shell";
				break;
			case TopAbs_FACE:
				str = "Face";
				break;
			case TopAbs_WIRE:
				str = "Wire";
				break;
			case TopAbs_EDGE:
				str = "Edge";
				break;
			case TopAbs_VERTEX:
				str = "Vertex";
				break;
			case TopAbs_SHAPE:
				str = "Shape";
				break;
			default:
				break;
			}
		}

		char* ch = new char[str.LengthOfCString() + 1];
		str.ToUTF8CString(ch);
		return ch;
	}

};

class RandomColorGenerator
{
public:
	RandomColorGenerator() {
		Ref = 0;
		for (int i = 1; i <= HUESTEP; i++)
		{
			HueSamplers.push_back((83 * i) % HUESTEP);
		}

		// 饱和度 0.0 - 1.0
		for (int i = 1; i <= SATURATIONSTEP; i++)
		{
			double randomNum = (7 * i) % SATURATIONSTEP;
			SaturationSamplers.push_back(0.5 * randomNum / SATURATIONSTEP + 0.5);
		}

		// 亮度 0.2 - 0.8
		for (int i = 1; i <= LIGHTNESSSTEP; i++)
		{
			int randomNum = (11 * i) % LIGHTNESSSTEP;
			LightSamplers.push_back(0.5 * randomNum / LIGHTNESSSTEP + 0.5);
		}

		// #ifdef OUTFILEFLAG
				// outfile.open("E:\\colorDefine.log", ios::out);
				// outfile << "\nHueSamplers:" << endl;
				// for (int i = 0; i < HueSamplers.size(); i++)
				// {
					// outfile << HueSamplers[i] << endl;
				// }
				// outfile << "\nLightSamplers:" << endl;
				// for (int i = 0; i < LightSamplers.size(); i++)
				// {
					// outfile << LightSamplers[i] << endl;
				// }
				// outfile << "\nSaturationSamplers:" << endl;
				// for (int i = 0; i < SaturationSamplers.size(); i++)
				// {
					// outfile << SaturationSamplers[i] << endl;
				// }
				// outfile << "\nGet Color:" << endl;
				// outfile.close();
		// #endif

	}
	~RandomColorGenerator() {}

	Quantity_Color GetColor()
	{
		int indexHue = Ref % HUESTEP;
		int indexSaturation = Ref % SATURATIONSTEP;
		int indexLight = Ref % LIGHTNESSSTEP;
		// #ifdef OUTFILEFLAG
				// outfile.open("E:\\colorDefine.log", ios::app);
				// outfile << "Ref: " << Ref << endl;
				// outfile << "HueSamplers[indexHue]: " << HueSamplers[indexHue] << endl;
				// outfile << "LightSamplers[indexLight]: " << LightSamplers[indexLight] << endl;
				// outfile << "SaturationSamplers[indexSaturation]: " << SaturationSamplers[indexSaturation] << endl;
				// outfile << endl;
				// outfile.close();
		// #endif // OUTFILEFLAG


		color.SetValues(HueSamplers[indexHue], LightSamplers[indexLight], SaturationSamplers[indexSaturation], Quantity_TOC_HLS);
		//color.SetValues(HueSamplers[indexHue], 1.0, 0.5, Quantity_TOC_HLS);
		//double R, G, B, H, L, S;
		//color.HlsRgb(HueSamplers[indexHue], 1.0, 0.5, R, G, B);

		//cout << R * 255 << "  " << G * 255 << "  " << B * 255 << endl;
		//cout << HueSamplers[indexHue] << "  " << 1.0 << "  " << 0.5 << endl;


		Ref++;

		return color;
	}

	void ResetRef()
	{
		Ref = 0;
	}

	void SetRefBegin(const int _Ref)
	{
		Ref = _Ref;
	}


private:
	int Ref;
	vector<int> HueSamplers;
	vector<double> SaturationSamplers;
	vector<double> LightSamplers;
	Quantity_Color color;

	//ofstream outfile;

};

class BaseIO
{
public:
	BaseIO()
	{
		aMeshParams.Deflection = 0.005;		//For Fine Model: 0.02
		aMeshParams.Angle = 0.3;
		aMeshParams.Relative = Standard_True;
		aMeshParams.InParallel = Standard_True;
		aMeshParams.MinSize = Precision::Confusion();
		aMeshParams.InternalVerticesMode = Standard_True;
		aMeshParams.ControlSurfaceDeflection = Standard_True;
		//aMeshParams.MeshAlgo = IMeshTools_MeshAlgoType_Delabella;

		defaultCurveColor.SetValues(1.00000000f, 1.00000000f, 1.00000000f, 1.00000000f);
		defaultSurfColor.SetValues(0.644479692f, 0.644479692f, 1.00000000f, 1.00000000f);
	}
	~BaseIO() {}

	//virtual void Read(string fileName) = 0;
	void Perform(string filepath, string bufFilepath = "");
	void GetFaceMap(NCollection_IndexedDataMap<int, FaceProperty> &_faceMap);
	string GetFaceHashCodes();
	void GetTree(tree<TreeLabel>& Tree);
	string GetJson(bool faceFlag = true);

protected:
	Handle(TDocStd_Document) doc;
	Handle(XCAFDoc_ShapeTool) ST;
	Handle(XCAFDoc_ColorTool) CT;
	IMeshTools_Parameters aMeshParams;
	Quantity_ColorRGBA defaultCurveColor;
	Quantity_ColorRGBA defaultSurfColor;
	//GTree<GTreeLabel> AssemlyTree;
	tree<TreeLabel> modelTree;

	RandomColorGenerator colorGen;

	XCAFPrs_IndexedDataMapOfShapeStyle aSettings;
	NCollection_IndexedDataMap<int, FaceProperty> faceMap;

	typedef NCollection_IndexedDataMap<TopoDS_Shape, tree<TreeLabel> *, TopTools_ShapeMapHasher> IndexedDataMapOfShapeTreeNode;
	typedef NCollection_IndexedDataMap<TopoDS_Shape, tree<TreeLabel> *, TopTools_ShapeMapHasher>::Iterator DataMapIteratorOfIndexedDataMapOfShapeTreeNode;

	TopoDS_Shape SewModel(const TopoDS_Shape& objShape);
	TCollection_ExtendedString GetLabelName(const TDF_Label &label);

	void FillComponentSolidColor(const TDF_Label& theLabel);

	void fillStyleColors(XCAFPrs_Style& theStyle,
		const Handle(XCAFDoc_ColorTool)& theTool,
		const TDF_Label& theLabel);

	Standard_Boolean getShapesOfSHUO(TopLoc_IndexedMapOfLocation& theaPrevLocMap,
		const Handle(XCAFDoc_ShapeTool)& theSTool,
		const TDF_Label& theSHUOlab,
		TopTools_SequenceOfShape& theSHUOShapeSeq);


	void CollectStyleSettings(const TDF_Label& theLabel,
		const TopLoc_Location& theLoc,
		XCAFPrs_IndexedDataMapOfShapeStyle& theSettings,
		IndexedDataMapOfShapeTreeNode& aShapeTreeNodeMapping,
		tree<TreeLabel>::iterator iter,
		const Quantity_ColorRGBA& theLayerColor = Quantity_ColorRGBA(Quantity_NOC_WHITE),
		int level = 1);

	void ParseTreeFaces(tree<TreeLabel>* treeNode);

	void BuildFaceData(const TopoDS_Shape& shape, const XCAFPrs_Style& style, tree<TreeLabel>* treeNode, std::vector<occ_perform_buffer_t>& buffers, bool exist, int &idx);

	void Node2Json(tree<TreeLabel>* node, Json::Value &jsonNode, bool faceFlag);


	void DumpTree(tree<TreeLabel> *Node);
	void DumpNode(tree<TreeLabel> *node);

	// ofstream outfile;

#ifdef OCCTEST
public:
	void Render();
	void SetAISInteractiveContext(Handle(AIS_InteractiveContext) _myAISContext);
	Handle(AIS_InteractiveContext) myAISContext;
#endif


};

inline void BaseIO::Perform(string filepath, string bufFilepath)
{
#ifdef OUTFILEFLAG
	ofstream outfile;
	outfile.open("E:\\colorMonitor.log", ios::out);
	outfile.close();
#endif

	colorGen.ResetRef();

	ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
	CT = XCAFDoc_DocumentTool::ColorTool(doc->Main());

	TDF_LabelSequence Labels;
	ST->GetFreeShapes(Labels);

	modelTree.clear();
	faceMap.Clear();

	// 处理不同Component，Solid不同颜色显示
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		FillComponentSolidColor(theLabel);
	}

        std::vector<occ_perform_buffer_t> buffers;
        auto md5_input_file = calculateMD5(filepath);

        bool buffer_not_need_update = false;

        if (bufFilepath != "" && fileExists(bufFilepath))
        {
                string md5;
                readOccPerformBuffer(buffers, md5, bufFilepath.c_str());

                buffer_not_need_update = true;

                if (md5_input_file != md5)
                {
                        buffer_not_need_update = false;
                        buffers.clear();
                }
        }

        bool bIsBufferExist = (buffers.size() != 0);

        int idx_global = 0;

	// 遍历所有的Free Label，构建Tree
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		BRepTools::Clean(objShape);

                if (!bIsBufferExist)
                {
                        BRepMesh_IncrementalMesh(objShape, aMeshParams);
                }

		TreeLabel _treeLabel(theLabel, ST->GetShape(theLabel), TreeShape, 1);

		tree<TreeLabel>::iterator it = modelTree.insert(_treeLabel);

		TopLoc_Location aLoc;

		IndexedDataMapOfShapeTreeNode aShapeTreeNodeMapping;
		CollectStyleSettings(theLabel, aLoc, aSettings, aShapeTreeNodeMapping, it, Quantity_ColorRGBA(Quantity_NOC_WHITE), 1);

		//for (XCAFPrs_DataMapIteratorOfIndexedDataMapOfShapeStyle aStyledShapeIter(aSettings);
		//	aStyledShapeIter.More(); aStyledShapeIter.Next())
		//{
		//	BuildFaceData(aStyledShapeIter.Key(), aStyledShapeIter.Value(), *(aShapeTreeNodeMapping.Seek(aStyledShapeIter.Key())));
		//}

		for (Standard_Integer anIter = aSettings.Extent(); anIter >= 1; --anIter)
		{
			BuildFaceData(aSettings.FindKey(anIter), aSettings.FindFromIndex(anIter), *(aShapeTreeNodeMapping.Seek(aSettings.FindKey(anIter))), buffers, bIsBufferExist, idx_global);
		}

		aSettings.Clear();
		aShapeTreeNodeMapping.Clear();
	}

	Labels.Clear();

        if (bufFilepath != "" && buffers.size() && ( (!fileExists(bufFilepath)) || !buffer_not_need_update) )
        {
                writeOccPerformBuffer(buffers, md5_input_file, bufFilepath.c_str());
        }
}

inline void BaseIO::FillComponentSolidColor(const TDF_Label& theLabel)
{
	TopoDS_Shape objShape;
	ST->GetShape(theLabel, objShape);

	TopExp_Explorer exp_solids;
	for (exp_solids.Init(objShape, TopAbs_SOLID); exp_solids.More(); exp_solids.Next())
	{
		TopoDS_Solid solid = TopoDS::Solid(exp_solids.Current());

	}
}

inline void BaseIO::GetFaceMap(NCollection_IndexedDataMap<int, FaceProperty> &_faceMap)
{
	_faceMap = faceMap;
}

inline string BaseIO::GetFaceHashCodes()
{
	vector<int> hashcodes;
	TDF_LabelSequence Labels;

	if (ST.IsNull())
	{
		return "[]";
	}

	ST->GetFreeShapes(Labels);

	// 遍历所有的Free Label，构建Tree
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		for (TopExp_Explorer faceExp(objShape, TopAbs_FACE); faceExp.More(); faceExp.Next())
		{
			TopoDS_Shape shapeChild = faceExp.Current();

			const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());
			hashcodes.push_back(objFace.HashCode(UPPERBOUND) + FACEOFFSET);
		}
	}

	Labels.Clear();

	std::stringstream ss;
	ss << "[";
	for (auto it = hashcodes.begin(); it != hashcodes.end(); ++it) {
		ss << *it;
		if (it != hashcodes.end() - 1) {
			ss << ", ";
		}
	}
	ss << "]";

	return ss.str();

}

inline void BaseIO::GetTree(tree<TreeLabel>& Tree)
{
	Tree = modelTree;
}
inline string BaseIO::GetJson(bool faceFlag)
{
	Json::Value jsonNode;
	Node2Json(&modelTree, jsonNode, faceFlag);

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";
	builder["emitUTF8"] = true; // 直接输出 UTF-8 字符
	//builder["indentation"] = ""; // 压缩格式，没有换行和不必要的空白字符
	//cout << Json::writeString(builder, jsonNode) << endl;
	return Json::writeString(builder, jsonNode);
}

inline void BaseIO::ParseTreeFaces(tree<TreeLabel>* treeNode)
{
	if (!treeNode->empty())
	{
		return;
	}

	const TopoDS_Shape shape = treeNode->get()->Shape;

	for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
	{
		TopoDS_Shape shapeChild = faceExp.Current();

		const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

		TreeLabel _treeLabel(objFace);
		treeNode->insert(_treeLabel);
	}
}
inline void BaseIO::BuildFaceData(const TopoDS_Shape& shape, const XCAFPrs_Style& style, tree<TreeLabel>* treeNode, std::vector<occ_perform_buffer_t>& buffers, bool bBufferExist, int &idx)
{

	Quantity_Color color = style.GetColorSurf();

#ifdef OUTFILEFLAG
	ofstream outfile;
	outfile.open("E:\\colorMonitor.log", ios::app);
	outfile << "BuildFaceData: ";
	outfile << color.Hue() << "  ";
	outfile << color.Light() << "  ";
	outfile << color.Saturation() << "  ";
	outfile << round(color.Red() * 255) << "  ";
	outfile << round(color.Green() * 255) << "  ";
	outfile << round(color.Blue() * 255) << "  ";
	outfile << endl;
	outfile.close();
#endif

        //cout << "bufFilepath is " << bufFilepath << endl;

        if (!bBufferExist)
        {
                // 遍历所有Face
                for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
                {
                        TopoDS_Shape shapeChild = faceExp.Current();

                        const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

                        TopLoc_Location location;
                        opencascade::handle<Poly_Triangulation> triFace = BRep_Tool::Triangulation(objFace, location);

                        TopoDS_Shape _shape = objFace;

                        if (triFace.IsNull())
                        {
                                IMeshTools_Parameters aMeshParams;
                                BRepMesh_IncrementalMesh(objFace, aMeshParams);

                                opencascade::handle<Poly_Triangulation> _triFace = BRep_Tool::Triangulation(objFace, location);
                                if (_triFace.IsNull())
                                {
                                        cout << "triFace is Null" << endl;
                                        continue;
                                }
                                else
                                {
                                        triFace = _triFace;
                                }
                        }

                        int hashCode = objFace.HashCode(UPPERBOUND) + FACEOFFSET;

                        // 这里是针对subshape定义颜色
                        FaceProperty *pFaceProp = faceMap.ChangeSeek(hashCode);
                        if (pFaceProp != NULL)
                        {
                                pFaceProp->SetColor(color);
                                continue;
                        }
                        //

                        FaceProperty faceProp;

                        faceProp.faceObj = objFace;

                        faceProp.SetColor(color);

                        faceProp.BuildPoints(triFace->Nodes(), location);
                        faceProp.BuildElements(triFace->Triangles());
                        faceProp.ComputeNormals();

                        // 将Face插入到节点
                        TreeLabel _node(objFace, TreeFace);
                        (*treeNode).get()->subFaceHashCodes.push_back(hashCode);
                        //(*treeNode).subFaceHashCodes.push_back(hashCode);
                        //_node.subFaceHashCodes.push_back(hashCode);
                        //cout << _node.GetName() << endl;
                        treeNode->insert(_node);

                        for (TopExp_Explorer edgeExp(objFace, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
                        {
                                const TopoDS_Edge &objEdge = TopoDS::Edge(edgeExp.Current());
                                opencascade::handle<Poly_PolygonOnTriangulation> triEdge = BRep_Tool::PolygonOnTriangulation(objEdge, triFace, location);

                                if (triEdge.IsNull())
                                {
                                        continue;
                                }

                                TColStd_Array1OfInteger nds = triEdge->Nodes();

                                faceProp.AddEdge(triEdge->Nodes());
                        }

                        faceMap.Add(hashCode, faceProp);

                        occ_perform_buffer_t buf;

                        buf.points = faceProp.points;
                        buf.edges = faceProp.edges;
                        buf.elements = faceProp.elements;
                        buf.normals = faceProp.normals;

                        buffers.push_back(buf);
                }
        }
        else
        {
                // 遍历所有Face
                for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
                {
                        TopoDS_Shape shapeChild = faceExp.Current();

                        const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

                        int hashCode = objFace.HashCode(UPPERBOUND) + FACEOFFSET;

                        // 这里是针对subshape定义颜色
                        FaceProperty *pFaceProp = faceMap.ChangeSeek(hashCode);
                        if (pFaceProp != NULL)
                        {
                                pFaceProp->SetColor(color);
                                continue;
                        }
                        //

                        FaceProperty faceProp;

                        faceProp.faceObj = objFace;

                        faceProp.SetColor(color);

                        occ_perform_buffer_t &buf = buffers[idx++];

                        faceProp.points = buf.points;
                        faceProp.edges = buf.edges;
                        faceProp.elements = buf.elements;
                        faceProp.normals = buf.normals;

                        // 将Face插入到节点
                        TreeLabel _node(objFace, TreeFace);
                        (*treeNode).get()->subFaceHashCodes.push_back(hashCode);
                        //(*treeNode).subFaceHashCodes.push_back(hashCode);
                        //_node.subFaceHashCodes.push_back(hashCode);
                        //cout << _node.GetName() << endl;
                        treeNode->insert(_node);

                        faceMap.Add(hashCode, faceProp);
                }
        }
}

inline TopoDS_Shape BaseIO::SewModel(const TopoDS_Shape& objShape)
{
	BRepBuilderAPI_Sewing aSewingTool;
	aSewingTool.Init(0.5);

	aSewingTool.Load(objShape);
	aSewingTool.Perform();
	TopoDS_Shape aShape = aSewingTool.SewedShape();
	if (aShape.IsNull())
		aShape = objShape;
	return aShape;
}

inline TCollection_ExtendedString BaseIO::GetLabelName(const TDF_Label &label)
{
	if (label.IsNull())
	{
		return "The Label is Null.";
	}

	Handle(TDataStd_Name) anAttribute;

	TCollection_ExtendedString str;

	if (label.FindAttribute(TDataStd_Name::GetID(), anAttribute))
	{
		str = anAttribute->Get();
	}

	return str;
}

inline void BaseIO::fillStyleColors(XCAFPrs_Style& theStyle,
	const Handle(XCAFDoc_ColorTool)& theTool,
	const TDF_Label& theLabel)
{
	Quantity_ColorRGBA aColor;
	if (theTool->GetColor(theLabel, XCAFDoc_ColorGen, aColor))
	{
		theStyle.SetColorCurv(aColor.GetRGB());
		theStyle.SetColorSurf(aColor);
		return;
	}
	//else
	//{
	//	//theStyle.SetColorCurv(defaultCurveColor.GetRGB());
	//	//theStyle.SetColorSurf(defaultSurfColor);
	//	//theLabel.EntryDump(cout);
	//	//cout << " :: " << endl;
	//	//theStyle.SetColorSurf(colorGen.GetColor());
	//}

	if (theTool->GetColor(theLabel, XCAFDoc_ColorSurf, aColor))
	{
		theStyle.SetColorSurf(aColor);
	}
	else
	{
		//theStyle.SetColorSurf(defaultSurfColor);
		Quantity_Color _color = colorGen.GetColor();
		theStyle.SetColorSurf(_color);
#ifdef OUTFILEFLAG
		ofstream outfile;
		outfile.open("E:\\colorMonitor.log", ios::app);
		outfile << "fillStyleColors: ";
		outfile << _color.Hue() << "  ";
		outfile << _color.Light() << "  ";
		outfile << _color.Saturation() << "  ";
		outfile << round(_color.Red() * 255) << "  ";
		outfile << round(_color.Green() * 255) << "  ";
		outfile << round(_color.Blue() * 255) << "  ";
		outfile << endl;
		outfile.close();
#endif
	}

	if (theTool->GetColor(theLabel, XCAFDoc_ColorCurv, aColor))
	{
		theStyle.SetColorCurv(aColor.GetRGB());
	}
	else
	{
		theStyle.SetColorCurv(defaultCurveColor.GetRGB());
	}
}


inline Standard_Boolean BaseIO::getShapesOfSHUO(TopLoc_IndexedMapOfLocation& theaPrevLocMap,
	const Handle(XCAFDoc_ShapeTool)& theSTool,
	const TDF_Label& theSHUOlab,
	TopTools_SequenceOfShape& theSHUOShapeSeq)
{
	Handle(XCAFDoc_GraphNode) SHUO;
	TDF_LabelSequence aLabSeq;
	theSTool->GetSHUONextUsage(theSHUOlab, aLabSeq);
	if (aLabSeq.Length() >= 1)
		for (Standard_Integer i = 1; i <= aLabSeq.Length(); i++) {
			TDF_Label aSubCompL = aLabSeq.Value(i);
			TopLoc_Location compLoc = XCAFDoc_ShapeTool::GetLocation(aSubCompL.Father());
			// create new map of laocation (to not merge locations from different shapes)
			TopLoc_IndexedMapOfLocation aNewPrevLocMap;
			for (Standard_Integer m = 1; m <= theaPrevLocMap.Extent(); m++)
				aNewPrevLocMap.Add(theaPrevLocMap.FindKey(m));
			aNewPrevLocMap.Add(compLoc);
			// got for the new sublocations and corresponding shape
			getShapesOfSHUO(aNewPrevLocMap, theSTool, aSubCompL, theSHUOShapeSeq);
		}
	else {
		TopoDS_Shape aSHUO_NUSh = theSTool->GetShape(theSHUOlab.Father());
		if (aSHUO_NUSh.IsNull()) return Standard_False;
		// cause got shape with location already.
		TopLoc_Location nullLoc;
		aSHUO_NUSh.Location(nullLoc);
		// multiply the locations
		Standard_Integer intMapLenght = theaPrevLocMap.Extent();
		if (intMapLenght < 1)
			return Standard_False; // should not be, but to avoid exception...?
		TopLoc_Location SupcompLoc;
		SupcompLoc = theaPrevLocMap.FindKey(intMapLenght);
		if (intMapLenght > 1) {
			Standard_Integer l = intMapLenght - 1;
			while (l >= 1) {
				SupcompLoc = theaPrevLocMap.FindKey(l).Multiplied(SupcompLoc);
				l--;
			}
		}
		aSHUO_NUSh.Location(SupcompLoc);
		theSHUOShapeSeq.Append(aSHUO_NUSh);
	}
	return (theSHUOShapeSeq.Length() > 0);
}


inline void BaseIO::CollectStyleSettings(const TDF_Label& theLabel,
	const TopLoc_Location& theLoc,
	XCAFPrs_IndexedDataMapOfShapeStyle& theSettings,
	IndexedDataMapOfShapeTreeNode& aShapeTreeNodeMapping,
	tree<TreeLabel>::iterator iter,
	const Quantity_ColorRGBA& theLayerColor,
	int level)
{
	// 检查是否有Face
	{
		TopoDS_Shape _shape = ST->GetShape(theLabel);
		TopExp_Explorer faceExp(_shape, TopAbs_FACE);
		if (!faceExp.More())
		{
			return;
		}
	}

	// for references, first collect colors of referred shape
	{
		TDF_Label aLabelRef;
		if (XCAFDoc_ShapeTool::GetReferredShape(theLabel, aLabelRef))
		{
			Quantity_ColorRGBA aLayerColor = theLayerColor;
			Handle(XCAFDoc_LayerTool) aLayerTool = XCAFDoc_DocumentTool::LayerTool(theLabel);
			Handle(TColStd_HSequenceOfExtendedString) aLayerNames = new TColStd_HSequenceOfExtendedString();
			aLayerTool->GetLayers(theLabel, aLayerNames);
			if (aLayerNames->Length() == 1)
			{
				TDF_Label aLayer = aLayerTool->FindLayer(aLayerNames->First());
				Handle(XCAFDoc_ColorTool) aColorTool = XCAFDoc_DocumentTool::ColorTool(theLabel);
				Quantity_ColorRGBA aColor;
				if (aColorTool->GetColor(aLayer, XCAFDoc_ColorGen, aColor))
					aLayerColor = aColor;
			}

			TopLoc_Location aLocSub = theLoc.Multiplied(XCAFDoc_ShapeTool::GetLocation(theLabel));

			TreeLabel _treeLabel(aLabelRef, ST->GetShape(aLabelRef), TreeShape, level);

			*iter = _treeLabel;

			CollectStyleSettings(aLabelRef, aLocSub, theSettings, aShapeTreeNodeMapping, iter, aLayerColor, level);

		}
	}

	// for assemblies, first collect colors defined in components
	{
		TDF_LabelSequence aComponentLabSeq;
		if (XCAFDoc_ShapeTool::GetComponents(theLabel, aComponentLabSeq)
			&& !aComponentLabSeq.IsEmpty())
		{
			for (TDF_LabelSequence::Iterator aComponentIter(aComponentLabSeq); aComponentIter.More(); aComponentIter.Next())
			{
				const TDF_Label& aComponentLab = aComponentIter.Value();

				TopoDS_Shape _shape = ST->GetShape(aComponentLab);
				TopExp_Explorer faceExp(_shape, TopAbs_FACE);
				if (!faceExp.More())
				{
					continue;
				}

				TreeLabel _treeLabel(aComponentLab, ST->GetShape(aComponentLab), TreeShape, level + 1);
				tree<TreeLabel>::iterator it = iter.node()->insert(_treeLabel);

				CollectStyleSettings(aComponentLab, theLoc, theSettings, aShapeTreeNodeMapping, it, theLayerColor, level + 1);

			}
		}
	}


	// collect settings on subshapes
	Handle(XCAFDoc_ColorTool) aColorTool = XCAFDoc_DocumentTool::ColorTool(theLabel);
	Handle(XCAFDoc_VisMaterialTool) aMatTool = XCAFDoc_DocumentTool::VisMaterialTool(theLabel);

	TDF_LabelSequence aLabSeq;
	XCAFDoc_ShapeTool::GetSubShapes(theLabel, aLabSeq);
	// and add the shape itself
	aLabSeq.Append(theLabel);
	//aLabSeq.InsertBefore(1, theLabel);

	for (TDF_LabelSequence::Iterator aLabIter(aLabSeq); aLabIter.More(); aLabIter.Next())
	{
		const TDF_Label& aLabel = aLabIter.Value();
		XCAFPrs_Style aStyle;
		aStyle.SetVisibility(aColorTool->IsVisible(aLabel));
		aStyle.SetMaterial(aMatTool->GetShapeMaterial(aLabel));

		Handle(TColStd_HSequenceOfExtendedString) aLayerNames;
		Handle(XCAFDoc_LayerTool) aLayerTool = XCAFDoc_DocumentTool::LayerTool(aLabel);
		if (aStyle.IsVisible())
		{
			aLayerNames = new TColStd_HSequenceOfExtendedString();
			aLayerTool->GetLayers(aLabel, aLayerNames);
			Standard_Integer aNbHidden = 0;
			for (TColStd_HSequenceOfExtendedString::Iterator aLayerIter(*aLayerNames); aLayerIter.More(); aLayerIter.Next())
			{
				const TCollection_ExtendedString& aLayerName = aLayerIter.Value();
				if (!aLayerTool->IsVisible(aLayerTool->FindLayer(aLayerName)))
				{
					++aNbHidden;
				}
			}
			aStyle.SetVisibility(aNbHidden == 0
				|| aNbHidden != aLayerNames->Length());
		}

		if (aColorTool->IsColorByLayer(aLabel))
		{
			Quantity_ColorRGBA aLayerColor = theLayerColor;
			if (aLayerNames.IsNull())
			{
				aLayerNames = new TColStd_HSequenceOfExtendedString();
				aLayerTool->GetLayers(aLabel, aLayerNames);
			}
			if (aLayerNames->Length() == 1)
			{
				TDF_Label aLayer = aLayerTool->FindLayer(aLayerNames->First());
				Quantity_ColorRGBA aColor;
				if (aColorTool->GetColor(aLayer, XCAFDoc_ColorGen, aColor))
				{
					aLayerColor = aColor;
				}
			}

			aStyle.SetColorCurv(aLayerColor.GetRGB());
			aStyle.SetColorSurf(aLayerColor);
		}
		else
		{
			fillStyleColors(aStyle, aColorTool, aLabel);
		}

		// PTV try to set color from SHUO structure
		const Handle(XCAFDoc_ShapeTool)& aShapeTool = aColorTool->ShapeTool();
		if (aShapeTool->IsComponent(aLabel))
		{
			TDF_AttributeSequence aShuoAttribSeq;
			aShapeTool->GetAllComponentSHUO(aLabel, aShuoAttribSeq);
			for (TDF_AttributeSequence::Iterator aShuoAttribIter(aShuoAttribSeq); aShuoAttribIter.More(); aShuoAttribIter.Next())
			{
				Handle(XCAFDoc_GraphNode) aShuoNode = Handle(XCAFDoc_GraphNode)::DownCast(aShuoAttribIter.Value());
				if (aShuoNode.IsNull())
				{
					continue;
				}

				const TDF_Label aShuolab = aShuoNode->Label();
				{
					TDF_LabelSequence aShuoLabSeq;
					aShapeTool->GetSHUONextUsage(aShuolab, aShuoLabSeq);
					if (aShuoLabSeq.IsEmpty())
					{
						continue;
					}
				}

				XCAFPrs_Style aShuoStyle;
				aShuoStyle.SetMaterial(aMatTool->GetShapeMaterial(aShuolab));
				aShuoStyle.SetVisibility(aColorTool->IsVisible(aShuolab));
				fillStyleColors(aShuoStyle, aColorTool, aShuolab);

				if (aShuoStyle.IsEmpty())
				{
					aShuoStyle.SetColorCurv(defaultCurveColor.GetRGB());
					//aShuoStyle.SetColorSurf(defaultSurfColor);
					aShuoStyle.SetColorSurf(colorGen.GetColor());
					//continue;
				}

				/*
				// may be work, but static it returns excess shapes. It is more faster to use OLD version.
				// PTV 14.02.2003 NEW version using API of ShapeTool
				TopTools_SequenceOfShape aShuoShapeSeq;
				aShapeTool->GetAllStyledComponents (aShuoNode, aShuoShapeSeq);
				for (TopTools_SequenceOfShape::Iterator aShuoShapeIter (aShuoShapeSeq); aShuoShapeIter.More(); aShuoShapeIter.Next())
				{
				  const TopoDS_Shape& aShuoShape = aShuoShapeIter.Value();
				  if (!aShuoShape.IsNull())
					theSettings.Bind (aShuoShape, aShuoStyle);
				}*/
				// OLD version that was written before ShapeTool API, and it FASTER for presentation
				// get TOP location of SHUO component
				TopLoc_Location compLoc = XCAFDoc_ShapeTool::GetLocation(aLabel);
				TopLoc_IndexedMapOfLocation aPrevLocMap;
				// get previous set location
				if (!theLoc.IsIdentity())
				{
					aPrevLocMap.Add(theLoc);
				}
				aPrevLocMap.Add(compLoc);

				// get shapes of SHUO Next_Usage components
				TopTools_SequenceOfShape aShuoShapeSeq;
				getShapesOfSHUO(aPrevLocMap, aShapeTool, aShuolab, aShuoShapeSeq);
				for (TopTools_SequenceOfShape::Iterator aShuoShapeIter(aShuoShapeSeq); aShuoShapeIter.More(); aShuoShapeIter.Next())
				{
					const TopoDS_Shape& aShuoShape = aShuoShapeIter.Value();
					XCAFPrs_Style* aMapStyle = theSettings.ChangeSeek(aShuoShape);
					if (aMapStyle == NULL)
						theSettings.Add(aShuoShape, aShuoStyle);
					else
						*aMapStyle = aShuoStyle;

					tree<TreeLabel>** _treeNode = aShapeTreeNodeMapping.ChangeSeek(aShuoShape);

					if (_treeNode == NULL)
					{
						aShapeTreeNodeMapping.Add(aShuoShape, iter.node());
					}
					//else
					//{
					//	*_treeNode = ParentNode;
					//}
				}

				continue;
			}
		}

		if (aStyle.IsEmpty())
		{
			aStyle.SetColorCurv(defaultCurveColor.GetRGB());
			//aStyle.SetColorSurf(defaultSurfColor);
			Quantity_Color _color = colorGen.GetColor();
			aStyle.SetColorSurf(_color);
#ifdef OUTFILEFLAG
			ofstream outfile;
			outfile.open("E:\\colorMonitor.log", ios::app);
			outfile << "CollectStyleSettings: ";
			outfile << _color.Hue() << "  ";
			outfile << _color.Light() << "  ";
			outfile << _color.Saturation() << "  ";
			outfile << round(_color.Red() * 255) << "  ";
			outfile << round(_color.Green() * 255) << "  ";
			outfile << round(_color.Blue() * 255) << "  ";
			outfile << endl;
			outfile.close();
#endif
			//continue;
		}


		TopoDS_Shape aSubshape = XCAFDoc_ShapeTool::GetShape(aLabel);

		Handle(TDataStd_Name) anAttribute;

		TCollection_ExtendedString str;

		if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), anAttribute))
		{
			str = anAttribute->Get();
		}


		if (aSubshape.ShapeType() == TopAbs_COMPOUND)
		{
			if (aSubshape.NbChildren() == 0)
			{
				continue;
			}
		}
		aSubshape.Move(theLoc);
		XCAFPrs_Style* aMapStyle = theSettings.ChangeSeek(aSubshape);
		if (aMapStyle == NULL)
		{
			theSettings.Add(aSubshape, aStyle);
		}
		else
			*aMapStyle = aStyle;

		tree<TreeLabel>** _treeNode = aShapeTreeNodeMapping.ChangeSeek(aSubshape);

		if (_treeNode == NULL)
		{
			aShapeTreeNodeMapping.Add(aSubshape, iter.node());
		}
		//else
		//{
		//	*_treeNode = ParentNode;
		//}
	}
}

inline void BaseIO::Node2Json(tree<TreeLabel>* node, Json::Value &jsonNode, bool faceFlag)
{
	int _id = node->get()->HashCode;

	if (!faceFlag && _id < FACEOFFSET * 2 && _id != 0)
	{
		return;
	}

	if (_id < FACEOFFSET * 2)
	{
		jsonNode["name"] = node->get()->Name + "_" + std::to_string(_id);
	}
	else
	{
		jsonNode["name"] = node->get()->Name;
	}
	jsonNode["id"] = _id;

	//cout << "Name: " << jsonNode["name"] << endl;
	//cout << "id: " << node->get()->HashCode << endl;
	//cout << "subFaceHashCodes: " << node->get()->subFaceHashCodes.size() << endl;

	if (node->get()->subFaceHashCodes.size() > 0)
	{
		Json::Value jsonArray;
		for (int i = 0; i < node->get()->subFaceHashCodes.size(); i++)
		{
			jsonArray.append(node->get()->subFaceHashCodes[i]);
		}

		jsonNode["subFace"] = jsonArray;
	}


	Json::Value jsonChildren;

	typename tree<TreeLabel>::iterator it = node->begin();

	for (; it != node->end(); it++) {
		Json::Value _jsonNode;
		Node2Json(it.node(), _jsonNode, faceFlag);

		if (!_jsonNode.empty())
		{
			jsonChildren.append(_jsonNode);
		}
	}

	if (!jsonChildren.empty())
	{
		jsonNode["children"] = jsonChildren;
	}
}


#ifdef OCCTEST

inline void BaseIO::SetAISInteractiveContext(Handle(AIS_InteractiveContext) _myAISContext)
{
	myAISContext = _myAISContext;
}
inline void BaseIO::Render()
{
	//cout << "Render Stp File... " << endl;

	Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	NCollection_IndexedDataMap<int, FaceProperty> _faceMap;

	GetFaceMap(_faceMap);

	for (NCollection_IndexedDataMap<int, FaceProperty>::Iterator aKeyFaceIter(_faceMap); aKeyFaceIter.More(); aKeyFaceIter.Next())
	{
		FaceProperty aProp = aKeyFaceIter.Value();
		Handle(AIS_Shape) myAISSurf = ::new AIS_Shape(aProp.faceObj);

		Quantity_Color color(aKeyFaceIter.Value().red / 255.0, aKeyFaceIter.Value().green / 255.0, aKeyFaceIter.Value().blue / 255.0, Quantity_TOC_RGB);

		myAISSurf->SetColor(color);
		myAISContext->Display(myAISSurf, Standard_False);

	}
}
#endif

#endif