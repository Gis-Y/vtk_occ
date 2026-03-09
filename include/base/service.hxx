/*HEAD service HXX ONESTEP */
#ifndef _KMAS_COMX_SERVICE_INTERFACE_DEFINE_H
#define _KMAS_COMX_SERVICE_INTERFACE_DEFINE_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

comx_interface IService : public IRoot{
     virtual TStatus STDCALL InitializeService() = 0;
     virtual TStatus STDCALL TerminalService() = 0;
};

// {3082B5B5-A962-4bfd-A761-400FC23A9D47}
const MUID IID_IService = 
{ 0x3082b5b5, 0xa962, 0x4bfd, { 0xa7, 0x61, 0x40, 0xf, 0xc2, 0x3a, 0x9d, 0x47 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS
#endif
