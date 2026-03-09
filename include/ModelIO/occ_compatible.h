/*******************************************************************************

程序说明

解决OCC7.5与7.8兼容性

*******************************************************************************/

#ifndef OCC_COMPATIBLE_H
#define OCC_COMPATIBLE_H

#include <vector>
#include <Standard_Version.hxx>

using namespace std;

namespace occ_compatible
{
	//
//#define OCCT_VERSION_LESS_THAN_7_8 \
//    ((OCC_VERSION_MAJOR < 7) || \
//    ((OCC_VERSION_MAJOR == 7) && (OCC_VERSION_MINOR < 8)))

#define OCC_VERSION_OPTION_7_5 ((OCC_VERSION_MAJOR == 7) && (OCC_VERSION_MINOR == 5))
#define OCC_VERSION_OPTION_7_8 ((OCC_VERSION_MAJOR == 7) && (OCC_VERSION_MINOR == 8))

#if OCC_VERSION_OPTION_7_5
vector<double> GetNormals(opencascade::handle<Poly_Triangulation> theTris)
{
	vector<double> res;
	for (int i = 1; i <= theTris->Normals().Size(); i++)
	{
		res.push_back(theTris->Normals().Value(i));
	}
	return res;
}


#define POLY_TRIGULATION_TRIANGLES(triFace) triFace->Triangles()
#define POLY_TRIGULATION_NODES(triFace) triFace->Nodes()
#define POLY_TRIGULATION_NORMALS(triFace) occ_compatible::GetNormals(triFace)
#define POLY_TRIGULATION_UVNODES(triFace) triFace->UVNodes()


#elif OCC_VERSION_OPTION_7_8
#include <gp_Vec3f.hxx>

	TColgp_Array1OfPnt GetNodes(opencascade::handle<Poly_Triangulation> theTris)
	{
		const Poly_ArrayOfNodes &aNodes = theTris->InternalNodes();
		TColgp_Array1OfPnt aPnt(1, aNodes.Size());
		for (int i = 1; i <= aNodes.Size(); i++)
		{
			gp_Pnt pt = aNodes.Value(i - 1);
			aPnt[i] = pt;
		}

		return aPnt;
	}

	vector<double> GetNormals(opencascade::handle<Poly_Triangulation> theTris)
	{
		vector<double> res;
		NCollection_Array1<gp_Vec3f> aNormals = theTris->InternalNormals();
		for (int i = 1; i <= aNormals.Size(); i++)
		{
			res.push_back(aNormals.Value(i - 1).x());
			res.push_back(aNormals.Value(i - 1).y());
			res.push_back(aNormals.Value(i - 1).z());
		}
		return res;
	}

	TColgp_Array1OfPnt2d GetUVNodes(opencascade::handle<Poly_Triangulation> theTris)
	{
		const Poly_ArrayOfUVNodes &aNodes = theTris->InternalUVNodes();
		cout << "aNodes Size: " << aNodes.Size() << endl;
		TColgp_Array1OfPnt2d aPnt(1, aNodes.Size());
		for (int i = 1; i <= aNodes.Size(); i++)
		{
			aPnt[i] = aNodes.Value(i - 1);
		}

		return aPnt;
	}
	

#define POLY_TRIGULATION_TRIANGLES(triFace) triFace->InternalTriangles()
#define POLY_TRIGULATION_NODES(triFace) occ_compatible::GetNodes(triFace)
#define POLY_TRIGULATION_NORMALS(triFace) occ_compatible::GetNormals(triFace)
#define POLY_TRIGULATION_UVNODES(triFace) occ_compatible::GetUVNodes(triFace)
#endif
};


#endif