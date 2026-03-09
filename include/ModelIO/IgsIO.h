#ifndef IGSIO_H
#define IGSIO_H

#include "BaseIO.h"

#include <IGESCAFControl_Reader.hxx>
#include <BinXCAFDrivers.hxx>
#include <XCAFApp_Application.hxx>

class IgsIO : public BaseIO
{
public:
	IgsIO() {}
	virtual void Read(string fileName);
};

inline void IgsIO::Read(string fileName)
{
	cout << "ReadStep fileName: " << fileName << endl;

	Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	BinXCAFDrivers::DefineFormat(anApp);
	anApp->NewDocument("BinXCAF", doc);

	IGESCAFControl_Reader aStepReader;
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

	cout << "ReadStep fileName Successed" << endl;
}

#endif
