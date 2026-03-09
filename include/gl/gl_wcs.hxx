//HEAD---GLWCS.hxx---HEAD 
/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                          All rights reserved

================================================================================
File description:
---Please append file description informations here---
User Description Begin:
1.	
User Description End.

================================================================================
Date			Name			Description of Change
User History Begin:
1.	
User History End.
==============================================================================*/

//IFNDEF Begin:
#ifndef _KMAS_COMX_WCS_INTERFACE_DECLARE_H
#define _KMAS_COMX_WCS_INTERFACE_DECLARE_H
//IFNDEF End.


//Include Files Information Begin:
#include <base/root.hxx>
#include <base/factory.hxx>
//Include Files Information End.
namespace KMAS
{
namespace Die_maker
{
namespace comx
{
// interface---IGlWCS---interface begin
const MUID IID_IGlWCS = 
// IID Begin.
{ 0x2c2ecdc3, 0xc30d, 0x44c9, { 0x96, 0x8d, 0x8c, 0x57, 0x51, 0xd7, 0xef, 0xcd } };
// IID End.
comx_interface IGlWCS : public IRoot
{
	virtual TStatus STDCALL GetRange(double &xmin , double &ymin , double &zmin , double &xmax , double &ymax , double &zmax) = 0;
	virtual TStatus STDCALL GetScale(double &scale) = 0;
	virtual TStatus STDCALL GetCenter(double &x , double &y , double &z) = 0;
};


// {1DE4BEB0-F6FB-4228-9E84-1C25AF2E7258}
const MUID IID_IGlWCS2 = 
{ 0x1de4beb0, 0xf6fb, 0x4228, { 0x9e, 0x84, 0x1c, 0x25, 0xaf, 0x2e, 0x72, 0x58 } };

comx_interface IGlWCS2 : public IGlWCS
{
	virtual TStatus STDCALL IsValid(bool &flag) = 0;
};
// interface---IGlWCS---interface end.

// 
// // interface---IGlWCS2---interface begin
// const MUID IID_IGlWCS2 = 
// // IID Begin.
// { 0xda835399, 0x2a6d, 0x45f2, { 0xac, 0x10, 0x19, 0x10, 0xc7, 0xca, 0xec, 0xc8 } };
// // IID End.
// interface IGlWCS2 : public IGlWCS
// {
// 	virtual TStatus STDCALL SetRange(double xmin , double ymin , double zmin , double xmax , double ymax , double zmax) = 0;
// 	virtual TStatus STDCALL SetScale(double scale) = 0;
// 	virtual TStatus STDCALL SetCenter(double x , double y , double z) = 0;
// };
// // interface---IGlWCS2---interface end.

struct gl_wcs_func_t
{
	bool (*fn_get_range)(
		double &xmin, 
		double &ymin, 
		double &zmin, 
		double &xmax, 
		double &ymax, 
		double &zmax);
};

// {50D08A2D-D904-4991-AE91-D9F5FD188EDE}
const MUID IID_IGlWCSFunc = 
{ 0x50d08a2d, 0xd904, 0x4991, { 0xae, 0x91, 0xd9, 0xf5, 0xfd, 0x18, 0x8e, 0xde } };

comx_interface IGlWCSFunc : public IRoot
{
	virtual TStatus STDCALL GetFunc(gl_wcs_func_t *&fn) = 0;
};

const MUID CLSID_IGlWCS = 
//CLSID Begin:
{ 0x78ca534e, 0xc025, 0x49f4, { 0x9a, 0xe8, 0xb5, 0xd0, 0xb0, 0xcf, 0x49, 0x7a } };
//CLSID End.

class TGLWCSWrap
{
	gl_wcs_func_t *_p_gl_wcs_func;
public:
	TGLWCSWrap()
	{
		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IGlWCS,
			IGlWCSFunc,
			p_gl_wcs_func);
		
		if (p_gl_wcs_func != NULL)
		{
			p_gl_wcs_func->GetFunc(_p_gl_wcs_func);
			p_gl_wcs_func->Release();
		}
	}
	~TGLWCSWrap()
	{
		_p_gl_wcs_func = NULL;
	}
public:
	bool GetRange(double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax)
	{
		return _p_gl_wcs_func->fn_get_range(xmin, ymin, zmin, xmax, ymax, zmax);
	}
	bool GetParameters(double &cx, double &cy, double &cz, double &scale)
	{
		double xmin = 0.0, ymin = 0.0, zmin = 0.0, xmax = 0.0, ymax = 0.0, zmax = 0.0;
		bool ret = GetRange(xmin, xmax, ymin, ymax, zmin, zmax);

		if (ret)
		{
			scale = max(xmax - xmin , ymax - ymin);
			scale = max(scale , zmax - zmin);
			scale *= 1.25;
			
			cx = (xmin + xmax) / 2.0;
			cy = (ymin + ymax) / 2.0;
			cz = (zmin + zmax) / 2.0;
		}

		return ret;
	}
	double max(double left, double right)
	{
		return left > right ? left : right;
	}
	double min(double left, double right)
	{
		return left < right ? left : right;
	}
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS
#endif