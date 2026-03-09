#ifndef _KMAS_TYPE_CAST_HXX_
#define _KMAS_TYPE_CAST_HXX_

#include "struct_stream.hxx"

#include <assert.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

// #define SUPPORT_NODE_API

namespace KMAS {
namespace type {

#ifdef SUPPORT_NODE_API
Napi::Env *g_pEnv = NULL;

class TNodeEnvWrap
{
  public:
    TNodeEnvWrap(Napi::Env &env)
    {
      assert(g_pEnv == NULL);
      g_pEnv = &env;
    }
  public:
    ~TNodeEnvWrap()
    {
      g_pEnv = NULL;
    }
};

#endif

typedef char *char_ptr_t;
typedef const char *char_c_ptr_t;

template <typename T, typename R> void _parse_input(T input, R &ret) {
  stringstream sstr;

  sstr << input;
  sstr >> ret;
}

void _parse_input(string input, bool &ret) {
  ret = (input != "false" && input != "no");
}

void _parse_input(char_ptr_t input, bool &ret) {
  ret = (strcmp(input, "false") != 0 && strcmp(input, "no") != 0);
}

void _parse_input(char_c_ptr_t input, bool &ret) {
  ret = (strcmp(input, "false") != 0 && strcmp(input, "no") != 0);
}

#ifdef SUPPORT_NODE_API

void _parse_input(Napi::Value &input, string &ret) {
  //assert(input.IsString());
  ret = input.ToString().As<Napi::String>().Utf8Value();
}

void _parse_input(Napi::Value &input, Napi::Value &ret) {
  ret = input;
}

void _parse_input(Napi::Value &input, Napi::Function &ret) {
  assert(input.IsFunction());
  ret = input.As<Napi::Function>();
}

void _parse_input(Napi::Value &input, Napi::ArrayBuffer &ret)
{
  assert(input.IsArrayBuffer());
  ret = input.As<Napi::ArrayBuffer>();
}
  
void _parse_input(Napi::Value &input, void *&ret)
{
  assert(input.IsArrayBuffer());
  ret = input.As<Napi::ArrayBuffer>().Data();
}

void _parse_input(Napi::Value &input, Napi::Int16Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_int16_array);
  ret = input.As<Napi::Int16Array>();
}

void _parse_input(Napi::Value &input, int16_t *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_int16_array);
  ret = input.As<Napi::Int16Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Int8Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_int8_array);
  ret = input.As<Napi::Int8Array>();
}

void _parse_input(Napi::Value &input, int8_t *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_int8_array);
  ret = input.As<Napi::Int8Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Uint16Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_uint16_array);
  ret = input.As<Napi::Uint16Array>();
}

void _parse_input(Napi::Value &input, uint16_t *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_uint16_array);
  ret = input.As<Napi::Uint16Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Uint8Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_uint8_array);
  ret = input.As<Napi::Uint8Array>();
}

void _parse_input(Napi::Value &input, uint8_t *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_uint8_array);
  ret = input.As<Napi::Uint8Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Int32Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_int32_array);
  ret = input.As<Napi::Int32Array>();
}

void _parse_input(Napi::Value &input, int32_t *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_int32_array);
  ret = input.As<Napi::Int32Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Uint32Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_uint32_array);
  ret = input.As<Napi::Uint32Array>();
}

void _parse_input(Napi::Value &input, uint32_t *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_uint32_array);
  ret = input.As<Napi::Uint32Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Float32Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_float32_array);
  ret = input.As<Napi::Float32Array>();
}

void _parse_input(Napi::Value &input, float *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_float32_array);
  ret = input.As<Napi::Float32Array>().Data();
}

void _parse_input(Napi::Value &input, Napi::Float64Array &ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_float64_array);
  ret = input.As<Napi::Float64Array>();
}

void _parse_input(Napi::Value &input, double *&ret)
{
  assert(input.IsTypedArray());
  assert(input.As<Napi::TypedArray>().TypedArrayType() == napi_float64_array);
  ret = input.As<Napi::Float64Array>().Data();
}

void _parse_input(Napi::Value &input, unsigned long long &ret) {
  assert(input.IsString());
  string str = input.ToString().As<Napi::String>().Utf8Value();
  _parse_input(str, ret);
}
  
  
void _parse_input(Napi::Value &input, int32_t &ret) {
  //assert(input.IsNumber());
  ret = input.ToNumber().As<Napi::Number>().Int32Value();
}

void _parse_input(Napi::Value &input, uint32_t &ret) {
  //assert(input.IsNumber());
  ret = input.ToNumber().As<Napi::Number>().Uint32Value();
}

void _parse_input(Napi::Value &input, int64_t &ret) {
  //assert(input.IsNumber());
  ret = input.ToNumber().As<Napi::Number>().Int64Value();
}

void _parse_input(Napi::Value &input, float &ret) {
  //assert(input.IsNumber());
  ret = input.ToNumber().As<Napi::Number>().FloatValue();
}

void _parse_input(Napi::Value &input, double &ret) {
  //assert(input.IsNumber());
  ret = input.ToNumber().As<Napi::Number>().DoubleValue();
}

void _parse_input(Napi::Value &input, bool &ret) {
  assert(input.IsBoolean());
  ret = input.As<Napi::Boolean>().Value();
}

void _parse_input(bool input, Napi::Value &ret)
{
  assert(g_pEnv != NULL);
  ret = Napi::Value::From(*g_pEnv, input);
}

void _parse_input(int input, Napi::Value &ret)
{
  assert(g_pEnv != NULL);
  ret = Napi::Value::From(*g_pEnv, input);
}

void _parse_input(double input, Napi::Value &ret)
{
  assert(g_pEnv != NULL);
  ret = Napi::Value::From(*g_pEnv, input);
}

void _parse_input(float input, Napi::Value &ret)
{
  assert(g_pEnv != NULL);
  ret = Napi::Value::From(*g_pEnv, input);
}

void _parse_input(const char *input, Napi::Value &ret)
{
  assert(g_pEnv != NULL);
  ret = Napi::Value::From(*g_pEnv, input);
}

void _parse_input(string input, Napi::Value &ret)
{
  assert(g_pEnv != NULL);
  ret = Napi::Value::From(*g_pEnv, input);
}

#endif 

template <typename R> class type_cast {
  R ret;

public:
  template <typename T> type_cast(T input) { _parse_input(input, ret); }

public:
  operator R() { return ret; }
};

template <> class type_cast<string> {
  string ret;

public:
  template <typename T> type_cast(T input) { _parse_input(input); }

private:
  template <typename T> void _parse_input(T input) {
    KMAS::type::_parse_input(input, ret);
  }

private:
  void _parse_input(bool input) {
    if (input) {
      ret = "true";
    } else {
      ret = "false";
    }
  }
  void _parse_input(string input) { ret = input; }

public:
  operator string() { return ret; }
};

template <> class type_cast<char_ptr_t> {
  char *ret;

public:
  template <typename T> type_cast(T input) { _parse_input(input); }

private:
  template <typename T> void _parse_input(T input) {
    stringstream sstr;

    sstr << input;

    _assign(sstr.str());
  }
private:
  void _assign(const string &str)
  {
    ret = new char[str.size() + 1];
    strcpy(ret, str.c_str());
  }
private:
  void _parse_input(bool input) {
    string vret = "";
    if (input) {
      vret = "true";
    } else {
      vret = "false";
    }

    _assign(vret);
  }
#ifdef SUPPORT_NODE_API
  void _parse_input(Napi::Value &input) 
  {
    string vret = "";
    KMAS::type::_parse_input(input, vret);
    _assign(vret);  
  }
#endif
public:
  operator char_ptr_t() { return ret; }
};

template <> class type_cast<char_c_ptr_t> {
  char *ret;

public:
  template <typename T> type_cast(T input) { _parse_input(input); }

private:
  template <typename T> void _parse_input(T input) {
    stringstream sstr;

    sstr << input;

    _assign(sstr.str());
  }
private:
  void _assign(const string &str)
  {
    ret = new char[str.size() + 1];
    strcpy(ret, str.c_str());
  }
private:
  void _parse_input(bool input) {
    string vret = "";
    if (input) {
      vret = "true";
    } else {
      vret = "false";
    }

    _assign(vret);
  }
#ifdef SUPPORT_NODE_API
  void _parse_input(Napi::Value &input) 
  {
    string vret = "";
    KMAS::type::_parse_input(input, vret);
    _assign(vret);  
  }
#endif
public:
  operator char_c_ptr_t() { return ret; }
};

template <typename R> class type_array_cast {
  R *ret;

public:
  type_array_cast(string input) {
    stringstream sstr;

    sstr << input;

    int num = 0;
    sstr >> num;

    ret = new R[num];

    for (int loop = 0; loop < num; ++loop) {
      R item;
      sstr >> item;
      ret[loop] = item;
    }
  }

public:
  operator R *() { return ret; }
};

template <> class type_array_cast<string> {
  string ret;

public:
  template <typename T> type_array_cast(T *input, int n) {
    stringstream sstr;

    sstr << n << " ";

    int loop = 0;
    for (loop = 0; loop < n - 1; ++loop) {
      sstr << input[loop] << " ";
    }
    sstr << input[loop];

    ret = sstr.str();
  }

public:
  operator string() { return ret; }
};

template <> class type_array_cast<char_ptr_t> {
private:
  char *ret;

public:
  template <typename T> type_array_cast(T *input, int n) {
    stringstream sstr;

    sstr << n << " ";

    int loop = 0;
    for (loop = 0; loop < n - 1; ++loop) {
      sstr << input[loop] << " ";
    }
    sstr << input[loop];

    ret = new char[sstr.str().size()];

    strcpy(ret, sstr.str().c_str());
  }

public:
  operator char_ptr_t() { return ret; }
};

template <> class type_array_cast<char_c_ptr_t> {
private:
  char *ret;

public:
  template <typename T> type_array_cast(T *input, int n) {
    stringstream sstr;

    sstr << n << " ";

    int loop = 0;
    for (loop = 0; loop < n - 1; ++loop) {
      sstr << input[loop] << " ";
    }
    sstr << input[loop];

    ret = new char[sstr.str().size()];

    strcpy(ret, sstr.str().c_str());
  }

public:
  operator char_c_ptr_t() { return ret; }
};

inline void type_cast_testing()
{
  cout << "Entry type_cast_testing()" << endl;
  string s1 = type_cast<string>(123.45);
  string s2 = type_cast<string>(321);
  string s3 = type_cast<string>(true);

  cout << s1 + "," + s2 + "," + s3 << endl;

  bool b1 = type_cast<bool>(string("yes"));
  bool b2 = type_cast<bool>(string("false"));

  cout << "b1 is " << (b1 ? "true" : "false") << endl;
  cout << "b2 is " << (b2 ? "true" : "false") << endl;

  int i1 = type_cast<int>("123");
  double d1 = type_cast<double>("4.56");
  float f1 = type_cast<float>("7.1");

  cout << i1 + d1 + f1 << endl;

  int arr[] = {1,3,5,7};
  string strBuf = type_array_cast<string>(arr, sizeof(arr) / sizeof(int));
  cout << strBuf << endl;

  double *arr_new = type_array_cast<double>(strBuf);

  copy(arr_new, arr_new + 4, ostream_iterator<double>(cout, ","));
  cout << endl;

  delete []arr_new;

  cout << "Leave type_cast_testing()" << endl;


}

} // namespace type.
} // namespace KMAS.

#endif
