// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBXSD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBXSD_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBXSD_EXPORTS
#define LIBXSD_API __declspec(dllexport)
#else
#define LIBXSD_API __declspec(dllimport)
#endif

LIBXSD_API bool xsd_validate(const char *xmlFile,const char *schemaFile, char *err_msg, int &err_line_number, int &err_column_number);
