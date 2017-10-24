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

json_t *vk_get_user(const char *access_token)
{
	return vk_call_api("users.get", 1,
				create_param("access_token", access_token));
}

json_t *vk_get_user_owned_groups(const char *access_token)
{
	return vk_call_api("groups.get", 3,
				create_param("access_token", access_token),
				create_param("extended", "1"),
				create_param("filter", "admin"));
}

json_t *vk_get_stream_categories(const char *access_token)
{
	return vk_call_api("video.liveGetCategories", 1,
				create_param("access_token", access_token));
}

json_t *vk_start_streaming(const char *access_token,
		const char *name, const char *group_id, const char *category_id)
{
	return vk_call_api("video.startStreaming", 5,
				create_param("access_token", access_token),
				create_param("name", name),
				create_param("group_id", group_id),
				create_param("wallpost", "1"),
				create_param("category_id", category_id));
}

void vk_stop_streaming(const char *access_token, const char *group_id)
{
	vk_call_api("video.stopStreaming", 2,
			create_param("access_token", access_token),
			create_param("group_id", group_id));
}