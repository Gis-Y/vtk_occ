/*******************************************************************************

程序说明

提供了一些处理模型的方法，ModelIO.h中并未使用

*******************************************************************************/

#ifndef GEOAPIUTIL_H
#define GEOAPIUTIL_H

#include <XCAFApp_Application.hxx>
#include <BinXCAFDrivers.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepTools.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <TopoDS.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <STANDARD_TYPE.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <StlAPI_Writer.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Lin.hxx>
#include <string>

using namespace std;

namespace GeoAPIUtil
{

	// 获取一个Shape的中心点，是多个Face中心点的平均值
	// Face为柱面时中心点为中轴的中点
	// Face为球面时，中心点为圆心
	inline gp_Pnt GetShapeCentroid(const TopoDS_Shape &shape)
	{
		gp_Pnt centroid;
		int centroidCount = 0;
		for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
		{
			const TopoDS_Face face = TopoDS::Face(faceExp.Current());
			const Handle(Geom_Surface)& surface = BRep_Tool::Surface(face);

			gp_Pnt center;

			// 如果是圆柱面则取圆柱的中心，否则计算其质心
			if (surface->IsInstance(STANDARD_TYPE(Geom_CylindricalSurface)))
			{
				Handle(Geom_CylindricalSurface) cylindricalSurface = Handle(Geom_CylindricalSurface)::DownCast(surface);
				center = cylindricalSurface->Location();
			}
			else if (surface->IsInstance(STANDARD_TYPE(Geom_SphericalSurface)))
			{
				Handle(Geom_SphericalSurface) sphericalSurface = Handle(Geom_SphericalSurface)::DownCast(surface);
				center = sphericalSurface->Location();
			}
			else
			{
				GProp_GProps properties;
				BRepGProp::SurfaceProperties(face, properties);
				center = properties.CentreOfMass();
			}

			centroid.SetX(centroid.X() + center.X());
			centroid.SetX(centroid.Y() + center.Y());
			centroid.SetX(centroid.Z() + center.Z());
			centroidCount++;
		}

		centroid.SetX(centroid.X() / centroidCount);
		centroid.SetX(centroid.Y() / centroidCount);
		centroid.SetX(centroid.Z() / centroidCount);

		return centroid;
	}
	inline void CreateFaceCentroid(const TopoDS_Face &face, gp_Pnt &center)
	{

		Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

		// 如果是圆柱面则取圆柱的中心，否则计算其质心
		if (surface->IsInstance(STANDARD_TYPE(Geom_CylindricalSurface)))
		{
			Handle(Geom_CylindricalSurface) cylindricalSurface = Handle(Geom_CylindricalSurface)::DownCast(surface);
			center = cylindricalSurface->Location();
		}
		else if (surface->IsInstance(STANDARD_TYPE(Geom_SphericalSurface)))
		{
			Handle(Geom_SphericalSurface) sphericalSurface = Handle(Geom_SphericalSurface)::DownCast(surface);
			center = sphericalSurface->Location();
		}
		else
		{
			GProp_GProps properties;
			BRepGProp::SurfaceProperties(face, properties);
			center = properties.CentreOfMass();
		}
	}
	// 获取中心点，并且投影到平面上
	inline void CreateFaceCentroidOnSurface(const TopoDS_Face &face, gp_Pnt &center)
	{
		Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

		gp_Pnt _center;

		GProp_GProps properties;
		BRepGProp::SurfaceProperties(face, properties);
		_center = properties.CentreOfMass();

		GeomAPI_ProjectPointOnSurf geomProj(_center, surface);

		if (geomProj.IsDone())
		{
			double disMin = 1e6;

			for (int i = 1; i <= geomProj.NbPoints(); i++)
			{
				gp_Pnt _pt = geomProj.Point(i);

				double squareDis = _center.SquareDistance(_pt);
				if (squareDis < disMin)
				{
					disMin = squareDis;
					center = _pt;
				}
			}
		}
		else
		{
			center = _center;
		}

	}

	// 用一个球体截取一个曲面，并且生成BRep数据
	// 输出为Brep文件，参数依次为球的圆心，半径，以及要裁剪的面TopoDS_Shape类型，最后是输出的BRep文件的路径
	inline bool SphereSurfaceIntersection(const gp_Pnt &center, const double &R, const TopoDS_Shape &shape, const string &brepfilePath, const string &stlfilePath)
	{
		TopoDS_Shape sphereShape = BRepPrimAPI_MakeSphere(center, R);

		TopoDS_Compound aCompound;
		BRep_Builder    aBuildTool;
		aBuildTool.MakeCompound(aCompound);

		for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
		{
			TopoDS_Shape shapeChild = faceExp.Current();

			BRepAlgoAPI_Common section(shapeChild, sphereShape);
			section.Build();

			TopExp_Explorer faceChildExp(section.Shape(), TopAbs_FACE);
			if (faceChildExp.More())
			{
				aBuildTool.Add(aCompound, section.Shape());
			}
		}

		if (!BRepTools::Write(aCompound, brepfilePath.c_str()))
		{
			return false;
		}

		BRepMesh_IncrementalMesh anAlgo;
		anAlgo.ChangeParameters().Deflection = 0.2;
		anAlgo.ChangeParameters().Angle = 20.0 * M_PI / 180.0; // 20 degrees
		anAlgo.ChangeParameters().InParallel = true;
		anAlgo.SetShape(aCompound);
		anAlgo.Perform();

		StlAPI_Writer anStlWriter;

		if (anStlWriter.Write(aCompound, stlfilePath.c_str()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// 用一个球体截取一个曲面，并且生成TopoDS_Compound
	// 返回为TopoDS_Compound，参数依次为球的圆心，半径，以及要裁剪的面TopoDS_Shape类型
	inline TopoDS_Compound SphereSurfaceIntersection(const gp_Pnt &center, const double &R, const TopoDS_Shape &shape)
	{
		TopoDS_Shape sphereShape = BRepPrimAPI_MakeSphere(center, R);

		TopoDS_Compound aCompound;
		BRep_Builder    aBuildTool;
		aBuildTool.MakeCompound(aCompound);

		for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next())
		{
			TopoDS_Shape shapeChild = faceExp.Current();

			BRepAlgoAPI_Common section(shapeChild, sphereShape);
			section.Build();

			TopExp_Explorer faceChildExp(section.Shape(), TopAbs_FACE);
			if (faceChildExp.More())
			{
				aBuildTool.Add(aCompound, section.Shape());
			}
		}

		return aCompound;
	}

	// 删除Face
	inline void RemoveFace(TopoDS_Shape& shape, const TopoDS_Face& faceToRemove)
	{
		BRepAlgoAPI_Cut cut(shape, faceToRemove);
		TopoDS_Shape resultShape = cut.Shape();
		shape = resultShape;
	}

	inline void CreatePoint(const double &x, const double &y, const double &z, gp_Pnt &aPoint)
	{
		aPoint.SetCoord(x, y, z);

	}
	inline void CreatePoint(const double &x, const double &y, const double &z, TopoDS_Vertex &vertex)
	{
		gp_Pnt aPoint(x, y, z);
		vertex = BRepBuilderAPI_MakeVertex(aPoint);
	}

	inline bool GetLineSurfaceIntersectionPoint(const gp_Pnt &pt, const gp_Dir &dir, const TopoDS_Face &face, gp_Pnt &intersectionPoint)
	{
		gp_Lin line(pt, dir);
		TopoDS_Edge lineEdge = BRepBuilderAPI_MakeEdge(line);
		BRepExtrema_DistShapeShape distSS(lineEdge, face, Extrema_ExtFlag_MIN);
		distSS.Perform();

		if (distSS.IsDone() && distSS.NbSolution() > 0 && distSS.Value() < 1e-3)
		{
			double minDistance = 1e6;
			for (int i = 1; i <= distSS.NbSolution(); i++)
			{

				double dis = distSS.PointOnShape2(i).Distance(pt);

				if (dis < minDistance)
				{
					minDistance = dis;
					intersectionPoint = distSS.PointOnShape2(i);
				}
			}

			return true;
		}
		else {
			return false;
		}
	}

	inline bool CreateStpByShape(const TopoDS_Shape& aShape, const string &filePath)
	{
		Handle(TDocStd_Document) localDoc;
		Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
		BinXCAFDrivers::DefineFormat(anApp);
		anApp->NewDocument("BinXCAF", localDoc);
		Handle(XCAFDoc_ShapeTool)shapeTool = XCAFDoc_DocumentTool::ShapeTool(localDoc->Main());

		// 将形状添加到文档中
		TDF_Label label;
		shapeTool->AddShape(aShape);

		STEPControl_StepModelType mode = STEPControl_AsIs;

		STEPCAFControl_Writer aWriter;
		aWriter.SetColorMode(true);
		aWriter.SetNameMode(true);

		// Translating document (conversion) to STEP
		if (!aWriter.Transfer(localDoc, mode)) {
			return false;
		}
		// Writing the File
		IFSelect_ReturnStatus status = aWriter.Write(filePath.c_str());

		// 检查是否成功写入
		if (status != IFSelect_RetDone) {
			return false;
		}

		return true;
	}


};
#endif // GEOAPIUTIL_H