/*HEAD factory HXX ONESTEP */
#ifndef _FACTORY_H
#define _FACTORY_H

//#pragma warning(disable:4786)

#include <base/root.hxx>

#include <vector>
#include <string>
#include <iostream>
using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {B28FEE75-D734-4248-A5EB-604A61A87B38}
const MUID IID_IFactory =
    {
        0xb28fee75, 0xd734, 0x4248, { 0xa5, 0xeb, 0x60, 0x4a, 0x61, 0xa8, 0x7b, 0x38 }
    };

comx_interface IFactory : public IRoot
{
    virtual TStatus STDCALL CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv) = 0;
    virtual TStatus STDCALL Register(CreateInstancePtr fpn, ServiceEntryPtr fpns) = 0;
    virtual TStatus STDCALL Dispose(void) = 0;
    virtual TStatus STDCALL Initialize() = 0;
    virtual TStatus STDCALL QueryService(MREFTYPEID rclsid,MREFIID riid,void **ppv) = 0;
};

// {849E46BC-B600-40f4-81C6-E051F84D87F5}
const MUID IID_IFactory2 = 
{ 0x849e46bc, 0xb600, 0x40f4, { 0x81, 0xc6, 0xe0, 0x51, 0xf8, 0x4d, 0x87, 0xf5 } };

comx_interface IFactory2 : public IFactory
{
    virtual TStatus STDCALL CreateInstanceEx(IRoot *pRootOuter, MREFTYPEID rclsid,MREFIID riid,void **ppv) = 0;
};

// {B4E0F997-B110-47a3-8C20-9A21034CE20B}
const MTYPEID CLSID_IFactory =
    {
        0xb4e0f997, 0xb110, 0x47a3, { 0x8c, 0x20, 0x9a, 0x21, 0x3, 0x4c, 0xe2, 0xb }
    };

//DECLARE_CREATE_INSTANCE()
extern "C" /*DLL_API*/ TStatus CreateFactory(MREFTYPEID rclsid,MREFIID riid,void **ppv);

class TFactory
{
    IFactory *i_factory;
    bool _is_ref;
    string _package_name;
public:
     enum TSolutionType
     {
          KDEBUG,
          KRELEASE
     };
public:
    inline TFactory();
public:
    inline TStatus Register(CreateInstancePtr fpn, ServiceEntryPtr fpns);
    inline TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv);
    inline TStatus Initialize();
    inline TStatus QueryService(MREFTYPEID rclsid,MREFIID riid,void **ppv);
    inline TStatus Dispose();
public:
    inline TStatus CreateInstanceEx(IRoot *pRootOuter,MREFTYPEID rclsid,MREFIID riid,void **ppv);
};

TFactory::TFactory() : i_factory(NULL)
{
    KMAS::Die_maker::comx::CreateFactory(CLSID_IFactory,IID_IFactory,(void**)&i_factory);
    assert(i_factory);
}

TStatus TFactory::Register(CreateInstancePtr fpn, ServiceEntryPtr fpns)
{
  return i_factory -> Register(fpn, fpns);
}

TStatus TFactory::Dispose()
{
  return i_factory->Dispose();
}

TStatus TFactory::CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
  return i_factory -> CreateInstance(rclsid , riid , ppv);
}

TStatus TFactory::CreateInstanceEx(IRoot *pRootOuter,MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
    IFactory2 *p_factory_2 = NULL;
    i_factory->QueryInterface(IID_IFactory2, (void**)&p_factory_2);
    
    if (p_factory_2)
    {
        p_factory_2->CreateInstanceEx(pRootOuter,rclsid,riid,ppv);
        p_factory_2->Release();

        return M_OK;
    }
    else
    {
        return M_FAIL;
    }
}

TStatus TFactory::Initialize()
{
    return i_factory -> Initialize();
}

TStatus TFactory::QueryService(MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
     return i_factory -> QueryService(rclsid , riid , ppv);
}

inline bool comx_register_component_to_gc(IRoot *pRoot)
{
    IGarbagCollect *p_comx_gc = NULL;
    pRoot->QueryInterface(IID_IGarbagCollect, (void**)&p_comx_gc);

    if (p_comx_gc != NULL)
    {
        p_comx_gc->Register();
        p_comx_gc->Release();

        return true;
    }
    else
    {
        return false;
    }
}

inline bool comx_exclude_component_from_gc(IRoot *pRoot)
{
    IGarbagCollect *p_comx_gc = NULL;
    pRoot->QueryInterface(IID_IGarbagCollect, (void**)&p_comx_gc);

    if (p_comx_gc != NULL)
    {
        p_comx_gc->Exclude();
        p_comx_gc->Release();

        return true;
    }
    else
    {
        return false;
    }
}

inline bool comx_collect_garbage(const MUID &clsid)
{
    TFactory factory;
    CREATE_COMX_OBJ(factory, clsid, IRoot, p_root);

    IGarbagCollect *p_comx_gc = NULL;
    p_root->QueryInterface(IID_IGarbagCollect, (void**)&p_comx_gc);

    bool ret = false;
    if (p_comx_gc != NULL)
    {
        p_comx_gc->Exclude();
        p_comx_gc->Collect();
        
        p_comx_gc->Release();
 
        ret = true;
    }

    p_root->Release();

    return ret;
}

inline bool comx_push_gc(const MUID &clsid)
{
    TFactory factory;
    CREATE_COMX_OBJ(factory, clsid, IRoot, p_root);

    IGarbagCollect *p_comx_gc = NULL;
    p_root->QueryInterface(IID_IGarbagCollect, (void**)&p_comx_gc);

    bool ret = false;
    if (p_comx_gc != NULL)
    {
        p_comx_gc->Exclude();
        p_comx_gc->PushGC();

        p_comx_gc->Release();

        ret = true;
    }

    p_root->Release();

    return ret;
}

inline bool comx_pop_gc(const MUID &clsid)
{
    TFactory factory;
    CREATE_COMX_OBJ(factory, clsid, IRoot, p_root);

    IGarbagCollect *p_comx_gc = NULL;
    p_root->QueryInterface(IID_IGarbagCollect, (void**)&p_comx_gc);

    bool ret = false;
    if (p_comx_gc != NULL)
    {
        p_comx_gc->Exclude();
        p_comx_gc->PopGC();

        p_comx_gc->Release();

        ret = true;
    }

    p_root->Release();

    return ret;
}

class TCOMXGarbageCollector
{
    const MUID &_clsid;
public:
    TCOMXGarbageCollector(const MUID &clsid) : _clsid(clsid){comx_push_gc(clsid);}
    ~TCOMXGarbageCollector(){comx_pop_gc(_clsid);}
};

template<typename T>
TStatus CreateSingletonInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
   *ppv = 0;

   if (rclsid == T::ServiceEntry())
     {
       if(T::s_pSingleton() == NULL)
	 T::s_pSingleton() = new T;

       T::s_pSingleton()->QueryInterface(riid,ppv);
       if(*ppv == 0)
	 {
	   delete T::s_pSingleton();
	   T::s_pSingleton() = NULL;
	 }
     }

   if (*ppv == 0)
     {
       return M_NOINTERFACE;
     }
   else
     {
       return M_OK;
     }
}

template<typename T>
TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
   *ppv = 0;

   if (rclsid == T::ServiceEntry())
     {
       T *obj = new T;
       obj->QueryInterface(riid,ppv);                                                
       if(*ppv == 0)                                                                 
	 delete obj;                                                               
     }

   if (*ppv == 0)
     {
       return M_NOINTERFACE;
     }
   else
     {
       return M_OK;
     }
}

template<typename T>
class TComxComponentRegister
{
public:
  TComxComponentRegister()
  {
    TFactory factory;
    factory.Register(CreateInstance<T>, NULL);
  }
};

template<typename T>
class TComxSingletonComponentRegister
{
public:
  TComxSingletonComponentRegister()
  {
    TFactory factory;
    factory.Register(CreateSingletonInstance<T>, T::ServiceEntry);
  }
};

template<typename T>
class TComxServiceComponentRegister
{
public:
  TComxServiceComponentRegister()
  {
    TFactory factory;
    factory.Register(CreateInstance<T>, T::ServiceEntry);
  }
};

/*
template<typename T>
class TComxSingletonServiceComponentRegister
{
public:
  TComxSingletonServiceComponentRegister()
  {
    TFactory factory;
    factory.Register(CreateSingletonInstance<T>, T::ServiceEntry);
  }
};
*/

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif /*_FACTORY_H*/
