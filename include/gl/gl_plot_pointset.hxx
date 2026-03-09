/*HEAD gl_plot_pointset HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_GL_PLOT_POINTSET_H_20074284611
#define _KMAS_COMX_GL_PLOT_POINTSET_H_20074284611

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {CC91497B-3E0C-4c28-92F2-A30ED5680B3B}
 const MUID IID_IGlPlotPointset = 
{ 0xcc91497b, 0x3e0c, 0x4c28, { 0x92, 0xf2, 0xa3, 0xe, 0xd5, 0x68, 0xb, 0x3b } };

interface IGlPlotPointset : public IRoot
{
     virtual TStatus STDCALL AddPoint(const double x, const double y) = 0;
     virtual TStatus STDCALL Clear() = 0;
     virtual TStatus STDCALL GetPoints(double*& points, int& num) = 0;
};

// {A09E4B3B-32B9-4f9f-A1FB-30462973183F}
 const MUID IID_IGlPlotPointsetColorRule = 
{ 0xa09e4b3b, 0x32b9, 0x4f9f, { 0xa1, 0xfb, 0x30, 0x46, 0x29, 0x73, 0x18, 0x3f } };

interface IGlPlotPointsetColorRule : public IRoot
{
     virtual TStatus STDCALL GetColor(double *pts, double *clrs, const int num) = 0;
};

// {70E6AF32-A73C-4d08-8DE8-B236782ECAB1}
 const MTYPEID CLSID_IGlPlotPointset = 
{ 0x70e6af32, 0xa73c, 0x4d08, { 0x8d, 0xe8, 0xb2, 0x36, 0x78, 0x2e, 0xca, 0xb1 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

