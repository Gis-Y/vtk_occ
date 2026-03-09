/*HEAD gl_legend.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_LEGEND_HEAD
#define _KMAS_COMX_GL_LEGEND_HEAD

#include <base/root.hxx>
#include "gl/gl_base.hxx"
#include "gl/gl_window.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {16F3D889-58E4-4fe5-A04C-30FDC1C6C9FB}
const MUID IID_IGlLegend = 
{ 0x16f3d889, 0x58e4, 0x4fe5, { 0xa0, 0x4c, 0x30, 0xfd, 0xc1, 0xc6, 0xc9, 0xfb } };

comx_interface IGlLegend : public IRoot
{
     virtual TStatus STDCALL SetVisible(const bool &flag = true) = 0;
     virtual TStatus STDCALL GetVisible(bool &flag) = 0;
     virtual TStatus STDCALL SetAttachment(const TGlWindowAttachmentType &) = 0;
     virtual TStatus STDCALL SetTitle(const char *title) = 0;
     virtual TStatus STDCALL SetUnit(const char *unit) = 0;
     virtual TStatus STDCALL SetExtremum(const double &max_value, const double &min_value) = 0;
     virtual TStatus STDCALL SetGradeNum(const int &grade = 512) = 0;
     virtual TStatus STDCALL GetColor(const double &value, double &red, double &green, double &blue) = 0;
     virtual TStatus STDCALL GetColors(const double *values, double *colors, const int &clen) = 0;
     virtual TStatus STDCALL Inverse(const bool &flag = true) = 0;
};

// {B38920C5-9554-4c6b-B922-1609839C0EDF}
const MUID IID_IGlLegend2 = 
{ 0xb38920c5, 0x9554, 0x4c6b, { 0xb9, 0x22, 0x16, 0x9, 0x83, 0x9c, 0xe, 0xdf } };

comx_interface IGlLegend2 : IGlLegend
{
  virtual TStatus STDCALL EnableCaptureMode(bool flag = true) = 0;
  virtual TStatus STDCALL Dispose() = 0;
};

// {21513CC8-4AFB-4f0a-B1C7-88057BDBA4FA}
const MUID CLSID_IGlLegend = 
{ 0x21513cc8, 0x4afb, 0x4f0a, { 0xb1, 0xc7, 0x88, 0x5, 0x7b, 0xdb, 0xa4, 0xfa } };
 
}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
