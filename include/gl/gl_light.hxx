/*HEAD gl_background.hxx COMX_COMPONENT_MODULE */
/*==============================================================================

  Copyright 2006 Jilin Kingmesh Corp.
  All rights reserved
  
	================================================================================ 
	File description:
	
	-- Please append file description informations here --
	  
	================================================================================
	Date            Name            Description of Change
	
  
	$HISTORY$
	================================================================================
*/

#ifndef _KMAS_COMX_GL_LIGHT_HXX
#define _KMAS_COMX_GL_LIGHT_HXX

#include "base/root.hxx"
#include "gl/gl_base.hxx"
#include "base/factory.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {510B7F71-E942-4be3-B33F-2359B4B6A3A3}
const MUID IID_IGlLightConnector = 
{ 0x510b7f71, 0xe942, 0x4be3, { 0xb3, 0x3f, 0x23, 0x59, 0xb4, 0xb6, 0xa3, 0xa3 } };

comx_interface IGlLightConnector : public IRoot{
	virtual TStatus STDCALL Activate() = 0;
	virtual TStatus STDCALL Closure() = 0;
};

// {E5844D1A-83AE-4c83-8A14-BBFECB2AD4A0}
const MUID IID_IGlLightCustomer = 
{ 0xe5844d1a, 0x83ae, 0x4c83, { 0x8a, 0x14, 0xbb, 0xfe, 0xcb, 0x2a, 0xd4, 0xa0 } };

comx_interface IGlLightCustomer : public IRoot{
	virtual TStatus STDCALL EnableLocalViewer(const bool &flag = true) = 0;
	virtual TStatus STDCALL EnableTwoSideLight(const bool &flag = true) = 0;
	virtual TStatus STDCALL SetModelAmbient(GLfloat model_ambient[4]) = 0;
	virtual TStatus STDCALL AddLampHouse(TGLLampHouse *ptr_lamp_house) = 0;
	virtual TStatus STDCALL UseDefaultLampHouse() = 0;
};

struct gl_light_fn_t
{
	bool (*pfn_activate)();
	bool (*pfn_closure)();
	bool (*pfn_enable_local_viewer)(bool flag);
	bool (*pfn_enable_two_side_light)(bool flag);
	bool (*pfn_set_mode_ambient)(GLfloat model_ambient[4]);
};

// {6BA714A4-8279-471f-A5E1-0B144404C6C8}
const MUID IID_IGlLightFn = 
{ 0x6ba714a4, 0x8279, 0x471f, { 0xa5, 0xe1, 0xb, 0x14, 0x44, 0x4, 0xc6, 0xc8 } };

comx_interface IGlLightFn : public IRoot
{
	virtual TStatus STDCALL GetGlLightFuncs(gl_light_fn_t *&fn) = 0;
};

// {C458AB62-C515-4965-80F8-79FFF7925944}
const MUID CLSID_IGlLight = 
{ 0xc458ab62, 0xc515, 0x4965, { 0x80, 0xf8, 0x79, 0xff, 0xf7, 0x92, 0x59, 0x44 } };

class TGLLightWrap
{
	gl_light_fn_t *_pfn_gl_light;
public:
	TGLLightWrap()
	{
		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IGlLight,
			IGlLightFn,
			p_gl_light_fn);
		
		if (p_gl_light_fn != NULL)
		{
			p_gl_light_fn->GetGlLightFuncs(_pfn_gl_light);
			p_gl_light_fn->Release();
		}
	}
	~TGLLightWrap()
	{
		_pfn_gl_light = NULL;
	}
public:
	void Activate()
	{
		if (_pfn_gl_light != NULL)
		{
			_pfn_gl_light->pfn_enable_two_side_light(true);
			_pfn_gl_light->pfn_activate();
		}
	}
	void Closure()
	{
		if (_pfn_gl_light != NULL)
		{
			_pfn_gl_light->pfn_closure();
		}
	}
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif