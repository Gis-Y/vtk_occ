/*HEAD gl_material.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_MATERIAL_HXX
#define _KMAS_COMX_GL_MATERIAL_HXX

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {DCAEECF1-69C7-4b42-9A5F-44A41BBFAEC9}
const MUID IID_IGlMaterialConnector = 
{ 0xdcaeecf1, 0x69c7, 0x4b42, { 0x9a, 0x5f, 0x44, 0xa4, 0x1b, 0xbf, 0xae, 0xc9 } };

comx_interface IGlMaterialConnector : public IRoot{
	virtual TStatus STDCALL Activate() = 0;
	virtual TStatus STDCALL Closure() = 0;
};

// {A839BFB5-85D9-4af2-87CF-301630828060}
const MUID IID_IGlMaterialCustomer = 
{ 0xa839bfb5, 0x85d9, 0x4af2, { 0x87, 0xcf, 0x30, 0x16, 0x30, 0x82, 0x80, 0x60 } };

comx_interface IGlMaterialCustomer : public IRoot{
	virtual TStatus STDCALL SetAmbient(const GLfloat &red,const GLfloat &green,const GLfloat &blue,const GLfloat &alpha) = 0;
	virtual TStatus STDCALL SetDiffuse(const GLfloat &red,const GLfloat &green,const GLfloat &blue,const GLfloat &alpha) = 0;
	virtual TStatus STDCALL SetSpecular(const GLfloat &red,const GLfloat &green,const GLfloat &blue,const GLfloat &alpha) = 0;
	virtual TStatus STDCALL SetEmission(const GLfloat &red,const GLfloat &green,const GLfloat &blue,const GLfloat &alpha) = 0;
	virtual TStatus STDCALL SetShininess(const GLfloat &shininess) = 0;
	virtual TStatus STDCALL SetColorMaterialMode(GLenum mode) = 0;
	virtual TStatus STDCALL SetMaterialFace(GLenum face) = 0;
	virtual TStatus STDCALL EnableColorMaterial(const GLboolean &is_color_material) = 0;
};

// {1568F328-E372-4664-B581-703E93128A9B}
const MUID CLSID_IGlMaterial = 
{ 0x1568f328, 0xe372, 0x4664, { 0xb5, 0x81, 0x70, 0x3e, 0x93, 0x12, 0x8a, 0x9b } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif