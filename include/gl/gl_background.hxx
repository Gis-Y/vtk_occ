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

#ifndef KMAS_COMX_SDK_GL_BACK_GROUND_H
#define KMAS_COMX_SDK_GL_BACK_GROUND_H

#include "base/root.hxx"

#include "gl/gl_sensor.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
// {2DDA9D89-A538-4337-8383-BAB3893204C5}
const MUID IID_IGlBackground = 
{ 0x2dda9d89, 0xa538, 0x4337, { 0x83, 0x83, 0xba, 0xb3, 0x89, 0x32, 0x4, 0xc5 } };

comx_interface IGlBackground : public IRoot
{
	virtual TStatus STDCALL EnableBackground(bool flag = true) = 0;
	virtual TStatus STDCALL GetBackgroundState(bool &flag) = 0;
};

// {23F3F7C0-9EC4-4291-B756-6F8B26C49145}
const MUID IID_IGlBackground2 = 
{ 0x23f3f7c0, 0x9ec4, 0x4291, { 0xb7, 0x56, 0x6f, 0x8b, 0x26, 0xc4, 0x91, 0x45 } };

comx_interface IGlBackground2 : public IGlBackground
{
	virtual TStatus STDCALL SetGlContent(IGlSensorContent *&p_gl_sc) = 0;
};

// {45D3C10B-DA9E-4dfc-B4CE-77B421993C25}
const MUID CLSID_IGlBackground = 
{ 0x45d3c10b, 0xda9e, 0x4dfc, { 0xb4, 0xce, 0x77, 0xb4, 0x21, 0x99, 0x3c, 0x25 } };
	
}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
