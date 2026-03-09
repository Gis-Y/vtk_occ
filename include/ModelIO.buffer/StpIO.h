#ifndef STPIO_H
#define STPIO_H

#include "BaseIO.h"

#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <BinXCAFDrivers.hxx>
#include <XCAFApp_Application.hxx>

class StpIO : public BaseIO
{
public:
	StpIO(){}
	virtual void Read(string fileName);
};

inline void StpIO::Read(string fileName)
{

	Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	BinXCAFDrivers::DefineFormat(anApp);
	anApp->NewDocument("BinXCAF", doc);

	STEPCAFControl_Reader aStepReader;
	aStepReader.SetColorMode(true);
	aStepReader.SetNameMode(true);

	aStepReader.ReadFile(fileName.c_str());

	aStepReader.Transfer(doc);

#ifdef OCCTEST

	PCDM_StoreStatus sstate = anApp->SaveAs(doc, "F:/test.xbf");
	if (sstate != PCDM_SS_OK)
	{
		cout << "Save Failed" << endl;
	}
#endif	

}

#endif
