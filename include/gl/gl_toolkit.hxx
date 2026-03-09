/*HEAD gl_toolkit HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_GL_TOOLKIT_H_2010713124616
#define _KMAS_COMX_GL_TOOLKIT_H_2010713124616

#include "base/root.hxx"
#include <base/factory.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {51901D99-D268-4a05-A86E-13396D1DE0AE}
 const MUID IID_IGlToolkit = 
{ 0x51901d99, 0xd268, 0x4a05, { 0xa8, 0x6e, 0x13, 0x39, 0x6d, 0x1d, 0xe0, 0xae } };

comx_interface IGlToolkit : public IRoot
{
     virtual TStatus STDCALL GetLookupVector(double& vx, double& vy, double& vz) = 0;
     virtual TStatus STDCALL GetMouseNearPosition(const int wx, const int wy, double& mx, double& my, double& mz) = 0;
     virtual TStatus STDCALL GetMouseFarPosition(const int wx, const int wy, double& mx, double& my, double& mz) = 0;
};

// {DF6AA75A-938A-4398-8059-8BA5AF6DFC4F}
 const MTYPEID CLSID_IGlToolkit = 
{ 0xdf6aa75a, 0x938a, 0x4398, { 0x80, 0x59, 0x8b, 0xa5, 0xaf, 0x6d, 0xfc, 0x4f } };


class TGLToolkit
{
	IGlToolkit *p_gl_toolkit;
public:
	TGLToolkit()
	{
		TFactory factory;
		QUERY_SERVICE(factory,
			CLSID_IGlToolkit,
			IGlToolkit,
			_p_gl_toolkit);

		assert(_p_gl_toolkit != NULL);
		p_gl_toolkit = _p_gl_toolkit;
	}
	~TGLToolkit()
	{
		p_gl_toolkit->Release();
		p_gl_toolkit = NULL;
	}
public:
	bool GetLookupVectorInModelSpace(double &vx, double &vy, double &vz)
	{
		return (p_gl_toolkit->GetLookupVector(vx, vy, vz) == M_OK);
	}
	bool GetMouseNearPositionInModelSpace(int wx, int wy, double &mx, double &my, double &mz)
	{
		return (p_gl_toolkit->GetMouseNearPosition(wx, wy, mx, my, mz) == M_OK);
	}
	bool GetMouseFarPositionInModelSpace(int wx, int wy, double &mx, double &my, double &mz)
	{
		return (p_gl_toolkit->GetMouseFarPosition(wx, wy, mx, my, mz) == M_OK);
	}
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

