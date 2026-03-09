
/*******************************************************************************

�����ļ�˵����
1. RandomColorGenerator.h��
	����HSVɫ��ģʽ�����һ����ɫ�����������˵�һЩ���ʺ���ʾģ�͵���ɫ�����紿ɫ��
	���⣬���ɵ�������ɫ���Ҳ�ϴ󣬱�����ɫ��������

2. tcl_5.0.6
	���ṹ�Ŀ⣬���ڰ汾�Ƚ��ϣ�����DCiP����ʱ����һЩ����ʹ���
	һ���Ǽ̳�binary_function���ִ����������ֱ��ע�͵��
	һ����һЩallocate�ڴ�ľ��棬����ͨ�������ķ�ʽ����
	_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

һЩ����˵����
1. GetFaceMap����ȡhashcode��TopoDS_Face��ӳ��
2. GetJson����ȡ������ʾ���νṹ��Json����
3. CreateBrepById������id��ȡ��Face������Brep
4. CreateCenterById������id��ȡ��Face���������ĵ�

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

#include <BRepLProp_SLProps.hxx>
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
#include <Geom_Plane.hxx>//new add
#include <Bnd_OBB.hxx>//new add


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
	bool GetFaceArea(const int &_id, double &area);
	bool CalculateBoundingCircleArea(const int& _id, double& area);
	
	void SetSewModel(const bool &flag);
	void CreateBrepById(const vector<int>& _hashCode, const string &filePath);
	void CreateCenterById(const int &_id, double &x, double &y, double &z);
	void GetNormalById(const int &_id, double &nx, double &ny, double &nz);
	bool CreateStpById(const int &_id, const string &filePath);
	inline void GetEdgeMap(map<int, TopoDS_Edge>& edges);
	inline void GetDocument(Handle(TDocStd_Document)& document);
	NCollection_IndexedDataMap<TopoDS_Face, vector<int>> getFaceEdgeMap();
	NCollection_IndexedDataMap<TopoDS_Shape, std::string> getShapeNameMap();
	NCollection_IndexedDataMap<TopoDS_Shape, std::string> getShapeNameMap1();
	NCollection_Map<TopoDS_Shape, TopTools_ShapeMapHasher> getShapes();

	string GetJson(bool faceFlag = true);
	string GetJsonEx(bool faceFlag, vector<int>& faces, vector<int>& components, NCollection_DataMap<Standard_Integer, Standard_Integer>& faceID_ComponentIDMap);
	string jsonPath;

	//void GetFaceEdgeMap(NCollection_IndexedDataMap<int, NCollection_Vector<int>>& m) { m = faceEdgeMap; }//new add
protected:
	typedef NCollection_IndexedDataMap<TopoDS_Shape, CTree *, TopTools_ShapeMapHasher> IndexedDataMapOfShapeTreeNode;
	typedef NCollection_IndexedDataMap<TopoDS_Shape, CTree *, TopTools_ShapeMapHasher>::Iterator DataMapIteratorOfIndexedDataMapOfShapeTreeNode;
	typedef NCollection_IndexedDataMap<TopoDS_Face, FaceProperty, TopTools_ShapeMapHasher> IndexedDataMapOfFaceProperty;
	typedef NCollection_IndexedDataMap<TopoDS_Edge, int, TopTools_ShapeMapHasher> IndexedDataMapOfEdgeId;
	typedef NCollection_IndexedDataMap<TopoDS_Shape, int, TopTools_ShapeMapHasher> IndexedDataMapOfShapeId;
	NCollection_IndexedDataMap<TopoDS_Face, vector<int>> faceEdgeMap;//new add
	NCollection_IndexedDataMap<TopoDS_Shape, std::string> shapeNameMap;//new add
	NCollection_IndexedDataMap<TopoDS_Shape, std::string> shapeNameMap1;

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
	//	SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // ����������ɫΪ��ɫ
	//	std::cout << str << std::endl;
	//	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // ����ΪĬ����ɫ
	//}

#ifdef OCCTEST
public:
	void Render();
	void SetAISInteractiveContext(Handle(AIS_InteractiveContext) _myAISContext);
	Handle(AIS_InteractiveContext) myAISContext;
#endif
};


NCollection_IndexedDataMap<TopoDS_Face, vector<int>> BaseIO::getFaceEdgeMap()
{
	return faceEdgeMap;
}
NCollection_IndexedDataMap<TopoDS_Shape, std::string> BaseIO::getShapeNameMap()
{
	return shapeNameMap;
}
NCollection_IndexedDataMap<TopoDS_Shape, std::string> BaseIO::getShapeNameMap1()
{
	return shapeNameMap1;
}
NCollection_Map<TopoDS_Shape, TopTools_ShapeMapHasher> BaseIO::getShapes()
{
	return parseComponentCodes;
}
inline void BaseIO::SetStartId(const int &faceStartId, const int &componentStartId, const int &edgeStartId)
{
	this->faceStartId = faceStartId;
	this->componentStartId = componentStartId;
	this->edgeStartId = edgeStartId;
}
inline bool BaseIO::GetFaceArea(const int& _id, double& area)
{
	FaceProperty faceProp;
	if (idFacePropertyMap.FindFromKey(_id, faceProp))
	{
		if (faceProp.faceObj.IsNull())
			return false;

		GProp_GProps props;
		BRepGProp::SurfaceProperties(faceProp.faceObj, props);

		area = props.Mass();   // Mass = Area
		return true;
	}
	return false;
}
inline bool BaseIO::CalculateBoundingCircleArea(const int& _id, double& area)
{
	std::cout << "调用了CalculateBoundingCircleArea" << std::endl;
	FaceProperty faceProp;

	if (!idFacePropertyMap.FindFromKey(_id, faceProp))
	{
		return false;  // 找不到对应的面
	}

	if (faceProp.faceObj.IsNull())
	{
		return false;  // 面对象为空
	}

	// 获取面的几何曲面
	Handle(Geom_Surface) surface = BRep_Tool::Surface(faceProp.faceObj);

	if (surface.IsNull())
	{
		return false;  // 曲面为空
	}

	// 判断是否是平面
	if (surface->DynamicType() == STANDARD_TYPE(Geom_Plane))
	{
		// 平面：计算外接圆面积

		// 获取平面几何
		Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);

		// 获取平面的UV边界
		Standard_Real umin, umax, vmin, vmax;
		BRepTools::UVBounds(faceProp.faceObj, umin, umax, vmin, vmax);

		// 计算平面的4个角点
		gp_Pnt p1 = plane->Value(umin, vmin);
		gp_Pnt p2 = plane->Value(umax, vmin);
		gp_Pnt p3 = plane->Value(umax, vmax);
		gp_Pnt p4 = plane->Value(umin, vmax);

		// 计算平面的中心点
		gp_Pnt center = plane->Value((umin + umax) / 2, (vmin + vmax) / 2);

		// 计算到4个角点的最大距离（外接圆半径）
		double maxDist = 0.0;
		double dist;

		dist = p1.Distance(center);
		if (dist > maxDist) maxDist = dist;

		dist = p2.Distance(center);
		if (dist > maxDist) maxDist = dist;

		dist = p3.Distance(center);
		if (dist > maxDist) maxDist = dist;

		dist = p4.Distance(center);
		if (dist > maxDist) maxDist = dist;

		// 计算外接圆面积
		area = M_PI * maxDist * maxDist;
		std::cout << "计算平面外接圆面积---" << "面积为：" << area << std::endl;
		return true;
	}
	else
	{
		// 曲面：计算外接球对应的圆面积
		
		Bnd_Box bbox;
		BRepBndLib::Add(faceProp.faceObj, bbox);

		if (bbox.IsVoid())
		{
			return false;
		}

		// 获取包围盒的最小和最大点
		double xmin, ymin, zmin, xmax, ymax, zmax;
		bbox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

		// 计算包围盒对角线长度（外接球直径）
		double dx = xmax - xmin;
		double dy = ymax - ymin;
		double dz = zmax - zmin;
		double diameter = sqrt(dx * dx + dy * dy + dz * dz);

		// 外接球直径对应的圆面积
		double radius = diameter / 2.0;
		area = M_PI * radius * radius;

		std::cout << "计算曲面外接球面积---" << "面积为：" << area << std::endl;
		return true;
	}
}
inline void BaseIO::SetSewModel(const bool& flag)
{
	sewFlag = flag;
}

inline void BaseIO::Perform()
{
	colorGen.ResetRef();

	// �������¸�ֵ�������л��������¶���ʱ��id������ۼ�
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
	faceEdgeMap.Clear();
	shapeNameMap.Clear();
	shapeNameMap1.Clear();
	parseComponentCodes.Clear();


	int faceCount = 0;

	//����ģ�Ͱ���face��Ŀ��ȷ��ģ�͵Ĵ�С
	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		// ����һ��TopExp_Explorer�������ڱ�����״�е���
		TopExp_Explorer explorer(objShape, TopAbs_FACE);
		// �����и��������Ա�����ʱ��
		while (explorer.More()) {
			// ���������1
			faceCount++;
			// �ƶ�����һ����
			explorer.Next();
		}
	}

	//���ģ����Խ�С�����Բ��ýϸߵľ���
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

	// ��������
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

	//// ����ظ���Hashcode
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
	//			//�Ƚ��������������

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


	// �������е�Free Label������Tree
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

		// �����߻��ظ���������Ҫ�ж�һ�¼�ֵ�Ƿ��Ѿ������ˣ�����һ�������߻���������ͬ��id
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

		for (TopExp_Explorer faceExp(objShape, TopAbs_FACE); faceExp.More(); faceExp.Next())
		{
			const TopoDS_Face &objFace = TopoDS::Face(faceExp.Current());

			// ���������subshape������ɫ
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
			vector<int> edgeIds;
			for (TopExp_Explorer edgeExp(objFace, TopAbs_EDGE); edgeExp.More(); edgeExp.Next())
			{
				const TopoDS_Edge& objEdge = TopoDS::Edge(edgeExp.Current());

				int edgeId;
				edgeId = edgeIdMap.FindFromKey(objEdge);
				if (edgeIdMap.FindFromKey(objEdge, edgeId))
				{
					edgeIds.push_back(edgeId);
				}
			}
			if (edgeIds.size() != 0)
			{
				faceEdgeMap.Add(faceProp.faceObj, edgeIds);
			}
			else
			{
				std::cout << "empty" << std::endl;
			}
			
			//add end
		}
	}

	// �����������࣬face id��ComponentId�ظ�
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
	builder["emitUTF8"] = true; // ֱ����� UTF-8 �ַ�
	builder["indentation"] = "   ";
	//builder["indentation"] = ""; // ѹ����ʽ��û�л��кͲ���Ҫ�Ŀհ��ַ�
	// �������json��������
	if (!jsonPath.empty())
	{
		ofstream outfile(jsonPath.c_str());
		outfile << Json::writeString(builder, jsonNode) << endl;
		outfile.close();
	}
	return Json::writeString(builder, jsonNode);
}

// �ȽϺ��������ڱȽ�����Json::Value����Ԫ�أ���������Ƚϵ�����ֵ����Ԫ�أ���ɰ����޸ıȽ��߼���
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
// �ݹ麯�������ڴ���JSON�ṹ�����е�����Ԫ������
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

		// �ݹ鴦�������е�ÿ��Ԫ�أ����Ԫ���Ƕ���������飩
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

	// ��������Face
	for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
	{
		TopoDS_Face &objFace = const_cast<TopoDS_Face &> (TopoDS::Face(faceExp.Current()));
		TopLoc_Location location;
		opencascade::handle<Poly_Triangulation> triFace = BRep_Tool::Triangulation(objFace, location);

		if (triFace.IsNull())
		{
			continue;
			/*
			// ������Ϊ���޸����ϸ��������Ĵ�ʩ��ʵ�ʲ����м�����������Ա��޸���
			// ���ڲ�̬���漴ʹ�޸���õ�������Ҳ������ǲ�̬���棬�������ע�͵��
			// ǿ�д򿪳���ᱨ�����Ϊ���ܵ�FaceFacepropertyMap�� edgeIdMap��Ӱ�죻
			// ��Ҫ���¶���FaceFacepropertyMap�� edgeIdMap��������
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

		// ���������subshape������ɫ
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

		// ��Face���뵽�ڵ�
		TreeLabel _node(objFace, TreeFace);
		_node.id = pFaceProp->id;
		treeNode->insert(_node);
	}
}

// ���������޸���������
inline TopoDS_Face BaseIO::FixFace(const TopoDS_Face& face)
{
	// ����ShapeFix_Shape�������޸���״
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
	// ����Ƿ���Face
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

		// �ж��Ƿ�����ײ��Shape������ǣ��ŵ�parseComponentCodes�У���������Build
		if (isLowestComponent)
		{
			TCollection_ExtendedString nameExt;
			Handle(TDataStd_Name) aNameAttr;
			if (aLabel.FindAttribute(TDataStd_Name::GetID(), aNameAttr)) {
				nameExt = aNameAttr->Get();
				TCollection_AsciiString asciiName(nameExt); // תΪ ASCII �ַ���
				shapeNameMap1.Add(aSubshape, asciiName.ToCString());
			}
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
		//����ΪСд������DCiP����ʶ��
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

	// 初始化 Compound 和 Builder
	TopoDS_Compound compound;
	BRep_Builder builder;
	builder.MakeCompound(compound);

	// 遍历 _hashCode (假设这是存储 ID 的容器，如 vector<int> 或 set<int>)
	for (auto& elem : _hashCode)
	{
		TopoDS_Shape shapeToAdd; // 初始化为空形状 (Null Shape)

		// --- 查找逻辑开始 ---

		// 1. 优先在 idFacePropertyMap 中查找
		if (idFacePropertyMap.Contains(elem))
		{
			// 获取 FaceProperty 对象并提取内部的 TopoDS_Face
			const FaceProperty& prop = idFacePropertyMap.FindFromKey(elem);
			shapeToAdd = prop.faceObj;
		}
		else if (idShapeMap.Contains(elem))
		{
			shapeToAdd = idShapeMap.FindFromKey(elem);
		}
		if (!shapeToAdd.IsNull())
		{
			builder.Add(compound, shapeToAdd);
		}
	}

	// 循环结束后，一次性将所有形状写入文件
	// 即使 compound 为空，这也是合法的，会生成一个空文件而不会崩
	BRepTools::Write(compound, filePath.c_str());
}


inline void BaseIO::GetNormalById(const int& _id, double& nx, double& ny, double& nz)
{
		if (!idFacePropertyMap.Contains(_id)) return;
		cout << "GetNormalById" << endl;
		// 1. 获取 TopoDS_Face
		const TopoDS_Face& face = idFacePropertyMap.FindFromKey(_id).faceObj;

		// 2. 使用 BRepAdaptor_Surface 适配器 (它是处理面几何的标准工具)
		BRepAdaptor_Surface adaptor(face);

		// 3. 获取 UV 范围并计算中点
		// 注意：更严谨的做法是用 BRepClass_FaceClassifier 来找一个确保在面内的点
		// 但为了性能，如果面形状不复杂，取中点通常可以接受。
		Standard_Real umin = adaptor.FirstUParameter();
		Standard_Real umax = adaptor.LastUParameter();
		Standard_Real vmin = adaptor.FirstVParameter();
		Standard_Real vmax = adaptor.LastVParameter();
		Standard_Real midU = (umin + umax) / 2.0;
		Standard_Real midV = (vmin + vmax) / 2.0;

		// 4. 计算该点的几何属性 (P点，法线等)
		// 2 表示需要计算二阶导数以求法线（其实只求一阶导做叉乘也可以，但SLProps封装好了）
		// 1e-7 是精度
		BRepLProp_SLProps props(adaptor, midU, midV, 2, 1e-7);

		if (props.IsNormalDefined())
		{
			// 获取法线
			gp_Dir normalDir = props.Normal(); // gp_Dir 自动是归一化的 (长度为1)

			// 5. 关键：处理拓扑方向 (Orientation)
			// 如果面的定义是反向的，我们需要把法线反过来
			if (face.Orientation() == TopAbs_REVERSED)
			{
				normalDir.Reverse();
			}

			nx = normalDir.X();
			ny = normalDir.Y();
			nz = normalDir.Z();
		}
		else
		{
			// 极少数情况（如奇点），法线未定义，设为默认值
			nx = 0.0; ny = 1.0; nz = 0.0;
		}
}

inline void BaseIO::CreateCenterById(const int &_id, double &x, double &y, double &z)
{
		// ֧��Face����֧��Component
		if (_id >= componentStartId)
		{
			return;
		}

		if (!idFacePropertyMap.Contains(_id))
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