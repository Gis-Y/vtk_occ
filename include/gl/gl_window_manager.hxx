/*HEAD gl_window_manager.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_WINDOW_MANAGER_HEAD_
#define _KMAS_COMX_GL_WINDOW_MANAGER_HEAD_

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {75863149-DC68-4dd3-BB77-E6C6A4037F21}
const MUID IID_IGlWindowManagerRender = 
{ 0x75863149, 0xdc68, 0x4dd3, { 0xbb, 0x77, 0xe6, 0xc6, 0xa4, 0x3, 0x7f, 0x21 } };

comx_interface IGlWindowManagerRender : public IRoot
{
    virtual TStatus STDCALL WmPaintGL(bool &bHandled) = 0;
	virtual TStatus STDCALL WmMouseGL(TMouseEvent event, bool &bHandled) = 0;
	virtual TStatus STDCALL WmInitializeGL() = 0;
};

// {C6BB2F7D-0791-4b26-A7BE-BBDD30A62A06}
const MUID IID_IGlWindowManager = 
{ 0xc6bb2f7d, 0x791, 0x4b26, { 0xa7, 0xbe, 0xbb, 0xdd, 0x30, 0xa6, 0x2a, 0x6 } };

comx_interface IGlWindowManager : public IRoot
{
     virtual TStatus STDCALL CreateGlWindow(const int &x, const int &y, 
          const int &cx, const int &cy,
          void **ppv) = 0;
     virtual TStatus STDCALL DestroyWindow(void **ppv) = 0;
     virtual TStatus STDCALL SetFocusWindow(void **ppv) = 0;
     virtual TStatus STDCALL GetFocusWindow(void **ppv) = 0;
     virtual TStatus STDCALL GetRootWindowRect(int &width, int &height, double &knear) = 0;
};

// {8361A477-2E26-4095-B020-06A4DE7067DF}
const MUID CLSID_IGlWindowManager = 
{ 0x8361a477, 0x2e26, 0x4095, { 0xb0, 0x20, 0x6, 0xa4, 0xde, 0x70, 0x67, 0xdf } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif