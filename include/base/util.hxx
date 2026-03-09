/*==============================================================================

  Copyright 2006 Jilin Kingmesh Corp.
  All rights reserved
  
  ================================================================================ 
  File description:
	
  -- Please append file description informations here --
       A dialog about check and modify overlap condition .
  ================================================================================
	  Date            Name                    Description of Change
	  05/08/2007      Zhang Xiang-kui 	      Create
	  
		$HISTORY$
================================================================================*/

#ifndef _KMAS_COMX_BASE_UTIL_HXX
#define _KMAS_COMX_BASE_UTIL_HXX

#include <string>
#include <vector>

using namespace std;

#include <base/function.hxx>
using namespace KMAS::func;

#include <base/factory.hxx>
#include <base/prop.hxx>
//#include <ui/ui.hxx>
//#include <fem/fem.hxx>
#include <gl/gl_content.hxx>
#include <gl/gl_wcs.hxx>
#include <gl/gl_material.hxx>

#include <fstream>
using namespace std;

#include <base/tag_pool.hxx>

using namespace KMAS::Die_maker::comx;

namespace KMAS
{
namespace util
{
    // class: TFileDialogFilter.
    // 
    class TFileDialogFilter
    {
        vector<string> _items;
        int _cur_activate_item;
    public:
        inline TFileDialogFilter();
    public:
        inline void AppendItem(const string &description, const string &fmt_info);
        inline string GetFilter();
        inline void ActivateItem(const int index);
        inline void ActivateItem(const char* ext_fname);
        inline void Serialize(const char* fname, bool is_save = true);
    };

    void TFileDialogFilter::AppendItem(
        const string &description, 
        const string &fmt_info)
    {
        string item = description + "|" + fmt_info;
        _items.push_back(item);
    }

    TFileDialogFilter::TFileDialogFilter() :
        _cur_activate_item(0)
    {
        // Do nothing.
    }

    string TFileDialogFilter::GetFilter()
    {
        string ret;

		if (_items.size() != 0) {
			ret += _items[_cur_activate_item];
			ret += "|";
		}

		for (vector<string>::size_type loop = 0; loop < _items.size(); ++loop)
        {
            if (loop != _cur_activate_item)
            {
                ret += _items[loop];
                ret += "|";
            }
        }

        ret += "All Files(*.*)|*.*|";

        return ret;
    }

    void TFileDialogFilter::ActivateItem(const int index)
    {
        _cur_activate_item = index;
    }

    void TFileDialogFilter::ActivateItem(const char* ext_fname)
    {
		for (vector<string>::size_type loop = 0; loop < _items.size(); ++loop)
        {
            vector<int> records;
            bool is_find = KMAS::func::regex_parser(
                ext_fname, 
                _items[loop].c_str(), 
                records);

            if (is_find)
            {
                ActivateItem(loop);
                break;
            }
        }
    }

	void TFileDialogFilter::Serialize(const char* fname, bool is_save)
	{  
		TFactory factory;

		char home_path[256] = "";
		factory.GetHomePath(home_path);

		string strFullName = home_path;
		strFullName += fname;

		if (is_save)
		{
		}
		else
		{
		}
	}

    // class: TUiMeshPickupToolkit.
    //
    class TUiMeshPickupToolkit
    {
    private:
        enum _TMethodType
        {
            GET_METHOD,
            SET_METHOD
        };
    private:
        bool _lock_method(_TMethodType type, bool is_locked = true)
        {
            static bool s_is_locked = false;

            if (type == SET_METHOD)
                s_is_locked = is_locked;

            return s_is_locked;
        }
        bool _is_lock()
        {
            return _lock_method(GET_METHOD);
        }
        bool _lock(const bool flag)
        {
            return _lock_method(SET_METHOD, flag);
        }
    private:
        bool _activate_method(_TMethodType type, bool is_activate = true)
        {
            static bool s_is_activate = false;

            if (type == SET_METHOD)
                s_is_activate = is_activate;

            return s_is_activate;
        }
        bool _is_activate()
        {
            return _activate_method(GET_METHOD);
        }
        bool _activate(const bool flag)
        {
            return _activate_method(SET_METHOD, flag);
        }
    private:
        TConnectManager cmgr;
    private:
        POINT _get_position()
        {
            IComxUiPluginServerFrm *p_plugin_frm = _get_server_frm();

            HWND clientWnd;
            p_plugin_frm->GetClientWnd(clientWnd);
            RECT rc;
            GetClientRect(clientWnd, &rc);
            POINT pt_rb;
            pt_rb.x = rc.left;
            pt_rb.y = rc.top;
            ClientToScreen(clientWnd, &pt_rb);
            pt_rb.x += 12;
            pt_rb.y += 12;

            p_plugin_frm->Release();

            return pt_rb;
        }
        IComxUiPluginServerFrm *_get_server_frm()
        {
            TFactory factory;
            QUERY_SERVICE(
                factory,
                CLSID_IPlugServer,
                IComxUiPluginServerFrm,
                p_plugin_frm);

            assert(p_plugin_frm);

            return p_plugin_frm;
        }
        IUiMeshesSelectionToolkit *_get_pickup_component()
        {
            TFactory factory;

            QUERY_SERVICE(
                factory,
                CLSID_IUiMeshesSelectionToolkit,
                IUiMeshesSelectionToolkit,
                p_pickup_toolkit);

            assert(p_pickup_toolkit);

            return p_pickup_toolkit;
        }
    public:
        TUiMeshPickupToolkit()
        {
            
        }
    public:
        bool Attach(IFemCore *&p_fem_core)
        {
            if (!_is_lock())
            {
                IUiMeshesSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
                cmgr.AddConnect(p_pickup_toolkit, IID_IFemCore, p_fem_core);
                p_pickup_toolkit->Release();

                _lock(true);

                return true;
            }

            return false;
        }
        bool Detach()
        {
            if (_is_lock())
            {
                cmgr.ReleaseConnects();
                _lock(false);

                return true;
            }

            return false;
        }
        void SetMask(int enable, int disable)
        {
            IUiMeshesSelectionToolkit *p_pickup_toolkit = _get_pickup_component();

            int mask = 0;
            p_pickup_toolkit->GetMask(mask);
            mask |= enable;
            mask &= (~disable);
            p_pickup_toolkit->SetMask(mask);

            p_pickup_toolkit->Release();
        }
        bool Activate(int content, int range, int operation)
        {
            if (_is_lock() && !_is_activate())
            {
                POINT pos = _get_position();

                IUiMeshesSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
                p_pickup_toolkit->SetPosition(pos.x, pos.y);
                p_pickup_toolkit->Activate(
					(TUiMeshesSMContentType)content, 
					(TUiMeshesSMRangeType)range, 
					(TUiMeshesSMOperationType)operation);
                p_pickup_toolkit->Release();

                _activate(true);
				
                return true;
            }

            return false;
        }
        bool Closure()
        {
			Detach(); // Added by Xiang-kui Zhang, at 09/26/2008

            if (_is_activate())
            {
                IUiMeshesSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
                p_pickup_toolkit->Clear();
                p_pickup_toolkit->Closure();
                p_pickup_toolkit->Release();

                _activate(false);

                return true;
            }

            return false;
        }
        void GetPickupResult(int *&ids, int &len)
        {
            if (!_is_activate())
            {
                ids = NULL;
                len = 0;

                return;
            }

            IUiMeshesSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
            p_pickup_toolkit->GetPickupResult(ids, len);
            p_pickup_toolkit->Release();

            if (ids[0] == -1)
            {
                ids = NULL;
                len = 0;
            }
        }
    };

#ifdef USE_ACIS_UI_COMPONENT
	// class: TUiAcisPickupToolkit.
    //
    class TUiAcisPickupToolkit
    {
    private:
        enum _TMethodType
        {
            GET_METHOD,
            SET_METHOD
        };
    private:
        bool _lock_method(_TMethodType type, bool is_locked = true)
        {
            static bool s_is_locked = false;

            if (type == SET_METHOD)
                s_is_locked = is_locked;

            return s_is_locked;
        }
        bool _is_lock()
        {
            return _lock_method(GET_METHOD);
        }
        bool _lock(const bool flag)
        {
            return _lock_method(SET_METHOD, flag);
        }
    private:
        bool _activate_method(_TMethodType type, bool is_activate = true)
        {
            static bool s_is_activate = false;

            if (type == SET_METHOD)
                s_is_activate = is_activate;

            return s_is_activate;
        }
        bool _is_activate()
        {
            return _activate_method(GET_METHOD);
        }
        bool _activate(const bool flag)
        {
            return _activate_method(SET_METHOD, flag);
        }
    private:
        TConnectManager cmgr;
    private:
        POINT _get_position()
        {
            IComxUiPluginServerFrm *p_plugin_frm = _get_server_frm();

            HWND clientWnd;
            p_plugin_frm->GetClientWnd(clientWnd);
            RECT rc;
            GetClientRect(clientWnd, &rc);
            POINT pt_rb;
            pt_rb.x = rc.left;
            pt_rb.y = rc.top;
            ClientToScreen(clientWnd, &pt_rb);
            pt_rb.x += 12;
            pt_rb.y += 12;

            p_plugin_frm->Release();

            return pt_rb;
        }
        IComxUiPluginServerFrm *_get_server_frm()
        {
            TFactory factory;
            QUERY_SERVICE(
                factory,
                CLSID_IPlugServer,
                IComxUiPluginServerFrm,
                p_plugin_frm);

            assert(p_plugin_frm);

            return p_plugin_frm;
        }
        IUiAcisSelectionToolkit *_get_pickup_component()
        {
            TFactory factory;

            QUERY_SERVICE(
                factory,
                CLSID_IUiAcisSelectionToolkit,
                IUiAcisSelectionToolkit,
                p_pickup_toolkit);

            assert(p_pickup_toolkit);

            return p_pickup_toolkit;
        }
    public:
        TUiAcisPickupToolkit()
        {
            
        }
    public:
        bool Attach(IAcisCore *&p_acis_core, IUiAcisSelectionToolkitNotify *p_sel_notify = NULL)
        {
            if (!_is_lock())
            {
                IUiAcisSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
                cmgr.AddConnect(p_pickup_toolkit, IID_IAcisCore, p_acis_core);

				if (p_sel_notify != NULL)
				{
					cmgr.AddConnect(p_pickup_toolkit, IID_IUiAcisSelectionToolkitNotify, p_sel_notify);
				}
                p_pickup_toolkit->Release();

                _lock(true);

                return true;
            }

            return false;
        }
        bool Detach()
        {
            if (_is_lock())
            {
                cmgr.ReleaseConnects();
                _lock(false);

                return true;
            }

            return false;
        }
		bool Activate(int content, int range, int operation, bool is_show = true)
        {
            if (/*_is_lock() &&*/ !_is_activate())
            {
                POINT pos = _get_position();

                IUiAcisSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
                p_pickup_toolkit->SetPosition(pos.x, pos.y);

				int mask = content|range|operation;

                p_pickup_toolkit->Activate(mask, is_show);
                p_pickup_toolkit->Release();

                _activate(true);

                return true;
            }

            return false;
        }
		bool SetActiveMask(const int mask)
		{
			 IUiAcisSelectionToolkit *p_pickup_toolkit = 
				 _get_pickup_component();

			 p_pickup_toolkit->SetActiveMask(mask);

			 p_pickup_toolkit->Release();

			 return true;
		}
        bool Closure()
        {
			Detach(); // Added by Xiang-kui Zhang, at 09/26/2008
            if (_is_activate())
            {
                IUiAcisSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
                p_pickup_toolkit->Clear();
                p_pickup_toolkit->Closure();
                p_pickup_toolkit->Release();

                _activate(false);

                return true;
            }

            return false;
        }
        void GetPickupResult(tag_t *&res, int &cnt)
        {
            if (!_is_activate())
            {
				res = NULL;
				cnt = 0;

                return;
            }

            IUiAcisSelectionToolkit *p_pickup_toolkit = _get_pickup_component();
            p_pickup_toolkit->GetPickupResult(res, cnt);
            p_pickup_toolkit->Release();
        }
    };
#endif

	// class TPluginToolbox.
	class TPluginToolbox
	{
		TFileDialogFilter &_filter;
	public:
		TPluginToolbox(TFileDialogFilter &filter) :
			_filter(filter){}
	public:
		bool CallOpenFileDialog(char *filename)
		{
			TFactory factory;
			QUERY_SERVICE(
				factory,
				CLSID_IPluginToolbox,
				IPluginToolbox,
				p_plugin_toolbox);

			assert(p_plugin_toolbox);

			TStatus ret = p_plugin_toolbox->OpenFile(
				_filter.GetFilter().c_str(),
				filename);

			p_plugin_toolbox->Release();

			if (ret == M_OK) {
				_filter.ActivateItem(get_ext_filename(filename).c_str());
			}

			return ret == M_OK;
		}

		bool CallSaveFileDialog(char *filename)
		{
			TFactory factory;
			QUERY_SERVICE(
				factory,
				CLSID_IPluginToolbox,
				IPluginToolbox,
				p_plugin_toolbox);

			assert(p_plugin_toolbox);

			TStatus ret = p_plugin_toolbox->SaveFile(
				_filter.GetFilter().c_str(),
				filename);

			p_plugin_toolbox->Release();

			if (ret == M_OK) {
				_filter.ActivateItem(get_ext_filename(filename).c_str());
			}

			return ret == M_OK;
		}

		void CallWarningDialog(const char *msg)
		{
			TFactory factory;
			QUERY_SERVICE(
				factory,
				CLSID_IPluginToolbox,
				IPluginToolbox,
				p_plugin_toolbox);

			assert(p_plugin_toolbox);

			p_plugin_toolbox->WarningMessageBox(msg);

			p_plugin_toolbox->Release();
		}

		void CallInfoDialog(const char *msg)
		{
			TFactory factory;
			QUERY_SERVICE(
				factory,
				CLSID_IPluginToolbox,
				IPluginToolbox,
				p_plugin_toolbox);

			assert(p_plugin_toolbox);

			p_plugin_toolbox->InfoMessageBox(msg);

			p_plugin_toolbox->Release();
		}

		void CallErrorInfoDialog(const char *msg)
		{
			TFactory factory;
			QUERY_SERVICE(
				factory,
				CLSID_IPluginToolbox,
				IPluginToolbox,
				p_plugin_toolbox);

			assert(p_plugin_toolbox);

			p_plugin_toolbox->ErrorMessageBox(msg);

			p_plugin_toolbox->Release();
		}

		bool CallFolderFileDialog(char *folder_name)
		{
			TFactory factory;
			QUERY_SERVICE(
				factory,
				CLSID_IPluginToolbox,
				IPluginToolbox2,
				p_plugin_toolbox2);

			assert(p_plugin_toolbox2);

			TStatus ret = p_plugin_toolbox2->FoldFile(folder_name);

			p_plugin_toolbox2->Release();

			return ret == M_OK;
		}
	};

	inline POINT get_gl_client_center()
	{
		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IPlugServer, 
			IComxUiPluginServerFrm, 
			p_plugin_frm);
		assert(p_plugin_frm);

		HWND clientWnd;
		p_plugin_frm->GetClientWnd(clientWnd);

		RECT rc;
		GetClientRect(clientWnd, &rc);

		POINT pt_rb;
		pt_rb.x = (rc.right+rc.left)/2;
		pt_rb.y = (rc.bottom+rc.top)/2;
		ClientToScreen(clientWnd, &pt_rb);

		p_plugin_frm->Release();

		return pt_rb;
	}

    inline POINT get_gl_client_right_bottom()
	{
		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IPlugServer, 
			IComxUiPluginServerFrm, 
			p_plugin_frm);
		assert(p_plugin_frm);

		HWND clientWnd;
		p_plugin_frm->GetClientWnd(clientWnd);

		RECT rc;
		GetClientRect(clientWnd, &rc);

		POINT pt_rb;
		pt_rb.x = rc.right;
		pt_rb.y = rc.bottom;
		ClientToScreen(clientWnd, &pt_rb);

		p_plugin_frm->Release();

		return pt_rb;
	}

	inline POINT get_gl_client_left_top()
	{
		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IPlugServer, 
			IComxUiPluginServerFrm, 
			p_plugin_frm);
		assert(p_plugin_frm);

		HWND clientWnd;
		p_plugin_frm->GetClientWnd(clientWnd);

		RECT rc;
		GetClientRect(clientWnd, &rc);

		POINT pt_rb;
		pt_rb.x = rc.left;
		pt_rb.y = rc.top;
		ClientToScreen(clientWnd, &pt_rb);

		p_plugin_frm->Release();

		return pt_rb;
	}

	/////////////////////////////////////////////////////////////////////////////
	// class TGuideToolbarUtil.
	// 

	class TGuideToolbarUtil
	{
		int _next_id;
		int _abort_id;
		int _cur_step_id_idx;
		vector<int> _step_ids;
	private:
		void _next()
		{
			_cur_step_id_idx++;
			if (_cur_step_id_idx == _step_ids.size()) 
				_cur_step_id_idx = 0;

			OnStep(_cur_step_id_idx);
		}
		void _abort()
		{
			OnAbort();
		}
	public:
		TGuideToolbarUtil(const int next_id = 0, const int abort_id = 1) :
			_next_id(next_id), 
			_abort_id(abort_id),
			_cur_step_id_idx(0)
		{
			// Nothing.
		}
	public:
		void DispatchCmd(const int cmd_id, int &cur_cmd_id)
		{
			if (cmd_id == _next_id)
			{
				_next();
			}

			if (cmd_id == _abort_id)
			{
				_abort();
			}

			for (vector<int>::size_type idx = 0; idx < _step_ids.size(); ++idx)
			{
				if (_step_ids[idx] == cmd_id)
				{
					_cur_step_id_idx = idx;
					OnStep(idx);
				}
			}

			cur_cmd_id = _step_ids[_cur_step_id_idx];
		}
		void AttachStepIds(const int *ids, const int ids_num)
		{
			for (int loop = 0; loop < ids_num; ++loop)
				_step_ids.push_back(ids[loop]);
		}
	protected:
		virtual void OnAbort() = 0;
		virtual void OnStep(const int cmd_idx) = 0;
	protected:
		void SetCurrentStepIndex(const int index){_cur_step_id_idx = index;}
	};

	// class TFemCorePackage.
	class TFemCorePackage : public IUiProgressBarDriver
	{
		typedef bool (*pfn_update_ui_t)(const int &step, const char *msg);
	public:
		BEGIN_IMPLEMENT_NO_COUNT_ROOT()
		    IMPLEMENT_INTERFACE(IUiProgressBarDriver)
		    IMPLEMENT_INTERFACE_ROOT(IUiProgressBarDriver)
		END_IMPLEMENT_ROOT()
	private:
		IFemCore *_p_fem_core;
		IFemRender *_p_fem_render;
		TConnectManager _cmgr;
		pfn_update_ui_t _pfn_update_ui;
	private:
		void _clear_fem_core_obj()
		{
			if (_p_fem_core != NULL) 
			{
				_p_fem_core->Release();
				_p_fem_core = NULL;
			}
		}
		void _clear_fem_render_obj()
		{
			if (_p_fem_render != NULL) 
			{
				_p_fem_render->Release();
				_p_fem_render = NULL;
			}
		}
		bool _is_nastran_file(const char *fname)
		{
			return get_ext_filename(fname) == "nas" || 
				get_ext_filename(fname) == "dat";
		}
		bool _clear_connect()
		{
			_cmgr.ReleaseConnects();
			return true;
		}
	public:
		TFemCorePackage()
		{
			_p_fem_core = NULL;
			_p_fem_render = NULL;
			_pfn_update_ui = NULL;
		}
		~TFemCorePackage()
		{
			_clear_fem_core_obj();
			_clear_fem_render_obj();
			_clear_connect();
		}
	public:
		void Import(const char *fname, pfn_update_ui_t pfn_update_ui)
		{
			if (_is_nastran_file(fname)) 
			{
				_clear_fem_core_obj();
				_clear_fem_render_obj();
				_clear_connect();

				TFactory factory;

				CREATE_COMX_OBJ(
					factory,
					CLSID_IFemCore,
					IFemCore,
					p_fem_core);
				assert(p_fem_core);
				_p_fem_core = p_fem_core;
				
				CREATE_COMX_OBJ(
					factory,
					CLSID_IFemSerialize,
					IFemSerialize,
					p_fem_serialize);
				assert(p_fem_serialize);

				_pfn_update_ui = pfn_update_ui;

				CREATE_COMX_OBJ(
					factory,
					CLSID_IUiProgressBar,
					IUiProgressBar,
					p_ui_progbar);
				assert(p_ui_progbar);

				TConnectManager cmgr;
				cmgr.AddConnect(
					p_ui_progbar, 
					IID_IUiProgressBarDriver, 
					(IUiProgressBarDriver*)this);

				cmgr.AddConnect(
					p_fem_serialize, 
					IID_IUiProgressBar,
					p_ui_progbar);

				cmgr.AddConnect(
					p_fem_serialize,
					IID_IFemCore,
					_p_fem_core);

				p_fem_serialize->Import(fname);

				cmgr.ReleaseConnects();
				p_ui_progbar->Release();
				_pfn_update_ui = NULL;
				p_fem_serialize->Release();
			}
		}
		void Import(const char *fname)
		{
			if (_is_nastran_file(fname)) 
			{
				_clear_fem_core_obj();
				_clear_fem_render_obj();
				_clear_connect();

				TFactory factory;

				CREATE_COMX_OBJ(
					factory,
					CLSID_IFemCore,
					IFemCore,
					p_fem_core);
				assert(p_fem_core);
				_p_fem_core = p_fem_core;
				
				CREATE_COMX_OBJ(
					factory,
					CLSID_IFemSerialize,
					IFemSerialize,
					p_fem_serialize);
				assert(p_fem_serialize);

                QUERY_SERVICE(
					factory,
					CLSID_IUiProgressBar,
					IUiProgressBar,
					p_ui_progbar);
				assert(p_ui_progbar);

				TConnectManager cmgr;
				
				cmgr.AddConnect(
					p_fem_serialize, 
					IID_IUiProgressBar,
					p_ui_progbar);

				cmgr.AddConnect(
					p_fem_serialize,
					IID_IFemCore,
					_p_fem_core);

				p_fem_serialize->Import(fname);

				cmgr.ReleaseConnects();
				p_ui_progbar->Release();
				p_fem_serialize->Release();
			}
		}
		void Attach(IFemCore *p_fem_core)
		{
			_clear_fem_core_obj();
			_clear_fem_render_obj();
			_clear_connect();

			p_fem_core->AddRef();
			_p_fem_core = p_fem_core;
		}
		void EnableRender(const bool flag)
		{
			if (_p_fem_core == NULL) 
			{
				return;
			}

			TFactory factory;   
			if (flag && _p_fem_render == NULL) 
			{
				CREATE_COMX_OBJ(
					factory,
					CLSID_IFemRender,
					IFemRender,
					p_fem_render);
				assert(p_fem_render);
				_p_fem_render = p_fem_render;

                _cmgr.AddConnect(
					p_fem_render, 
					IID_IFemCore, 
					_p_fem_core);

				QUERY_SERVICE(
					factory,
					CLSID_IGlContent,
					IGlContentDriver,
					p_gl_content_driver);
				assert(p_gl_content_driver);

				_cmgr.AddConnect(
					p_gl_content_driver,
					IID_IGlContentRender,
					_p_fem_render);

				p_gl_content_driver->Release();

				CREATE_COMX_OBJ(
					factory,
					CLSID_IGlMaterial,
					IGlMaterialCustomer,
					p_gl_material);

				_cmgr.AddConnect(
					_p_fem_render,
					IID_IGlMaterialConnector,
					p_gl_material);

				p_gl_material->Release();

// 				QUERY_INTERFACE(
// 					_p_fem_render,
// 					IGlWCSModalRedraw,
// 					p_gl_wcs_modal_redraw);
// 
// 				p_gl_wcs_modal_redraw->Redraw();
// 
// 				p_gl_wcs_modal_redraw->Release();
			}

			if (!flag && _p_fem_render != NULL)
			{
				_clear_connect();
				_clear_fem_render_obj();
			}
		}

		void Export(const char *fname)
		{
			if (_p_fem_core && _is_nastran_file(fname)) 
			{
				TFactory factory;

				CREATE_COMX_OBJ(
					factory,
					CLSID_IFemSerialize,
					IFemSerialize,
					p_fem_serialize);
				assert(p_fem_serialize);

				TConnectManager cmgr;
				cmgr.AddConnect(p_fem_serialize, IID_IFemCore, _p_fem_core);

				p_fem_serialize->Export(fname);

				cmgr.ReleaseConnects();
				p_fem_serialize->Release();
			}
		}

		bool GetFemRender(IFemRender *&p_fem_render)
		{
			p_fem_render = _p_fem_render;

			return (p_fem_render != NULL);
		}
		bool GetFemCore(IFemCore *&p_fem_core)
		{
			p_fem_core = _p_fem_core;

			return (p_fem_core != NULL);
		}
	public:
		virtual TStatus STDCALL UpdateUI(const int &step, const char *msg)
		{
			if (_pfn_update_ui != NULL &&
				(*_pfn_update_ui)(step, msg) == true) 
			{
				return M_OK;
			}
			else
			{
				return M_FAIL;
			}
		}
		virtual TStatus STDCALL Startup(bool is_ind = false)
		{
			return M_OK;
		}
		virtual TStatus STDCALL Closure()
		{
			return M_OK;
		}
	};

	inline gl_wcs_func_t* COMX_GetGlWCSFunction()
	{
		gl_wcs_func_t *pfn_gl_wcs = NULL;

		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IGlWCS,
			IGlWCSFunc,
			p_gl_wcs_func);
			
		if (p_gl_wcs_func != NULL)
		{
			p_gl_wcs_func->GetFunc(pfn_gl_wcs);
			p_gl_wcs_func->Release();
		}

		return pfn_gl_wcs;
	}

	class TGlModuleSpace
	{
	public:
		TGlModuleSpace()
		{
			double xmin = 1e10, ymin = 1e10, zmin = 1e10;
			double xmax = -1e10, ymax = -1e10, zmax = -1e10;
			COMX_GetGlWCSFunction()->
				fn_get_range(xmin, ymin, zmin, xmax, ymax, zmax);
			
			double scale = max(xmax - xmin , ymax - ymin);
			scale = max(scale , zmax - zmin);
			scale *= 1.25;
			
			double center_x = (xmin + xmax) / 2.0;
			double center_y = (ymin + ymax) / 2.0;
			double center_z = (zmin + zmax) / 2.0;
			
			glPushMatrix();
			glScaled(1.0/scale, 1.0/scale, 1.0/scale);
			glTranslated(-center_x, -center_y, -center_z);
		}
		~TGlModuleSpace()
		{
			glPopMatrix();
		}
	};

// 	class TFemCoreGrid
// 	{
// 	private:
// 		TFemCoreCacheBase *_p_cache;
// 		unsigned int _density;
// 	public:
// 		TFemCoreGrid(
// 			TFemCoreCacheBase *p_cache, 
// 			unsigned int density = 20)
// 		{
// 			_p_cache = p_cache;
// 			_density = density;
// 		}
// 	};
	inline void KMAS_RefreshScreen()
	{
		IGlContentDriver* p_content_driver = NULL;
		TFactory factory;
		factory.QueryService(CLSID_IGlContent, IID_IGlContentDriver, (void**)&p_content_driver);
		assert(p_content_driver);
		p_content_driver->PaintEvent();
		p_content_driver->Release();
	}

#define COMX_RefreshScreen KMAS_RefreshScreen

	inline void COMX_GetPackageProp(char *name, void **ppv)
	{
		TFactory factory;
		//QUERY_SERVICE(factory, CLSID_IPlugServer, IProperty, p_prop);

		KMAS::Die_maker::comx::IProperty *p_prop = NULL;
		factory.QueryService(CLSID_IPlugServer, KMAS::Die_maker::comx::IID_IProperty, (void**)&p_prop);
		
		p_prop->GetProperty(name, ppv);
		
		p_prop->Release();
	}
}//namespace util
}//namespace KMAS

#endif
