
/*******************************************************************************

包含文件说明：
1. RandomColorGenerator.h：
	基于HSV色彩模式定义的一个颜色生成器，过滤掉一些不适合显示模型的颜色，比如纯色；
	另外，生成的相邻颜色差别也较大，避免颜色过于相似

2. tcl_5.0.6
	树结构的库，由于版本比较老，所以DCiP编译时会有一些警告和错误；
	一个是继承binary_function出现错误，这个可以直接注释掉；
	一个是一些allocate内存的警告，可以通过定义宏的方式过滤
	_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

一些函数说明：
1. GetFaceMap：获取hashcode与TopoDS_Face的映射
2. GetJson：获取用于显示树形结构的Json数据
3. CreateBrepById：根据id获取到Face后，生成Brep
4. CreateCenterById：根据id获取到Face后，生成中心点

*******************************************************************************/

#ifndef BASEIO_H
#define BASEIO_H

#ifdef new
#undef new
#endif // new


#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <assert.h>

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
#include <Poly_Connect.hxx>
#include <Poly.hxx>
#include <TDocStd_Document.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <GeomLib.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepGProp_Face.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <IGESCAFControl_Writer.hxx>
#include <TopTools_DataMapOfShapeShape.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>


#include "occ_compatible.h"
#include "tcl_5.0.6/tree.h"
#include "jsoncpp/json/json.h"
//#include"jsoncpp/jsoncpp.cpp"
#include "RandomColorGenerator.h"
#include "FaceProperty.h"
#include "TreeLabel.h"
#include "GeoAPIUtil.h"


using namespace std;
using namespace tcl;
using namespace Json;

class CompareLess {
public:
	bool operator()(const TreeLabel& lhs, const TreeLabel& rhs) const {
		return lhs.id < rhs.id;
	}
};

typedef tree<TreeLabel, CompareLess> CTree;
//typedef tree<TreeLabel> CTree;

class BaseIO
{
public:
	BaseIO()
	{
		aMeshParams.Deflection = 0.02;		//For Fine Model: 0.02
		aMeshParams.Angle = 0.5;
		aMeshParams.Relative = Standard_True;
		aMeshParams.InParallel = Standard_True;
		//aMeshParams.MinSize = Precision::Confusion();
		aMeshParams.InternalVerticesMode = Standard_True;
		aMeshParams.ControlSurfaceDeflection = Standard_True;

		defaultCurveColor.SetValues(1.0, 1.0, 1.0, 1.0);
		defaultSurfColor.SetValues(static_cast<float>(0.64), static_cast<float>(0.64), static_cast<float>(1.0), static_cast<float>(1.0));

		sewFlag = false;	

		faceStartId = 1000000;
		componentStartId = 6000000;
		edgeStartId = 10000000;
	}
	~BaseIO() {}

	void Perform();

	void GetFaceMap(NCollection_IndexedDataMap<int, FaceProperty> &_faceMap);
	void SetSewModel(const bool &flag);
	void CreateBrepById(const vector<int>& _hashCode, const string &filePath);
	void CreateCenterById(const int &_id, double &x, double &y, double &z);
	bool CreateStpById(const int &_id, const string &filePath);
	inline void GetEdgeMap(map<int, TopoDS_Edge>& edges);
	inline void GetDocument(Handle(TDocStd_Document)& document);

	string GetJson(bool faceFlag = true);
	string GetJsonEx(bool faceFlag, vector<int>& faces, vector<int>& components, NCollection_DataMap<Standard_Integer, Standard_Integer>& faceID_ComponentIDMap);
	string jsonPath;

protected:
	typedef NCollection_IndexedDataMap<TopoDS_Shape, CTree *, TopTools_ShapeMapHasher> IndexedDataMapOfShapeTreeNode;
	typedef NCollection_IndexedDataMap<TopoDS_Shape, CTree *, TopTools_ShapeMapHasher>::Iterator DataMapIteratorOfIndexedDataMapOfShapeTreeNode;
	typedef NCollection_IndexedDataMap<TopoDS_Face, FaceProperty, TopTools_ShapeMapHasher> IndexedDataMapOfFaceProperty;
	typedef NCollection_IndexedDataMap<TopoDS_Edge, int, TopTools_ShapeMapHasher> IndexedDataMapOfEdgeId;
	typedef NCollection_IndexedDataMap<TopoDS_Shape, int, TopTools_ShapeMapHasher> IndexedDataMapOfShapeId;

	int faceStartId;
	int componentStartId;
	int edgeStartId;
	int gFaceId;
	int gEdgeId;
	int gComponentId;

	bool sewFlag;	

	Handle(TDocStd_Document) doc;
	Handle(XCAFDoc_ShapeTool) ST;
	Handle(XCAFDoc_ColorTool) CT;

	IMeshTools_Parameters aMeshParams;
	Quantity_ColorRGBA defaultCurveColor;
	Quantity_ColorRGBA defaultSurfColor;
	CTree modelTree;

	RandomColorGenerator colorGen;

	map<Standard_Integer, set<Standard_Integer> > mapEdgeType;
	XCAFPrs_IndexedDataMapOfShapeStyle aSettings;
	IndexedDataMapOfFaceProperty faceFacepropertyMap;
	IndexedDataMapOfEdgeId edgeIdMap;
	IndexedDataMapOfShapeId shapeIdMap;

	NCollection_IndexedDataMap<int, FaceProperty> idFacePropertyMap;
	NCollection_IndexedDataMap<int, TopoDS_Edge> idEdgeMap;
	NCollection_IndexedDataMap<int, TopoDS_Shape> idShapeMap;
	NCollection_IndexedDataMap<TopoDS_Face, NCollection_Vector<int>> faceEdgeMap;//new add
	NCollection_IndexedDataMap<TopoDS_Shape, std::string> shapeNameMap;//new add
	NCollection_Map<TopoDS_Shape, TopTools_ShapeMapHasher> parseComponentCodes;


	TopoDS_Shape SewModel(const TopoDS_Shape& objShape);
	TCollection_ExtendedString GetLabelName(const TDF_Label &label);
	void DefineDataId();
	void GetEdgeType(const TopoDS_Shape &objShape);
	TopoDS_Face FixFace(const TopoDS_Face& face);
	void fillStyleColors(XCAFPrs_Style& theStyle, const Handle(XCAFDoc_ColorTool)& theTool,	const TDF_Label& theLabel);
	Standard_Boolean getShapesOfSHUO(TopLoc_IndexedMapOfLocation& theaPrevLocMap, const Handle(XCAFDoc_ShapeTool)& theSTool, const TDF_Label& theSHUOlab, TopTools_SequenceOfShape& theSHUOShapeSeq);
	void CollectStyleSettings(const TDF_Label& theLabel, const TopLoc_Location& theLoc,	XCAFPrs_IndexedDataMapOfShapeStyle& theSettings,
		IndexedDataMapOfShapeTreeNode& aShapeTreeNodeMapping, CTree::iterator iter,	const Quantity_ColorRGBA& theLayerColor = Quantity_ColorRGBA(Quantity_NOC_WHITE), int level = 1);
	void BuildFaceData(const TopoDS_Shape& shape, const XCAFPrs_Style& style, CTree* treeNode);
	void Node2Json(CTree* node, Json::Value &jsonNode, bool faceFlag);
	bool compareJsonValue(const Json::Value& a, const Json::Value& b);
	void sortJsonArrays(Json::Value& node);
	void displayJsonTree(const Json::Value& node, int indent = 0);
	void sortFaceId(Json::Value& node);
	void SetStartId(const int &faceStartId, const int &componentStartId, const int &edgeStartId);
	void GetTree(CTree& Tree);
	string rgbToHex(int r, int g, int b);

	//template <typename T>
	//void coutRed(const T &str)
	//{
	//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//	SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // 设置文字颜色为红色
	//	std::cout << str << std::endl;
	//	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 重置为默认颜色
	//}

#ifdef OCCTEST
public:
	void Render();
	void SetAISInteractiveContext(Handle(AIS_InteractiveContext) _myAISContext);
	Handle(AIS_InteractiveContext) myAISContext;
#endif
};

inline void BaseIO::SetStartId(const int &faceStartId, const int &componentStartId, const int &edgeStartId)
{
	this->faceStartId = faceStartId;
	this->componentStartId = componentStartId;
	this->edgeStartId = edgeStartId;
}

inline void BaseIO::SetSewModel(const bool &flag)
{
	sewFlag = flag;
}

inline void BaseIO::Perform()
{
	colorGen.ResetRef();

	// 这里重新赋值，否则切换窗口重新读入时，id会继续累加
	gFaceId = faceStartId;
	gEdgeId = edgeStartId;
	gComponentId = componentStartId;

	ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
	CT = XCAFDoc_DocumentTool::ColorTool(doc->Main());

	TDF_LabelSequence Labels;
	ST->GetFreeShapes(Labels);
	modelTree.clear();
	idFacePropertyMap.Clear();
	idEdgeMap.Clear();
	idShapeMap.Clear();

	int faceCount = 0;

	//计算模型包含face数目，确定模型的大小
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		// 创建一个TopExp_Explorer对象，用于遍历形状中的面
		TopExp_Explorer explorer(objShape, TopAbs_FACE);
		// 当还有更多的面可以遍历的时候
		while (explorer.More()) {
			// 面的数量加1
			faceCount++;
			// 移动到下一个面
			explorer.Next();
		}
	}

	//如果模型相对较小，可以采用较高的精度
	if (faceCount < 50)
	{
		aMeshParams.Deflection = 0.005;
	}
	else if (faceCount < 100)
	{
		aMeshParams.Deflection = 0.01;
	}
	else if (faceCount < 200)
	{
		aMeshParams.Deflection = 0.02;
	}

	cout << "Total Face Count:" << faceCount << endl;

	// 划分网格
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		if (objShape.IsNull())
		{
			continue;
		}

		if (sewFlag)
		{
			SewModel(objShape);
		}

		BRepTools::Clean(objShape);
		BRepMesh_IncrementalMesh(objShape, aMeshParams);
	}

	DefineDataId();

	//// 检查重复的Hashcode
	//for (int idx = 1; idx <= Labels.Length(); ++idx)
	//{
	//	TDF_Label theLabel = Labels.ChangeValue(idx);
	//	TopoDS_Shape objShape;
	//	ST->GetShape(theLabel, objShape);

	//	map<size_t, TopoDS_Face> mapFaces;

	//	NCollection_IndexedDataMap<TopoDS_Face, int, TopTools_ShapeMapHasher>  faceIndexMap;

	//	int faceCount = 0;

	//	for (TopExp_Explorer faceExp(objShape, TopAbs_FACE); faceExp.More(); faceExp.Next())
	//	{
	//		const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

	//		faceIndexMap.Add(objFace, faceCount);

	//		size_t hashcode = std::hash<TopoDS_Face>{}(objFace);

	//		faceCount++;

	//		if (mapFaces.find(hashcode) != mapFaces.end()) {
	//			//比较面积，重心坐标

	//			GProp_GProps system;
	//			GProp_GProps props;

	//			BRepGProp::VolumeProperties(mapFaces[hashcode], system);
	//			gp_Pnt centerOfMass = system.CentreOfMass();
	//			Standard_Real x = centerOfMass.X();
	//			Standard_Real y = centerOfMass.Y();
	//			Standard_Real z = centerOfMass.Z();
	//			BRepGProp::SurfaceProperties(mapFaces[hashcode], props);
	//			Standard_Real area = props.Mass();

	//			cout << "Origin: " << hashcode << "  " << x << "  " << y << "  " << z << "  " << area << endl;


	//			BRepGProp::VolumeProperties(objFace, system);
	//			centerOfMass = system.CentreOfMass();
	//			x = centerOfMass.X();
	//			y = centerOfMass.Y();
	//			z = centerOfMass.Z();				
	//			BRepGProp::SurfaceProperties(objFace, props);
	//			area = props.Mass();

	//			cout << "Repeat: " << hashcode << "  " <<  x << "  " << y << "  " << z << "  " << area << endl;


	//			continue;
	//		}
	//		mapFaces.insert(make_pair(hashcode, objFace));
	//	}

	//	cout << "Face Counts:  " << faceCount << endl;
	//	cout << "Map Size:     " << mapFaces.size() << endl;
	//	cout << "faceIndexMap Size: " << faceIndexMap.Size() << endl;
	//}

	modelTree.get()->id = componentStartId;


	// 遍历所有的Free Label，构建Tree
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		if (objShape.IsNull())
		{
			continue;
		}

		TreeLabel _treeLabel(theLabel, objShape, TreeShape, 1);
		_treeLabel.id = ++gComponentId;
		idShapeMap.Add(_treeLabel.id, objShape);
		shapeIdMap.Add(objShape, _treeLabel.id);
		CTree::iterator it = modelTree.insert(_treeLabel);

		TopLoc_Location aLoc;

		GetEdgeType(objShape);

		IndexedDataMapOfShapeTreeNode aShapeTreeNodeMapping;
		CollectStyleSettings(theLabel, aLoc, aSettings, aShapeTreeNodeMapping, it, Quantity_ColorRGBA(Quantity_NOC_WHITE), 1);

		for (Standard_Integer anIter = aSettings.Extent(); anIter >= 1; --anIter)
		{
			XCAFPrs_Style aProp = aSettings.FindFromIndex(anIter);
		}

		//for (Standard_Integer anIter = 1; anIter <= aSettings.Extent(); ++anIter)
		for (Standard_Integer anIter = aSettings.Extent(); anIter >= 1; --anIter)
		{
			BuildFaceData(aSettings.FindKey(anIter), aSettings.FindFromIndex(anIter), *(aShapeTreeNodeMapping.Seek(aSettings.FindKey(anIter))));
		}	

		aSettings.Clear();
		aShapeTreeNodeMapping.Clear();		
	}

	Labels.Clear();

	for (Standard_Integer anIter = 1; anIter <= faceFacepropertyMap.Size(); ++anIter)
	{
		idFacePropertyMap.Add(faceFacepropertyMap.FindFromIndex(anIter).id, faceFacepropertyMap.FindFromIndex(anIter));
	}

	faceFacepropertyMap.Clear();
}

inline void BaseIO::DefineDataId()
{
	TDF_LabelSequence Labels;
	ST->GetFreeShapes(Labels);

	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		//changed
		// 公共边会重复，所以需要判断一下键值是否已经存在了，否则一个公共边会有两个不同的id
		for (TopExp_Explorer edgeExp(objShape, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
		{
			const TopoDS_Edge& objEdge = TopoDS::Edge(edgeExp.Current());

			int edgeId;
			if (!edgeIdMap.FindFromKey(objEdge, edgeId))
			{
				idEdgeMap.Add(gEdgeId, objEdge);
				edgeIdMap.Add(objEdge, gEdgeId++);
			}
		}
		//changed end

		for (TopExp_Explorer faceExp(objShape, TopAbs_FACE); faceExp.More(); faceExp.Next())
		{
			const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

			// 这里是针对subshape定义颜色
			FaceProperty *pFaceProp = faceFacepropertyMap.ChangeSeek(objFace);
			if (pFaceProp != NULL)
			{
				continue;
			}

			FaceProperty faceProp;
			faceProp.faceObj = objFace;
			faceProp.id = gFaceId++;

			faceFacepropertyMap.Add(objFace, faceProp);

			//new add
			NCollection_Vector<int> edgeIds;
			for (TopExp_Explorer edgeExp(objFace, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
			{
				const TopoDS_Edge& objEdge = TopoDS::Edge(edgeExp.Current());

				int edgeId;
				edgeId = edgeIdMap.FindFromKey(objEdge);
				edgeIds.Append(edgeId);
			}
			faceEdgeMap.Add(objFace, edgeIds);
			//add end
		}
	}

	// 避免面数过多，face id与ComponentId重复
	if (gComponentId < gFaceId)
	{
		componentStartId = gFaceId + 1;
		gComponentId = gFaceId + 1;
	}
}

inline void BaseIO::GetFaceMap(NCollection_IndexedDataMap<int, FaceProperty> &_faceMap)
{
	_faceMap = idFacePropertyMap;
}

inline void BaseIO::GetTree(CTree& Tree)
{
	Tree = modelTree;
}

inline void BaseIO::sortFaceId(Json::Value& node)
{

	if (node.isArray()) {

		for (auto& element : node) {

			if (element["type"] == "Face")
			{
				gFaceId++;
				element["faceId"] = to_string(gFaceId);

				string tail;

				if (gFaceId < 10)
				{
					tail = "_0000" + to_string(gFaceId);
				}
				else if (gFaceId < 100)
				{
					tail = "_000" + to_string(gFaceId);
				}
				else if (gFaceId < 1000)
				{
					tail = "_00" + to_string(gFaceId);
				}
				else if (gFaceId < 10000)
				{
					tail = "_0" + to_string(gFaceId);
				}
				else
				{
					tail = "_" + to_string(gFaceId);
				}

				element["name"] = "Face" + tail;

			}
			else
			{
				element["faceId"] = "0";
			}
		}

		for (auto& element : node) {
			sortFaceId(element);
		}
	}
	else if (node.isObject()) {

		//if (node["name"] == "Model")
		//{
		//	node["faceId"] = "0";
		//}

		for (auto& key : node.getMemberNames()) {
			sortFaceId(node[key]);
		}
	}
}

inline string BaseIO::GetJson(bool faceFlag)
{
	Json::Value jsonNode;
	Node2Json(&modelTree, jsonNode, faceFlag);
	sortJsonArrays(jsonNode);

	gFaceId = 0;

	//sortFaceId(jsonNode);

	//displayJsonTree(jsonNode);

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";	
	builder["emitUTF8"] = true; // 直接输出 UTF-8 字符
	builder["indentation"] = "   ";
	//builder["indentation"] = ""; // 压缩格式，没有换行和不必要的空白字符
	// 用于输出json，测试用
	if (!jsonPath.empty())
	{
		ofstream outfile(jsonPath.c_str());
		outfile << Json::writeString(builder, jsonNode) << endl;
		outfile.close();
	}
	return Json::writeString(builder, jsonNode);
}

// 比较函数，用于比较两个Json::Value类型元素（假设这里比较的是数值类型元素，你可按需修改比较逻辑）
inline bool BaseIO::compareJsonValue(const Json::Value& a, const Json::Value& b) {
	if (a.isObject() && b.isObject())
	{
		if (a["name"].isString() && b["name"].isString()) {
			std::string nameA = a["name"].asString();
			std::string nameB = b["name"].asString();
			return nameA < nameB;
		}
	}
	else
	{
		return a < b;
	}
	return false;
}
// 递归函数，用于处理JSON结构中所有的数组元素排序
inline void BaseIO::sortJsonArrays(Json::Value& node) {
	if (node.isArray()) {
		std::vector<Json::Value> arrayElements;
		for (auto& element : node) {
			arrayElements.push_back(element);
		}

		std::sort(arrayElements.begin(), arrayElements.end(), [this](const Json::Value& a, const Json::Value& b) {
			return this->compareJsonValue(a, b);
		});

		node.clear();
		for (const auto& element : arrayElements) {
			node.append(element);
		}

		// 递归处理数组中的每个元素（如果元素是对象或者数组）
		for (auto& element : node) {
			sortJsonArrays(element);
		}
	}

	else if (node.isObject()) {
		for (auto& key : node.getMemberNames()) {
			sortJsonArrays(node[key]);
		}
	}
}


inline void BaseIO::displayJsonTree(const Json::Value& node, int indent) {
	if (node.isObject()) {
		bool hasName = false;
		for (const auto& key : node.getMemberNames()) {
			if (key == "name") {
				hasName = true;
			}
			displayJsonTree(node[key], indent + 1);
		}
		if (!hasName) {
			for (const auto& key : node.getMemberNames()) {
				displayJsonTree(node[key], indent + 1);
			}
		}
	}
	else if (node.isArray()) {
		for (const auto& element : node) {
			displayJsonTree(element, indent + 1);
		}
	}
}

inline void BaseIO::BuildFaceData(const TopoDS_Shape& shape, const XCAFPrs_Style& style, CTree* treeNode)
{
	if (!parseComponentCodes.Contains(shape))
	{
		return;
	}

	Quantity_Color color = style.GetColorSurf();	

	// 遍历所有Face
	for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
	{
		TopoDS_Face &objFace = const_cast<TopoDS_Face &> (TopoDS::Face(faceExp.Current()));
		TopLoc_Location location;
		opencascade::handle<Poly_Triangulation> triFace = BRep_Tool::Triangulation(objFace, location);

		if (triFace.IsNull())
		{
			continue;
			/*
			// 下面是为了修复不合格曲面做的措施，实际测试中极少有曲面可以被修复，
			// 由于病态曲面即使修复后得到的曲面也大概率是病态曲面，因此这里注释掉；
			// 强行打开程序会报错，因为会受到FaceFacepropertyMap， edgeIdMap的影响；
			// 需要重新定义FaceFacepropertyMap， edgeIdMap才能运行
			TopoDS_Face fixFace = FixFace(objFace);
			BRepMesh_IncrementalMesh(fixFace, 1.0);
			opencascade::handle<Poly_Triangulation> _triFace = BRep_Tool::Triangulation(fixFace, location);

			if (_triFace.IsNull())
			{
				continue;
			}
			else
			{
				triFace = _triFace;
				faceFacepropertyMap.ChangeSeek(objFace)->faceObj = fixFace;
			}
			*/
		}		

		// 这里是针对subshape定义颜色
		FaceProperty *pFaceProp = faceFacepropertyMap.ChangeSeek(objFace);

		assert(pFaceProp);

		pFaceProp->faceObj = objFace;
		pFaceProp->SetColor(color);
		//pFaceProp->BuildPoints(triFace->Nodes(), location);
		pFaceProp->BuildPoints(POLY_TRIGULATION_NODES(triFace), location);
		pFaceProp->BuildElements(POLY_TRIGULATION_TRIANGLES(triFace));
		pFaceProp->ComputeNormals();

		for (TopExp_Explorer edgeExp(pFaceProp->faceObj, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
		{
			const TopoDS_Edge &objEdge = TopoDS::Edge(edgeExp.Current());
			opencascade::handle<Poly_PolygonOnTriangulation> triEdge = BRep_Tool::PolygonOnTriangulation(objEdge, triFace, location);

			if (triEdge.IsNull())
			{
				continue;
			}		

			Standard_Integer edgeId = edgeIdMap.FindFromKey(objEdge);
			EdgeType _edgeType;

			if (mapEdgeType[edgeId].size() == 1)
			{
				_edgeType = FreeEdge;
			}
			else if (mapEdgeType[edgeId].size() == 2)
			{
				_edgeType = InnerEdge;
			}
			else
			{
				_edgeType = ShareEdge;
			}

			EdgeProperty _edgeProperty;
			_edgeProperty.edgeType = _edgeType;
			_edgeProperty.id = edgeId;

			for (TColStd_Array1OfInteger::Iterator anIter(triEdge->Nodes()); anIter.More(); anIter.Next())
			{
				_edgeProperty.edges.push_back(anIter.Value() - 1);
			}
			pFaceProp->edgeProperties.insert(_edgeProperty);
		}

		// 将Face插入到节点
		TreeLabel _node(objFace, TreeFace);
		_node.id = pFaceProp->id;
		treeNode->insert(_node);
	}
}

// 函数用于修复给定的面
inline TopoDS_Face BaseIO::FixFace(const TopoDS_Face& face)
{
	// 创建ShapeFix_Shape对象来修复形状
	ShapeFix_Shape fixer(face);

	if (fixer.Perform())
	{
		TopoDS_Face _face = TopoDS::Face(fixer.Shape());
		return _face;
	}
	else
	{
		return face;
	}
}

inline void BaseIO::GetEdgeType(const TopoDS_Shape &objShape)
{
	for (TopExp_Explorer faceExp(objShape, TopAbs_FACE); faceExp.More(); faceExp.Next())
	{
		TopoDS_Shape shapeChild = faceExp.Current();

		const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

		int faceId = faceFacepropertyMap.FindFromKey(objFace).id;

		for (TopExp_Explorer edgeExp(objFace, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
		{
			const TopoDS_Edge &objEdge = TopoDS::Edge(edgeExp.Current());

			int edgeId = edgeIdMap.FindFromKey(objEdge);

			if (mapEdgeType.find(edgeId) == mapEdgeType.end())
			{
				set<Standard_Integer> tempFaces;
				tempFaces.insert(faceId);
				mapEdgeType.insert(make_pair(edgeId, tempFaces));
			}
			else
			{
				mapEdgeType[edgeId].insert(faceId);
			}
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

inline void BaseIO::fillStyleColors(XCAFPrs_Style& theStyle, const Handle(XCAFDoc_ColorTool)& theTool, const TDF_Label& theLabel)
{
	Quantity_ColorRGBA aColor;
	if (theTool->GetColor(theLabel, XCAFDoc_ColorGen, aColor))
	{
		theStyle.SetColorCurv(aColor.GetRGB());
		theStyle.SetColorSurf(aColor);
		return;
	}

	if (theTool->GetColor(theLabel, XCAFDoc_ColorSurf, aColor))
	{
		theStyle.SetColorSurf(aColor);
	}
	else
	{
		//theStyle.SetColorSurf(defaultSurfColor);
		Quantity_Color _color = colorGen.GetColor();
		theStyle.SetColorSurf(_color);
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


inline void BaseIO::CollectStyleSettings(const TDF_Label& theLabel,	const TopLoc_Location& theLoc, XCAFPrs_IndexedDataMapOfShapeStyle& theSettings,
	IndexedDataMapOfShapeTreeNode& aShapeTreeNodeMapping,
	CTree::iterator iter,
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

	bool isLowestComponent = true;

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
			_treeLabel.id = iter->id;
			*iter = _treeLabel;

			shapeIdMap.Add(ST->GetShape(aLabelRef), _treeLabel.id);
			(*idShapeMap.ChangeSeek(_treeLabel.id)) = ST->GetShape(aLabelRef);

			isLowestComponent = false;

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
				_treeLabel.id = ++gComponentId;
				CTree::iterator it = iter.node()->insert(_treeLabel);

				shapeIdMap.Add(ST->GetShape(aComponentLab), _treeLabel.id);
				idShapeMap.Add(_treeLabel.id, ST->GetShape(aComponentLab));

				isLowestComponent = false;

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

					CTree** _treeNode = aShapeTreeNodeMapping.ChangeSeek(aShuoShape);

					if (_treeNode == NULL)
					{
						aShapeTreeNodeMapping.Add(aShuoShape, iter.node());
					}
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
		}

		TopoDS_Shape aSubshape = XCAFDoc_ShapeTool::GetShape(aLabel);

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

		CTree** _treeNode = aShapeTreeNodeMapping.ChangeSeek(aSubshape);

		if (_treeNode == NULL)
		{
			aShapeTreeNodeMapping.Add(aSubshape, iter.node());
		}

		// 判断是否是最底层的Shape，如果是，放到parseComponentCodes中，后续将不Build
		if (isLowestComponent)
		{
			parseComponentCodes.Add(aSubshape);
		}
	}
}

inline void BaseIO::Node2Json(CTree* node, Json::Value &jsonNode, bool faceFlag)
{
	int _id = node->get()->id;

	TreeType NodeType = node->get()->NodeType;

	if (!faceFlag && NodeType == TreeFace && _id != 0)
	{
		return;
	}

	FaceProperty fProp;	

	if (idFacePropertyMap.FindFromKey(_id, fProp))
	{
		//string color16 = rgbToHex(idFacePropertyMap.Seek(_id)->red, idFacePropertyMap.Seek(_id)->green, idFacePropertyMap.Seek(_id)->blue);
		string color16 = rgbToHex(fProp.red, fProp.green, fProp.blue);
		jsonNode["color"] = color16;
		//jsonNode["faceId"] = fProp.faceId;
	}
	else
	{
		jsonNode["color"] = "#FFFFFF";
		//jsonNode["faceId"] = node->get()->faceId;
	}	

	if (NodeType == TreeFace)
	{
		node->get()->id = fProp.id;
		//必须为小写，否则DCiP不能识别
		jsonNode["name"] = node->get()->GetJsonName();
		jsonNode["type"] = "Face";
	}
	else
	{
		jsonNode["name"] = node->get()->Name;
		jsonNode["type"] = "Shape";
		shapeNameMap.Add(node->get()->Shape, node->get()->Name);//new add
	}

	jsonNode["id"] = node->get()->id;

	Json::Value jsonChildren;

	typename CTree::iterator it = node->begin();

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

inline string BaseIO::rgbToHex(int r, int g, int b)
{
	stringstream ss;
	ss << "#"
		<< std::setfill('0') << std::setw(2) << std::hex << std::uppercase << r
		<< std::setw(2) << g
		<< std::setw(2) << b;

	return ss.str();
}

inline void BaseIO::CreateBrepById(const vector<int> &_hashCode, const string &filePath)
{

	TopoDS_Compound compound;
	BRep_Builder builder;
	builder.MakeCompound(compound);
	for (auto& elem : _hashCode)
	{
		if (elem < componentStartId)
		{
			TopoDS_Shape data = idFacePropertyMap.FindFromKey(elem).faceObj;
			builder.Add(compound, data);
		}
		else
		{
			BRepTools::Write(idShapeMap.FindFromKey(elem), filePath.c_str());
			TopoDS_Shape data1 = idShapeMap.FindFromKey(elem);
			builder.Add(compound, data1);
		}
	}
	BRepTools::Write(compound, filePath.c_str());
	/*
	if (_id < componentStartId)
	{
		BRepTools::Write(idFacePropertyMap.FindFromKey(_id).faceObj, filePath.c_str());
	}
	else
	{
		BRepTools::Write(idShapeMap.FindFromKey(_id), filePath.c_str());
	}
	*/
}
inline void BaseIO::CreateCenterById(const int &_id, double &x, double &y, double &z)
{
	// 支持Face，不支持Component
	if (_id >= componentStartId)
	{
		return;
	}

	BRepGProp_Face analysisFace(idFacePropertyMap.FindFromKey(_id).faceObj);
	Standard_Real umin, umax, vmin, vmax;
	analysisFace.Bounds(umin, umax, vmin, vmax);
	Standard_Real midU = (umin + umax) / 2;
	Standard_Real midV = (vmin + vmax) / 2;
	gp_Vec norm;
	gp_Pnt midPoint;
	analysisFace.Normal(midU, midV, midPoint, norm);
	x = midPoint.X();
	y = midPoint.Y();
	z = midPoint.Z();
}

inline bool BaseIO::CreateStpById(const int &_id, const string &filePath)
{
	TopoDS_Shape aShape;

	if (_id < componentStartId)
	{
		aShape = idFacePropertyMap.FindFromKey(_id).faceObj;
	}
	else
	{
		aShape = idShapeMap.FindFromKey(_id);
	}

	return GeoAPIUtil::CreateStpByShape(aShape, filePath);
}

inline void BaseIO::GetEdgeMap(map<int, TopoDS_Edge>& edges)
{
	for (NCollection_IndexedDataMap<int, TopoDS_Edge>::Iterator aKeyEdgeIter(idEdgeMap); aKeyEdgeIter.More(); aKeyEdgeIter.Next())
	{
		edges.insert(make_pair(aKeyEdgeIter.Key(), aKeyEdgeIter.Value()));
	}
}

inline void BaseIO::GetDocument(Handle(TDocStd_Document)& document)
{
	document = doc;
}

#ifdef OCCTEST

inline void BaseIO::SetAISInteractiveContext(Handle(AIS_InteractiveContext) _myAISContext)
{
	myAISContext = _myAISContext;
}
inline void BaseIO::Render()
{

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