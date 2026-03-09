/*HEAD gl_data_base HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change
10/01/2008      XK Zhang        Created
$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_GL_DATA_BASE_H_200810104859
#define _KMAS_COMX_GL_DATA_BASE_H_200810104859

#include "base/root.hxx"

#include "base/tag_code.hxx"
#include "base/tag_util.hxx"
#include "base/factory.hxx"
#include "gl/gl_content.hxx"
#include <gl/vtk_context.hxx>

#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum gl_db_render_mode_t
{
    GL_COMMON_RENDER_MODE,
    GL_HIGHTLIGHT_RENDER_MODE,
    GL_MARK_RENDER_MODE
};

enum gl_db_polygon_mode_t
{
    GL_POLYGON_WIRE,
    GL_POLYGON_FACE,
    GL_POLYGON_VERTEX
};

extern "C"
{
  struct gl_db_record_fun_t
  {
    tag_t (*get_key_tag)(const tag_t tag_rec);
    tag_t (*get_dl_id)(const tag_t tag_rec);
    gl_db_render_mode_t (*get_render_mode)(const tag_t tag_rec);
    void (*set_render_mode)(
			    const tag_t tag_rec, 
			    const gl_db_render_mode_t mode);
    void  (*get_color)(
		       const tag_t tag_rec, 
		       double &r, 
		       double &g, 
		       double &b, 
		       double &a);
    void  (*set_color)(
		       const tag_t tag_rec, 
		       const double r, 
		       const double g, 
		       const double b, 
		       const double a);
    void  (*get_high_light_color)(
				  const tag_t tag_rec, 
				  double &r, 
				  double &g, 
				  double &b, 
				  double &a);
    void  (*set_high_light_color)(
				  const tag_t tag_rec, 
				  const double r, 
				  const double g, 
				  const double b, 
				  const double a);
    void  (*get_mark_color)(
			    const tag_t tag_rec, 
			    double &r, 
			    double &g, 
			    double &b, 
			    double &a);
    void  (*set_mark_color)(
			    const tag_t tag_rec, 
			    const double r, 
			    const double g, 
			    const double b, 
			    const double a);
    gl_db_polygon_mode_t (*get_polygon_mode)(const tag_t tag_rec);
    void (*set_polygon_mode)(
			     const tag_t tag_rec, 
			     const gl_db_polygon_mode_t pmode);
    double (*get_line_width)(const tag_t tag_rec);
    void (*set_line_width)(const tag_t tag_rec, const double width);
    double (*get_point_size)(const tag_t tag_rec);
    void (*set_point_size)(const tag_t tag_rec, const double size);
    bool   (*is_light)(const tag_t tag_rec);
    void   (*enable_light)(const tag_t tag_rec, bool flag/* = true*/);
    bool   (*is_blend)(const tag_t tag_rec);
    void   (*enable_blend)(const tag_t tag_rec, bool flag/* = true*/);
    bool   (*is_visible)(const tag_t tag_rec);
    void   (*enable_visible)(const tag_t tag_rec, bool flag/* = true*/);
    bool   (*is_bosseyed)(const tag_t tag_rec);
    void   (*enable_bosseyed)(const tag_t tag_rec, bool flag/* = true*/);
    bool   (*is_plot_mode)(const tag_t tag_rec);
    void   (*enable_plot_mode)(const tag_t tag_rec, bool flag/* = true*/);
    
    void   (*clear_list)(tag_t list_id);
    void   (*begin_list)(tag_t list_id);
    void   (*end_list)();
    void   (*begin_primitive)(GLenum mode);
    void   (*end_primitive)();
    void   (*input_vertex)(const double x, const double y, const double z);
    void   (*input_normal)(const double x, const double y, const double z);
    bool   (*is_primitive_pickup_mode)(const tag_t tag_rec);
    void   (*enable_primitive_pickup_mode)(const tag_t tag_rec, bool flag/* = true*/);
    void   (*input_property)(const unsigned int id);
    
    void   (*set_primitive_render_mode)(
				      const tag_t tag_rec,
				      const GLuint primitive_id,
				      const gl_db_render_mode_t mode);
    
    void    (*input_vertex_ex)(const double x, const double y, const double z);
    void    (*input_normal_ex)(const double x, const double y, const double z);
    void    (*input_property_ex)(const unsigned int id);
    void    (*input_indice_ex)(const unsigned int indice);

    void    (*set_transform)(const tag_t tag_rec, double matrix[16]);
    double* (*get_transform)(const tag_t tag_rec);

    void    (*clone_record)(const tag_t tag_rec, uint64_t sid, double *matrix);
    void    (*delete_record)(const tag_t tag_rec);
  };
}
struct picker_t
{
        int     mode;
        bool    autoHighlight;
        int     behavior;
};

// {ADE08891-4333-472a-8A3D-B282510CA180}
 const MUID IID_IGlDataBase = 
{ 0xade08891, 0x4333, 0x472a, { 0x8a, 0x3d, 0xb2, 0x82, 0x51, 0xc, 0xa1, 0x80 } };

comx_interface IGlDataBase : public IRoot
{
    virtual TStatus STDCALL AppendGlRecord(const tag_t tag_key) = 0;
    virtual TStatus STDCALL DeleteGlRecord(const tag_t tag_key) = 0;
    virtual TStatus STDCALL IsExistGlRecord(const tag_t tag_key, bool &is_exist) = 0;
    virtual TStatus STDCALL GetGlFirstRecord(tag_t &tag_rec) = 0;
    virtual TStatus STDCALL GetGlNextRecord(tag_t &tag_rec, bool &is_end) = 0;
    virtual TStatus STDCALL GetGlRecord(const tag_t tag_key, tag_t &tag_rec) = 0;
    virtual TStatus STDCALL GetGlRecordFuncs(gl_db_record_fun_t &gdrf) = 0;
};

// {13E7E074-F1C2-4efc-92AB-08BF181B91AE}
const MUID IID_IGlDataBase2 = 
{ 0x13e7e074, 0xf1c2, 0x4efc, { 0x92, 0xab, 0x8, 0xbf, 0x18, 0x1b, 0x91, 0xae } };

comx_interface IGlDataBase2 : public IGlDataBase
{
	virtual TStatus STDCALL GetRecordCount(int &cnt) = 0;
	virtual TStatus STDCALL RenderPickup(const int code) = 0;
};

// {6EB2F691-90A8-4db3-BDCB-A469FCE665EC}
const MUID IID_IGlDataBase3 = 
{ 0x6eb2f691, 0x90a8, 0x4db3, { 0xbd, 0xcb, 0xa4, 0x69, 0xfc, 0xe6, 0x65, 0xec } };

comx_interface IGlDataBase3 : public IGlDataBase2
{
	virtual TStatus STDCALL EnableLight(const bool flag = true) = 0;
	virtual TStatus STDCALL CleanDrawList() = 0;
};

// {62073E2A-66B1-4442-ADFC-6BD199ED36EE}
const MUID IID_IGlDataBase4 = 
{ 0x62073e2a, 0x66b1, 0x4442, { 0xad, 0xfc, 0x6b, 0xd1, 0x99, 0xed, 0x36, 0xee } };

comx_interface IGlDataBase4 : public IGlDataBase3
{
	virtual TStatus STDCALL EnableCADRender(bool flag) = 0;
};

// {2D5775CC-C314-4d28-920B-A6A969D5CECE}
const MUID IID_IGlDataBase5 = 
{ 0x2d5775cc, 0xc314, 0x4d28, { 0x92, 0xb, 0xa6, 0xa9, 0x69, 0xd5, 0xce, 0xce } };

comx_interface IGlDataBase5 : public IGlDataBase4
{
	virtual TStatus STDCALL EnableAutoRange(bool flag) = 0;
};

// {02FD8F05-29BA-43FC-B6E3-35C4E11DD87C}
const MUID IID_IGlDataBase6 = 
{ 0x2fd8f05, 0x29ba, 0x43fc, { 0xb6, 0xe3, 0x35, 0xc4, 0xe1, 0x1d, 0xd8, 0x7c } };

comx_interface IGlDataBase6 : public IGlDataBase5
{
    virtual TStatus STDCALL DrawPrimitives(tag_t *rec_arr, int *propid_arr, const int len, gl_db_render_mode_t render_mode) = 0;
};

// {E2D6A7F9-6289-4A09-B0F9-DA3C01272B10}
const MUID IID_IGlDataBase7 = 
{ 0xe2d6a7f9, 0x6289, 0x4a09, { 0xb0, 0xf9, 0xda, 0x3c, 0x1, 0x27, 0x2b, 0x10 } };

comx_interface IGlDataBase7 : public IGlDataBase6
{
	virtual TStatus STDCALL EnableLazyGroupMode(bool flag) = 0;
	virtual TStatus STDCALL UpdateGroupInfoByDirtyMode() = 0;
};

// {11302078-8E07-4416-A6E8-5D930D25951E}
const MUID IID_IGlDataBase8 = 
{ 0x11302078, 0x8e07, 0x4416, { 0xa6, 0xe8, 0x5d, 0x93, 0xd, 0x25, 0x95, 0x1e } };


comx_interface IGlDataBase8 : public IGlDataBase7
{
	virtual TStatus STDCALL Textout(TGlTextInfo &gi) = 0;
	virtual TStatus STDCALL TextoutClear() = 0;
	virtual TStatus STDCALL SetTextoutInterface(IGlText *p_gl_text) = 0;
	virtual TStatus STDCALL SetTextDisplayFlag(bool flag) = 0;
	virtual TStatus STDCALL SetTextColor(double r, double g, double b) = 0;
};

// {F80B7A0D-5287-4fde-BB0F-39314CCD7171}
const MUID IID_IGlDataBase9 = 
{ 0xf80b7a0d, 0x5287, 0x4fde, { 0xbb, 0xf, 0x39, 0x31, 0x4c, 0xcd, 0x71, 0x71 } };


comx_interface IGlDataBase9 : public IGlDataBase8
{
	virtual TStatus STDCALL EnableVisibleByCode(int code, bool flag) = 0;
};

// {34836A49-1D90-4C0F-94A9-5E859F5429E5}
const MUID IID_IGlDataBase10 = 
{ 0x34836a49, 0x1d90, 0x4c0f, { 0x94, 0xa9, 0x5e, 0x85, 0x9f, 0x54, 0x29, 0xe5 } };

comx_interface IGlDataBase10 : public IGlDataBase9
{
	virtual TStatus STDCALL CreatePlot(const float * const node_coords, const int nlen, const float * const color_buf) = 0;
	virtual TStatus STDCALL	ClearPlot() = 0;
};

// {9445AE24-5BD9-45B6-A5DA-21877E58C24E}
const MUID  IID_IGlDataBase11 =
{ 0x9445ae24, 0x5bd9, 0x45b6, { 0xa5, 0xda, 0x21, 0x87, 0x7e, 0x58, 0xc2, 0x4e } };

comx_interface IGlDataBase11 : public IGlDataBase10
{
        virtual TStatus STDCALL RenderPickupEx(int *codes, int len) = 0;
        virtual TStatus STDCALL PushViewportSize(int w, int h) = 0;
};

// {FB0BE8DC-584B-42B2-BE84-C0FFF7B1BA1C}
const MUID IID_IGlDataBase12 =
{ 0xfb0be8dc, 0x584b, 0x42b2, { 0xbe, 0x84, 0xc0, 0xff, 0xf7, 0xb1, 0xba, 0x1c } };

comx_interface IGlDataBase12 : public IGlDataBase11
{
        virtual TStatus STDCALL RenderPickupEx2(int *codes, int len, uint64_t *keys, int klen, picker_t picker) = 0;
        virtual TStatus STDCALL ReceiveMessage(const char *msg) = 0;
        virtual TStatus STDCALL IsNeedRepaint(bool &flag) = 0;
        virtual TStatus STDCALL SetVtkContext(IVtkContext *vtkContext) = 0;
        virtual TStatus STDCALL SetGlobalColor(double r, double g, double b, bool flag) = 0;
        virtual TStatus STDCALL GetModelParameters(double &cx, double &cy, double &cz, double &scale) = 0;
};

//virtual TStatus STDCALL ProjectModelPoint(double x, double y, double z, int &wx, int wy) = 0;
// {8F59342A-3301-49FB-8BAA-3AF22C12E749}
const MUID IID_IGlUtils =
{ 0x8f59342a, 0x3301, 0x49fb, { 0x8b, 0xaa, 0x3a, 0xf2, 0x2c, 0x12, 0xe7, 0x49 } };

comx_interface IGlUtils : public IRoot
{
    virtual TStatus STDCALL ProjectModelPoint(double x, double y, double z, int &wx, int &wy) = 0;
};

// {B447026B-D14B-4cf8-9893-C9301457AE14}
const MUID IID_IGlDataBaseEvent = 
{ 0xb447026b, 0xd14b, 0x4cf8, { 0x98, 0x93, 0xc9, 0x30, 0x14, 0x57, 0xae, 0x14 } };

comx_interface IGlDataBaseEvent : public IRoot
{
	virtual TStatus STDCALL OnBeginPaint(const tag_t tag_key) = 0;
	virtual TStatus STDCALL OnEndPaint(const tag_t tag_key) = 0;
};

// {C38EB256-1334-4dcb-9942-2DF0FDACB398}
 const MTYPEID CLSID_IGlDataBase = 
{ 0xc38eb256, 0x1334, 0x4dcb, { 0x99, 0x42, 0x2d, 0xf0, 0xfd, 0xac, 0xb3, 0x98 } };

class TGlDataBaseRecord
{
    tag_t _tag_rec;
    gl_db_record_fun_t _gdrf;
private:
    void _init_gl_db_record_funs()
    {
        TFactory factory;
        QUERY_SERVICE(
            factory,
            CLSID_IGlDataBase,
            IGlDataBase,
            p_gl_db);
        assert(p_gl_db);

        p_gl_db->GetGlRecordFuncs(_gdrf);

        p_gl_db->Release();
    }
  void _init_gl_db_record_funs(IGlDataBase *p_gl_db)
    {
//         TFactory factory;
//         QUERY_SERVICE(
//             factory,
//             CLSID_IGlDataBase,
//             IGlDataBase,
//             p_gl_db);
//         assert(p_gl_db);
		
        p_gl_db->GetGlRecordFuncs(_gdrf);
		
       //  p_gl_db->Release();
    }
public:
    TGlDataBaseRecord(const tag_t tag_rec)
    {
        _tag_rec = tag_rec;
        _init_gl_db_record_funs();
    }
	TGlDataBaseRecord(const	tag_t tag_rec, IGlDataBase *p_gl_db)
	{
		_tag_rec = tag_rec;
		_init_gl_db_record_funs(p_gl_db);
	}
public:
    operator tag_t()
    {
        return _tag_rec;
    }
public:
    tag_t GetKeyTag(){return _gdrf.get_key_tag(_tag_rec);}
    tag_t GetDisplayListID(){return _gdrf.get_dl_id(_tag_rec);}
    gl_db_render_mode_t GetRenderMode(){return _gdrf.get_render_mode(_tag_rec);}
    void  SetRenderMode(const gl_db_render_mode_t mode)
    {
        _gdrf.set_render_mode(_tag_rec, mode);
    }
    void  GetColor(double &r, double &g, double &b, double &a)
    {
        _gdrf.get_color(_tag_rec, r, g, b, a);
    }
    void  SetColor(
        const double r, 
        const double g, 
        const double b, 
        const double a)
    {
        _gdrf.set_color(_tag_rec, r, g, b, a);
    }
    void GetHighLightColor(double &r, double &g, double &b, double &a)
    {
        _gdrf.get_high_light_color(_tag_rec, r, g, b, a);
    }
    void SetHighLightColor(
        const double r,
        const double g,
        const double b,
        const double a)
    {
        _gdrf.set_high_light_color(_tag_rec, r, g, b, a);
    }
    void GetMarkColor(double &r, double &g, double &b, double &a)
    {
        _gdrf.get_mark_color(_tag_rec, r, g, b, a);
    }
    void SetMarkColor(
        const double r,
        const double g,
        const double b,
        const double a)
    {
        _gdrf.set_mark_color(_tag_rec, r, g, b, a);
    }
    gl_db_polygon_mode_t GetPolygonMode()
    {
        return _gdrf.get_polygon_mode(_tag_rec);
    }
    void SetPolygonMode(const gl_db_polygon_mode_t mode)
    {
        _gdrf.set_polygon_mode(_tag_rec, mode);
    }
    double GetLineWidth()
    {
        return _gdrf.get_line_width(_tag_rec);
    }
    void SetLineWidth(const double width)
    {
        _gdrf.set_line_width(_tag_rec, width);
    }
    double GetPointSize()
    {
        return _gdrf.get_point_size(_tag_rec);
    }
    void SetPointSize(const double size)
    {
        _gdrf.set_point_size(_tag_rec, size);
    }
    bool IsLight()
    {
        return _gdrf.is_light(_tag_rec);
    }
    void EnableLight(const bool flag)
    {
        _gdrf.enable_light(_tag_rec, flag);
    }
    bool IsBlend()
    {
        return _gdrf.is_blend(_tag_rec);
    }
    void EnableBlend(const bool flag)
    {
        _gdrf.enable_blend(_tag_rec, flag);
    }
    bool IsVisible()
    {
        return _gdrf.is_visible(_tag_rec);
    }
    void EnableVisible(const bool flag)
    {
        _gdrf.enable_visible(_tag_rec, flag);
    }
    bool IsBosseyed()
    {
        return _gdrf.is_bosseyed(_tag_rec);
    }
    void EnableBosseyed(const bool flag)
    {
        _gdrf.enable_bosseyed(_tag_rec, flag);
    }
	bool IsPlotMode()
	{
		return _gdrf.is_plot_mode(_tag_rec);
	}
	void EnablePlotMode(const bool flag)
	{
		_gdrf.enable_plot_mode(_tag_rec, flag);
	}
	void  ClearList(tag_t list_id)
	{
		_gdrf.clear_list(list_id);
	}
	void  BeginList(tag_t list_id)
	{
		_gdrf.begin_list(list_id);
	}
	void  EndList()
	{
		_gdrf.end_list();
	}
	void  BeginPrimitive(GLenum mode)
	{
		_gdrf.begin_primitive(mode);
	}
	void  EndPrimitive()
	{
		_gdrf.end_primitive();
	}
	void  InputVertex(const double x, const double y, const double z)
	{
		_gdrf.input_vertex(x, y, z);
	}
    void  InputVertexEx(const double x, const double y, const double z)
    {
        _gdrf.input_vertex_ex(x, y, z);
    }
	void  InputNormal(const double x, const double y, const double z)
	{
		_gdrf.input_normal(x, y, z);
	}
    void  InputNormalEx(const double x, const double y, const double z)
    {
        _gdrf.input_normal_ex(x, y, z);
    }
    bool IsPrimitivePickupMode()
    {
        return _gdrf.is_primitive_pickup_mode(_tag_rec);
    }
    void EnablePrimitivePickupMode(const bool flag)
    {
        _gdrf.enable_primitive_pickup_mode(_tag_rec, flag);
    }
    void InputProperty(const unsigned int id)
    {
        _gdrf.input_property(id);
    }
    void InputPropertyEx(const unsigned int id)
    {
        _gdrf.input_property_ex(id);
    }
    void SetPrimitiveRenderMode( const tag_t tag_rec, const GLuint primitive_id, const gl_db_render_mode_t mode)
    {
        _gdrf.set_primitive_render_mode(tag_rec, primitive_id, mode);
    }
    void InputIndiceEx(const unsigned int indice)
    {
        _gdrf.input_indice_ex(indice);
    }

    void SetTransorm(double matrix[16])
    {
            _gdrf.set_transform(_tag_rec, matrix);
    }

    double *GetTransform()
    {
            return _gdrf.get_transform(_tag_rec);
    }

    void CloneRecord(uint64_t sid, double *matrix = NULL)
    {
            _gdrf.clone_record(_tag_rec, sid, matrix);
    }

    void Dispose()
    {
            _gdrf.delete_record(_tag_rec);
    }

    /*gdrf.set_transform = set_transform;
    gdrf.get_transform = get_transform;
    gdrf.clone_record = clone_record;
    gdrf.delete_record = delete_record;*/
};

class TGlDataBase
{
	IGlDataBase *p_gl_data_base;
public:
	TGlDataBase(IRoot *v_p_gl_db)
	{
		assert(v_p_gl_db != NULL);
		v_p_gl_db->QueryInterface(IID_IGlDataBase, (void**)&p_gl_data_base);
	}
	TGlDataBase()
	{
		TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_IGlDataBase,
			IGlDataBase,
			p_gl_db);

		p_gl_data_base = NULL;

		if (p_gl_db != NULL) 
		{
			p_gl_data_base = p_gl_db;
			p_gl_data_base->AddRef();
	
			p_gl_db->Release();
		}
	}
	~TGlDataBase()
	{
		if(p_gl_data_base) 
		{
			p_gl_data_base->Release();
		}
	}
public:
	void AppendRecord(const tag_t tag_key)
	{
		p_gl_data_base->AppendGlRecord(tag_key);
	}
	void DeleteRecord(const tag_t tag_key)
	{
		p_gl_data_base->DeleteGlRecord(tag_key);
	}
	bool IsExistRecord(const tag_t tag_key)
	{
		bool is_exist = false;
		p_gl_data_base->IsExistGlRecord(tag_key, is_exist);

		return is_exist;
	}
	void GetFirstRecord(tag_t &tag_rec)
	{
		p_gl_data_base->GetGlFirstRecord(tag_rec);
	}
	void GetNextRecord(tag_t &tag_rec, bool &is_end)
	{
		p_gl_data_base->GetGlNextRecord(tag_rec, is_end);
	}
	tag_t GetRecord(const tag_t tag_key)
	{
		tag_t tag_rec;
		p_gl_data_base->GetGlRecord(tag_key, tag_rec);

		return tag_rec;
	}
	int GetRecordCount()
	{
		QUERY_INTERFACE(
			p_gl_data_base,
			IGlDataBase2,
			p_gl_data_base2);

		int cnt = 0;
		if (p_gl_data_base2)
		{
			p_gl_data_base2->GetRecordCount(cnt);
			p_gl_data_base2->Release();
		}

		return cnt;
	}

	void RenderPickup(int code)
	{
		QUERY_INTERFACE(
			p_gl_data_base,
			IGlDataBase2,
			p_gl_data_base2);

		if (p_gl_data_base2)
		{
			p_gl_data_base2->RenderPickup(code);
			p_gl_data_base2->Release();
		}
	}

	void CleanDrawList()
	{
		QUERY_INTERFACE(
			p_gl_data_base,
			IGlDataBase3,
			p_gl_data_base3);
		
		if (p_gl_data_base3)
		{
			p_gl_data_base3->CleanDrawList();
			p_gl_data_base3->Release();
		}
	}
public:
	void CreatePlot(const float * const node_coords, const int nlen, const float * const color_buf)
	{
		QUERY_INTERFACE(
			p_gl_data_base,
			IGlDataBase10,
			p_gl_data_base10);

		if (p_gl_data_base10)
		{
			p_gl_data_base10->CreatePlot(node_coords, nlen, color_buf);
			p_gl_data_base10->Release();
		}
	}
	void ClearPlot()
	{
		QUERY_INTERFACE(
			p_gl_data_base,
			IGlDataBase10,
			p_gl_data_base10);

		if (p_gl_data_base10)
		{
			p_gl_data_base10->ClearPlot();
			p_gl_data_base10->Release();
		}
	}
};

/*
class oglstream
{
	TGlDataBase gl_db;
	int m_iContent;
	tag_t m_tagCurrentRecord;
public:
	struct record
	{
		int key;
		int val;
	public:
		record(int k, int v) : key(k), val(v){}
	};
	struct color
	{
		double red;
		double green;
		double blue;
		double alpha;
	public:
		color(double r, double g, double b, double a) : red(r), green(g), blue(b), alpha(a){}
	};
	struct vertex
	{
		double x, y, z;
		double nx, ny, nz;
	public:
		vertex(double _x, double _y, double _z, double _nx = 0, double _ny = 0, double _nz = 1) 
			: x(_x), y(_y), z(_z), nx(_nx), ny(_ny), nz(_nz){}
	};
	struct light
	{
		bool flag;
	public:
		light(bool _flag) : flag(_flag){}
	};

	struct visible
	{
		bool flag;
	public:
		visible(bool _flag) : flag(_flag){}
	};
	enum gl_flags_t
	{
		EMPTY,
		BEGIN,
		CLOSE,
		TRIANGLES,
		LINES,
		END,
		WIRE,
		FACE
	};
	enum status_t
	{
		IDLE,
		RECORD,
		PRIMITIVE
	};
	enum primitive_t
	{
		TRIANGLE,
		LINE,
		NIL_PRIMITIVE
	};
private:
	status_t _status;
	vector<vertex> _vertexs;
	primitive_t _primitive_type;
public:
	oglstream(int iContent, bool bAutoEmpty = false) : gl_db((IGlDataBase*)iContent), m_iContent(iContent), m_tagCurrentRecord(0)
	{
		if (bAutoEmpty)
		{
			empty();
		}

		_status = IDLE;
		_primitive_type = NIL_PRIMITIVE;
	}
private:
	void empty()
	{
		//Clear old opengl cache.
		if (CurrentTagKeys().find(m_iContent) != CurrentTagKeys().end())
		{
			map<int, set<tag_t> >::iterator iter = CurrentTagKeys().find(m_iContent);

			set<tag_t> &current_tag_set = iter->second;
			for (set<tag_t>::iterator iter_tag = current_tag_set.begin(); iter_tag != current_tag_set.end(); ++iter_tag)
			{
				gl_db.DeleteRecord(*iter_tag);
			}

			CurrentTagKeys().erase(iter);
		}
		
		gl_db.CleanDrawList();
	}
	int get_dl_id()
	{
		assert(m_tagCurrentRecord != 0);

		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
		return gl_db_rec.GetDisplayListID();
	}
	void begin_draw()
	{
		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
		int dl_id = get_dl_id();
		
		gl_db_rec.BeginList(dl_id);
	}
	void disable_light()
	{
		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
		gl_db_rec.EnableLight(false);
	}
public:
	oglstream &operator<<(gl_flags_t flag)
	{
		if (flag == EMPTY && _status == IDLE)
		{
			empty();
		}

		if (flag == BEGIN)
		{
			//begin_draw();
		}

		if (flag == CLOSE && _status == RECORD)
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
			gl_db_rec.EndList();

			_status = IDLE;
		}

		if (flag == TRIANGLES && _status == RECORD)
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
			gl_db_rec.BeginPrimitive(GL_TRIANGLES);

			_status = PRIMITIVE;
			_primitive_type = TRIANGLE;
		}

		if (flag == END && _status == PRIMITIVE)
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
			
			_status = RECORD;
			if (_primitive_type != NIL_PRIMITIVE)
			{
				if (_primitive_type == TRIANGLE)
				{
					for (unsigned int loop = 0; loop < _vertexs.size() / 3; ++loop)
					{
						vertex v1 = _vertexs[loop * 3 + 0];
						vertex v2 = _vertexs[loop * 3 + 1];
						vertex v3 = _vertexs[loop * 3 + 2];

						gl_db_rec.InputVertex(v1.x, v1.y, v1.z);
						gl_db_rec.InputNormal(v1.nx, v1.ny, v1.nz);

						gl_db_rec.InputVertex(v2.x, v2.y, v2.z);
						gl_db_rec.InputNormal(v2.nx, v2.ny, v2.nz);

						gl_db_rec.InputVertex(v3.x, v3.y, v3.z);
						gl_db_rec.InputNormal(v3.nx, v3.ny, v3.nz);
					}
				}

				if (_primitive_type == LINE)
				{
					for (unsigned int loop = 0; loop < _vertexs.size() / 2; ++loop)
					{
						vertex v1 = _vertexs[loop * 2 + 0];
						vertex v2 = _vertexs[loop * 2 + 1];
						
						gl_db_rec.InputVertex(v1.x, v1.y, v1.z);
						gl_db_rec.InputNormal(v1.nx, v1.ny, v1.nz);
						
						gl_db_rec.InputVertex(v2.x, v2.y, v2.z);
						gl_db_rec.InputNormal(v2.nx, v2.ny, v2.nz);
					}
				}

				_vertexs.clear();

				_primitive_type = NIL_PRIMITIVE;
			}

			gl_db_rec.EndPrimitive();
		}

		if (flag == LINES && _status == RECORD)
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
			gl_db_rec.BeginPrimitive(GL_LINES);

			_status = PRIMITIVE;
			_primitive_type = LINE;
		}

		if (flag == WIRE && _status == IDLE)
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
			gl_db_rec.SetPolygonMode(GL_POLYGON_WIRE);
		}

		if (flag == FACE && _status == IDLE)
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
			gl_db_rec.SetPolygonMode(GL_POLYGON_FACE);
		}

		return *this;
	}

	oglstream &operator<<(record rec)
	{
		KMAS::tag_util::TTagUtil tag_obj(rec.key, (void*)rec.val);
		m_tagCurrentRecord = tag_obj;
		
		//int dl_id = 0;
		
		if (gl_db.IsExistRecord(tag_obj))
		{
			TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(tag_obj), (IGlDataBase*)m_iContent);
			gl_db_rec.ClearList(gl_db_rec.GetDisplayListID());
		}
		else
		{
			gl_db.AppendRecord(tag_obj);
			//S_cur_tag_keys.push_back(tag_obj);
			CurrentTagKeys()[m_iContent].insert(tag_obj);
		}

		*this << WIRE;

		begin_draw();

		_status = RECORD;
		
		return *this;
	}

	oglstream &operator<<(color clr)
	{
		if (_status != IDLE)
		{
			return *this;
		}

		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
		gl_db_rec.SetColor(clr.red, clr.green, clr.blue, clr.alpha);

		return *this;
	}

	oglstream &operator<<(light li)
	{
		if (_status != IDLE)
		{
			return *this;
		}
		
		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
		gl_db_rec.EnableLight(li.flag);

	
		return *this;
	}

	oglstream &operator<<(visible li)
	{
		if (_status != IDLE)
		{
			return *this;
		}
		
		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
		gl_db_rec.EnableVisible(li.flag);
		
		
		return *this;
	}

	oglstream &operator<<(vertex v)
	{
		if (_status != PRIMITIVE)
		{
			return *this;
		}

// 		TGlDataBaseRecord gl_db_rec(gl_db.GetRecord(m_tagCurrentRecord), (IGlDataBase*)m_iContent);
// 		gl_db_rec.InputVertex(v.x, v.y, v.z);
// 		gl_db_rec.InputNormal(v.nx, v.ny, v.nz);

		_vertexs.push_back(v);

		return *this;
	}
private:
	static map<int, set<tag_t> > &CurrentTagKeys()
	{
		static map<int, set<tag_t> > ms_mapCurrentTagKeys;
		return ms_mapCurrentTagKeys;
	}
};
*/

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif


