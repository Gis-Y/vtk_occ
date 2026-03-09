
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBXLST_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBXLST_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBXLST_EXPORTS
#define LIBXLST_API __declspec(dllexport)
#else
#define LIBXLST_API __declspec(dllimport)
#endif

LIBXLST_API bool LIBXSLT_Initialize(const char *ns);
LIBXLST_API void LIBXSLT_Terminate();
LIBXLST_API	bool LIBXSLT_Transform(const char *input, const char *output, const char *xsl);
LIBXLST_API void LIBXSLT_GetLastError(char *err_msg);