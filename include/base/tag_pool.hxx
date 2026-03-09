/*HEAD tag_pool HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_TAG_POOL_H_20089288401
#define _KMAS_COMX_TAG_POOL_H_20089288401

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

typedef uint64_t tag_t;

// {0256EDFD-C792-4753-97AF-3A7638D69B9A}
 const MUID IID_ITagPool = 
{ 0x256edfd, 0xc792, 0x4753, { 0x97, 0xaf, 0x3a, 0x76, 0x38, 0xd6, 0x9b, 0x9a } };

comx_interface ITagPool : public IRoot
{
     virtual TStatus STDCALL CreateTag(
		 const int code, 
		 void *val, 
		 tag_t *tag) = 0;
     virtual TStatus STDCALL DeleteTag(
		 const tag_t tag) = 0;
     virtual TStatus STDCALL GetTagCode(
		 const tag_t tag,
		 int *p_code) = 0;
     virtual TStatus STDCALL GetTagValue(
		 const tag_t tag,
		 void **val) = 0;
     virtual TStatus STDCALL IsValueExist(
		 const int code,
		 const void *val, 
		 bool& is_exist) = 0;
	 virtual TStatus STDCALL IsTagExist(
		 const tag_t tag,
		 bool& is_exist) = 0;
};

extern "C"
{
	struct tag_pool_fun_t
	{
		tag_t (*pfn_create_tag)(const int code, void *val);
		void  (*pfn_delete_tag)(const tag_t tag);
		int   (*pfn_get_tag_code)(const tag_t tag);
		void* (*pfn_get_tag_val)(const tag_t tag);
		bool  (*pfn_is_value_exist)(const int code,const void *val);
		bool  (*pfn_is_tag_exist)(const tag_t tag);
	};
}

// {A8066601-485B-4955-B42C-F5E6728BB56B}
const MUID IID_ITagPoolEx = 
{ 0xa8066601, 0x485b, 0x4955, { 0xb4, 0x2c, 0xf5, 0xe6, 0x72, 0x8b, 0xb5, 0x6b } };

comx_interface ITagPoolEx : public ITagPool
{
	 virtual TStatus STDCALL GetTagPoolFuncs(tag_pool_fun_t &tpf) = 0;
};

// {E48A9FFC-141C-4e13-86A7-AA8EA3BA4037}
 const MTYPEID CLSID_ITagPool = 
{ 0xe48a9ffc, 0x141c, 0x4e13, { 0x86, 0xa7, 0xaa, 0x8e, 0xa3, 0xba, 0x40, 0x37 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

