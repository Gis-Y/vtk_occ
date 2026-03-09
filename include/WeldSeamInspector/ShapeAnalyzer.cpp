// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _USE_MATH_DEFINES
#include <cmath>

#include <GeomAbs_SurfaceType.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRep_Tool.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <GeomLib.hxx>
#include <GeomAPI.hxx>
#include <TopoDS.hxx>
#include <GeomLib_IsPlanarSurface.hxx>
#include <gp_Vec.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepIntCurveSurface_Inter.hxx>
#include <IntTools_EdgeEdge.hxx>
#include <GeomLProp_SLProps.hxx>
#include <ProjLib.hxx>
#include <IntAna2d_AnaIntersection.hxx>
#include <Geom_Ellipse.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <Geom_BezierCurve.hxx>
#include <iostream>
#include <iomanip>
#include<BRepAlgoAPI_Common.hxx>
#include<GeomAPI_IntCS.hxx>
#include<Geom_Curve.hxx>
#include<BRepAlgoAPI_Section.hxx>
#include<BRepClass_FaceClassifier.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <IntCurvesFace_ShapeIntersector.hxx>
#include <TopAbs_State.hxx>
#include <gp_Pnt.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include<GeomLib_IsPlanarSurface.hxx>
#include <Geom_BSplineCurve.hxx>
#include <gce_MakeCirc.hxx>

#include <TopoDS_Shape.hxx>
#include <omp.h>

#include <unordered_map>
#include <mutex>
#include <map>
#include <TDataStd_Name.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>

#include<unordered_set>
#include<set>
#include <algorithm>

// 添加缺失的OpenCASCADE头文件
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <gp_Pln.hxx>
#include <gp_Dir.hxx>
#include <Precision.hxx>
#include <Standard_Version.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <TDF_Label.hxx>
#include <TDF_LabelSequence.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

#include "ModelIO/ModelIO.h"

// 包围盒 + 索引
struct BoundingVolume {
	Bnd_Box box;
	int index;
};

struct CachedFaceInfo {
	gp_Pln  pln;
	gp_Dir  n;        // 已统一方向
	Bnd_Box box;      // 面包围盒
	bool    ok = false;
};

enum FaceType {
	rectangle,
	ring,
	unknown
};

// 存放结果：每个匹配记录两个形状的索引，以及对应的面和边
struct IntersectRecord {
	int shapeIndex1;
	int shapeIndex2;
	TopoDS_Face face1;
	TopoDS_Face face2;
	TopoDS_Edge edge;
};

// 形状信息结构体
struct ShapeInfo {
	std::string name;                    // 形状名称
	TopoDS_Shape shape;                  // 形状对象
	std::vector<TopoDS_Face> faces;      // 形状的所有面
	std::vector<TopoDS_Edge> edges;      // 形状的所有边
	Bnd_Box boundingBox;                 // 包围盒
};

// 检测结果结构体
struct DetectionResult {
	std::string shape1Name;              // 第一个形状名称
	std::string shape2Name;              // 第二个形状名称
	TopoDS_Shape shape1;                 // 第一个形状对象
	TopoDS_Shape shape2;                 // 第二个形状对象
	std::vector<TopoDS_Face> attachedFaces1; // 第一个形状的相关面
	std::vector<TopoDS_Face> attachedFaces2; // 第二个形状的相关面
	double distance;                     // 两个形状间的距离

	enum RelationType {
		FACE_ATTACHED,      // 面贴合
		PARALLEL_CLOSE      // 平行且接近
	} relationType;

	std::string relationDescription;     // 关系描述
};

// 形状分析器类
class ShapeAnalyzer {
private:
	Handle(TDocStd_Document) doc;
	// 缓存相关
	static thread_local std::unordered_map<const void*, CachedFaceInfo> g_faceCache;
	

public:
	// 构造函数
	ShapeAnalyzer() = default;

	// 析构函数  
	~ShapeAnalyzer() = default;



	// 计算一个面（TopoDS_Shape）的面积
	double computeFaceArea(const TopoDS_Shape& shape);

	// 检查两个面是否有重合，并返回重合面积
	double computeOverlappingArea(const TopoDS_Face& face1, const TopoDS_Face& face2);

	// 找到相交的非垂直面
	std::vector<TopoDS_Face> FindIntersectingNonPerpendicularFaces(
		const TopoDS_Shape& shape1,
		const TopoDS_Face& face1,
		double angleTolDeg = 5.0,
		double distTol = 0.5
	);

	// 获取面积
	double GetFaceArea(const TopoDS_Face& face);

	// 检查两个shape之间的所有面，找到重合部分并计算面积
	double detectFaceOverlapAndArea(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2,
		TopoDS_Face& face1, TopoDS_Face& face2, std::vector<TopoDS_Face>& merged);

	// 计算两个 TopoDS_Shape 的最小距离
	double computeMinimumDistance(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2);

	// 分析面形状
	int analyzeFaceShape(const TopoDS_Face& face, double& distance);

	// 获取所有的shape
	std::vector<TopoDS_Shape> GetShape();

	// 从一个Shape中提取所有面和边
	void GetFacesAndEdges(const TopoDS_Shape& shape,
		std::vector<TopoDS_Face>& faces,
		std::vector<TopoDS_Edge>& edges);

	// 检查面和边是否相交
	bool IsFaceEdgeIntersecting1(const TopoDS_Face& face,
		const TopoDS_Edge& edge,
		double tolerance = 1e-3);

	bool IsFaceEdgeIntersecting(const TopoDS_Face& face,
		const TopoDS_Edge& edge,
		double tolerance = 1e-3);

	// 辅助：判断两个面／边的包围盒是否相交
	bool BBoxIntersect(const TopoDS_Shape& a, const TopoDS_Shape& b);

	// 返回相交次数
	int CountFaceEdgeIntersections(
		const std::vector<TopoDS_Face>& faces,
		const std::vector<TopoDS_Edge>& edges,
		int maxCount,
		TopoDS_Face& outFace,
		TopoDS_Edge& outEdge,
		double tol = 1e-3);

	// 建立 AABB 索引
	std::vector<BoundingVolume> BuildAABBIndex(const std::vector<TopoDS_Shape>& shapes);

	// 查询可能与 i 相交的索引
	std::vector<int> QueryPotentialOverlaps(
		const BoundingVolume& target,
		const std::vector<BoundingVolume>& bvh);

	std::vector<int> QueryPotentialUnoverlaps(
		const BoundingVolume& target,
		const std::vector<BoundingVolume>& bvh);



	// 包围盒最小距离
	double BBoxMinDistance(const Bnd_Box& a, const Bnd_Box& b);

	// 查找包含边的面
	std::vector<TopoDS_Face> FindFacesContainingEdge(
		const std::vector<TopoDS_Face>& faces,
		const TopoDS_Edge& edge);


	// 主要处理函数
	void Process();

	// 获取形状名称
	std::string GetShapeName(const TDF_Label& label, int index);

	// 输出形状映射信息
	void PrintShapeMapping();

	// 新增功能：检测形状关系
	std::vector<DetectionResult> DetectShapeRelations(
		double maxDistance = 5.0,           // 最大检测距离
		double overlapAreaThreshold = 1e-7, // 重合面积阈值
		double parallelTolerance = 1e-3     // 平行度容差
	);

	// 输出检测结果
	void PrintDetectionResults(const std::vector<DetectionResult>& results);


	// 获取所有形状信息（包含名称）
	std::vector<ShapeInfo> GetAllShapeInfo();

	// 辅助函数：获取指定检测结果中的面对信息
	void GetFacePairs(const DetectionResult& result,
		std::vector<std::pair<TopoDS_Face, TopoDS_Face>>& facePairs);

	// 判断投影是否至少部分落在目标面上
	bool IsProjectionOnTargetFace(const TopoDS_Face& sourceFace,
		const TopoDS_Face& targetFace,
		int numSample = 10);

	static bool GetCachedFaceInfo(const TopoDS_Face& F, CachedFaceInfo& out);

	bool AreFacesParallelAndClose(const TopoDS_Face& f1,
		const TopoDS_Face& f2,
		double maxDist = 5.0,
		double angTol = 1e-3);
	
	// 设置文档对象
	void SetDocument(const Handle(TDocStd_Document)& doc);
	void getShapeRelation(const Handle(TDocStd_Document)& doc, std::vector<DetectionResult>& detectionResults, double maxDistance = 5., double overlapTolerance = 1e-7, double parallelTolerance = 1e-3);
	void GetCheckFace(Handle(TDocStd_Document)& document, vector<TopoDS_Face>& faces, double maxDistance);
	// 获取文档对象
	Handle(TDocStd_Document) GetDocument() const;
};

// 静态成员变量定义
thread_local std::unordered_map<const void*, CachedFaceInfo> ShapeAnalyzer::g_faceCache;



double ShapeAnalyzer::computeFaceArea(const TopoDS_Shape& shape)
{
	GProp_GProps props;
	BRepGProp::SurfaceProperties(shape, props);
	return props.Mass(); // 对面来说，Mass 就是面积
}

double ShapeAnalyzer::computeOverlappingArea(const TopoDS_Face& face1, const TopoDS_Face& face2)
{
	BRepAlgoAPI_Common common(face1, face2);
	common.Build();

	if (!common.IsDone())
	{
		std::cerr << "Intersection failed!" << std::endl;
		return 0.0;
	}

	TopoDS_Shape commonShape = common.Shape();
	if (commonShape.IsNull())
	{
		return 0.0; // 没有交集
	}

	// 有些时候交集可能是边或点，要判断是否真的是面
	TopExp_Explorer exp(commonShape, TopAbs_FACE);
	if (!exp.More())
	{
		return 0.0; // 交集不是面
	}

	double totalArea = 0.0;
	for (; exp.More(); exp.Next())
	{
		TopoDS_Face overlapFace = TopoDS::Face(exp.Current());
		double area = computeFaceArea(overlapFace);
		if (area > Precision::Confusion()) // 排除非常小的数值误差
		{
			totalArea += area;
		}
	}

	return totalArea;
}

std::vector<TopoDS_Face> ShapeAnalyzer::FindIntersectingNonPerpendicularFaces(
	const TopoDS_Shape& shape1,
	const TopoDS_Face& face1,
	double angleTolDeg,         // 容许角度误差：5°
	double distTol              // 面距离小于此视为相交
)
{
	std::vector<TopoDS_Face> result;

	// 获取 face1 中心点处法向
	Handle(Geom_Surface) surf1 = BRep_Tool::Surface(face1);
	Standard_Real u1, u2, v1, v2;
	BRepTools::UVBounds(face1, u1, u2, v1, v2);
	double uc = (u1 + u2) / 2;
	double vc = (v1 + v2) / 2;
	GeomLProp_SLProps props1(surf1, uc, vc, 1, Precision::Confusion());
	if (!props1.IsNormalDefined()) return result;
	gp_Dir n1 = props1.Normal();

	const double angle90 = 90.0;
	const double angleLow = angle90 - angleTolDeg;
	const double angleHigh = angle90 + angleTolDeg;

	for (TopExp_Explorer exp(shape1, TopAbs_FACE); exp.More(); exp.Next())
	{
		TopoDS_Face faceX = TopoDS::Face(exp.Current());

		if (faceX.IsSame(face1)) continue;

		Handle(Geom_Surface) surfX = BRep_Tool::Surface(faceX);
		Standard_Real u3, u4, v3, v4;
		BRepTools::UVBounds(faceX, u3, u4, v3, v4);
		double ucX = (u3 + u4) / 2;
		double vcX = (v3 + v4) / 2;
		GeomLProp_SLProps propsX(surfX, ucX, vcX, 1, Precision::Confusion());
		if (!propsX.IsNormalDefined()) continue;

		gp_Dir nX = propsX.Normal();
		double angleDeg = n1.Angle(nX) * 180.0 / M_PI;

		// 角度太接近 90°，跳过
		if (angleDeg > angleLow && angleDeg < angleHigh)
			continue;

		// 判断是否相交
		BRepExtrema_DistShapeShape distCalc(face1, faceX);
		distCalc.SetDeflection(0.1);
		distCalc.Perform();
		if (!distCalc.IsDone()) continue;

		double d = distCalc.Value();
		if (d < distTol) {
			result.push_back(faceX);
		}
	}

	return result;
}

double ShapeAnalyzer::GetFaceArea(const TopoDS_Face& face)
{
	GProp_GProps system;
	BRepGProp::SurfaceProperties(face, system);
	return system.Mass();  // 面积
}

double ShapeAnalyzer::detectFaceOverlapAndArea(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2,
	TopoDS_Face& face1, TopoDS_Face& face2, std::vector<TopoDS_Face>& merged)
{
	double overlapArea = 0;
	for (TopExp_Explorer exp1(shape1, TopAbs_FACE); exp1.More(); exp1.Next())
	{
		face1 = TopoDS::Face(exp1.Current());

		for (TopExp_Explorer exp2(shape2, TopAbs_FACE); exp2.More(); exp2.Next())
		{
			face2 = TopoDS::Face(exp2.Current());

			overlapArea = computeOverlappingArea(face1, face2);
			if (overlapArea > Precision::Confusion())
			{
				std::vector<TopoDS_Face> faces1, faces2;
				if (GetFaceArea(face1) < 700)
				{
					faces1 = FindIntersectingNonPerpendicularFaces(shape1, face1);
				}
				if (GetFaceArea(face2) < 700)
				{
					faces2 = FindIntersectingNonPerpendicularFaces(shape2, face2);
				}
				merged = faces1;
				merged.insert(merged.end(), faces2.begin(), faces2.end());
				return overlapArea;
			}
		}
	}
	return overlapArea;
}

double ShapeAnalyzer::computeMinimumDistance(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2)
{
	BRepExtrema_DistShapeShape distShapeShape(shape1, shape2);
	distShapeShape.Perform();

	if (!distShapeShape.IsDone())
	{
		std::cerr << "Distance computation failed." << std::endl;
		return -1.0; // 出错
	}

	return distShapeShape.Value(); // 返回最小距离
}

int ShapeAnalyzer::analyzeFaceShape(const TopoDS_Face& face, double& distance)
{
	std::vector<gp_Circ> circles;
	std::vector<TopoDS_Edge> lines;
	int otherline = 0;

	TopExp_Explorer exp(face, TopAbs_EDGE);
	while (exp.More())
	{
		TopoDS_Edge edge = TopoDS::Edge(exp.Current());
		BRepAdaptor_Curve curve(edge);
		GeomAbs_CurveType type = curve.GetType();

		if (type == GeomAbs_Circle)
		{
			circles.push_back(curve.Circle());
		}
		else if (type == GeomAbs_Line)
		{
			lines.push_back(edge);
		}
		else if (type == GeomAbs_BSplineCurve)
		{
			// 对 BSpline 做拟合判断
			Standard_Real first, last;
			Handle(Geom_Curve) gc = BRep_Tool::Curve(edge, first, last);
			if (gc.IsNull()) {
				++otherline;
				exp.Next(); continue;
			}

			// 三点拟合
			gp_Pnt p1, p2, p3;
			Standard_Real u1 = gc->FirstParameter();
			Standard_Real u2 = gc->LastParameter();
			Standard_Real umid = (u1 + u2) / 2;

			gc->D0(u1, p1);
			gc->D0(umid, p2);
			gc->D0(u2, p3);

			// 判断是否近似直线：中点在首尾连线上的投影距离很小
			gp_Vec v1(p1, p3);
			gp_Vec v2(p1, p2);
			double projLen = v2.Dot(v1.Normalized());       // 投影长度
			gp_Pnt proj = p1.Translated(v1.Normalized() * projLen);
			if (proj.Distance(p2) < 0.01) {                  // 允许误差 0.01 mm
				lines.push_back(edge);
			}
			else {
				// 否则认为是圆弧（可选也可拟合 gp_Circ）
				try {
					gp_Circ c = gce_MakeCirc(p1, p2, p3).Value();  // 三点圆拟合
					circles.push_back(c);
				}
				catch (...) {
					++otherline; // 拟合失败
				}
			}
		}
		else
		{
			++otherline;
		}

		exp.Next();
	}

	// 圆环面：两个圆形边
	if (circles.size() >= 2 && circles.size() <= 4 && lines.size() <= 2 && otherline == 0)
	{
		gp_Pnt c1 = circles[0].Location();
		gp_Pnt c2 = circles[1].Location();
		Standard_Real r1 = circles[0].Radius();
		Standard_Real r2 = circles[1].Radius();

		// 判断两个圆是否同心
		if (c1.Distance(c2) < 2)
		{
			distance = std::abs(r1 - r2);
			return ring;
		}
	}

	// 矩形面：四条直线边
	if (lines.size() == 4)
	{
		GProp_GProps gProps;
		BRepGProp::SurfaceProperties(face, gProps);
		double area = gProps.Mass();

		// 提取边界长宽
		std::vector<double> lengths;
		TopExp_Explorer exp2(face, TopAbs_EDGE);
		while (exp2.More())
		{
			TopoDS_Edge edge = TopoDS::Edge(exp2.Current());
			BRepAdaptor_Curve curve(edge);
			double length = curve.LastParameter() - curve.FirstParameter();
			lengths.push_back(length);
			exp2.Next();
		}

		if (lengths.size() == 4)
		{
			std::sort(lengths.begin(), lengths.end());
			double width = (lengths[0] + lengths[1]) / 2.0;
			double height = (lengths[2] + lengths[3]) / 2.0;

			if (width > 1e-6 && height > 1e-6)
			{
				distance = height / width;
				return rectangle;
			}
		}
	}
	return unknown;
}

std::vector<TopoDS_Shape> ShapeAnalyzer::GetShape()
{
	Handle(XCAFDoc_ShapeTool)ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	TDF_LabelSequence Labels;
	ST->GetFreeShapes(Labels);
	int i = Labels.Length();
	std::vector<TopoDS_Shape> shapes;

	for (int idx = 1; idx <= Labels.Length(); ++idx)
	{
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		for (TopExp_Explorer exp(objShape, TopAbs_SOLID); exp.More(); exp.Next())
		{
			TopoDS_Shape subShape = exp.Current();
			// 此处可以对 subShape 进一步转换，比如 TopoDS::Solid(subShape)
			shapes.push_back(subShape);
		}
	}
	return shapes;
}

void ShapeAnalyzer::GetFacesAndEdges(const TopoDS_Shape& shape,
	std::vector<TopoDS_Face>& faces,
	std::vector<TopoDS_Edge>& edges)
{
	// 遍历面
	for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
		TopoDS_Face face = TopoDS::Face(exp.Current());
		faces.push_back(face);
	}
	// 遍历边
	for (TopExp_Explorer exp(shape, TopAbs_EDGE); exp.More(); exp.Next()) {
		TopoDS_Edge edge = TopoDS::Edge(exp.Current());
		edges.push_back(edge);
	}
}

bool ShapeAnalyzer::IsFaceEdgeIntersecting1(const TopoDS_Face& face,
	const TopoDS_Edge& edge,
	double tolerance)
{
	// 粗筛
	BRepExtrema_DistShapeShape distCalc(face, edge);
	distCalc.Perform();
	if (!distCalc.IsDone()) return false;

	if (distCalc.Value() > tolerance)
		return false;

	// 精筛（保留原来的 Section 检查）
	BRepAlgoAPI_Section section(edge, face);
	section.ComputePCurveOn1(Standard_True);
	section.Approximation(Standard_True);
	section.Build();

	if (!section.IsDone() || section.Shape().IsNull())
		return false;

	TopExp_Explorer exp(section.Shape(), TopAbs_EDGE);
	double totalOverlapLength = 0.0;

	while (exp.More()) {
		TopoDS_Edge e = TopoDS::Edge(exp.Current());
		GProp_GProps props;
		BRepGProp::LinearProperties(e, props);
		totalOverlapLength += props.Mass();
		exp.Next();
	}

	return totalOverlapLength > tolerance;
}

bool ShapeAnalyzer::IsFaceEdgeIntersecting(const TopoDS_Face& face,
	const TopoDS_Edge& edge,
	double tolerance)
{
	// 使用 BRepAlgoAPI_Section 求交集
	BRepAlgoAPI_Section section(edge, face);
	section.ComputePCurveOn1(Standard_True);
	section.Approximation(Standard_True);
	section.Build();

	if (!section.IsDone() || section.Shape().IsNull()) {
		std::cout << "Intersection computation failed." << std::endl;
		return false;
	}

	// 遍历交集结果中的所有边，并累计重合部分的长度
	double totalOverlapLength = 0.0;
	TopExp_Explorer exp(section.Shape(), TopAbs_EDGE);

	while (exp.More()) {
		TopoDS_Shape intersectEdge = exp.Current();
		// 将交集结果转换为 Edge
		TopoDS_Edge e = TopoDS::Edge(intersectEdge);

		// 计算当前交集边的长度
		GProp_GProps props;
		BRepGProp::LinearProperties(e, props);
		double length = props.Mass();  // 对于线性属性，Mass() 返回长度

		totalOverlapLength += length;
		exp.Next();
	}

	// 当累计的重合部分总长度大于容差值时，认为边与面存在重合部分
	if (totalOverlapLength > tolerance) {
		return true;
	}
	else {
		return false;
	}
}

bool ShapeAnalyzer::BBoxIntersect(const TopoDS_Shape& a, const TopoDS_Shape& b) {
	Bnd_Box boxA, boxB;
	BRepBndLib::Add(a, boxA);
	BRepBndLib::Add(b, boxB);
	return !boxA.IsOut(boxB);
}

int ShapeAnalyzer::CountFaceEdgeIntersections(
	const std::vector<TopoDS_Face>& faces,
	const std::vector<TopoDS_Edge>& edges,
	int maxCount,
	TopoDS_Face& outFace,
	TopoDS_Edge& outEdge,
	double tol) {

	std::vector<TopoDS_Edge> seen;
	seen.reserve(maxCount);
	int count = 0;
	for (auto& f : faces) {
		for (auto& e : edges) {
			if (!this->BBoxIntersect(f, e))
				continue;
			if (this->IsFaceEdgeIntersecting(f, e, tol)) {
				// 去重
				bool dup = std::any_of(seen.begin(), seen.end(),
					[&](const TopoDS_Edge& se) { return se.IsSame(e); });
				if (!dup) {
					seen.push_back(e);
					outFace = f;
					outEdge = e;
					if (++count > maxCount)
						return count;
				}
			}
		}
	}
	return count;
}

std::vector<BoundingVolume> ShapeAnalyzer::BuildAABBIndex(const std::vector<TopoDS_Shape>& shapes) {
	std::vector<BoundingVolume> bvh;
	for (int i = 0; i < shapes.size(); ++i) {
		Bnd_Box box;
		BRepBndLib::Add(shapes[i], box);
		
		// 取消扩展，使用最紧致的包围盒
		box.SetGap(0.0);
		
		bvh.push_back({ box, i });
	}
	return bvh;
}

std::vector<int> ShapeAnalyzer::QueryPotentialOverlaps(
	const BoundingVolume& target,
	const std::vector<BoundingVolume>& bvh) {

	std::vector<int> result;
	for (const auto& other : bvh) {
		if (target.index >= other.index) continue;  // 恢复原来的索引过滤，避免重复检测
		
		// 只检测包围盒重叠，保持职责单一
		if (!target.box.IsOut(other.box)) {
			result.push_back(other.index);
		}
	}
	return result;
}

std::vector<int> ShapeAnalyzer::QueryPotentialUnoverlaps(
	const BoundingVolume& target,
	const std::vector<BoundingVolume>& bvh) {

	std::vector<int> result;
	for (const auto& other : bvh) {
		if (target.index >= other.index) continue;  // 恢复原来的索引过滤，避免重复检测
		
		// 只检测包围盒不重叠，保持职责单一
		if (target.box.IsOut(other.box)) {
			result.push_back(other.index);
		}
	}
	return result;
}




bool ShapeAnalyzer::GetCachedFaceInfo(const TopoDS_Face& F, CachedFaceInfo& out)
{
	const void* key = F.TShape().get();          // 唯一指针
	auto it = g_faceCache.find(key);
	if (it != g_faceCache.end()) { out = it->second; return out.ok; }

	CachedFaceInfo info;                         // 第一次计算
	BRepBndLib::Add(F, info.box);

	BRepAdaptor_Surface adapt(F, Standard_True);
	if (adapt.GetType() == GeomAbs_Plane) {
		info.pln = adapt.Plane();
		info.n = info.pln.Axis().Direction();
		info.ok = true;
	}
	else {
		Handle(Geom_Surface) S = BRep_Tool::Surface(F);
		if (!S.IsNull()) {
			GeomLib_IsPlanarSurface tst(S, Precision::Confusion());
			if (tst.IsPlanar()) {
				info.pln = tst.Plan();
				info.n = info.pln.Axis().Direction();
				info.ok = true;
			}
		}
	}
	g_faceCache.emplace(key, info);              // 写入本线程 cache
	out = info;
	return info.ok;
}

// 判断投影是否至少部分落在目标面上
bool ShapeAnalyzer::IsProjectionOnTargetFace(const TopoDS_Face& sourceFace,
	const TopoDS_Face& targetFace,
	int numSample)
{
	// 获取源面的法向量（我们用面的中心点）
	Handle(Geom_Surface) sourceSurf = BRep_Tool::Surface(sourceFace);
	GeomLProp_SLProps props(sourceSurf, 0.5, 0.5, 1, 1e-6);
	if (!props.IsNormalDefined()) return false;
	gp_Dir normal = props.Normal();

	// 遍历源面边界的每条边
	TopExp_Explorer edgeExplorer(sourceFace, TopAbs_EDGE);
	for (; edgeExplorer.More(); edgeExplorer.Next()) {
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());

		// 拿到边的曲线
		Standard_Real first, last;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
		if (curve.IsNull()) continue;

		for (int i = 0; i <= numSample; ++i) {
			double t = first + (last - first) * i / numSample;
			gp_Pnt pt = curve->Value(t);

			// 投影点沿法线方向
			gp_Vec projVec(normal);
			pt.Translate(projVec);  // 可放大偏移量以防数值问题

			// 检查点是否落在目标面上
			BRepClass_FaceClassifier classifier;
			classifier.Perform(targetFace, pt, 1e-6);
			if (classifier.State() != TopAbs_OUT) {
				return true; // 有投影落在目标面上
			}
		}
	}

	return false; // 所有投影点都在面外
}

bool ShapeAnalyzer::AreFacesParallelAndClose(const TopoDS_Face& f1,
	const TopoDS_Face& f2,
	double maxDist ,
	double angTol )
{
	CachedFaceInfo a, b;
	if (!GetCachedFaceInfo(f1, a) || !GetCachedFaceInfo(f2, b))
		return false;

	/* ---------- 1. 平行 + 法向距离 ---------- */
	if (!a.n.IsParallel(b.n, angTol))
		return false;

	gp_Dir n = a.n;                      // 统一方向
	if (n.Dot(b.n) < 0) n.Reverse();

	gp_XYZ nn = n.XYZ();
	double d1 = -nn.Dot(a.pln.Location().XYZ());
	double d2 = -nn.Dot(b.pln.Location().XYZ());
	double d_perp = std::abs(d2 - d1);
	if (d_perp > maxDist)          // 快速筛
		return false;

	/* ---------- 2. 共面时再算平面内最近距离 ---------- */
	const double coplanarTol = 0.05;
	if (d_perp <= coplanarTol) {
		BRepExtrema_DistShapeShape ds(f1, f2);
		ds.SetDeflection(0.1);
		ds.Perform();
		if (!ds.IsDone() || ds.Value() > maxDist) return false;
	}


	bool ok = IsProjectionOnTargetFace(f1, f2);
	if (!ok) return false;

	return true;
}




double ShapeAnalyzer::BBoxMinDistance(const Bnd_Box& a, const Bnd_Box& b) {
	Standard_Real xmin1, ymin1, zmin1, xmax1, ymax1, zmax1;
	Standard_Real xmin2, ymin2, zmin2, xmax2, ymax2, zmax2;
	a.Get(xmin1, ymin1, zmin1, xmax1, ymax1, zmax1);
	b.Get(xmin2, ymin2, zmin2, xmax2, ymax2, zmax2);
	double dx = std::max({ xmin2 - xmax1, xmin1 - xmax2, 0.0 });
	double dy = std::max({ ymin2 - ymax1, ymin1 - ymax2, 0.0 });
	double dz = std::max({ zmin2 - zmax1, zmin1 - zmax2, 0.0 });
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

std::vector<TopoDS_Face> ShapeAnalyzer::FindFacesContainingEdge(
	const std::vector<TopoDS_Face>& faces,
	const TopoDS_Edge& edge) {

	std::vector<TopoDS_Face> result;

	for (const TopoDS_Face& face : faces) {
		for (TopExp_Explorer ex(face, TopAbs_EDGE); ex.More(); ex.Next()) {
			const TopoDS_Edge& e = TopoDS::Edge(ex.Current());
			if (e.IsSame(edge)) {
				result.push_back(face);
				break;
			}
		}
	}

	return result;
}



std::string ShapeAnalyzer::GetShapeName(const TDF_Label& label, int index) {
	Handle(TDataStd_Name) nameAttr;
	static int SOLIDCount = 0;
	if (label.FindAttribute(TDataStd_Name::GetID(), nameAttr)) {
		TCollection_ExtendedString extName = nameAttr->Get();
		if (extName == "SOLID")
		{
			extName = extName + "_" + SOLIDCount;
			SOLIDCount++;
		}
		TCollection_AsciiString asciiName(extName);
		return std::string(asciiName.ToCString());
	}
	return "Shape_" + std::to_string(index);
}

void ShapeAnalyzer::PrintShapeMapping() {
	std::cout << "\n=== 形状映射信息 ===" << std::endl;
	Handle(XCAFDoc_ShapeTool) ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
	TDF_LabelSequence Labels;
	ST->GetFreeShapes(Labels);

	int shapeIndex = 0;
	for (int idx = 1; idx <= Labels.Length(); ++idx) {
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		for (TopExp_Explorer exp(objShape, TopAbs_SOLID); exp.More(); exp.Next()) {
			std::string shapeName = GetShapeName(theLabel, shapeIndex);
			std::cout << "形状 " << shapeIndex << ": " << shapeName << std::endl;
			shapeIndex++;
		}
	}
}




std::vector<ShapeInfo> ShapeAnalyzer::GetAllShapeInfo() {
	std::vector<ShapeInfo> shapeInfos;
	Handle(XCAFDoc_ShapeTool) ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
	TDF_LabelSequence Labels;
	ST->GetFreeShapes(Labels);

	int shapeIndex = 0;
	for (int idx = 1; idx <= Labels.Length(); ++idx) {
		TDF_Label theLabel = Labels.ChangeValue(idx);
		TopoDS_Shape objShape;
		ST->GetShape(theLabel, objShape);

		for (TopExp_Explorer exp(objShape, TopAbs_SOLID); exp.More(); exp.Next()) {
			ShapeInfo info;
			info.shape = exp.Current();
			info.name = GetShapeName(theLabel, shapeIndex);

			// 提取面和边
			GetFacesAndEdges(info.shape, info.faces, info.edges);

			// 计算包围盒
			BRepBndLib::Add(info.shape, info.boundingBox);

			shapeInfos.push_back(info);
			shapeIndex++;
		}
	}

	return shapeInfos;
}

void ShapeAnalyzer::GetFacePairs(const DetectionResult& result,
	std::vector<std::pair<TopoDS_Face, TopoDS_Face>>& facePairs) {

	facePairs.clear();

	// 确保两个面数组大小相同
	if (result.attachedFaces1.size() != result.attachedFaces2.size()) {
		std::cout << "警告: 面对数量不匹配！" << std::endl;
		return;
	}

	// 成对组合面
	for (size_t i = 0; i < result.attachedFaces1.size(); ++i) {
		facePairs.push_back(std::make_pair(result.attachedFaces1[i], result.attachedFaces2[i]));
	}
}

std::vector<DetectionResult> ShapeAnalyzer::DetectShapeRelations(
	double maxDistance,
	double overlapAreaThreshold,
	double parallelTolerance) {

	std::vector<DetectionResult> results;
	
	// 1. 获取所有形状信息
	std::vector<ShapeInfo> shapeInfos = GetAllShapeInfo();
	if (shapeInfos.size() < 2) {
		return results;
	}
	
	const int shapeCount = static_cast<int>(shapeInfos.size());

	// 2. 预提取形状拓扑信息（类似Process方法）
	struct ShapeTopology {
		std::vector<TopoDS_Face> faces;
		std::vector<TopoDS_Edge> edges;
		std::string name;
		TopoDS_Shape shape;
	};
	
	std::vector<TopoDS_Shape> shapes;
	std::vector<ShapeTopology> topology(shapeCount);
	shapes.reserve(shapeCount);

	for (int i = 0; i < shapeCount; ++i) {
		const auto& info = shapeInfos[i];
		shapes.push_back(info.shape);
		
		topology[i].name = info.name;
		topology[i].shape = info.shape;
		topology[i].faces.reserve(12);
		topology[i].edges.reserve(48);
		GetFacesAndEdges(info.shape, topology[i].faces, topology[i].edges);
	}

	// 3. 构建BVH包围盒索引
	auto bvh = BuildAABBIndex(shapes);
	int numThreads = omp_get_max_threads();
	std::vector<std::vector<DetectionResult>> all_results(numThreads);

	
	
#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		auto& local = all_results[tid];
		local.reserve(64);

#pragma omp for schedule(dynamic, 16)
		for (int i = 0; i < shapeCount - 1; ++i) {
			const auto& target = bvh[i];
			const auto& topo1 = topology[i];

			// 查询可能相交和不相交但接近的候选形状
			auto candidates = QueryPotentialOverlaps(target, bvh);
			auto uncandidates = QueryPotentialUnoverlaps(target, bvh);

			// 4. 处理重叠候选形状（先精确判断距离，再决定检测类型）
			for (int j : candidates) {
				const auto& topo2 = topology[j];

				DetectionResult result;
				result.shape1Name = topo1.name;
				result.shape2Name = topo2.name;
				result.shape1 = topo1.shape;
				result.shape2 = topo2.shape;

				// 精确距离判断，决定检测类型
				result.distance = computeMinimumDistance(topo1.shape, topo2.shape);
				
				if (result.distance > maxDistance) {
					continue; // 距离太大，跳过
				}
				
				if (result.distance <= 1e-6) {
					// 距离很小，进行面贴合检测
					bool foundAttachedFaces = false;
					double totalOverlapArea = 0.0;

					for (const TopoDS_Face& face1 : topo1.faces) {
						for (const TopoDS_Face& face2 : topo2.faces) {
							// 快速包围盒检查
							if (!BBoxIntersect(face1, face2)) {
								continue;
							}

							double overlapArea = computeOverlappingArea(face1, face2);

							if (overlapArea > overlapAreaThreshold) {
								if (!foundAttachedFaces) {
									result.relationType = DetectionResult::FACE_ATTACHED;
									foundAttachedFaces = true;
								}

								// 成对保存贴合的面
								result.attachedFaces1.push_back(face1);
								result.attachedFaces2.push_back(face2);
								totalOverlapArea += overlapArea;
							}
						}
					}

					if (foundAttachedFaces) {
						result.relationDescription = "面贴合 (总重合面积: " + std::to_string(totalOverlapArea) +
							", 贴合面对数: " + std::to_string(result.attachedFaces1.size()) + 
							", 距离: " + std::to_string(result.distance) + ")";
						local.push_back(result);
					}
				}
				else {
					// 距离较大但在检测范围内，改为平行面检测逻辑
					bool foundParallelFaces = false;
					int parallelCount = 0;

					for (const TopoDS_Face& f1 : topo1.faces) {
						for (const TopoDS_Face& f2 : topo2.faces) {
							if (AreFacesParallelAndClose(f1, f2, maxDistance, parallelTolerance)) {
								if (!foundParallelFaces) {
									result.relationType = DetectionResult::PARALLEL_CLOSE;
									foundParallelFaces = true;
								}

								result.attachedFaces1.push_back(f1);
								result.attachedFaces2.push_back(f2);
								parallelCount++;
							}
						}
					}

					if (foundParallelFaces) {
						result.relationDescription = "平行接近面(包围盒重叠但实际分离) (距离: " + std::to_string(result.distance) +
							", 平行面对数: " + std::to_string(parallelCount) + ")";
						local.push_back(result);
					}
				}
			}

			// 5. 处理不重叠但距离较近的形状（检测平行面）
			for (int j : uncandidates) {
				const auto& topo2 = topology[j];

				// 快速包围盒距离检查
				double bboxDist = BBoxMinDistance(target.box, bvh[j].box);
				if (bboxDist > maxDistance) {
					continue;
				}

				DetectionResult result;
				result.shape1Name = topo1.name;
				result.shape2Name = topo2.name;
				result.shape1 = topo1.shape;
				result.shape2 = topo2.shape;

				// 计算精确距离
				result.distance = computeMinimumDistance(topo1.shape, topo2.shape);

				if (result.distance <= maxDistance) {
					// 检测平行且接近的面
					bool foundParallelFaces = false;
					int parallelCount = 0;

					for (const TopoDS_Face& f1 : topo1.faces) {
						for (const TopoDS_Face& f2 : topo2.faces) {
							if (AreFacesParallelAndClose(f1, f2, maxDistance, parallelTolerance)) {
								if (!foundParallelFaces) {
									result.relationType = DetectionResult::PARALLEL_CLOSE;
									foundParallelFaces = true;
								}

								result.attachedFaces1.push_back(f1);
								result.attachedFaces2.push_back(f2);
								parallelCount++;
							}
						}
					}

					if (foundParallelFaces) {
						result.relationDescription = "平行接近面 (距离: " + std::to_string(result.distance) +
							", 平行面对数: " + std::to_string(parallelCount) + ")";
						local.push_back(result);
					}
				}
			}
		}
	}

	// 6. 串行合并所有线程的结果
	for (auto& vec : all_results) {
		results.insert(results.end(), vec.begin(), vec.end());
	}
	return results;
}

void ShapeAnalyzer::PrintDetectionResults(const std::vector<DetectionResult>& results) {
	std::cout << "\n=== 形状关系检测结果 ===" << std::endl;
	std::cout << "共检测到 " << results.size() << " 个关系" << std::endl;

	for (size_t i = 0; i < results.size(); ++i) {
		const auto& result = results[i];
		std::cout << "\n关系 " << (i + 1) << ":" << std::endl;
		std::cout << "  形状1: " << result.shape1Name << std::endl;
		std::cout << "  形状2: " << result.shape2Name << std::endl;
		std::cout << "  关系类型: " << result.relationDescription << std::endl;
		std::cout << "  距离: " << result.distance << std::endl;

		if (result.relationType == DetectionResult::FACE_ATTACHED) {
			std::cout << "  贴合面对详情:" << std::endl;
			for (size_t j = 0; j < result.attachedFaces1.size(); ++j) {
				std::cout << "    面对 " << (j + 1) << ": "
					<< result.shape1Name << " 的面 <-> "
					<< result.shape2Name << " 的面" << std::endl;
			}
		}
		else if (result.relationType == DetectionResult::PARALLEL_CLOSE) {
			std::cout << "  平行接近面对详情:" << std::endl;
			for (size_t j = 0; j < result.attachedFaces1.size(); ++j) {
				std::cout << "    面对 " << (j + 1) << ": "
					<< result.shape1Name << " 的面 <-> "
					<< result.shape2Name << " 的面" << std::endl;
			}
		}

		std::cout << "  形状1相关面数量: " << result.attachedFaces1.size() << std::endl;
		std::cout << "  形状2相关面数量: " << result.attachedFaces2.size() << std::endl;
	}
}


void ShapeAnalyzer::GetCheckFace(Handle(TDocStd_Document)& document, vector<TopoDS_Face>& results, double maxDistance)
{
	SetDocument(document);


	std::vector<TopoDS_Shape> shapes = GetShape();
	if (shapes.empty())
	{
		std::cout << "No shape found." << std::endl;
		return;
	}
	const int shapeCount = static_cast<int>(shapes.size());

	std::cout << "shapeCount:" << shapeCount << std::endl;

	// 1. 预提取每个 shape 的面和边
	struct ShapeTopo { std::vector<TopoDS_Face> faces; std::vector<TopoDS_Edge> edges; };

	std::vector<ShapeTopo> topo(shapeCount);
	for (size_t i = 0; i < shapeCount; ++i) {
		topo[i].faces.reserve(12);
		topo[i].edges.reserve(48);
		GetFacesAndEdges(shapes[i], topo[i].faces, topo[i].edges);
	}

	// 2. 结果容器
	results.reserve(40);

	auto bvh = BuildAABBIndex(shapes);
	int numThreads = omp_get_max_threads();
	std::vector<std::vector<TopoDS_Face>> all_results(numThreads);

	using FaceID = const void*;
#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		auto& local = all_results[tid];
		local.reserve(1024);

#pragma omp for schedule(dynamic, 64)
		for (int ii = 0; ii < shapeCount - 1; ++ii) {
			const auto& target = bvh[ii];
			auto candidates = QueryPotentialOverlaps(target, bvh);
			auto uncandidates = QueryPotentialUnoverlaps(target, bvh);
			std::unordered_set<FaceID> seenFaces;
			seenFaces.reserve(128);

			for (int j : candidates) {
				TopoDS_Face foundF1;
				TopoDS_Face foundF2;
				TopoDS_Edge foundE1, foundE2;
				std::vector<TopoDS_Face> faces;
				double area = detectFaceOverlapAndArea(shapes[ii], shapes[j], foundF1, foundF2, faces);
				if (area > 1e-7)
				{
					if (!foundF1.IsNull()) seenFaces.insert(foundF1.TShape().get());
					if (!foundF2.IsNull()) seenFaces.insert(foundF2.TShape().get());
				}
				if (area > 1 && area < 700)
				{
					auto insertFacesAndRecord = [&]()
						{
							for (const TopoDS_Face& f : faces) {
								local.push_back(f);
							}
						};
					double distance1 = 0.0, distance2 = 0.0;
					int type1 = analyzeFaceShape(foundF1, distance1);
					int type2 = analyzeFaceShape(foundF2, distance2);
					if ((type1 == rectangle && distance1 > 400) || (type2 == rectangle && distance2 > 400))
					{
						insertFacesAndRecord();
					}
					else if ((type1 == ring && distance1 <= 2) && (type2 != ring))
					{
						insertFacesAndRecord();
					}
					else if ((type2 == ring && distance2 <= 2) && (type1 != ring))
					{
						insertFacesAndRecord();
					}
				}
				auto insertFacesAndRecord = [&]()
					{
						for (const TopoDS_Face& f : faces) {
							local.push_back(f);
						}
					};
				int c1 = CountFaceEdgeIntersections(
					topo[ii].faces, topo[j].edges, 2, foundF1, foundE1);
				int c2 = CountFaceEdgeIntersections(
					topo[j].faces, topo[ii].edges, 2, foundF2, foundE2);
				if (c2 == 1 && c1 == 1) {
					faces = FindFacesContainingEdge(topo[j].faces, foundE1);
					insertFacesAndRecord();
					faces = FindFacesContainingEdge(topo[ii].faces, foundE2);
					insertFacesAndRecord();
				}
				else if (c1 == 1 && c2 == 0)
				{
					local.push_back(foundF1);
					faces = FindFacesContainingEdge(topo[j].faces, foundE1);
					insertFacesAndRecord();
				}
				else if (c2 == 1 && c1 == 0)
				{
					local.push_back(foundF2);
					faces = FindFacesContainingEdge(topo[ii].faces, foundE2);
					insertFacesAndRecord();
				}
			}

			for (int j : uncandidates) {
				if (j <= ii) continue;

				if (BBoxMinDistance(target.box, bvh[j].box) > 5.0) continue;

				for (const TopoDS_Face& f1 : topo[ii].faces) {
					for (const TopoDS_Face& f2 : topo[j].faces) {
						if (AreFacesParallelAndClose(f1, f2)) {
							FaceID id1 = f1.TShape().get();
							FaceID id2 = f2.TShape().get();
							if (seenFaces.count(id1) == 0 && seenFaces.count(id2) == 0)
							{
								local.push_back(f1);
								local.push_back(f2);
								goto nextPair;
							}
						}
					}
				}
			nextPair:;
			}
		}
	}
	// 最后串行合并
	for (auto& vec : all_results) {
		results.insert(results.end(), vec.begin(), vec.end());
	}

	std::cout << "找到 " << results.size() << " 组匹配。\n";
}

void ShapeAnalyzer::Process()
{
#include <Standard_Version.hxx>
	std::cout << "OCC version: " << OCC_VERSION_MAJOR << "." << OCC_VERSION_MINOR << std::endl;

	// 读 STEP、generateShape 等准备工作
	std::vector<TopoDS_Shape> shapes = GetShape();
	if (shapes.empty())
	{
		std::cout << "No shape found." << std::endl;
		return;
	}
	const int shapeCount = static_cast<int>(shapes.size());

	std::cout << "shapeCount:" << shapeCount << std::endl;

	// 1. 预提取每个 shape 的面和边
	struct ShapeTopo { std::vector<TopoDS_Face> faces; std::vector<TopoDS_Edge> edges; };

	std::vector<ShapeTopo> topo(shapeCount);
	for (size_t i = 0; i < shapeCount; ++i) {
		topo[i].faces.reserve(12);
		topo[i].edges.reserve(48);
		GetFacesAndEdges(shapes[i], topo[i].faces, topo[i].edges);
	}

	// 2. 结果容器
	std::vector<TopoDS_Face> results;
	results.reserve(40);

	auto bvh = BuildAABBIndex(shapes);
	int numThreads = omp_get_max_threads();
	std::vector<std::vector<TopoDS_Face>> all_results(numThreads);

	using FaceID = const void*;
#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		auto& local = all_results[tid];
		local.reserve(1024);

#pragma omp for schedule(dynamic, 64)
		for (int ii = 0; ii < shapeCount - 1; ++ii) {
			const auto& target = bvh[ii];
			auto candidates = QueryPotentialOverlaps(target, bvh);
			auto uncandidates = QueryPotentialUnoverlaps(target, bvh);
			std::unordered_set<FaceID> seenFaces;
			seenFaces.reserve(128);

			for (int j : candidates) {
				TopoDS_Face foundF1;
				TopoDS_Face foundF2;
				TopoDS_Edge foundE1, foundE2;
				std::vector<TopoDS_Face> faces;
				double area = detectFaceOverlapAndArea(shapes[ii], shapes[j], foundF1, foundF2, faces);
				if (area > 1e-7)
				{
					if (!foundF1.IsNull()) seenFaces.insert(foundF1.TShape().get());
					if (!foundF2.IsNull()) seenFaces.insert(foundF2.TShape().get());
				}
				if (area > 1 && area < 700)
				{
					auto insertFacesAndRecord = [&]()
						{
							for (const TopoDS_Face& f : faces) {
								local.push_back(f);
							}
						};
					double distance1 = 0.0, distance2 = 0.0;
					int type1 = analyzeFaceShape(foundF1, distance1);
					int type2 = analyzeFaceShape(foundF2, distance2);
					if ((type1 == rectangle && distance1 > 400) || (type2 == rectangle && distance2 > 400))
					{
						insertFacesAndRecord();
					}
					else if ((type1 == ring && distance1 <= 2) && (type2 != ring))
					{
						insertFacesAndRecord();
					}
					else if ((type2 == ring && distance2 <= 2) && (type1 != ring))
					{
						insertFacesAndRecord();
					}
				}
				auto insertFacesAndRecord = [&]()
					{
						for (const TopoDS_Face& f : faces) {
							local.push_back(f);
						}
					};
				int c1 = CountFaceEdgeIntersections(
					topo[ii].faces, topo[j].edges, 2, foundF1, foundE1);
				int c2 = CountFaceEdgeIntersections(
					topo[j].faces, topo[ii].edges, 2, foundF2, foundE2);
				if (c2 == 1 && c1 == 1) {
					faces = FindFacesContainingEdge(topo[j].faces, foundE1);
					insertFacesAndRecord();
					faces = FindFacesContainingEdge(topo[ii].faces, foundE2);
					insertFacesAndRecord();
				}
				else if (c1 == 1 && c2 == 0)
				{
					local.push_back(foundF1);
					faces = FindFacesContainingEdge(topo[j].faces, foundE1);
					insertFacesAndRecord();
				}
				else if (c2 == 1 && c1 == 0)
				{
					local.push_back(foundF2);
					faces = FindFacesContainingEdge(topo[ii].faces, foundE2);
					insertFacesAndRecord();
				}
			}

			for (int j : uncandidates) {
				if (j <= ii) continue;

				if (BBoxMinDistance(target.box, bvh[j].box) > 5.0) continue;

				for (const TopoDS_Face& f1 : topo[ii].faces) {
					for (const TopoDS_Face& f2 : topo[j].faces) {
						if (AreFacesParallelAndClose(f1, f2)) {
							FaceID id1 = f1.TShape().get();
							FaceID id2 = f2.TShape().get();
							if (seenFaces.count(id1) == 0 && seenFaces.count(id2) == 0)
							{
								local.push_back(f1);
								local.push_back(f2);
								goto nextPair;
							}
						}
					}
				}
			nextPair:;
			}
		}
	}

	// 最后串行合并
	for (auto& vec : all_results) {
		results.insert(results.end(), vec.begin(), vec.end());
	}

	// 4. 把结果加入 Compound 并导出
	TopoDS_Compound aCompound;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(aCompound);
	for (auto& r : results) {
		if (!r.IsNull())
		{
			aBuilder.Add(aCompound, r);
		}
	}
	std::cout << "找到 " << results.size() << " 组匹配。\n";
}

// 设置和获取文档对象的函数实现
void ShapeAnalyzer::SetDocument(const Handle(TDocStd_Document)& docoment) {
	doc = docoment;
}

Handle(TDocStd_Document) ShapeAnalyzer::GetDocument() const {
	return doc;
}


void ShapeAnalyzer::getShapeRelation(const Handle(TDocStd_Document)& doc, std::vector<DetectionResult>& detectionResults,double maxDistance, double overlapTolerance, double parallelTolerance)
{
	SetDocument(doc);
	detectionResults = DetectShapeRelations(maxDistance, overlapTolerance, parallelTolerance);
}

