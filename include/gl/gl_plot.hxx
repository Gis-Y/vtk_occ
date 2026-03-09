/*HEAD gl_plot.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_GL_PLOT_HEAD_HXX
#define _KMAS_COMX_GL_PLOT_HEAD_HXX

#define CLOSE_ACIS_ASSOCIATED_INTERFACE 

#include "base/root.hxx"
#include "base/type.hxx"
//#include <lists.hxx>
#ifndef CLOSE_ACIS_ASSOCIATED_INTERFACE 
#include <body.hxx>
#endif
#include "gl/gl_base.hxx"
#include "gl/gl_window.hxx"
#include "gl/gl_plot_shape.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {D71299A5-2B83-4094-99A2-8CC1DD4EB82A}
const MUID IID_IGlPlot = 
{ 0xd71299a5, 0x2b83, 0x4094, { 0x99, 0xa2, 0x8c, 0xc1, 0xdd, 0x4e, 0xb8, 0x2a } };

comx_interface IGlPlot : public IRoot
{
    virtual TStatus STDCALL Create(const int x, const int y, const int cx, const int cy) = 0;
    virtual TStatus STDCALL SetFrame(const double ox, const double oy, const double rx, const double ry) = 0;
    virtual TStatus STDCALL EnableCaptureLine(const bool &flag) = 0;
    virtual TStatus STDCALL EnableScaleLine(const bool &flag) = 0;
    virtual TStatus STDCALL SetTitle(const char* xtitle, const char* ytitle, const char *title) = 0;
    virtual TStatus STDCALL SetUnit(const double ux, const double uy) = 0;
    virtual TStatus STDCALL SetVisible(const bool &flag) = 0;
    virtual TStatus STDCALL Center() = 0;
};

// {D4D6D1FC-3730-4b0d-8888-0675EEB07355}
const MUID IID_IGlPlotShapManager = 
{ 0xd4d6d1fc, 0x3730, 0x4b0d, { 0x88, 0x88, 0x6, 0x75, 0xee, 0xb0, 0x73, 0x55 } };

comx_interface IGlPlotShapManager : public IRoot
{
	virtual TStatus STDCALL AppendCurve(const char*name,IGlPlotShape *ptr) = 0;
	virtual TStatus STDCALL DeleteCurve(const char*name) = 0;
    virtual TStatus STDCALL RenameCurve(const char*name,const char*new_name) = 0;
	virtual TStatus STDCALL GetCurve(const char*name, IGlPlotShape *&) = 0;
	virtual TStatus STDCALL GetCurveName(const int &index, char *) = 0;
    virtual TStatus STDCALL GetCurveCount(int &count) = 0;
	virtual TStatus STDCALL EnableInteractive(const bool &flag) = 0;
};

// {8667DE0C-B5A8-4cc7-BF2F-3589241550E5}
const MUID IID_ISectionCurvePlot = 
{ 0x8667de0c, 0xb5a8, 0x4cc7, { 0xbf, 0x2f, 0x35, 0x89, 0x24, 0x15, 0x50, 0xe5 } };

comx_interface ISectionCurvePlot : public IRoot
{
	virtual TStatus STDCALL CreateWnd(const int x, const int y, const int cx, const int cy) = 0;
	virtual TStatus STDCALL DestroyWnd() = 0;
};

// {79245244-CB4F-4e24-B52A-75FAA2782A08}
const MUID IID_ISectionCurvePlotManager = 
{ 0x79245244, 0xcb4f, 0x4e24, { 0xb5, 0x2a, 0x75, 0xfa, 0xa2, 0x78, 0x2a, 0x8 } };

comx_interface ISectionCurvePlotManager : public IRoot
{
	virtual TStatus STDCALL CreateLine(point_t &pt1, point_t &pt2,unsigned long &sid) = 0;
	virtual TStatus STDCALL CreateArc(point_t &start, point_t &end, 
		bool state,	bool state_init,const double& init_val, unsigned long &sid) = 0;
	virtual TStatus STDCALL CreateNurbs(point_t &start,point_t &end,
			  point_t &part,point_t &binder,unsigned long &sid) = 0;
	virtual TStatus STDCALL CreatePartCurve(double *cof,unsigned long &sid) = 0;
	virtual TStatus STDCALL CreateBinderCurve(int type,double *cof,unsigned long &sid) = 0;
	virtual TStatus STDCALL SetNurbsOperationState(int state) = 0;
};

#ifndef CLOSE_ACIS_ASSOCIATED_INTERFACE
// {464BB2A8-72C5-44bb-BEA1-CABDA404F130}
const MUID IID_ISectionCurveTemplate = 
{ 0x464bb2a8, 0x72c5, 0x44bb, { 0xbe, 0xa1, 0xca, 0xbd, 0xa4, 0x4, 0xf1, 0x30 } };

comx_interface ISectionCurveTemplate : public IRoot
{
	virtual TStatus STDCALL CreateTemplate(int template_id,int part_type,int binder_type,double *bnd) = 0;
	virtual TStatus STDCALL GetAcisBody(BODY *&BodyList) = 0;
};
#endif

// {FC7F1791-3F97-40be-824D-70E1E043A1EA}
const MUID IID_ISectionCurveConstraint = 
{ 0xfc7f1791, 0x3f97, 0x40be, { 0x82, 0x4d, 0x70, 0xe1, 0xe0, 0x43, 0xa1, 0xea } };

comx_interface ISectionCurveConstraint : public IRoot
{
	virtual TStatus STDCALL AddLineArcCons(unsigned long sid1,unsigned long sid2) = 0;
	virtual TStatus STDCALL AddArcLineCons(unsigned long sid1,unsigned long sid2) = 0;
	virtual TStatus STDCALL	AddLineLineCons(unsigned long sid1,unsigned long sid2) = 0;
};

// {EADEAA5E-67B8-48ae-AFCC-7B0D424893E8}
const MUID CLSID_IGlPlot = 
{ 0xeadeaa5e, 0x67b8, 0x48ae, { 0xaf, 0xcc, 0x7b, 0xd, 0x42, 0x48, 0x93, 0xe8 } };
 
}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
