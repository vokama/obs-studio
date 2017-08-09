#include <curl/curl.h>
#include <util/bmem.h>

#include "params.h"

char *param_to_string(struct parameter param)
{
	CURL *curl;
	char *enc_name;
	char *enc_value;
	char *result;

	curl = curl_easy_init();

	enc_name = curl_easy_escape(curl, param.name, 0);
	enc_value = curl_easy_escape(curl, param.value, 0);

	result = bmalloc(strlen(enc_name) + strlen("=") + strlen(enc_value)
		+ 1);
	strcpy(result, enc_name);
	strcat(result, "=");
	strcat(result, enc_value);

	curl_free(enc_value);
	curl_free(enc_name);
	curl_easy_cleanup(curl);

	return result;
}

struct parameter create_param(const char *name, const char *value)
{
	struct parameter param = { name, value };
	return param;
}