#ifndef COMX_COMX_JS_EXT_5b3ab22b483b4c23922724cba9e4fb47_HXX
#define COMX_COMX_JS_EXT_5b3ab22b483b4c23922724cba9e4fb47_HXX

#ifndef COMX_KERNEL_COMX_EXPORT_81c958c386154af495a84ae63135623e_HXX
#define COMX_KERNEL_COMX_EXPORT_81c958c386154af495a84ae63135623e_HXX

#ifdef LIBCOMXKERNEL_EXPORTS
#define LIBCOMXKERNEL_API __declspec(dllexport)
#else
#define LIBCOMXKERNEL_API __declspec(dllimport)
#endif

#endif

#include <Windows.h>

#include <assert.h>

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

#define KUL_PANE_RUN_JS_MSG (WM_USER + 512)

typedef bool (*comx_js_ext_entry_fn_t)(char *func_name, int argc, char **argv, const char*& res, const char* formid);

struct comx_js_ext_entry
{
	char **func_list;
	int func_num;
	char *ns;
	comx_js_ext_entry_fn_t entry_func;
};

#ifdef _M_X64
    #define JS_PARSER_MODULE "js_parser64.dll"
    #define COMX_KERNEL_REGISTEREXTENTRY "COMX_KERNEL_RegisterExtEntry"
    #define COMX_KERNEL_RUNJS "COMX_KERNEL_RunJS"
    #define COMX_KERNEL_RUNASYNCJS "COMX_KERNEL_RunAsyncJS"
    #define COMX_KERNEL_RUNJS_EX "COMX_KERNEL_RunJS_Ex"
#else
    #define JS_PARSER_MODULE "js_parser.dll"
    #define COMX_KERNEL_REGISTEREXTENTRY "_COMX_KERNEL_RegisterExtEntry@8"
    #define COMX_KERNEL_RUNJS "_COMX_KERNEL_RunJS@12"
    #define COMX_KERNEL_RUNASYNCJS "_COMX_KERNEL_RunAsyncJS@8"
    #define COMX_KERNEL_RUNJS_EX "_COMX_KERNEL_RunJS_Ex@20"
#endif

#ifndef LIBCOMXKERNEL_EXPORTS

//extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RegisterExtEntry(comx_js_ext_entry *&entry, char *dll_path);

inline bool COMX_KERNEL_RegisterExtEntry(comx_js_ext_entry *&entry, char *dll_path)
{
    typedef bool (FAR __stdcall *ptr_COMX_KERNEL_RegisterExtEntry_t)(comx_js_ext_entry *&entry, char *dll_path);
    static ptr_COMX_KERNEL_RegisterExtEntry_t ptr_COMX_KERNEL_RegisterExtEntry = NULL;

    if(NULL == ptr_COMX_KERNEL_RegisterExtEntry)
    {
        ptr_COMX_KERNEL_RegisterExtEntry = (ptr_COMX_KERNEL_RegisterExtEntry_t)GetProcAddress(GetModuleHandle(NULL), COMX_KERNEL_REGISTEREXTENTRY);
    }

    if(NULL == ptr_COMX_KERNEL_RegisterExtEntry)
    {
        ptr_COMX_KERNEL_RegisterExtEntry = (ptr_COMX_KERNEL_RegisterExtEntry_t)GetProcAddress(GetModuleHandle(JS_PARSER_MODULE), COMX_KERNEL_REGISTEREXTENTRY);
    }

    assert(ptr_COMX_KERNEL_RegisterExtEntry);

    return (*ptr_COMX_KERNEL_RegisterExtEntry)(entry, dll_path);
}

//extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunJS(const char* js_codes, char *ret, const char* formid);

inline bool COMX_KERNEL_RunJS(const char* js_codes, char *ret, const char* formid)
{
    typedef bool (FAR __stdcall *ptr_COMX_KERNEL_RunJS_t)(const char* js_codes, char *ret, const char* formid);
    static ptr_COMX_KERNEL_RunJS_t ptr_COMX_KERNEL_RunJS = NULL;

    if(NULL == ptr_COMX_KERNEL_RunJS)
    {
        ptr_COMX_KERNEL_RunJS = (ptr_COMX_KERNEL_RunJS_t)GetProcAddress(GetModuleHandle(NULL), COMX_KERNEL_RUNJS);
    }

    if(NULL == ptr_COMX_KERNEL_RunJS)
    {
        ptr_COMX_KERNEL_RunJS = (ptr_COMX_KERNEL_RunJS_t)GetProcAddress(GetModuleHandle(JS_PARSER_MODULE), COMX_KERNEL_RUNJS);
    }

    assert(ptr_COMX_KERNEL_RunJS);

    return (*ptr_COMX_KERNEL_RunJS)(js_codes, ret, formid);
}

//extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunAsyncJS(const char* js_codes, const char *formid);

inline bool COMX_KERNEL_RunAsyncJS(const char* js_codes, const char *formid)
{
    typedef bool (FAR __stdcall *ptr_COMX_KERNEL_RunAsyncJS_t)(const char* js_codes, const char *formid);
    static ptr_COMX_KERNEL_RunAsyncJS_t ptr_COMX_KERNEL_RunAsyncJS = NULL;

    if(NULL == ptr_COMX_KERNEL_RunAsyncJS)
    {
        ptr_COMX_KERNEL_RunAsyncJS = (ptr_COMX_KERNEL_RunAsyncJS_t)GetProcAddress(GetModuleHandle(NULL), COMX_KERNEL_RUNASYNCJS);
    }

    if(NULL == ptr_COMX_KERNEL_RunAsyncJS)
    {
        ptr_COMX_KERNEL_RunAsyncJS = (ptr_COMX_KERNEL_RunAsyncJS_t)GetProcAddress(GetModuleHandle(JS_PARSER_MODULE), COMX_KERNEL_RUNASYNCJS);
    }

    assert(ptr_COMX_KERNEL_RunAsyncJS);

    return (*ptr_COMX_KERNEL_RunAsyncJS)(js_codes, formid);
}

//extern "C" bool LIBCOMXKERNEL_API _stdcall COMX_KERNEL_RunJS_Ex(const char *js_file,  const char* js_lib, const char* js_codes, char *ret, const char *formid);

inline bool COMX_KERNEL_RunJS_Ex(const char *js_file,  const char* js_lib, const char* js_codes, char *ret, const char *formid)
{
    typedef bool (FAR __stdcall *ptr_COMX_KERNEL_RunJS_Ex_t)(const char *js_file,  const char* js_lib, const char* js_codes, char *ret, const char *formid);
    static ptr_COMX_KERNEL_RunJS_Ex_t ptr_COMX_KERNEL_RunJS_Ex = NULL;

    if(NULL == ptr_COMX_KERNEL_RunJS_Ex)
    {
        ptr_COMX_KERNEL_RunJS_Ex = (ptr_COMX_KERNEL_RunJS_Ex_t)GetProcAddress(GetModuleHandle(NULL), COMX_KERNEL_RUNJS_EX);
    }

    if (NULL == ptr_COMX_KERNEL_RunJS_Ex)
    {
        ptr_COMX_KERNEL_RunJS_Ex = (ptr_COMX_KERNEL_RunJS_Ex_t)GetProcAddress(GetModuleHandle(JS_PARSER_MODULE), COMX_KERNEL_RUNJS_EX);
    }

    assert(ptr_COMX_KERNEL_RunJS_Ex);

    return (*ptr_COMX_KERNEL_RunJS_Ex)(js_file, js_lib, js_codes, ret, formid);
}

#endif


#define JS_EXT_MAIN_BEGIN(sub_ns,num) \
	BOOL APIENTRY DllMain( HANDLE hModule, \
	DWORD  ul_reason_for_call, \
	LPVOID lpReserved\
	)\
{\
	if (DLL_PROCESS_ATTACH == ul_reason_for_call)\
{\
	S_entry.entry_func = comx_entry_ext_main;\
	S_namespace = sub_ns;\
	S_entry.ns = (char*)S_namespace.c_str();\
	S_func_list.resize(num);\
	S_entry.func_list = &S_func_list[0];\
	S_entry.func_num = num;\
	int cnt = 0;

#define JS_EXT_FUNC_REG(name) S_func_list[cnt++] = #name;

#define JS_EXT_MAIN_END() \
	static comx_js_ext_entry *entry = NULL;\
	entry = &S_entry;\
	\
	char szModuleFileName[1024] = "";\
	GetModuleFileName((HMODULE)hModule, szModuleFileName, 4096);\
	\
	COMX_KERNEL_RegisterExtEntry(entry, szModuleFileName);\
}\
	return TRUE;\
}

#define JS_EXT_ENTRY_BEGIN() bool comx_entry_ext_main(char *func_name, int argc, char **argv, const char *&res, const char *formid){S_formid = formid;

#define JS_EXT_ENTRY(func) \
	if (strcmp(func_name, #func) == 0)\
{\
	return func(argc, argv, res);\
}

#define JS_EXT_ENTRY_END() \
	S_ret = "function isn't exist";\
	res = S_ret.c_str();\
	\
	return false;\
}

typedef void (*ptr_fn_gl_cache_entry_t)(double *vertexs, int *vertexs_prop, int vertex_num, 
										double *triangles, double *triangle_normals, int *triangles_prop, int triangle_num, 
										double *lines, int *lines_prop, int line_num,
										const char *comment,
										int display_mode,
										int *color,
										int mid,
										int sid,
										bool is_show);

typedef void(*ptr_fn_gl_cache_entry_ex_t)(double *points, double *normals, int nlen,
    int *triangles, int *triangles_prop, int triangle_num,
    int *lines, int *lines_prop, int line_num,
    int *vertexes, int *vertexes_prop, int vertex_num,

    const char *comment,
    int display_mode,
    int *color,
    int mid,
    int sid,
    bool is_show);

#define JS_EXT_DATA_DECLARE() \
string S_formid;\
string S_ret;\
comx_js_ext_entry S_entry;\
vector<char*> S_func_list;\
string S_namespace;\
inline void JS_EXT_GL_CACHE_ENTRY(double *vertexs, int *vertexs_prop, int vertex_num, \
    double *triangles, double *triangle_normals, int *triangles_prop, int triangle_num, \
    double *lines, int *lines_prop, int line_num,\
    const char *comment,\
    int display_mode,\
    int *color,\
    int mid,\
    int sid,\
    bool is_show)\
{\
	char szEntry[256] = "";\
	COMX_KERNEL_RunJS("gl.cache.Entry()", szEntry, JS_EXT_FORMID);\
\
	unsigned long long ulEntry = type_cast<unsigned long long>(szEntry);\
	ptr_fn_gl_cache_entry_t ptrFnEntry = (ptr_fn_gl_cache_entry_t)ulEntry;\
\
	ptrFnEntry(vertexs, vertexs_prop, vertex_num, \
		triangles, triangle_normals, triangles_prop, triangle_num, \
		lines, lines_prop, line_num,\
		comment,\
		display_mode,\
		color,\
		mid,\
		sid,\
		is_show);\
}\
inline void JS_EXT_GL_CACHE_ENTRY_EX(double *points, double *normals, int nlen, \
    int *triangles, int *triangles_prop, int triangle_num, \
    int *lines, int *lines_prop, int line_num, \
    int *vertexes, int *vertexes_prop, int vertex_num,\
    const char *comment, \
    int display_mode, \
    int *color, \
    int mid, \
    int sid, \
    bool is_show)\
{\
char szEntryEx[256] = "";\
COMX_KERNEL_RunJS("gl.cache.EntryEx()", szEntryEx, JS_EXT_FORMID);\
\
unsigned long long ulEntryEx = type_cast<unsigned long long>(szEntryEx);\
ptr_fn_gl_cache_entry_ex_t ptrFnEntryEx = (ptr_fn_gl_cache_entry_ex_t)ulEntryEx;\
\
ptrFnEntryEx(points, normals, nlen, \
    triangles, triangles_prop, triangle_num, \
    lines, lines_prop, line_num, \
    vertexes, vertexes_prop, vertex_num,\
    comment, \
    display_mode, \
    color, \
    mid, \
    sid, \
    is_show);\
}

inline string JS_EXT_FixedFileExtName(string fname, string ext_fnames[], int len)
{
    string::size_type idx = fname.find_last_of('.');

    if(std::find(ext_fnames, ext_fnames + len, fname.substr(idx + 1)) == ext_fnames + len)
    {
        fname += ".";
        fname += ext_fnames[0];
    }

    return fname;
}

inline string JS_EXT_FixedFileExtName(string fname, string ext_fname)
{
    string::size_type idx = fname.find_last_of('.');

    if(fname.substr(idx + 1) != ext_fname)
    {
        fname += ".";
        fname += ext_fname;
    }

    return fname;
}

template<typename Reader, typename Value>
inline bool JS_EXT_LOAD_JSON(string json_fpath, Reader &reader, Value &root)
{
    bool ret = true;

    bool bExist = true;
    ifstream ifos(json_fpath.c_str());
    if (!ifos) 
    {
        bExist = false;
    }

    if (bExist)
    {
        ifstream ifs;

        ifs.open(json_fpath.c_str());

        if (!reader.parse(ifs, root, false))
        {
            ret = false;
        }
    }

    return ret;
}


#define JS_EXT_FUNC_BEGIN(func_name, para_num, usage) \
	bool func_name(int argc, char** argv, const char*&res)\
{\
	bool ret = false;\
	if (argc != para_num)\
{\
	S_ret = usage;\
}\
	else\
{

#define JS_EXT_FUNC_ASSIGN_RET(val) S_ret = (val);

#define JS_EXT_FUNC_END() \
	ret = true;\
}\
	\
	res = S_ret.c_str();\
	\
	return ret;\
}

#define  JS_EXT_PARA(type_inf, para_index) \
	type_cast<type_inf>(string(argv[para_index]))

#define JS_EXT_FORMID (S_formid.c_str())

#ifndef LIBCOMXKERNEL_EXPORTS

inline bool COMX_KERNEL_IsCallbackValid(string formid)
{
    char szRet[4096] = "";
    string szTestCmd = "typeof(__callback_c0b08f9ee5844f46bdbac03fefbecf82)";
    COMX_KERNEL_RunJS(szTestCmd.c_str(), szRet, formid.c_str());

    return (string(szRet) == "function");
}

inline string COMX_KERNEL_RunCallback(string formid, int n, ...)
{  
    va_list arg_ptr;

    string parameters = "";

    va_start(arg_ptr, n); 

    for (int loop = 0; loop < n; ++loop)
    {
        string para = va_arg(arg_ptr, char*);

        if (loop != 0)
        {
            parameters += ",";
        }

        para = "'" + para + "'";

        parameters += para;
    }
    va_end(arg_ptr); 

    string cmd = "if (typeof(__callback_c0b08f9ee5844f46bdbac03fefbecf82) == 'function')\n{\n    __callback_c0b08f9ee5844f46bdbac03fefbecf82(" + parameters + ");\n}";
    char szRet[4096] = "";
    
    COMX_KERNEL_RunJS(cmd.c_str(), szRet, formid.c_str());

    return szRet;
}

#endif

inline bool HasBOM(const void *buf, const int buf_size)
{
    if (buf_size < 3)
    {
        return false;
    }

    unsigned char* start = (unsigned char*)buf;

    if (start[0] == 0xef && start[1] == 0xbb && start[2] == 0xbf)
    {
        return true;
    }

    return false;
}

inline bool IsUTF8NoBOM(const void* pBuffer, long size)
{
    bool bIsUTF8 = true;
    unsigned char* start = (unsigned char*)pBuffer;
    unsigned char* end = (unsigned char*)pBuffer + size;
    while (start < end)
    {
        if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
        {
            start++;
        }
        else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
        {
            bIsUTF8 = false;
            break;
        }
        else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
        {
            if (start >= end - 1) 
                break;
            if ((start[1] & (0xC0)) != 0x80)
            {
                bIsUTF8 = false;
                break;
            }
            start += 2;
        } 
        else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
        {
            if (start >= end - 2) 
                break;
            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
            {
                bIsUTF8 = false;
                break;
            }
            start += 3;
        } 
        else
        {
            bIsUTF8 = false;
            break;
        }
    }
    return bIsUTF8;
}

inline bool isGBKCode(const string& strIn)
{
    unsigned char ch1;
    unsigned char ch2;

    for (string::size_type loop = 0; loop < strIn.size() - 1; ++loop)
    {
        ch1 = (unsigned char)strIn.at(loop);
        ch2 = (unsigned char)strIn.at(loop + 1);
        if (ch1>=129 && ch1<=254 && ch2>=64 && ch2<=254)
            return true;
    }

    return false;
}

enum TCOMXStringEncodingTypeEnum
{
    COMX_UNICODE = 1,
    COMX_UNICODE_BIG_ENDIAN = 2,
    COMX_UTF8 = 3,
    COMX_GBK = 4,
    COMX_ANSI = 0
};

inline TCOMXStringEncodingTypeEnum CheckStringEncodingType(const string &str)
{
    stringstream sstr(str);
    unsigned char  s2;  
    sstr.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位  
    int p = s2<<8;  
    sstr.read((char*)&s2, sizeof(s2));//读取第二个字节  
    p +=s2;  

    if (IsUTF8NoBOM((void*)str.c_str(), str.size()))
    {
        return COMX_UTF8;
    }

    switch(p)//判断文本前两个字节  
    {  
    case 0xfffe:  //65534  
        return COMX_UNICODE;
    case 0xfeff://65279  
        return COMX_UNICODE_BIG_ENDIAN;
    case 0xefbb://61371  
        return COMX_UTF8;  
    default:   
        return isGBKCode(str) ? COMX_GBK : COMX_ANSI;
    }  
}

inline bool IsUTF8(const string &str)
{
    return CheckStringEncodingType(str) == COMX_UTF8;
}

inline bool IsANSI(const string &str)
{
    return CheckStringEncodingType(str) == COMX_ANSI || CheckStringEncodingType(str) == COMX_GBK;
}

inline bool IsGBK(const string &str)
{
    return CheckStringEncodingType(str) == COMX_GBK;
}

#include <Windows.h>

inline string JS_utf8_trans_impl(string const& from, int from_code, int to_code)
{
    int len16 = MultiByteToWideChar(from_code, 0, /*from.begin()*/&from[0], (int)from.size(), 0, 0);
    if (len16 > 0)
    {
        vector<wchar_t> ucs2_buf(len16, 0);
        len16 = MultiByteToWideChar(from_code, 0,/* from.begin()*/&from[0], (int)from.size(), /*ucs2_buf.begin()*/&ucs2_buf[0], len16);

        int len8 = WideCharToMultiByte(to_code, 0, &ucs2_buf[0], len16, 0,  0, 0, 0);
        string result(len8, 0);
        WideCharToMultiByte(to_code, 0, &ucs2_buf[0]/*ucs2_buf.begin()*/, len16, &result[0]/*result.begin()*/,  len8, 0, 0);
        return result;
    }

    return  string();
}

inline string string_To_UTF8(const std::string & str)  
{  
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);  

    wchar_t * pwBuf = new wchar_t[nwLen + 2];//一定要加1，不然会出现尾巴  
    ZeroMemory(pwBuf, nwLen * 2 + 2);  

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);  

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);  

    char * pBuf = new char[nLen + 1];  
    ZeroMemory(pBuf, nLen + 1);  

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);  

    std::string retStr(pBuf);  

    delete []pwBuf;  
    delete []pBuf;  

    pwBuf = NULL;  
    pBuf = NULL;  

    return retStr;  
}  

inline std::string UTF8_To_string(const std::string & str)  
{  
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);  

    wchar_t * pwBuf = new wchar_t[nwLen + 2];//一定要加1，不然会出现尾巴  
    memset(pwBuf, 0, nwLen * 2 + 2);  

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);  

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);  

    char * pBuf = new char[nLen + 1];  
    memset(pBuf, 0, nLen + 1);  

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);  

    std::string retStr = pBuf;  

    delete []pBuf;  
    delete []pwBuf;  

    pBuf = NULL;  
    pwBuf = NULL;  

    return retStr;  
}  
//////////////////////////////////////////////////////////////////////////  

inline string  JS_FROM_UTF8(string const& from)
{    
    //return JS_utf8_trans_impl( from, CP_UTF8, CP_ACP);

    return IsUTF8(from) ? UTF8_To_string(from) : from;
}

inline string  JS_TO_UTF8(string const& from)
{    
    //return JS_utf8_trans_impl(from, CP_ACP, CP_UTF8);
    return (IsANSI(from) || IsGBK(from)) ? string_To_UTF8(from) : from;
}

#endif