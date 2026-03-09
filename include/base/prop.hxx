/*HEAD prop HXX ONESTEP */
#ifndef _KMAS_COMX_BASE_PROPERTY_H
#define _KMAS_COMX_BASE_PROPERTY_H

#include <base/root.hxx>

#include <string>
#include <map>
#include <iterator>
#include <functional>
#include <algorithm>
#include <vector>
using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {334F2521-57C0-40e0-AC9A-F493CE754F4C}
const MUID IID_IProperty = 
{ 0x334f2521, 0x57c0, 0x40e0, { 0xac, 0x9a, 0xf4, 0x93, 0xce, 0x75, 0x4f, 0x4c } };

comx_interface IProperty : public IRoot
{
     virtual TStatus STDCALL GetProperty(const char *name , void **prop) = 0;
};

template<typename T>
class TPropertyImpl
{
protected:
     class TPropertyItem
     {
     public:
          MTYPEID   _clsid;
          MUID      _iid;
          IRoot    *_p_root;
     };

     class TServiceItem
     {
     public:
          MTYPEID   _clsid;
          MUID      _iid;
          IRoot    *_p_root;
     };

     class TConnectItem
     {
     public:
          string    _src;
          string    _tar;
          COMX_DWORD     _cookie;
     };

     typedef map<string, TPropertyItem> TPropertyMapList;
     typedef map<string, TServiceItem>  TServiceMapList;
protected:
     TPropertyMapList     _property_map_list;
     TServiceMapList      _service_map_list;
     vector<TConnectItem> _connect_list;
protected:
     void addPropertyItem(const string &name, const MTYPEID &clsid, const MUID &iid)
     {
          TPropertyItem prop_item;
          //memset(&prop_item, 0, sizeof(prop_item));

          prop_item._clsid  = clsid;
          prop_item._iid    = iid;
          prop_item._p_root = NULL;

          T *pT = static_cast<T*>(this);

          pT->CreateInstance(clsid, iid, (void**)&prop_item._p_root);

          if (prop_item._p_root == NULL)
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");
               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "addPropertyItem - error : \"%s\".\n", name.c_str());
               fprintf(fp, "reason : the corresponding interface to \"%s\" isn't exist.\n", name.c_str());
               fprintf(fp, "advice : <1>firstly please check if or not you have loaded corresponding kpi file.\n");
               fprintf(fp, "         <2>secondly please check your source codes of kpi component if or not\n");
               fprintf(fp, "         have implemented corresponding interface in the component.\n");
               fclose(fp);
          }

          assert(prop_item._p_root);

          _property_map_list[name] = prop_item;
     }

     void educePropertyItem(const string &name, const string &from_name, const MUID &iid)
     {
          TPropertyItem prop_item;
          //memset(&prop_item, 0, sizeof(prop_item));

          if (_property_map_list.find(from_name) == _property_map_list.end())
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");
               
               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "educePropertyItem - error : \"%s\".\n", from_name.c_str());
               fprintf(fp, "reason : the property-\"%s\" isn't defined.\n", from_name.c_str());
               fprintf(fp, "advice : please ensure you have defined the property-\"%s\".\n", from_name.c_str());

               fclose(fp);
          }

          assert(_property_map_list.find(from_name) != _property_map_list.end());

          prop_item._clsid  = _property_map_list[from_name]._clsid;
          prop_item._iid    = iid;
          prop_item._p_root = NULL;

          _property_map_list[from_name]._p_root->QueryInterface(iid, (void**)&prop_item._p_root);

          if (prop_item._p_root == NULL)
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");
               
               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "educePropertyItem - error : \"%s\".\n", name.c_str());
               fprintf(fp, "reason : the corresponding interface to \"%s\" isn't exist.\n", name.c_str());
               fprintf(fp, "advice : <1>firstly please check if or not you have loaded corresponding kpi file.\n");
               fprintf(fp, "         <2>secondly please check your source codes of kpi component if or not\n");
               fprintf(fp, "         have implemented corresponding interface in the component.\n");

               fclose(fp);
          }

          assert(prop_item._p_root);

          _property_map_list[name] = prop_item;
     }

     void addServiceItem(const string &name, const MTYPEID &clsid, const MUID &iid)
     {
          TServiceItem svr_item;
          //memset(&svr_item, 0, sizeof(svr_item));

          svr_item._clsid  = clsid;
          svr_item._iid    = iid;
          svr_item._p_root = NULL;

          T *pT = static_cast<T*>(this);
          pT->QueryService(clsid, iid, (void**)&svr_item._p_root);

          if (svr_item._p_root == NULL)
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");
               
               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "addServiceItem - error : \"%s\".\n", name.c_str());
               fprintf(fp, "reason : the corresponding interface to \"%s\" isn't exist.\n", name.c_str());
               fprintf(fp, "advice : <1>firstly please check if or not you have loaded corresponding kpi file.\n");
               fprintf(fp, "         <2>secondly please check your source codes of kpi component if or not\n");
               fprintf(fp, "         have implemented corresponding interface in the component.\n");

               fclose(fp);
          }

          assert(svr_item._p_root);

          _service_map_list[name] = svr_item;
     }

     void addConnectItem(const string &src, const string &tar)
     {
          TConnectItem connect_item;
          //memset(&connect_item, 0, sizeof(connect_item));
          
          IRoot *pSrcRoot = NULL;
          IRoot *pTarRoot = NULL;

          MUID tar_iid;
          memset(&tar_iid, 0, sizeof(tar_iid));

          if (_property_map_list.find(src) != _property_map_list.end())
          {
               pSrcRoot = _property_map_list[src]._p_root;
          }
          else if (_service_map_list.find(src) != _service_map_list.end())
          {
               pSrcRoot = _service_map_list[src]._p_root;
          }
          else
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");

               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "addConnectItem - error : src = \"%s\".\n", src.c_str());
               fprintf(fp, "reason : the corresponding interface to \"%s\" isn't exist.\n", src.c_str());
               fprintf(fp, "advice : please check if or not you have addPropertyItem for \"%s\".\n", src.c_str());
               
               fclose(fp);
               assert(false);
          }

          pSrcRoot->AddRef();

          if (_property_map_list.find(tar) != _property_map_list.end())
          {
               pTarRoot = _property_map_list[tar]._p_root;
               tar_iid = _property_map_list[tar]._iid;
          }
          else if (_service_map_list.find(tar) != _service_map_list.end())
          {
               pTarRoot = _service_map_list[tar]._p_root;
               tar_iid = _service_map_list[tar]._iid;
          }
          else
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");

               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "addConnectItem - error : tar = \"%s\".\n", tar.c_str());
               fprintf(fp, "reason : the corresponding interface to \"%s\" isn't exist.\n", tar.c_str());
               fprintf(fp, "advice : please check if or not you have addPropertyItem for \"%s\".\n", tar.c_str());

               fclose(fp);
               assert(false);
          }

          pTarRoot->AddRef();

          connect(pSrcRoot, pTarRoot, tar_iid, connect_item._cookie);
          connect_item._src = src;
          connect_item._tar = tar;

          _connect_list.push_back(connect_item);

          pSrcRoot->Release();
          pTarRoot->Release();
     }

     void getProperty(const string &name, void **prop)
     {
          if (_property_map_list.find(name) == _property_map_list.end())
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");

               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "getProperty - error : \"%s\".\n", name.c_str());
               fprintf(fp, "reason : the corresponding property to \"%s\" isn't exist.\n", name.c_str());

               fclose(fp);
          }
               
          assert(_property_map_list.find(name) != _property_map_list.end());
          _property_map_list[name]._p_root->QueryInterface(_property_map_list[name]._iid, prop);
     }
     
     void dispose()
     {
          for (typename vector<TConnectItem>::iterator iter_connect = _connect_list.begin() ; 
               iter_connect != _connect_list.end() ; ++iter_connect)
          {
               _disposeConnect(*iter_connect);
          }
          for (typename TPropertyMapList::iterator iter_prop = _property_map_list.begin() ;
               iter_prop != _property_map_list.end() ; ++iter_prop)
          {
               _disposeProperty(iter_prop->second);
          }

          for (typename TServiceMapList::iterator iter_svr = _service_map_list.begin() ;
               iter_svr != _service_map_list.end() ; ++iter_svr)
          {
               _disposeService(iter_svr->second);
          }

          _connect_list.clear();
          _property_map_list.clear();
          _service_map_list.clear();
     }

protected:
     void _disposeProperty(TPropertyItem &prop_item)
     {
          prop_item._p_root->Release();
          prop_item._p_root = NULL;
     }

     void _disposeService(TServiceItem &svr_item)
     {
          svr_item._p_root->Release();
          svr_item._p_root = NULL;
     }
     void _disposeConnect(TConnectItem &connect_item)
     {
          IRoot *pSrcRoot = NULL;
          string src = connect_item._src;
          COMX_DWORD &dwCookie = connect_item._cookie;

          if (_property_map_list.find(src) != _property_map_list.end())
          {
               pSrcRoot = _property_map_list[src]._p_root;
               pSrcRoot->AddRef();
          }
          else if (_service_map_list.find(src) != _service_map_list.end())
          {
               pSrcRoot = _service_map_list[src]._p_root;
               pSrcRoot->AddRef();
          }
          else
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");

               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "disposeConnect - error : \"%s\".\n", src.c_str());
               fprintf(fp, "reason : the corresponding property to \"%s\" isn't exist.\n", src.c_str());

               fclose(fp);
               assert(false);
          }

          string tar = connect_item._tar;
          MUID tar_iid;
          //memset(&tar_iid, 0, sizeof(tar_iid));

          if (_property_map_list.find(tar) != _property_map_list.end())
          {
               tar_iid = _property_map_list[tar]._iid;
          }
          else if (_service_map_list.find(tar) != _service_map_list.end())
          {
               tar_iid = _service_map_list[tar]._iid;
          }
          else
          {
               FILE *fp = fopen("c:/comx_debug.txt", "w");

               fprintf(fp, "COMX Debug Infomation\n============================================\n");
               fprintf(fp, "disposeConnect - error : \"%s\".\n", tar.c_str());
               fprintf(fp, "reason : the corresponding property to \"%s\" isn't exist.\n", tar.c_str());

               fclose(fp);
               assert(false);
          }

          disconnect(pSrcRoot, tar_iid, dwCookie);

          pSrcRoot->Release();
     }
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif
