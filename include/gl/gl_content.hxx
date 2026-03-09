/*HEAD gl_content.hxx COMX_COMPONENT_MODULE */
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

#ifndef _GL_CONTENT_H
#define _GL_CONTENT_H

#include "base/root.hxx"
#include "gl/gl_base.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {ED487CE0-13DF-453a-B249-F6B4A843F098}
const MUID IID_IGlContentRender = 
{ 0xed487ce0, 0x13df, 0x453a, { 0xb2, 0x49, 0xf6, 0xb4, 0xa8, 0x43, 0xf0, 0x98 } };

comx_interface IGlContentRender : public IRoot{
	virtual TStatus STDCALL PaintGL() = 0;
	virtual TStatus STDCALL MouseGL(TMouseEvent event) = 0;
	virtual TStatus STDCALL InitializeGL() = 0;
};

// {9FBF76AD-0594-4294-A288-0E2122EE138F}
const MUID IID_IGlContentRenderEx =
{ 0x9fbf76ad, 0x594, 0x4294, { 0xa2, 0x88, 0xe, 0x21, 0x22, 0xee, 0x13, 0x8f } };

comx_interface IGlContentRenderEx : public IGlContentRender{
    virtual TStatus STDCALL KeyGL(TKeyEvent event) = 0;
};

enum TGlFontSize
{
     GF_SMALL,
     GF_MEDIUM,
     GF_LARGE,
     GF_HUGE
};

struct TGlTextInfo
{
     TGlFontSize eSize;
     float fX, fY, fZ;
     char szText[256];
};

struct TGlTextInfoEx
{
    int nFontSize;
    float fX, fY, fZ;
    char szText[256];
};

// {9ACD9A79-A328-4159-8B20-DB3973F00112}
const MUID IID_IGlText = 
{ 0x9acd9a79, 0xa328, 0x4159, { 0x8b, 0x20, 0xdb, 0x39, 0x73, 0xf0, 0x1, 0x12 } };

comx_interface IGlText : public IRoot
{
     virtual TStatus STDCALL TextOut(TGlTextInfo *ti, const int &rows) = 0;
};

// {541A2595-01F1-4EB6-8411-B8BC5C057595}
 const MUID IID_IGlTextEx =
{ 0x541a2595, 0x1f1, 0x4eb6, { 0x84, 0x11, 0xb8, 0xbc, 0x5c, 0x5, 0x75, 0x95 } };

comx_interface IGlTextEx : public IGlText
{
     virtual TStatus STDCALL TextOutEx(TGlTextInfoEx *ti, const int &rows, const char* szFont) = 0;
     virtual TStatus STDCALL GetTextWidth(const char *szText, const int fontSize, const char *szFont, int &width) = 0;
};

// {F2C281C8-4935-44f7-9DF1-01FFC1B5DC7C}
const MUID IID_IGlMouseHook = 
{ 0xf2c281c8, 0x4935, 0x44f7, { 0x9d, 0xf1, 0x1, 0xff, 0xc1, 0xb5, 0xdc, 0x7c } };

comx_interface IGlMouseHook : public IRoot
{
	virtual TStatus STDCALL MouseGL(TMouseEvent event, bool &is_break) = 0;
};

// {2C762A80-827E-48b2-92C3-45D7CEF3800D}
const MUID IID_IGlMouseHook2 = 
{ 0x2c762a80, 0x827e, 0x48b2, { 0x92, 0xc3, 0x45, 0xd7, 0xce, 0xf3, 0x80, 0xd } };

comx_interface IGlMouseHook2 : public IGlMouseHook
{
	virtual TStatus STDCALL SetContent(IRoot *p_gl_content, IRoot *p_gl_sensor) = 0;
};

// {685C85F9-BAA9-4d25-93C6-AF5FFDE3E305}
const MUID IID_IGlMouseHookManager = 
{ 0x685c85f9, 0xbaa9, 0x4d25, { 0x93, 0xc6, 0xaf, 0x5f, 0xfd, 0xe3, 0xe3, 0x5 } };

comx_interface IGlMouseHookManager : public IRoot
{
	virtual TStatus STDCALL RegisterHook(IGlMouseHook *hook) = 0;
	virtual TStatus STDCALL UnRegisterHook() = 0;
};

// {F8FFFDAE-9328-4cc6-84E4-8753DE1CD9FA}
const MUID IID_IGlMouseHookManager2 = 
{ 0xf8fffdae, 0x9328, 0x4cc6, { 0x84, 0xe4, 0x87, 0x53, 0xde, 0x1c, 0xd9, 0xfa } };

comx_interface IGlMouseHookManager2 : public IGlMouseHookManager
{
	virtual TStatus STDCALL ShieldContentMenu(bool flag) = 0; 
	virtual TStatus STDCALL IsContentMenuShielded(bool &flag) = 0;
};

// {D7E4C94A-326A-4718-88A8-732851F8ED6E}
const MUID CLSID_IGlContent = 
{ 0xd7e4c94a, 0x326a, 0x4718, { 0x88, 0xa8, 0x73, 0x28, 0x51, 0xf8, 0xed, 0x6e } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
