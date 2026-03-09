
/*******************************************************************************

����˵��

Poly_Connect_ex.h��
	����ļ��Ļ����ļ���Opencascade��Poly_Connect.hxx
	���ڵ���Poly_Connect����㷨��ʱ������ֳ���ִ����ϣ�polyedge�ͷŴ����Bug��
	�����ԭ���Ļ��������¶��ļ������˱༭��ʵ��Poly_Connect.hxx�ļ��а����ͷ�polyedge����
	���ǣ�ע�͵��ˣ���˽�������´򿪣�

���㷨��ע�͵��ComputeNormals()������ͨ���������ڵ�Ԫ��ƽ�����߷�����ķ��ߣ���׼ȷ
�����ComputeNormals()������ͨ��OCC�ڲ����������ڼ��Σ����������ϵ�ķ��߷���

*******************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <TopoDS_Face.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopLoc_Location.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangle.hxx>
#include <Quantity_Color.hxx>
#include <TDF_Label.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_Tool.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <Geom_Surface.hxx>

#include "Poly_Connect_ex.h"

using namespace std;

enum EdgeType
{
	FreeEdge,		// Single Edge in Ansa
	InnerEdge,		// Double Edge in Ansa
	ShareEdge		// Triple Edge in Ansa
};

struct EdgeProperty
{
	EdgeType edgeType;
	int id;
	vector<int> edges;
};

inline bool operator<(const EdgeProperty& p1, const EdgeProperty& p2)
{
	if (p1.edges < p2.edges)
	{
		return true;
	}
	else
	{
		return false;
	}
}

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

	int id;

	//int faceId;
	////int alpha;
	//int sid;

	//int hashCode;

	TopoDS_Face faceObj;

	//unsigned long long faceAddress;

	string label;
	string name;

	// �������¹����ڵ㵥Ԫ���ߵ����ݽṹ��������ʹ��OCC�е����е����ݣ�ԭ���ǹ��������ݿ���ֱ�����뵽OpenGL��
	// ����DCiP�н�����Ⱦʱ��һ����Ҫ������ͬ��vector�����ܰѽ�����ݵ���Ⱦ����
	vector<double> points;
	vector<double> normals;
	vector<int> elements;
	set<EdgeProperty> edgeProperties;
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

			// -1��Ϊ�˴�1��ʼ��Ϊ0��ʼ
			elements.push_back(nVertexIndex1 - 1);
			elements.push_back(nVertexIndex2 - 1);
			elements.push_back(nVertexIndex3 - 1);
		}
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
	//*
	//OCC��ȡ�ڵ㷨�ߣ�����ȷ
	void ComputeNormals()
	{
		TopLoc_Location location;
		Handle(Poly_Triangulation) theTris = BRep_Tool::Triangulation(faceObj, location);

		if (theTris.IsNull())
		{
			return;
		}
		if (theTris->HasNormals()) {

			//normals = POLY_TRIGULATION_NORMALS(theTris);
			return;
		}

		// take in face the surface location
		const TopoDS_Face aZeroFace =
			TopoDS::Face(faceObj.Located(TopLoc_Location()));
		Handle(Geom_Surface) aSurf = BRep_Tool::Surface(aZeroFace);
		const Poly_Array1OfTriangle &aTriangles = POLY_TRIGULATION_TRIANGLES(theTris);
		if (!theTris->HasUVNodes() || aSurf.IsNull()) {
			// compute normals by averaging triangulation normals sharing
			// the same vertex
			Poly::ComputeNormals(theTris);
			return;
		}

		const Standard_Real aTol = Precision::Confusion();
		//const TColgp_Array1OfPnt2d &aNodesUV = theTris->UVNodes();
		const TColgp_Array1OfPnt2d &aNodesUV = POLY_TRIGULATION_UVNODES(theTris);
		//const TColgp_Array1OfPnt &aNodes = theTris->Nodes();
		const TColgp_Array1OfPnt &aNodes = POLY_TRIGULATION_NODES(theTris);
		gp_Dir aNorm;

		for (Standard_Integer aNodeIter = aNodes.Lower(); aNodeIter <= aNodes.Upper(); ++aNodeIter)
		{
			if (GeomLib::NormEstim(aSurf, aNodesUV.Value(aNodeIter), aTol, aNorm) > 1)
			{
				Poly_Connect_Ex thePolyConnect;
				thePolyConnect.Load(theTris);

				// compute flat normals
				gp_XYZ eqPlan(0.0, 0.0, 0.0);
				Standard_Integer aTri[3];
				for (thePolyConnect.Initialize(aNodeIter); thePolyConnect.More(); thePolyConnect.Next())
				{
					aTriangles(thePolyConnect.Value()).Get(aTri[0], aTri[1], aTri[2]);
					const gp_XYZ v1(aNodes(aTri[1]).Coord() - aNodes(aTri[0]).Coord());
					const gp_XYZ v2(aNodes(aTri[2]).Coord() - aNodes(aTri[1]).Coord());
					const gp_XYZ vv = v1 ^ v2;
					const Standard_Real aMod = vv.Modulus();
					if (aMod >= aTol) {
						eqPlan += vv / aMod;
					}
				}
				const Standard_Real aModMax = eqPlan.Modulus();
				aNorm = (aModMax > aTol) ? gp_Dir(eqPlan) : gp::DZ();
			}

			normals.push_back(aNorm.X());
			normals.push_back(aNorm.Y());
			normals.push_back(aNorm.Z());
		}

	}
	//*/
	/*
	// ƽ�����߷�����ڵ㷨��
	void ComputeNormals()
	{
		normals.resize(points.size());

		for (unsigned int i = 0, il = elements.size(); i < il; i += 3) {

			// elements��1��ʼ��������0������-1
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
	*/
	void Dump()
	{
		cout << "Face Property: " << endl;
		cout << "Color Red  : " << red << endl;
		cout << "Color Green: " << green << endl;
		cout << "Color Blue : " << blue << endl;
		cout << "id   : " << id << endl;
		cout << "points size: " << points.size() << endl;
		cout << "elems  size: " << elements.size() << endl;
		cout << endl;
	}

};