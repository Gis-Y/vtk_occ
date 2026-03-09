
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBKULREG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBKULREG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBKULREG_EXPORTS
#define LIBKULREG_API __declspec(dllexport)
#else
#define LIBKULREG_API __declspec(dllimport)
#endif

extern "C" LIBKULREG_API bool lib_kul_reg(const char *kul_path, const char *unit_name, char *tar_kul_path);
extern "C" LIBKULREG_API bool lib_kul_reg_silent(const char *kul_path, const char *unit_name, char *tar_kul_path);
extern "C" LIBKULREG_API bool lib_kul_reg_clean(const char *unit_name);