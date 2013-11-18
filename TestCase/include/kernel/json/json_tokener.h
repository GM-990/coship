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
*@brief ����json�ַ������� str ��� JSON �ַ������� JSON ����
*@param[in] str ����JSON �ַ���
*@return ����json_object����
*@note 
*    (1)����json_tokener_parse���صĶ���ͨ��is_error�ж����Ƿ�Ϊjson_object����
*    (2)����json_tokener_parse���صĶ���, ͨ��json_object_is_type���ж϶��������
*    (3)����ͨ��json_tokener_parse���ɵĶ�����Ҫ��json_object_put���ͷ�������ڴ�;
*    	json_object_put�ͷ��ڴ��ʱ�򣬲��õ��ǵݹ��ͷţ������ͨ��json_object_object_get��
*    	json_object_array_get_idx��json_tokener_parse���ɵĶ����л�ȡ�����Ķ������ٵ�������
*		json_object_put	���ͷţ���:
*    	jsInfo = json_tokener_parse(acMainFreInfo);
*    	if (!is_error(jsInfo))
*    	{
*    		if (json_object_is_type(jsInfo, json_type_array)
*    		{
*    			jsIter = json_object_array_get_idx(jsInfo, 0);
*
*    			.......
*
*    			json_object_put(jsInfo);//�˴�����json_object_put(jsInfo)�������ͷ�jsInfo��ͬʱ���ݹ��ͷŵ�jsIter���ڴ�
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
