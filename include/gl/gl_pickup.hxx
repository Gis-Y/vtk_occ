/*HEAD gl_pickup.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_PICKUP_HXX
#define _KMAS_COMX_GL_PICKUP_HXX

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TPickupType{
	POINT_PICKUP,
	RECT_PICKUP,
	NIL_PICKUP
};

// {9B4E4A49-7B87-4e57-A9BE-96048D370708}
const MUID IID_IGlPickupToolkit = 
{ 0x9b4e4a49, 0x7b87, 0x4e57, { 0xa9, 0xbe, 0x96, 0x4, 0x8d, 0x37, 0x7, 0x8 } };

comx_interface IGlPickupToolkit : public IRoot{
	virtual TStatus STDCALL Activate(const TPickupType &type) = 0;
};

// {E79DB4E3-6FD7-4112-9D7F-D897BE79EDF5}
const MUID IID_IGlPickupCustomer = 
{ 0xe79db4e3, 0x6fd7, 0x4112, { 0x9d, 0x7f, 0xd8, 0x97, 0xbe, 0x79, 0xed, 0xf5 } };

comx_interface IGlPickupCustomer : public IRoot{
	virtual TStatus STDCALL InitializePickup() = 0;
	virtual TStatus STDCALL RenderPickup() = 0;
	virtual TStatus STDCALL ProcessHits(const GLint &hits,GLuint *buf,const bool &inverse) = 0;
	virtual TStatus STDCALL GetSelectBufferSize(int &size) = 0;
};

// {46BBF651-2D3B-441e-949F-5CC82A8B2167}
const MUID IID_IGlPickupCustomer2 = 
{ 0x46bbf651, 0x2d3b, 0x441e, { 0x94, 0x9f, 0x5c, 0xc8, 0x2a, 0x8b, 0x21, 0x67 } };

comx_interface IGlPickupCustomer2 : public IGlPickupCustomer
{
	virtual TStatus STDCALL SetPickupMousePosition(int wx, int wy) = 0; 
};

// {33B3CF0B-1EDC-44f4-A98C-E4F33B257915}
const MUID IID_IGlPickupToolkit2 = 
{ 0x33b3cf0b, 0x1edc, 0x44f4, { 0xa9, 0x8c, 0xe4, 0xf3, 0x3b, 0x25, 0x79, 0x15 } };

comx_interface IGlPickupToolkit2 : public IGlPickupToolkit
{
	virtual TStatus STDCALL EnableAutoPickup(const bool flag = true) = 0;
};

// {27752483-C40D-40cb-89E8-51DDF10DBFAC}
const MUID CLSID_IGlPickup = 
{ 0x27752483, 0xc40d, 0x40cb, { 0x89, 0xe8, 0x51, 0xdd, 0xf1, 0xd, 0xbf, 0xac } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif