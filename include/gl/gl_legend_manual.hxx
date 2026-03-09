/*HEAD gl_legend_manual HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_GLLEGENDMANUAL_H_2007269041
#define _KMAS_COMX_GLLEGENDMANUAL_H_2007269041

#include <base/root.hxx>
#include <gl/gl_base.hxx>
#include <gl/gl_window.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

struct glLegendGradeItem
{
     char message[20];
     double red;
     double green;
     double blue;
};

// {E7B61EC8-E9DB-4244-9A65-ACB299CEFE1F}
const MUID IID_IGlLegendManual = 
{ 0xe7b61ec8, 0xe9db, 0x4244, { 0x9a, 0x65, 0xac, 0xb2, 0x99, 0xce, 0xfe, 0x1f } };

comx_interface IGlLegendManual : public IRoot
{
     virtual TStatus STDCALL SetVisible(const bool flag = true) = 0;
     virtual TStatus STDCALL GetVisible(bool& flag) = 0;
     virtual TStatus STDCALL SetTitle(const char* title) = 0;
	 virtual TStatus STDCALL SetColorPlaneMode(const int pMode = 0) = 0; //planeMode = 0 Êú£»planeMode = 1ºá£»
     //virtual TStatus STDCALL SetGradeNum(const int grade_num = 3) = 0;
     virtual TStatus STDCALL AddGradeItem(const glLegendGradeItem& gradeItem) = 0;
	 virtual TStatus STDCALL ClearItems() = 0;
     virtual TStatus STDCALL SetAttachment(const TGlWindowAttachmentType & t) = 0;
};

// {F3EE57B4-2EA2-4471-A759-44383C8665DD}
const MTYPEID CLSID_IGlLegendManual = 
{ 0xf3ee57b4, 0x2ea2, 0x4471, { 0xa7, 0x59, 0x44, 0x38, 0x3c, 0x86, 0x65, 0xdd } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

