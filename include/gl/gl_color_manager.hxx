/*HEAD gl_color_manager.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_COLOR_MANAGER_H
#define _KMAS_COMX_GL_COLOR_MANAGER_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum COLOR_TYPE
{
	FEM_SAFE_COLOR,
	FEM_ERR_COLOR,
	FEM_WARING_COLOR,
	FEM_CHOOSEED_FACE_COLOR,
	FEM_CHOOSEED_LINE_COLOR,
	FEM_CHOOSEED_POINT_COLOR,
	FEM_CHOOSING_FACE_COLOR,
	FEM_CHOOSING_POINT_COLOR,
	FEM_CHOOSING_LINE_COLOR,
	FEM_ACTIVE_FACE_COLOR,
	FEM_ACTIVE_LINE_COLOR,
	FEM_ACTIVE_POINT_COLOR,
	FEM_ARROW_COLOR,
	FEM_NORMAL_FRONT_COLOR,
	FEM_NORMAL_BACK_COLOR,
	FEM_NONSTANDARD_ELEMENT_COLOR
};

// {81AA9FB1-2D24-4416-A52D-85B3E27CC873}
const MUID IID_IGlColorManager = 
{ 0x81aa9fb1, 0x2d24, 0x4416, { 0xa5, 0x2d, 0x85, 0xb3, 0xe2, 0x7c, 0xc8, 0x73 } };

comx_interface IGlColorManager : public IRoot
{
	virtual TStatus STDCALL SetColor(const COLOR_TYPE &color,const double &red, const double &green, const double &blue) = 0;
	virtual TStatus STDCALL GetColor(const COLOR_TYPE &color, double &red, double &green, double &blue) = 0;
};

// {FF4CA1F8-4F6A-4601-8982-0816A3A33D54}
const MUID CLSID_IGlColorManager = 
{ 0xff4ca1f8, 0x4f6a, 0x4601, { 0x89, 0x82, 0x8, 0x16, 0xa3, 0xa3, 0x3d, 0x54 } };

} // namespace comx
} // namespace Die_maker
} // namespace KMAS


#endif
