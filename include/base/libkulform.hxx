#ifndef LIB_KUL_FORM_HXX_3ECB4002_F04C_4323_910D_1A34608BB0F4
#define LIB_KUL_FORM_HXX_3ECB4002_F04C_4323_910D_1A34608BB0F4

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBKULFORM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBKULFORM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef COMX_LIBKULFORM_EXPORTS_68bf2ddf3e6d4fb3b7d2459b31895aa7_HXX
#define COMX_LIBKULFORM_EXPORTS_68bf2ddf3e6d4fb3b7d2459b31895aa7_HXX

#ifdef LIBKULFORM_EXPORTS
#define LIBKULFORM_API __declspec(dllexport)
#else
#define LIBKULFORM_API __declspec(dllimport)
#endif

#endif

#ifndef KUL_MODALESS_DIALOG_DESTROY 
#define KUL_MODALESS_DIALOG_DESTROY (WM_USER + 10240)
#endif

#ifndef KUL_UPDATE_FROM_COMMON_AREA 
#define KUL_UPDATE_FROM_COMMON_AREA (WM_USER + 10241)
#endif

#include <base/type_wrap.hxx>
#include <assert.h>

#include <string>
#include <map>
using namespace std;

using namespace KMAS::Die_maker::comx;
using namespace KMAS;

#ifndef interface
#define interface struct
#endif

#ifndef COMX_LIBKULFORM_DATA_PROC_653db066316649b78a5508dbd87d23d6_HXX
#define COMX_LIBKULFORM_DATA_PROC_653db066316649b78a5508dbd87d23d6_HXX

interface IKULData
{
	virtual bool Update(const bool &is_save) = 0;
	virtual bool Bind(const MUID &formid, const int ctrl_id) = 0;
	virtual bool Bind(const MUID &formid, const char *id_alias) = 0;
	virtual bool GetTypeID(char *szID) = 0;
};

#endif

interface IKULSimpleData : public IKULData//TypeID is "simple"
{
	virtual bool GetFromModule(char *&data) = 0;
	virtual bool SetToModule(const char *data) = 0;
	virtual bool SetValidInfoToModule(const bool &flag) = 0;
};

interface IKULRangeData : public IKULData//TypeID is "range"
{
	virtual bool GetFromModule(int &low, int &high) = 0;
	virtual bool SetToModule(const int &low, const int &high) = 0;
};

interface IKULListData : public IKULData// type id is "list"
{
	virtual bool GetFromModule(char *&list) = 0;
	virtual bool SetToModule(const char *list) = 0;
};

interface IKULIndex1Data : public IKULData// type id is "index1"
{
	virtual bool GetFromModule(int &index) = 0;
	virtual bool SetToModule(const int &index) = 0;
};

interface IKULTableData : public IKULData// type id is "table"
{
	virtual bool GetFromModule(char *&list) = 0;
	virtual bool SetToModule(const char *list) = 0;
};

interface IKULIndex2Data : public IKULData// type id is "index2"
{
	virtual bool GetFromModule(int &index) = 0;
	virtual bool SetToModule(const int &index) = 0;
};

typedef LRESULT (*fn_wnd_proc_t)(const MUID &formid, UINT message, WPARAM wParam, LPARAM lParam);
typedef bool (*fn_init_proc_t)(const MUID &formid);
//typedef bool (*fn_data_proc_t)(unsigned int item_id, KVARIANT *pparas);

extern "C" bool LIBKULFORM_API KUL_SetInitEnvironmentWithCOMX();
extern "C" bool LIBKULFORM_API KUL_SetInitDirectory(
	const char* const dir_path);
extern "C" bool LIBKULFORM_API KUL_SetBitmapDirectory(
	const char* const dir_path);
extern "C" bool LIBKULFORM_API KUL_SetLocalBitmapDirectory(
	const char* const dir_path);
extern "C" bool LIBKULFORM_API KUL_SetAddinDirectory(
	const char* const dir_path);
extern "C" bool LIBKULFORM_API KUL_GetInitDirectory(char* dir_path);
extern "C" bool LIBKULFORM_API KUL_GetBitmapDirectory(char* dir_path);
extern "C" bool LIBKULFORM_API KUL_GetLocalBitmapDirectory(char* dir_path);
extern "C" bool LIBKULFORM_API KUL_GetAddinDirectory(char* dir_path);
extern "C" bool LIBKULFORM_API KUL_PickupDialogTemplate(
	const char* const kul_fname, 
	const MUID &formid, 
	DLGTEMPLATE *&buf, 
	int buf_size);
extern "C" bool LIBKULFORM_API KUL_ClearLastDialogTemplate();
extern "C" bool LIBKULFORM_API KUL_CreateModalForm(
	const char* const kul_fname, 
	const MUID &formid, 
	HWND hParent, 
	fn_wnd_proc_t wnd_proc);
extern "C" bool LIBKULFORM_API KUL_CreateModallessForm(
	const char* const kul_fname, 
	const MUID &formid, 
	HWND hParent, 
	fn_wnd_proc_t wnd_proc,
	HWND *&hWnd);
extern "C" bool LIBKULFORM_API KUL_OnSizeEvent(
	const MUID &formid,
	int cx,
	int cy);
extern "C" bool LIBKULFORM_API KUL_UpdateForm(
	const MUID &formid,
	bool is_save);
extern "C" bool LIBKULFORM_API KUL_PickupDialogTemplateFormUnit(
	const MUID &formid, 
	const char* const unit_name, 
	DLGTEMPLATE *&buf, 
	int buf_size);
extern "C" bool LIBKULFORM_API KUL_CreateModalFormFromUnit(
	const char* const unit_name, 
	const MUID &formid, 
	HWND hParent, 
	fn_wnd_proc_t wnd_proc);
extern "C" bool LIBKULFORM_API KUL_CreateModallessFormFromUnit(
	const char* const unit_name, 
	const MUID &formid, 
	HWND hParent, 
	fn_wnd_proc_t wnd_proc,
	HWND *&hWnd);

extern "C" bool LIBKULFORM_API KUL_PostMessage(const MUID &formid, UINT Msg, WPARAM wParam, LPARAM lParam);

extern "C" bool LIBKULFORM_API KUL_GetWindowHandle(
	const MUID &formid,
	HWND &hWnd);

extern "C" bool LIBKULFORM_API KUL_GetIDFromAlias(
	const MUID &formid,
	const char *id_alias,
	int &id);

extern "C" bool LIBKULFORM_API KUL_EnableSubControl(const MUID &formid, const char *id_alias, bool flag);
extern "C" bool LIBKULFORM_API KUL_IsEnableSubControl(const MUID &formid, const int id);
extern "C" bool LIBKULFORM_API KUL_GetSubControlHandle(const MUID &formid, const int id, unsigned long &hdr);
extern "C" bool LIBKULFORM_API KUL_VisibleSubControl(const MUID &formid, const int id, bool flag);
extern "C" bool LIBKULFORM_API KUL_IsVisibleSubControl(const MUID &formid, const int id);
extern "C" bool LIBKULFORM_API KUL_EnableSubControlByID(const MUID &formid, const int id, bool flag);
// extern "C" bool LIBKULFORM_API KUL_UpdateKULProperty(const MUID &formid);
extern "C" bool LIBKULFORM_API KUL_GetKULProperty(const MUID &formid, const char *id_alias, const char *name, char *val);
extern "C" bool LIBKULFORM_API KUL_GetKULPropertyByID(const MUID &formid, const int id, const char *name, char *val);
// extern "C" bool LIBKULFORM_API KUL_SetKULProperty(const MUID &formid, const char *id_alias, const char *name, const char *val);
extern "C" bool LIBKULFORM_API KUL_BindDataProc(const MUID &formid, const int id, IKULData *p_data_proc);
extern "C" bool LIBKULFORM_API KUL_UnBindDataProc(const MUID &formid, const int id);
extern "C" bool LIBKULFORM_API KUL_UpdateData(const MUID &formid, const int id, const bool is_save);
extern "C" bool LIBKULFORM_API KUL_UpdateAllData(const MUID &formid, const bool is_save);
extern "C" bool LIBKULFORM_API KUL_RegisterInitProc(const MUID &formid, fn_init_proc_t init_proc);
extern "C" bool LIBKULFORM_API KUL_GetBindVarCount(const MUID &formid, int &cnt);
extern "C" bool LIBKULFORM_API KUL_GetBindVar(const MUID &formid, const int &index, char *type, char *id, char *var);
//extern "C" bool LIBKULFORM_API KUL_GetSubFormIDs(const MUID &formid, int *&ids, int &ids_num);
extern "C" bool LIBKULFORM_API KUL_SetStackWidgetIndex(const MUID &formid, const int id, const int index);
extern "C" int  LIBKULFORM_API KUL_GetStackWidgetIndex(const MUID &formid, const int id);

extern "C" bool LIBKULFORM_API KUL_SetTabctrlWidgetIndex(const MUID &formid, const int id, const int index);
extern "C" int  LIBKULFORM_API KUL_GetTabctrlWidgetIndex(const MUID &formid, const int id);

extern "C" void LIBKULFORM_API KUL_RegisterRunOnceCallback(void (*cb)());

#define IMPLMENT_KUL_BINDER(sub_type_name, type_id)\
private:                                           \
	MUID _formid;                                    \
	int  _ctrl_id;                                   \
private:                                                                 \
	void init(){_ctrl_id = -1; memset(&_formid, 0, sizeof(_formid));}  \
public:\
	~sub_type_name(){if(_ctrl_id != -1)KUL_UnBindDataProc(_formid, _ctrl_id);}\
public:                                                                 \
	virtual bool Update(const bool &is_save = true)                        \
{                                                                      \
	bool ret = false;                                                    \
	\
	if (-1 != _ctrl_id)                                                  \
{                                                                  \
	ret = KUL_UpdateData(_formid, _ctrl_id, is_save);                \
}                                                                  \
	\
	return ret;                                                          \
}                                                                      \
	\
	virtual bool Bind(const MUID &formid, const int ctrl_id)               \
{                                                                      \
	_formid = formid;                                                    \
	_ctrl_id = ctrl_id;                                                  \
	KUL_BindDataProc(_formid, _ctrl_id, (IKULData*)this);                \
	\
	return true;                                                         \
}                                                                      \
	\
	virtual bool Bind(const MUID &formid, const char *id_alias)            \
{                                                                      \
	KUL_GetIDFromAlias(formid, id_alias, _ctrl_id);                      \
	assert(_ctrl_id != -1);                                              \
	_formid = formid;                                                    \
	KUL_BindDataProc(_formid, _ctrl_id, (IKULData*)this);                \
	\
	return true;                                                         \
}                                                                      \
	\
	virtual bool Bind(TKULFormBase *pThis, const int ctrl_id)              \
{                                                                      \
	_formid = pThis->_form_id;                                           \
	_ctrl_id = ctrl_id;                                                  \
	KUL_BindDataProc(_formid, _ctrl_id, (IKULData*)this);                \
	\
	return true;                                                         \
}                                                                      \
	\
	virtual bool Bind(TKULFormBase *pThis, const char *id_alias)           \
{                                                                      \
	KUL_GetIDFromAlias(pThis->_form_id, id_alias, _ctrl_id);             \
	assert(_ctrl_id != -1);                                              \
	_formid = pThis->_form_id;                                           \
	KUL_BindDataProc(_formid, _ctrl_id, (IKULData*)this);                \
	\
	return true;                                                         \
}                                                                      \
public:                                                                 \
	virtual bool GetTypeID(char *szID)                                     \
{                                                                      \
	strcpy(szID, type_id);                                               \
	return true;                                                         \
}

#define KUL_BEGIN_MSG_MAP(theClass) \
	virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam)\
{\
	LRESULT lResult = 0;

#define KUL_END_MSG_MAP()\
	return lResult;\
}

#define KUL_MESSAGE_FILTER(func) \
	if (func(message, wParam, lParam))\
	{\
		return lResult;\
	}

#define KUL_MESSAGE_HANDLER(msg, func) \
	if(message == msg) \
{ \
	lResult = func(message, wParam, lParam); \
}

#define KUL_MESSAGE_RANGE_HANDLER(msgFirst, msgLast, func) \
	if(message >= msgFirst && message <= msgLast) \
{ \
	lResult = func(message, wParam, lParam); \
}

#define KUL_COMMAND_ID_HANDLER(id, func) \
	if(message == WM_COMMAND && GetIDFromAlias(id) == LOWORD(wParam)) \
{ \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam); \
}

#define KUL_COMMAND_HANDLER(func) \
	if(message == WM_COMMAND) \
{ \
	lResult = func(LOWORD(wParam)); \
}

#define KUL_COMMAND_CODE_HANDLER(code, func) \
	if(message == WM_COMMAND && code == HIWORD(wParam)) \
{ \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam); \
}

#define KUL_COMMAND_RANGE_HANDLER(idFirst, idLast, func) \
	if(message == WM_COMMAND && LOWORD(wParam) >= GetIDFromAlias(idFirst)  && LOWORD(wParam) <= GetIDFromAlias(idLast)) \
{ \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam); \
}

#define KUL_COMMAND_RANGE_CODE_HANDLER(idFirst, idLast, code, func) \
	if(message == WM_COMMAND && code == HIWORD(wParam) && LOWORD(wParam) >= GetIDFromAlias(idFirst)  && LOWORD(wParam) <= GetIDFromAlias(idLast)) \
{ \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam); \
}

#define KUL_NOTIFY_HANDLER(id, cd, func) \
	if(message == WM_NOTIFY && GetIDFromAlias(id) == ((LPNMHDR)lParam)->idFrom && cd == ((LPNMHDR)lParam)->code) \
{ \
	lResult = func((int)wParam, (LPNMHDR)lParam); \
}

#define KUL_NOTIFY_ID_HANDLER(id, func) \
	if(message == WM_NOTIFY && GetIDFromAlias(id) == ((LPNMHDR)lParam)->idFrom) \
{ \
	lResult = func((int)wParam, (LPNMHDR)lParam); \
}

#define KUL_NOTIFY_CODE_HANDLER(cd, func) \
	if(message == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
{ \
	lResult = func((int)wParam, (LPNMHDR)lParam); \
}

#define KUL_NOTIFY_RANGE_HANDLER(idFirst, idLast, func) \
	if(message == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= GetIDFromAlias(idFirst) && ((LPNMHDR)lParam)->idFrom <= GetIDFromAlias(idLast)) \
{ \
	lResult = func((int)wParam, (LPNMHDR)lParam); \
}

#define KUL_NOTIFY_RANGE_CODE_HANDLER(idFirst, idLast, cd, func) \
	if(message == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code && ((LPNMHDR)lParam)->idFrom >= GetIDFromAlias(idFirst) && ((LPNMHDR)lParam)->idFrom <= GetIDFromAlias(idLast)) \
{ \
	lResult = func((int)wParam, (LPNMHDR)lParam); \
}

class TKULFormBase
{
	bool m_bIsInitSuccessful;
public:
	bool IsInitSuccessful(){return m_bIsInitSuccessful;}
public:
	MUID _form_id;
	string _unit_name;
public:
	operator HWND()
	{
		HWND hWnd = NULL;
		KUL_GetWindowHandle(_form_id, hWnd);

		return hWnd;
	}
public:
	TKULFormBase(const MUID &formid, const string &unit) : 
	  _form_id(formid), _unit_name(unit)
	  {
		  m_bIsInitSuccessful = RegisterEntry(formid, this);
	  }
	  ~TKULFormBase()
	  {
		  UnregisterEntry(_form_id);
	  }
protected:
	static LRESULT KulWndProc(const MUID &formid, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT ret = 0;

		if (getEntry().find(formid) != getEntry().end())
		{
			ret = getEntry()[formid]->WndProc(message, wParam, lParam);
		}

		if(message == WM_CREATE)
		{
			KUL_RegisterInitProc(formid, initProc);
		}

		return ret;
	}
public:
	void DoModal(HWND hParent = NULL)
	{
// 		if (!getModalLock())
// 		{
// 			lockModal();

			KUL_SetInitEnvironmentWithCOMX();
			KUL_CreateModalFormFromUnit(_unit_name.c_str(), 
				_form_id, hParent, KulWndProc);

// 			unlockModal();
// 		}
	}
	void DoModalLess(HWND *&hWndRet, HWND hParent = NULL)
	{
		hWndRet = NULL;

		KUL_SetInitEnvironmentWithCOMX();
		KUL_CreateModallessFormFromUnit(_unit_name.c_str(), 
			_form_id, hParent, KulWndProc, hWndRet);
	}
	int GetIDFromAlias(const char *id_alias)
	{
		int ret = -1;
		KUL_GetIDFromAlias(_form_id, id_alias, ret);

		return ret;
	}
	// 	void UpdateKULProperty()
	// 	{
	// 		KUL_UpdateKULProperty(_form_id);
	// 	}
	bool GetKULProperty(const char *id_alias, const char *name, char *val)
	{
		return KUL_GetKULProperty(_form_id, id_alias, name, val);
	}
	bool GetKULProperty(const int id, const char *name, char *val)
	{
		return KUL_GetKULPropertyByID(_form_id, id, name, val);
	}
	// 	bool SetKULProperty(const char *id_alias, const char *name, const char *val)
	// 	{
	// 		return KUL_SetKULProperty(_form_id, id_alias, name, val);
	// 	}
	void EnableSubControl(const char *id_alias, bool flag)
	{
		KUL_EnableSubControl(_form_id, id_alias, flag);
	}
	bool IsEnableSubControl(const int id)
	{
		return KUL_IsEnableSubControl(_form_id, id);
	}
	bool GetSubControlHandle(const MUID &formid, const int id, unsigned long &hdr)
	{
		return KUL_GetSubControlHandle(_form_id, id, hdr);
	}
	void VisibleSubControl(const int id, bool flag)
	{
		KUL_VisibleSubControl(_form_id, id, flag);
	}
	bool IsVisibleSubControl(const int id)
	{
		return KUL_IsVisibleSubControl(_form_id, id);
	}
	void EnableSubControl(const int id, bool flag)
	{
		KUL_EnableSubControlByID(_form_id, id, flag);
	}
	bool GetBindVarCount(int &cnt)
	{
		return KUL_GetBindVarCount(_form_id, cnt);
	}
    bool GetBindVar(const int &index, char *type, char *id, char *var)
	{
		return KUL_GetBindVar(_form_id, index, type, id, var);
	}
	void SetStackWidgetIndex(const int id, const int index)
	{
		KUL_SetStackWidgetIndex(_form_id, id, index);
	}
	int  GetStackWidgetIndex(const int id)
	{
		return KUL_GetStackWidgetIndex(_form_id, id);
	}
	void SetTabctrlWidgetIndex(const int id, const int index)
	{
		KUL_SetTabctrlWidgetIndex(_form_id, id, index);
	}
	int  GetTabctrlWidgetIndex(const int id)
	{
		return KUL_GetTabctrlWidgetIndex(_form_id, id);
	}
private:
	static map<MUID, TKULFormBase*> &getEntry()
	{
		static map<MUID, TKULFormBase*> entry;
		return entry;
	}
	static bool RegisterEntry(const MUID &formid, TKULFormBase* pForm)
	{
		bool ret = false;
		if (getEntry().find(formid) == getEntry().end())
		{
			getEntry()[formid] = pForm;
			ret = true;
		}
		return ret;
	}
	static void UnregisterEntry(const MUID &formid)
	{
		if (getEntry().find(formid) != getEntry().end())
		{
			getEntry().erase(getEntry().find(formid));
		}
	}
protected:
	bool PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return KUL_PostMessage(_form_id, Msg, wParam, lParam);
	}
protected:
	virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;
private:
	static bool &getModalLock()
	{
		static bool is_lock = false;

		return is_lock;
	}
	static void lockModal()
	{
		getModalLock() = true;
	}
	static void unlockModal()
	{
		getModalLock() = false;
	}
private:
	static bool initProc(const MUID &formid)
	{
		return getEntry()[formid]->OnInitData();
	}
public:
	virtual bool OnInitData(){return true;}
};

//////////////////////////////////////////////////////////////////////////
// Deal with simple data.

template<typename T>
class TKULSimpleData : public IKULSimpleData
{
	IMPLMENT_KUL_BINDER(TKULSimpleData, "simple")
private:
	T _data;
	bool _is_valid;
	string _buf;
public:
	TKULSimpleData(){memset(&_data, 0, sizeof(_data));init();}
public:
	virtual bool GetFromModule(char *&data)
	{
		//string str_data = type_cast<string>(_data);
		_buf = type_cast<string>(_data);
		data = (char*)_buf.c_str();
		//strcpy(data, str_data.c_str());

		return true;
	}

	virtual bool SetToModule(const char *data)
	{
		_data = type_cast<T>(data);
		return true;
	}

	virtual bool SetValidInfoToModule(const bool &flag)
	{
		_is_valid = flag;
		return true;
	}
public:
	T    Get(){Update(true); return _data;}
	void Set(const T &val){_data = val; Update(false);}
	bool IsValid(){Update(true); return _is_valid;}
public:
	operator T(){Update(true); return _data;}
private:
	const TKULSimpleData &operator=(const TKULSimpleData &right);
	TKULSimpleData(const TKULSimpleData &right);
};

template<>
class TKULSimpleData<string> : public IKULSimpleData
{
	IMPLMENT_KUL_BINDER(TKULSimpleData, "simple")
private:
	string _data;
	bool _is_valid;
public:
	TKULSimpleData(){_data = "";init();_is_valid = false;}
public:
	virtual bool GetFromModule(char *&data)
	{
		//strcpy(data, _data.c_str());
		data = (char*)_data.c_str();

		return true;
	}

	virtual bool SetToModule(const char *data)
	{
		_data = data;
		return true;
	}

	virtual bool SetValidInfoToModule(const bool &flag)
	{
		_is_valid = flag;
		return true;
	}
public:
	string    Get(){Update(true); return _data;}
	void Set(const string &val){_data = val; Update(false);}
	bool IsValid(){Update(true); return _is_valid;}
public:
	operator string(){return _data;}
private:
	const TKULSimpleData &operator=(const TKULSimpleData &right);
	TKULSimpleData(const TKULSimpleData &right);
};

template<typename T>
class kul_simple_bind : public TKULSimpleData<T>
{
public:
	kul_simple_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
	}
	kul_simple_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
	}
	kul_simple_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
	}
	kul_simple_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
	}
public:
	operator T(){return Get();}
	void operator=(const T &val){Set(val);}
};

class kul_simple_valid_bind : public TKULSimpleData<string>
{
public:
	kul_simple_valid_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
	}
	kul_simple_valid_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
	}
	kul_simple_valid_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
	}
	kul_simple_valid_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
	}
public:
	operator bool(){return IsValid();}
	void operator=(const bool &val){Set(val? "true" : "false");}
};

//////////////////////////////////////////////////////////////////////////
// Deal with range data.

template<typename T>
class TKULRangeData : public IKULRangeData
{
	IMPLMENT_KUL_BINDER(TKULRangeData, "range")
private:
	T _low,_high;
public:
	TKULRangeData(){memset(&_low, 0, sizeof(_low));memset(&_high, 0, sizeof(_high));init();}
public:
	virtual bool GetFromModule(int &low, int &high)
	{
		low = type_cast<int>(_low);
		high = type_cast<int>(_high);

		return true;
	}

	virtual bool SetToModule(const int &low, const int &high)
	{
		_low = type_cast<T>(low);
		_high = type_cast<T>(high);

		return true;
	}

public:
	T    GetLow(){Update(true); return _low;}
	void SetLow(const T &val){_low = val; Update(false);}
	T    GetHigh(){Update(true); return _high;}
	void SetHigh(const T &val){_high = val; Update(false);}

private:
	const TKULRangeData &operator=(const TKULRangeData &right);
};

template<typename T>
class kul_range_low_bind : public TKULRangeData<T>
{
public:
	kul_range_low_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
		Update(true);
	}
	kul_range_low_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
		Update(true);
	}
	kul_range_low_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
		Update(true);
	}
	kul_range_low_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
		Update(true);
	}
public:
	operator T(){return GetLow();}
	void operator=(const T &val){SetLow(val);}
};

template<typename T>
class kul_range_high_bind : public TKULRangeData<T>
{
public:
	kul_range_high_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
		Update(true);
	}
	kul_range_high_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
		Update(true);
	}
	kul_range_high_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
		Update(true);
	}
	kul_range_high_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
		Update(true);
	}
public:
	operator T(){return GetHigh();}
	void operator=(const T &val){SetHigh(val);}
};

//////////////////////////////////////////////////////////////////////////
// Deal with list data.

template<typename T>
class TKULListData : public IKULListData
{
	IMPLMENT_KUL_BINDER(TKULListData, "list")
private:
// 	vector<T> _list_data;
	string buf_data;
public:
	TKULListData(){init();}
public:
// 	virtual bool GetFromModule(char *&list)
// 	{
// 		stringstream sstr;
// 		sstr << _list_data.size() << " ";
// 
// 		for (size_t loop = 0; loop < _list_data.size(); ++loop)
// 		{
// 			sstr << _list_data[loop] << " ";
// 		}
// 
// 		buf_data = sstr.str();
// 		list = const_cast<char*>(buf_data.c_str());
// 
// 		return true;
// 	}
// 
// 	virtual bool SetToModule(const char *list)
// 	{
// 		int cnt = 0;
// 		stringstream sstr(list);
// 		sstr >> cnt;
// 
// 		_list_data.clear();
// 		for (int loop = 0; loop < cnt; ++loop)
// 		{
// 			T val;
// 			sstr >> val;
// 			_list_data.push_back(val);
// 		}
// 
// 		return true;
// 	}
	virtual bool GetFromModule(char *&list)
	{
		list = const_cast<char*>(buf_data.c_str());
		
		return true;
	}
	
	virtual bool SetToModule(const char *list)
	{
		buf_data = list;
		
		return true;
	}
public:
	string    Get()
	{
		Update(true); 
		char *szList = NULL;
		GetFromModule(szList);
		return szList;
	}
	//int  Size(){Update(true); return _list_data.size();}
public:
// 	T operator[](const int index)
// 	{
// 		assert(index >= 0 && index < _list_data.size());
// 		return _list_data[index];
// 	}
// 	void operator=(string buf)
// 	{
// 		int cnt = 0;
// 		stringstream sstr(buf);
// 		sstr >> cnt;
// 
// 		_list_data.clear();
// 		for (int loop = 0; loop < cnt; ++loop)
// 		{
// 			T val;
// 			sstr >> val;
// 			_list_data.push_back(val);
// 		}
// 
// 		Update(false);
// 	}
	operator T(){return Get();}
	void operator=(string buf)
	{
		buf_data = buf;
		
		Update(false);
	}
private:
	const TKULListData &operator=(const TKULListData &right);
};

template<typename T>
class kul_list_bind : public TKULListData<T>
{
public:
	kul_list_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
		Update(true);
	}
	kul_list_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
		Update(true);
	}
	kul_list_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
		Update(true);
	}
	kul_list_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
		Update(true);
	}
public:
	void operator=(string buf)
	{
		TKULListData<T>::operator =(buf);
	}
// 	T operator[](const int index)
// 	{
// 		return TKULListData<T>::operator[](index);
// 	}
	operator T(){return Get();}
};

//////////////////////////////////////////////////////////////////////////
// deal with index1 data.

template<typename T>
class TKULIndex1Data : public IKULIndex1Data
{
	IMPLMENT_KUL_BINDER(TKULIndex1Data, "index1")
private:
	T _index;
public:
	TKULIndex1Data(){init(); memset(&_index, 0, sizeof(_index));}
public:
	virtual bool GetFromModule(int &index)
	{
		index = type_cast<int>(_index);
		return true;
	}
	virtual bool SetToModule(const int &index)
	{
		_index = type_cast<T>(index);
		return true;
	}
public:
	operator T(){Update(true); return _index;}
	void operator=(const T index){_index = index; Update(false);}
};

template<typename T>
class kul_index1_bind : public TKULIndex1Data<T>
{
public:
	kul_index1_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
		Update(true);
	}
	kul_index1_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
		Update(true);
	}
	kul_index1_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
		Update(true);
	}
	kul_index1_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
		Update(true);
	}
public:
	void operator=(const T index)
	{
		TKULIndex1Data<T>::operator =(index);
	}
	operator T()
	{
		return TKULIndex1Data<T>::operator T();
	}
};

//////////////////////////////////////////////////////////////////////////
// Deal with table data.

template<typename T>
class TKULTableData : public IKULTableData
{
	IMPLMENT_KUL_BINDER(TKULTableData, "table")
private:
	string buf_data;
public:
	TKULTableData(){init();}
public:
	virtual bool GetFromModule(char *&list)
	{
		list = const_cast<char*>(buf_data.c_str());

		return true;
	}

	virtual bool SetToModule(const char *list)
	{
		buf_data = list;

		return true;
	}
public:
	string    Get()
	{
		Update(true); 
		char *szList = NULL;
		GetFromModule(szList);
		return szList;
	}
public:
	operator T(){return Get();}
	void operator=(string buf)
	{
		buf_data = buf;

		Update(false);
	}
private:
	const TKULTableData &operator=(const TKULTableData &right);
};

template<typename T>
class kul_table_bind : public TKULTableData<T>
{
public:
	kul_table_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
		Update(true);
	}
	kul_table_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
		Update(true);
	}
	kul_table_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
		Update(true);
	}
	kul_table_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
		Update(true);
	}
public:
	void operator=(string buf)
	{
		TKULTableData<T>::operator =(buf);
	}
	operator T(){return Get();}
};

//////////////////////////////////////////////////////////////////////////
// deal with index2 data.

template<typename T>
class TKULIndex2Data : public IKULIndex2Data
{
	IMPLMENT_KUL_BINDER(TKULIndex2Data, "index2")
private:
	T _index;
public:
	TKULIndex2Data(){init(); memset(&_index, 0, sizeof(_index));}
public:
	virtual bool GetFromModule(int &index)
	{
		index = type_cast<int>(_index);
		return true;
	}
	virtual bool SetToModule(const int &index)
	{
		_index = type_cast<T>(index);
		return true;
	}
public:
	operator T(){Update(true); return _index;}
	void operator=(const T index){_index = index; Update(false);}
};

template<typename T>
class kul_index2_bind : public TKULIndex2Data<T>
{
public:
	kul_index2_bind(const MUID &formid, const char *id_alias)
	{
		Bind(formid, id_alias);
		Update(true);
	}
	kul_index2_bind(TKULFormBase *pThis, const char *id_alias)
	{
		Bind(pThis->_form_id, id_alias);
		Update(true);
	}
	kul_index2_bind(const MUID &formid, const int id)
	{
		Bind(formid, id);
		Update(true);
	}
	kul_index2_bind(TKULFormBase *pThis, const int id)
	{
		Bind(pThis->_form_id, id);
		Update(true);
	}
public:
	void operator=(const T index)
	{
		TKULIndex2Data<T>::operator =(index);
	}
	operator T()
	{
		return TKULIndex2Data<T>::operator T();
	}
};

#endif
