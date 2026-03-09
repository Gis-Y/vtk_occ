/*HEAD gl_plot_shape.hxx COMX_COMPONENT_MODULE */
/*==============================================================================

  Copyright 2006 Jilin Kingmesh Corp.
  All rights reserved
  
	================================================================================ 
	File description:
	
	-- Please append file description informations here --
	  
	================================================================================
	Date            Name            Description of Change
	2007/03/02      Zhang Xiangkui  Created
        2008/12/32      Zhang Xiangkui  Adjust the indent format of source codes.
  
	$HISTORY$
	================================================================================
*/
#ifndef _KMAS_COMX_GL_PLOT_SHAPE_HEAD_HXX
#define _KMAS_COMX_GL_PLOT_SHAPE_HEAD_HXX

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {430209ED-5E49-4f38-A30B-705D5C55927C}
const MUID IID_IGlPlotShape = 
{ 0x430209ed, 0x5e49, 0x4f38, { 0xa3, 0xb, 0x70, 0x5d, 0x5c, 0x55, 0x92, 0x7c } };

comx_interface IGlPlotShape : public IRoot
{
	virtual TStatus STDCALL GetType(char *type) = 0;
	virtual TStatus STDCALL EnableInteraction(const bool &flag) = 0;
	virtual TStatus STDCALL SetColor(
		const double &red, 
		const double &green, 
		const double &blue) = 0;
	virtual TStatus STDCALL SetCtlPointColor(
		const double &red, 
		const double &green, 
		const double &blue) = 0;
	virtual TStatus STDCALL SetHighLightColor(
		const double &red, 
		const double &green, 
		const double &blue) = 0;
	virtual TStatus STDCALL SetCentriodColor(
		const double &red, 
		const double &green, 
		const double &blue) = 0;
	virtual TStatus STDCALL SetSize(const double &width) = 0;
	virtual TStatus STDCALL SetCtlPointSize(const double &size) = 0;
	virtual TStatus STDCALL SetLineStyle(
		const GLint &factor, 
		const GLushort &pattern) = 0;
	virtual TStatus STDCALL SetCtlLineStyle(
		const GLint &factor, 
		const GLushort &pattern) = 0;
	virtual TStatus STDCALL SetSmooth(const bool &flag) = 0;
};

// {9E245B1D-E497-44ed-832E-42D95DDD64E7}
const MUID IID_IGlPlotShapeRender = 
{ 0x9e245b1d, 0xe497, 0x44ed, { 0x83, 0x2e, 0x42, 0xd9, 0x5d, 0xdd, 0x64, 0xe7 } };

comx_interface IGlPlotShapeRender : public IRoot
{
     virtual TStatus STDCALL Paint() = 0;
	 virtual TStatus STDCALL PaintInteractive() = 0;
     virtual TStatus STDCALL Mouse(const double &x, const double &y) = 0;
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
