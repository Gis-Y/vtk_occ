#ifndef COMX_FLEXER_HXX_51004D384D794B17B23C77CF89CE8F05
#define COMX_FLEXER_HXX_51004D384D794B17B23C77CF89CE8F05

#if defined(_WINDOWS) || defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#include <cstddef>

#endif

#include <assert.h>

class TComxFlexer
{
private:
    typedef void  (*COMX_LEXER_FlexEntry_t)(const char *input_file);
    typedef void  (*COMX_LEXER_RegisterCallback_t)(const char *key,
		      bool (*cb)(int argc, char** argv));
    typedef void  (*COMX_LEXER_FlexEntryEx_t)(const char *input_file, const char *output_file);
    typedef void  (*COMX_LEXER_RegisterReplaceCallback_t)(const char *key,
	  	      bool (*cb)(int argc, char **argv, int index, char *&res));
private:
#if defined(_WINDOWS) || defined(WIN32)
    HINSTANCE m_hInstLib;
#else
  void       *m_hInstLib;
#endif
    COMX_LEXER_FlexEntry_t m_fnFlexEntry;
    COMX_LEXER_RegisterCallback_t m_fnRegisterCallback;
    COMX_LEXER_FlexEntryEx_t m_fnFlexEntryEx;
    COMX_LEXER_RegisterReplaceCallback_t m_fnRegisterReplaceCallback;
public:
    TComxFlexer(const char *dll_fpath)
    {
#if defined(_WINDOWS) || defined(WIN32)
        m_hInstLib = LoadLibrary(dll_fpath);
        assert(m_hInstLib != NULL);

        m_fnFlexEntry = (COMX_LEXER_FlexEntry_t)GetProcAddress(m_hInstLib,
	    "COMX_LEXER_FlexEntry");
        assert(m_fnFlexEntry != NULL);

        m_fnRegisterCallback = (COMX_LEXER_RegisterCallback_t)GetProcAddress(m_hInstLib,
	    "COMX_LEXER_RegisterCallback");
        assert(m_fnRegisterCallback != NULL);

        m_fnFlexEntryEx = (COMX_LEXER_FlexEntryEx_t)GetProcAddress(m_hInstLib,
	    "COMX_LEXER_FlexEntryEx");
        m_fnRegisterReplaceCallback = (COMX_LEXER_RegisterReplaceCallback_t)
	    GetProcAddress(m_hInstLib, "COMX_LEXER_RegisterReplaceCallback");
#else
	m_hInstLib = dlopen(dll_fpath, RTLD_LAZY);
	assert(m_hInstLib != NULL);

	m_fnFlexEntry = (COMX_LEXER_FlexEntry_t)dlsym(m_hInstLib, "COMX_LEXER_FlexEntry");
        assert(m_fnFlexEntry != NULL);

	m_fnRegisterCallback = (COMX_LEXER_RegisterCallback_t)
	    dlsym(m_hInstLib, "COMX_LEXER_RegisterCallback");
        assert(m_fnRegisterCallback != NULL);

        m_fnFlexEntryEx = (COMX_LEXER_FlexEntryEx_t)dlsym(m_hInstLib, "COMX_LEXER_FlexEntryEx");
        m_fnRegisterReplaceCallback = (COMX_LEXER_RegisterReplaceCallback_t)
	    dlsym(m_hInstLib, "COMX_LEXER_RegisterReplaceCallback");
#endif
    }
    ~TComxFlexer()
    {
#if defined(_WINDOWS) || defined(WIN32)
        BOOL ret = FreeLibrary(m_hInstLib);
        assert(ret == TRUE);

        m_hInstLib = NULL;
#else
	dlclose(m_hInstLib);
	m_hInstLib = NULL;
#endif
    }
public:
    void ExecuteFlexParse(const char *target_fpath)
    {
        assert(m_fnFlexEntry != NULL);
        (*m_fnFlexEntry)(target_fpath);
    }
    void RegisterCallback(const char *key, bool (*cb)(int argc, char** argv))
    {
        assert(m_fnRegisterCallback != NULL);
        (*m_fnRegisterCallback)(key, cb);
    }
    void ExecuteFlexParseEx(const char *target_fpath, const char *output_fpath)
    {
        assert(m_fnFlexEntryEx != NULL);
        (*m_fnFlexEntryEx)(target_fpath, output_fpath);
    }
    void RegisterReplaceCallback(const char *key,
				 bool (*cb)(int argc, char **argv, int index, char *&res))
    {
        assert(m_fnRegisterReplaceCallback != NULL);
        (*m_fnRegisterReplaceCallback)(key, cb);
    }
};

#endif
