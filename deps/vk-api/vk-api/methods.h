#pragma once

#include <jansson.h>

/* a set of wrapper functions for calling VK authentication servers and API */
#ifdef __cplusplus
extern "C" {
#endif

/* authentication methods */
json_t *login_simple(const char *username, const char *password);
json_t *login_captcha(const char *username, const char *password,
		const char *sid, const char *key);
json_t *login_2fa(const char *username, const char *password, const char *code);

/* api methods */
void vk_record_visit(const char *access_token);
json_t *vk_get_user(const char *access_token);
json_t *vk_get_user_owned_groups(const char *access_token);
json_t *vk_get_stream_categories(const char *access_token);
json_t *vk_start_streaming(const char *access_token,
	const char *name, const char *group_id, const char *category_id);
void vk_stop_streaming(const char *access_token, const char *group_id);

#define JSON_GET_STRING(json, key) \
		json_string_value(json_object_get(json, key))
#define JSON_GET_INTEGER(json, key) \
		json_integer_value(json_object_get(json, key))

#ifdef __cplusplus
}
#endif