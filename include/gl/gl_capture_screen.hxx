/*HEAD gl_capture_screen HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_GL_CAPTURE_SCREEN_H_2008563637
#define _KMAS_COMX_GL_CAPTURE_SCREEN_H_2008563637

#include "base/root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum ImageType
{
     KMAS_IMAGE_JPEG		=		0,
     KMAS_IMAGE_BITMAP		=		1,
     KMAS_IMAGE_PNG			=		2
};

// {113419C3-9807-4ffe-95E2-F3DF670FDAA3}
 const MUID IID_IGlCaptureScreen = 
{ 0x113419c3, 0x9807, 0x4ffe, { 0x95, 0xe2, 0xf3, 0xdf, 0x67, 0xf, 0xda, 0xa3 } };

comx_interface IGlCaptureScreen : public IRoot
{
     virtual TStatus STDCALL CaptureScreen(const char* fname, ImageType type = KMAS_IMAGE_BITMAP) = 0;
	 virtual TStatus STDCALL GetBackgroundState(bool &flag) = 0;
};

// {A262146E-A261-4cca-8D68-F2081E3F3B31}
 const MTYPEID CLSID_IGlCaptureScreen = 
{ 0xa262146e, 0xa261, 0x4cca, { 0x8d, 0x68, 0xf2, 0x8, 0x1e, 0x3f, 0x3b, 0x31 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

