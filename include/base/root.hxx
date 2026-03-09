/*HEAD root HXX ONESTEP */
#ifndef _CUSTOM_IROOT_H
#define _CUSTOM_IROOT_H

//#pragma warning(disable:4227)

#define ONESTEP_INTEGERATE_TO_UG_NX

#include <memory.h>
#include <assert.h>
#include <iostream>
//#include <windows.h>

#include <vector>
using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
#ifndef comx_interface
#define comx_interface struct
#endif /*comx_interface*/

#ifndef TStatus
#define TStatus int
#endif /*TStatus*/

#ifndef M_OK
#define M_OK 0
#endif /*M_OK*/

#ifndef M_NOINTERFACE
#define M_NOINTERFACE -1
#endif /*M_NOINTERFACE*/

#ifndef M_FAIL
#define M_FAIL -2
#endif

#ifndef M_NOEVENT
#define M_NOEVENT -3
#endif

#ifndef M_ADVISELIMIT_EVENT
#define M_ADVISELIMIT_EVENT -4
#endif

#ifndef M_NO_SERVICE_SUPPORT_ENTRY
#define M_NO_SERVICE_SUPPORT_ENTRY -5
#endif

#ifndef M_NO_SERVICE_INTERFACE
#define M_NO_SERVICE_INTERFACE - 6
#endif

#ifndef M_AMBIGUOUS_EVENT
#define M_AMBIGUOUS_EVENT -7
#endif

#ifndef M_INVALID_ARG
#define M_INVALID_ARG -8
#endif

#ifndef STDCALL
#ifdef ONESTEP_INTEGERATE_TO_UG_NX
#define STDCALL /*_stdcall*/
#else
#define STDCALL _stdcall
#endif
#endif /*STDCALL*/

#ifndef MUID_DEFINED
#define MUID_DEFINED

typedef struct _MUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} MUID;

typedef MUID MIID;
typedef MUID MTYPEID; 
#define MREFIID  const MIID & /*const*/
#define MREFTYPEID const MTYPEID & /*const*/
#define MREFEID  const MUID &

  //#define IsEqualMUID(rguid1, rguid2) (memcmp(rguid1, rguid2, sizeof(MUID)) == 0)

  inline string muid2str(MUID cid)
  {
    char szBuf[256];
    sprintf_s(szBuf, "%x%x%x%x%x%x%x%x%x%x%x",
	   (unsigned int)cid.Data1,
	   (unsigned int)cid.Data2,
	   (unsigned int)cid.Data3, 
	   (unsigned int)cid.Data4[0],
	   (unsigned int)cid.Data4[1],
	   (unsigned int)cid.Data4[2],
	   (unsigned int)cid.Data4[3],
	   (unsigned int)cid.Data4[4],
	   (unsigned int)cid.Data4[5],
	   (unsigned int)cid.Data4[6],
	   (unsigned int)cid.Data4[7]);

    return szBuf;
  }

inline bool operator==(const MIID &left,const MIID &right)
{
  //return IsEqualMUID(&left,&right);

  return muid2str(left) == muid2str(right);
}

#endif /* MUID_DEFINED */

template<typename I , typename F>
inline F for_each_index(I first , I last , F f)
{
     for(;first != last ; ++first)
          f(first);

     return (f);
}

/*
 *    Macro BEGIN_IMPLEMENT_ROOT define
 */

#define BEGIN_IMPLEMENT_NO_COUNT_ROOT()                               \
virtual TStatus STDCALL AddRef(void)                                  \
{                                                                     \
    return 2;                                                         \
}                                                                     \
virtual TStatus STDCALL Release(void)                                 \
{                                                                     \
    return 1;                                                         \
}                                                                     \
virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv)      \
{

#define BEGIN_IMPLEMENT_ROOT()                                        \
virtual TStatus STDCALL AddRef(void)                                  \
{                                                                     \
    return ++m_cRef;                                                  \
}                                                                     \
virtual TStatus STDCALL Release(void)                                 \
{                                                                     \
    long res = --m_cRef;                                              \
    if (res == 0) {                                                   \
        delete this;                                                  \
    }                                                                 \
    return res;                                                       \
}                                                                     \
virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv)      \
{

/*
 *    Macro IMPLEMENT_INTERFACE define
 */

#define IMPLEMENT_INTERFACE(Itf)                                      \
    if (riid == IID_##Itf) {                                          \
        *ppv = static_cast<Itf*>(this);                               \
    }                                      

#define IMPLEMENT_EVENT_SOURCE_INTERFACE(Itf)                         \
    if (riid == IID_IEventSourceContainer) {                          \
        *ppv = static_cast<IEventSourceContainer*>(this);             \
    }                                      

#define IMPLEMENT_EVENT_SOURCE_INTERFACE_EX(Itf)                      \
    if (riid == IID_IEventSourceContainer) {                          \
        *ppv = static_cast<IEventSourceContainer*>(this);             \
    }                                                                 \
                                                                      \
    if (riid == IID_IEventSourceContainerEx) {                        \
        *ppv = static_cast<IEventSourceContainerEx*>(this);           \
    }                                      

/*
 *    Macro IMPLEMENT_INTERFACE_ROOT define
 */

#define IMPLEMENT_INTERFACE_ROOT(Itf)                                 \
    if (riid == IID_IRoot)                                            \
        *ppv = static_cast<Itf*>(this);                               \

/*
 *    Macro END_IMPLEMENT_ROOT define
 */

#define END_IMPLEMENT_ROOT()                                          \
    if(*ppv == 0)                                                     \
        return M_NOINTERFACE;                                         \
                                                                      \
    reinterpret_cast<IRoot*>(*ppv)->AddRef();                         \
    return M_OK;                                                      \
}                                                                     \
private:                                                              \
    long m_cRef;

#define INITIALIZE_REF_COUNT() m_cRef(0)

#define COMX_LOCK_REF_COUNT() ++m_cRef
#define COMX_UNLOCK_REF_COUNT() --m_cRef

/*
 *  Interface root define.
 *  1.IID_IRoot
 *  2.IRoot
 */

// {97159EFA-DD23-41a0-B7F2-1BC893380313}
const MIID IID_IRoot = 
{ 0x97159efa, 0xdd23, 0x41a0, { 0xb7, 0xf2, 0x1b, 0xc8, 0x93, 0x38, 0x3, 0x13 } };

class IRoot{
public:
     virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv) = 0;
     virtual TStatus STDCALL AddRef(void) = 0;
     virtual TStatus STDCALL Release(void) = 0;
};

// {C9540745-9C35-4b24-970D-D211B34962E8}
const MUID IID_IAggregation = 
{ 0xc9540745, 0x9c35, 0x4b24, { 0x97, 0xd, 0xd2, 0x11, 0xb3, 0x49, 0x62, 0xe8 } };

comx_interface IAggregation : public IRoot
{
     virtual TStatus STDCALL QueryAggregation(MREFIID riid, void **ppv) = 0;
};

// {FCE8C38A-C699-41ad-B4BB-D8B34FBAA8C3}
const MUID IID_IAggregationOwner = 
{ 0xfce8c38a, 0xc699, 0x41ad, { 0xb4, 0xbb, 0xd8, 0xb3, 0x4f, 0xba, 0xa8, 0xc3 } };

comx_interface IAggregationOwner : public IRoot
{
    virtual TStatus STDCALL InitOwnerResource() = 0;
};

//#define DLL_EXPORTS

#ifndef ONESTEP_INTEGERATE_TO_UG_NX
#ifdef DLL_EXPORTS
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport)
#endif
#else
    #define DLL_API 
#endif

#define COMX_COMPONENT_ENTRY(type, clsid)  \
    static const MUID ServiceEntry(){return clsid;} \
    static type *&s_pSingleton() \
    {\
      static type *singleton = NULL; \
      return singleton; \
    }

#define DECLARE_CREATE_INSTANCE()                                                 \
extern "C" {                                                                      \
DLL_API TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv);        \
}

#define BEGIN_IMPLEMENT_CREATE_INSTANCE()                                         \
extern "C" {                                                                      \
DLL_API TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv)         \
{                                                                                 \
    *ppv = 0;

#define IMPLEMENT_CREATE_INSTANCE(type,clsid)                                     \
if (rclsid == clsid) {                                                            \
    type *obj = new type;                                                         \
    obj->QueryInterface(riid,ppv);                                                \
    if(*ppv == 0)                                                                 \
        delete obj;                                                               \
}

#define END_IMPLEMENT_CREATE_INSTANCE()                                           \
if (*ppv == 0) {                                                                  \
    return M_NOINTERFACE;                                                        \
}                                                                                 \
else                                                                              \
{                                                                                 \
    return M_OK;                                                                  \
}                                                                                 \
}                                                                                 \
}   

#define DECLARE_CREATE_INSTANCE_SINGLETON(type)                                   \
    static type *s_pSingleton;                                                 

#define DELETE_SINGLETON_OBJ() s_pSingleton = NULL;

#define BEGIN_IMPLEMENT_CREATE_INSTANCE_SINGLETON(type)                           \
type *type::s_pSingleton = NULL;                                                  \
extern "C" {                                                                      \
DLL_API TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv)         \
{                                                                                 \
    *ppv = 0;

#define IMPLEMENT_CREATE_INSTANCE_SINGLETON(type,clsid)                           \
if (rclsid == clsid) {                                                            \
    if(type::s_pSingleton == NULL)                                                \
        type::s_pSingleton = new type;                                            \
    type::s_pSingleton->QueryInterface(riid,ppv);                                 \
    if(*ppv == 0)                                                                 \
    {                                                                             \
                                                                                  \
        delete type::s_pSingleton;                                                \
        type::s_pSingleton = NULL;                                                \
    }                                                                             \
}

#define END_IMPLEMENT_CREATE_INSTANCE_SINGLETON()                                 \
if (*ppv == 0) {                                                                  \
    return M_NOINTERFACE;                                                        \
}                                                                                 \
else                                                                              \
{                                                                                 \
    return M_OK;                                                                  \
}                                                                                 \
}                                                                                 \
}                                      

typedef TStatus (*CreateInstancePtr)(MREFTYPEID rclsid,MREFIID riid,void **ppv);
typedef TStatus (*CreateInstancePtrEx)(IRoot*,MREFTYPEID rclsid,MREFIID riid,void **ppv);
typedef const MUID (*ServiceEntryPtr)(); 

#ifndef ONESTEP_INTEGERATE_TO_UG_NX
#ifdef DLL_EXPORTS1
inline CreateInstancePtr GetComponentEntry(const char *szDll)
{
    //HINSTANCE h =  LoadLibraryEx(szDll, NULL,DONT_RESOLVE_DLL_REFERENCES);
    HINSTANCE h =  AfxLoadLibrary(szDll);
    CreateInstancePtr pfn = NULL;

    if(h)
        *(FARPROC*)&pfn = GetProcAddress(h,"CreateInstance");

    return pfn;
}
#else
inline CreateInstancePtr GetComponentEntry(const char *szDll)
{
    //HINSTANCE h =  LoadLibraryEx(szDll, NULL,DONT_RESOLVE_DLL_REFERENCES);
    HINSTANCE h =  LoadLibrary(szDll);
    CreateInstancePtr pfn = NULL;

    if(h)
        *(FARPROC*)&pfn = GetProcAddress(h,"CreateInstance");

    return pfn;
}
#endif
#endif

/*
 *    Define export interface.
 */

#ifndef COMX_DWORD_DECLARE
#define COMX_DWORD_DECLARE
#ifdef  _M_X64
typedef unsigned long long COMX_DWORD;
#else
typedef unsigned long COMX_DWORD;
#endif

#endif


template<typename T>
class TOutInterface : public vector<T*>{
public:
     inline void Advise(T *ptr_T , COMX_DWORD &dwCookie);
     inline bool Unadvise(COMX_DWORD &dwCookie);
};

template<typename T>
void TOutInterface<T>::Advise(T *ptr_T , COMX_DWORD &dwCookie)
{
     vector<T*>::push_back(ptr_T);
     dwCookie = reinterpret_cast<COMX_DWORD>(ptr_T);
}

template<typename T>
bool TOutInterface<T>::Unadvise(COMX_DWORD &dwCookie)
{
     typename vector<T*>::iterator iter;
     for(iter = this->begin() ; iter != this->end() ; ++iter)
     {
          T* ptr_T = *iter;
          if((reinterpret_cast<COMX_DWORD>(ptr_T)) == dwCookie)
          {
               break;
          }
     }
     if(iter != this->end())
     {
          (*iter) -> Release();
          vector<T*>::erase(iter);
          return true;
     }
     else
     {
          return false;
     }
}
// {E4F76EDD-6519-455e-B0B9-B81E1BE8AE65}
const MIID IID_IEventSourceContainer = 
{ 0xe4f76edd, 0x6519, 0x455e, { 0xb0, 0xb9, 0xb8, 0x1e, 0x1b, 0xe8, 0xae, 0x65 } };

comx_interface IEventSource;

comx_interface IEventSourceContainer : public IRoot
{
    virtual TStatus STDCALL FindEventSource(MREFIID riid, IEventSource **ppES) = 0;
};

// {787A9F58-4E52-49d2-B96F-F0308D988407}
const MUID IID_IEventSourceContainerEx = 
{ 0x787a9f58, 0x4e52, 0x49d2, { 0xb9, 0x6f, 0xf0, 0x30, 0x8d, 0x98, 0x84, 0x7 } };

comx_interface IEventSourceContainerEx : public IEventSourceContainer
{
    virtual TStatus STDCALL FindEventSourceEx(MREFEID reid, MREFIID riid, IEventSource **ppES) = 0;
};

// {5CB3C65D-F1DF-4436-8AFC-C2147D45B4D3}
const MIID IID_IEventSource = 
{ 0x5cb3c65d, 0xf1df, 0x4436, { 0x8a, 0xfc, 0xc2, 0x14, 0x7d, 0x45, 0xb4, 0xd3 } };

comx_interface IEventSource : public IRoot
{
    virtual TStatus STDCALL GetEventSourceInterface(MIID *pIID) = 0;
    virtual TStatus STDCALL GetEventSourceContainer(IEventSourceContainer **ppESC) = 0;
    virtual TStatus STDCALL Advise(IRoot *pRoot,COMX_DWORD *pwdCookie) = 0;
    virtual TStatus STDCALL Unadvise(COMX_DWORD dwCookie) = 0;
};

// {3C1EDF05-4D73-4287-B288-1F922C0821EE}
const MUID EID_Default = 
{ 0x3c1edf05, 0x4d73, 0x4287, { 0xb2, 0x88, 0x1f, 0x92, 0x2c, 0x8, 0x21, 0xee } };

/*
 *  Macro defines for export interface.
 */

#define INIT_EXPORT_INTERFACE_MULTI(name) m_xcp##name(this)// ,m_p##name(NULL)
#define INIT_EXPORT_INTERFACE_MULTI_EX(eid, name) m_xcp##eid##name(this)// ,m_p##name(NULL)
#define INIT_EXPORT_INTERFACE(name) m_p##name(NULL),m_xcp##name(this)
#define INIT_EXPORT_INTERFACE_EX(eid, name) m_p##eid##name(NULL),m_xcp##eid##name(this)

#define BEGIN_IMPLEMENT_EVENT_SOURCE_CONTAINER()                                       \
virtual TStatus STDCALL FindEventSource(MREFIID riid, IEventSource **ppES)             \
{                                                                                      \
    *ppES = 0;

#define BEGIN_IMPLEMENT_EVENT_SOURCE_CONTAINER_EX()            \
virtual TStatus STDCALL FindEventSource(MREFIID riid, IEventSource **ppES) \
{                                    \
    return FindEventSourceEx(EID_Default, riid, ppES);            \
}                                    \
virtual TStatus STDCALL FindEventSourceEx(MREFEID reid, MREFIID riid, IEventSource **ppES) \
{                                    \
    *ppES = 0;                                \
                                    \
    int cnt = 0;

#define IMPLEMENT_EVENT_SOURCE_CONTAINER(name)                                         \
    if (riid == IID_##name)                                                            \
    {                                                                                  \
        *ppES = &m_xcp##name;                                                          \
    }

#define IMPLEMENT_EVENT_SOURCE_CONTAINER_EX(eid, name)            \
    if ((reid == eid || reid == EID_Default) && riid == IID_##name)    \
    {                                    \
    *ppES = &m_xcp##eid##name;                    \
                                    \
    cnt++;                                \
    }

#define END_IMPLEMENT_EVENT_SOURCE_CONTAINER()                                         \
    if (*ppES == 0)                                                                    \
    {                                                                                  \
        return M_NOEVENT;                                                              \
    }                                                                                  \
                                                                                       \
    (*ppES)->AddRef();                                                                 \
                                                                                       \
    return M_OK;                                                                       \
}

#define END_IMPLEMENT_EVENT_SOURCE_CONTAINER_EX()                    \
    if (*ppES == 0)                            \
    {                                    \
    return M_NOEVENT;                        \
    }                                    \
                                    \
    (*ppES)->AddRef();                            \
                                    \
    if (cnt >= 2)                            \
    {                                    \
    return M_AMBIGUOUS_EVENT;                    \
    }                                    \
                                    \
    return M_OK;                            \
}

#define IMPLEMENT_EVENT_SOURCE(owner,name)                                             \
public:                                                                               \
    name *m_p##name;                                                                   \
public:                                                                                \
    class XCP##name : public IEventSource{                                             \
    private:                                                                           \
        owner *m_pParent;                                                              \
    public:                                                                            \
        XCP##name(owner *parent){m_pParent = parent;}                                  \
    public:                                                                            \
        virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv)               \
        {                                                                              \
            if (riid == IID_IRoot || riid == IID_IEventSource)                         \
            {                                                                          \
                *ppv = static_cast<IEventSource*>(this);                               \
            }                                                                          \
            else                                                                       \
            {                                                                          \
                return (*ppv = 0) , M_NOINTERFACE;                                     \
            }                                                                          \
                                                                                       \
            ((IRoot*)*ppv)->AddRef();                                                  \
                                                                                       \
            return M_OK;                                                               \
        }                                                                              \
        virtual TStatus STDCALL AddRef(void)                                           \
        {                                                                              \
            return m_pParent->AddRef();                                                \
        }                                                                              \
        virtual TStatus STDCALL Release(void)                                          \
        {                                                                              \
            return m_pParent->Release();                                               \
        }                                                                              \
    public:                                                                            \
        virtual TStatus STDCALL GetEventSourceInterface(MIID *pIID)                    \
        {                                                                              \
            assert(pIID);                                                              \
            *pIID = IID_##name;                                                        \
            return M_OK;                                                               \
        }                                                                              \
        virtual TStatus STDCALL GetEventSourceContainer(IEventSourceContainer **ppESC) \
        {                                                                              \
            assert(ppESC);                                                             \
            (*ppESC = m_pParent)->AddRef();                                            \
            return M_OK;                                                               \
        }                                                                              \
        virtual TStatus STDCALL Advise(IRoot *pRoot,COMX_DWORD *pwdCookie)                  \
        {                                                                              \
            assert(pwdCookie&&pRoot);                                                  \
            *pwdCookie = 0;                                                            \
                                                                                       \
            if (m_pParent->m_p##name)                                                  \
            {                                                                          \
                return M_ADVISELIMIT_EVENT;                                            \
            }                                                                          \
                                                                                       \
            TStatus status = pRoot->QueryInterface(IID_##name,                         \
                (void**)&(m_pParent->m_p##name));                                      \
                                                                                       \
            if (status == M_NOINTERFACE)                                               \
            {                                                                          \
                status = M_NOEVENT;                                                    \
            }                                                                          \
                                                                                       \
            if(status == M_OK)                                                         \
            {                                                                          \
                *pwdCookie = reinterpret_cast <COMX_DWORD>(m_pParent->m_p##name);                              \
            }                                                                          \
                                                                                       \
            return status;                                                             \
        }                                                                              \
        virtual TStatus STDCALL Unadvise(COMX_DWORD dwCookie)                               \
        {                                                                              \
            if (reinterpret_cast <COMX_DWORD>(m_pParent->m_p##name) != dwCookie)                               \
            {                                                                          \
                return M_NOEVENT;                                                      \
            }                                                                          \
                                                                                       \
            m_pParent->m_p##name->Release();                                           \
            m_pParent->m_p##name = NULL;                                               \
                                                                                       \
            return M_OK;                                                               \
        }                                                                              \
    }m_xcp##name;

#define IMPLEMENT_EVENT_SOURCE_EX(owner, eid, name)            \
public:                                                                \
    name *m_p##eid##name;                        \
public:                                    \
    class XCP##eid##name : public IEventSource{                \
    private:                                \
        owner *m_pParent;                        \
    public:                                \
        XCP##eid##name(owner *parent){m_pParent = parent;}        \
    public:                                \
        virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv) \
        {                                \
            if (riid == IID_IRoot || riid == IID_IEventSource)        \
            {                                \
                *ppv = static_cast<IEventSource*>(this);        \
            }                                \
            else                            \
            {                                \
                return (*ppv = 0) , M_NOINTERFACE;            \
            }                                \
                                    \
            ((IRoot*)*ppv)->AddRef();                    \
                                    \
            return M_OK;                        \
        }                                \
        virtual TStatus STDCALL AddRef(void)                \
        {                                \
            return m_pParent->AddRef();                    \
        }                                \
        virtual TStatus STDCALL Release(void)                \
        {                                \
            return m_pParent->Release();                \
        }                                \
    public:                                \
        virtual TStatus STDCALL GetEventSourceInterface(MIID *pIID)    \
        {                                \
            assert(pIID);                        \
            *pIID = IID_##name;                        \
            return M_OK;                        \
        }                                \
        virtual TStatus STDCALL GetEventSourceContainer(IEventSourceContainer **ppESC) \
        {                                \
            assert(ppESC);                        \
            (*ppESC = m_pParent)->AddRef();                \
            return M_OK;                        \
        }                                \
        virtual TStatus STDCALL Advise(IRoot *pRoot,COMX_DWORD *pwdCookie)    \
        {                                \
            assert(pwdCookie&&pRoot);                    \
            *pwdCookie = 0;                        \
                                    \
            if (m_pParent->m_p##eid##name)                \
            {                                \
                return M_ADVISELIMIT_EVENT;                \
            }                                \
                                    \
            TStatus status = pRoot->QueryInterface(IID_##name,        \
                (void**)&(m_pParent->m_p##eid##name));            \
                                    \
            if (status == M_NOINTERFACE)                \
            {                                \
                status = M_NOEVENT;                    \
            }                                \
                                    \
            if(status == M_OK)                        \
            {                                \
                *pwdCookie = COMX_DWORD(m_pParent->m_p##eid##name);        \
            }                                \
                                    \
            return status;                        \
        }                                \
        virtual TStatus STDCALL Unadvise(COMX_DWORD dwCookie)        \
        {                                \
            if (COMX_DWORD(m_pParent->m_p##eid##name) != dwCookie)        \
            {                                \
                return M_NOEVENT;                    \
            }                                \
                                    \
            m_pParent->m_p##eid##name->Release();            \
            m_pParent->m_p##eid##name = NULL;                \
                                    \
            return M_OK;                        \
        }                                \
    }m_xcp##eid##name;

#define IMPLEMENT_EVENT_SOURCE_MULTI(owner,name)                                       \
public:                                                                               \
    TOutInterface<name> m_p##name;                                                     \
public:                                                                                \
    class XCP##name : public IEventSource{                                             \
    private:                                                                           \
        owner *m_pParent;                                                              \
    public:                                                                            \
        XCP##name(owner *parent){m_pParent = parent;}                                  \
    public:                                                                            \
        virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv)               \
        {                                                                              \
            if (riid == IID_IRoot || riid == IID_IEventSource)                         \
            {                                                                          \
                *ppv = static_cast<IEventSource*>(this);                               \
            }                                                                          \
            else                                                                       \
            {                                                                          \
                return (*ppv = 0) , M_NOINTERFACE;                                     \
            }                                                                          \
                                                                                       \
            ((IRoot*)*ppv)->AddRef();                                                  \
                                                                                       \
            return M_OK;                                                               \
        }                                                                              \
        virtual TStatus STDCALL AddRef(void)                                           \
        {                                                                              \
            return m_pParent->AddRef();                                                \
        }                                                                              \
        virtual TStatus STDCALL Release(void)                                          \
        {                                                                              \
            return m_pParent->Release();                                               \
        }                                                                              \
    public:                                                                            \
        virtual TStatus STDCALL GetEventSourceInterface(MIID *pIID)                    \
        {                                                                              \
            assert(pIID);                                                              \
            *pIID = IID_##name;                                                        \
            return M_OK;                                                               \
        }                                                                              \
        virtual TStatus STDCALL GetEventSourceContainer(IEventSourceContainer **ppESC) \
        {                                                                              \
            assert(ppESC);                                                             \
            (*ppESC = m_pParent)->AddRef();                                            \
            return M_OK;                                                               \
        }                                                                              \
        virtual TStatus STDCALL Advise(IRoot *pRoot,COMX_DWORD *pwdCookie)                  \
        {                                                                              \
            assert(pwdCookie&&pRoot);                                                  \
            *pwdCookie = 0;                                                            \
                                                                                       \
            name *p_##name = NULL;                                                     \
            TStatus status = pRoot->QueryInterface(IID_##name,                         \
                (void**)&p_##name);                                                    \
                                                                                       \
                                                                                       \
            if (status == M_NOINTERFACE)                                               \
            {                                                                          \
                status = M_NOEVENT;                                                    \
            }                                                                          \
                                                                                       \
            if(status == M_OK)                                                         \
            {                                                                          \
                m_pParent->m_p##name.Advise(p_##name , *pwdCookie);                    \
            }                                                                          \
                                                                                       \
            return status;                                                             \
        }                                                                              \
        virtual TStatus STDCALL Unadvise(COMX_DWORD dwCookie)                               \
        {                                                                              \
            bool flag = m_pParent->m_p##name.Unadvise(dwCookie);                       \
                                                                                       \
            if(flag == true)                                                           \
                return M_OK;                                                           \
            else                                                                       \
                return M_NOEVENT;                                                      \
        }                                                                              \
    }m_xcp##name;

#define IMPLEMENT_EVENT_SOURCE_MULTI_EX(owner, eid, name)        \
public:                                \
    TOutInterface<name> m_p##eid##name;                                 \
public:                                    \
    class XCP##eid##name : public IEventSource{                         \
    private:                                \
    owner *m_pParent;                            \
    public:                                \
        XCP##eid##name(owner *parent){m_pParent = parent;}              \
    public:                                \
        virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv)\
        {                                \
        if (riid == IID_IRoot || riid == IID_IEventSource)    \
        {                            \
            *ppv = static_cast<IEventSource*>(this);        \
        }                            \
        else                            \
        {                            \
            return (*ppv = 0) , M_NOINTERFACE;            \
        }                            \
                                    \
        ((IRoot*)*ppv)->AddRef();                \
                                    \
        return M_OK;                        \
        }                                \
        virtual TStatus STDCALL AddRef(void)                \
        {                                \
        return m_pParent->AddRef();                \
        }                                \
        virtual TStatus STDCALL Release(void)                \
        {                                \
        return m_pParent->Release();                \
        }                                \
    public:                                \
        virtual TStatus STDCALL GetEventSourceInterface(MIID *pIID)     \
        {                                \
        assert(pIID);                        \
        *pIID = IID_##name;                    \
        return M_OK;                        \
        }                                \
        virtual TStatus STDCALL GetEventSourceContainer(IEventSourceContainer **ppESC) \
        {                                \
        assert(ppESC);                        \
        (*ppESC = m_pParent)->AddRef();                \
        return M_OK;                        \
        }                                \
        virtual TStatus STDCALL Advise(IRoot *pRoot,COMX_DWORD *pwdCookie)    \
        {                                \
        assert(pwdCookie&&pRoot);                \
        *pwdCookie = 0;                        \
                                    \
        name *p_##name = NULL;                    \
        TStatus status = pRoot->QueryInterface(IID_##name,    \
                               (void**)&p_##name); \
                                    \
        if (status == M_NOINTERFACE)                \
        {                            \
            status = M_NOEVENT;                    \
        }                            \
                                    \
        if(status == M_OK)                    \
        {                            \
            m_pParent->m_p##eid##name.Advise(p_##name , *pwdCookie); \
        }                            \
                                    \
        return status;                        \
        }                                \
        virtual TStatus STDCALL Unadvise(COMX_DWORD dwCookie)        \
        {                                \
        bool flag = m_pParent->m_p##eid##name.Unadvise(dwCookie); \
                                    \
        if(flag == true)                    \
            return M_OK;                    \
        else                            \
            return M_NOEVENT;                    \
        }                                \
    }m_xcp##eid##name;

#define ACTIVATE_EVENT(source,method)                                                  \
    assert(m_p##source);                                                               \
    if (m_p##source != NULL)                                                           \
    {                                                                                  \
        m_p##source->method();                                                         \
    }

#define ACTIVATE_EVENT1(source,method,para1)                                           \
    assert(m_p##source);                                                               \
    if (m_p##source != NULL)                                                           \
    {                                                                                  \
        m_p##source->method((para1));                                                  \
    }

#define ACTIVATE_EVENT2(source,method,para1,para2)                                     \
    assert(m_p##source);                                                               \
    if (m_p##source != NULL)                                                           \
    {                                                                                  \
        m_p##source->method((para1),(para2));                                          \
    }

#define ACTIVATE_EVENT3(source,method,para1,para2,para3)                               \
    assert(m_p##source);                                                               \
    if (m_p##source != NULL)                                                           \
    {                                                                                  \
        m_p##source->method((para1),(para2),(para3));                                  \
    }

#define ACTIVATE_EVENT4(source,method,para1,para2,para3,para4)                         \
    assert(m_p##source);                                                               \
    if (m_p##source != NULL)                                                           \
    {                                                                                  \
        m_p##source->method((para1),(para2),(para3),(para4));                          \
    }

#define ACTIVATE_EVENT5(source,method,para1,para2,para3,para4,para5)                   \
    assert(m_p##source);                                                               \
    if (m_p##source != NULL)                                                           \
    {                                                                                  \
        m_p##source->method((para1),(para2),(para3),(para4),(para5));                  \
    }

#define ACTIVATE_EVENT_EX(eid,source,method)                \
    assert(m_p##eid##source);                        \
    if (m_p##eid##source != NULL)                    \
    {                                    \
        m_p##eid##source->method();                    \
    }

#define ACTIVATE_EVENT1_EX(eid, source,method,para1)            \
    assert(m_p##eid##source);                        \
    if (m_p##eid##source != NULL)                    \
    {                                    \
        m_p##eid##source->method((para1));                \
    }

#define ACTIVATE_EVENT2_EX(eid,source,method,para1,para2)        \
    assert(m_p##eid##source);                        \
    if (m_p##eid##source != NULL)                    \
    {                                    \
        m_p##eid##source->method((para1),(para2));            \
    }

#define ACTIVATE_EVENT3_EX(eid, source,method,para1,para2,para3)    \
    assert(m_p##eid##source);                        \
    if (m_p##eid##source != NULL)                    \
    {                                    \
        m_p##eid##source->method((para1),(para2),(para3));        \
    }

#define ACTIVATE_EVENT4_EX(eid,source,method,para1,para2,para3,para4)    \
    assert(m_p##eid##source);                        \
    if (m_p##eid##source != NULL)                    \
    {                                    \
        m_p##eid##source->method((para1),(para2),(para3),(para4));    \
    }

#define ACTIVATE_EVENT5_EX(eid,source,method,para1,para2,para3,para4,para5) \
    assert(m_p##eid##source);                        \
    if (m_p##eid##source != NULL)                    \
    {                                    \
        m_p##eid##source->method((para1),(para2),(para3),(para4),(para5)); \
    }

#define ACTIVATE_EVENT_MULTI(source,method)                                            \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method();                                                           \
    }

#define ACTIVATE_EVENT1_MULTI(source,method,para1)                                     \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method(para1);                                                      \
    }

#define ACTIVATE_EVENT2_MULTI(source,method,para1,para2)                               \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method(para1,para2);                                                \
    }

#define ACTIVATE_EVENT3_MULTI(source,method,para1,para2,para3)                         \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method(para1,para2,para3);                                          \
    }

#define ACTIVATE_EVENT4_MULTI(source,method,para1,para2,para3,para4)                   \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method(para1,para2,para3,para4);                                    \
    }

#define ACTIVATE_EVENT5_MULTI(source,method,para1,para2,para3,para4,para5)             \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method(para1,para2,para3,para4,para5);                              \
    }

#define ACTIVATE_EVENT6_MULTI(source,method,para1,para2,para3,para4,para5,para6)       \
    for(TOutInterface<source>::iterator iter = m_p##source.begin() ;                   \
        iter != m_p##source.end() ; ++iter)                                            \
    {                                                                                  \
        (*iter) -> method(para1,para2,para3,para4,para5,para6);                        \
    }

#define ACTIVATE_EVENT_MULTI_EX(eid, source,method)            \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method();                        \
    }

#define ACTIVATE_EVENT1_MULTI_EX(eid, source,method,para1)        \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1);                    \
    }

#define ACTIVATE_EVENT2_MULTI_EX(eid, source,method,para1,para2)    \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2);                    \
    }

#define ACTIVATE_EVENT3_MULTI_EX(eid, source,method,para1,para2,para3)    \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3);                \
    }

#define ACTIVATE_EVENT4_MULTI_EX(eid,source,method,para1,para2,para3,para4) \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4);            \
    }

#define ACTIVATE_EVENT5_MULTI_EX(eid,source,method,para1,para2,para3,para4,para5) \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4,para5);        \
    }

#define ACTIVATE_EVENT6_MULTI_EX(eid,source,method,para1,para2,para3,para4,para5,para6)    \
    for(TOutInterface<source>::iterator iter = m_p##eid##source.begin() ; \
        iter != m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4,para5,para6);        \
    }

#define ACTIVATE_OWNER_EVENT(owner,source,method)            \
    assert(owner->m_p##source);                        \
    if (owner->m_p##source != NULL)                    \
    {                                    \
        owner->m_p##source->method();                    \
    }

#define ACTIVATE_OWNER_EVENT1(owner,source,method,para1)        \
    assert(owner->m_p##source);                        \
    if (owner->m_p##source != NULL)                    \
    {                                    \
        owner->m_p##source->method((para1));                \
    }

#define ACTIVATE_OWNER_EVENT2(owner,source,method,para1,para2)        \
    assert(owner->m_p##source);                        \
    if (owner->m_p##source != NULL)                    \
    {                                    \
        owner->m_p##source->method((para1),(para2));            \
    }

#define ACTIVATE_OWNER_EVENT3(owner,source,method,para1,para2,para3)    \
    assert(owner->m_p##source);                        \
    if (owner->m_p##source != NULL)                    \
    {                                    \
        owner->m_p##source->method((para1),(para2),(para3));        \
    }

#define ACTIVATE_OWNER_EVENT4(owner,source,method,para1,para2,para3,para4) \
    assert(owner->m_p##source);                        \
    if (owner->m_p##source != NULL)                    \
    {                                    \
        owner->m_p##source->method((para1),(para2),(para3),(para4));    \
    }

#define ACTIVATE_OWNER_EVENT5(owner,source,method,para1,para2,para3,para4,para5) \
    assert(owner->m_p##source);                        \
    if (owner->m_p##source != NULL)                    \
    {                                    \
    owner->m_p##source->method((para1),(para2),(para3),(para4),(para5)); \
    }

#define ACTIVATE_OWNER_EVENT_EX(owner,eid,source,method)        \
    assert(owner->m_p##eid##source);                    \
    if (owner->m_p##eid##source != NULL)                \
    {                                    \
        owner->m_p##eid##source->method();                \
    }

#define ACTIVATE_OWNER_EVENT1_EX(owner,eid, source,method,para1)    \
    assert(owner->m_p##eid##source);                    \
    if (owner->m_p##eid##source != NULL)                \
    {                                    \
        owner->m_p##eid##source->method((para1));            \
    }

#define ACTIVATE_OWNER_EVENT2_EX(owner,eid,source,method,para1,para2)    \
    assert(owner->m_p##eid##source);                    \
    if (owner->m_p##eid##source != NULL)                \
    {                                    \
        owner->m_p##eid##source->method((para1),(para2));        \
    }

#define ACTIVATE_OWNER_EVENT3_EX(owner,eid, source,method,para1,para2,para3) \
    assert(owner->m_p##eid##source);                    \
    if (owner->m_p##eid##source != NULL)                \
    {                                    \
        owner->m_p##eid##source->method((para1),(para2),(para3));    \
    }

#define ACTIVATE_OWNER_EVENT4_EX(owner,eid,source,method,para1,para2,para3,para4) \
    assert(owner->m_p##eid##source);                    \
    if (owner->m_p##eid##source != NULL)                \
    {                                    \
        owner->m_p##eid##source->method((para1),(para2),(para3),(para4)); \
    }

#define ACTIVATE_OWNER_EVENT5_EX(owner,eid,source,method,para1,para2,para3,para4,para5) \
    assert(owner->m_p##eid##source);                    \
    if (owner->m_p##eid##source != NULL)                \
    {                                    \
        owner->m_p##eid##source->method((para1),(para2),(para3),(para4),(para5)); \
    }

#define ACTIVATE_OWNER_EVENT_MULTI(owner,source,method)            \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method();                        \
    }

#define ACTIVATE_OWNER_EVENT1_MULTI(owner,source,method,para1)        \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1);                    \
    }

#define ACTIVATE_OWNER_EVENT2_MULTI(owner,source,method,para1,para2)    \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2);                    \
    }

#define ACTIVATE_OWNER_EVENT3_MULTI(owner,source,method,para1,para2,para3) \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3);                \
    }

#define ACTIVATE_OWNER_EVENT4_MULTI(owner,source,method,para1,para2,para3,para4) \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4);            \
    }

#define ACTIVATE_OWNER_EVENT5_MULTI(owner,source,method,para1,para2,para3,para4,para5) \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4,para5);        \
    }

#define ACTIVATE_OWNER_EVENT6_MULTI(owner,source,method,para1,para2,para3,para4,para5,para6) \
    for(TOutInterface<source>::iterator iter = owner->m_p##source.begin() ; \
        iter != owner->m_p##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4,para5,para6);        \
    }

#define ACTIVATE_OWNER_EVENT_MULTI_EX(owner,eid, source,method)        \
    for(TOutInterface<source>::iterator iter = owner->m_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method();                        \
    }

#define ACTIVATE_OWNER_EVENT1_MULTI_EX(owner,eid, source,method,para1)    \
    for(TOutInterface<source>::iterator iter = owner->m_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1);                    \
    }

#define ACTIVATE_OWNER_EVENT2_MULTI_EX(owner,eid, source,method,para1,para2) \
    for(TOutInterface<source>::iterator iter = owner-m>_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2);                    \
    }

#define ACTIVATE_OWNER_EVENT3_MULTI_EX(owner,eid, source,method,para1,para2,para3) \
    for(TOutInterface<source>::iterator iter = owner->m_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3);                \
    }

#define ACTIVATE_OWNER_EVENT4_MULTI_EX(owner,eid,source,method,para1,para2,para3,para4) \
    for(TOutInterface<source>::iterator iter = owner->m_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4);            \
    }

#define ACTIVATE_OWNER_EVENT5_MULTI_EX(owner,eid,source,method,para1,para2,para3,para4,para5) \
    for(TOutInterface<source>::iterator iter = owner->m_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4,para5);        \
    }

#define ACTIVATE_OWNER_EVENT6_MULTI_EX(owner,eid,source,method,para1,para2,para3,para4,para5,para6) \
    for(TOutInterface<source>::iterator iter = owner->m_p##eid##source.begin() ; \
        iter != owner->m_p##eid##source.end() ; ++iter)            \
    {                                    \
        (*iter) -> method(para1,para2,para3,para4,para5,para6);        \
    }

template<typename I>
void connect(IRoot *pRoot,I *pInterface,const MIID iid,COMX_DWORD &rdwCookie)
{
    IEventSourceContainer *pcES = 0;
    TStatus ret = pRoot->QueryInterface(IID_IEventSourceContainer,(void**)&pcES);

    if (ret != M_OK)
    {
        return;
    }

    IEventSource *pES = 0;
    ret = pcES->FindEventSource(iid,&pES);
    if (ret != M_OK)
    {
        pcES->Release();
        return;
    }

    pES->Advise((I*)pInterface,&rdwCookie);

    pES->Release();
    pcES->Release();
}

inline void disconnect(IRoot *pRoot,const MIID &iid,COMX_DWORD dwCookie)
{
    IEventSourceContainer *pcES = 0;
    TStatus ret = pRoot->QueryInterface(IID_IEventSourceContainer,(void**)&pcES);

    if (ret != M_OK)
    {
        return;
    }

    IEventSource *pES = 0;
    ret = pcES->FindEventSource(iid,&pES);
    if (ret != M_OK)
    {
        pcES->Release();
        return;
    }

    pES->Unadvise(dwCookie);

    pES->Release();
    pcES->Release();
}

template<typename I>
void connect(IRoot *pRoot,I *pInterface, MREFEID eid, const MIID &iid,COMX_DWORD &rdwCookie)
{
    IEventSourceContainerEx *pcES = 0;
    TStatus ret = pRoot->QueryInterface(IID_IEventSourceContainerEx,(void**)&pcES);

    if (ret != M_OK)
    {
        return;
    }

    IEventSource *pES = 0;
    ret = pcES->FindEventSourceEx(eid, iid,&pES);
    if (ret != M_OK)
    {
        pcES->Release();
        return;
    }

    pES->Advise((I*)pInterface,&rdwCookie);

    pES->Release();
    pcES->Release();
}

inline void disconnect(IRoot *pRoot, MREFEID eid, const MIID &iid,COMX_DWORD dwCookie)
{
    IEventSourceContainerEx *pcES = 0;
    TStatus ret = pRoot->QueryInterface(IID_IEventSourceContainerEx,(void**)&pcES);

    if (ret != M_OK)
    {
        return;
    }

    IEventSource *pES = 0;
    ret = pcES->FindEventSourceEx(eid, iid,&pES);
    if (ret != M_OK)
    {
        pcES->Release();
        return;
    }

    pES->Unadvise(dwCookie);

    pES->Release();
    pcES->Release();
}

#define GET_EVENT_INTERFACE(event_interface) m_p##event_interface
#define GET_EVENT_INTERFACE_OWNER(owner, event_interface) owner->m_p##event_interface

#define GET_EVENT_INTERFACE_EX(eid, itype) m_p##eid##itype
#define GET_EVENT_INTERFACE_OWNER_EX(owner, eid, itype) owner->m_p##eid##itype

#ifndef _KMAS_COMX_EXCEPTION_INTERFACE_DEFINE
#define _KMAS_COMX_EXCEPTION_INTERFACE_DEFINE

#define QUERY_INTERFACE(src, target_type, tar) \
    target_type *tar = NULL;\
    src->QueryInterface(IID_##target_type, (void**)&tar);\
    assert(tar)

#define CREATE_COMX_OBJ(factory, clsid, type, obj) \
    type *obj = NULL;\
    (factory).CreateInstance(clsid, IID_##type, (void**)&obj);\
    assert(obj)

#define QUERY_SERVICE(factory, clsid, type, obj) \
    type *obj = NULL;\
    (factory).QueryService(clsid, IID_##type, (void**)&obj);\
    assert(obj)

class TConnectManager
{
    vector<COMX_DWORD> _dwCookie;
    vector<MUID> _iid;
    vector<IRoot*> _pSrc;
public:
    TConnectManager(){}
    ~TConnectManager(){ReleaseConnects();}
public:
    void AddConnect(IRoot *pSrc, const MUID &iid, IRoot *pTar)
    {
        COMX_DWORD dwCookie = 0;

        KMAS::Die_maker::comx::connect(pSrc, pTar, iid, dwCookie);

        _dwCookie.push_back(dwCookie);
        _iid.push_back(iid);
        _pSrc.push_back(pSrc);
    }
    void ReleaseConnects()
    {
        for (int loop = 0; loop < static_cast<int>(_dwCookie.size()); ++loop)
        {
            KMAS::Die_maker::comx::disconnect(_pSrc[loop], _iid[loop], _dwCookie[loop]);
        }

        _dwCookie.clear();
        _iid.clear();
        _pSrc.clear();
    }
};

class TConnectManagerEx
{
    vector<COMX_DWORD> _dwCookie;
    vector<MUID> _iid;
    vector<IRoot*> _pSrc;
    vector<MUID> _eid;
public:
    TConnectManagerEx(){}
    ~TConnectManagerEx(){/*ReleaseConnects();*/}
public:
    void AddConnect(IRoot *pSrc, const MUID &eid, const MUID &iid, IRoot *pTar)
    {
        COMX_DWORD dwCookie = 0;

        KMAS::Die_maker::comx::connect(pSrc, pTar, eid, iid, dwCookie);

        _dwCookie.push_back(dwCookie);
        _iid.push_back(iid);
        _pSrc.push_back(pSrc);
        _eid.push_back(eid);
    }
    void ReleaseConnects()
    {
        for (int loop = 0; loop < static_cast<int>(_dwCookie.size()); ++loop)
        {
	  KMAS::Die_maker::comx::disconnect(_pSrc[loop], _eid[loop],
					    _iid[loop], _dwCookie[loop]);
        }

        _dwCookie.clear();
        _iid.clear();
        _pSrc.clear();
        _eid.clear();
    }
};

#endif

/* 
 * Define some macros for aggregation feature.
 */

#define INITIALIZE_AGGREGATION_FEATURE()           \
    _is_aggregation = false;            \
    _is_init_aggregation = false;        \
                        \
    if (pRootOuter)                \
    {                        \
    m_pRootOuter = pRootOuter;        \
    _is_aggregation = true;            \
    }                        \
    else                    \
    {                        \
    m_pRootOuter = &m_innerRoot;        \
    }

#define BEGIN_IMPLEMENT_AGGREGATION_ROOT()                              \
    virtual TStatus STDCALL InnerAddRef(void)                \
    {                                \
        return ++m_cRef;                        \
    }                                \
    virtual TStatus STDCALL InnerRelease(void)                \
    {                                \
        long res = --m_cRef;                    \
        if (res == 0) {                        \
        delete this;                        \
        }                                \
        return res;                            \
    }                                \
    virtual TStatus STDCALL InnerQueryInterface(MREFIID riid, void **ppv) \
    {

#define IMPLEMENT_INTERFACE_AGGREGATION_ROOT(Itf)          \
    if (riid == IID_IRoot)                \
    {                            \
    *ppv = static_cast<IRoot*>(&m_innerRoot);    \
    }

#define END_IMPLEMENT_AGGREGATION_ROOT()                \
        if(*ppv == 0)                        \
            return M_NOINTERFACE;                \
                                    \
        reinterpret_cast<IRoot*>(*ppv)->AddRef();            \
        return M_OK;                        \
    }                                \
public:                                    \
    virtual TStatus STDCALL AddRef(void)                \
    {                                \
        return m_pRootOuter->AddRef();                \
    }                                \
    virtual TStatus STDCALL Release(void)                \
    {                                \
        return m_pRootOuter->Release();                \
    }                                \
    virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv)    \
    {                                \
        return m_pRootOuter->QueryInterface(riid, ppv);        \
    }                                \
private:                                \
    long m_cRef;

#define IMPLEMENT_AGGREGATION_FEATURE(type)                    \
    class XNDRoot : public IRoot                    \
    {                                \
        type *This()                    \
        {                            \
            return (type*)((unsigned char*)this - offsetof(type, m_innerRoot));    \
        }                            \
        virtual TStatus STDCALL AddRef()            \
        {                            \
            return This()->InnerAddRef();            \
        }                            \
        virtual TStatus STDCALL Release()            \
        {                            \
            return This()->InnerRelease();            \
        }                            \
        virtual TStatus STDCALL QueryInterface(MREFIID riid, void **ppv) \
        {                            \
            return This()->InnerQueryInterface(riid, ppv);    \
        }                            \
    }m_innerRoot;                            \
private:                                \
    bool _is_aggregation;                        \
    bool _is_init_aggregation;                    \
private:                                \
    IRoot *m_pRootOuter;                        \
protected:                                \
    void InitAggregation()                        \
    {                                \
        if (!_is_init_aggregation && _is_aggregation)        \
        {                            \
            _is_init_aggregation = true;            \
            IAggregationOwner *p_aggregation_owner = NULL;    \
            m_pRootOuter->QueryInterface(IID_IAggregationOwner, (void**)&p_aggregation_owner); \
            assert(p_aggregation_owner);            \
                                    \
            p_aggregation_owner->InitOwnerResource();    \
                                    \
            p_aggregation_owner->Release();            \
        }                            \
    }

#define BEGIN_IMPLEMENT_CREATE_AGGREGATION_INSTANCE()                \
extern "C" {                                \
DLL_API TStatus CreateInstanceEx(IRoot *pRootOuter,MREFTYPEID rclsid,MREFIID riid,void **ppv) \
{                                    \
    *ppv = 0;                                \
                                    \
    if (pRootOuter != NULL && !(riid == IID_IRoot))            \
    {                                \
        return M_INVALID_ARG;                    \
    }

#define IMPLEMENT_CREATE_AGGREGATION_INSTANCE(type,clsid)    \
    if (rclsid == clsid)                    \
    {                                \
    type *obj = new type(pRootOuter);            \
                                \
    obj->InnerAddRef();                    \
    obj->InnerQueryInterface(riid,ppv);            \
    obj->InnerRelease();                    \
                                \
    if(*ppv == 0)                        \
    {                            \
        delete obj;                        \
    }                            \
    }


#define END_IMPLEMENT_CREATE_AGGREGATION_INSTANCE()            \
    if (*ppv == 0)                            \
    {                                \
        return M_NOINTERFACE;                    \
    }                                    \
    else                                \
    {                                    \
        return M_OK;                        \
    }                                    \
}                                    \
                                    \
DLL_API TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv) \
{                                    \
    return CreateInstanceEx(NULL, rclsid, riid, ppv);        \
}                                    \
}   

class TAggregationManager
{
    vector<IRoot*> _vec_p_root_aggregation;
    vector<MUID> _vec_p_root_aggregation_clsid;
public:
    ~TAggregationManager()
    {
        ReleaseAggregationObj();
    }
public:
    void Manage(MREFTYPEID clsid, IRoot *&pRoot)
    {
        assert(pRoot);

        _vec_p_root_aggregation.push_back(pRoot);
        _vec_p_root_aggregation_clsid.push_back(clsid);
    }
    void ReleaseAggregationObj()
    {
        for(int loop = 0; loop < static_cast<int>(_vec_p_root_aggregation.size()); ++loop)
        {
            _vec_p_root_aggregation[loop]->Release();
        }

        _vec_p_root_aggregation.clear();
        _vec_p_root_aggregation_clsid.clear();
    }
public:
    TStatus QueryInterface(MREFIID riid, void **ppv)
    {
        TStatus status = M_NOINTERFACE;

        for(int loop = 0; loop < static_cast<int>(_vec_p_root_aggregation.size()); ++loop)
        {
            status =  _vec_p_root_aggregation[loop]->QueryInterface(riid, ppv);
            if(status == M_OK && *ppv != 0)
                break;
        }

        return status;
    }
    IRoot *&GetAggregationObj(MREFTYPEID clsid)
    {
        int loop = 0;
        for (loop = 0; loop < static_cast<int>(_vec_p_root_aggregation_clsid.size()); ++loop)
        {
            if (_vec_p_root_aggregation_clsid[loop] == clsid)
            {
                break;
            }
        }
        
        return _vec_p_root_aggregation[loop];
    }
};  

#define BEGIN_IMPLEMENT_AGGREGATION_CONTAINER()    \
private:                    \
    void CreateAggregationObj()        \
    {                    \
        ++m_cRef;

#define CREATE_AGGREGATION_OBJ(clsid)                    \
    {                                    \
    IRoot *pRoot = NULL;                        \
    CreateInstanceEx((IAggregationOwner*)this, clsid, IID_IRoot, (void**)&pRoot); \
    assert(pRoot);                            \
                                    \
    _aggregation_mgr.Manage(clsid, pRoot);                \
    }


#define END_IMPLEMENT_AGGREGATION_CONTAINER()    \
        --m_cRef;                \
    }                    \
private:                    \
    TAggregationManager _aggregation_mgr;

#define CHAIN_AGGREGATION_OBJ_INTERFACE()    \
    if(*ppv == 0)                    \
    {                            \
    _aggregation_mgr.QueryInterface(riid, ppv);    \
    if(*ppv != 0)                    \
        return M_OK;                \
    }

#define IMPLEMENT_AGGREGATION_CONTAINER(type)                \
private:                                \
    class TInnerAggregationManager                    \
    {                                \
    public:                                \
        TInnerAggregationManager()                \
        {                            \
            This()->CreateAggregationObj();            \
        }                            \
        ~TInnerAggregationManager()                \
        {                            \
            This()->m_cRef++;                \
            This()->_cmgr.ReleaseConnects();        \
            This()->m_cRef--;                \
        }                            \
    public:                                \
        type *This()                        \
        {                            \
            return (type*)((unsigned char*)this - offsetof(type, _inner_aggregation_mgr)); \
        }                            \
    }_inner_aggregation_mgr;                    \
                                    \
    friend class TInnerAggregationManager;                \
private:                                \
    TConnectManagerEx _cmgr; 


// {6DDCB24C-3698-4770-8929-6B34C8F7B860}
const MUID IID_IGarbagCollect = 
{ 0x6ddcb24c, 0x3698, 0x4770, { 0x89, 0x29, 0x6b, 0x34, 0xc8, 0xf7, 0xb8, 0x60 } };

comx_interface IGarbagCollect : IRoot
{
    virtual TStatus STDCALL Register() = 0;
    virtual TStatus STDCALL Collect() = 0;
    virtual TStatus STDCALL Exclude() = 0;
    virtual TStatus STDCALL PushGC() = 0;
    virtual TStatus STDCALL PopGC() = 0;
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif /*_CUSTOM_IROOT_H*/
