#ifndef COMX_LIBKIDLPROJECT_dacf1cd4acbe404fa57e106fe2592197_HXX
#define COMX_LIBKIDLPROJECT_dacf1cd4acbe404fa57e106fe2592197_HXX

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBKIDLPROJECT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBKIDLPROJECT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBKIDLPROJECT_EXPORTS
#define LIBKIDLPROJECT_API __declspec(dllexport)
#else
#define LIBKIDLPROJECT_API __declspec(dllimport)
#endif

typedef void (*info_display_cb_t)(const char *msg);
LIBKIDLPROJECT_API bool LIBKIDLPROJECT_GenerateVC6ProjectFromKIDL(
	const char *kidl_path,
	const char *target_dir = NULL, 
	char *last_err = NULL, 
	info_display_cb_t cb = NULL,
	bool is_auto_open = false);

#endif


