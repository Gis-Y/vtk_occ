/**
 * @file v8_macros.h
 *
 * @author email4zxk@gmail.com
 * @version 1.0
 * @date 2011-03-08
*/

#ifndef _JS_MACROS_H
#define _JS_MACROS_H

#define JS_MACRO_TO_STRING( x ) JS_MACRO_TO_STRING1( x )
#define JS_MACRO_TO_STRING1( x ) #x

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <assert.h>

using namespace std;

class TV8InternalFieldManager
{
	map<string, void*> _internal_field;
private:
	TV8InternalFieldManager(){}
	static TV8InternalFieldManager &_get_myself()
	{
		static TV8InternalFieldManager myself;
		return myself;
	}
public:
	static TV8InternalFieldManager &Intance(){return _get_myself();}
public:
	bool SetField(string key, void* val)
	{
		if (_internal_field.find(key) == _internal_field.end())
		{
			_internal_field[key] = val;
			return true;
		}

		return false;
	}
	bool GetField(string key, void **val)
	{
		if (_internal_field.find(key) != _internal_field.end())
		{
			*val = _internal_field[key];
			return true;
		}

		return false;
	}
	bool EraseField(string key)
	{
		if (_internal_field.find(key) != _internal_field.end())
		{
			_internal_field.erase(_internal_field.find(key));
			return true;
		}

		return false;
	}
};

#define BEGIN_DECLARE_JS_CLASS(name) \
private:\
	static string className(){return #name;}\
public:\
static Persistent<FunctionTemplate> &GetFunctionTemplate()\
{\
	static Persistent<FunctionTemplate> ft = Persistent<FunctionTemplate>::New(FunctionTemplate::New(_class##name##Constructor, Handle<Value>(), Handle<Signature>()));;\
	return ft;\
}\
private:\
	static Handle<Value> _class##name##Constructor(const Arguments &args)\
{\
	if (!args.IsConstructCall()) \
{\
	return ThrowException(Exception::Error(String::New("Invalid call format. Please use the 'new' operator."))); \
}\
	\
	TV8InternalFieldManager::Intance().SetField(className(), new name(args));\
	\
	return args.This();\
}\
	\
	static Handle<Value> _class##name##Destructor(const Arguments &args)\
{\
	name *pObj = NULL;\
	TV8InternalFieldManager::Intance().GetField(className(), (void**)&pObj);\
	assert(pObj != NULL);\
	\
	delete pObj;\
	\
	TV8InternalFieldManager::Intance().EraseField(className());\
	\
	return args.This();\
}\
public:\
	static Persistent<FunctionTemplate> RegisterMyselfToV8(Handle<ObjectTemplate> &global)\
{\
	Persistent<FunctionTemplate> &ft = GetFunctionTemplate();\
	ft->SetClassName(String::New(className().c_str()));\
	\
	Handle<ObjectTemplate> obj = ft->PrototypeTemplate();

#define END_DECLARE_JS_CLASS(name) \
	obj->Set("Dispose", FunctionTemplate::New(_class##name##Destructor, Handle<Value>(), Handle<Signature>()));\
	global->Set(String::New(className().c_str()),  ft);\
	return ft;\
}

#define JS_METHOD_DECLARE(class_name, method_name) \
	obj->Set(#method_name, FunctionTemplate::New(_class##class_name##method_name, Handle<Value>(), Handle<Signature>()));

//#define JS_STR_PROPERTY_DECLARE(property_name, property_val) \
//	obj->Set(#property_name, String::New(property_val));

#define JS_METHOD_IMPLEMENT(class_name, method_name) \
	static Handle<Value> _class##class_name##method_name(const Arguments &args)\
{\
	class_name *pObj = NULL;\
	TV8InternalFieldManager::Intance().GetField(className(), (void**)&pObj);\
	assert(pObj != NULL);\
	\
	Handle<Value> ret = method_name(args, pObj);\
	\
	return ret;\
}\
	\
	static Handle<Value> method_name(const Arguments &args, TPerson *pThis)

#define JS_CONSTRUCTOR(class_name) \
	class_name(const Arguments &args)

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

inline string  JS_FROM_UTF8(string const& from)
{    
	return JS_utf8_trans_impl( from, CP_UTF8, CP_ACP);
}

inline string  JS_TO_UTF8(string const& from)
{    
	return JS_utf8_trans_impl(from, CP_ACP, CP_UTF8);
}

#endif


