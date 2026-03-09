
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBKULPROP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBKULPROP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBKULPROP_EXPORTS
#define LIBKULPROP_API __declspec(dllexport)
#else
#define LIBKULPROP_API __declspec(dllimport)
#endif

LIBKULPROP_API void filling_kul_property(
	const char* input_fname, 
	const char* output_fname, 
	const int mode,
	bool deal_with_subform/* = false*/); // 1-simple, 2-full.

LIBKULPROP_API void beautiful_xml_file(
	const char* input_fname);

LIBKULPROP_API void set_init_directory(
	const char* const dir_path);