/*HEAD gl_common_display.hxx COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change
2006_12_22		ykk				create
2007_01_12		ykk				modified_finsh
2007_01_16		ykk				modify_interface

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_COMMON_DISPLAY_H_200612191147
#define _KMAS_COMX_COMMON_DISPLAY_H_200612191147

#include "base/root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum  PAINT_TYPE
{
	FILL,		//input element id
	FRAME,		//input element id
	NODE,		//input node id
	FILL_FRAME, //input element id
};

enum  GRADE_TYPE
{
	CONTINUOUS,
	DISCRETE,
};
enum  COLOR_PALATTE_TYPE
{
	AUTO,
	MANUAL,
};

struct COLOR_VALUE
{
public:
	COLOR_VALUE( int r = 255, int g = 0, int b = 0) :
	  red(r/255.0), green(g/255.0), blue(b/255.0)
	{
	}
public:
	double red;
	double green;
	double blue;
};

// {EBB333BF-9CCC-421a-BBF7-22C1665C5409}
const MUID IID_IGlCommonDisplay = 
{ 0xebb333bf, 0x9ccc, 0x421a, { 0xbb, 0xf7, 0x22, 0xc1, 0x66, 0x5c, 0x54, 0x9 } };

comx_interface IGlCommonDisplay : public IRoot
{
	/*	use note:
		continue display of kmas post set: AUTO + CONTINUOUS + PAINT_TYPE
		discrete display of kmas post set: AUTO + DISCRETE + PAINT_TYPE
		check result display t set: MANUAL + PAINT_TYPE  	*/

	 virtual TStatus STDCALL SetColorPalatteMode(const COLOR_PALATTE_TYPE &color_type) = 0;
	 virtual TStatus STDCALL GetColorPalatteMode(COLOR_PALATTE_TYPE &color_type) = 0; 

	 virtual TStatus STDCALL SetGradeMode(const GRADE_TYPE &grade_type) = 0;
	 virtual TStatus STDCALL GetGradeMode(GRADE_TYPE &grade_type) = 0;
	 
	 virtual TStatus STDCALL SetPaintMode(const PAINT_TYPE &paint_type) = 0;
	 virtual TStatus STDCALL GetPaintMode(PAINT_TYPE &paint_type) = 0;

	 virtual TStatus STDCALL EnableDisplayALL(const bool enable_display = true) = 0; 
	 virtual TStatus STDCALL Reset() = 0;

	 virtual TStatus STDCALL SetPosGradeNum(const int grade_num = 16) = 0;	
	 virtual TStatus STDCALL GetGradeNum( int &grade_num) = 0;

	 virtual TStatus STDCALL SetColorPlaneVisible( const bool display_colorplane) = 0;
	 
	  virtual TStatus STDCALL SetGradeCheckName(const char* grade_name) = 0;

	 virtual TStatus STDCALL SetPosExtremum(const double max_p, const double min_p) = 0;
	 virtual TStatus STDCALL SetDisplayControl(const int grade_sn, const int display) = 0; // grade_sn start from 0

	 virtual TStatus STDCALL SetPostDisplayContent(const char* para_name, const char* units,  const double* parameter, const int array_len ) = 0;
	 virtual TStatus STDCALL AddGradeDisplayItem(const char* message,const int* item_id, const int num ,const double* color/*RGB*/ ) = 0;
	 virtual TStatus STDCALL AddGradeDisplayItem(const char* message,const int* item_id, const int num ,const COLOR_VALUE &color_value /*RDB(255,255,255)*/ ) = 0;
};

// {DD21343F-8FA5-4296-88B4-C4DE65ED65EF}
const MTYPEID CLSID_IGlCommonDisplay = 
{ 0xdd21343f, 0x8fa5, 0x4296, { 0x88, 0xb4, 0xc4, 0xde, 0x65, 0xed, 0x65, 0xef } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

