/*HEAD gl_sensor_hook HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_GL_SENSOR_HOOK_H_201011981738
#define _KMAS_COMX_GL_SENSOR_HOOK_H_201011981738

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {BBABCFA2-9D23-46e0-AB86-99C0A3895018}
 const MUID IID_IGlSensorHook = 
{ 0xbbabcfa2, 0x9d23, 0x46e0, { 0xab, 0x86, 0x99, 0xc0, 0xa3, 0x89, 0x50, 0x18 } };

interface IGlSensorHook : public IRoot
{
};

// {8E167591-3FDC-4dc9-AAA6-E1788E785424}
 const MTYPEID CLSID_IGlSensorHook = 
{ 0x8e167591, 0x3fdc, 0x4dc9, { 0xaa, 0xa6, 0xe1, 0x78, 0x8e, 0x78, 0x54, 0x24 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

