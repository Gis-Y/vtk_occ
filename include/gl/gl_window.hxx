/*HEAD gl_window.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_WINDOW_HEAD_
#define _KMAS_COMX_GL_WINDOW_HEAD_

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TGlWindowAttachmentType
{
     ATTACHMENT_LEFT,
     ATTACHMENT_RIGHT,
     ATTACHMENT_TOP,
     ATTACHMENT_BOTTOM,
     ATTACHMENT_NONE
};

enum TGlWindowCSType
{
     WCS1,     //Window  Coordinate System.
     GCS,     //Opengl  Coordinate System.
     CCS,     //Client  Coordinate System.
     LCS,     //Logical Coordinate System.
     TCS      //Title   Coordinate System.
};

struct  TGlWindowClientGLRect
{
     double width;
     double height;
     double z_buffer;
};

// {675108CF-9832-4e76-A9EE-238506876FB5}
const MUID IID_IGlWindowRender = 
{ 0x675108cf, 0x9832, 0x4e76, { 0xa9, 0xee, 0x23, 0x85, 0x6, 0x87, 0x6f, 0xb5 } };

comx_interface IGlWindowRender : public IRoot
{
     virtual TStatus STDCALL WindowPaint(TGlWindowClientGLRect &rect, bool &bHandled) = 0;
     virtual TStatus STDCALL WindowMouse(TMouseEvent event, bool &bHandled) = 0;
     virtual TStatus STDCALL WindowInitialize() = 0;
};

// {380B222F-3F5F-49b8-8B11-B1EDC9ABBF1B}
const MUID IID_IGlWindowRenderNC = 
{ 0x380b222f, 0x3f5f, 0x49b8, { 0x8b, 0x11, 0xb1, 0xed, 0xc9, 0xab, 0xbf, 0x1b } };

comx_interface IGlWindowRenderNC : public IRoot
{
     virtual TStatus STDCALL WindowPaintNC(bool &bHandled) = 0;
     virtual TStatus STDCALL WindowMouseNC(TMouseEvent event, bool &bHandled) = 0;
     virtual TStatus STDCALL WindowInitializeNC() = 0;
};

// {33EF0C87-1ADE-45e0-98AF-193CE78E0CDC}
const MUID IID_IGlWindow = 
{ 0x33ef0c87, 0x1ade, 0x45e0, { 0x98, 0xaf, 0x19, 0x3c, 0xe7, 0x8e, 0xc, 0xdc } };

comx_interface IGlWindow : public IRoot
{
     virtual TStatus STDCALL SetRefFrame(const int &x, const int &y,
          const int &cx, const int &cy) = 0;
     virtual TStatus STDCALL SetLocalFrame(const double &lx, const double &ly,
          const double &lcx, const double &lcy) = 0;
     virtual TStatus STDCALL GetLocalFrameRect(double &l, double &r, double &t, double &b) = 0;
     virtual TStatus STDCALL GetRefFrame(int &x, int &y,
          int &cx, int &cy) = 0;
     virtual TStatus STDCALL SetAlpha(const double &alpha) = 0;
     virtual TStatus STDCALL GetAlpha(double &alpha) = 0;
     virtual TStatus STDCALL SetVisible(const bool &flag) = 0;
     virtual TStatus STDCALL EnableCaption(const bool flag = true) = 0;
     virtual TStatus STDCALL IsVisible(bool &flag) = 0;
     virtual TStatus STDCALL SetFocus(const bool &flag) = 0;
     virtual TStatus STDCALL IsFocus(bool &flag) = 0;
     virtual TStatus STDCALL SetAttachment(const TGlWindowAttachmentType &attach_type, const int &margin = 15) = 0;
     virtual TStatus STDCALL SetTitle(const char* title) = 0;
     virtual TStatus STDCALL Center() = 0;
     virtual TStatus STDCALL SetMargin(const int &ml, const int &mr, const int &mt, const int &mb) = 0;
};

// {0BAF31E1-3248-4c2b-929B-7E919323F0EF}
const MUID IID_IGlWindowTransform = 
{ 0xbaf31e1, 0x3248, 0x4c2b, { 0x92, 0x9b, 0x7e, 0x91, 0x93, 0x23, 0xf0, 0xef } };

comx_interface IGlWindowTransform : public IRoot
{
     virtual TStatus STDCALL Transform(const TGlWindowCSType &from_type, 
          const TGlWindowCSType &to_type, 
          const double &from_val_x,
          const double &from_val_y,
          double &to_val_x, 
          double &to_val_y) = 0;
     virtual TStatus STDCALL GetOuterZBuffer(double &z_buffer) = 0;
};

// {5DBB5167-E4E3-44c7-937A-7F83A3D6D380}
const MUID CLSID_IGlWindow = 
{ 0x5dbb5167, 0xe4e3, 0x44c7, { 0x93, 0x7a, 0x7f, 0x83, 0xa3, 0xd6, 0xd3, 0x80 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif