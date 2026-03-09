#ifndef COMX_DEBUG_110d226a9d1246f7a1c3a2070b88ab19_HXX
#define COMX_DEBUG_110d226a9d1246f7a1c3a2070b88ab19_HXX

#pragma warning(disable:4786)

#include <base/root.hxx>

#include <vector>
#include <string>
using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {f3630b37-0a78-4e92-950f-60c21f1e3e35}
const MUID IID_IComxDebug =
{0xf3630b37, 0x0a78, 0x4e92, {0x95, 0x0f, 0x60, 0xc2, 0x1f, 0x1e, 0x3e, 0x35 } };

interface IComxDebug : public IRoot
{
    virtual TStatus STDCALL GetComponentRefCount(unsigned int &ref_cnt) = 0;
    virtual TStatus STDCALL GetComponentAddress(unsigned int &address) = 0;
};

// {29067556-ae37-4c6a-9e78-7b7601aba326}
const MUID IID_IComxDebug2 =
{0x29067556, 0xae37, 0x4c6a, {0x9e, 0x78, 0x7b, 0x76, 0x01, 0xab, 0xa3, 0x26 } };

enum comx_debug_mode_enum_t
{
	COMX_DEBUG_INITIALIZE = 0,
	COMX_DEBUG_EXITIALIZE = 1
};

typedef int (*fn_comx_debug_cb_t)(int mode, unsigned int address);

interface IComxDebug2 : public IComxDebug
{
	virtual TStatus STDCALL SetupHook(fn_comx_debug_cb_t) = 0;	
};


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
