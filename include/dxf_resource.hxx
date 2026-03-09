#include <comx_napi.hxx>

#include <type_cast.hxx>
using namespace KMAS::type;

#include <string>
#include <vector>

#ifndef _DXF_RESOURCE_7F584AC9CF4D4582B1B3279E347021A2
#define _DXF_RESOURCE_7F584AC9CF4D4582B1B3279E347021A2

namespace comx {
	namespace dxf {

		typedef bool(*functor_get_color_by_index_t)(int index, int &r, int &g, int &b);

		struct IDxfResource
		{
			virtual functor_get_color_by_index_t GetColorFunctor() = 0;
		};

		namespace A24E09264E8AD42CB930C6C76ED898C95{
			inline IDxfResource* _GetInf(Napi::Env *pEnv = NULL)
			{
				static IDxfResource *s_ptrDxfResourceInf = NULL;
				if (s_ptrDxfResourceInf) return s_ptrDxfResourceInf;

				if (pEnv == NULL) return NULL;

				Napi::Value result = comx::napi::InvokeGlobalFunction(*pEnv, { "comx", "dxf_resource", "GetInterface" }, {});
				if (result.IsUndefined()) return NULL;

				std::string strInf = result.ToString().Utf8Value();

				unsigned long long ullInf = type_cast<unsigned long long>(strInf);

				s_ptrDxfResourceInf = (IDxfResource*)((void*)ullInf);
				return s_ptrDxfResourceInf;
			}
		}
		
		inline bool InitInterface(Napi::Env &env)
		{
			return A24E09264E8AD42CB930C6C76ED898C95::_GetInf(&env) != NULL;
		}

		inline bool GetColorByIndex(int index, int &r, int &g, int &b)
		{
			static functor_get_color_by_index_t entry = NULL;

			if (entry == NULL)
			{
				IDxfResource *inf = A24E09264E8AD42CB930C6C76ED898C95::_GetInf();
				if (inf == NULL) return false;

				entry = inf->GetColorFunctor();
			}
			if (entry == NULL) return false;

			return (*entry)(index, r, g, b);
		}

}//dxf namespace
}//comx namespace
#endif
