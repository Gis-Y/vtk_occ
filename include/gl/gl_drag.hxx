#ifndef _KMAS_COMX_GL_DRAG_HXX
#define _KMAS_COMX_GL_DRAG_HXX

#include <base\root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {F4729985-490D-4919-8986-37AD524309C4}
 const MUID IID_IGlDragModule = 
{ 0xf4729985, 0x490d, 0x4919, { 0x89, 0x86, 0x37, 0xad, 0x52, 0x43, 0x9, 0xc4 } };

interface IGlDragModule : public IRoot
{
	virtual TStatus STDCALL GetCtrlPoints(double* &pts, int& num, double *&nors) = 0;
	virtual TStatus STDCALL Preview(const int ctl_nid, double* pt) = 0;
	virtual TStatus STDCALL Update(const int ctl_nid, double* pt) = 0;
};

// {00D003A4-6601-44b1-B8BA-E352398DB6D1}
const MUID IID_IGlDragModule2 = 
{ 0xd003a4, 0x6601, 0x44b1, { 0xb8, 0xba, 0xe3, 0x52, 0x39, 0x8d, 0xb6, 0xd1 } };

interface IGlDragModule2 : public IGlDragModule
{
	virtual TStatus STDCALL EnableLazyMode(bool flag = true) = 0;
	virtual TStatus STDCALL DoUpdate() = 0;
	virtual TStatus STDCALL Cancel() = 0;
};


// {B2071D68-C718-492f-9A0F-DAC9A5C8592C}
const MUID IID_IGlDrag = 
{ 0xb2071d68, 0xc718, 0x492f, { 0x9a, 0xf, 0xda, 0xc9, 0xa5, 0xc8, 0x59, 0x2c } };

interface IGlDrag : public IRoot
{
	virtual TStatus STDCALL Activate() = 0;
	virtual TStatus STDCALL Closure() = 0;
};

// {5CBF2A5D-2441-424c-8924-A42E1304C48A}
 const MTYPEID CLSID_IGlDrag = 
{ 0x5cbf2a5d, 0x2441, 0x424c, { 0x89, 0x24, 0xa4, 0x2e, 0x13, 0x4, 0xc4, 0x8a } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
