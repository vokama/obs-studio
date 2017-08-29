#pragma once

#include <jansson.h>

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

#ifdef __cplusplus
}
#endif