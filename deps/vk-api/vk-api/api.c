#include <curl/curl.h>
#include <util/bmem.h>

#include "api.h"

#define BUFFER_SIZE (256 * 1024)  /* 256 KB */

struct response_data {
	char *data;
	size_t size;
};

static size_t write_response(void *input, size_t memb_size, size_t nmemb,
		void *userp)
{
	size_t inputsize = memb_size * nmemb;
	struct response_data *target = userp;

	if (target->size + inputsize >= BUFFER_SIZE)
		return 0;

	memcpy(target->data + target->size, input, inputsize);
	target->size += inputsize;

	return inputsize;
}

static char *do_httpget(const char *url)
{
	CURL *curl = NULL;
	CURLcode code;
	struct curl_tlssessioninfo *tlsinfo;
	long response_code;
	char errorstr[CURL_ERROR_SIZE];
	struct response_data response = { bmalloc(BUFFER_SIZE), 0 };

	curl = curl_easy_init();
	if (!curl)
		goto error;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorstr);

#if LIBCURL_VERSION_NUM >= 0x072400
	curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPN, 0);
#endif

	/* Certificate revocation checking produces constant connection errors
	   for some users */
	curl_easy_getinfo(curl, CURLINFO_TLS_SSL_PTR, &tlsinfo);
	if (tlsinfo->backend == CURLSSLBACKEND_SCHANNEL)
		curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS,
					CURLSSLOPT_NO_REVOKE);

	code = curl_easy_perform(curl);
	if (code != CURLE_OK) {
		blog(LOG_WARNING,
			"HTTP GET failed, CURL error %d (%s)", code, errorstr);
		goto error;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
	if (response_code >= 400)
		blog(LOG_WARNING,
			"HTTP GET failed, HTTP/%ld", response_code);

	response.data[response.size] = '\0';  /* zero-terminate the result */

	curl_easy_cleanup(curl);
	return response.data;
error:
	curl_easy_cleanup(curl);
	bfree(response.data);
	return NULL;
}

static char *params_to_query(size_t params_count, va_list params)
{
	struct parameter param;
	char *query;
	char *str_param;

	query = bstrdup("?");
	for (size_t i = 0; i < params_count; i++) {
		param = va_arg(params, struct parameter);
		str_param = param_to_string(param);

		query = brealloc(query, strlen(query)
			+ (i ? strlen("&") : 0) + strlen(str_param) + 1);

		strcat(query, i ? "&" : "");
		strcat(query, str_param);

		bfree(str_param);
	}

	return query;
}

static bool url_add_params(char (*url_p)[2048],
		enum calltype calltype, size_t params_count, va_list params)
{
	char *query;
	char *url = *url_p;
	const char *mandatory;

	if (!params_count)
		return true;

	query = params_to_query(params_count, params);

	mandatory = calltype == AUTH ? AUTH_MANDATORY_PRMS : API_MANDATORY_PRMS;
	query = brealloc(query, strlen(query) + strlen(mandatory) + 1);
	strcat(query, mandatory);

	if (strlen(url) + strlen(query) >= sizeof(*url_p)) {
		blog(LOG_WARNING, "Url size exceeds %z bytes: %s%s",
						sizeof(*url_p), url, query);
		goto error;
	}

	strcat(url, query);

	bfree(query);
	return true;
error:
	bfree(query);
	return false;
}

json_t *vk_call_auth(size_t params_count, ...)
{
	json_t *root;
	va_list params;
	bool url_constructed;
	char *response;
	char url[2048] = "https://oauth.vk.com/token";

	va_start(params, params_count);
	url_constructed = url_add_params(&url, AUTH, params_count, params);
	va_end(params);
	if (!url_constructed)
		return NULL;

	response = do_httpget(url);
	if (!response)
		return NULL;

	root = json_loads(response, 0, NULL);
	bfree(response);

	return root;
}

json_t *vk_call_api(const char *method, size_t params_count, ...)
{
	json_t *root;
	va_list params;
	bool url_constructed;
	char *response;
	char url[2048] = "https://api.vk.com/method";

	strcat(url, "/");
	strcat(url, method);
	
	va_start(params, params_count);
	url_constructed = url_add_params(&url, API, params_count, params);
	va_end(params);
	if (!url_constructed)
		return NULL;

	response = do_httpget(url);
	if (!response)
		return NULL;

	root = json_loads(response, 0, NULL);
	bfree(response);

	return root;
}