#ifndef _KMAS_TYPE_CAST_HXX_
#define _KMAS_TYPE_CAST_HXX_

#include "struct_stream.hxx"

#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <assert.h>
#include <vector>
using namespace std;

namespace KMAS{
namespace type{

typedef char* char_ptr_t;
typedef const char* char_c_ptr_t;

template<typename R>
class type_cast
{
	R ret;
public:
	template<typename T>
	type_cast(T input)
	{
		_parse_input(input);
	}
public:
	operator R(){return ret;}
private:
	template<typename T>
	void _parse_input(T input)
	{
		stringstream sstr;
		
		sstr << input;
		sstr >> ret;
	}
public:
	template<>
	void _parse_input<string>(string input)
	{
		_assign<R>(input);
	}
	template<typename T>
	void _assign(string input)
	{
		stringstream sstr;
		
		sstr << input;
		sstr >> ret;
	}
	template<>
	void _assign<bool>(string input)
	{
		ret = (input != "false" &&
			   input != "no");
	}
public:
	template<>
	void _parse_input<char_ptr_t>(char_ptr_t input)
	{
		_assign_c_str<R>(input);
	}
	template<typename T>
	void _assign_c_str(char_ptr_t input)
	{
		stringstream sstr;
		
		sstr << input;
		sstr >> ret;
	}
	template<>
	void _assign_c_str<bool>(char_ptr_t input)
	{
		ret = (strcmp(input, "false") != 0 &&
			   strcmp(input, "no") != 0);
	}
public:
	template<>
		void _parse_input<char_c_ptr_t>(char_c_ptr_t input)
	{
		_assign_c_str_<R>(input);
	}
	template<typename T>
		void _assign_c_str_(char_c_ptr_t input)
	{
		stringstream sstr;
		
		sstr << input;
		sstr >> ret;
	}
	template<>
	void _assign_c_str_<bool>(char_c_ptr_t input)
	{
		ret = (strcmp(input, "false") != 0 &&
			   strcmp(input, "no") != 0);
	}
};

template<>
class type_cast<string>
{
	string ret;
public:
	template<typename T>
	type_cast(T input)
	{
		_parse_input(input);
	}
private:
    template<typename T>
    void _parse_input(T input)
    {
        stringstream sstr;

        sstr << input;
        sstr >> ret;
    }
public:
    template<>
    void _parse_input<bool>(bool input)
    {
        if (input)
        {
           ret = "true";
        }
        else
        {
            ret = "false";
        }
    }
    template<>
    void _parse_input<string>(string input)
    {
        ret = input;
    }
public:
	operator string(){return ret;}
};

template<>
class type_cast<char_ptr_t>
{
	char *ret;
public:
	template<typename T>
		type_cast(T input)
	{
		stringstream sstr;
		
		sstr << input;
		
		ret = new char[sstr.str().size()];
		strcpy(ret, sstr.str().c_str());
	}
public:
	operator char_ptr_t(){return ret;}
};

template<>
class type_cast<char_c_ptr_t>
{
	char *ret;
public:
	template<typename T>
		type_cast(T input)
	{
		stringstream sstr;
		
		sstr << input;

		ret = new char[sstr.str().size()];
		strcpy(ret, sstr.str().c_str());
	}
public:
	operator char_c_ptr_t(){return ret;}
};

template<typename R>
class type_array_cast
{
	R *ret;
public:
	type_array_cast(string input)
	{
		stringstream sstr;

		sstr << input;

		int num = 0;
		sstr >> num;

		ret = new R[num];
		
		for(int loop = 0; loop < num; ++loop)
		{
			R item;
			sstr >> item;
			ret[loop] = item;
		}
	}
public:
	operator R*(){return ret;}
};

template<>
class type_array_cast<string>
{
	string ret;
public:
	template<typename T>
	type_array_cast(T *input, int n)
	{
		stringstream sstr;

		sstr << n << " ";

		for (int loop = 0; loop < n - 1; ++loop)
		{
			sstr << input[loop] << " ";
		}
		sstr << input[loop];

		ret = sstr.str();
	}
public:
	operator string(){return ret;}
};

template<>
class type_array_cast<char_ptr_t>
{
private:
	char *ret;
public:
	template<typename T>
		type_array_cast(T *input, int n)
	{
		stringstream sstr;

		sstr << n << " ";
		
		for (int loop = 0; loop < n - 1; ++loop)
		{
			sstr << input[loop] << " ";
		}
		sstr << input[loop];

		ret = new char[sstr.str().size()];
		
		strcpy(ret, sstr.str().c_str());
	}
public:
	operator char_ptr_t(){return ret;}	
};

template<>
class type_array_cast<char_c_ptr_t>
{
private:
	char *ret;
public:
	template<typename T>
		type_array_cast(T *input, int n)
	{
		stringstream sstr;

		sstr << n << " ";
		
		for (int loop = 0; loop < n - 1; ++loop)
		{
			sstr << input[loop] << " ";
		}
		sstr << input[loop];

		ret = new char[sstr.str().size()];
		
		strcpy(ret, sstr.str().c_str());
	}
public:
	operator char_c_ptr_t(){return ret;}	
};

}//namespace type.
}//namespace KMAS.

#endif