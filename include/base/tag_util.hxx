#ifndef _KMAS_COMX_BASE_TAG_UTIL_HXX
#define _KMAS_COMX_BASE_TAG_UTIL_HXX

#include <base/tag_pool.hxx>
#include <base/factory.hxx>

using namespace KMAS::Die_maker::comx;

namespace KMAS{
namespace tag_util{

namespace _wrap{
	inline KMAS::Die_maker::comx::ITagPoolEx *get_tag_pool_ex()
	{
		KMAS::Die_maker::comx::TFactory factory;
		QUERY_SERVICE(
			factory,
			CLSID_ITagPool,
			ITagPoolEx,
			p_tag_pool_ex);

		p_tag_pool_ex->Release();

		return p_tag_pool_ex;
	}

	inline KMAS::Die_maker::comx::tag_pool_fun_t get_pool_fun()
	{
                static KMAS::Die_maker::comx::tag_pool_fun_t tpf;
                static bool init = false;

		//KMAS::Die_maker::comx::tag_pool_fun_t tpf;
                if (!init)
                {
                        init = true;
                        get_tag_pool_ex()->GetTagPoolFuncs(tpf);
                }
		
		return tpf;
	}
}// end namespace _wrap.

inline tag_t create_tag(const int code, void* val)
{
	return (*_wrap::get_pool_fun().pfn_create_tag)(code, val);
}

inline void  delete_tag(const tag_t tag)
{
	(*_wrap::get_pool_fun().pfn_delete_tag)(tag);
}

inline int get_tag_code(const tag_t tag)
{
	return (*_wrap::get_pool_fun().pfn_get_tag_code)(tag);
}

inline void* get_tag_val(const tag_t tag)
{
	return (*_wrap::get_pool_fun().pfn_get_tag_val)(tag);
}

inline bool is_value_exist(const int code, const void *val)
{
	return (*_wrap::get_pool_fun().pfn_is_value_exist)(code, val);
}

inline bool is_tag_exist(const tag_t tag)
{
	return (*_wrap::get_pool_fun().pfn_is_tag_exist)(tag);
}

class TTagUtil
{
private:
	tag_t _tag;
	bool _auto_release;
public:
	TTagUtil(const int code, void* val, bool auto_release = false)
	{
		_tag = create_tag(code, val);
		_auto_release = auto_release;

		assert(is_tag_exist(_tag));
	}
	TTagUtil(const tag_t tag, bool auto_release = false)
	{
		assert(is_tag_exist(tag));
		_tag = tag;
		_auto_release = auto_release;
	}
	~TTagUtil()
	{
		if (_auto_release) 
		{
			delete_tag(_tag);
		}
	}
public:
	int GetCode()
	{
		return get_tag_code(_tag);
	}
	void* GetValue()
	{
		return get_tag_val(_tag);
	}
public:
	operator tag_t()
	{
		return _tag;
	}
};

}// end namespace tag_util.
}// end namespace KMAS.

#endif
