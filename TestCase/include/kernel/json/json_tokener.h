/*
 * $Id: json_tokener.h,v 1.10 2006/07/25 03:24:50 mclark Exp $
 *
 * Copyright (c) 2004, 2005 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef _json_tokener_h_
#define _json_tokener_h_

#include <stddef.h>
#include "json_object.h"

#ifdef __cplusplus
extern "C" {
#endif

enum json_tokener_error {
  json_tokener_success,
  json_tokener_continue,
  json_tokener_error_depth,
  json_tokener_error_parse_eof,
  json_tokener_error_parse_unexpected,
  json_tokener_error_parse_null,
  json_tokener_error_parse_boolean,
  json_tokener_error_parse_number,
  json_tokener_error_parse_array,
  json_tokener_error_parse_object_key_name,
  json_tokener_error_parse_object_key_sep,
  json_tokener_error_parse_object_value_sep,
  json_tokener_error_parse_string,
  json_tokener_error_parse_comment
};

enum json_tokener_state {
  json_tokener_state_eatws,
  json_tokener_state_start,
  json_tokener_state_finish,
  json_tokener_state_null,
  json_tokener_state_comment_start,
  json_tokener_state_comment,
  json_tokener_state_comment_eol,
  json_tokener_state_comment_end,
  json_tokener_state_string,
  json_tokener_state_string_escape,
  json_tokener_state_escape_unicode,
  json_tokener_state_boolean,
  json_tokener_state_number,
  json_tokener_state_array,
  json_tokener_state_array_add,
  json_tokener_state_array_sep,
  json_tokener_state_object_field_start,
  json_tokener_state_object_field,
  json_tokener_state_object_field_end,
  json_tokener_state_object_value,
  json_tokener_state_object_value_add,
  json_tokener_state_object_sep
};

struct json_tokener_srec
{
  enum json_tokener_state state, saved_state;
  struct json_object *obj;
  struct json_object *current;
  char *obj_field_name;
};

#define JSON_TOKENER_MAX_DEPTH 32

struct json_tokener
{
  char *str;
  struct printbuf *pb;
  int depth, is_double, st_pos, char_offset;
  ptrdiff_t err;
  unsigned int ucs_char;
  char quote_char;
  struct json_tokener_srec stack[JSON_TOKENER_MAX_DEPTH];
};

extern const char* json_tokener_errors[];

extern struct json_tokener* json_tokener_new(void);
extern void json_tokener_free(struct json_tokener *tok);
extern void json_tokener_reset(struct json_tokener *tok);

/**
*@brief 解析json字符串，由 str 里的 JSON 字符串生成 JSON 对象
*@param[in] str 输入JSON 字符串
*@return 返回json_object对象
*@note 
*    (1)对于json_tokener_parse返回的对象，通过is_error判断其是否为json_object对象
*    (2)对于json_tokener_parse返回的对象, 通过json_object_is_type来判断对象的类型
*    (3)对于通过json_tokener_parse生成的对象，需要用json_object_put来释放申请的内存;
*    	json_object_put释放内存的时候，采用的是递归释放；即如果通过json_object_object_get、
*    	json_object_array_get_idx从json_tokener_parse生成的对象中获取出来的对象不需再单独调用
*		json_object_put	来释放，如:
*    	jsInfo = json_tokener_parse(acMainFreInfo);
*    	if (!is_error(jsInfo))
*    	{
*    		if (json_object_is_type(jsInfo, json_type_array)
*    		{
*    			jsIter = json_object_array_get_idx(jsInfo, 0);
*
*    			.......
*
*    			json_object_put(jsInfo);//此处调用json_object_put(jsInfo)，会在释放jsInfo的同时，递归释放掉jsIter的内存
*    		}
*    	}
*/
extern json_object* json_tokener_parse(const char *str);

extern struct json_object* json_tokener_parse_ex(struct json_tokener *tok,
						 const char *str, int len);

#ifdef __cplusplus
}
#endif

#endif
