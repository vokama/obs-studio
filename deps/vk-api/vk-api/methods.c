#include "api.h"

json_t *login_simple(const char *username, const char *password)
{
	return vk_call_auth(2, create_param("username", username),
			create_param("password", password));
}

json_t *login_captcha(const char *username, const char *password,
		const char *sid, const char *key)
{
	return vk_call_auth(4, create_param("username", username),
			create_param("password", password),
			create_param("captcha_sid", sid),
			create_param("captcha_key", key));
}

json_t *login_2fa(const char *username, const char *password, const char *code)
{
	return vk_call_auth(3, create_param("username", username),
			create_param("password", password),
			create_param("code", code));
}

void vk_record_visit(const char *access_token)
{
	json_t *root;
	root = vk_call_api("stats.trackVisitor", 1,
				create_param("access_token", access_token));
	json_decref(root);
}