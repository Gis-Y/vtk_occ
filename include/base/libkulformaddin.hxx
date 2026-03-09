#ifndef COMX_LIBKULFORM_ADDIN_HEADER_64e954fdaf5a4b7ea8922fdc6a4ed858_HXX
#define COMX_LIBKULFORM_ADDIN_HEADER_64e954fdaf5a4b7ea8922fdc6a4ed858_HXX

#include <string>
#include <map>
using namespace std;

#ifndef COMX_LIBKULFORM_EXPORTS_68bf2ddf3e6d4fb3b7d2459b31895aa7_HXX
#define COMX_LIBKULFORM_EXPORTS_68bf2ddf3e6d4fb3b7d2459b31895aa7_HXX

#ifdef LIBKULFORM_EXPORTS
#define LIBKULFORM_API __declspec(dllexport)
#else
#define LIBKULFORM_API __declspec(dllimport)
#endif

#endif

#include <base/type_wrap.hxx>

#ifndef COMX_LIBKULFORM_DATA_PROC_653db066316649b78a5508dbd87d23d6_HXX
#define COMX_LIBKULFORM_DATA_PROC_653db066316649b78a5508dbd87d23d6_HXX

interface IKULData
{
	virtual bool Update(bool is_save = true) = 0;
	virtual bool Bind(const MUID &formid, const int ctrl_id) = 0;
	virtual bool Bind(const MUID &formid, const char *id_alias) = 0;
	virtual bool GetTypeID(char *szID) = 0;
};

#endif

struct IKulXmlElement
{
	virtual int GetX() = 0;
	virtual int GetY() = 0;
	virtual int GetMinWidth() = 0;
	virtual int GetMinHeight() = 0;
	virtual char *GetTagName() = 0;
	virtual char *GetProperty(const char* prop_name) = 0;
	virtual char *GetAttribute(const char *attr_name) = 0;
};

struct IWidgetParser
{
	virtual void OnParser(IKulXmlElement *element, DWORD &type, char *label, DWORD &style, DWORD &ex_style) = 0;
	virtual void OnPickupData(CWnd *pParent, IKulXmlElement *element) = 0;
	virtual void OnInitializeCtrl(CWnd *pParent) = 0;
	virtual void OnTerminalCtrl(CWnd *pParent) = 0;
	virtual void OnUpdataCtrl(CWnd *pParent, const int id, IKULData* p_data_proc, const bool is_save) = 0;
	virtual void OnAssociateParser(IKulXmlElement *element, DWORD &type, char *label, DWORD &style, DWORD &ex_style) = 0;
	virtual void OnBindCheck(IKULData *p_data_proc, bool &can_bind){can_bind = false;}
};

struct IWidgetParser2 : public IWidgetParser
{
	virtual void OnWndProc(CWnd *pParent, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

extern "C" bool LIBKULFORM_API KUL_RegisterAddin(const char *widget_name, IWidgetParser *parser);
extern "C" bool LIBKULFORM_API KUL_GetStaticModuleState(unsigned long &state);

#endif
