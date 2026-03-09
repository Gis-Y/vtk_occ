#ifndef MODELIO_H
#define MODELIO_H

#include "BaseIO.h"

#include <BinXCAFDrivers.hxx>
#include <XCAFApp_Application.hxx>

#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_LabelMap.hxx>
#include <TDF_LabelSequence.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <Interface_Static.hxx>
#include <Interface_EntityIterator.hxx>
#include <Interface_Check.hxx>
#include <Standard_CString.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>
#include <Standard_OStream.hxx>

#include <XCAFDoc_Color.hxx>

#include <STEPControl_StepModelType.hxx>
#include <UnitsMethods.hxx>
#include <Prs3d_Drawer.hxx>

#include <IGESCAFControl_Reader.hxx>
#include <IGESCAFControl_Writer.hxx>

#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>

#include <RWGltf_CafReader.hxx>
#include <RWGltf_CafWriter.hxx>

#include <StlAPI_Writer.hxx>
#include <StlAPI_Reader.hxx>

enum GeomType
{
	GeomSTP,
	GeomIGS,
	GeomSTL,
	GeomBREP,
	GeomGLTF
};

class ModelIO : public BaseIO
{
public:
	ModelIO() {}
	bool Read(const string &fileName, const GeomType &modelType);
	bool Write(const string &fileName, const GeomType &modelType);

private:
	bool ReadIGS(const string &fileName);
	bool ReadSTP(const string &fileName);
	bool ReadBREP(const string &filename);
	bool ReadSTL(const string &filename);
	bool ReadGLTF(const string &filename);

	bool WriteSTP(const string &fileName);
	bool WriteIGS(const string &fileName);
	bool WriteBREP(const string &fileName);
	bool WriteSTL(const string &fileName);
	bool WriteGLTF(const string &fileName);

	GeomType modelFormat;
};

inline bool ModelIO::Read(const string &fileName, const GeomType &modelType)
{
	modelFormat = modelType;

	switch (modelType)
	{
	case GeomSTP:
		return ReadSTP(fileName);
	case GeomIGS:
		return ReadIGS(fileName);
	case GeomSTL:
		return ReadSTL(fileName);
	case GeomBREP:
		return ReadBREP(fileName);
	case GeomGLTF:
		return ReadGLTF(fileName);
	default:
		break;
	}

	return false;
}

inline bool ModelIO::Write(const string &fileName, const GeomType &modelType)
{
	switch (modelType)
	{
	case GeomSTP:
		return WriteSTP(fileName);
	case GeomIGS:
		return WriteIGS(fileName);
	case GeomSTL:
		return WriteSTL(fileName);
	case GeomBREP:
		return WriteBREP(fileName);
	case GeomGLTF:
		return WriteGLTF(fileName);
	default:
		break;
	}

	return false;
}

inline bool ModelIO::ReadIGS(const string &fileName)
{
	IGESCAFControl_Reader aIgesReader;
	aIgesReader.SetColorMode(true);
	aIgesReader.SetNameMode(true);

	IFSelect_ReturnStatus status = aIgesReader.ReadFile(fileName.c_str());

	Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	BinXCAFDrivers::DefineFormat(anApp);
	anApp->NewDocument("BinXCAF", doc);

	if (status == IFSelect_RetDone)
	{
		aIgesReader.Transfer(doc);
		return true;
	}

	return false;
}

inline bool ModelIO::ReadSTP(const string &fileName)
{

	Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	BinXCAFDrivers::DefineFormat(anApp);
	anApp->NewDocument("BinXCAF", doc);

	STEPCAFControl_Reader aStepReader;
	aStepReader.SetColorMode(true);
	aStepReader.SetNameMode(true);

	IFSelect_ReturnStatus status = aStepReader.ReadFile(fileName.c_str());


	//--------------
	//TopoDS_Shape shape_Step;

	//STEPControl_Reader aReader_Step = aStepReader.Reader();
	//	int j = 1;

	//for (Standard_Integer i = 1; i <= aReader_Step.NbRootsForTransfer(); i++)
	//	aReader_Step.TransferRoot(i);
	//for (Standard_Integer i = 1; i <= aReader_Step.NbShapes(); i++)
	//	shape_Step = aReader_Step.Shape(i);

	//TopTools_IndexedMapOfShape solids_map, shells_map, faces_map, wires_map, edges_map, vertices_map;
	//TopExp_Explorer exp_solids, exp_shells, exp_faces, exp_wires, exp_edges, exp_vertices;
	//for (exp_faces.Init(shape_Step, TopAbs_FACE); exp_faces.More(); exp_faces.Next())
	//{
	//	TopoDS_Face face = TopoDS::Face(exp_faces.Current().Composed(shape_Step.Orientation()));

	//	cout << face.HashCode()

	//	j++;
	//}

	//--------------

	if (status == IFSelect_RetDone)
	{
		aStepReader.Transfer(doc);
		return true;
	}

	return false;
}

inline bool ModelIO::ReadBREP(const string &filename)
{
	TopoDS_Shape aShape;
	BRep_Builder aBuilder;

	if (!BRepTools::Read(aShape, filename.c_str(), aBuilder))
	{
		return false;
	}	

	doc = new TDocStd_Document("BRep");

	Handle(XCAFDoc_ShapeTool) ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	ST->AddShape(aShape);

	return true;
}

inline bool ModelIO::ReadSTL(const string &filename)
{
	TopoDS_Shape aShape;

	StlAPI_Reader anStlReader;

	if (!anStlReader.Read(aShape, filename.c_str()))
	{
		return false;
	}

	doc = new TDocStd_Document("STL");

	Handle(XCAFDoc_ShapeTool) ST = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
	Handle(XCAFDoc_ColorTool) CT = XCAFDoc_DocumentTool::ColorTool(doc->Main());
	Quantity_ColorRGBA aColor(0.644479692f, 0.644479692f, 1.00000000f, 1.00000000f);

	TDF_Label aLabel = ST->AddShape(aShape);
	CT->SetColor(aShape, aColor, XCAFDoc_ColorSurf);

	TDataStd_Name::Set(aLabel, "STL_Shape");

	return true;
}

inline bool ModelIO::ReadGLTF(const string &filename)
{
	Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	BinXCAFDrivers::DefineFormat(anApp);
	anApp->NewDocument("BinXCAF", doc);

	RWGltf_CafReader aReader;
	Standard_Real aSystemUnitFactor = UnitsMethods::GetCasCadeLengthUnit() * 0.001;
	aReader.SetSystemLengthUnit(aSystemUnitFactor);
	aReader.SetSystemCoordinateSystem(RWMesh_CoordinateSystem_Zup);
	aReader.SetDocument(doc);
	aReader.SetParallel(Standard_True);
	Message_ProgressRange theProgress;

	if (aReader.Perform(filename.c_str(), theProgress))
	{
		Handle(XCAFDoc_ColorTool) CT = XCAFDoc_DocumentTool::ColorTool(doc->Main());
		Quantity_ColorRGBA aColor(0.644479692, 0.644479692, 1.00000000, 1.00000000);
		CT->SetColor(doc->Main(), aColor, XCAFDoc_ColorSurf);
		return true;
	}
	else
	{
		return false;
	}
}
// ------------------------------------------
// Write Files
// ------------------------------------------
inline bool ModelIO::WriteSTP(const string &fileName)
{
	if (modelFormat == GeomGLTF)
	{
		return false;
	}

	STEPControl_StepModelType mode = STEPControl_AsIs;

	STEPCAFControl_Writer aWriter;
	aWriter.SetColorMode(true);
	aWriter.SetNameMode(true);

	// Translating document (conversion) to STEP
	if (!aWriter.Transfer(doc, mode)) {
		return false;
	}
	// Writing the File
	IFSelect_ReturnStatus status = aWriter.Write(fileName.c_str());

	// 检查是否成功写入
	if (status != IFSelect_RetDone) {
		return false;		
	}

	return true;
}
inline bool ModelIO::WriteIGS(const string &fileName)
{
	if (modelFormat == GeomGLTF)
	{
		return false;
	}

	IGESCAFControl_Writer aWriter;
	aWriter.SetColorMode(true);
	aWriter.SetNameMode(true);

	return aWriter.Perform(doc, fileName.c_str());
}

inline bool ModelIO::WriteBREP(const string &fileName)
{
	Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	TDF_LabelSequence aRootLabels;
	aShapeTool->GetFreeShapes(aRootLabels);

	TopoDS_Compound aCompound;
	BRep_Builder    aBuildTool;
	aBuildTool.MakeCompound(aCompound);
	for (TDF_LabelSequence::Iterator aRootIter(aRootLabels); aRootIter.More(); aRootIter.Next())
	{
		const TDF_Label& aRootLabel = aRootIter.Value();
		TopoDS_Shape aRootShape;
		if (XCAFDoc_ShapeTool::GetShape(aRootLabel, aRootShape))
		{
			aBuildTool.Add(aCompound, aRootShape);
		}
	}

	return BRepTools::Write(aCompound, fileName.c_str());
}


inline bool ModelIO::WriteSTL(const string &fileName)
{
	cout << "WriteSTL " << endl;
	Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	TDF_LabelSequence aRootLabels;
	aShapeTool->GetFreeShapes(aRootLabels);

	TopoDS_Compound aCompound;
	BRep_Builder    aBuildTool;
	aBuildTool.MakeCompound(aCompound);
	for (TDF_LabelSequence::Iterator aRootIter(aRootLabels); aRootIter.More(); aRootIter.Next())
	{
		const TDF_Label& aRootLabel = aRootIter.Value();
		TopoDS_Shape aRootShape;
		if (XCAFDoc_ShapeTool::GetShape(aRootLabel, aRootShape))
		{
			aBuildTool.Add(aCompound, aRootShape);
		}
	}

	// perform meshing
	Handle(Prs3d_Drawer) aDrawer = new Prs3d_Drawer(); // holds visualization defaults
	BRepMesh_IncrementalMesh anAlgo;
	anAlgo.ChangeParameters().Deflection = 0.2;
	anAlgo.ChangeParameters().Angle = 20.0 * M_PI / 180.0; // 20 degrees
	anAlgo.ChangeParameters().InParallel = true;
	anAlgo.SetShape(aCompound);
	anAlgo.Perform();

	

	StlAPI_Writer anStlWriter;
	//anStlWriter.ASCIIMode() = false;

	cout << "Start Writing STL." << endl;

	if (anStlWriter.Write(aCompound, fileName.c_str()))
	{
		return true;
	}
	return false;
}

inline bool ModelIO::WriteGLTF(const string &fileName)
{
	Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

	TDF_LabelSequence aRootLabels;
	aShapeTool->GetFreeShapes(aRootLabels);

	TopoDS_Compound aCompound;
	BRep_Builder    aBuildTool;
	aBuildTool.MakeCompound(aCompound);
	for (TDF_LabelSequence::Iterator aRootIter(aRootLabels); aRootIter.More(); aRootIter.Next())
	{
		const TDF_Label& aRootLabel = aRootIter.Value();
		TopoDS_Shape aRootShape;
		if (XCAFDoc_ShapeTool::GetShape(aRootLabel, aRootShape))
		{
			aBuildTool.Add(aCompound, aRootShape);
		}
	}

	// perform meshing
	Handle(Prs3d_Drawer) aDrawer = new Prs3d_Drawer(); // holds visualization defaults
	BRepMesh_IncrementalMesh anAlgo;
	anAlgo.ChangeParameters().Deflection = 0.2;
	anAlgo.ChangeParameters().Angle = 20.0 * M_PI / 180.0; // 20 degrees
	anAlgo.ChangeParameters().InParallel = true;
	anAlgo.SetShape(aCompound);
	anAlgo.Perform();

	TColStd_IndexedDataMapOfStringString aMetadata;
	Message_ProgressRange theProgress;
	RWGltf_CafWriter aGltfWriter(fileName.c_str(), true);
	// STEP reader translates into mm units by default
	aGltfWriter.ChangeCoordinateSystemConverter().SetInputLengthUnit(0.001);
	aGltfWriter.ChangeCoordinateSystemConverter().SetInputCoordinateSystem(RWMesh_CoordinateSystem_Zup);
	if (aGltfWriter.Perform(doc, aMetadata, theProgress))
	{
		return true;
	}
	return false;
}
#endif
