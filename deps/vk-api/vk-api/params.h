#pragma once

struct parameter {
	const char *name;
	const char *value;
};

struct parameter create_param(const char *name, const char *value);
char *param_to_string(struct parameter param);