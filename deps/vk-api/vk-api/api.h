#pragma once

#include <jansson.h>

#include "params.h"

#define API_VER "5.68"

#define API_MANDATORY_PRMS "&v="API_VER
#define AUTH_MANDATORY_PRMS "&grant_type=password"\
				"&client_id=5554299"\
				"&client_secret=DEADBEEFDEADBEEFDEADBEEF"

enum calltype {
	AUTH,
	API
};

/**
* Calls vk.com API and returns the answer in a JSON format
*
* @note  Here is a guide to using the API: https://vk.com/dev/api_requests
*
* @param  method        Method to use. List of all the available methods:
*                       https://vk.com/dev/methods
* @param  params_count  Number of method parameters you will provide.
*                       After that you must provide this exact amount
*                       of "struct parameter" variables, separated by coma.
* @return               Reply in JSON format, or NULL on fail.
*                       Use json_decref on it after you're done.
*/
json_t *vk_call_api(const char *method, size_t params_count, ...);
/*
#define vk_call_method(method, params_count, ...)
	vk_call_api(method, params_count, ##__VA_ARGS__, \
				create_parameter("v", API_VER))
*/
json_t *vk_call_auth(size_t params_count, ...);